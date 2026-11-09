;   =========== Bootloader ===========
;
;   Second stage of the bootloader.
;   This stage is responsible to setup the environment for the kernel before jumping to it.
;
;   ==================================

; loaded at 0x7E00 which is after the 512 byte-first stage of the bootloader
[BITS 16]
[ORG 0x7E00]
global s2_start

section .data

section .text
s2_start:
    