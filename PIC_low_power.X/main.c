/*
 * File:   main.c
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on February 1, 2018, 5:12 PM
 */


#include <xc.h>

void main(void) {
    
    OSCCONbits.IRCF = 0b0111; //4 Mhz
    
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;

    while(1) {
        NOP();
    }
    
    return;
}
