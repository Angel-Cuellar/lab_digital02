/*
 * File:   libreria_slave.c
 * Author: angel
 *
 * Created on 7 de agosto de 2021, 11:24 PM
 */


#include <xc.h>
#include <stdint.h>
#include "libreria_slave.h"

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