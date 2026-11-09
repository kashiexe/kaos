/* bootloader starts at _start */
ENTRY(_start)

SECTIONS 
{
    /* kernel needs to be loaded at 1MB */
    . = 1M;

    .text : {
        *(.multiboot)
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