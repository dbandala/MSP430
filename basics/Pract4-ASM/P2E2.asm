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
SetupP1		bis.b	#00001111b, &P1DIR		; establece los pines 0,1,2,3 de P1 como salida
SetupP2		bic.b	#00000001b, &P2DIR		; establece p2.0 como entrada
			bis.b	#00000001b, &P2REN		; enciende la resistencia pull up de p2.0
			mov.b	#00h, &P1OUT			; inicializa los pines en 0
			mov.b	#0010h, R5				;inicializa en cero el conteo
;-------------------------------------------------------------------------------
MainLoop:
			bit		#00000001b, &P2IN       ; condicion
			jnz		conteo_asc
			dec		R5
			mov.w	R5, &P1OUT
			call	#delay
			jmp		MainLoop

conteo_asc:
			inc		R5
			mov.w	R5, &P1OUT
			call	#delay
			jmp		MainLoop

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
