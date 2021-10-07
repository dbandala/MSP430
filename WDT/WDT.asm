;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
;SECCIÓN DE DATOS
;-------------------------------------------------------------------------------
SMCLK       .equ    1000000                 ; 1.0 Mhz clock
WDT_CTL     .equ    WDT_MDLY_8              ; WDT SMCLK, 8 ms (@1 Mhz)
WDT_CPS     .equ    SMCLK/8000              ; WD clocks / second count
;-------------------------------------------------------------------------------
            .bss    WDTSecCnt,2             ; WDT second counter
;-------------------------------------------------------------------------------
;SECCIÓN DE CÓDIGO
;-------------------------------------------------------------------------------
            .text
;-------------------------------------------------------------------------------
RESET:	    mov.w   #__STACK_END,SP         ; Initialize stackpointer
;-------------------------------------------------------------------------------
start:      mov.w   #__STACK_END,SP         ; initialize stack pointer
            mov.w   #WDT_CTL,&WDTCTL        ; set WD timer interval
            mov.w   #WDT_CPS,&WDTSecCnt     ; initialize 1 sec WD counter
            bis.b   #WDTIE,&IE1             ; enable WDT interrupt
            bis.b   #0x01,&P1DIR            ; P1.0 output
            bis.w   #GIE,SR                 ; enable interrupts
;-------------------------------------------------------------------------------
loop:
            jmp     loop                    ; loop indefinitely
;-------------------------------------------------------------------------------
; Watchdog ISR
;-------------------------------------------------------------------------------
WDT_ISR:    dec.w   &WDTSecCnt              ; decrement counter, 0?
              jne   WDT_02                  ; n
            mov.w   #WDT_CPS,&WDTSecCnt     ; y, re-initialize counter
            xor.b   #0x01,&P1OUT            ; toggle P1.0
WDT_02:     reti                            ; return from interrupt
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
;-------------------------------------------------------------------------------
; Interrupt Vectors -------------------------------------------------------
            .sect   ".int10"                ; Watchdog Vector
            .word   WDT_ISR                 ; Watchdog ISR
            .sect   ".reset"                ; PUC Vector
            .word   start                   ; RESET ISR
            .end
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
;#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL)                         /* 32ms interval (default) */
;#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS0)                  /* 8ms     " */
;#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1)                  /* 0.5ms   " */
;#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
;#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL)                 /* 1000ms  " */
;#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0)          /* 250ms   " */
;#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1)          /* 16ms    " */
;#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)   /* 1.9ms   " */
;-------------------------------------------------------------------------------
;-------------------------------------------------------------------------------
