//SE OBTIENE UN PULSO EN EL PUERTO P1.0 CON UN CICLO DE TRABAJO DE 1/100
//SE CONFIGURA EL RELOJ DE BAJA FRECUENCIA (APROX. 952.38 Hz).
//ACLK=VLO, MCLK=VLO/8, SMCLK=N/A
#include  <msp430.h>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void main(void)
//---------------------------------------------------------------------------
{
  volatile unsigned int i;                  // RESERVA DE MEMORIA RAM PARA LA VARIABLE i
  WDTCTL = WDTPW + WDTHOLD;                 // PARAR EL PERRO GUARDIAN
  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
  IFG1 &= ~OFIFG;                           // LIMPIA LA BANDERA OSCFault, EVITA LA INTERRUPCION POR FALLO DE OSC.
  __bis_SR_register(SCG1 + SCG0);           // DETIENE EL DCO
  BCSCTL2 |= SELM_3 + DIVM_3;               // ASIGNA EL RELOJ MCLK DEL LVO/8 (MCLK = LFXT1/8)
  P1DIR = 0xFF;                             // TODO EL PUERTO P1.x COMO SALIDAS
  P1OUT = 0;                                // TODO EL PUERTO P1.x LIMPIO
  P2DIR = 0xFF;                             // TODO EL PUERTO P2.x COMO SALIDAS
  P2OUT = 0;                                // TODO EL PUERTO P2.x LIMPI
 //---------------------------------------------------------------------------
  for (;;)
  {
    P1OUT |= 0x01;                          // PONE P1.0 = 1
    for (i = 10; i > 0; i--);               // Delay 1x
    P1OUT &= ~0x01;                         // PONE P1.0 = 0
    for (i = 1000; i > 0; i--);             // Delay 100x
  }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
