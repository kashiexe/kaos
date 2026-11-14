/*
    =========== BASIC ===========

    Contains basic temporary functions for the kernel

    =============================
*/
#include "include/basic.h"

void print_num(volatile unsigned int* screen, const uint16_t pitch, int num) {
    // bitmap array

    // 0
    const uint8_t char0[16] = {
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00000000,
        0b00000000,
    };

    // 1
    const uint8_t char1[16] = {
        0b00000000,
        0b00000000,
        0b00011000,
        0b00111000,
        0b01111000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b01111110,
        0b00000000,
        0b00000000,
    };

    // go through each digit in num and print it to the screen (white color as default)
    int digit;
    for(int i = 0; num > 0; i++) {
        digit = num % 10;
        num /= 10;

        // which character bitmap to use
        uint8_t* char_bitmap = 0;

        switch(digit) {
            case 0:
                char_bitmap = (uint8_t*)char0;
                break;
            case 1:
                char_bitmap = (uint8_t*)char1;
                break;
            default:
                char_bitmap = (uint8_t*)char0;
                break;
        }

        // print digit to screen
        for(int y = 0; y < 16; y++) {
            for(int x = 0 * i; x < 8 * i; x++) {
                if(char_bitmap[y] & (1 << (7 - x))) {
                    screen[(y * (pitch / 4)) + (x + (i * 8))] = 0xFFFFFF;
                }
            }
        }
    }
}