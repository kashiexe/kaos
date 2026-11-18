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

    =========== serial.h ===========

    Sets up and communicates with the serial port.

    ================================
*/
#ifndef SERIAL_H
#define SERIAL_H

#include<stdint.h>

#define SERIAL_PORT 0x3F8 // COM1

void serial_init();
int is_transmit_empty();
void serial_write(char a);
void serial_print(const char* str);
void serial_hex(uint32_t num);

#endif