// Ejemplo de un programa que toma muestras con el ADC y lo manda por el puerto serie

//*****************************************************************************
//
// Single _ terminado. C - ejemplo de cómo configurar el adc para
// una operación única.
//
// Derechos de autor (c) 2010-2015 instrumentos de Texas incorporados. Todos los derechos reservados.
// acuerdo de licencia de software
//
// Redistribución y uso en formas de origen y binarios, con o sin
// modificación, se permite siempre que las siguientes condiciones
// se cumplen:
//
// redistribuciones del código fuente debe conservar los derechos de autor arriba mencionados
// AVISO, esta lista de condiciones y el siguiente descargo de responsabilidad.
//
// redistribuciones en forma binaria debe reproducir los derechos de autor arriba mencionados
// AVISO, esta lista de condiciones y el siguiente descargo de responsabilidad en el
// documentación y / u otros materiales proporcionados con el
// Distribución.
//
// ni el nombre de Texas instruments incorporated ni los nombres de
// sus colaboradores pueden ser utilizados para apoyar o promocionar productos derivados
// de este software sin autorización escrita previa.
//
// este software es proporcionado por los titulares de derechos de autor y colaboradores
// como es " y cualquier garantía expresa o implícita, incluyendo, pero no
// limitado a, las garantías implícitas de comerciabilidad y aptitud para
// se descargo un propósito particular. En ningún caso los derechos de autor
// el propietario o los contribuyentes serán responsables de cualquier directa, indirecta, incidental,
// daños especiales, ejemplares o consecuentes (incluyendo, pero no
// Limitada a la adquisición de bienes o servicios sustitutivos; pérdida de uso,
// Datos, o beneficios; o interrupción de negocios) sin embargo causado y en cualquier
// Teoría de la responsabilidad, ya sea en contrato, responsabilidad objetiva o agravio
// (incluida la negligencia o de otro modo) que surjan en cualquier forma fuera del uso
// de este software, incluso si se aconseja la posibilidad de tal daño.
//
// esto es parte de la revisión 2.1.2.111 del paquete de desarrollo de firmware reanimación.
//
//*****************************************************************************

#Incluye <stdbool. H>
#include <stdint.h>
#Incluye " Inc / HW _ Memap. H "
#Incluye " Driverlib / ADC. H "
#Incluye " Driverlib / gpio. H "
#Incluye " Driverlib / pin _ MAP H "
#Incluye " Driverlib / sysctl. H "
#Incluye " Driverlib / uart. H "
#Include " Utils / uartstdio. H "

//*****************************************************************************
//
//! \ addtogroup adc _ ejemplos _ list
//! <h1>single terminó adc (single _ finalizado) < / h1>
//!
//! este ejemplo muestra cómo configurar adc0 como una sola entrada final y tomar un
//! una muestra única en ain0 / pe7.
//!
//! este ejemplo usa los siguientes periféricos y señales de e / s. Usted debe
//! revise estos y cambie según sea necesario para su propio consejo:
//!-Periférico
//!-Gpio Puerto E Periférico (para ain0 pin)
//! - AIN0 - PE7
//!
//! las siguientes señales uart están configuradas sólo para mostrar consola
//! mensajes para este ejemplo. Estos no son necesarios para el funcionamiento del
//! ADC.
//!-Periférico
//!-Puerto de gpio un periférico (para alfileres uart0)
//! - UART0RX - PA0
- Uart0tx //!- Juntos.
//!
//! este ejemplo usa los siguientes controladores de interrupción. Para usar este ejemplo
//! en tu propia aplicación debes agregar a estos controladores de interrupción a tu
//! tabla vectorial.
- Ninguno.
//
//*****************************************************************************

//*****************************************************************************
//
// esta función configura uart0 para ser usado para una consola para mostrar información
// como el ejemplo está corriendo.
//
//*****************************************************************************
Vacío
Initconsole (vacío)
{
//
// habilitar el puerto de gpio a que se utiliza para los pines uart0.
// Todo: cambia esto a cualquier puerto gpio que estés usando.
//
Sysctlperipheralenable (sysctl _. _ Gpioa);

//
// configure el pin muxing para las funciones uart0 en el puerto a0 y a1.
// este paso no es necesario si su parte no apoya a pin muxing.
// Todo: cambia esto para seleccionar el puerto / pin que estás usando.
//
Gpiopinconfigure (gpio _ pa0 _ u0rx);
Gpiopinconfigure (gpio _ de _ u0tx);

//
// habilitar uart0 para que podamos configurar el reloj.
//
Sysctlperipheralenable (sysctl _. _ uart0)

//
// utilice el oscilador interno de 16 Mhz como la fuente del reloj uart.
//
Uartclocksourceset (uart0 _ base, uart _ clock _ Piosc);

//
// seleccione la función alternativa (uart) para estos pines.
// Todo: cambia esto para seleccionar el puerto / pin que estás usando.
//
Gpiopintypeuart (gpio _ Porta _ base, gpio _ PIN _ 0 | Gpio _ PIN _ 1);

//
// Inicializar el uart para la consola i / o.
//
UARTStdioConfig(0, 115200, 16000000);
}

