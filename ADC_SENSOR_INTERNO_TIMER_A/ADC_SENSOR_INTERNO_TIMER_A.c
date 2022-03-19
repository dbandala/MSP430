//EL ADC TRABAJA CON EL SENSOR INTERNO DE TERMPERATURA, DETECTA UN INCREMENTO DE
//TEMPERATURA.
//LA TEMPERATURA DEL SENSOR ES MUESTREADA APROX. CADA 120MS Y ES COMPARADO
// CON UN VALOR DELTA USANDO UNA ISR.
//LA DIVISION DE (ADC10OSC/4)/64 DETERMINA EL TIEMPO DE MUESTREO NECESARIO
//DE POCO MAS DE 30US PARA LA TEMPERATURA DEL SENSOR.
//EL ADC OPERA EN MODO DE CANAL SIMPLE DE MANERA REPETIDA Y SE REALIZA DESDE EL
//TIMER A CCR1
//  ACLK = n/a, MCLK = SMCLK = default DCO ~ 1.2MHz, ADC10CLK = ADC10OSC
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |A10          P1.0|-->LED
#include <msp430.h> 


static unsigned int FirstADCVal;            // holds 1st ADC result

#define ADCDeltaOn       3                  // ~ 2 Deg C delta for LED on

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog
  ADC10CTL1 = ADC10DIV_3 + INCH_10 + SHS_1 + CONSEQ_2;  // TA trig., rpt, A10
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REF2_5V + ADC10IE + REFON + ADC10ON;
  __enable_interrupt();                     // Enable interrupts.
  TACCR0 = 30;                              // Delay to allow Ref to settle
  TACCTL0 |= CCIE;                          // Compare-mode interrupt.
  TACTL = TASSEL_2 | MC_1;                  // TACLK = SMCLK, Up mode.
  LPM0;                                     // Wait for delay.
  TACCTL0 &= ~CCIE;                         // Disable timer Interrupt
  __disable_interrupt();
  ADC10CTL0 |= ENC;
  TACCTL1 = OUTMOD_4;                       // Toggle on EQU1 (TAR = 0)
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, cont-mode
  while (!(ADC10IFG & ADC10CTL0));          // First conversion?
  FirstADCVal = ADC10MEM;                   // Read out 1st ADC value
  P1OUT = 0x00;                             // Clear P1
  P1DIR = 0x01;                             // P1.0 as output
  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  if (ADC10MEM >= FirstADCVal + ADCDeltaOn)
    P1OUT |= 0x01;                          // LED on
  else
    P1OUT &= ~0x01;                         // LED off
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void ta0_isr(void)
{
  TACTL = 0;
  LPM0_EXIT;                                // Exit LPM0 on return
}
