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
;   =========== Bootloader ===========
;
;   Third and final stage of the bootloader.
;   Jumps directly to the kernel after copying it to 1MB.
;
;   ==================================

[BITS 32]
[ORG 0x8800]

%define KERNEL_SIZE 32768

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
    mov esi, 0x10000                ; from (0x1000:0x0000)
    mov edi, 0x100000               ; to 1MB
    mov ecx, KERNEL_SIZE            ; size (for now only 1 sector)
    rep movsb

    ; far jump to kernel
    jmp 0x08:0x100000 

times 512 - ($ - $$) db 0