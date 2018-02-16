/*
 * File:   main.c
 * Author: Julien LE MELLEC
 * Mail: julien.lemellec@gmail.com
 *
 * Created on February 1, 2018, 5:12 PM
 */


#include <xc.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = ON      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF      // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = OFF      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)


typedef struct Time_struct Time;
struct Time_struct {
    unsigned char century;
    unsigned char years;
    unsigned char months;
    unsigned char days;
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;
};

void pin_config(void);
void icremente_time(Time* time);
void init(void);

Time time;

void main(void) {
    
    OSCCONbits.IRCF = 0b0111;   // 500KHz
    OSCCONbits.SCS = 2;         // Internal oscillator block

    init();

    while(1) {
        NOP();
    }
    
    return;
}

void __interrupt time_managing(void) {
    
    char received_data;
    char received_command = 0;

    // Timer1 interrupt flag, every second
    // Timer triggered by quartz
    if(PIR1bits.TMR1IF) {
        TMR1IF = 0;
        TMR1H = 0xF0;
        // Led blinking
        PORTAbits.RA2 ? PORTAbits.RA2 = 0: PORTAbits.RA2=1;
        // Increment time
        icremente_time(&time);
    }
}

void pin_config(void) {
    // Set every pin to out, low
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;   
}

void init_timer1(void) {
    T1CONbits.TMR1ON = 1;       // Enable timer1
    T1CONbits.T1CKPS = 3;       // 1:8 Prescale value
    T1CONbits.TMR1CS = 2;       // Timer1 clock source is Crystal oscillator
    T1CONbits.T1OSCEN = 1;      // Dedicated Timer1 oscillator circuit enable
    T1CONbits.nT1SYNC = 1;      // Do not synchronize external clock input
    
    T1GCONbits.TMR1GE = 0;      // Timer 1 counts regardless of Timer1 gate fct
}

void init_interrupt(void) {
    // General Interruption configuration
    INTCONbits.GIE = 1;        // Enables all active interrupts
    INTCONbits.PEIE = 1;       // Enables all active peripheral interrupts 
    
    // timer Interruption configuration
    PIE1bits.TMR1IE = 1;     // Enable the Timer1 Gate Acquisition interrupt
    
    // USART Receive Interrupt Enable bit
    PIE1bits.RCIE = 1;      // Enables the USART receive interrupt
}

void init(void) {
    pin_config();
    init_timer1();
    init_interrupt();
}

void icremente_time(Time* time) {
    (*time).seconds++;
    if((*time).seconds == 60) {
        (*time).seconds = 0;
        (*time).minutes++;
        if((*time).minutes == 60) {
            (*time).minutes = 0;
            (*time).hours++;
            if((*time).hours == 24) {
                (*time).hours = 0;
                (*time).days++;
                if((*time).months == 2 && (*time).days == 29) {
                    (*time).days = 0;
                    (*time).months++;
                } else if((*time).months == 4 || (*time).months == 6 || (*time).months == 9 || (*time).months == 11) {
                    if((*time).days == 31) {
                        (*time).days = 0;
                        (*time).months++;
                    }
                } else {
                    if((*time).days == 32) {
                        (*time).days = 0;
                        (*time).months++;
                        if((*time).months == 13) {
                            (*time).years++;
                        }
                    }
                }
            }
        }
    }
}
