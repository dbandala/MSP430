;Event Driven Programming Example
;-------------------------------------------------------------------------------
; MSP430 Assembler Code Template for use with TI Code Composer Studio
;-------------------------------------------------------------------------------
            .cdecls C,LIST,"msp430.h"       ; Include device header file
;-------------------------------------------------------------------------------
TA_CTL      .equ    TASSEL_2|ID_3|MC_1|TAIE ; SMCLK,/8,UP,IE
TA_FREQ     .equ    0xFFFF                  ; FRECUENCIA DEL timerA
WDT_CPS     .equ    1200000/7692           ; WDT clocks/second
;-------------------------------------------------------------------------------
WDT_EVENT   .equ    0x0001                  ; EVENTO DEL WDT
TA_EVENT    .equ    0x0002                  ; EVENTO DEL timerA
;-------------------------------------------------------------------------------
            .def    RESET                   ;
;-------------------------------------------------------------------------------
;SECCIÓN DE DATOS
;-------------------------------------------------------------------------------
            .bss    WDTSecCnt,2             ; CONTADOR PARA EL WDT
            .bss    sys_event,2             ; VARIABLE PARA EVENTOS
;-------------------------------------------------------------------------------
;SECCIÓN DE CÓDIGO
;-------------------------------------------------------------------------------
            .text                           ; DIRECTIVA DE INICIO DE INSTRUCCIONES DE ENSAMBLADOR
;-------------------------------------------------------------------------------
; Main loop
;-------------------------------------------------------------------------------
RESET:
START:      mov.w   #__STACK_END,SP         ; INICIO DE LA PILA
            mov.w   #WDT_MDLY_32,&WDTCTL    ; INTERVALO DEL WDT
            mov.w   #WDT_CPS,&WDTSecCnt     ; NUMERO DE VECES A EJECUTAR PARA ACUMULAR 1s EN EL WDT
            bis.b   #WDTIE,&IE1             ; HABILITA LA INTERRUPCIÓN DEL WDT
            clr.w   &TAR                    ; LIMPIA EL REGISTRO DEL TIMER_A
            mov.w   #TA_CTL,&TACTL          ; SE CARGA EL VALOR DE TA_CTL EN EL REGISTRO TACTL (timerA control register)
            mov.w   #TA_FREQ,&TACCR0        ; SE CARGA EL VALOR DE TTA_FREQ EN EL REGISTRO TACCR0 (interval frequency)
            mov.w	#0x00,&P1OUT			; LIMPIA EL PUERTO P1
            bis.b   #0x41,&P1DIR            ; CONFIGURA P1.0 Y P1.6 COMO SALIDAS
            clr.w   &sys_event              ; LIMPIA EL REGISTRO DE EVENTOS
;-------------------------------------------------------------------------------
; Event Loop
;-------------------------------------------------------------------------------
loop:       bic.w   #GIE,SR                 ; DESABILITA LAS INTERRUPCIONES
            cmp.w   #0,&sys_event           ; VERIFICA SI HAY ALGUNA INTERRUPCION PENDIENTE
              jne   loop02                  ; SI
            bis.w   #GIE|LPM0,SR            ; NO, MODO LPM0 DE BAJO CONSUMO (SLEEP)
;-------------------------------------------------------------------------------
loop02:     cmp.w   #WDT_EVENT,&sys_event   ; EL EVENTO ES DEL WDT?
              jne   loop04                  ; NO
            bic.w   #WDT_EVENT,&sys_event   ; SI, LIMPIAR EVENTO
            xor.b   #0x01,&P1OUT            ; TOGGLEA EL LED ROJO
;-------------------------------------------------------------------------------
loop04:     cmp.w   #TA_EVENT,&sys_event    ; EL EVENTO ES DEL TIMER_A?
              jne   loop06                  ; NO
            bic.w   #TA_EVENT,&sys_event    ; SI, LIMPIA EL EVENTO
            xor.b   #0x40,&P1OUT            ; TOGGLEA EL LED VERDE
;-------------------------------------------------------------------------------
loop06:
;           << PROCESOS DE OTROS EVENTOS ACÁ>>
            jmp     loop                    ; LAZO INFINITO
;-------------------------------------------------------------------------------
; Watchdog ISR
;-------------------------------------------------------------------------------
WDT_ISR:    dec.w   &WDTSecCnt              ; SE TEMPORIZO UN SEGUNDO?
              jne   WDT_02                  ; NO
            mov.w   #WDT_CPS,&WDTSecCnt     ; SI, REINICIA EL CONTADOR
            bis.w   #WDT_EVENT,&sys_event   ; AGENDA EL EVENTO DEL WDT
            bic.w   #GIE|LPM0,0(SP)         ; DESPIERTA EL PROCESADOR
WDT_02:     reti                            ; SALE DE LA WDT_ISR
;-------------------------------------------------------------------------------
; Timer A ISR
;-------------------------------------------------------------------------------
TA_ISR:     bic.w   #TAIFG,&TACTL           ; DESHABILITA LA BANDERA DE INTERRUPCIÓN POR TIMER_A
            bis.w   #TA_EVENT,&sys_event    ; AGENDA EL EVENTO DEL TIMER_A
            bic.w   #GIE|LPM0,0(SP)         ; DESPIERTA EL PROCESADOR
            reti
;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack
;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".int08"                ; VECTOR DEL TIMER_A
            .word   TA_ISR                  ; TIMER_A ISR
            .sect   ".int10"                ; VECTOR DEL WDT
            .word   WDT_ISR                 ; WDT ISR
            .sect   ".reset"                ; VECTOR DE PUC
            .word   START                   ; RESET ISR
            .end
;-------------------------------------------------------------------------------

