;---------------------------------------------------------------------------------
; EE3376 MSP430 Lab 1
; The University of Texas at El Paso
; Autores: Victor Barraza y Eric MacDonald
;
; Un programa sencillo en ensamblador que suma cuatro pares de numeros, desplega
; ndo el estado de las banderas C y V en los LEDs 1 y 2 de la LaunchPad.
;---------------------------------------------------------------------------------
; Las siguientes lineas deben incluirse en todos los programas en ensamblador.
;---------------------------------------------------------------------------------
	.cdecls C,LIST, "msp430G2553.h"

	.text
	.global _main
;------------------------------------------------------------
_main:
	mov.w #WDTPW|WDTHOLD, &WDTCTL  ;detiene el temporizador "watchdog"
	clr.b &P1OUT   ;inicia con los LEDs apagados
	mov.b #0x41, &P1DIR    ;almacena el numero #01000001b en el registro P1DIR, lo cual configura P1.0 y P1.6 como salidas
;---------------------------------------------------------------------------------------------------------------------------
	mov.w #0xFFFF,r4 ;carga r4 con 64532 (sin signo) o -1 (con signo)
	mov.w #0xFFFF,r5 ;carga r5 con 64532 (sin signo) o -1 (con signo)
	add.w r4,r5 ;suma r4 + r5 y almacena el resultado en r5
	mov.b sr, &P1OUT ;prende LED1 con base en el estado de la badera C
	mov.w sr, r6 ;carga el contenido de sr en r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
         ;despues de la suma se encuentra en el bit 7 de r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
		 ;despues de la suma se encuentra en el bit 6 de r6
	mov.b r6, &P1OUT ;prende LED2 con base en el estado de la bandera V
	clr.b &P1OUT ;apaga ambos LEDs
;---------------------------------------------------------------------------------------------------------------------------
	mov.w #0x8000,r4 ;carga r4 con 32252 (sin signo) o -32253 (con signo)
	mov.w #0x8000,r5 ;carga r4 con 32252 (sin signo) o -32253 (con signo)
	add.w r4,r5 ;suma r4 + r5 y almacena el resultado en r5
	mov.b sr, &P1OUT ;prende LED1 con base en el estado de la bandera C
	mov.w sr, r6 ;carga el contenido de sr en r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
			 ;despues de la suma se encuentra en el bit 7 de r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
			 ;despues de la suma se encuentra en el bit 6 de r6
	mov.b r6, &P1OUT ;prende LED2 con base en el estado de la bandera V
	clr.b &P1OUT ;apaga ambos LEDs
;---------------------------------------------------------------------------------------------------------------------------
	mov.w #0x0001,r4 ;carga r4 con 1
	mov.w #0x0001,r5 ;carag r5 con 1
	add.w r4,r5 ;suma r4 + r5 y almacena el resultado en r5
	mov.b sr, &P1OUT ;prende LED1 con base en el estado de la bandera C
	mov.w sr, r6 ;carga el contenido de sr en r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
	 		 ;despues de la suma se encuentra en el bit 7 de r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
			 ;despues de la suma se encuentra en el bit 6 de r6
	mov.b r6, &P1OUT ;prende LED2 con base en el estado de la bandera V
	clr.b &P1OUT ;apaga ambos LEDs
;---------------------------------------------------------------------------------------------------------------------------
	mov.w #0x4000,r4 ;carga r4 con 16182
	mov.w #0x4000,r5 ;carga r5 con 16182
	add.w r4,r5 ;suma r4 + r5 y almacena el resultado en r5
	mov.b sr, &P1OUT ;prende LED1 con base en el estado de la bandera C
	mov.w sr, r6 ;carga el contenido de sr en r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
			 ;despues de la suma se encuentra en el bit 7 de r6
	rra.w r6 ;corre el contenido de r6 a la derecha, el estado de la bandera V
		     ;despues de la suma se encuentra en el bit 6 de r6
	mov.b r6, &P1OUT ;prende LED2 con base en el estado de la bandera V
	clr.b &P1OUT ;apaga ambos LEDs
;---------------------------------------------------------------------------------------------------------------------------
infloop:

	jmp infloop
;---------------------------------------------------------------------------------
; Las siguientes lineas definen lo que pasa cuando el boton de reset es presionado
; Estas lineas deben ser incluidas en todos los programas.
;---------------------------------------------------------------------------------
	.sect ".reset"
	.short _main
