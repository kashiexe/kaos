/*
    =========== ISR : BREAKPOINT ===========

    Contains the code for the breakpoint interrupt handler (interrupt 0x03)

    ========================================
*/
#include "../include/basic.h"
#include "../include/boot_info.h"

/**
 * handler for breakpoint interrupt (INT 0x03)
 */
void breakpoint_handler() {
    __asm__ volatile("cli"); // disable interrupts

    volatile vesa_info_t* vesa = VESA_INFO;
    volatile uint32_t* screen = (volatile uint32_t*)vesa->addr;
    print_num(screen, vesa->pitch, 3);
}