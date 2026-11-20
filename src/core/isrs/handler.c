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

    =========== ISR : Common Handler ===========

    Contains the code for the common interrupt handler.

    ============================================
*/
#include "../include/basic.h"
#include "../include/boot_info.h"
#include "../include/idt.h"
#include "../include/pic.h"

extern void outb(uint16_t port, uint8_t value);

void isr_handler(interrupt_frame_t* frame) {
    const uint8_t vector = frame->vector;
    volatile vesa_info_t* vesa = VESA_INFO;
    volatile uint32_t* screen = (volatile uint32_t*)vesa->addr;

    switch(vector) {
        case 0: {
            // zero division exception
            print_num(screen, vesa->pitch, frame->vector);
            break;    
        }

        case 0x08: {
            // #DF
            

            // halt
            while(1) {
                __asm__ volatile("hlt");
            }
        }

        case 0x0D: {
            // #GP
            print_num(screen, vesa->pitch, frame->vector);
            break;
        }

        case 0x0E: {
            // #PF
            print_num(screen, vesa->pitch, frame->vector);
            break;
        }

        default: {
            // pit
            if(vector == 0x20) {
                
            }

            // kb
            if(vector == 0x21) {
                // read scancode
                uint8_t scancode = 0;
                asm volatile("inb %1, %0" : "=a"(scancode) : "Nd"(0x60));
                print_num(screen, vesa->pitch, scancode);
            }

            if(vector >= 32 && vector <= 47) {
                if(vector >= 40) {
                    // send slave PIC EOI
                    outb(pic2_cmd, pic_eoi);
                }

                // master PIC EOI
                outb(pic1_cmd, pic_eoi);
            }
        }
    }
}