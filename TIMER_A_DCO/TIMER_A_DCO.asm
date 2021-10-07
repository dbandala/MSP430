;CONFIGURACIÓN DEL TIMER_A. LA FUENTE DE RELOJ ES SMCLK, EL DIVISOR DE RELOJ ES DE 8,
;MODO DE TRABAJO DEL TIMER=UP MODE; SE HABILITA LA BANDERA DE INTERRUPCIÓN DEL TIMER
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
TA_CTL      .equ    TASSEL_2|ID_3|MC_1|TAIE ; 000000 10 11 01 000 1 = SMCLK,/8,UP,IE
TA_FREQ     .equ    0xFFFF                  ; clocks, F_DCO=1.1375 MHz
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------                                            ;-------------------------------------------------------------------------------
RESET:
START:      mov.w   #__STACK_END,SP         ; Initialize stackpointer
            mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; stop WDT
            bis.b   #0x01,&P1DIR            ; set P1.0 as output
;-------------------------------------------------------------------------------
            clr.w   &TAR                    ; reset timerA, ADDR=0X0170
            mov.w   #TA_CTL,&TACTL          ; set timerA control reg
            mov.w   #TA_FREQ,&TACCR0        ; set interval (frequency)
	        bis.w   #LPM0|GIE,SR            ; enter LPM0 w/interrupts
;-------------------------------------------------------------------------------
LPM0M:		jmp     LPM0M
;-------------------------------------------------------------------------------
; Timer A ISR
;-------------------------------------------------------------------------------
TA_ISR:     bic.w   #TAIFG,&TACTL           ; acknowledge interrupt
            xor.b   #0x01,&P1OUT            ; toggle P1.0
            reti
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int08"                ; Watchdog Vector
            .word   TA_ISR                  ; Watchdog ISR
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  START
            .end
