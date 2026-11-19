/*
    Copyright 2026 Kashi

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    =========== BASIC ===========

    Contains basic temporary functions for the kernel

    =============================
*/
#include "include/basic.h"
#include "include/boot_info.h"

static uint32_t y_pos = 0;

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

    // 0 wasn't being printed, so deliberately print if 0
    if (num == 0) {
        for(int y = 0; y < 16; y++) {
            uint16_t row = y_pos + y;
            for(int x = 0; x < 8; x++) {
                if (char0[y] & (1 << (7 - x))) {
                    screen[(row * (pitch / 4)) + x] = 0xFFFFFF;
                }
            }
        }
        y_pos += 16;
        if(y_pos >= ((volatile vesa_info_t*)VESA_INFO)->height) {
            y_pos = 0;
        }
        return;
    }

    // extract digits into an array
    unsigned int digits[10];
    unsigned int num_digits = 0;
    while(num > 0) {
        digits[num_digits++] = num % 10;
        num /= 10;
    }

    for(uint8_t i = 0; i < num_digits; i++) {
        uint8_t digit = digits[num_digits - 1 - i]; // print in reverse order

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
                char_bitmap = (uint8_t*)char0; // fallback to '0'
                break;
        }

        // print digit to screen
        for(int y = 0; y < 16; y++) {
            uint16_t row = y_pos + y;
            for(int x = 0; x < 8; x++) {
                if(char_bitmap[y] & (1 << (7 - x))) {
                    screen[(row * (pitch / 4)) + (x + (i * 8))] = 0xFFFFFF; // white color
                }
            }
        }
    }

    // move down for next number
    y_pos += 16;
    if(y_pos >= ((volatile vesa_info_t*)VESA_INFO)->height) {
        y_pos = 0;
    }
}

/**
 * future function to actually print a string using the font, for now its just a placeholder tho
 */
/*
extern void* font_start;
extern void* font_end;
static uint32_t x_pos = 0;

void easy_print(volatile unsigned int* screen, const uint16_t pitch, const char* str) {
    
}
*/