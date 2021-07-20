/*
 * File:   lab00_main.c
 * Author: angel
 *
 * Created on 11 de julio de 2021, 09:15 PM
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
#define  _XTAL_FREQ 4000000

///////////////// declaración de variables /////////////////////////////////////

char retardo = 100;  
char cont = 3; 
char ganador = 0; 
char incre1 = 0; 
char incre2 = 0;

void __interrupt()isr(void){
    
    if (T0IF == 1){
        INTCONbits.T0IF = 0; // bajo la bandera del timer0 
        TMR0 = 99;          // reseteo el timer0 para que funcione a 10 ms
        if (retardo > 0) {
            retardo--; 
        }
        else {
            retardo = 100; 
            if (cont > 0) {
                cont--; 
            }
        }
    }
    
    if (RBIF == 1){
        if (RB5 == 0){                  
            cont = 3; 
            retardo = 100;
            ganador = 0; 
            incre1 = 0;
            incre2 = 0; 
            PORTA = 0x00; 
            PORTD = 0x00; 
            RB0 = 0; 
            RB1 = 0; 
        }
        if (RB6 == 0){  
            if ((cont == 0) && (ganador != 2)) {
                incre1++;
            }    
        }
        if (RB7 == 0){                  
            if ((cont == 0) && (ganador != 1)) {
                incre2++;
            }  
        }
    INTCONbits.RBIF = 0; // luego de todo esto, bajo la bandera de ON CHANGE. 
    }
    
    return; 
}

void main(void) {
    
///////////////////////// configuracion de pines ///////////////////////////////

ANSEL = 0x00; 
ANSELH = 0x00; 
    
TRISA = 0x00; 
TRISB = 0b11100000;
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
IOCBbits.IOCB5 = 1;         // configuracion de los PULL UP internos en los
IOCBbits.IOCB6 = 1;         // pines RB0, RB1, RB5, RB6 y RB7
IOCBbits.IOCB7 = 1;
WPUB = 0b11100000;

/////////////////  configuracion de reloj interno //////////////////////////////

OSCCONbits.IRCF2 = 1; 
OSCCONbits.IRCF1 = 1;  // configurando el clk interno a 4M hz 
OSCCONbits.IRCF0 = 0;
OSCCONbits.SCS = 1;

/////////////// configuracion del timer0 ///////////////////////////////////////

OPTION_REGbits.T0CS = 0; 
OPTION_REGbits.PSA = 0;
OPTION_REGbits.PS = 0b101; // configurando para utilizar un pre_escaler de
TMR0= 99;                 // 1:44 opera a 10 ms  
    
////////// habilitando las banderas de interrupcion ////////////////////////////

INTCONbits.RBIE = 1;
INTCONbits.RBIF = 0;  // interrupciones del ON CHANGE 
INTCONbits.T0IE = 1; 
INTCONbits.T0IF = 0; // banderas de interrupcion del timer0
INTCONbits.GIE = 1;

while (1) {
    
//////////////// esta parte es para determinar que jugagador gano primero //////
    
    switch (incre1){
        case (0) :
            PORTA = 0b00000000; 
        break; 
        case (1) :
            PORTA = 0b00000001;
        break;
        case (2) :
            PORTA = 0b00000010;
        break;
        case (3) :
            PORTA = 0b00000100;  
        break;
        case (4) :
            PORTA = 0b00001000; 
        break; 
        case (5) :
            PORTA = 0b00010000;
        break;
        case (6) :
            PORTA = 0b00100000;
        break;
        case (7) :
            PORTA = 0b01000000;
        break;
        case (8) :
            PORTA = 0b10000000;
            ganador = 1; 
            RB0 = 1; 
            RB1 = 0;
        break;
        }
    
        switch (incre2){
        case (0) :
            PORTD = 0b00000000; 
        break; 
        case (1) :
            PORTD = 0b00000001;
        break;
        case (2) :
            PORTD = 0b00000010;
        break;
        case (3) :
            PORTD = 0b00000100;  
        break;
        case (4) :
            PORTD = 0b00001000; 
        break; 
        case (5) :
            PORTD = 0b00010000;
        break;
        case (6) :
            PORTD = 0b00100000;
        break;
        case (7) :
            PORTD = 0b01000000;
        break;
        case (8) :
            PORTD = 0b10000000;
            ganador = 2; 
            RB0 = 0; 
            RB1 = 1;
        break;
        }
    
///////// parte de codigo que controla el semaforo ////////////////////////////
    
    if ((cont == 3) || (cont == 2)) {
        RE0 = 1; 
        RE1 = 0; 
        RE2 = 0; 
    }
    if (cont == 1) {
        RE0 = 0; 
        RE1 = 1; 
        RE2 = 0;
    }
    if ((ganador != 0) && (cont == 0)) {
        RE0 = 1; 
        RE1 = 1; 
        RE2 = 1;
    }
    else if (cont == 0) {
        RE0 = 0; 
        RE1 = 0; 
        RE2 = 1;
    }
    
//////////// parte para controlar el display de 7 segmentos ////////////////////
    
    if (ganador == 1) {
        PORTC = 0b00000110;
    }
    else if (ganador == 2) {
        PORTC = 0b01011011;
    }
    else {
        switch (cont){
            case (0) : PORTC = 0b00111111; 
            break; 
            case (1) : PORTC = 0b00000110;
            break;
            case (2) : PORTC = 0b01011011;
            break;
            case (3) : PORTC = 0b01001111;  
            break;
        }
    } 
}
return;
}
