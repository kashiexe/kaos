#ifndef KBASIC
#define KBASIC

#include<stdint.h>

/**
 * extremely basic helper to print a number to the screen without a proper printf implementation or font
 */
void print_num(volatile unsigned int* screen, const uint16_t pitch, int num);

#endif