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

; known address for stage 1's boot drive (in scratch space)
%define BOOT_DRIVE 0x0500

; beginning of stage 2
s2_start:
    ; re-setup segments
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; enable A20 (if needed)
    call enable_a20

    ; load kernel from disk
    mov ah, 0x42
    mov dl, [BOOT_DRIVE]
    mov si, disk_packet
    int 0x13
    jc .disk_error

    ; get LFB
    call lfb_init

    ; load gdt
    cli
    lgdt [gdtr]

    ; jump to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; far jump to protected mode (code segment is at 0x08 in GDT and offset at 0x8800 which is 2560 bytes after the start of this stage)
    jmp 0x08:0x8800

.disk_error:
    mov si, disk_error_msg
    call print_string
    jmp $       ; hang

enable_a20:
    ; check if A20 is already enabled
    call check_if_a20_enabled
    cmp al, 1
    je .is_enabled

; activates A20 line with different methods until it is enabled
.activate:
    mov si, a20_activating_msg
    call print_string

    ; first try with keyboard controller
    call keyboard
    call check_if_a20_enabled
    cmp al, 1
    je .is_enabled

    ; if not enabled, try fast A20
    call fast_a20_gate
    call check_if_a20_enabled
    cmp al, 1
    je .is_enabled

    ; if still not enabled, int 0x15
    call a20_inth15
    call check_if_a20_enabled
    cmp al, 1
    je .is_enabled

    ; if all fails, print message and hang
    jmp .failed_to_enable

; once it's enabled, just return to when was called
.is_enabled:
    mov si, a20_enabled_msg
    call print_string
    ret

; failed to enable the a20 line (just print message and hang)
.failed_to_enable:
    mov si, a20_failed_msg
    call print_string
    jmp $       ; hang


; checks if A20 line is enabled
; AL = 1 if enabled
check_if_a20_enabled:
    in al, 0x92
    test al, 2
    jnz .enabled

    ; not enabled
    mov al, 0
    ret

.enabled:
    mov al, 1
    ret


; different A20 activation methods

; first method: through keyboard controller (8042 chip)
keyboard:
    ; the flow to activate the A20 line through the keyboard controller is as follows:
    ; disable interrupts -> disable keyboard -> read output port -> save res -> write next byte to output port -> activate A20 -> activate keyboard -> enable interrupts
    cli

    ; disable keyboard
    call a20_kb_wait
    mov al, 0xAD
    out 0x64, al

    ; read controller output port
    call a20_kb_wait
    mov al, 0xD0
    out 0x64, al

    ; save response byte
    call a20_kb_wait_response
    in al, 0x60
    push ax

    ; write next byte to output port
    call a20_kb_wait
    mov al, 0xD1
    out 0x64, al

    ; activate A20
    call a20_kb_wait
    pop ax
    or al, 2
    out 0x60, al

    ; activate keyboard
    call a20_kb_wait
    mov al, 0xAE
    out 0x64, al

    ; reactivate interrupts
    call a20_kb_wait
    sti
    ret
    
; wait until the input buffer is empty
a20_kb_wait:
    in al, 0x64
    test al, 2
    jnz a20_kb_wait
    ret

; wait until response byte arrives
a20_kb_wait_response:
    in al, 0x64
    test al, 1
    jz a20_kb_wait_response
    ret


; another method to activate the A20 line is through the fast A20 gate (only IF supported, though, which we need to check as it could do something else on unsupported systems)
fast_a20_gate:
    ; check if fast A20 is supported by checking bit 1 of port 0x92 (0x10 = fast a20 gate)
    in al, 0x92
    test al, 2
    jz .not_supported

    ; if supported, just set bit 1 of port 0x92 to enable A20 line
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

.not_supported:
    ret


; some older BIOSes provide a function at int 0x15 to enable the A20 line
a20_inth15:
    ; query A20 support
    mov ax, 0x2403 
    int 0x15
    jc .ns          ; not supported
    test ah, ah
    jnz .ns

    ; if this is hit, that means a20 was already tested for not being activated, so we can skip 0x2402 query

    ; activating A20 line
    mov ax, 0x2401
    int 0x15
    jc .failed      ; failed to activate
    test ah, ah
    jnz .failed
    ret

; if one of the below is hit, just return and try the last method
.failed:
.ns:
    ret


; LFB
lfb_init:
    ; get vbe info
    mov ax, 0x4F00
    int 0x10
    cmp ax, 0x004F
    jne .failed

    

.failed:
    mov si, lfb_failed
    call print_string
    ret

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

; --- Messages & data ---
a20_enabled_msg db "[Bootloader] A20 line enabled.", 10, 13, 0
a20_activating_msg db "[Bootloader] Activating A20 line...", 10, 13, 0
a20_failed_msg db "[Bootloader] Failed to enable A20 line.", 10, 13, 0
disk_error_msg db "[Bootloader] Error reading disk.", 10, 13, 0
lfb_failed db "[Bootloader] Failed to get LFB.", 10, 13, 0

; GDT
gdt_start: dq 0x0000000000000000, 0x00CF9A000000FFFF, 0x00CF92000000FFFF, 0x00CFFA000000FFFF, 0x00CFF2000000FFFF
gdt_end db 0
gdtr: 
    dw gdt_end - gdt_start - 1
    dd gdt_start

; data for disk packet to load kernel from disk
align 4
disk_packet:
    db 0x10     ; size of the packet
    db 0        ; reserved (must be 0)
    dw 1        ; sectors to read
    dw 0x0000   ; offset
    dw 0x1000   ; segment
    dq 6        ; start LBA sector to read from


; pad to 2560 bytes (5 sectors, each sector 512 bytes)
times 2560-($-$$) db 0