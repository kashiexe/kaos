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

    =========== vmm.h ===========

    Exposed C functions for the VMM to be used in the kernel's C code.

    =============================
*/
#ifndef VMM_H
#define VMM_H

#include<stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    typedef enum pde_flags {
        P = 0x1,            // present
        W = 0x2,            // writable
        U = 0x4,            // user/kernel
        PWT = 0x8,          // write through
        PCD = 0x10,         // cache disabled
        A = 0x20,           // accessed
        D = 0x40,           // dirty
        PS = 0x80,          // page size
        G = 0x100           // global
    } pde_flags_t;

    typedef enum pte_flags {
        PTE_P = 0x1,        // present
        PTE_W = 0x2,        // writable
        PTE_U = 0x4,        // user/kernel
        PTE_PWT = 0x8,      // write through
        PTE_PCD = 0x10,     // cache disabled
        PTE_A = 0x20,       // accessed
        PTE_D = 0x40,       // dirty
        PTE_PAT = 0x80,     // page attribute table
        PTE_G = 0x100       // global
    } pte_flags_t;

    // methods
    void vmm_init();
    void vmm_identity_map(uint32_t start, uint32_t end, uint32_t flags);
    void vmm_map(uint32_t vaddr, uint32_t paddr, uint32_t flags);
    void vmm_map4m(uint32_t vaddr, uint32_t paddr, uint32_t flags);
    void vmm_unmap(uint32_t vaddr);

    // utilities
    uint32_t vmm_get_paddr(uint32_t vaddr);
    uint32_t* vmm_get_page_directory();
    void vmm_set_page_directory(uint32_t pd);
    void vmm_enable_paging();
    void vmm_flush_tlb(uint32_t vaddr);
#ifdef __cplusplus
}
#endif

#endif