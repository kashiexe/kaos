;   =========== Bootloader ===========
;
;   Second stage of the bootloader.
;   This stage is responsible to setup the environment for the kernel before jumping to it.
;   Mainly: getting info from BIOS, activating A20 line, GDT, switching to protected mode and some other things.
;
;   ==================================

; loaded at 0x7E00 which is after the 512 byte-first stage of the bootloader
[BITS 16]
[ORG 0x7E00]

; beginning of stage 2
s2_start:
    ; re-setup segments
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; enable A20 (if needed)
    call enable_a20
    jmp $ ; hang for now

enable_a20:
    ; check if A20 is already enabled
    jmp .check_if_a20_enabled

; activates A20 line with different methods until it is enabled
.activate:
    mov si, a20_activating_msg
    call print_string
    jmp $ ; hang for now

; once it's enabled, just return to when was called
.is_enabled:
    mov si, a20_enabled_msg
    call print_string
    ret

; checks if A20 line is enabled
.check_if_a20_enabled:
    in al, 0x92
    test al, 2
    jnz .is_enabled
    jmp .activate

; different A20 activation methods

; --- Utilities ---

; prints string at si until null terminator
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

; --- Messages ---
a20_enabled_msg db "[Bootloader] A20 line enabled.", 10, 13, 0
a20_activating_msg db "[Bootloader] Activating A20 line...", 10, 13, 0



; pad to 5120 bytes (10 sectors, each sector 512 bytes)
times 5120-($-$$) db 0