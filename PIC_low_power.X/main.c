/*
 * File:   main.c
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on February 1, 2018, 5:12 PM
 */


#include <xc.h>

void main(void) {
    
    OSCCONbits.IRCF = 0b1111; //4 Mhz
    
    while(1) {
        NOP();
    }
    
    return;
}
