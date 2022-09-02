//ISR POR MEDIO DEL P1.4 EN MODO LPM4
//--------------------------------------------------------------------------
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |                 |
//      --o--|P1.4         P1.0|-->LED
//     \|/
//  ACLK = n/a, MCLK = SMCLK = default DCO
#include <msp430.h> 
//--------------------------------------------------------------------------
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // DETIENE EL WDT
  P1DIR |= 0x01;                            // P1.0 COMO SALIDA AL LED
  P1REN |= 0X08;                            // HABILITA LA RESISTENCIA DE PULL-UP
  P1IE  |= 0x08;                            // P1.4 HABILITACIÓN DE INTERRUPCIÓN
  P1IES |= 0x08;                            // P1.4 TRANSICIÓN DE ALTO A BAJO
  P1IFG &= ~0x08;                           // P1.4 BANDERA DE INTERRUPCIÓN=0
  //--------------------------------------------------------------------------
  _BIS_SR(LPM4_bits + GIE);                 // ENTRA EN MODO LPM4
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  P1OUT ^= 0x01;                            // P1.0 = toggle
  P1IFG &= ~0x08;                           // P1.4 BANDERA DE INTERRUPCIÓN=0
}
//--------------------------------------------------------------------------
