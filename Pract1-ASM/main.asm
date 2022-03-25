;-------------------------------------------------------------------------------
	.cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
	.text                           ; Assemble into program memory
;-------------------------------------------------------------------------------
RESET		mov.w	#0280h,SP                 ; inicializa pila
StopWDT		mov.w	#WDTPW+WDTHOLD,&WDTCTL    ;detiene el temporizador "whatdog"
setupP1		bis.b	#001h,&P1DIR              ;configura p1.0 como salida

Mainloop	xor.b	#001h,&P1OUT              ; P1.0 xor 0x001
Wait		mov.w	#050000,R15               ; cuenta inicial para retardo
L1			dec.w	R15                       ; decrementa r15
			jnz		L1                        ;r15=0?
			jmp		Mainloop                  ;salto a mainloop
;-------------------------------------------------------------------------------
;           Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .end
