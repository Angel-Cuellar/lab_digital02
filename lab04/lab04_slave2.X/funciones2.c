/*
 * File:   funciones2.c
 * Author: angel
 *
 * Created on 9 de agosto de 2021, 07:34 PM
 */


#include <xc.h>
#include <stdint.h>
#include "funciones2.h"

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