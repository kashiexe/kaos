;   =========== Kernel Entry Point ===========
;
;   Stub code to set up the stack and call kmain()
;
;   =========================================
[BITS 32]
[GLOBAL _start]
[EXTERN kmain]

_start:
    mov esp, 0x90000

    call kmain

.hang:
    cli
    hlt 
    jmp .hang