/*
    =========== ISR : DIVERR ===========

    Contains the code for the division error interrupt handler (interrupt 0x00)

    ====================================
*/
#include "../include/boot_info.h"
#include "../include/basic.h"

void diverr_handler() {
    __asm__ volatile("cli"); // disable interrupts

    volatile vesa_info_t* vesa = VESA_INFO;
    volatile uint32_t* screen = (volatile uint32_t*)vesa->addr;
    print_num(screen, vesa->pitch, 0);
}