// CANAL ADC1 CON AVcc COMO REFERENCIA DE VOLTAJE
// EL LAZO PRINCIPAL DEL PROGRAMA ENTRA EN MODO LPM0 HASTA QUE EL ADC
// LE INDICA POR MEDIO DE UNA INTERRUPCION QUE LA CONVERSION SE HA COMPLETADO
// LA INTERRUPCION FORZA AL MICROCONTROLADOR A SALIR DE LPM0
// EL ADC TRABAJA CON SU OSCILADOR
// SI A1>0.5*AVcc, P1.0 ENCIENDE, SI NO PERMANECE APAGADO

//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        >---|P1.1/A1      P1.0|-->LED

#include <msp430.h> 

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                     // DETIENE AL PERRO GUARDIAN
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;   // ADC10ON, HABILITA INTERRUPCION
  ADC10CTL1 = INCH_1;                           // A1 COMO ENTRADA ANALOGICA
  ADC10AE0 |= 0x02;                             // PA.1 ADC SELECCION DEL CANAL
  P1DIR |= 0x01;                                // P1.0 COMO SALIDA, PARA EL LED

  for (;;)
  {
    ADC10CTL0 |= ENC + ADC10SC;                 // INICIO DE MUESTREO Y CONVERSION
    __bis_SR_register(CPUOFF + GIE);            // ENTRA AL MODO LPM0 PARA DEJAR SOLO AL ADC
    if (ADC10MEM < 0x1FF)
      P1OUT &= ~0x01;                           // P1.0 LED off
    else
      P1OUT |= 0x01;                            // P1.0 LED on
  }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // FORZA AL uC A SALIR DE LPM0
}
