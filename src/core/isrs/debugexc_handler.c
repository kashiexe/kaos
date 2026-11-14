/*
    =========== ISR : DEBUG EXCEPTION ===========

    Contains the code for the debug exception interrupt handler (interrupt 0x01)

    =============================================
*/
#include "../include/basic.h"
#include "../include/boot_info.h"

void debugexc_handler() {
    __asm__ volatile("cli"); // disable interrupts

    volatile vesa_info_t* vesa = VESA_INFO;
    volatile uint32_t* screen = (volatile uint32_t*)vesa->addr;
    print_num(screen, vesa->pitch, 1);
}