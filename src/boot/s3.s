;   =========== Bootloader ===========
;
;   Third and final stage of the bootloader.
;   Jumps directly to the kernel after copying it to 1MB.
;
;   ==================================

[BITS 32]
[ORG 0x8800]

s3_start:
    mov ax, 0x10    ; data segment in GDT is at 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; safe stack pointer
    mov esp, 0x9000

    ; prep kernel
    mov esi, 0x10000    ; from (0x1000:0x0000)
    mov edi, 0x100000   ; to 1MB
    mov ecx, 0x10000    ; size (64KB)
    rep movsb

    ; far jump to kernel
    jmp 0x08:0x100000 

times 512 - ($ - $$) db 0