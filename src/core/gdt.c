#include "include/gdt.h"

/**
 * utility to help create a GDT entry struct with the given parameters
 * ```c
 * uint64_t entry = create_gdt_entry(base, limit, flags);
 * ```
 */
uint64_t create_gdt_entry(const uint32_t base, const uint32_t limit, const uint32_t flags) {
    uint64_t entry = 0;

    entry = limit & 0x000F0000;             // limit bits 19:16
    entry |= (flags << 8) & 0x00F0FF00;     // type, p, dpl, s, g, d/b, l and avl
    entry |= (base >> 16) & 0x000000FF;     // base bits 23:16
    entry |= base & 0xFF000000;             // base bits 31:24

    // shift 32
    entry <<= 32;

    // low 32 bit segment
    entry |= base << 16;                    // base bits 15:0
    entry |= limit & 0x000000FF;            // limit bits 15:0

    return entry;
}

void write_tss(int i, uint64_t* gdt, uint32_t base, uint32_t limit) {
    uint64_t descriptor = 0;

    descriptor |= (uint64_t)(limit&0xFFFF);             // limit low
    descriptor |= (uint64_t)(base&0xFFFFFF) << 16;      // base low
    descriptor |= (uint64_t)(0x89) << 40;               // access byte
    descriptor |= (uint64_t)((limit>>16)&0xF) << 48;    // limit high
    descriptor |= (uint64_t)((base>>24)&0xFF) << 56;    // base high

    gdt[i] = descriptor;
}


/**
 * function used to help setup the GDT with all entries necessary and automatically load it into the GDTR
 * ```c
 * // simply use it by calling it with a pointer to an array of (6) uint64_ts
 * uint64_t gdt[6];
 * setup_gdt(gdt);
 * ```
 */
void setup_gdt(uint64_t* gdt, tss_entry_t* tss) {
    gdt[0] = create_gdt_entry(0, 0, 0); // null entry

    // kernel code and data segments
    // code
    gdt[1] = create_gdt_entry(0, 0xFFFFF, seg_desctype(1) | seg_present(1) | seg_savl(0) | seg_long(0) | seg_size(1) | seg_gran(1) | seg_priv(0) | seg_code_exrd);
    
    // data
    gdt[2] = create_gdt_entry(0, 0xFFFFF, seg_desctype(1) | seg_present(1) | seg_savl(0) | seg_long(0) | seg_size(1) | seg_gran(1) | seg_priv(0) | seg_data_rdwr);
    
    // user code and data segments
    // code
    gdt[3] = create_gdt_entry(0, 0xFFFFF, seg_desctype(1) | seg_present(1) | seg_savl(0) | seg_long(0) | seg_size(1) | seg_gran(1) | seg_priv(3) | seg_code_exrd);

    // data
    gdt[4] = create_gdt_entry(0, 0xFFFFF, seg_desctype(1) | seg_present(1) | seg_savl(0) | seg_long(0) | seg_size(1) | seg_gran(1) | seg_priv(3) | seg_data_rdwr);

    // TSS
    write_tss(5, gdt, (uint32_t)tss, sizeof(tss_entry_t) - 1);

    // create GDTR
    struct gdt_descriptor gdtr = {
        .limit = (6 * sizeof(uint64_t)) - 1,
        .base = (uint32_t)gdt
    };

    // load and flush segments
    __asm__ volatile(
        "lgdt (%0)\n"
        "mov $0x10, %%ax\n" // load the data segment selector (0x10) into ax
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "ljmp $0x08, $.next\n" // load the code segment selector
        ".next:\n"
        : : "r"(&gdtr) : "ax", "memory"
    );
}