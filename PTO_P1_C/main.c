//P1.0 CONFIGURADO COMO SALIDA, LED; P1.4 COMO ENTRADA
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |                 |
//      --o--|P1.4         P1.0|-->LED
//     \|/
#include <msp430.h> 


void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // DETIENE EL WDT
  P1DIR |= 0x01;                            // P1.0 COMO SALIDA AL LED
  P1OUT |= 0X08;                            // HABILITA LA RESISTENCIA DE PULL-UP
  P1REN |= 0X08;                            // HABILITA LA RESISTENCIA DE PULL-UP

  while (1)                                 // PRUEBA EN P1.4
  {
    if ((0x08 & P1IN))
        P1OUT |=  0x01;                      // SI P1.4=1, PRENDE P1.0
    else
        P1OUT &= ~0x01;                     // SI NO APAGA P1.0
  }
}
