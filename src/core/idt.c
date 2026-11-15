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

    =========== IDT ===========

    This file contains the code necessary to populate the IDT and load it into the IDTR

    ===========================
*/
#include "include/idt.h"

/*
list of all isrs
*/
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr32(void);
extern void isr33(void);
extern void isr34(void);
extern void isr35(void);
extern void isr36(void);
extern void isr37(void);
extern void isr38(void);
extern void isr39(void);
extern void isr40(void);
extern void isr41(void);
extern void isr42(void);
extern void isr43(void);
extern void isr44(void);
extern void isr45(void);
extern void isr46(void);
extern void isr47(void);

/**
 * helper to create an IDT entry with the given parameters
 */
uint64_t create_idt_entry(uint32_t offset, uint16_t selector, uint8_t type) {
    uint64_t entry = 0;

    entry |= (uint64_t)(offset & 0xFFFF);                       // offset low
    entry |= (uint64_t)(selector) << 16;                        // selector
    entry |= (uint64_t)(0) << 32;                               // unused
    entry |= (uint64_t)(type) << 40;                            // type
    entry |= (uint64_t)((offset >> 16) & 0xFFFF) << 48;         // offset high

    return entry;
}

/**
 * actually sets up the IDT by populating it and then loading it into the IDTR
 */
void setup_idt(uint64_t* idt, idtr_t* idtr) {
    // populate the IDT with the appropriate entries
    for(int i = 0; i < 256; i++) {
        idt[i] = create_idt_entry(0, 0, 0);
    }

    // change some manually
    idt[0] = create_idt_entry((uint32_t)(&isr0), 0x08, interrupt(0, false));
    idt[1] = create_idt_entry((uint32_t)(&isr1), 0x08, interrupt(0, false));
    idt[2] = create_idt_entry((uint32_t)(&isr2), 0x08, interrupt(0, false));
    idt[3] = create_idt_entry((uint32_t)(&isr3), 0x08, interrupt(3, false));
    idt[4] = create_idt_entry((uint32_t)(&isr4), 0x08, interrupt(3, false));
    idt[5] = create_idt_entry((uint32_t)(&isr5), 0x08, interrupt(0, false));
    idt[6] = create_idt_entry((uint32_t)(&isr6), 0x08, interrupt(0, false));
    idt[7] = create_idt_entry((uint32_t)(&isr7), 0x08, interrupt(0, false));
    idt[8] = create_idt_entry((uint32_t)(&isr8), 0x08, interrupt(0, false));
    idt[9] = create_idt_entry((uint32_t)(&isr9), 0x08, interrupt(0, false));
    idt[10] = create_idt_entry((uint32_t)(&isr10), 0x08, interrupt(0, false));
    idt[11] = create_idt_entry((uint32_t)(&isr11), 0x08, interrupt(0, false));
    idt[12] = create_idt_entry((uint32_t)(&isr12), 0x08, interrupt(0, false));
    idt[13] = create_idt_entry((uint32_t)(&isr13), 0x08, interrupt(0, false));
    idt[14] = create_idt_entry((uint32_t)(&isr14), 0x08, interrupt(0, false));
    idt[15] = create_idt_entry((uint32_t)(&isr15), 0x08, interrupt(0, false));
    idt[16] = create_idt_entry((uint32_t)(&isr16), 0x08, interrupt(0, false));
    idt[17] = create_idt_entry((uint32_t)(&isr17), 0x08, interrupt(0, false));
    idt[18] = create_idt_entry((uint32_t)(&isr18), 0x08, interrupt(0, false));
    idt[19] = create_idt_entry((uint32_t)(&isr19), 0x08, interrupt(0, false));
    idt[20] = create_idt_entry((uint32_t)(&isr20), 0x08, interrupt(0, false));
    idt[21] = create_idt_entry((uint32_t)(&isr21), 0x08, interrupt(0, false));
    idt[22] = create_idt_entry((uint32_t)(&isr22), 0x08, interrupt(0, false));
    idt[23] = create_idt_entry((uint32_t)(&isr23), 0x08, interrupt(0, false));
    idt[24] = create_idt_entry((uint32_t)(&isr24), 0x08, interrupt(0, false));
    idt[25] = create_idt_entry((uint32_t)(&isr25), 0x08, interrupt(0, false));
    idt[26] = create_idt_entry((uint32_t)(&isr26), 0x08, interrupt(0, false));
    idt[27] = create_idt_entry((uint32_t)(&isr27), 0x08, interrupt(0, false));
    idt[28] = create_idt_entry((uint32_t)(&isr28), 0x08, interrupt(0, false));
    idt[29] = create_idt_entry((uint32_t)(&isr29), 0x08, interrupt(0, false));
    idt[30] = create_idt_entry((uint32_t)(&isr30), 0x08, interrupt(0, false));
    idt[31] = create_idt_entry((uint32_t)(&isr31), 0x08, interrupt(0, false));
    idt[32] = create_idt_entry((uint32_t)(&isr32), 0x08, interrupt(0, false));
    idt[33] = create_idt_entry((uint32_t)(&isr33), 0x08, interrupt(0, false));
    idt[34] = create_idt_entry((uint32_t)(&isr34), 0x08, interrupt(0, false));
    idt[35] = create_idt_entry((uint32_t)(&isr35), 0x08, interrupt(0, false));
    idt[36] = create_idt_entry((uint32_t)(&isr36), 0x08, interrupt(0, false));
    idt[37] = create_idt_entry((uint32_t)(&isr37), 0x08, interrupt(0, false));
    idt[38] = create_idt_entry((uint32_t)(&isr38), 0x08, interrupt(0, false));
    idt[39] = create_idt_entry((uint32_t)(&isr39), 0x08, interrupt(0, false));
    idt[40] = create_idt_entry((uint32_t)(&isr40), 0x08, interrupt(0, false));
    idt[41] = create_idt_entry((uint32_t)(&isr41), 0x08, interrupt(0, false));
    idt[42] = create_idt_entry((uint32_t)(&isr42), 0x08, interrupt(0, false));
    idt[43] = create_idt_entry((uint32_t)(&isr43), 0x08, interrupt(0, false));
    idt[44] = create_idt_entry((uint32_t)(&isr44), 0x08, interrupt(0, false));
    idt[45] = create_idt_entry((uint32_t)(&isr45), 0x08, interrupt(0, false));
    idt[46] = create_idt_entry((uint32_t)(&isr46), 0x08, interrupt(0, false));
    idt[47] = create_idt_entry((uint32_t)(&isr47), 0x08, interrupt(0, false));

    // load it
    __asm__ volatile("lidt %0" : : "m" (*idtr));

    // enable interrupts
    __asm__ volatile("sti");
}