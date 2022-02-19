//OPERA AL MICROCONTROLADOR EN MODO DE BAJO CONSUMO LPM3, REALIZA
//EL APAGADO Y ENCENDIDO DEL LED CONECTADO EN EL PUERTO P1.0 CADA
//INTERVALO DE 4 SEGUNDOS. EL uC SE DESPIERTA POR LA ACCION DEL WDT.
//TODOS LOS PUERTOS SON CONFIGURADOS COMO SALIDAS.
//
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
#include <msp430.h> 


void main(void)
{
  BCSCTL1 |= DIVA_0;                        // ACLK/4
  WDTCTL = WDT_ADLY_1000;                   // WDT 1s/4 interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P1DIR = 0xFF;                             // All P1.x outputs
  P1OUT = 0;                                // All P1.x reset
  P2DIR = 0xFF;                             // All P2.x outputs
  P2OUT = 0;                                // All P2.x reset
  _BIS_SR(GIE);

  while(1)
  {
int i;
    P1OUT |= 0x01;                          // Set P1.0 LED on
    for (i = 50; i>0; i--);                 // Delay
    P1OUT &= ~0x01;                         // Reset P1.0 LED off
    _BIS_SR(LPM3_bits + GIE);               // Enter LPM3
  }
}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
    _BIC_SR_IRQ(LPM3_bits);                 // Clear LPM3 bits from 0(SR)
}

