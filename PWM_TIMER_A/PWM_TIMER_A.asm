;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;-------------------------------------------------------------------------------
TA_CTL      .equ    TASSEL_2|ID_0|MC_1|TAIE ; SMCLK, /1, UP, IE
TA_FREQ     .equ    50                     ; FREQ / SMCLK = 0.0001 = 100 us
DUTY		.equ	100
;-------------------------------------------------------------------------------
;SECCIÓN DE DATOS
;-------------------------------------------------------------------------------
            .bss    pwm_duty,2              ; PWM duty cycle counter
            .bss    pwm_cnt,2               ; PWM frequency counter
;-------------------------------------------------------------------------------
;SECCIÓN DE CÓDIGO
;-------------------------------------------------------------------------------
            .text                           ; DIRECTIVA DE INICIO DE INSTRUCCIONES DE ENSAMBLADOR
;-------------------------------------------------------------------------------
; Main loop
;-------------------------------------------------------------------------------
START:		mov.w   #__STACK_END,SP               ; init stack pointer
RESET:      mov.w   #WDTPW|WDTHOLD,&WDTCTL  ; stop watchdog
            mov.b   #0x00,&P1OUT			; OFF LEDs RED,GREEN
            bis.b   #0x41,&P1DIR            ; set P1.0,6 as output
            mov.b   #0x40,&P1OUT			; ON LED6 GREEN
            clr.w   &TAR                    ; reset timerA
            mov.w   #TA_CTL,&TACTL          ; set timerA control reg
            mov.w   #TA_FREQ,&TACCR0        ; set interval (frequency)
            clr.w   &pwm_duty               ; init PWM counters
            clr.w   &pwm_cnt
;-------------------------------------------------------------------------------
loop:       bis.w   #LPM0|GIE,SR            ; red -> green, enter LPM0 w/interrupts
            inc.w   &pwm_duty               ; increment &
            cmp.w   #100,&pwm_duty          ; 100% (full on)?
              jlo   loop                    ; n
;-------------------------------------------------------------------------------
loop02:     bis.w   #LPM0|GIE,SR            ; green -> red, enter LPM0 w/interrupts
            dec.w   &pwm_duty               ; decrement % (full off)?
              jne   loop02                  ; n
            jmp     loop                    ; y, repeat
;-------------------------------------------------------------------------------
TA_isr:     bic.w   #TAIFG,&TACTL           ; acknowledge interrupt
            cmp.w   &pwm_duty,&pwm_cnt      ; in duty cycle?
              jne   TA_isr2                 ; n
            xor.b   #0x41,&P1OUT            ; y, turn on LEDs

TA_isr2:    inc.w   &pwm_cnt                ; increment %
            cmp.w   #100,&pwm_cnt           ; 100%?
              jlo   TA_isr4                 ; n
            mov.b   #0x40,&P1OUT            ; y, reset LEDs state
            clr.w   &pwm_cnt                ; clear counter
            bic.w   #LPM0,0(SP)             ; wakeup processor

TA_isr4:    reti                            ; return from interrupt
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int08"
            .word   TA_isr                  ; timerA isr
            .sect   ".reset"
            .short  START                  ; PUC reset
            .end


