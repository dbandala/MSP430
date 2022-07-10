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
SetupP1		bis.b	#10000000b, &P1DIR		; establece p1.7 como salida
			bic.b	#00010000b, &P1DIR		; establece p1.4 como entrada
			bis.b	#00010000b, &P1REN		; enciende la resistencia pull up de p1.4
			mov.b	#00h, &P1OUT			; inicializa los pines en 0
;-------------------------------------------------------------------------------
Mainloop:
			bit		#00010000b, &P1IN       ; condicion
			jnz		prender_led
			jmp		Mainloop

prender_led
			mov.b	#10000000b, &P1OUT		; prende el led
			call	#tiempo
			mov.b	#00000000b, &P1OUT		;apaga el led
			jmp 	Mainloop				; regresa al ciclo de espera

tiempo		mov.w	#0FFFFh, R4
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
            .end
