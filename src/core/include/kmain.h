#ifndef KMAIN
#define KMAIN

#include<stdint.h>

/* All information gotten from bootloader */
struct BootInfo {
    uint32_t fb_addr;
    uint32_t fb_width;
    uint32_t fb_height;
    uint32_t fb_pitch;
    uint32_t fb_bpp;
} __attribute__((packed));


/* GDT descriptor */
struct gdt_descriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#endif