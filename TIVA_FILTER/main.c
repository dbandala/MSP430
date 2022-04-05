#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "driverlib/ssi.h"
#include "driverlib/interrupt.h"
#include "fdacoefs.h"


// ****** Constantes ******
const int Fmuestreo = 44100;//44100; // Frecuencia de muestreo
// ****** Variables ******
uint32_t muestra_e, muestra_s; // muestra_e es el entero que se lee del ADC
                               // muestra_s es el entero que se env�a al DAC

uint32_t ui32SysClkFreq;



float x, y;  // a almacena la entrada actual como flotante, y es el valor flotante de la salida actual
float a1;
float a2;
float a3;
float a4;
float a5;
float a6;
float a7;
float a8;
float a9;
float a10;
float a11;
float a12;
float a13;
float a14;
float a15;
float a16;
float a17;
float a18;
float a19;
float a20;
float b0;
float b1;
float b2;
float b3;
float b4;
float b5;
float b6;
float b7;
float b8;
float b9;
float b10;
float b11;
float b12;
float b13;
float b14;
float b15;
float b16;
float b17;
float b18;
float b19;
float b20;
float xnm1=0,xnm2=0,xnm3=0,xnm4=0,xnm5=0,xnm6=0,xnm7=0,xnm8=0,xnm9=0;
float xnm10=0,xnm11=0,xnm12=0,xnm13=0,xnm14=0,xnm15=0,xnm16=0,xnm17=0,xnm18=0,xnm19=0,xnm20=0;
float ynm1=0,ynm2=0,ynm3=0,ynm4=0,ynm5=0,ynm6=0,ynm7=0,ynm8=0,ynm9=0;
float ynm10=0,ynm11=0,ynm12=0,ynm13=0,ynm14=0,ynm15=0,ynm16=0,ynm17=0,ynm18=0,ynm19=0,ynm20=0;
float xnm[17];
float ynm[17];
uint8_t i=0;
// Definitions
void systemSetup(void){

    uint16_t Pmuestreo;

    // *** Establecer el reloj del sistema a 80 MHz
    //SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    // *** Habilitaci�n de perif�ricos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // *** ADC0
    //ADCHardwareOversampleConfigure(ADC0_BASE,2);
    ADCReferenceSet(ADC0_BASE,ADC_REF_INT);
    ADCSequenceDisable(ADC0_BASE, 3);
    ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_TIMER,0);
    ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE,3);

    ADCIntClear(ADC0_BASE, 2);

    // *** Timer0
    TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
    Pmuestreo = ui32SysClkFreq /Fmuestreo;
    TimerLoadSet(TIMER0_BASE,TIMER_A,Pmuestreo - 1);
    TimerControlTrigger(TIMER0_BASE,TIMER_A,true);


    // *** GPIO
    GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);

    // *** Interrupciones
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    IntEnable(INT_ADC0SS3);
    ADCIntEnable(ADC0_BASE,3);

    // *** SSI0
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);                 // Habilita la terminal CLK
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);                  // Habilita la terminal SS
    GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);                  // Habilita la terminal TX
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_2);      // Habilita las terminales para el m�dulo SSI0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);                                 // Habilita el perif�rico SSI0
    SSIConfigSetExpClk(SSI0_BASE, ui32SysClkFreq, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 20000000, 16);
    SSIEnable(SSI0_BASE);                               // Habilita SSI0

    IntMasterEnable();
    TimerEnable(TIMER0_BASE,TIMER_A);
}

