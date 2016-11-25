#ifndef __io_h__
#define __io_h__

#include "player.h"
#include "field.h"

#define SNES_R      0x0010
#define SNES_L      0x0020
#define SNES_X      0x0040     
#define SNES_A      0x0080     
#define SNES_RIGHT  0x0100     
#define SNES_LEFT   0x0200  
#define SNES_DOWN   0x0400
#define SNES_UP     0x0800
#define SNES_ST     0x1000
#define SNES_SL     0x2000
#define SNES_Y      0x4000
#define SNES_B      0x8000

//special controller filters
#define SNES_DPAD   0x0F00

//SNES Controller
  //used to store SNES controller inputs for players
  unsigned short p1_input;
  unsigned short p2_input;
  //SNES controller functions
  void get_inputs();
  
//LCD
  void LCD_init();
  void LCD_ClearScreen(void);
  void LCD_WriteCommand (unsigned char Command);
  void LCD_Cursor (unsigned char column);
  void LCD_DisplayString(unsigned char column,
                         const unsigned char *string);

// LED matrix 
  // stores positions of red/green matrix lights
  unsigned short green[8];
  unsigned short red[8];
  //matrix functions
  void updateMatrix ();
  void updatePlayers();
  void updateField();
  void readAttacks(unsigned char *fieldv);
  void updateTimerGuage();
  void cleanMatrix();
  void transmit_data(unsigned short redData,
                     unsigned short greenData);

unsigned short fieldrToMatrix(unsigned char fieldrow);

//timer delay
  void delay_ms(int miliSec);
  
#endif

