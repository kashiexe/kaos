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

    =========== pit.hh ===========

    Contains the C++ PIT driver/handler

    ==============================
*/
#ifndef PIT_HH
#define PIT_HH

#include "pit.h"

namespace kaos {
    namespace drivers {
        /*
            drivers::pit != drivers::chrono 

            what's the difference?
            > drivers::pit is for the PIT alone
            > drivers::chrono is more sophisticated and uses more methods to measure all types of time requests
        */
        namespace pit {
            unsigned char make_cmd(uint8_t channel, uint8_t rw_access, uint8_t mode, uint8_t bcd);
            void init();
            void set_frequency(uint32_t freq = PIT_FREQ);
            uint32_t get_frequency();
        }
    }
}

#endif