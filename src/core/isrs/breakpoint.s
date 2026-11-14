[BITS 32]
[GLOBAL breakpoint]
[EXTERN breakpoint_handler]

breakpoint:
    pushad
    cld
    call breakpoint_handler
    popad
    iret