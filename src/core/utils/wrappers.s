; =========== WRAPPERS ===========
;
; Some basic wrappers for things like in, out, and some other utilities
;
; ================================
[BITS 32]
[GLOBAL inb]
[GLOBAL outb]
[GLOBAL io_wait]

; [esp + 8] -> port
inb:
    push ebp
    mov ebp, esp

    mov dx, [esp + 8]
    in al, dx

    pop ebp
    ret

; [esp + 8] -> port
; [esp + 12] -> value
outb:
    push ebp
    mov ebp, esp

    mov dx, [esp + 8]
    mov al, [esp + 12]
    out dx, al

    pop ebp
    ret

; small utility to get a small delay because of older hardware
io_wait:
    push ebp
    mov ebp, esp

    ; write to port 0x80 to introduce a small delay
    mov dx, 0x80
    out dx, al

    pop ebp
    ret