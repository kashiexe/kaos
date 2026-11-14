[BITS 32]
[GLOBAL bound_range]
[EXTERN bound_range_handler]

bound_range:
    pushad
    cld
    call bound_range_handler
    popad
    iret