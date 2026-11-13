#ifndef KMAIN
#define KMAIN

// std includes
#include<stdint.h>

// local includes
#include "gdt.h"

/* All information gotten from bootloader */
typedef struct vesa_info {
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint32_t addr;
} __attribute__((packed)) vesa_info_t;

// known address (physical 0x0510)
#define VESA_INFO ((vesa_info_t*)0x0510)

#endif