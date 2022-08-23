; TOGGLEA LOS LEDS CON UN PERIODO CERCANO A UN SEGUNDO. (PAG. 97)
; LA VARIABLE EMPLEADA EN EL CONTADOR DEL RETARDO SE INICIALIZA EN MEMORIA RAM.
;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
DELAYLOOPS 	.EQU	50000						; DECLARACION DE VALOR CONSTANTE EN LA VARIABLE DELAYLOOPS 50000
Var_1:      .BSS    LoopCtr,2				; SE RESERVAN DOS BYTES EN RAM PARA CONTENER A LA VARIABLE LoopCtr
;-------------------------------------------------------------------------------
            .def    RESET                   ; EXPORTA EL PUNTO DE ENTRADA AL PROGRAMA
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
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
Main:		MOV.B 	#80h, &P1OUT 			; PRECARGA EL LED1 CON OFF
			BIS.B 	#BIT6|BIT0,&P1DIR		; CONFIGURA EL BIT0 Y EL BIT6 COMO SALIDAS HACIA LOS LEDS. "|" ES UNA OPERACION OR
;-------------------------------------------------------------------------------
InfLoop: 									; CICLO INDEFINIDO
			mov.w 	#DELAYLOOPS,&LoopCtr 	; INICIALIZA EL CONTADOR DEL RETARDO
DelayLoop: 									; [NUMERO DE CICLOS]
			dec.w 	&LoopCtr 				; DECREMENTA EL CONTADOR DEL RETARDO [4], INSTRUCCION EMULADA
			jnz 	DelayLoop 				; REPITE EL LAZO SI NO ES CERO [2]
			xor.b 	#BIT6|BIT0,& P1OUT 		; TOGGLEA LOS LEDs
			jmp 	InfLoop 				; SALTA AL CICLO INDEFINIDO

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
