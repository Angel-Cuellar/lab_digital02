/*
 * File:   libreria.c
 * Author: angel
 *
 * Created on 16 de agosto de 2021, 01:35 AM
 */


#include <xc.h>
#include <stdint.h>
#include "libreria.h"
#define  _XTAL_FREQ 8000000

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

///////////////// configuracion comunicacion USART /////////////////////////////

void conf_USART() {
    
    TXSTAbits.TX9 = 0; 
    TXSTAbits.TXEN = 1;   // configuracion de registro TXSTA 
    TXSTAbits.SYNC = 0; 
    TXSTAbits.BRGH = 1; 

    RCSTAbits.SPEN = 1; 
    RCSTAbits.RX9 = 0;    // configuracion de registro RCSTA 
    RCSTAbits.CREN = 1; 

    BAUDCTLbits.BRG16 = 1;   // configuracion de registro BAUDCTL

    SPBRG = 207; 
    SPBRGH = 0;           // configurando que opere a 9600 BAULIOS 
    
    return;
}

// funcion encargada de convertir el valor decimal de centenas del contado a 
// caracter para poderse enviar serialmente 
char centenas(uint8_t cantidad) {
    
    char centenas = 0;        // creacion de variables para hacer operaciones 
    uint8_t cont = 0; 
    
    while (cantidad >= 100) {
        cantidad = cantidad - 100;  // proceso de division para saber cuantas 
        cont++;                 // centenas que tiene la variable "cantidad"
    }
    
    switch (cont){
        case (0) : centenas = '0';
        break; 
        case (1) : centenas = '1';
        break;
        case (2) : centenas = '2';
        break;
        case (3) : centenas = '3';  
        break;
        case (4) : centenas = '4';
        break;
        case (5) : centenas = '5';
        break;
        case (6) : centenas = '6'; 
        break;
        case (7) : centenas = '7';
        break;
        case (8) : centenas = '8';
        break;
        case (9) : centenas = '9'; 
        break;
    }
    return (centenas) ;     // retorno el valor decimal del ASCII a mostrar
}

// funcion encargada de convertir el valor decimal de decenas del contado a 
// caracter para poderse enviar serialmente
char decenas(uint8_t cantidad) {
    
    char decenas = 0;        // creacion de variables para hacer operaciones  
    uint8_t cont = 0;
    
    while (cantidad >= 100) {
        cantidad = cantidad - 100;  // proceso de division para saber cuantas               
    }
    
    while (cantidad >= 10) {
        cantidad = cantidad -10;
        cont++;              // decenas que tiene la variable "cantidad"
    }
    switch (cont){
        case (0) : decenas = '0';
        break; 
        case (1) : decenas = '1';
        break;
        case (2) : decenas = '2';
        break;
        case (3) : decenas = '3';  
        break;
        case (4) : decenas = '4';
        break;
        case (5) : decenas = '5';
        break;
        case (6) : decenas = '6'; 
        break;
        case (7) : decenas = '7';
        break;
        case (8) : decenas = '8';
        break;
        case (9) : decenas = '9'; 
        break;
    }
    return (decenas) ;     // retorno el valor decimal del ASCII a mostrar
}

// funcion encargada de convertir el valor decimal de unidades del contado a 
// caracter para poderse enviar serialmente
char unidades(uint8_t cantidad) {
    
    char unidades = 0;       // creacion de variables para hacer operaciones
    uint8_t cont = 0;
    
    while (cantidad >= 100) {
        cantidad = cantidad - 100;  // proceso de division para saber cuantas               
    }
    
    while (cantidad >= 10) {
        cantidad = cantidad -10;    
    }
    if (cantidad < 10) {
        cont = cantidad;       // unidades que tiene la variable "cantidad"
    }
    switch (cont){
        case (0) : unidades = '0';
        break; 
        case (1) : unidades = '1';
        break;
        case (2) : unidades = '2';
        break;
        case (3) : unidades = '3';  
        break;
        case (4) : unidades = '4';
        break;
        case (5) : unidades = '5';
        break;
        case (6) : unidades = '6'; 
        break;
        case (7) : unidades = '7';
        break;
        case (8) : unidades = '8';
        break;
        case (9) : unidades = '9'; 
        break;
    }
    return (unidades) ;     // retorno el valor decimal del ASCII a mostrar
}
