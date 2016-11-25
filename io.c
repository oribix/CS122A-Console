#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"
#include "attack.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))
          
/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC    // port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD  // port connected to pins 4 and 6 of LCD disp.
#define RS 6      // pin number of uC connected to pin 4 of LCD disp.
#define E 7      // pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
  LCD_WriteCommand(0x01);
}

void LCD_init(void) {

    //wait for 100 ms.
  delay_ms(100);
  LCD_WriteCommand(0x38);
  LCD_WriteCommand(0x06);
  LCD_WriteCommand(0x0f);
  LCD_WriteCommand(0x01);
  delay_ms(10);             
}

void LCD_WriteCommand (unsigned char Command) {
  CLR_BIT(CONTROL_BUS,RS);
  DATA_BUS = Command;
  SET_BIT(CONTROL_BUS,E);
  asm("nop");
  CLR_BIT(CONTROL_BUS,E);
  delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
  SET_BIT(CONTROL_BUS,RS);
  DATA_BUS = Data;
  SET_BIT(CONTROL_BUS,E);
  asm("nop");
  CLR_BIT(CONTROL_BUS,E);
  delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
  LCD_ClearScreen();
  unsigned char c = column;
  while(*string) {
    LCD_Cursor(c++);
    LCD_WriteData(*string++);
  }
}

void LCD_Cursor(unsigned char column) {
  if ( column < 17 ) { // 16x1 LCD: column < 9
                       // 16x2 LCD: column < 17
     LCD_WriteCommand(0x80 + column - 1);
   } else {
     LCD_WriteCommand(0xB8 + column - 9); // 16x1 LCD: column - 1
                                          // 16x2 LCD: column - 9
  }
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<1625;j++) //775
      {
        asm("nop");
      }
}


/* Transmit data for matrix on PORTD
   THIS FUNCTION IS IN OCTAL FOR CONVENIENCE
   
   RED
   D0 = SER   0 = ON, 1 = OFF
   D1 = OE    0 = enabled, 1 = disabled
   D2 = CLK   SER is latched on rising edge
   
   GREEN
   D3 = SER   0 = ON, 1 = OFF
   D4 = OE    0 = enabled, 1 = disabled
   D5 = CLK   SER is latched on rising edge 

PORTD bit layout:               -76 543 210
DONT MESS WITH LCD BITS         LCD GRN RED
*/
void transmit_data(unsigned short redData,
                   unsigned short greenData) {
  PORTD |= 0022; // disable output (OE High) 
  char i;
  for (i = 0; i < 16 ; i++) {
    PORTD &= 0322; // Set clock LOW, Clear data bit
    // set SER to next bit of data to be sent.
    PORTD |= (redData   & 0x0001 << (15 - i)) ? 0000 : 0001;
    PORTD |= (greenData & 0x0001 << (15 - i)) ? 0000 : 0010;
    // Set CLK high. Rising edge latches SER into Shift Register
    PORTD |= 0044;
  }
  PORTD &= 0322; // Set clock LOW, Clear data bit
  // set CLK High. Copies “Shift” register to “Storage” register
  PORTD |= 0044;
  // clears all lines. Output enabled
  PORTD &= 0300;
  return;
}

/* Gets the input vectors from SNES controllers
   A0 = controller 1 Data
   A1 = controller 1 Latch
   A2 = controller 1 Clock
   
   A3 = controller 2 Data
   A4 = controller 2 Latch
   A5 = controller 2 Clock
   
   A6 = RESERVED 
   A7 = RESERVED */
void get_inputs() {
  //make sure clock is high 
  PORTA |=  0044;

  //latch inputs
  PORTA |=  0022;
  PORTA &= ~0022;
  
  //clear controller bit vectors
  p1_input = 0x0000;
  p2_input = 0x0000;
  
  unsigned char i;
  for(i = 0; i < 16; i++) {
    p1_input += (GET_BIT(PINA, 0)) ? 0x0001 : 0x0000;
    p2_input += GET_BIT(PINA, 3) ? 0x0001 : 0x0000;
    
    if (i < 15) {
      p1_input <<= 1;
      p2_input <<= 1;
    }
    
    //prepare next bit to be read
    PORTA &= ~0044; //set clock low
    PORTA |=  0044; //set clock high
  }
  
  p1_input = ~p1_input;
  p2_input = ~p2_input;
  
  return;
}

