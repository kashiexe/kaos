#ifndef KGDT
#define KGDT

#include<stdint.h>

// flags
#define seg_desctype(x) ((x) << 0x04)           // descriptor type (0 for system, 1 for code/data)
#define seg_present(x) ((x) << 0x07)            // present
#define seg_savl(x) ((x) << 0x0C)               // available for system use
#define seg_long(x) ((x) << 0x0D)               // long mode
#define seg_size(x) ((x) << 0x0E)               // size (0 for 16-bit, 1 for 32)
#define seg_gran(x) ((x) << 0x0F)               // granularity (0 for 1B-1MB, 1 for 4KB-4GB)
#define seg_priv(x) (((x) & 0x03) << 0x05)      // set privilege level (0-3)

// rwxc...
#define seg_data_rd 0x00            // read-only
#define seg_data_rda 0x01           // read-only, accessed
#define seg_data_rdwr 0x02          // read / write
#define seg_data_rdwra 0x03         // read / write, accessed
#define seg_data_rdexpd 0x04        // read-only, expand-down
#define seg_data_rdexpda 0x05       // read-only, expand-down, accessed
#define seg_data_rdwrexpd 0x06      // read / write, expand-down
#define seg_data_rdwrexpda 0x07     // read / write, expand-down, accessed
#define seg_code_ex 0x08            // execute-only
#define seg_code_exa 0x09           // execute-only, accessed
#define seg_code_exrd 0x0A          // execute / read
#define seg_code_exrda 0x0B         // execute / read, accessed
#define seg_code_exc 0x0C           // execute-only, conforming
#define seg_code_exca 0x0D          // execute-only, conforming, accessed
#define seg_code_exrdc 0x0E         // execute / read, conforming
#define seg_code_exrdca 0x0F        // execute / read, conforming, accessed

// utilities

uint64_t create_gdt_entry(const uint32_t base, const uint32_t limit, const uint32_t flags);

/**
 * TSS struct
 */
typedef struct tss_entry {
    uint32_t prev_tss;
    uint32_t esp0, ss0; // pretty much the only ones we care about since the rest is for hardware task switching and we use software task switching
    uint32_t esp1, ss1;
    uint32_t esp2, ss2;
    uint32_t cr3;
    uint32_t eip, eflags;
    uint32_t eax, ecx, edx, ebx;
    uint32_t esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap, iomap_base;
} __attribute__((packed)) tss_entry_t;

/**
 * contains the structure of a GDT descriptor
 */
struct gdt_descriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void setup_gdt(uint64_t* gdt, tss_entry_t* tss);

#endif