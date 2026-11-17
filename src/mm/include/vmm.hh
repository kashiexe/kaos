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

    =========== vmm.hh ===========

    Includes the definition of all C++ functions, structures, etc used and declared by the VMM.
    This should be used only in the kernel, obviously.
    [if used in the C side of the kernel, please include vmm.h instead]

    =============================
*/
#ifndef VMM_HH
#define VMM_HH

// std
#include<stdint.h>

// local includes
#include "vmm.h"
#include "pmm.hh"

namespace kaos {
    namespace vmm {
        // methods
        void init();
        void identity_map(uint32_t start, uint32_t end, uint32_t flags);
        void map(uint32_t vaddr, uint32_t paddr, uint32_t flags);
        void map4m(uint32_t vaddr, uint32_t paddr, uint32_t flags);
        void unmap(uint32_t vaddr);

        // utilities
        uint32_t get_paddr(uint32_t vaddr);
        uint32_t* get_page_directory();
        void set_page_directory(uint32_t pd);
        void enable_paging();
        void flush_tlb(uint32_t vaddr);
    }
}

#endif