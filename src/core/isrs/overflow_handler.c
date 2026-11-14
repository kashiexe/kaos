/*
    =========== ISR : OVERFLOW ===========

    Contains the code for the overflow interrupt handler (interrupt 0x04)

    =================================
*/
#include "../include/basic.h"
#include "../include/boot_info.h"

/**
 * handler for overflow interrupt (INT 0x04)
 */
void overflow_handler() {
    __asm__ volatile("cli"); // disable interrupts

    volatile vesa_info_t* vesa = VESA_INFO;
    volatile uint32_t* screen = (volatile uint32_t*)vesa->addr;
    print_num(screen, vesa->pitch, 4);
}