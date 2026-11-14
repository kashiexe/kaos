/*
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
    // save masks
    unsigned char a1 = inb(pic1_data);
    unsigned char a2 = inb(pic2_data);

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

    // restore saved masks
    outb(pic1_data, a1);
    outb(pic2_data, a2);
}