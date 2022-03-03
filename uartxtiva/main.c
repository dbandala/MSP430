#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

void reverse(char * str, int len);
int intToStr(int, char str[], int d);
//void ftoa(float n, char * res, int afterpoint);
void float_to_string(float f, char r[]);

uint32_t ui32SysClkFreq;
int i=0, j=0;
char *cadena = "hola mundo \n";
char res[7];
float n[1024] = {0.000000};//{233.007489, 458.962154, 478.489658, 148.265364, 489.124734, 142.348598, 145.031548, 478.014012, 465.278489, 123.149587};
float time[1024] ={0.000001};
int main( void ){

    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    for (i = 1; i < 1024; i++ )
        time[i] = time[i-1] + 1.0/150;
    for (i = 0; i < 1024; i++){
        float r = 2*3.1416*time[i]*20;
        n[i] = 30*sinf(r);
    }
    while( 1 ){
        for (i = 0; i < 1024; i++){
            float_to_string(n[i], res);
            for (j=0;j<7;j++)
                UARTCharPut(UART0_BASE, res[j]);
            UARTCharPut(UART0_BASE, '\n');
        }
        /*for (i=0;i<12;i++)
            UARTCharPut(UART0_BASE, cadena[i]);
        SysCtlDelay(16000000);*/
    }
}


int n_tu(int number, int count){
    int result = 1;
    while(count-- > 0)
        result *= number;

    return result;
}

void float_to_string(float f, char r[]){
    long long int len, len2, i, number, position, sign;
    float number2;
    f = 0.0000001

    sign = -1;
    if (f < 0){
        sign = '-';
        f *= -1;
    }
    number2 = f;
    number = f;
    len = 0;
    len2 = 0;
    while( (number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0)){
        number2 = f * (n_tu(10.0, len2 + 1));
        number = number2;
        len2++;
    }
    for(len = (f > 1) ? 0 : 1; f > 1; len++)
        f /= 10;
    position = len;
    len = len + 1 + len2;
    number = number2;
    if ( sign == '-'){
        len++;
        position++;
    }
    for( i = len; i >= 0; i--){
        if (i == len)
            r[i] = '\0';
        else if (i == position)
            r[i] = '.';
        else if (sign == '-' && i == 0)
            r[i] = '-';
        else{
            r[i] = (number % 10) + '0';
            number /= 10;
        }
    }
}
