/* bootloader starts at _start */
ENTRY(_start)

SECTIONS 
{
    /* kernel needs to be loaded at 1MB */
    . = 1M;

    .text : {
        build/core/kentry.o(.text) /* entry stub */
        *(.text)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }
}