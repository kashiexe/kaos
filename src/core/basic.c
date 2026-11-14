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

    // 2
    const uint8_t char2[16] = {
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00110000,
        0b01100000,
        0b01100110,
        0b01100110,
        0b01111110,
        0b00000000,
        0b00000000,
    };
    
    // 3
    const uint8_t char3[16] = {
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b00000110,
        0b00011100,
        0b00000110,
        0b00000110,
        0b00000110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00000000,
        0b00000000,
    };

    // 4
    const uint8_t char4[16] = {
        0b00000000,
        0b00000000,
        0b00001100,
        0b00011100,
        0b00111100,
        0b01101100,
        0b11001100,
        0b11111110,
        0b00001100,
        0b00001100,
        0b00001100,
        0b00001100,
        0b00001100,
        0b00000000,
        0b00000000,
    };

    // 5
    const uint8_t char5[16] = {
        0b00000000,
        0b00000000,
        0b01111110,
        0b01100000,
        0b01100000,
        0b01111100,
        0b00000110,
        0b00000110,
        0b00000110,
        0b00000110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00000000,
        0b00000000,
    };

    // 6
    const uint8_t char6[16] = {
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100000,
        0b01111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00000000,
        0b00000000,
    };

    // 7
    const uint8_t char7[16] = {
        0b00000000,
        0b00000000,
        0b01111110,
        0b01100110,
        0b00000110,
        0b00001100,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00000000,
        0b00000000,
    };

    // 8
    const uint8_t char8[16] = {
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00000000,
        0b00000000,
    };

    // 9
    const uint8_t char9[16] = {
        0b00000000,
        0b00000000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111110,
        0b00000110,
        0b00000110,
        0b00000110,
        0b01100110,
        0b01100110,
        0b00111100,
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
            case 2:
                char_bitmap = (uint8_t*)char2;
                break;
            case 3:
                char_bitmap = (uint8_t*)char3;
                break;
            case 4:
                char_bitmap = (uint8_t*)char4;
                break;
            case 5:
                char_bitmap = (uint8_t*)char5;
                break;
            case 6:
                char_bitmap = (uint8_t*)char6;
                break;
            case 7:
                char_bitmap = (uint8_t*)char7;
                break;
            case 8:
                char_bitmap = (uint8_t*)char8;
                break;
            case 9:
                char_bitmap = (uint8_t*)char9;
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