/*
    =========== IDT ===========

    This file contains the code necessary to populate the IDT and load it into the IDTR

    ===========================
*/
#include "include/idt.h"

/*
list of all isrs
*/
extern void diverr(void);
extern void debugexc(void);
extern void nmi(void);
extern void breakpoint(void);
extern void overflow(void);

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
    idt[0] = create_idt_entry((uint32_t)(&diverr), 0x08, interrupt(0, false));
    idt[1] = create_idt_entry((uint32_t)(&debugexc), 0x08, interrupt(0, false));
    idt[2] = create_idt_entry((uint32_t)(&nmi), 0x08, interrupt(0, false));
    idt[3] = create_idt_entry((uint32_t)(&breakpoint), 0x18, interrupt(3, false));
    idt[4] = create_idt_entry((uint32_t)(&overflow), 0x18, interrupt(3, false));

    // load it
    // __asm__ volatile("lidt %0" : : "m" (*idtr));
}