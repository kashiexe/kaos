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
; =========== RNG ===========
;
; Implementation for the "rand" function
;
; ===========================
[BITS 32]
[GLOBAL rand]

; seed for the random number generator
rng_seed dd 0

; expects [esp + 4] to contain max
; and [esp + 8] to contain if cpuid is available
; returns a random number between 0 and max - 1 at eax
rand:
    push ebp
    mov ebp, esp

    mov ecx, [esp + 8] ; max
    cmp dword [rng_seed], 0
    jz .init_rng

    ; return 0 if max less than or equal to 1
    cmp ecx, 1
    jbe .zero

.loop_:
    push edx
    push ecx
    call rng_next       ; random 32-bit number
    pop ecx
    pop edx

    ; perform eax*ecx -> edx:eax (to remove statistical bias)
    mul ecx
    cmp eax, edx
    jb .loop_           ; if eax < edx, retry

    mov eax, edx
    pop ebp
    ret

.init_rng:
    ; pass cpuid_available to rng_init
    push dword [ebp + 12]
    call rng_init

    ; clean up the stack
    add esp, 4
    jmp .loop_

.zero:
    xor eax, eax
    pop ebp
    ret

; we will use XorShift32 algorithm for random number generation
rng_init:
    push ebp
    mov ebp, esp

    mov eax, [esp + 8] ; cpuid_available
    cmp byte [eax], 0
    je .fallback

    ; check if rdrand is supported
    mov eax, 1
    cpuid
    test ecx, 1 << 30 ; check if bit 30 of ecx is set
    jz .fallback

    mov ecx, 10

.loop_rdrand:
    rdrand eax
    jc .save
    loop .loop_rdrand

.fallback:
    ; use rdtsc as a fallback
    rdtsc

; saves the seed
.save:
    test eax, eax       ; state must not be zero
    jnz .store
    mov eax, 0x9E3779B9 ; fallback constant
    
.store:
    mov [rng_seed], eax
    pop ebp
    ret

; advance state
rng_next:
    mov eax, [rng_seed]

    ; failsafe 
    test eax, eax
    jnz .step
    mov eax, 0x9E3779B9 ; fallback constant

.step:
    ; algorithm
    mov edx, eax
    shl eax, 13
    xor eax, edx        ; state ^= state << 13

    mov edx, eax
    shr eax, 17
    xor eax, edx        ; state ^= state >> 17

    mov edx, eax
    shl eax, 5
    xor eax, edx        ; state ^= state << 5

    mov [rng_seed], eax
    ret