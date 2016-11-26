/*
 * SNES_controller.h
 *
 * Created: 11/25/2016 2:54:44 PM
 *  Author: Marco Morelos
 */ 


#ifndef SNES_CONTROLLER_H_
#define SNES_CONTROLLER_H_

#include "bit.h"

#define SNES_DDR   DDRD
#define SNES_PORT  PORTD
#define SNES_PIN   PIND
#define SNES_DATA  4
#define SNES_LATCH 5
#define SNES_CLOCK 6

void SNES_init(){
    //data is an input
    SNES_DDR = SetBit(SNES_DDR, SNES_DATA, INPUT_PIN);
    SNES_PORT = SetBit(SNES_PORT, SNES_DATA, high);
    //latch is output, starts low
    SNES_DDR = SetBit(SNES_DDR, SNES_LATCH, OUTPUT_PIN);
    SNES_PORT = SetBit(SNES_PORT, SNES_LATCH, low);
    //clock is output, starts high
    SNES_DDR = SetBit(SNES_DDR, SNES_CLOCK, OUTPUT_PIN);
    SNES_PORT = SetBit(SNES_PORT, SNES_CLOCK, high);
}

//gets the controller vector for SNES
unsigned short getSNESVector(){
    //make sure clock is high
    SNES_PORT = SetBit(SNES_PORT, SNES_CLOCK, high);
    
    //latch inputs
    SNES_PORT = SetBit(SNES_PORT, SNES_LATCH, high);
    SNES_PORT = SetBit(SNES_PORT, SNES_LATCH, low);
    
    unsigned short snesVector = 0;
    unsigned char i;
    for(i = 0; i < 16; i++) {
        //make space for next bit
        snesVector <<= 1;
        //get data bit
        snesVector |= GetBit(SNES_PIN, SNES_DATA);
        //prepare next bit to be read
        pulse01(&SNES_PORT, SNES_CLOCK);
    }
    
    return ~snesVector; //inverts the snesVector for easier reading
}



#endif /* SNES_CONTROLLER_H_ */