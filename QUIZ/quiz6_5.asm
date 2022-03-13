
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
			.bss 	i,2                    ; int i
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
Main:		mov.b 	#80h, &P1OUT 			; PRECARGA EL LED1 CON OFF
			bis.b 	#BIT6|BIT0,&P1DIR		; CONFIGURA EL BIT0 Y EL BIT6 COMO 
											; SALIDAS
			mov.w	#1h, i     				;i=1
estr_for: 	cmp.w	#10,i					;for (i=1; i<10; i++)
            jge  	termina					;

estr_if:    cmp		#5, i
            jhs		func2
func1:      inc		i
			mov.w	#00000001,&P1OUT
			mov.w	#0x0000,&P1OUT
            jmp		estr_for
func2:		add		#2h, i
			mov.w	#BIT6,&P1OUT
			mov.w	#0x0000,&P1OUT
			jmp		estr_for

termina:
			jmp		estr_for
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
