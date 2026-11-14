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
#ifndef KBASIC
#define KBASIC

#include<stdint.h>

/**
 * extremely basic helper to print a number to the screen without a proper printf implementation or font
 */
void print_num(volatile unsigned int* screen, const uint16_t pitch, int num);

#endif