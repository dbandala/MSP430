;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file

;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory
            .retain                         ; Override ELF conditional linking
                                            ; and retain current section
            .retainrefs                     ; Additionally retain any sections
                                            ; that have references to current
                                            ; section
;-------------------------------------------------------------------------------
RESET       mov.w   #__STACK_END,SP         ; Initialize stackpointer
StopWDT     mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; Stop watchdog timer
SetupP1		bis.b	#01111111b, &P1DIR		; establece los pines 0,1,2,3 de P1 como salida
SetupP2		bic.b	#00000001b, &P2DIR		; establece p2.0 como entrada
			bis.b	#00000001b, &P2REN		; enciende la resistencia pull up de p2.0
			mov.b	#0xFFFF, &P1OUT			; inicializa los pines en 0
			mov.b	#0000h, R5				;inicializa en cero el conteo
;-------------------------------------------------------------------------------
MainLoop:
			call	#display
			cmp		#0x0010, R5
			jhs		reinicia_0
			bit		#00000001b, &P2IN       ; condicion
			jnz		conteo_asc
conteo_desc:
			dec		R5
			call	#delay
			cmp		#0xFFFF, R5
			jeq		reinicia_F
			jmp		MainLoop

conteo_asc:
			inc		R5
			call	#delay
			jmp		MainLoop

reinicia_0:	mov.b	#0000h, R5
			jmp		MainLoop

reinicia_F:	mov.b	#0x000F, R5
			jmp		MainLoop

display:
			cmp		#0x0000, R5
			jnz		S1
			mov.w	#0x0001, &P1OUT
			ret
S1:			cmp		#0x0001, R5
			jnz		S2
			mov.w	#0x004F, &P1OUT
			ret
S2:			cmp		#0x0002, R5
			jnz		S3
			mov.w	#0x0012, &P1OUT
			ret
S3:			cmp		#0x0003, R5
			jnz		S4
			mov.w	#0x0006, &P1OUT
			ret
S4:			cmp		#0x0004, R5
			jnz		S5
			mov.w	#0x004C, &P1OUT
			ret
S5:			cmp		#0x0005, R5
			jnz		S6
			mov.w	#0x0024, &P1OUT
			ret
S6:			cmp		#0x0006, R5
			jnz		S7
			mov.w	#0x0020, &P1OUT
			ret
S7:			cmp		#0x0007, R5
			jnz		S8
			mov.w	#0x000F, &P1OUT
			ret
S8:			cmp		#0x0008, R5
			jnz		S9
			mov.w	#0x0000, &P1OUT
			ret
S9:			cmp		#0x0009, R5
			jnz		S10
			mov.w	#0x0004, &P1OUT
			ret
S10:		cmp		#0x000A, R5
			jnz		S11
			mov.w	#0x0008, &P1OUT
			ret
S11:		cmp		#0x000B, R5
			jnz		S12
			mov.w	#0x0060, &P1OUT
			ret
S12:		cmp		#0x000C, R5
			jnz		S13
			mov.w	#0x0031, &P1OUT
			ret
S13:		cmp		#0x000D, R5
			jnz		S14
			mov.w	#0x0042, &P1OUT
			ret
S14:		cmp		#0x000E, R5
			jnz		S15
			mov.w	#0x0030, &P1OUT
			ret
S15:		mov.w	#0x0038, &P1OUT
			ret

delay:
			mov.w	#0xFFFF, R4
retardo:	dec.w	R4
 			jnz		retardo
retardo2:	dec.w	R4
			jnz		retardo2
retardo3:	dec.w	R4
			jnz		retardo3
retardo4:	dec.w	R4
			jnz		retardo4
retardo5:	dec.w	R4
			jnz		retardo5
retardo6:	dec.w	R4
			jnz		retardo6
			ret

;-------------------------------------------------------------------------------
;           Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect 	.stack

;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
