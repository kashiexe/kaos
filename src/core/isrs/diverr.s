[BITS 32]
[GLOBAL diverr]
[EXTERN diverr_handler]

; diverr interrupt handler wrapper
diverr:
    pushad
    cld
    call diverr_handler
    popad
    iret