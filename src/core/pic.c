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

    =========== PIC ===========

    Remaps the PIC to avoid conflicts with CPU exceptions from the IDT

    ===========================
*/
#include "include/pic.h"

// some utilities from wrappers.s
extern unsigned char inb(unsigned short port);
extern void outb(unsigned short port, unsigned char data);
extern void io_wait();

/**
 * this function handles the PIC remapping
 */
void remap_pic() {
    // initialization sequence
    outb(pic1_cmd, icw1_init | icw1_icw4);
    io_wait();
    outb(pic2_cmd, icw1_init | icw1_icw4);
    io_wait();

    // set interrupt vector offset (32 for PIC1, 40 for PIC2)
    outb(pic1_data, 0x20);
    io_wait();
    outb(pic2_data, 0x28);
    io_wait();

    // set up cascading
    outb(pic1_data, 0x04);
    io_wait();
    outb(pic2_data, 0x02);
    io_wait();

    // set up 8086 mode
    outb(pic1_data, 0x01);
    io_wait();
    outb(pic2_data, 0x01);
    io_wait();

    // unmask all interrupts (restore saved masks)
    outb(pic1_data, 0x00);
    outb(pic2_data, 0x00);
}