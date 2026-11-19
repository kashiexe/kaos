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

    =========== kheap.hh ========

    Includes kmalloc and kfree functions, as well as some other utilities
    [kheap.h includes the C side as well as some structures and definitions]

    =============================
*/
#ifndef KHEAP_HH
#define KHEAP_HH

// std
#include<stdint.h>

// local
#include "kheap.h"
#include "pmm.hh"
#include "vmm.hh"

namespace kaos {
    namespace core {
        // core::heap != user::heap [core::heap is for kernel]
        namespace heap {
            void init();
            void* kmalloc(uint32_t size);
            void kfree(void* ptr);
            void* grow(uint32_t size);
        }
    }
}

#endif