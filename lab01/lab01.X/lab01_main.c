/*
 * File:   lab01_main.c
 * Author: angel
 *
 * Created on 17 de julio de 2021, 11:06 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include "libreria_funciones.h"
#define  _XTAL_FREQ 4000000

///////////////// declaración de variables /////////////////////////////////////

unsigned char turno = 0; 
unsigned char conteo = 0;
unsigned char conversion = 0; 
unsigned char disp1 = 0;
unsigned char disp2 = 0;

/*  codigo para subrutina de interrupción medinte el uso de timer0, conversion 
 * por el modulo del ADC a demas de interrupciones por ON CHANGE*/ 

void __interrupt()isr(void){
    
    // interrucion por medio de bandera del modulo ADC
    if (ADIF == 1) {
        conversion = ADRESH;
        PIR1bits.ADIF = 0;            // bajo la bandera de la interrupcion
    }
    
    // interrucion por medio de bandera de timer0 para manejo de displays. 
    if (T0IF == 1){
        INTCONbits.T0IF = 0; // bajo la bandera del timer0 
        TMR0 = 231;          // reseteo el timer0 para que funcione a 0.1 ms.
        RD0 = 0;
        RD1 = 0;
        if (turno == 1){    
            PORTC = disp1;    // si la variable "turno" tiene valor 1,
            RD0 = 1;             // entonces enciendo el pin RD0 y apago RD1 y 
            RD1 = 0;             // muevo el valor de  "display1" al PORTC.
            turno = 0;  // luego dejo en cero "turno" para pasar al otro display
        }
        else { 
            PORTC = disp2;    // si la variable "turno" tiene valor 1,
            RD0 = 0;             // entonces apago el pin RD0 y enciendo RD1 y 
            RD1 = 1;             // muevo el valor de  "display2" al PORTC.
            turno = 1;  // luego dejo en 1 "turno" para pasar al otro display
        }
    } 
    
    // interrucion por medio de bandera del ON CHANGE.
    if (RBIF == 1){
        if (RB6 == 0){  
            PORTA++; 
        }
        if (RB7 == 0){
            PORTA--;  
        }
    conteo = PORTA;
    INTCONbits.RBIF = 0; // luego de todo esto, bajo la bandera de ON CHANGE.
    }
    return; 
}

////////////////////////////// MENU PRINCIPAL //////////////////////////////////

void main(void) {
    
///////////////////// configuraciones de pines del pic /////////////////////////
 
ANSEL = 0x00; 
ANSELH = 0b00000001;

TRISA = 0x00; 
TRISB = 0b11000001; 
TRISC = 0x00;
TRISD = 0x00;
TRISE = 0x00;

PORTA = 0x00;
PORTB = 0x00;
PORTC = 0x00;
PORTD = 0x00;
PORTE = 0x00;

// configuracion de PULL UP internos en PORTB///////////////////////////////////

OPTION_REGbits.nRBPU = 0;       
IOCBbits.IOCB6 = 1;        // configuracion de los PULL UP internos en los
IOCBbits.IOCB7 = 1;        // pines RB6 y RB7
WPUB = 0b11000000;

//////////////////////// configuracion del oscilador interno ///////////////////

/* en este caso voy a configurarlo para que opere a 4 Mhz por lo que sera la 
 opcion 7 que le ingrese a la funcion de la libreria. */

conf_OSCCON(7);

/////////////// configuracion del timer0 ///////////////////////////////////////

/* seleccionare un pre_escaler de 1:8 de manera que TMR0 realice interrupciones 
 * cada 0.1 ms por ello, seleccionare la opcion 3 de la funcion de mi libreia */

conf_TMR0(3);
TMR0 = 231; 

//////////////////////// configuracion del modulo ADC //////////////////////////

/* selecione el canal 12 ademas de la opcion 2 debido que tengo un FOSC = 4 Mhz
 para lo cual el modulo ADC tendra una FOSC/8 = 2 us */

conf_ADC(12 , 2);
__delay_us(50);         // espero para que se cargen las conversiones 
ADCON0bits.GO = 1;     // empezando la coversion de valores 

////////// habilitando las banderas de interrupcion ////////////////////////////

INTCONbits.PEIE = 1; 
PIE1bits.ADIE = 1;  // activo la interrupcion por el ADC
PIR1bits.ADIF = 0;  // bajo la bandera de interrupcion del ADC
INTCONbits.RBIE = 1;
INTCONbits.RBIF = 0;  // interrupciones del ON CHANGE 
INTCONbits.T0IE = 1; 
INTCONbits.T0IF = 0; // banderas de interrupcion del timer0
INTCONbits.GIE = 1;

////////////////////////////// LOOP principal //////////////////////////////////

while (1) {
    /* esta parte me sirve cuando termina la conversion del ADC, se apaga el bit 
     de conteo "GO" y para que vuelva hacer otra conversion debo encerder de 
     nuevo el bit "GO".*/
    if (ADCON0bits.GO == 0) {
        __delay_us(50); 
        ADCON0bits.GO = 1;
    }
    
    // esta parte controlo los valores a mostrar en los displays mediante el uso
    // de las funciones que defini en la libreria. 
    disp1 = display1(conversion);
    disp2 = display2(conversion);
    
    // en esta parte es donde se programa el led de alarma cuando el ADC supere 
    // al contado de los leds. 
    
    if (conversion > conteo) {
        RD2 = 1; 
    }
    else {
        RD2 = 0; 
    }
}
return;
}
