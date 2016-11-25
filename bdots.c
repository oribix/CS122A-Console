#include <avr/io.h>
#include <avr/sleep.h>

#include "field.h"
#include "player.h"
#include "attack.h"
#include "io.h"
#include "scheduler.h"
#include "timer.h"

#include "bdots.h"

#define NULL 0

/* This code was created with the aid of the Riverside-Irvine State
   machine Builder tool */

int main() {
  //initialize pins
  DDRA = 0066; PORTA = 0311;
  DDRB = 0xFF; PORTB = 0x00;
  DDRC = 0xFF; PORTC = 0x00;
  DDRD = 0xFF; PORTD = 0x00;
  
  LCD_init(); //initialize LCD
  
  //initialize task manager
  if (tasks_init() != TASKSNUM) {
    LCD_ClearScreen();
    const unsigned char tskNmErr[16] = "error: TASKSNUM";
    LCD_DisplayString(0x01, tskNmErr);
    while(1);
  }
  
  //initialize game objects
  init_players();
  init_field();
  unsigned short clear_matrix = 0;
  transmit_data(clear_matrix, clear_matrix);
  
  //cool animation
  //PORTB = 0x01;
  //unsigned short red = 0xAAAA;
  //unsigned short green = 0x5555;
  //transmit_data(red, green);
  //
  //while(1){
  //  int i;
  //  for (i = 0; i < 7; i++) {
  //    PORTB = 0x01 << i;
  //    delay_ms(71);
  //  }
  //
  //  for (i = 7; i > 0; i--) {
  //    PORTB = 0x01 << i;
  //    delay_ms(71);
  //  }
  //
  //}
  
  ////test controller inputs
  //PORTB = 0xFF;
  //while(1) {
  //  get_inputs();
  //  transmit_data(p1_input, p2_input);
  //  LCD_ClearScreen();
  //  switch(p1_input) {
  //    case SNES_R   : LCD_DisplayString(1, "R");    break;
  //    case SNES_L   : LCD_DisplayString(1, "L");    break;
  //    case SNES_X   : LCD_DisplayString(1, "X");    break;
  //    case SNES_A   : LCD_DisplayString(1, "A");    break;
  //    case SNES_UP  : LCD_DisplayString(1, "UP");   break;
  //    case SNES_DOWN: LCD_DisplayString(1, "DOWN"); break;
  //    default: break;
  //  }  
  //  delay_ms(16);
  //}
  
  //PORTB = 0xFF;
  //init_shockwave();
  ////shockwave1[1] = 0x01;
  ////shockwave2[1] = 0x20;
  //while(1) {
  //  update_hprojectile(shockwave1, RIGHT);
  //  update_hprojectile(shockwave2, LEFT);
  //  cleanMatrix();
  //  readAttacks(shockwave1);
  //  readAttacks(shockwave2);
  //  if (check_collision(shockwave1, 1)) {
  //    LCD_DisplayString(1, "collided");
  //    destroy_projectile(shockwave1, player2.xpos, player2.ypos);
  //  }
  //  transmit_data(red[3], green[3]);
  //  delay_ms(500);
  //}
  
  //initialize timer
  TimerSet(TASKSGCD);
  TimerOn();
  
  /* timer interrupts take care of the rest .
     See timerISR() in timer.h */
  while(1) {}
  return 0;
}