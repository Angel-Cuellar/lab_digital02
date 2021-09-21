/*
 * File:   sensor.c
 * Author: angel
 *
 * Created on 20 de agosto de 2021, 08:41 PM
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
#include "libreria.h"
#define  _XTAL_FREQ 8000000

///////////////// declaración de variables /////////////////////////////////////
 
unsigned char volt[2];
uint16_t dist;

////////////////////////////// MENU PRINCIPAL //////////////////////////////////

void main(void) {

///////////////////// configuraciones de pines del pic /////////////////////////
 
ANSEL = 0x00; 
ANSELH = 0x00;

TRISA = 0x00; 
TRISB = 0b00000010; 
TRISC = 0x00; 
TRISD = 0x00;
TRISE = 0x00;

PORTA = 0x00;
PORTB = 0x00;
PORTC = 0x00;
PORTD = 0x00;
PORTE = 0x00;

//////////////////////// configuracion del oscilador interno ///////////////////

/* en este caso voy a configurarlo para que opere a 8 Mhz por lo que sera la 
 opcion 8 que le ingrese a la funcion de la libreria. */

conf_OSCCON(8);

/* seleccionare un pre_escaler de 1:2 de manera que TMR0 realice interrupciones 
 * cada 1.0 us por ello, seleccionare la opcion 0 de la funcion de mi libreia */

conf_TMR1(1);
TMR1 = 0; 

///////////////////////// configuracion pantalla LCD ///////////////////////////

init_LCD();

////////////////////////////// LOOP principal //////////////////////////////////

while (1) {
    
    RB0 = 1;
    __delay_us(15);
    RB0 = 0;
    
    TMR1 = 0;
    
    while (!RB1);
    T1CONbits.TMR1ON = 1; 
    
    while(RB1);
    
    T1CONbits.TMR1ON = 0;
    
    dist = (TMR1/2)/58;
    
    volt[0] = centenas(dist);
    volt[1] = decenas(dist);       /// guardo los valores en un array 
    volt[2] = unidades(dist);
    
    centrado_LCD(0,0);
    __delay_us(100);
    sed_LCD(&volt);               // envio el mensaje en la LCD.
    __delay_us(100);
    
}
return;
}


