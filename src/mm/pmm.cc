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

    =========== PMM ===========

    PMM serves as the manager for KAOS' physical memory.
    It is responsible for calculating, allocating, and freeing physical memory pages.
    It basically creates a bitmap using E820's mmap gotten from the bootloader, which is then used to manage PHYSICAL pages.
    Each page is 4KB in size, which is the standard.

    ===========================
*/
#include "include/pmm.hh"

using namespace kaos;

namespace kaos::pmm {
    // address to the bitmap
    static uint32_t* bitmap = nullptr; 
    static uint32_t pages = 0;
    static uint32_t used_pages = 0;
    static uint32_t chunks = 0;
};

/**
 * initializes the physical memory manager
 * this function is called by the kernel's main function
 */
void pmm::init() {
    // snatch memory map and size from bootloader
    uint16_t* size = (uint16_t*)MMAP_ENTRY_AMT;
    mmap_entry_t* mmap = (mmap_entry_t*)MMAP_TABLE;

    // find highest address
    uint64_t highest_addr = 0;

    for(uint16_t i = 0; i < *size; i++) {
        const auto* entry = mmap_at(i);

        if(entry->type == USABLE) {
            uint64_t end_addr = entry->base_addr + entry->length;
            if(end_addr > highest_addr) {
                if(end_addr <= MAX_MEM) highest_addr = end_addr;
                else highest_addr = MAX_MEM; // cap at MAX_MEM (4GB)
            }
        }
    }

    // calculate bitmap size
    pages = highest_addr / PAGE_SIZE;
    uint32_t bitmap_size = (pages + 7) / 8;         // round up to nearest byte
    chunks = (bitmap_size + 3) / 4;        // round up to nearest uint32_t

    // set bitmap address to end of the kernel
    uintptr_t bitmap_addr = (uintptr_t)KERNEL_END;
    // align 4-byte
    bitmap_addr = (bitmap_addr + 3) & ~3;
    bitmap = reinterpret_cast<uint32_t*>(bitmap_addr);

    // mark everything as used for now
    for(uint32_t i = 0; i < chunks; i++) {
        bitmap[i] = 0xFFFFFFFF;
    }

    used_pages = pages;

    // go through mmap again but free the safe usable regions
    for(uint16_t i = 0; i < *size; i++) {
        const auto* entry = mmap_at(i);

        if(entry->type == USABLE) {
            const uint64_t start_addr = entry->base_addr;
            const uint64_t end_addr = entry->base_addr + entry->length;

            // calculate page indices
            uint32_t start_page = start_addr / PAGE_SIZE;
            uint32_t end_page = end_addr / PAGE_SIZE;

            // free pages in the range
            for(uint32_t page = start_page; page < end_page; page++) {
                free_page(reinterpret_cast<void*>(page * PAGE_SIZE));
            }
        }
    }

    // re-reserve known used regions (0 to end of bitmap are known reserved)
    uint32_t reserved_pages = (bitmap_addr + bitmap_size + PAGE_SIZE - 1) / PAGE_SIZE;
    for(uint32_t page = 0; page < reserved_pages; page++) {
        uint32_t page_index = page;
        bitmap[page_index / 32] |= (1 << (page_index % 32));
    }
}

/**
 * allocates a single page of physical memory
 */
void* pmm::alloc_page() {
    for(uint32_t i = 0; i < chunks; i++) {
        // if the chunk is full, skip it
        if(bitmap[i] == 0xFFFFFFFF) continue;

        // get index of the first free page in this chunk
        for(uint32_t j = 0; j < 32; j++) {
            if(!(bitmap[i] & (1 << j))) {
                uint32_t page_index = i * 32 + j;
                if(page_index >= pages) return nullptr; // out of bounds

                // mark the page as used
                bitmap[i] |= (1 << j);
                used_pages++;
                return reinterpret_cast<void*>(page_index * PAGE_SIZE);
            }
        }
    }

    // no free pages available
    return nullptr;
}

/**
 * frees a single page of physical memory
 */
void pmm::free_page(void* ptr) {
    uint32_t addr = reinterpret_cast<uint32_t>(ptr);
    if(addr & 0xFFF) return; // not aligned to 4kB
    uint32_t page_index = addr / PAGE_SIZE;
    if(page_index < pages) {
        // clear the bit in the bitmap
        bitmap[page_index / 32] &= ~(1 << (page_index % 32));
        used_pages--;
    }
}

/* Utilities */
uint64_t pmm::get_total_memory() {
    return static_cast<uint64_t>(pages) * PAGE_SIZE;
}

uint64_t pmm::get_used_memory() {
    return static_cast<uint64_t>(used_pages) * PAGE_SIZE;
}

/**
 * helper to set a memory region to a specific value
 */
void pmm::memset(void* ptr, uint8_t value, size_t size) {
    // check if size can be divided into full chunks, and if so, zero out in chunks until the modulo
    uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(ptr);
    size_t full_chunks = size / sizeof(uint32_t);
    size_t remaining_bytes = size % sizeof(uint32_t);

    // zero out full chunks (easier than byte by byte)
    auto* word_ptr = reinterpret_cast<uint32_t*>(ptr);
    const auto chunk = static_cast<uint32_t>(value) * 0x01010101U;

    for(size_t i = 0; i < full_chunks; i++) {
        word_ptr[i] = chunk;
    }

    // zero out remaining bytes
    for(size_t i = 0; i < remaining_bytes; i++) {
        byte_ptr[full_chunks * sizeof(uint32_t) + i] = value;
    }
}

/**
 * these are just the original functions' wrappers for the C side of the kernel to use
 */
extern "C" {
    void pmm_init() { pmm::init(); }
    void* pmm_alloc() { return pmm::alloc_page(); }
    void pmm_free(void* ptr) { pmm::free_page(ptr); }
    uint64_t pmm_get_total_memory() { return pmm::get_total_memory(); }
    uint64_t pmm_get_used_memory() { return pmm::get_used_memory(); }
}