/*
    Copyright 2026 Kashi

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    =========== Kernel Heap ===========

    This file contains the implementation of the kernel heap
    It provides functions like kmalloc and kfree from 0xC0000000 to 0xC4000000 (64MB) [minimum]
    The allocator uses a simple first-fit algorithm and coalesces free blocks to reduce fragmentation

    ===================================
*/
#include "include/kheap.hh"

using namespace kaos;

static kheap_block_t* heap_head = nullptr;
static kheap_block_t* heap_tail = nullptr;
static uint32_t heap_size = 0x4000000; // 64 MB for now
static uint32_t heap_end = KHEAP_START + heap_size;

/**
 * initializes the kernel heap, allocating a page and setting up the initial free block at 0xC0000000
 * (64MB reserved for the kernel heap)
 */
void core::heap::init() {
    // allocate a page for the heap
    auto page = reinterpret_cast<uint32_t>(pmm::alloc_page());
    vmm::map(KHEAP_START, page, PTE_P | PTE_W);

    // create free block to cover this page
    auto* initial = (kheap_block_t*)KHEAP_START;
    initial->size = PAGE_SIZE;
    initial->next = nullptr;
    initial->prev = nullptr;
    initial->free = true;

    // set heap head to this block
    heap_head = initial;
}

/**
 * allocates a block of memory of the given size from the kernel heap
 * @note current implementation of the kernel uses a simple first-fit algorithm for allocation
 * @returns a `void*` pointer to the allocated memory or `nullptr` if allocation fails
 */
void* core::heap::kmalloc(uint32_t size) {
    // check memory alignment and adjust size to include header
    auto need = size + sizeof(kheap_block_t);
    need = (need + 15) & ~15;

    // walk through the heap to find a free block that fits
    kheap_block_t* current = heap_head;
    while(current) {
        if(current->free && current->size >= need) {
            // found a block
            // check if we can split the block
            if(current->size >= need + sizeof(kheap_block_t) + 16) {
                // split block
                kheap_block_t* new_block = (kheap_block_t*)((uint8_t*)current + need);
                new_block->size = current->size - need;
                new_block->next = current->next;
                new_block->prev = current;
                new_block->free = true;

                // update current block
                if(current->next) {
                    current->next->prev = new_block;
                }
                current->next = new_block;
                current->size = need;
            }
            // if we can't split, just mark the block as used
            // (split or no split it's gonna be marked as used anyway)
            current->free = false;
            return (void*)((uint8_t*)current + sizeof(kheap_block_t));
        }

        // move to the next block
        current = current->next;
    }

    // no block found so we grow the heap
    return grow(need);
}

/**
 * frees a block of memory 
 * @note this function will coalesce blocks from both sides to reduce fragmentation
 * @note you should use this function only to free ALLOCATED KHEAP BLOCKS, NOT user memory or any other type
 * @param ptr pointer to the memory block to free
 */
void core::heap::kfree(void* ptr) {
    if(!ptr) return;

    // get block header
    kheap_block_t* block = ((kheap_block_t*)ptr) - 1;
    block->free = true;

    // coalesce with next block if it's free
    if(block->next && block->next->free) {
        // merge with next block
        block->size += block->next->size;
        block->next = block->next->next;

        // update next block's prev pointer (if possible)
        if(block->next) {
            block->next->prev = block;
        }
    }

    // coalesce with previous block if it's free
    if(block->prev && block->prev->free) {
        // merge with previous block
        block->prev->size += block->size;
        block->prev->next = block->next;

        // update next block's prev pointer (if possible)
        if(block->next) {
            block->next->prev = block->prev;
        }
    }
}

/**
 * grows the kernel heap and returns a pointer to the newly allocated memory
 * @param size the size of the memory to allocate
 * @return a pointer to the newly allocated memory, or NULL if allocation fails
 */
void* core::heap::grow(uint32_t size) {
    // check how many pages are needed
    uint32_t pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;

    // map the pages to the heap
    uint32_t start = heap_end;
    for(uint32_t i = 0; i < pages_needed; i++) {
        uint32_t page = reinterpret_cast<uint32_t>(pmm::alloc_page());
        vmm::map(start + (i * PAGE_SIZE), page, PTE_P | PTE_W);
    }

    // create a new free block for the new pages
    kheap_block_t* new_block = (kheap_block_t*)heap_end;
    new_block->size = pages_needed * PAGE_SIZE;
    new_block->next = nullptr;
    new_block->prev = heap_tail;
    new_block->free = true;

    // link the new block to the previous block
    if(heap_tail) {
        heap_tail->next = new_block;
    } else {
        // if there was no tail, this is the first block
        heap_head = new_block;
    }
    heap_tail = new_block;

    // coalesce with previous tail if it's free
    if(new_block->prev && new_block->prev->free) {
        new_block->prev->size += new_block->size;
        new_block->prev->next = nullptr;
        heap_tail = new_block->prev;
        new_block = new_block->prev;
    }

    // update heap_end
    heap_end += pages_needed * PAGE_SIZE;

    // retry kmalloc
    return kmalloc(size - sizeof(kheap_block_t));
}

// C interface
extern "C" {
    void kheap_init() { core::heap::init(); }
    void* kmalloc(uint32_t size) { return core::heap::kmalloc(size); }
    void kfree(void* ptr) { core::heap::kfree(ptr); }
    void* kheap_grow(uint32_t size) { return core::heap::grow(size); }
}