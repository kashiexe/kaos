#ifndef KIDT
#define KIDT

#include<stdint.h>

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

uint64_t create_idt_entry(uint32_t offset, uint16_t selector, uint8_t type);
void setup_idt(uint64_t* idt);

#endif