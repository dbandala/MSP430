//EL uC TRABAJA EN MODO DE BAJO CONSUMO LPM3, ENCENDIENDO Y APAGANDO
//EL PUERTO P1.0 CADA 6 SEGUNDOS. EL WDT ES USADO PARA DESPERTAR EL SISTEMA
//TODOS LOS PUERTOS SE CONFIGURAN COMO SALIDAS PARA ELIMINAR LAS ENTRADAS
//FLOTANTES.
//ACLK = VLO/2, MCLK = SMCLK = default DCO



//         ---------------
//     /|\|            XIN|-
//      | |               |
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
#include <msp430.h> 


void main(void)
{
  BCSCTL1 |= DIVA_1;                        // DIVISOR DE RELOJ ACLK/2
  BCSCTL3 |= LFXT1S_0;                      // ASIGNACION DE ACLK = VLO
  WDTCTL = WDT_ADLY_1000;                   // INTERVALO DE INTERRUPCIÓN DE WDT
  BCSCTL2 |= DIVM_0;                        // DIVISOR DE RELOJ DCO
  IE1 |= WDTIE;                             // HABILITA INTERRUPCIÓN DE WDT
  P1DIR = 0xFF;                             // TODOS LOS P1.x COMO SALIDAS
  P1OUT = 0;                                // TODOS LOS P1.x A CERO
  P2DIR = 0xFF;                             // TODOS LOS P2.x COMO SALIDAS
  P2OUT = 0;                                // TODOS LOS P2.x A CERO

  while(1)
  {
    int i;
    P1OUT |= 0x01;                          // ENCIENDE P1.0 LED
    //for (i = 10000; i>0; i--);              // RETARDO
    P1OUT &= ~0x01;                         // APAGA P1.0 LED
    _BIS_SR(LPM3_bits + GIE);               // ENTRA A MODO LPM3
  }
}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
  _BIC_SR_IRQ(LPM3_bits);                   // LIMPIA LPM3 DE 0(SR)
}


//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
//#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL)                         /* 32ms interval (default) */#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS0)                  /* 8ms     " */
//#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1)                  /* 0.5ms   " */
//#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1+WDTIS0)           /* 0.064ms " */
/* WDT is clocked by fACLK (assumed 32KHz) */
//#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL)                 /* 1000ms  " */
//#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0)          /* 250ms   " */
//#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1)          /* 16ms    " */
//#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)   /* 1.9ms   " */
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
