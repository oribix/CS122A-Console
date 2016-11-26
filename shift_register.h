/*
 * shift_register.h
 *
 * Created: 10/21/2016 4:47:11 PM
 *  Author: Marco Morelos
 */ 


#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

#include "bit.h"



/* 
    See SN74HC595 for pin out
    
    SR_transmit assumes that RCLK and SRCLK are bound together by default.
    SR_transmit also assumes that all 3 inputs are on the same PORTx.
*/

/* 
    Functionality - transmits data through the shift register
    Parameter: data: Contains the bits being transmitted (32 maximum)
               size: # bits to be transmitted (8 without daisy chaining)
               port: The PORTx that the shift register is connected to
               SER:  Pin number for SER
               OE:   Pin number for OE
               CLK:  Pin number for CLK
    Returns: VOID
*/

void SR_transmit(unsigned long data, unsigned char size,
                 volatile unsigned char *port, unsigned char SER,
                 unsigned char OE, unsigned char clk) {
	// disable output (OE High)
    //PORTA |= 0002;
    *port = SetBit(*port, OE, 1);
    ////*port |= 0002;
    
	char i;
	for (i = 0; i < size ; i++) {
		// Set clock LOW, Clear SER bit
        //PORTA &= 0372;
        *port = SetBit(*port, SER, 0);
        *port = SetBit(*port, clk, 0);
		////*port &= 0372;
        
        // set SER to next bit of data to be sent.
		//PORTA |= (data & 0x0001 << (15 - i)) ? 0001 : 0000;
        unsigned long bMask = 0x01 << (size - 1 - i);
        unsigned char b = (data & bMask) ? 0x01 : 0x00;
        *port = SetBit(*port, SER, b);
        ////*port |= (data & 0x01 << (15 - i)) ? 0001 : 0000;
        
		// Set CLK high. Rising edge latches SER into Shift Register
		//PORTA |= 0004;
        *port = SetBit(*port, clk, 1);
        ////*port |= 0004;
	}
    
    // Set clock LOW, Clear SER bit
	//PORTA &= 0372;
    *port = SetBit(*port, clk, 0);
    *port = SetBit(*port, SER, 0);
    ////*port &= 0372;
	
    // set CLK High. Copies “Shift” register to “Storage” register
	//PORTA |= 0004;
    *port = SetBit(*port, clk, 1);
    ////*port |= 0004;
    
	// clears all lines. Output enabled (OE low)
	//PORTA &= 0370;
    *port = SetBit(*port, OE, 0);
    ////*port &= 0370;
}

#endif /* SHIFT_REGISTER_H_ */