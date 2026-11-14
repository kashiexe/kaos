[BITS 32]
[GLOBAL overflow]
[EXTERN overflow_handler]

overflow:
    pushad
    cld
    call overflow_handler
    popad
    iret