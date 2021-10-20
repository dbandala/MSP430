//MODO DE MUESTREO SIMPLE SOBRE EL CANAL A11 (AVcc/2) CON REFERENCIA A VREF DE 1.5V.
//SE INICIA UNA CONVERSION, EL OSCILADOR INTERNO DEL ADC MUESTREA EN MODO (16X)
//SI (AVcc/2)<0311h INDICA QUE AVcc ES MENOS QUE 2.3, INDICANDO EN EL LED LA CONDICION DE
//BATERIA BAJA
#include <msp430.h> 


void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC10CTL1 = INCH_11;                      // AVcc/2
  ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON;
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  for (;;)
  {
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    while (ADC10CTL1 & ADC10BUSY);          // ADC10BUSY?
    if (ADC10MEM < 0x311)                   // ADC10MEM = A11 > 0.65?
      P1OUT |= 0x01;                        // Set P1.0 LED on
    else
      P1OUT &= ~0x01;                       // Clear P1.0 LED off
  }
}
