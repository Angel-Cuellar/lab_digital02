/*
 * File:   funciones.c
 * Author: angel
 *
 * Created on 26 de julio de 2021, 10:32 AM
 */


#include <xc.h>
#include <stdint.h>
#include "funciones.h"
#define  _XTAL_FREQ 4000000

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

////////// funcion encargada de hacer el ENABLE de datos en la LCD /////////////

    
/////// funcion encargada de inicializar la secuencia de arranque del LCD //////

void init_LCD() {
    
    PORTD = 0x00;
    RC0 = 0;
    RC1 = 0;
    
    __delay_ms(20);
    comando_LCD(0x30);
    
    __delay_ms(5);
    comando_LCD(0x30);
   
    __delay_us(200);
    comando_LCD(0x30);
    
    comando_LCD(0x38);
    comando_LCD(0x0c);
    comando_LCD(0x06);
    
    return;
}

void comando_LCD(unsigned char dato){
    PORTD = dato;
    RC0 = 0;                 //modo comando
    RC1 = 0;                 //modo escritura

    RC2 = 1; 
    __delay_us(40);
    RC2 = 0;
    __delay_us(40);

    return;
}

void centrado_LCD(unsigned char x,unsigned char y){
    if(x>0){
        comando_LCD(0xC0 + y);
    }
    else{
        comando_LCD(0x80 + y);
    }
    return;
}

void sed_LCD(char *dato){
    while(*dato){
        __delay_us(100);
        caracteres_LCD(*dato);
        dato++;
    }
    return;
}

void caracteres_LCD(char data){
    __delay_us(100);
    
    PORTD = data;
    //Configuración
    RC0 = 1;     //Modo Escritura
    RC1 = 0;     //Modo Caracter
    __delay_us(10); 
    RC2 = 1;      //Habilita LCD
    __delay_us(10);
    RC2 = 0;      //Deshabilita LCD
    
    return;
}

// funcion encargada de convertir el valor del ADC a decimales que muestren el 
// valor de la conversion en un voltaje entre 0V-5V, en este caso es para el 
// numero entero antes del punto decimal. 
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

// funcion encargada de convertir el valor del ADC a decimales que muestren el 
// valor de la conversion en un voltaje entre 0V-5V, en este caso es para el 
// perimer numero entero despues del punto decimal, osea las unidades de millar.  
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

// funcion encargada de convertir el valor del ADC a decimales que muestren el 
// valor de la conversion en un voltaje entre 0V-5V, en este caso es para el 
// segundo numero entero despues del punto decimal, osea las decemas de millar.  
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