; file invalid_input_2.as
.entry LIST

.extern M

.define sz = 2
MAIN:	mov r3, LIST[sz]
;error
        mov r1 , UNREAL[-2]
LOOP: 	jmp W
;error
	prn -5
	mov STR[5], STR[2]
;error
	sub r1, r9
;error
	cmp K, 3
;error
	bne
L1: 	inc L3
.entry LOOP
	bne LOOP
;error
END: hlt "abcd"
.define len = 4
;error
STR: .string "abcdef
MOVE: rts
;error
LIST: .dATa 6, -9, len
;error
K: .data
.extern L3
;ubuntu