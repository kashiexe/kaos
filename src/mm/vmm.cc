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

    =========== VMM ===========

    VMM serves as the manager for KAOS' virtual memory.
    It is responsible for calculating, allocating, and freeing virtual memory.

    ===========================
*/
#include "include/vmm.hh"

using namespace kaos;

/**
 * initializes the VMM
 */
void vmm::init() {
    // create the kernel's page directory
    uint32_t kpd = (uint32_t)(pmm::alloc_page());

    // zero it
    pmm::memset(reinterpret_cast<void*>(kpd), 0, PAGE_SIZE);

    // set page directory as active
    set_page_directory(kpd);

    // identity map the first 4MB of memory
    identity_map(0x00000000, 0x00400000, P | W);
}

/**
 * identity maps a range of virtual addresses to physical addresses
 */
void vmm::identity_map(uint32_t start, uint32_t end, uint32_t flags) {
    // get the page directory
    auto* pd = get_page_directory();

    // loop through the range of addresses
    for(uint32_t addr = start; addr < end; addr += PAGE_SIZE) {
        // get the page directory index and page table index
        uint32_t pd_index = (addr >> 22) & 0x3FF;
        uint32_t pt_index = (addr >> 12) & 0x3FF;

        // check if the page table exists
        if(!(pd[pd_index] & 0x1)) {
            // allocate a new page table
            uint32_t pt = (uint32_t)(pmm::alloc_page());
            pmm::memset(reinterpret_cast<void*>(pt), 0, PAGE_SIZE);

            // set the page directory entry
            pd[pd_index] = pt | flags;
        }

        // set the page table entry
        reinterpret_cast<uint32_t*>(pd[pd_index] & ~0x3FF)[pt_index] = addr | flags;
    }
}

/**
 * maps a virtual address to a physical address with given flags
 */
void vmm::map(uint32_t vaddr, uint32_t paddr, uint32_t flags) {
    // extract pd and pt indices
    uint32_t pd_index = (vaddr >> 22) & 0x3FF; // first 10 bits
    uint32_t pt_index = (vaddr >> 12) & 0x3FF; // middle 10 bits

    // get the page directory and check if pde[pd_index] is present
    auto* physical_pd = get_page_directory();

    if(!(physical_pd[pd_index] & P)) {
        // allocate a new page table
        auto* pt = pmm::alloc_page();
        pmm::memset(pt, 0, PAGE_SIZE);

        // set the page directory entry with the new page table
        physical_pd[pd_index] = (uint32_t)pt | P | W; // present and writable
    }

    // get page table
    uint32_t* pt = reinterpret_cast<uint32_t*>(physical_pd[pd_index] & 0xFFFFF000);
    pt[pt_index] = (paddr & 0xFFFFF000) | flags; // set the page table entry

    // invalidate the TLB for this virtual address
    flush_tlb(vaddr);
}

void vmm::map4m(uint32_t vaddr, uint32_t paddr, uint32_t flags) {
    // extract pd index
    uint32_t pd_index = (vaddr >> 22) & 0x3FF; // first 10 bits
    auto* physical_pd = get_page_directory();

    // paddr must be aligned to 4MB
    physical_pd[pd_index] = (paddr & 0xFFC00000) | flags | PS; // set the page directory entry with 4MB page size
    flush_tlb(vaddr);
}

/**
 * unmaps a virtual address
 * basically removes the mapping from the page table and invalidates the TLB for that address
 */
void vmm::unmap(uint32_t vaddr) {
    // extract pd and pt indices
    uint32_t pd_index = (vaddr >> 22) & 0x3FF; // first 10 bits
    uint32_t pt_index = (vaddr >> 12) & 0x3FF; // middle 10 bits

    // get the page directory and check if pde[pd_index] is present
    auto* physical_pd = get_page_directory();

    if(!(physical_pd[pd_index] & P)) return; // page table not present, nothing to unmap

    // get page table
    uint32_t* pt = reinterpret_cast<uint32_t*>(physical_pd[pd_index] & 0xFFFFF000);
    pt[pt_index] = 0; // clear the page table entry

    // invalidate the TLB for this virtual address
    flush_tlb(vaddr);
}

/* --- UTILITIES --- */

/**
 * gets the physical address corresponding to a virtual address
 */
uint32_t vmm::get_paddr(uint32_t vaddr) {
    // extract pd and pt indices
    uint32_t pd_index = (vaddr >> 22) & 0x3FF; // first 10 bits
    uint32_t pt_index = (vaddr >> 12) & 0x3FF; // middle 10 bits

    // get the page directory and check if pde[pd_index] is present
    auto* physical_pd = get_page_directory();

    if(!(physical_pd[pd_index] & P)) return 0; // page table not present

    // get page table
    uint32_t* pt = reinterpret_cast<uint32_t*>(physical_pd[pd_index] & 0xFFFFF000);
    if(!(pt[pt_index] & P)) return 0; // page not present

    return pt[pt_index] & 0xFFFFF000; // return the physical address
}

/**
 * assembly wrapper over invlpg to invalidate the TLB for a specific virtual address
 */
void vmm::flush_tlb(uint32_t vaddr) {
    __asm__ volatile("invlpg (%0)" ::"r"(vaddr) : "memory");
}

/**
 * save page directory to CR3
 */
void vmm::set_page_directory(uint32_t pd) {
    // set the page directory by writing to CR3
    __asm__ volatile("mov %0, %%cr3" :: "r"(pd));
}

/**
 * gets the page directory from CR3
 */
uint32_t* vmm::get_page_directory() {
    uint32_t pd;
    __asm__ volatile("mov %%cr3, %0" : "=r"(pd));
    return reinterpret_cast<uint32_t*>(pd);
}

/**
 * simple utility to enable paging once ready
 */
void vmm::enable_paging() {
    // enable paging by setting CR0.PG (31st bit) to 1
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));      // get current cr0
    cr0 |= 0x80000000;                                  // 31st bit
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));
}

// C wrappers of the C++ functions
extern "C" {
    // main methods
    void vmm_init() { vmm::init(); }
    void vmm_identity_map(uint32_t start, uint32_t end, uint32_t flags) { vmm::identity_map(start, end, flags); }
    void vmm_map(uint32_t vaddr, uint32_t paddr, uint32_t flags) { vmm::map(vaddr, paddr, flags); }
    void vmm_map4m(uint32_t vaddr, uint32_t paddr, uint32_t flags) { vmm::map4m(vaddr, paddr, flags); }
    void vmm_unmap(uint32_t vaddr) { vmm::unmap(vaddr); }

    // utilities
    uint32_t vmm_get_paddr(uint32_t vaddr) { return vmm::get_paddr(vaddr); }
    uint32_t* vmm_get_page_directory() { return vmm::get_page_directory(); }
    void vmm_set_page_directory(uint32_t pd) { vmm::set_page_directory(pd); }
    void vmm_enable_paging() { vmm::enable_paging(); }
    void vmm_flush_tlb(uint32_t vaddr) { vmm::flush_tlb(vaddr); }
}