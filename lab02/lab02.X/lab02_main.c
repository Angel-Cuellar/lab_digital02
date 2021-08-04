/*
 * File:   lab02_main.c
 * Author: angel
 *
 * Created on 26 de julio de 2021, 10:18 AM
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
#include <string.h>
#include "funciones.h"
#define  _XTAL_FREQ 4000000

///////////////// declaración de variables /////////////////////////////////////

uint32_t conversion1 = 0; 
uint32_t conversion2 = 0;
char voltaje1[4] ;
char voltaje2[4] ;

/*  codigo para subrutina de interrupción medinte el uso de timer0, conversion 
 * por el modulo del ADC a demas de interrupciones por ON CHANGE*/ 

void __interrupt()isr(void){
    
    // interrucion por medio de bandera del modulo ADC
    if (ADIF == 1) {
        if (ADCON0bits.CHS == 0){
            conversion1 = ((ADRESH*500)/255) ;
            ADCON0bits.CHS = 1;
        }
        else if (ADCON0bits.CHS == 1) {
            conversion2 = ((ADRESH*500)/255);
            ADCON0bits.CHS = 0;
        }
        PIR1bits.ADIF = 0;            // bajo la bandera de la interrupcion
    }
    return;
}

void main(void) {
    
///////////////////// configuraciones de pines del pic /////////////////////////
 
ANSEL = 0b00000011; 
ANSELH = 0x00;

TRISA = 0b00000011; 
TRISB = 0x00; 
TRISC = 0x00;
TRISD = 0x00;
TRISE = 0x00;

PORTA = 0x00;
PORTB = 0x00;
PORTC = 0x00;
PORTD = 0x00;
PORTE = 0x00;

//////////////////////// configuracion del oscilador interno ///////////////////

/* en este caso voy a configurarlo para que opere a 4 Mhz por lo que sera la 
 opcion 7 que le ingrese a la funcion de la libreria. */

conf_OSCCON(7);

///////////////////////// configuracion pantalla LCD ///////////////////////////

init_LCD();

//////////////////////// configuracion del modulo ADC //////////////////////////

/* selecione el canal 0 ademas de la opcion 2 debido que tengo un FOSC = 4 Mhz
 para lo cual el modulo ADC tendra una FOSC/8 = 2 us */

conf_ADC(0 , 2);
__delay_us(50);         // espero para que se cargen las conversiones 
ADCON0bits.GO = 1;     // empezando la coversion de valores 

////////// habilitando las banderas de interrupcion ////////////////////////////

INTCONbits.PEIE = 1; 
PIE1bits.ADIE = 1;  // activo la interrupcion por el ADC
PIR1bits.ADIF = 0;  // bajo la bandera de interrupcion del ADC
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
    
    voltaje1[1] = centenas(conversion1);
    
    centrado_LCD(0,0);
    __delay_us(100);
    sed_LCD(&voltaje1[1]);
    __delay_us(100);
    
    
    
;
}
return;
}
