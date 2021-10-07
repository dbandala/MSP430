;Reloj básico, modo de ahorro de energía LPM3 usando el WDT ISR, ACLK = VLO/2
;Este programa opera normalmente en modo LPM3, enciende con un pulso a P1.0 cada 4 segundos
;WDT ISR es usado para despertar al sistema. Todos los pines se configuran como salidas
;para eliminar las entradas flotadas.
;Se requiere instalar el cristal en los pines XIN XOUT para trabajar con ACLK.
;ACLK = VLO/2, MCLK = SMCLK = default DCO
;                MSP430G2xx3
;             -----------------
;         /|\|              XIN|-
;          | |                 | 32kHz
;          --|RST          XOUT|-
;            |                 |
;            |             P1.0|-->LED
;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;
;
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
            
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
            .text                           ; Assemble into program memory.
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------                                            ;-------------------------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; Initialize stackpointer
SetupWDT    mov.w   #WDT_ADLY_1000,&WDTCTL  ; WDT 1s*4 interval timer
            bis.b   #DIVA_1,&BCSCTL1        ; ACLK/2
            bis.b   #LFXT1S_2,&BCSCTL3      ; ACLK = VLO
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
SetupP1     mov.b   #0FFh,&P1DIR            ; All P1.x outputs
            clr.b   &P1OUT                  ; All P1.x reset
SetupP2     mov.b   #0FFh,&P2DIR            ; All P2.x outputs
            clr.b   &P2OUT                  ; All P2.x reset
                                            ;
Mainloop    bis.b   #001h,&P1OUT            ; Set P1.0
            push.w  #5000                   ; Delay to TOS
Delay       dec.w   0(SP)                   ; Decrement TOS
            jnz     Delay                   ; Delay over?
            incd.w  SP                      ; Clean stack
            bic.b   #001h,&P1OUT            ; Reset P1.0
            bis.w   #LPM3+GIE,SR            ; Enter LPM3, enable interrupts
            jmp     Mainloop                ; Again
                                            ;
;-------------------------------------------------------------------------------
WDT_ISR  ;  Exit LPM3 on reti
;-------------------------------------------------------------------------------
            bic.w   #LPM3,0(SP)             ; Clear LPM3 from TOS
            reti                            ;
                                            ;
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int10"                ; Watchdog Vector
            .word   WDT_ISR                 ; Watchdog ISR
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
            .end
