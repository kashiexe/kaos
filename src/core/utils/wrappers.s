;
;   Copyright 2026 Kashi
;
;   Licensed under the Apache License, Version 2.0 (the "License");
;   you may not use this file except in compliance with the License.
;   You may obtain a copy of the License at
;
;       http://www.apache.org/licenses/LICENSE-2.0
;
;   Unless required by applicable law or agreed to in writing, software
;   distributed under the License is distributed on an "AS IS" BASIS,
;   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;   See the License for the specific language governing permissions and
;   limitations under the License.
;
; =========== WRAPPERS ===========
;
; Some basic wrappers for things like in, out, and some other utilities
;
; ================================
[BITS 32]
[GLOBAL inb]
[GLOBAL outb]
[GLOBAL io_wait]

; uint8_t inb(uint16_t port)
; [esp + 8] -> port
inb:
    push ebp
    mov ebp, esp

    mov dx, [esp + 8]
    in al, dx

    pop ebp
    ret

; void outb(uint16_t port, uint8_t value)
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