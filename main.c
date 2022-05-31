#include "device_config.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


#define _XTAL_FREQ 8000000
#define SWEEP_STEP 1
#define GREEN1 PORTDbits.RD6
#define YELLOW1 PORTDbits.RD5
#define RED1 PORTDbits.RD4
#define GREEN2 PORTDbits.RD2
#define YELLOW2 PORTDbits.RD1
#define RED2 PORTDbits.RD0
#define PEDESTRIAN1 PORTDbits.RD7
#define PEDESTRIAN2 PORTDbits.RD3

int solicitudes = 0;

void portsInit(void);
void interruptInit(void);
void delay_1s(void);
void interrupt_int0(void);
    
void main(void) {
    int i=0;
    portsInit();
    interruptInit();
    
    while(1){
        for(i=0; i<3; i++){
            RED1 = 1;
            YELLOW1 = 0;
            GREEN1 = 0;
            RED2 = 0;
            YELLOW2 = 0;
            GREEN2 = 1;
            PEDESTRIAN1 = 0;
            PEDESTRIAN2 = 0;
            delay_1s();
        }
        for(i=0; i<1; i++){
            RED1 = 1;
            YELLOW1 = 0;
            GREEN1 = 0;
            RED2 = 0;
            YELLOW2 = 1;
            GREEN2 = 0;
            PEDESTRIAN1 = 0;
            PEDESTRIAN2 = 0;
            delay_1s();
        }
        if(solicitudes!=0){
            for(i=0; i<solicitudes; i++){
            RED1 = 1;
            YELLOW1 = 0;
            GREEN1 = 0;
            RED2 = 1;
            YELLOW2 = 0;
            GREEN2 = 0;
            PEDESTRIAN1 = 1;
            PEDESTRIAN2 = 1;
            delay_1s();
        }
            solicitudes = 0;
        }
        for(i=0; i<3; i++){
            RED1 = 0;
            YELLOW1 = 0;
            GREEN1 = 1;
            RED2 = 1;
            YELLOW2 = 0;
            GREEN2 = 0;
            PEDESTRIAN1 = 0;
            PEDESTRIAN2 = 0;
            delay_1s();
        }
        for(i=0; i<1; i++){
            RED1 = 0;
            YELLOW1 = 1;
            GREEN1 = 0;
            RED2 = 1;
            YELLOW2 = 0;
            GREEN2 = 0;
            PEDESTRIAN1 = 0;
            PEDESTRIAN2 = 0;
            delay_1s();
        }
    }
    
}    

void portsInit(void){
    OSCCON = 0b01100100;    // Set the internal oscillator to 8MHz and stable
    ANSELBbits.ANSB2 = 0;   // PUSH-BUTTON
    TRISBbits.TRISB2 = 1;
    ANSELBbits.ANSB1 = 0;   // PUSH-BUTTON debounce
    TRISBbits.TRISB1 = 0;
    ANSELBbits.ANSB0 = 0;   // Interrupt
    TRISBbits.TRISB0 = 1;
    ANSELD = 0;             //  LEDs   
    TRISD  = 0x00;
}

void interruptInit(void){
    RCONbits.IPEN = 1;
    INTCON2bits.INTEDG0 = 1;
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
    INTCONbits.GIEH = 1;
}

void delay_1s(void){
    TMR0H = 0x0B;
    TMR0L = 0xDC;
    INTCONbits.TMR0IF = 0;
    T0CON = 0b10000100;
    while(INTCONbits.TMR0IF == 0);
    T0CON = 0x00;
    INTCONbits.TMR0IF = 0;
}

void __interrupt(high_priority) interrupt_int0(void){
    __delay_ms(20);
    if(PORTBbits.RB0 ==1){
        if(solicitudes<8){
            solicitudes++;
        }
    }
    INTCONbits.INT0IF = 0;
}