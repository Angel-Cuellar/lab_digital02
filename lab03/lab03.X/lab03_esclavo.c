/*
 * File:   lab03_esclavo.c
 * Author: angel
 *
 * Created on 2 de agosto de 2021, 03:59 PM
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
#include "SPI.h"
#define  _XTAL_FREQ 8000000

///////////////// declaración de variables /////////////////////////////////////

unsigned char conversion1 = 0; 
unsigned char conversion2 = 0;
unsigned char canal; 

/*  codigo para subrutina de interrupción medinte el uso de timer0, conversion 
 * por el modulo del ADC a demas de interrupciones por ON CHANGE*/ 

void __interrupt()isr(void){
    
    // interrucion por medio de bandera del modulo ADC
    if (ADIF == 1) {
        if (ADCON0bits.CHS == 0){
            conversion1 = ADRESH;
            ADCON0bits.CHS = 1;
        }
        else if (ADCON0bits.CHS == 1) {
            conversion2 = ADRESH;
            ADCON0bits.CHS = 0;
        }
        PIR1bits.ADIF = 0;            // bajo la bandera de la interrupcion
    }
    // interrupcion de la comunicacion SPI 
    if(SSPIF == 1){
        canal = spiRead();
        switch (canal) {
            case(0):
                spiWrite(conversion1);
                break;
            case(1):
                spiWrite(conversion2);
        }
        SSPIF = 0;
    }
    return;
}

void main(void) {
    
///////////////////// configuraciones de pines del pic /////////////////////////
 
ANSEL = 0b00000011; 
ANSELH = 0x00;

TRISA = 0b00000011; 
TRISEbits.TRISE0 = 1;       // Slave Select

PORTA = 0x00;
PORTB = 0x00;
PORTC = 0x00;
PORTD = 0x00;
PORTE = 0x00;

//////////////////////// configuracion del oscilador interno ///////////////////

/* en este caso voy a configurarlo para que opere a 4 Mhz por lo que sera la 
 opcion 7 que le ingrese a la funcion de la libreria. */

conf_OSCCON(8);

//////////////////////// configuracion del modulo ADC //////////////////////////

/* selecione el canal 0 ademas de la opcion 3 debido que tengo un FOSC = 8 Mhz
 para lo cual el modulo ADC tendra una FOSC/32 = 4 us */

conf_ADC(0 , 3);
__delay_us(50);         // espero para que se cargen las conversiones 
ADCON0bits.GO = 1;     // empezando la coversion de valores 

//////////////// configuracion de comunicacion ISP  ////////////////////////////

spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

////////// habilitando las banderas de interrupcion ////////////////////////////

INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
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
