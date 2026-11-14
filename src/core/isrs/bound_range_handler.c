/*
    =========== ISR : BOUND RANGE ===========

    Contains the code for the bound range interrupt handler (interrupt 0x05)

    =================================
*/
#include "../include/basic.h"
#include "../include/boot_info.h"

/**
 * handler for bound range interrupt (BOUND)
 */
void bound_range_handler() {
    __asm__ volatile("cli"); // disable interrupts

    volatile vesa_info_t* vesa = VESA_INFO;
    volatile uint32_t* screen = (volatile uint32_t*)vesa->addr;
    print_num(screen, vesa->pitch, 5);
}