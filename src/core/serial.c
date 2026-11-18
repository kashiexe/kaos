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
*/
#include "include/serial.h"

extern uint8_t inb(uint16_t port);
extern void outb(uint16_t port, uint8_t value);

/**
 * sets up the serial port for communication
 */
void serial_init() {
    outb(SERIAL_PORT + 1, 0x00);    // disable all interrupts
    outb(SERIAL_PORT + 3, 0x80);    // enable DLAB (set baud rate divisor)
    outb(SERIAL_PORT + 0, 0x03);    // set divisor to 3 (lo byte) 38400 baud
    outb(SERIAL_PORT + 1, 0x00);    // (hi byte)
    outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(SERIAL_PORT + 2, 0xC7);    // enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

/**
 * checks if the transmit FIFO is empty
 */
int is_transmit_empty() {
    return inb(SERIAL_PORT + 5) & 0x20;
}

/**
 * writes a single character to the serial port
 */
void serial_write(char a) {
    // wait until transmit FIFO is empty
    while(is_transmit_empty() == 0) {};
    outb(SERIAL_PORT, a);
}

/**
 * prints a string to the serial port
 */
void serial_print(const char* str) {
    while(*str) {
        serial_write(*str++);
    }
}

/**
 * prints a number in hex to the serial port
 */
void serial_hex(uint32_t num) {
    const char* hex_chars = "0123456789ABCDEF";
    for(int i = 28; i >= 0; i -= 4) {
        char hex_digit = hex_chars[(num >> i) & 0xF];
        serial_write(hex_digit);
    }
}