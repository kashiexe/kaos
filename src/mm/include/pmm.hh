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

    =========== pmm.hh ===========

    Includes the definition of all C++ functions, structures, etc used and declared by the PMM.
    This should be used only in the kernel, obviously.
    [if used in the C side of the kernel, please include pmm.h instead]

    =============================
*/
#ifndef PMM_HH
#define PMM_HH

#include<stdint.h>
#include<stddef.h>
#include "../../core/include/boot_info.h"
#include "pmm.h"

// 4kB which is standard for KAOS
constexpr size_t PAGE_SIZE = 4096;

/**
 * actual functions
 */
namespace kaos {
    namespace pmm {
        // main functions
        void init();
        void* alloc_page();
        void free_page(void* ptr);

        // utility functions
        uint64_t get_total_memory();
        uint64_t get_used_memory();
        void memset(void* ptr, uint8_t value, size_t size);
    }
}

#endif