//Programa principal
int main(void) {
    systemSetup();
    /*for(i=0;i<NL;i++)
    {
    	xnm[i]=0;
    }
    for(i=0;i<DL;i++)
    {
    	ynm[i]=0;
    }*/
    a1=DEN[1];
    a2=DEN[2];
    a3=DEN[3];
    a4=DEN[4];
    a5=DEN[5];
    a6=DEN[6];
    a7=DEN[7];
    a8=DEN[8];
    a9=DEN[9];
    a10=DEN[10];
    a11=DEN[11];
    a12=DEN[12];
    a13=DEN[13];
    a14=DEN[14];
    a15=DEN[15];
    a16=DEN[16];
    a17=DEN[17];
    a18=DEN[18];
    a19=DEN[19];
    a20=DEN[20];
    b0=NUM[0];
    b1=NUM[1];
    b2=NUM[2];
    b3=NUM[3];
    b4=NUM[4];
    b5=NUM[5];
    b6=NUM[6];
    b7=NUM[7];
    b8=NUM[8];
    b9=NUM[9];
    b10=NUM[10];
    b11=NUM[11];
    b12=NUM[12];
    b13=NUM[13];
    b14=NUM[14];
    b15=NUM[15];
    b16=NUM[16];
    b17=NUM[17];
    b18=NUM[18];
    b19=NUM[19];
    b20=NUM[20];
    while(1){
             }
}

void Timer0IntHandler(void){
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    }


// Rutina de atenci�n a interrupci�n del secuenciador 3 del ADC
void ADC0SS3IntHandler(void){
    ADCIntClear(ADC0_BASE,3);
    ADCSequenceDataGet(ADC0_BASE,3,&muestra_e); // Lee el dato del ADC y lo almacena en muestra
    //****************************************************************************************
    // En esta parte se llevan a cabo las operaciones del filtro
    // Las operaciones se llevan a cabo con flotantes y se env�an al DAC como enteros

    x=(float)muestra_e;
    y=(b0*x)+(b1*xnm1)+(b2*xnm2)+(b3*xnm3)+(b4*xnm4)+(b5*xnm5)+(b6*xnm6)+(b7*xnm7)+(b8*xnm8)+(b9*xnm9)
    		+(b10*xnm10)+(b11*xnm11)+(b12*xnm12)+(b13*xnm13)+(b14*xnm14)+(b15*xnm15)+(b16*xnm16)
    		+(b17*xnm17)+(b18*xnm18)+(b19*xnm19)+(b20*xnm20)-(a1*ynm1)-(a2*ynm2)-(a3*ynm3)-(a4*ynm4)-(a5*ynm5)
    		-(a6*ynm6)-(a7*ynm7)-(a8*ynm8)-(a9*ynm9)-(a10*ynm10)-(a11*ynm11)-(a12*ynm12)-(a13*ynm13)-(a14*ynm14)-(a15*ynm15)
    		-(a16*ynm16)-(a17*ynm17)-(a18*ynm18)-(a19*ynm19)-(a20*ynm20);
    xnm20=xnm19;
    xnm19=xnm18;
    xnm18=xnm17;
    xnm17=xnm16;
    xnm16=xnm15;
    xnm15=xnm14;
    xnm14=xnm13;
    xnm13=xnm12;
    xnm12=xnm11;
    xnm11=xnm10;
    xnm10=xnm9;
    xnm9=xnm8;
    xnm8=xnm7;
    xnm7=xnm6;
    xnm6=xnm5;
    xnm5=xnm4;
    xnm4=xnm3;
    xnm3=xnm2;
    xnm2=xnm1;
    xnm1=x;
    ynm20=ynm19;
    ynm19=ynm18;
    ynm18=ynm17;
    ynm17=ynm16;
    ynm16=ynm15;
    ynm15=ynm14;
    ynm14=ynm13;
    ynm13=ynm12;
    ynm12=ynm11;
    ynm11=ynm10;
    ynm10=ynm9;
    ynm9=ynm8;
    ynm8=ynm7;
    ynm7=ynm6;
    ynm6=ynm5;
    ynm5=ynm4;
    ynm4=ynm3;
    ynm3=ynm2;
    ynm2=ynm1;
    ynm1=y;

    muestra_s = (uint32_t)y;
    //****************************************************************************************

    SSIDataPut(SSI0_BASE, (0x3000 | muestra_s) & 0x3FFF);
                while(SSIBusy(SSI0_BASE)){ }
}
