//SE REALIZA UNA MUESTRA CON A1 CONSIDERANDO LA REFERENCIA EN 1.5V Vref.
// EL uC ENTRA EN MODO DE BAJO CONSUMO Y ESPERA A QUE LA CONVERSION SE REALICE
// TRABAJA CON EL OSCILADOR DEL ADC10 CON UN TIEMPO DE MUESTREO DE 16
// SI A1>0.2V, PI.0 ENCIENDE, DE OTRA FORMA PERMANECE APAGADO.
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        >---|P1.1/A1      P1.0|-->LED
//
#include <msp430.h> 

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                                      // DETIENE AL WDT
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE;
  __enable_interrupt();                                          // HABILITA LAS INTERRUPCIONES
  TACCR0 = 30;                                                   // RETARDO PARA PERMITIR QUE REF SE ESTABILICE
  TACCTL0 |= CCIE;                                               // INTERRUPCION DEL COMPARADOR
  TACTL = TASSEL_2 | MC_1;                                       // TACLK = SMCLK, Up mode., FUENTE DE RELOJ DEL TIMER Y MODO DE CONTEO
  LPM0;                                                          // ESPERA MODO LPM0
  TACCTL0 &= ~CCIE;                                              // LIMPIA LA BANDERA DE INTERRUPCION DEL COMPARADOR
  __disable_interrupt();                                         // DESHABILITA INTERRUPCIONES
//'---------------------------------------------------------------------------------------------------------------
  ADC10CTL1 = INCH_1;                                            // ENTRADA ANALOGICA EN A1
  ADC10AE0 |= 0x02;                                              // ENTRADA ANALOGICA EN A1
  P1DIR |= 0x01;                                                 // P1.0 COMO SALIDA A LED
  //'---------------------------------------------------------------------------------------------------------------
  for (;;)
  {
    ADC10CTL0 |= ENC + ADC10SC;                                  // INICIO DE S+H
    __bis_SR_register(CPUOFF + GIE);                             // MODO LPM0, ADC10_ISR FORZA A SALIR DE MODO
    if (ADC10MEM < 0x88)                                         // ADC10MEM = A1 > 0.2V?
      P1OUT &= ~0x01;                                            // APAGA P1.0 LED
    else
      P1OUT |= 0x01;                                             // PRENDE P1.0 LED
  }
}
//'---------------------------------------------------------------------------------------------------------------
// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);                            // REGRESA A MODO ACTIVO AL uC
}
//'---------------------------------------------------------------------------------------------------------------

//'---------------------------------------------------------------------------------------------------------------
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  TACTL = 0;
  LPM0_EXIT;                                                    // REGRESA A MODO ACTIVO AL uC
}
//'---------------------------------------------------------------------------------------------------------------
