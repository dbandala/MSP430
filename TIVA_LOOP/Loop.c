// ****** Librerias ******
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"


// ****** Variables ******
volatile float h;
volatile int flat;
volatile uint32_t y[3];
//uint32_t fant=0;
// ****** Constants ******
const int sampleFreq = 10000; // Sample Frequency

void envia(uint32_t x){
    SSIDataPut(SSI0_BASE, x);
    while(SSIBusy(SSI0_BASE)){ }
}

int main(void) {
	//====================================================================================================================
	//												CONFIGURACION DEL TIMER
	//====================================================================================================================
	uint32_t ui32Period;
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);//configura el reloj
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);//habilita el periferico GPIO, parpadeo azul
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3); //configural los pines de salida GPIO
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);//habilita el periferico del Timer 0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);// configura el modo de operacion del timer en periodo completo

	ui32Period = (SysCtlClockGet())/sampleFreq ; //SysCtlClockGet: determina la frecuencia del reloj del procesador
	//ui32Period = SysCtlClockGet(); //relacion de 1s=1Hz

	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period-1); //carga al temporizador el valor deseado
	//TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1); //carga al temporizador el valor deseado
	IntEnable(INT_TIMER0A);// habilita la inrrupcion para el timer 0A
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);//habilita la interrupcion para el timer A timeout
	IntMasterEnable();//permite que el procesador responda a las interrupciones

	//====================================================================================================================
	//												CONFIGURACION DEL ADC
	//====================================================================================================================
//	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ); //Establecimiento de un reloj de a 40 MHz
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);//Habilitacion del periferico ADC0

	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);//. Configurar el muestreo del ADC a 1M sps con alta prioridad

	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH1);//ADC_CTL_CHx x=0,1,2...23 puerto pin del ADC
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);//ADC_CTL_IE configura la interrupcion cuando el paso se complete,
																				//ADC_CTL_END define este como el ultimo paso

	//====================================================================================================================
	//												CONFIGURACION DEL DAC
	//====================================================================================================================
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);                                 // Enable SSI0 peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA2_SSI0CLK);                 // Enable CLK pin
	GPIOPinConfigure(GPIO_PA5_SSI0TX);                  // Enable TX pin
	GPIOPinConfigure(GPIO_PA3_SSI0FSS);                  // Enable SS pin}
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_3 | GPIO_PIN_2);      // Enable comm. pins for SSI module

    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 20000000, 16);

	//====================================================================================================================

    TimerEnable(TIMER0_BASE, TIMER_A);// Habilita el funcionamiento del timer A
	ADCSequenceEnable(ADC0_BASE, 1);// Especiifaca la secuencia de muestreo
    SSIEnable(SSI0_BASE);                               // Enable SSI0

	while(1)  {
	}
	//return 0;
}

void Timer0IntHandler(void) {//Secuencia de interrupcion
	float ui32Valor;
	uint32_t entrada; //variables
	uint32_t filtrada; //variables
	uint32_t ui32ADC0Value[4]; //matris para almacenar los datos leidos del adc
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	ADCIntClear(ADC0_BASE, 1);//limpia el registro de interrupcion
	ADCProcessorTrigger(ADC0_BASE, 1);	//activa la secuencia de muestreo
	while(!ADCIntStatus(ADC0_BASE, 1, false)) //devuelve el estado de la interrupcion que no se a procesado
	{}
	ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value); //asigna la secuencia de datos muestreados a la varaible especificada en memoria deve ser de tamaño correcto

	y[3] = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3])/4; //el mas 2 es para el redondeo


	//========================================== Filtros
		filtrada=0.01*y[3]+0.99*y[2]; //funcion 1

		y[2]=filtrada;
		y[1]=y[2];

	SSIDataPut(SSI0_BASE, (0x3000 | filtrada) & 0x3FFF); //envio de datos
	while(SSIBusy(SSI0_BASE)){ } //espera termino de envio

}

void ADC0IntHandler(void){
}
