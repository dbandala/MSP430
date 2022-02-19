#include <msp430.h> 

void Conf_Puertos(void);
#define SEG_A BIT0
#define SEG_B BIT1
#define SEG_C BIT2
#define SEG_D BIT3
#define SEG_E BIT4
#define SEG_F BIT5
#define SEG_G BIT6
#define SEG_DP BIT7
#define PSEG P1OUT
#define DISP1 BIT0
#define DISP2 BIT1
#define DISP3 BIT2
#define DISP4 BIT3
#define DISP5 BIT4
#define DISP6 BIT5
#define PDISP P2OUT
//defincion de la tabla de verdad para un display de anodo comun
#define SEG_CL (0XFF) //apaga todos los leds
#define SEG_N0 ~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)
#define SEG_N1 ~(SEG_B|SEG_C)
#define SEG_N2 ~(SEG_A|SEG_B|SEG_D|SEG_E|SEG_G)
#define SEG_N3 ~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_G)
#define SEG_N4 ~(SEG_B|SEG_C|SEG_F|SEG_G)
#define SEG_N5 ~(SEG_A|SEG_C|SEG_D|SEG_F|SEG_G)
#define SEG_N6 ~(SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define SEG_N7 ~(SEG_A|SEG_B|SEG_C)
#define SEG_N8 ~(SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define SEG_N9 ~(SEG_A|SEG_B|SEG_C|SEG_F|SEG_G)

unsigned int numero=0;
unsigned int i=0;
unsigned int j=0;
unsigned int configura=0;
unsigned long segundos, minutos, horas, dig_1, dig_2, dig_3, dig_4, dig_5, dig_6;
unsigned long aux=0;
unsigned long contador=0x00;
unsigned int retardo=0;
const unsigned char codificacion[10] =
{
    SEG_N0,SEG_N1,SEG_N2,SEG_N3,SEG_N4,SEG_N5,SEG_N6,SEG_N7,SEG_N8,SEG_N9
};
const unsigned char multiplexor[6] =
{
     DISP1,DISP2,DISP3,DISP4,DISP5,DISP6
};

void Conf_Puertos(void)
{
   P1OUT = 0xFF; //se apagan todos los displays
   P1DIR = 0xFF; //se declara todos el puerto 1 como salida
   P1REN = 0; //desactiva las resistencias internas en las entradas del puerto
   P1DIR &= ~BIT7;  //se declara el puerto p1.7 como entrada
   P1REN &= ~BIT7;  //se deshabilita la resistencia interna de p1.7
   P1IES &= ~BIT7;
   P1IE |= BIT7;
   P1IFG &= ~BIT7;

   P2OUT = 0x00; //se apagan todos los displays
   P2DIR = BIT0|BIT1|BIT2|BIT3|BIT4|BIT5; //se declaran los pines de salida
   P2REN = 0; //desactiva las resistencias internas en las entradas del puerto
}

void Muestra_Displays(void)
{
    dig_1 = contador&0xF;
	dig_2 = contador&0xF0;
	dig_3 = contador&0xF00;
	dig_4 = contador&0xF000;
	dig_5 = contador&0xF0000;
	dig_6 = contador&0xF00000;
	segundos = contador&0xFF;
	minutos = contador&0xFF00;
	horas = contador&0xFF0000;
	if (dig_1 == 0xA)
	{
	   contador=contador+0x6;
	}
	else if (dig_2 == 0xA0)
	{
	    contador=contador+0x60;
	}
	else if (dig_3 == 0xA00)
	{
	    contador=contador+0x600;
	}
	else if (dig_4 == 0xA000)
	{
	    contador=contador+0x6000;
	}
	else if (dig_5 == 0xA0000)
	{
	    contador=contador+0x60000;
	}
	else if (dig_6 == 0xA00000)
	{
	    contador=contador+0x600000;
	}
    if (segundos==0x60 | segundos>0x60)
    {
       contador=contador+0xA0;
    }
    else if (minutos==0x6000 | minutos>0x6000)
    {
        contador=contador+0xA000;
    }
    else if (horas==0x240000 | horas>0x240000)
    {
        contador=0x00;
    }
    aux=contador;
    for(i=0;i<6;i++) //ciclo necesario para imprimir a los 6 display
    {
        for(j=0;j<150;j++);//retardo necesario para evitar interferencia entre dsplay
        PDISP=multiplexor[i];//selecciona cual Display se va a modificar o visualizar
        numero=aux&0xF; //obtiene el siguiente niple de menor peso
        PSEG=codificacion[numero];//manda la codificacion del numero
        aux>>=4; //obtiene el siguiente niple
    }

}

