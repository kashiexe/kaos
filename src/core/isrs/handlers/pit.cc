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

    =========== PIT Handler ===========

    Handles the PIT interrupt and channels

    ===================================
*/
#include "include/pit.hh"

using namespace kaos;
using namespace kaos::drivers;

// from wrappers.s
extern "C" void outb(uint16_t port, uint8_t value);
extern "C" uint8_t inb(uint16_t port);

/**
 * helper function to make a command byte for the PIT
 * @param channel the channel to use (0, 1, or 2)
 * @param rw_access the read/write access mode 
 * @param mode the operating mode
 * @param bcd binary or BCD mode
 * @returns the command byte to send to the PIT
 */
unsigned char pit::make_cmd(uint8_t channel, uint8_t rw_access, uint8_t mode, uint8_t bcd) {
    return (channel << 6) | (rw_access << 4) | (mode << 1) | bcd;
}

/**
 * configures the PIT to the frequency in `pit.h`
 */
void pit::init() {
    set_frequency(PIT_FREQ);
}

/**
 * programs the PIT to a specific frequency
 * @param freq the frequency to set the PIT to (in hz, default is `100hz`)
 */
void pit::set_frequency(uint32_t freq) {
    if(freq == 0) freq = PIT_FREQ; // default to 100hz if freq is 0

    // calculate the divisor
    uint32_t div = PIT_HZ / freq;

    // the PIT cannot be programmed to a frequency so high (or negative/0 frequency)
    if(div == 0 or div > 65536) {
        return;
    }

    // must be 16 bit
    uint16_t proper_div = static_cast<uint16_t>(div);

    // program the PIT to the desired frequency
    outb(PIT_CMD, make_cmd(0b00, 0b11, 0b011, 0b0));

    // write the divisor to the PIT channel 0 data port
    outb(PIT_CH0, proper_div & 0xFF);           // low byte
    outb(PIT_CH0, (proper_div >> 8) & 0xFF);    // high byte
}

/**
 * this returns the ACTUAL frequency of the PIT
 * even if you configure the PIT to 100hz, the actual frequency may be different
 * this function returns the actual frequency of the PIT
 * @returns the actual frequency of the PIT
 */
uint32_t pit::get_frequency() {
    // read the current divisor from the PIT channel 0 data port
    outb(PIT_CMD, make_cmd(0b00, 0b00, 0b000, 0b0)); // latch command

    // get low and high bytes of the divisor
    uint8_t low = inb(PIT_CH0);     // low byte
    uint8_t high = inb(PIT_CH0);    // high byte

    // build the divisor
    uint16_t div = (high << 8) | low;

    // return 0 if the div is 0 (no division by 0)
    if(div == 0) return 0;

    // return the actual frequency of the PIT
    return PIT_HZ / div;
}

// C wrapper functions for the PIT
extern "C" {
    void pit_init() { pit::init(); }
    void pit_set_frequency(uint32_t freq) { pit::set_frequency(freq); }
    uint32_t pit_get_frequency() { return pit::get_frequency(); }
    unsigned char pit_make_cmd(uint8_t channel, uint8_t rw_access, uint8_t mode, uint8_t bcd) {
        return pit::make_cmd(channel, rw_access, mode, bcd);
    }
}