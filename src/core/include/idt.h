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
#ifndef KIDT
#define KIDT

#include<stdint.h>
#include<stdbool.h>

/**
 * "p" field describes present, must be 1 to be valid
 * "dpl" field describes the privilege level, 0 is highest, 3 is lowest
 */
typedef enum gate_type {
    gate_task = 0x05,                   // type=0101
    gate_interrupt_16 = 0x06,           // type=0110
    gate_trap_16 = 0x07,                // type=0111
    gate_interrupt = 0x0E,              // type=1110
    gate_trap = 0x0F                    //type=1111
} gate_type_t;

/**
 * Creates an interrupt gate with the specified privilege level.
 * p=1 by default
 * dpl is provided (should be 0 for most of them [kernel])
 * if is_16bit is true, then the gate will use the 16-bit type instead
 */
static inline uint8_t interrupt(uint8_t dpl, _Bool is_16bit) {
    if (is_16bit) {
        return 0b10000000 | (dpl << 5) | gate_interrupt_16;
    }

    return 0b10000000 | (dpl << 5) | gate_interrupt;
}

/**
 * Creates a trap gate with the specified privilege level.
 * p=1 by default
 * dpl is provided (should be 0 for most of them [kernel])
 * if is_16bit is true, then the gate will use the 16-bit type instead
 */
static inline uint8_t trap(uint8_t dpl, _Bool is_16bit) {
    if (is_16bit) {
        return 0b10000000 | (dpl << 5) | gate_trap_16;
    }

    return 0b10000000 | (dpl << 5) | gate_trap;
}

/**
 * contains the structure of an entry for the IDT
 */
typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;          // segment selector in GDT
    uint8_t zero;               // unused
    uint8_t type;               // gate type, dpl and p fields
    uint16_t offset_high;
} __attribute__((packed)) idt_entry_t;

/**
 * contains the structure of an IDT descriptor
 */
typedef struct idtr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

/**
 * interrupt frame structure
 */
typedef struct interrupt_frame {
    // segment registers (for future ring 3 interrupts)
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    // pusha
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // pushed by the wrapper
    uint32_t vector;
    uint32_t error_code;

    // pushed by the CPU
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} interrupt_frame_t;

uint64_t create_idt_entry(uint32_t offset, uint16_t selector, uint8_t type);
void setup_idt(uint64_t* idt, idtr_t* idtr);

#endif