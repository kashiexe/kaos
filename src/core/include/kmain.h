#ifndef KMAIN
#define KMAIN

// std includes
#include<stdint.h>

// local includes
#include "gdt.h"

/* All information gotten from bootloader */

// vesa information gotten from bootloader
typedef struct vesa_info {
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint32_t addr;
} __attribute__((packed)) vesa_info_t;

// memory map

typedef enum mmap_type {
    USABLE = 1,
    RESERVED = 2,
    ACPI_TABLES = 3,
    ACPI_NVS = 4,
    BAD_MEMORY = 5
} mmap_type_t;

typedef struct mmap_entry {
    uint64_t base_addr;
    uint64_t length;
    mmap_type_t type;
    uint32_t flags;
} __attribute__((packed)) mmap_entry_t;

// EDID
typedef struct edid {
    uint8_t header[8];
    uint16_t manufacturer_id;
    uint16_t product_code;
    uint32_t serial_number;
    uint8_t week_of_manufacture;
    uint8_t year_of_manufacture;
    uint8_t version;
    uint8_t revision;
    uint8_t video_input_type;               // digital vs analog, bit depth, sync type, etc
    uint8_t horizontal_size_cm;
    uint8_t vertical_size_cm;
    uint8_t display_gamma;                  // gamma = (display_gamma * 100) + 100
    uint8_t feature_support;                // DPMS, standby, suspend, active off, RGB color, preferred timing mode, GTF support
    uint8_t color_characteristics[10];      // point chromaticity coordinates
    uint8_t established_timings[3];         // bitmask for VGA modes
    uint16_t standard_timings[8];           // up to 8 standard resolution/refresh rate pairs
    uint8_t dtds[4][18];                    // 4 detailed timing descriptors
    uint8_t extension_flag;
} __attribute__((packed)) edid_t;

// known address pointers to specific structs (info from bootloader)

// BOOT drive
#define BOOT_DRIVE ((uint8_t*)0x0500) // boot drive pointer

// VESA information pointer
#define VESA_INFO ((vesa_info_t*)0x0510)

// mmap table pointer
#define MMAP_ENTRY_AMT ((uint32_t*)0x0540)
#define MMAP_ENTRY_SIZE ((uint32_t*)0x0550)
#define MMAP_TABLE ((mmap_entry_t*)0x0560)

// EDID pointer
#define EDID ((edid_t*)0x6900)

#endif