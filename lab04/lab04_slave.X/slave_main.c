/*
 * File:   slave_main.c
 * Author: angel
 *
 * Created on 7 de agosto de 2021, 11:29 PM
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
#include <pic16f887.h>
#include "libreria_slave.h"
#include "I2C.h"
#define  _XTAL_FREQ 8000000

///////////////// declaración de variables /////////////////////////////////////

unsigned char conversion = 0; 
unsigned char canal = 0;
uint8_t z;
uint8_t dato; 

/////////////////// codigo de interrupcion /////////////////////////////////////

void __interrupt() isr(void){
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
            __delay_us(250);            // en este caso no recibo datos del master
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = conversion;     /// envio el valor de la conversion 
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
   if (ADIF == 1) {
       conversion = ADRESH;      // paso la conversión a la variable.
   }
   PIR1bits.ADIF = 0;            // bajo la bandera de la interrupcion 
   
   return; 
}


void main(void) {
    
///////////////////// configuraciones de pines del pic /////////////////////////
    
ANSEL = 0b00000001; 
ANSELH = 0x00;

TRISA = 0b00000001;
TRISB = 0x00;
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

//////////////////////// configuracion del modulo ADC //////////////////////////

/* selecione el canal 0 ademas de la opcion 3 debido que tengo un FOSC = 8 Mhz
 para lo cual el modulo ADC tendra una FOSC/32 = 4 us */

conf_ADC(0,3);
__delay_us(50);         // espero para que se cargen las conversiones 
ADCON0bits.GO = 1;     // empezando la coversion de valores 

//////////////// configuracion de comunicacion I2C /////////////////////////////

I2C_Slave_Init(0xB0);

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
}
    return;
}
