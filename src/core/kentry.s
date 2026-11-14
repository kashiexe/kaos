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