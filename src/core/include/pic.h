#ifndef PIC_H
#define PIC_H

// ports for the PIC
#define pic1_cmd 0x20
#define pic1_data 0x21
#define pic2_cmd 0xA0
#define pic2_data 0xA1

// initialization control words for the PIC
#define icw1_init 0x10
#define icw1_icw4 0x01
#define icw4_8086 0x01

void remap_pic();

#endif