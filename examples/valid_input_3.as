;file valid_input_3.as
.entry XYZ
    .entry MAIN

KINITIALVALUE: sub r4   ,    r3

MAIN:   mov r3, LENGTH

LOOP:   jmp L1

    mcr M1
sub r1, r7
    bne STR
    endmcr

    .entry GGG
prn #-5
bne LOOP

XYZ: mov r4, r2
GGG: .data 5, 6, 7
M1

L1:     inc K
bne LOOP

STR:    .string "666"abcdef"

LENGTH: .data 6, -9, 15

K:      .data 4    ,  ,  -55,4,4,4,6

    mcr M2
mov reg1, val
add reg2, reg1
    endmcr

M2
ABC: mov XYZ, r3
reg1: .data 6,5,-555,66
reg2: .data 6,5,-555,66
val: .string "asfas   %%dfjk"
;ubuntu