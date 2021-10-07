;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
SMCLK       .equ    1000000                 ; 1.2 Mhz clock
WDT_CTL     .equ    WDT_MDLY_8              ; WDT SMCLK, 8 ms (@1 Mhz)
WDT_CPS     .equ    SMCLK/8000              ; WDT clocks / second count
;-------------------------------------------------------------------------------
;SECCIÓN DE DATOS
;-------------------------------------------------------------------------------
            .bss    WDTSecCnt,2             ; WDT second counter
;-------------------------------------------------------------------------------
;SECCIÓN DE CÓDIGO
;-------------------------------------------------------------------------------
            .text                           ; DIRECTIVA DE INICIO DE INSTRUCCIONES DE ENSAMBLADOR
;-------------------------------------------------------------------------------
; Main loop
;-------------------------------------------------------------------------------
START:
RESET:      mov.w   #__STACK_END,SP         ; Initialize stackpointer
			mov.w   #WDT_CTL,&WDTCTL        ; set WD timer interval
            mov.w   #WDT_CPS,&WDTSecCnt     ; initialize 1 sec WD counter
            bis.b   #WDTIE,&IE1             ; enable WDT interrupt
            mov.b	#0x00,&P1OUT            ; P1 OFF
            bis.b   #0x01,&P1DIR            ; P1.0 output

;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
loop:       bis.w   #LPM0|GIE,SR            ; sleep/enable interrupts
            xor.b   #0x01,&P1OUT            ; toggle P1.0
            jmp     loop                    ; loop indefinitely
;-------------------------------------------------------------------------------
; Watchdog ISR ------------------------------------------------------------
WDT_ISR:    dec.w   &WDTSecCnt              ; decrement counter, 0?
              jne   WDT_02                  ; n
            mov.w   #WDT_CPS,&WDTSecCnt     ; y, re-initialize counter
            bic.b   #LPM0,0(SP)             ; wakeup processor
WDT_02:     reti                            ; return from interrupt
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int10"                ; Watchdog Vector
            .word   WDT_ISR                 ; Watchdog ISR
            .sect   ".reset"                ; MSP430 RESET Vector
			.word   START                   ; start address
            .end


