// ACLK EN PUERTO P1.0, SMCLK(DCO) DEFAULT EN P1.4 Y MCLK/10 EN P1.1
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |       P1.4/SMCLK|-->SMCLK = Default DCO
//            |             P1.1|-->MCLK/10 = DCO/10
//            |        P1.0/ACLK|-->ACLK = 32kHz
//ACLK = LFXT1 = 32768, MCLK = SMCLK = default DCO
#include  <msp430.h>

void main(void)
{
  WDTCTL = WDTPW +WDTHOLD;                  // DETIENE AL PERRO GUARDIAN
  P1DIR = 0x13;                            // P1.0, P1.1 Y P1.4 CONFIGURADO COMO SALIDAS
  P1SEL = 0x11;                            // P1.0 ACLK DE CRISTAL, P1.4 SALIDA DEL SMCLK (DCO)

  while(1)
  {                                         // EN LA EJECUCION DE ESTA PARTE DE CÓDIGO SE EMPLEA EL DCO/10
    P1OUT |= 0x02;                          // OPERACION A NIVEL DE BIT ENTRE EL VALOR DE P1OUT Y 0X02: P1.1 = 1
    P1OUT &= ~0x02;                         // OPERACION A NIVEL DE BIT ENTRE EL VALOR DE P1OUT Y EL COMPLEMENTO A UNO DE 0X02: P1.1 = 0
  }                                         // EL VALOR DE FREC. DE SALIDA DE ESTE PIN, DADO QUE SOLO EMPLEA UN CICLO DE RELOJ SERÁ EL DEL DCO/10
}
