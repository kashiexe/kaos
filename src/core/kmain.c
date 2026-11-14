/*
    =========== Kernel Main Function ===========

    This is the main function of the kernel. 
    - It coordinates the essential setups
    - It sets up all necessary components
    - Initializes the kernel's main loop (essential for services and tasks)
    among other things.

    ============================================
*/
#include "include/kmain.h"

// (certain functions are assembly functions from libc/src) from rand.s
extern uint32_t rand(uint32_t max, _Bool cpuid_available); 

/**
 * for bootup screen
 */
typedef struct star {
    int x;
    int y;
    int size;
    _Bool going_up;
} star_t;

void kmain() {
    // setup GDT and tss
    uint64_t gdt[6];
    tss_entry_t tss = {};
    setup_gdt(gdt, &tss);

    // remap PIC
    remap_pic();

    // setup IDT
    uint64_t idt[256];
    idtr_t idtr = {sizeof(idt) - 1, (uint32_t)(&idt)};
    setup_idt(idt, &idtr);

    // draw line
    vesa_info_t* vesa_info = VESA_INFO;
    volatile unsigned int* screen = (volatile unsigned int*)vesa_info->addr;

    // all stars on the screen
    const int amt = 70;
    star_t stars[amt];
    _Bool cpuid_available = *CPUID_AVAILABLE;

    for(int i = 0; i < amt; i++) {
        // get the random position and size of the star
        int rand_x = rand(vesa_info->width, cpuid_available);
        int rand_y = rand(vesa_info->height, cpuid_available);
        int rand_size = rand(3, cpuid_available) + 1;
        int rand_opacity = rand(255, cpuid_available) + 1;

        // draw the star
        for(int y = 0; y < rand_size; y++) {
            for(int x = 0; x < rand_size; x++) {
                if(rand_x + x < vesa_info->width && rand_y + y < vesa_info->height) {
                    const int index = (rand_y + y) * (vesa_info->pitch / 4) + (rand_x + x);
                    int color = rand_opacity << 24 | 0xFFFFFF;
                    screen[index] = color;
                }
            }
        }

        // save the star's position and size
        stars[i].size = rand_size;
        stars[i].x = rand_x;
        stars[i].y = rand_y;
        stars[i].going_up = rand(2, cpuid_available) == 0 ? true : false;
    }

    // move stars
    unsigned int tick = 0;
    while(1) {
        tick++;

        // new direction every 3 frames
        if(tick % 30000000 == 0) {
            for(int i = 0; i < amt; i++) {
                // move up or down (up if rand is even)
                if(rand(10, cpuid_available) % 2 == 0) {
                    if(stars[i].y > 0) stars[i].going_up = rand(2, cpuid_available) == 0 ? true : false;
                } else {
                    if(stars[i].y < vesa_info->height - 1) stars[i].going_up = rand(2, cpuid_available) == 0 ? true : false;
                }
            }
        }

        // 1 frame every 10 million ticks
        if(tick % 10000000 == 0) {
            for(int i = 0; i < amt; i++) {
                // erase the star
                for(int y = 0; y < stars[i].size; y++) {
                    for(int x = 0; x < stars[i].size; x++) {
                        if(stars[i].x + x < vesa_info->width && stars[i].y + y < vesa_info->height) {
                            const int index = (stars[i].y + y) * (vesa_info->pitch / 4) + (stars[i].x + x);
                            screen[index] = 0x000000;
                        }
                    }
                }

                // update position based on direction
                if(stars[i].going_up) {
                    if(stars[i].y > 0) stars[i].y--;
                    else {
                        stars[i].going_up = false;
                        if(stars[i].y < vesa_info->height - 1) stars[i].y++;
                    }
                } else {
                    if(stars[i].y < vesa_info->height - 1) stars[i].y++;
                    else {
                        stars[i].going_up = true;
                        if(stars[i].y > 0) stars[i].y--;
                    }
                }

                // redraw
                const int new_rand_opacity = rand(255, cpuid_available) + 1;

                for(int y = 0; y < stars[i].size; y++) {
                    for(int x = 0; x < stars[i].size; x++) {
                        if(stars[i].x + x < vesa_info->width && stars[i].y + y < vesa_info->height) {
                            const int index = (stars[i].y + y) * (vesa_info->pitch / 4) + (stars[i].x + x);
                            int color = new_rand_opacity << 24 | 0xFFFFFF;
                            screen[index] = color;
                        }
                    }
                }
            }
        }
    };
}