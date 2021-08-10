/*
 * File:   slave2_main.c
 * Author: angel
 *
 * Created on 9 de agosto de 2021, 07:15 PM
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
#include "funciones2.h"
#include "I2C.h"
#define  _XTAL_FREQ 8000000

///////////////// declaración de variables /////////////////////////////////////

uint8_t z;
uint8_t dato;

/*  codigo para subrutina de interrupción por el ON CHANGE*/ 

void __interrupt()isr(void){
    
       if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
                                        // en este caso no recibo datos de master
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = PORTD;              // envio el valor del contador que tengo
            SSPCONbits.CKP = 1;          // en el puerto D. 
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }

    // interrucion por medio de bandera del ON CHANGE.
    if (RBIF == 1){
        if (RB0 == 0) {
            if (PORTD == 0x0f) {
                PORTD = 0x00;        // condiciones para que al hacer onderflow 
            }                        // pase el contado a 0
            else {
              PORTD++;               // incremento el puerto o contador
            }
        }
        else if (RB1 == 0) {
            if (PORTD == 0x00) {
                PORTD = 0x0f;        // condiciones para que al hacer underflow
            }                        // pase el contado a 15
            else {
              PORTD--;               // decremento el puerto. 
            }
        }   
    INTCONbits.RBIF = 0; // luego de todo esto, bajo la bandera de ON CHANGE.
    }
    return; 
}

////////////////////////////// MENU PRINCIPAL //////////////////////////////////

void main(void) {
    
///////////////////// configuraciones de pines del pic /////////////////////////
 
ANSEL = 0x00; 
ANSELH = 0x00;

TRISA = 0x00; 
TRISB = 0b00000011; 
TRISD = 0x00;
TRISE = 0x00;

PORTA = 0x00;
PORTB = 0x00;
PORTC = 0x00;
PORTD = 0x00;
PORTE = 0x00;

// configuracion de PULL UP internos en PORTB///////////////////////////////////

OPTION_REGbits.nRBPU = 0;       
IOCBbits.IOCB0 = 1;        // configuracion de los PULL UP internos en los
IOCBbits.IOCB1 = 1;        // pines RB0 y RB1
WPUB = 0b00000011;

//////////////////////// configuracion del oscilador interno ///////////////////

/* en este caso voy a configurarlo para que opere a 8 Mhz por lo que sera la 
 opcion 8 que le ingrese a la funcion de la libreria. */

conf_OSCCON(8);

//////////////// configuracion de comunicacion I2C /////////////////////////////

I2C_Slave_Init(0xC0);

////////// habilitando las banderas de interrupcion ////////////////////////////

INTCONbits.RBIE = 1;
INTCONbits.RBIF = 0;  // interrupciones del ON CHANGE 
INTCONbits.GIE = 1;

////////////////////////////// LOOP principal //////////////////////////////////

while (1) {
    
  
}
return;
}