//*****************************************************************************
//
// configurar adc0 para una entrada única y una sola muestra. Una vez
// la muestra está lista, se pondrá una bandera de interrupción. Utilizando un método de votación,
// los datos se mostrarán luego en la consola a través de uart0.
//
//*****************************************************************************
Int
Principal (vacío)
{
/* #si se define (objetivo _ es _ tm4c129 _ Ra0) || \
Definido (objetivo _ es _ tm4c129 _ Ta1) || \
Definido (objetivo _ es _ tm4c129 _ Ra2)
Uint32 _ t ui32sysclock;
#endif */

//
// esta matriz se utiliza para almacenar los datos leídos desde el adc fifo. It
// debe ser tan grande como el fifo para el secuenciador en uso. Este ejemplo
// USA LA SECUENCIA 3 que tiene una profundidad fifo de 1. Si otra secuencia
// fue usado con un fifo más profundo, entonces el tamaño de la matriz debe ser cambiado.
//
Uint32 _ t pui32adc0value [1];

//
// fijar el reloj para correr a 20 Mhz (200 Mhz / 10) usando el pll. Cuando
// usando el adc, usted debe usar el pll o suministrar un reloj de 16 Mhz
// Fuente.
// Todo: el sysctl _ Xtal _ valor debe ser cambiado para coincidir con el valor del
// Cristal en tu pizarra.
//
/* #si se define (objetivo _ es _ tm4c129 _ Ra0) || \
Definido (objetivo _ es _ tm4c129 _ Ta1) || \
Definido (objetivo _ es _ tm4c129 _ Ra2)
Ui32sysclock = Sysctlclockfreqset (sysctl _ Xtal _ 25 Mhz |
Sysctl _ OSC _ Main |
Sysctl _ USA _ PLL |
Sysctl _ cfg _ Vco _ 480), 20000000
#else */
Sysctlclockset (sysctl _ sysdiv _ 10 | Sysctl _ uso _ PLL | Sysctl _ OSC _ Main |
SYSCTL_XTAL_16MHZ);
// #endif

//
// configurar la consola serial para usar para mostrar mensajes. Esto es
// solo para este programa de ejemplo y no es necesario para la operación adc.
//
Initconsole ();

//
// muestra la configuración en la consola.
//
Uartprintf ("ADC -> \ n");
Uartprintf ("tipo : single final \ n");
Uartprintf ("muestras : uno \ n");
Uartprintf ("tasa de actualización : 250 MS \ n");
Uartprintf ("pin de entrada : Ain0 / pe7 \ n");

//
// el periférico periférico debe estar habilitado para su uso.
//
Sysctlperipheralenable (sysctl _. _ adc0)

//
// para este ejemplo adc0 se utiliza con ain0 en port e7.
// el puerto y los pines reales utilizados pueden ser diferentes de tu parte, consultar
// la hoja de datos para más información. Gpio Port e tiene que ser activado
// así que estos alfileres pueden ser usados.
// Todo: cambia esto a cualquier puerto gpio que estés usando.
//
Sysctlperipheralenable (sysctl _. _ Gpioe);

//
// seleccione la función analógica adc para estos pines.
// consulte la ficha de datos para ver qué funciones se asignan por pin.
// Todo: cambia esto para seleccionar el puerto / pin que estás usando.
//
GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_7);

//
// activar la secuencia de muestreo 3 con un disparador de señal de procesador. SECUENCIA 3
// hará una sola muestra cuando el procesador envíe una señal para iniciar el
// conversión. CADA MÓDULO ADC TIENE 4 SECUENCIAS PROGRAMABLES, SECUENCIA 0
// para la secuencia 3. Este ejemplo está usando arbitrariamente la secuencia 3.
//
Adcsequenceconfigure (adc0 _ base, 3, cad _ gatillo _ procesador, 0);

//
// CONFIGURE EL PASO 0 en la secuencia 3. MUESTRA CANAL 0 (ADC _ Ctl _ Ch0) en
// modo único (predeterminado) y configurar la bandera de interrupción
// (ADC _ Ctl _ IE) que se establezca cuando se haga la muestra. Cuenta la lógica adc
// que esta es la última conversión en la secuencia 3 (ADC _ Ctl _ end). Secuencia
// 3 tiene sólo un paso programable. La secuencia 1 y 2 TIENEN 4 PASOS, y
// LA SECUENCIA 0 TIENE 8 PASOS PROGRAMABLES. Ya que sólo estamos haciendo un solo
// conversión usando la secuencia 3 solo se el paso 0. Para más
// información sobre las secuencias y pasos adc, haga referencia a la hoja de datos.
//
Adcsequencestepconfigure (adc0 _ base, 3, 0, ADC _ Ctl _ Ch0 | ADC _ Ctl _ IE |
ADC _ Ctl _ end);

//
// ya que la secuencia de muestra 3 está configurada, debe estar habilitada.
//
Adcsequenceenable (adc0 _ base, 3);

//
// despejen la bandera del estado de interrupción. Esto se hace para asegurarse de que
// la bandera de interrupción está despejada antes de probar.
//
Adcintclear (adc0 _ base, 3);

//
// muestra ain0 para siempre. Muestra el valor en la consola.
//
Mientras que (1)
{
//
// activar la conversión adc.
//
Adcprocessortrigger (adc0 _ base, 3);

//
// espera a que se complete la conversión.
//
Mientras! Adcintstatus (adc0 _ base, 3, falso))
{
}

//
// despejen la bandera de interrupción del adc.
//
Adcintclear (adc0 _ base, 3);

//
// LEE EL VALOR ADC.
//
Adcsequencedataget (adc0 _ base, 3, pui32adc0value);

//
// muestra el valor digital ain0 (pe7) en la consola.
//
Uartprintf ("Ain0 = % 4 D \ r", pui32adc0value [0]);

//
// esta función proporciona un medio de generar una longitud constante
// retraso. La función retardo (en ciclos) = 3 * parámetro. Demora
// 250 ms de manera arbitraria.
//
/*# si se define (objetivo _ es _ tm4c129 _ Ra0) || \
Definido (objetivo _ es _ tm4c129 _ Ta1) || \
Definido (objetivo _ es _ tm4c129 _ Ra2)
Sysctldelay (Ui32sysclock / 12);
#else */
Sysctldelay (Sysctlclockget () / 4096);
// #endif
}
}
