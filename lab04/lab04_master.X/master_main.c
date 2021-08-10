/*
 * File:   master_main.c
 * Author: angel
 *
 * Created on 8 de agosto de 2021, 09:48 PM
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
#include <stdio.h>
#include <string.h>
#include "libreria_master.h"
#include "I2C.h"
#define  _XTAL_FREQ 8000000

///////////////// declaración de variables /////////////////////////////////////

uint8_t ADC ;
uint8_t contador;
uint32_t conver ; 
int temperatura ;
unsigned char volt[3];
unsigned char conta[1];
unsigned char tempe[3];

////////////////////////////////////////////////////////////////////////////////

void main(void) {

///////////////////// configuraciones de pines del pic /////////////////////////
    
ANSEL = 0x00; 
ANSELH = 0x00;

TRISA = 0x00;
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

//////////////// configuracion de comunicacion I2C /////////////////////////////

I2C_Master_Init(100000);         // configurado a 100kHz 

///////////////////////// configuracion pantalla LCD ///////////////////////////

init_LCD();

////////////////////////////// LOOP principal //////////////////////////////////

while(1){
    
    //// lectura del primer esclavo con direccion 0xB0
       
    I2C_Master_Start();
    I2C_Master_Write(0xB1);                // guardo el valor de lectura en la
    ADC = I2C_Master_Read(0);              // variable ADC
    I2C_Master_Stop();
    __delay_ms(200);
    
    //// lectura del sensor TC74 de microchip con direccion 0xA0 = 0b1001000
    
    I2C_Master_Start();
    I2C_Master_Write(0b10010000);
    I2C_Master_Write(0x00);        // me comunico con el sensor y le mando que 
    I2C_Master_Stop();             // me envie el valor de la temperatura y 
    __delay_ms(200);               // guardo el valor en la variable temperatura
    I2C_Master_Start();
    I2C_Master_Write(0b10010001);
    temperatura = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(200);
    
        //// lectura del segundo esclavo con direccion 0xC0
       
    I2C_Master_Start();
    I2C_Master_Write(0xC1);
    contador = I2C_Master_Read(0);       // guardo el valor de lectura en la 
    I2C_Master_Stop();                   // variable contador
    __delay_ms(200);
    
    ///////////////////// despliege de datos en LCD ////////////////////////////
    
    centrado_LCD(0,0);
    __delay_us(100);
    sed_LCD("S1:    S2:   S3:");    ////// titulo superior de la LCD
    __delay_us(100);
    
    ////////////// para parte inferior del lcd para voltaje/////////////////////
    
    conver = (510/255)*ADC;          // factor de conversion de 0-5.1V 
    
    volt[0] = centenas(conver);
    volt[1] = '.';
    volt[2] = decenas(conver);       /// guardo los valores en un array 
    volt[3] = unidades(conver);
    volt[4] = 'V';
    
    centrado_LCD(1,0);
    __delay_us(100);
    sed_LCD(&volt);               // envio el mensaje en la LCD.
    __delay_us(100);
    
    /////////////// para parte inferior del lcd para contador///////////////////
    
    conta[0] = decenas(contador);           // guardo los caracteres del proceso
    conta[1] = unidades(contador);         // de division en un array
    
    centrado_LCD(1,7);
    __delay_us(100);
    sed_LCD(&conta);            // envio el mensaje a la LCD. 
    __delay_us(100);
    
    /////// para parte inferior del lcd para sensor de temperatura TC74 ////////
    
    if (temperatura >= 191) {
        temperatura = -(temperatura - 256);
        tempe[0] = '-';
        tempe[1] = decenas(temperatura);        /// esto lo hago para pasar el 
        tempe[2] = unidades(temperatura);       /// complemento A2 que me da el 
        tempe[3] = 'C';                   // sensor de la temperatura a positivo          
    }
    else {
        tempe[0] = centenas(temperatura);
        tempe[1] = decenas(temperatura);
        tempe[2] = unidades(temperatura);         /// aqui paso directo el valor
        tempe[3] = 'C';                    // del sensor a tempeatura. 
    }
    
    centrado_LCD(1,12);
    __delay_us(100);
    sed_LCD(&tempe);           /// despliego en la LCD el valor. 
    __delay_us(100);
  
    }
return;
}
