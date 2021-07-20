/*
 * File:   funciones.c
 * Author: angel
 *
 * Created on 17 de julio de 2021, 11:08 PM
 */


#include <xc.h>
#include <stdint.h>
#include "libreria_funciones.h"

// funcion encargada de programar el oscilador interno el PIC y seleccionar la 
// frecuencia de uso deseada. 
void conf_OSCCON(uint8_t frecuencia){
    
    switch (frecuencia) {
        case (1):
            OSCCONbits.IRCF = 0b000;     // para frec = 31 Khz 
            break;
        case (2):
            OSCCONbits.IRCF = 0b001;     // para frec = 125 Khz
            break;
        case (3):
            OSCCONbits.IRCF = 0b010;     // para frec = 250 Khz
            break;
        case (4):
            OSCCONbits.IRCF = 0b011;     // para frec = 500 Khz
            break;
        case (5):
            OSCCONbits.IRCF = 0b100;     // para frec = 1 Mhz 
            break;
        case (6):
            OSCCONbits.IRCF = 0b101;     // para frec = 2 Mhz
            break;
        case (7):
            OSCCONbits.IRCF = 0b110;     // para frec = 4 Mhz
            break;
        case (8):
            OSCCONbits.IRCF = 0b111;     // para frec = 8 Mhz
            break;
    }       
    OSCCONbits.SCS = 1;    // seleccionando el oscilador interno del PIC
    return;
}

// funcion encargada de seleccionar el pre_escaler el TMR0 ademas de seleccionar
//que esta funcionara con el FOSC/4 y con el oscilador interno. 
void conf_TMR0(uint8_t pre_escaler) {
    
    OPTION_REGbits.T0CS = 0;    // para trabajar con ciclo interno  FOSC/4 
    OPTION_REGbits.PSA = 0;     // para seleccionar un pre_escaler del TMR0
     
    switch (pre_escaler) {
        case (1):
            OPTION_REGbits.PS = 0b000;     // para pre_escaler = 1:2
            break;
        case (2):
            OPTION_REGbits.PS = 0b001;     // para pre_escaler = 1:4
            break;
        case (3):
            OPTION_REGbits.PS = 0b010;     // para pre_escaler = 1:8
            break;
        case (4):
            OPTION_REGbits.PS = 0b011;     // para pre_escaler = 1:16
            break;
        case (5):
            OPTION_REGbits.PS = 0b100;     // para pre_escaler = 1:32
            break;
        case (6):
            OPTION_REGbits.PS = 0b101;     // para pre_escaler = 1:64
            break;
        case (7):
            OPTION_REGbits.PS = 0b110;     // para pre_escaler = 1:128
            break;
        case (8):
            OPTION_REGbits.PS = 0b111;     // para pre_escaler = 1:256
            break;
    }
    return; 
}

// funcion encargada de configurar el canal de conversion deseado ademas de 
//segun la frecuencia del osculador interno seleccionar el tiempo de conversion 
//adecuado. 
void conf_ADC(uint8_t canal, uint8_t frec_reloj) {
    
    switch (frec_reloj) {
        case (1):
            ADCON0bits.ADCS = 0b00;   // para frec = 1 Mhz ; FOSC/2 = 2.0 us
            break;
        case (2):
            ADCON0bits.ADCS = 0b01;   // para frec = 4 Mhz ; FOSC/8 = 2.0 us
            break;
        case (3):
            ADCON0bits.ADCS = 0b10;   // para frec = 8 Mhz ; FOSC/32 = 4.0 us 
            break;                    
        case (4):
            ADCON0bits.ADCS = 0b10;   // para frec = 20 Mhz ; FOSC/32 = 1.4 us
            break;
    }
    ADCON0bits.CHS = canal;
    ADCON0bits.ADON = 1;   // activando el modulo ADC 

    ADCON1bits.ADFM = 0;   // datos agrupados hacia la izquierda 
    ADCON1bits.VCFG1 = 0;  // voltajes de refenrencia 5v = Vdd y
    ADCON1bits.VCFG0 = 0;  // como voltaje inferior 0v. 

    ADRESH = 0x00;         // dejando en cero los registro para almacenar los 
    ADRESL = 0x00;         // datos de conversion del ADC
    return; 
}

// funcion encargada de convertir los 4 nibbles mas significativos a hexadecimal 
// y para dar la posicion en el 7 segmentos para enseñar ese valor en el primer
// display de 7 segmentos 
uint8_t display1(uint8_t cantidad) {
    
    uint8_t decenas = 0;
    uint8_t display1 = 0;       // creacion de variables para hacer operaciones
    
    while (cantidad >= 16) {
        cantidad = cantidad - 16;  // proceso de division para saber cuantas 
        decenas++;                 // decenas que tiene la variable "cantidad"
    }
    
    /*  esta parte es para poder alistar los valores que seran mostrados en los 
     *  displays de 7 segmentos   */
    
    switch (decenas){
        case (0) : display1 = 0b00111111; 
        break; 
        case (1) : display1 = 0b00000110;
        break;
        case (2) : display1 = 0b01011011;
        break;
        case (3) : display1 = 0b01001111;  
        break;
        case (4) : display1 = 0b01100110;
        break;
        case (5) : display1 = 0b01101101;
        break;
        case (6) : display1 = 0b01111101; 
        break;
        case (7) : display1 = 0b00000111;
        break;
        case (8) : display1 = 0b01111111;
        break;
        case (9) : display1 = 0b01100111; 
        break;
        case (10) : display1 = 0b01110111;
        break;
        case (11) : display1 = 0b01111100;
        break;
        case (12) : display1 = 0b00111001;
        break;
        case (13) : display1 = 0b01011110;
        break;
        case (14) : display1 = 0b01111001;
        break;
        case (15) : display1 = 0b01110001;
        break;
    }
    return (display1) ;      // retorno del valor de "display1" 
}

// funcion encargada de convertir los 4 nibbles menos significativos  hexadecimal 
// y para dar la posicion en el 7 segmentos para enseñar ese valor en el segundo
// display de 7 segmentos
uint8_t display2(uint8_t cantidad) {
    
    uint8_t unidades = 0;
    uint8_t display2 = 0;       // creacion de variables para hacer operaciones
    
    while (cantidad >= 16) {
        cantidad = cantidad - 16;   // proceso de division para saber cuantas
    }                               // unidades que tiene la variable "cantidad"
    if (cantidad < 16) {
        unidades = cantidad;
    }
    
    /*  esta parte es para poder alistar los valores que seran mostrados en los 
     *  displays de 7 segmentos   */
    
    switch (unidades){
        case (0) : display2 = 0b00111111;  
        break;
        case (1) : display2 = 0b00000110;
        break;
        case (2) : display2 = 0b01011011;
        break;
        case (3) : display2 = 0b01001111;  
        break;
        case (4) : display2 = 0b01100110;
        break;
        case (5) : display2 = 0b01101101;
        break;
        case (6) : display2 = 0b01111101; 
        break;
        case (7) : display2 = 0b00000111;
        break;
        case (8) : display2 = 0b01111111;
        break;
        case (9) : display2 = 0b01100111; 
        break;
        case (10) : display2 = 0b01110111;
        break;
        case (11) : display2 = 0b01111100;
        break;
        case (12) : display2 = 0b00111001; 
        break;
        case (13) : display2 = 0b01011110;
        break;
        case (14) : display2 = 0b01111001;
        break;
        case (15) : display2 = 0b01110001;
        break;
    }
    return (display2) ;          // retorno del valor de display2"
}