/*
 * led_matrix.h
 *
 * Created: 11/24/2016 9:26:29 PM
 *  Author: Marco Morelos
 */ 


#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_

#define MATRIXROWPORT PORTB
#define MATRIXDATAPORT PORTA

void disableMatrix(){
    MATRIXDATAPORT |= 0022; // disable output (OE High)
}

void enableMatrix(){
    MATRIXDATAPORT &= ~0022; // enable output (OE low)
}

void displayMatrixRow(unsigned char row, unsigned short red[], unsigned short green[]){
    
    if(row > 7) return;
    
    MATRIXROWPORT = 1 << row;
    
    unsigned short redData = red[row];
    unsigned short greenData = green[row];
    
    MATRIXDATAPORT |= 0022; // disable output (OE High)
    char i;
    for (i = 0; i < 16 ; i++) {
        MATRIXDATAPORT &= 0322; // Set clock LOW, Clear data bit
        // set SER to next bit of data to be sent.
        MATRIXDATAPORT |= (redData   & 0x0001 << (15 - i)) ? 0000 : 0001;
        MATRIXDATAPORT |= (greenData & 0x0001 << (15 - i)) ? 0000 : 0010;
        // Set CLK high. Rising edge latches SER into Shift Register
        MATRIXDATAPORT |= 0044;
    }
    MATRIXDATAPORT &= 0322; // Set clock LOW, Clear data bit
    // set CLK High. Copies “Shift” register to “Storage” register
    MATRIXDATAPORT |= 0044;
    // clears all lines. Output enabled
    MATRIXDATAPORT &= 0300;
    return;
}


#endif /* LED_MATRIX_H_ */