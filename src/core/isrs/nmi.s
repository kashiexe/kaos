[BITS 32]
[GLOBAL nmi]
[EXTERN nmi_handler]

nmi:
    pushad
    cld
    call nmi_handler
    popad
    iret