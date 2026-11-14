[BITS 32]
[GLOBAL debugexc]
[EXTERN debugexc_handler]

; debug exception interrupt handler wrapper
debugexc:
    pushad
    cld
    call debugexc_handler
    popad
    iret