void Muestra_Displays_Configuracion(int indicador)
{
    dig_1 = contador&0xF;
    dig_2 = contador&0xF0;
    dig_3 = contador&0xF00;
    dig_4 = contador&0xF000;
    dig_5 = contador&0xF0000;
    dig_6 = contador&0xF00000;
    segundos = contador&0xFF;
    minutos = contador&0xFF00;
    horas = contador&0xFF0000;
    if (dig_1 == 0xA)
    {
       contador=contador+0x6;
    }
    else if (dig_2 == 0xA0)
    {
        contador=contador+0x60;
    }
    else if (dig_3 == 0xA00)
    {
        contador=contador+0x600;
    }
    else if (dig_4 == 0xA000)
    {
        contador=contador+0x6000;
    }
    else if (dig_5 == 0xA0000)
    {
        contador=contador+0x60000;
    }
    else if (dig_6 == 0xA00000)
    {
        contador=contador+0x600000;
    }
    if (segundos==0x60 | segundos>0x60)
    {
       contador=contador+0xA0;
    }
    else if (minutos==0x6000 | minutos>0x6000)
    {
        contador=contador+0xA000;
    }
    else if (horas==0x240000 | horas>0x240000)
    {
        contador=0x00;
    }
    aux=contador;
    for(i=0;i<6;i++) //ciclo necesario para imprimir a los 6 display
    {
        for(j=0;j<150;j++);//retardo necesario para evitar interferencia entre dsplay
        if (i==indicador){}
        else
        {
            PDISP=multiplexor[i];//selecciona cual Display se va a modificar o visualizar
            numero=aux&0xF; //obtiene el siguiente niple de menor peso
            PSEG=codificacion[numero];//manda la codificacion del numero

        }
        aux>>=4; //obtiene el siguiente niple
    }

}

void Configura_Reloj(void)
{
    _BIC_SR(GIE);
    unsigned int flag = 0;
    unsigned int flag_aux=0;
    unsigned int boton = 0;
    int dig=0;
    unsigned int toogle_retardo=0;
    unsigned int cnt_aux = 0;
    contador = 0x00;
    flag_aux=0;
    while(flag_aux==0)
    {
        boton = P1IN & 0x80;
        if (boton==0)
        {
            flag_aux=1;
        }
     }
    for (dig=0;dig<6;dig++)
    {
        while(flag_aux==0)
        {
            boton = P1IN & 0x80;
            if (boton==0)
            {
                flag_aux=1;
            }
            PDISP = 0x00;
        }
        flag=0;
        while(flag==0)
        {
            cnt_aux=0;
            boton = P1IN & 0x80;
            while(boton==0X80 & cnt_aux<70000)
            {
                boton = P1IN & 0x80;
                cnt_aux++;
                PDISP = 0x00;
            }
            if (cnt_aux<25000 & cnt_aux>3000)
            {
                if (dig==0)
                    contador = contador+0x01;
                else if (dig==1)
                    contador = contador+0x10;
                else if (dig==2)
                    contador = contador+0x100;
                else if (dig==3)
                    contador = contador+0x1000;
                else if (dig==4)
                    contador = contador+0x10000;
                else if (dig==5)
                    contador = contador+0x100000;
            }
            else if (cnt_aux>25000 | cnt_aux==25000)
            {
                flag=1;
            }
            toogle_retardo++;
            if (toogle_retardo<50)
            {
                Muestra_Displays_Configuracion(dig);
            }
            else if (toogle_retardo>49 & toogle_retardo<100 )
            {
                Muestra_Displays();
            }
            else
            {
                toogle_retardo=0x00;
            }


        }
        while(flag_aux==0)
        {
            boton = P1IN & 0x80;
            if (boton==0)
            {
                flag_aux=1;
            }
            PDISP = 0x00;
         }
    }
    flag_aux=0;
    while(flag_aux==0)
    {
        boton = P1IN & 0x80;
        if (boton==0)
        {
            flag_aux=1;
        }
     }
    _BIS_SR(GIE);
}

int main(void)
{
    BCSCTL1 |= DIVA_0;                        // DIVISOR DE RELOJ ACLK/1
    BCSCTL3 |= LFXT1S_0;                      // ASIGNACION DE ACLK = VLO
    WDTCTL = WDT_ADLY_1000;                   // INTERVALO DE INTERRUPCIÓN DE WDT
    BCSCTL2 |= DIVM_0;                        // DIVISOR DE RELOJ DCO
    IE1 |= WDTIE;                             // HABILITA INTERRUPCIÓN DE WDT
    //TACCTL0= CCIE;
    //TACCR0 = 1;
    //TACTL = TASSEL_1 + MC_1 + ID_0;
    Conf_Puertos();
    _BIS_SR(GIE);
	while(1)
	{
	    if (configura==1)
	    {
	        Configura_Reloj();
	        configura=0;
	    }
	    Muestra_Displays();
	    //_BIS_SR(LPM3_bits + GIE);

	}
}

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    contador++;
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    _BIC_SR_IRQ(LPM3_bits);
}
#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    configura=1;
    P1IFG &= ~BIT7;
}
