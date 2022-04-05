//TOGGLEA EL LED CONECTADO AL PUERTO P1.0 DENTRO DEL CICLO FOR.
// ACLK = n/a, MCLK = SMCLK = default DCO
#include <msp430.h> 


void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // PARA EL PERRO GUARDIAN
  P1DIR |= 0x01;                            // CONFIGURA COMO SALIDA EL PUERTO P1.0

  for (;;)
  {
    volatile unsigned int i;

    P1OUT ^= 0x01;                          // TOGGLEA EL LED CONECTADO AL PUERTO P1.0 USANDO OR-EXCLUSIVO

    i = 50000;                              // RETARDO
    do (i--);
    while (i != 0);
  }
}
