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
*/
#ifndef PIC_H
#define PIC_H

// ports for the PIC
#define pic1_cmd 0x20
#define pic1_data 0x21
#define pic2_cmd 0xA0
#define pic2_data 0xA1

// initialization control words for the PIC
#define icw1_init 0x10
#define icw1_icw4 0x01
#define icw4_8086 0x01

// other variables
#define pic_eoi 0x20

void remap_pic();

#endif