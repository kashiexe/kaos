/*
    =========== Kernel Main Function ===========

    This is the main function of the kernel. 
    - It coordinates the essential setups
    - It sets up all necessary components
    - Initializes the kernel's main loop (essential for services and tasks)
    among other things.

    ============================================
*/
#include "include/kmain.h"

void kmain() {
    // setup GDT and tss
    uint64_t gdt[6];
    tss_entry_t tss = {};
    setup_gdt(gdt, &tss);

    // setup IDT
    uint64_t idt[256];
    setup_idt(idt);

    // draw line
    vesa_info_t* vesa_info = VESA_INFO;
    volatile unsigned int* screen = (volatile unsigned int*)vesa_info->addr;

    // green dot at 100, 100 for testing purposes
    int x = 100;
    int y = 100;
    unsigned int color = 0x5c9aff;

    // green line at y = 100 - 105 from x = 100 to x = 200
    for(int i = y; i < 105; i++) { 
        for (int j = x; j < 200; j++) {
            const int index = i * (vesa_info->pitch / 4) + j;
            screen[index] = color;
        }
    }

    while(1) {};
}