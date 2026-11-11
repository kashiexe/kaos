#ifndef KMAIN
#define KMAIN

#include<stdint.h>

/* All information gotten from bootloader */
typedef struct vesa_info {
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint32_t addr;
} __attribute__((packed)) vesa_info_t;

// known address (physical 0x0510)
#define VESA_INFO ((vesa_info_t*)0x0510)


/* GDT descriptor */
struct gdt_descriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#endif