//converts field x position to matrix x position
unsigned char get_xpos(unsigned char pos) {
  switch(pos){
    case 0: return 2;
    case 1: return 4;
    case 2: return 6;
    case 3: return 9;
    case 4: return 11;
    case 5: return 13;
  }
  return 0;
}

//converts field ypos to matrix ypos
unsigned char get_ypos( unsigned char pos) {
  return pos + pos + 1;
}

void updateMatrix() {
  cleanMatrix();
  updateField();
  readAttacks(shockwave1);
  readAttacks(shockwave2);
  updatePlayers();
  updateTimerGuage();
  return;
}

void cleanMatrix() {
  unsigned char i;
  for (i = 0; i < 8; i++) {
    green[i] = 0x0000;
    red[i]   = 0x0000;
  }
}

/* Player 1 field is green. Player 2 field is red */
void updateField(/*unsigned char ** field*/) {
  unsigned char i, j, xpos, ypos;
  
  if (NULL == field) return;
  
  for (i = 0; i < 6; i++) {
    for (j = 0; j < 3; j++) {
      xpos = get_xpos(i);
      ypos = get_ypos(j);
      
      //octal because tiles are 3 LEDs wide
      //player 1 is owner
      if ((field[i][j] & 0x01) == 0x00) {
        green[ypos - 1] |=  (0000007 << (xpos - 1));
        green[ypos]     |=  (0000007 << (xpos - 1));
        green[ypos + 1] |=  (0000007 << (xpos - 1));
        green[ypos]     &= ~(0000002 << (xpos - 1));
      }
      
      //player 2 is owner
      if ((field[i][j] & 0x01) == 0x01) {
        red[ypos - 1] |=  (0000007 << (xpos - 1));
        red[ypos]     |=  (0000007 << (xpos - 1));
        red[ypos + 1] |=  (0000007 << (xpos - 1));
        red[ypos]     &= ~(0000002 << (xpos - 1));
      }
    }
  }
  return;
}

/* Player 1 is red. Player 2 is green */
void updatePlayers() {
  unsigned char xpos, ypos;
  
  //update player 1
  xpos = get_xpos(player1.xpos) - 1; //1 is offset
  ypos = get_ypos(player1.ypos);
  green[ypos - 1] &= ~(0000002 << xpos); //2
  green[ypos]     &= ~(0000007 << xpos); //7
  green[ypos + 1] &= ~(0000002 << xpos); //2
  red  [ypos - 1] |=  (0000002 << xpos);
  red  [ypos]     |=  (0000007 << xpos);
  red  [ypos + 1] |=  (0000002 << xpos);
  
  //update player2
  xpos = get_xpos(player2.xpos) - 1;
  ypos = get_ypos(player2.ypos);
  green[ypos - 1] |=  (0000002 << xpos);
  green[ypos]     |=  (0000007 << xpos);
  green[ypos + 1] |=  (0000002 << xpos);
  red  [ypos - 1] &= ~(0000002 << xpos);
  red  [ypos]     &= ~(0000007 << xpos);
  red  [ypos + 1] &= ~(0000002 << xpos);
  
  return;
}

unsigned short fieldrToMatrix(unsigned char fieldrow) {
  unsigned short mRow = 0;
  unsigned char i, filter;
  filter = 0x01;
  for (i = 0; i < 6; i++) {
    if (fieldrow & filter) mRow |= (0x0001 << get_xpos(i));
    filter <<= 1;
  }
  return mRow;
}

void readAttacks(unsigned char * fieldv) {
  unsigned char ypos, i;
  unsigned short mRow;
  for (i = 0; i < 3; i++) {
    ypos = get_ypos(i);
    mRow = fieldrToMatrix(fieldv[i]);
    
    green[ypos] |= mRow;
    red[ypos] |= mRow; 
  }
  return;
}

void updateTimerGuage() {
  return;
}