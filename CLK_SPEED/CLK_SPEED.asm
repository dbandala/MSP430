;-------------------------------------------------------------------------------
; PROGRAMA PARA VERIFICAR LA CONFIGURACION DEL RELOJ DCO A ALTA Y BAJA FRECUENCIA
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
;SMCLK       .equ    1200000                 ; 1.2 Mhz clock
;WDT_CPS     .equ    SMCLK/8000              ; WDT clocks / second count
MHz8        .set    0                        ; 8 MHz flag
MHz12       .set    0                        ; 12 MHz flag
Low_Hz		.set	1						 ; Frecuencia más baja para el DCO
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
            .text                           ; DIRECTIVA DE INICIO DE INSTRUCCIONES DE ENSAMBLADOR
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
RESET:       mov.w   #__STACK_END,SP         ; Initialize stackpointer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
start:      mov.w   #__STACK_END,SP         ; Initialize stackpointer
            mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; stop WDT
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
           .if     MHz8                     ; set DCO to 8 MHz
            mov.b   #CALBC1_8MHZ,&BCSCTL1   ; set range
            mov.b   #CALDCO_8MHZ,&DCOCTL    ; set DCO step + modulation
           .endif
;-------------------------------------------------------------------------------
           .if     Low_Hz                   ; set DCO lowesr DCOx
			clr.b  &DCOCTL 					; Select lowest DCOx
           .endif
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
            bis.b   #0x01,&P1DIR            ; set P1.0 as output
mainloop:   xor.b   #0x01,&P1OUT            ; toggle P1.0
            mov.w   #8,r14                  ; use R14 as outer loop counter
delaylp1:   mov.w   #0,r15                  ; use R15 as delay counter
delaylp2:   sub.w   #1,r15                  ; delay over?
            jne   	delaylp2                ; n
            sub.w   #1,r14                  ; y, outer loop done?
            jne   	delaylp1                ; n
            jmp     mainloop                ; y, toggle led
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
			.word   start                   ; start address
            .end


