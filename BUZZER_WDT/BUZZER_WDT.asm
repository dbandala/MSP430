;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
WDT_CPS     .equ    1200000/500             ; WD clocks / second count
;-------------------------------------------------------------------------------
;SECCIÓN DE DATOS
;-------------------------------------------------------------------------------
            .bss    WDTSecCnt,2             ; WDT second counter
            .bss    buzzON,1                ; buzzer on flag
;-------------------------------------------------------------------------------
;SECCIÓN DE CÓDIGO
;-------------------------------------------------------------------------------
            .text                           ; DIRECTIVA DE INICIO DE INSTRUCCIONES DE ENSAMBLADOR
;-------------------------------------------------------------------------------
; Main loop
;-------------------------------------------------------------------------------
START:		mov.w   #__STACK_END,SP         ; init stack pointer
RESET:      mov.w   #WDT_MDLY_0_5,&WDTCTL   ; set WD timer interval to 0.5 ms
            mov.w   #WDT_CPS,&WDTSecCnt     ; initialize 1 sec WD counter
            mov.b   #WDTIE,&IE1             ; enable WDT interrupt
            bis.b   #0x07,&P1DIR            ; P1.0 (LED) P1.1-2 (speaker)
            mov.b   #0x04,&P1OUT            ; set P1.1 & P1.2 to toggle
            clr.b   buzzON                  ; turn off buzzer
            bis.w   #LPM0|GIE,SR            ; enable interrupts / sleep
            jmp     $                       ; (should never get here!)
;-------------------------------------------------------------------------------
; Watchdog ISR ------------------------------------------------------------
WDT_ISR:    tst.b   buzzON                  ; buzzer on?, CMP.B #0,buzzON
              jeq   WDT_02                  ; n, Z==1
            xor.b   #0x06,&P1OUT            ; y, use 50% PWM
;-------------------------------------------------------------------------------
WDT_02:     dec.w   &WDTSecCnt              ; decrement counter, 0?
              jne   WDT_04                  ; n, Z==0
            mov.w   #WDT_CPS,&WDTSecCnt     ; y, re-initialize counter
            xor.b   #0x01,&P1OUT            ; toggle led
            xor.b   #0xff,buzzON            ; toggle buzzer on/off
;-------------------------------------------------------------------------------
WDT_04:     reti                            ; return from WDT_ISR
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int10"
            .word   WDT_ISR                 ; Watchdog ISR
            .sect   ".reset"
            .word   START                   ; RESET ISR
            .end
;-------------------------------------------------------------------------------



