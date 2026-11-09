;   =========== Bootloader ===========
;
;   First stage of the bootloader.
;   This stage loads the second stage of the bootloader from disk and jumps to it.
;
;   ==================================

; loaded at 0x7C00 by BIOS
[BITS 16]
[ORG 0x7C00]

; actual start code
_start:
    jmp 0x0000:flush_cs

flush_cs:
    ; save boot_drive from BIOS
    mov [boot_drive], dl

    ; setup segments
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    cld

    ; load the second stage of the bootloader from disk
    mov si, msg
    call print_string
    call load_second_stage
    
    ; jump to the second stage of the bootloader
    jmp 0x0000:0x7E00

; utility to print string
print_string:
    ; print string in si
    mov ah, 0x0E
.next_char:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .next_char
.done:
    ret

; actually loads the second stage of the bootloader from disk
load_second_stage:
    mov ah, 0x02            ; read sectors
    mov al, 4               ; number of sectors to read
    mov ch, 0               ; cylinder
    mov cl, 2               ; sector (start from sector 2)
    mov dh, 0               ; head
    mov dl, [boot_drive]    ; drive number (first hard disk)
    mov bx, 0x7E00          ; buffer to load the second stage
    int 0x13                ; call BIOS

    ; check for errors
    jc .error
    ret
.error:
    ; print error message and halt
    mov si, error_msg
    call print_string
    jmp $

; message strings
msg db "[Bootloader] Loading second stage...", 0
error_msg db "[Bootloader] Error loading second stage!", 0
boot_drive db 0

; 0 pad to 512 bytes and signature
times 510 - ($ - $$) db 0
dw 0xAA55