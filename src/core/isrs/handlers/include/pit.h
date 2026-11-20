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

    =========== pit.h ===========

    Contains the definitions for the C-wrapped PIT functions

    =============================
*/
#ifndef PIT_H
#define PIT_H

#include<stdint.h>

// global variables for (or about) the PIT

#define PIT_HZ 1193182  // the PIT's clock frequency
#define PIT_FREQ 100    // KAOS's default frequency for the PIT (in hz, default is 100hz)
#define PIT_CMD 0x43
#define PIT_CH0 0x40
#define PIT_CH1 0x41 // not used
#define PIT_CH2 0x42

#ifdef __cplusplus
extern "C" {
#endif
    unsigned char pit_make_cmd(uint8_t channel, uint8_t rw_access, uint8_t mode, uint8_t bcd);
    void pit_init();
    void pit_set_frequency(uint32_t freq);
    uint32_t pit_get_frequency();
#ifdef __cplusplus
}
#endif

#endif