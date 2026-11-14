/*
    =========== ISR : NMI ===========

    Contains the code for the nonmaskable interrupt handler (interrupt 0x02)

    =================================
*/
#include "../include/basic.h"
#include "../include/boot_info.h"

/**
 * handler for nonmaskable interrupt (NMI)
 */
void nmi_handler() {
    __asm__ volatile("cli"); // disable interrupts

    volatile vesa_info_t* vesa = VESA_INFO;
    volatile uint32_t* screen = (volatile uint32_t*)vesa->addr;
    print_num(screen, vesa->pitch, 2);
}