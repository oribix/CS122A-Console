#include <stddef.h>
#include <avr/io.h>
#include "io.h"
#include "scheduler.h"

/* Initializes scheduler tasks */
unsigned char tasks_init() {
  // Priority assigned to lower position tasks in array
  unsigned char i = 0;
  //initialize the tasks
  tasks[i].TickFct      = &TickFct_Example;
  tasks[i].period       = 200;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_GetInput;
  tasks[i].period       = 16;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_movePlayer1;
  tasks[i].period       = 16;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_movePlayer2;
  tasks[i].period       = 16;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_p1ShockWave;
  tasks[i].period       = 16;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_p2ShockWave;
  tasks[i].period       = 16;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_updateShockwave;
  tasks[i].period       = 256;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_checkCollisions;
  tasks[i].period       = 16;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_updateMatrix;
  tasks[i].period       = 16;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_printMatrix;
  tasks[i].period       = 2;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  tasks[i].TickFct      = &TickFct_reset;
  tasks[i].period       = 200;
  tasks[i].elapsedTime  = tasks[i].period;
  tasks[i].state        = -1;
  ++i;
  
  return i;
}

//literally just here for reference
enum SM1_States { SM1_init, SM1_wait } SM1_State;
int TickFct_Example(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/

  switch(state) { // Transitions
    case -1:
    state = SM1_init;
    break;
    case SM1_init:
      if (1) {
        state = SM1_wait;
      }
      break;
    case SM1_wait:
      if (1) {
        state = SM1_wait;
      }
      break;
    default:
    state = -1;
  } // Transitions

  switch(state) { // State actions
    case SM1_init:
      LCD_ClearScreen();
      const unsigned char bdots[13] = "Battle Dots!";
      LCD_DisplayString(0x01, bdots);
      break;
    default: // ADD default behavior below
      //do nothing
      break;
  } // State actions

  SM1_State = state;
  return state;
}

//gets input from SNES controllers
enum SM4_States { SM4_init } SM4_State;
int TickFct_GetInput(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  
  switch(state) { //Transitions
    case -1:
    state = SM4_init;
    break;
    
    case SM4_init:
    if (1) {
      state = SM4_init;
    }
    break;
    
    default:
    state = -1;
    break;
  } // Transitions

  switch(state) { // State actions
    case SM4_init:
    get_inputs();
    break;
    
    default:
    break;
  } // State actions

  SM4_State = state;
  return state;
}

//lights up the matrix
enum SM2_States { SM2_init} SM2_State;
int TickFct_printMatrix(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  static unsigned char i;
  
  switch(state) { // Transitions
    case -1:
      state = SM2_init;
      break;
    
    case SM2_init:
      if (1) {
        state = SM2_init;
      }        
      break;
    
    default:
      state = -1;
  } // Transitions

  switch(state) { // State actions
    case SM2_init:
      PORTB = 0x00; //turn off output
      //if (i % 2) transmit_data(red1, green1);
      //else transmit_data(red2, green2);
      transmit_data(red[i], green[i]);
      PORTB = 0x01 << i;
      i++;
      if (i == 8) i = 0;
      break;
      
    default:
      break;
  } // State actions

  SM2_State = state;
  return state;
}

//updates the matrix
enum SM3_States { SM3_init} SM3_State;
int TickFct_updateMatrix(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  
  switch(state) { //Transitions
    case -1:
    state = SM3_init;
    break;
    
    case SM3_init:
    if (1) {
      state = SM3_init;
    }
    break;
    
    default:
    state = -1;
  } // Transitions

  switch(state) { // State actions
    case SM3_init:
    updateMatrix();
    break;
    
    default:
    break;
  } // State actions

  SM3_State = state;
  return state;
}

enum SM5_States { SM5_init, SM5_letGo} SM5_State;
int TickFct_movePlayer1(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  
  static unsigned short prev;
  
  switch(state) { //Transitions
    case -1:
      state = SM5_init;
      break;
    
    case SM5_init:
      if (p1_input & SNES_DPAD) {
        state = SM5_letGo;
        move_player1();
        prev = p1_input & SNES_DPAD;
      }        
      else state = SM5_init;
      break;
    
    case SM5_letGo:
      if (!(p1_input & SNES_DPAD)) state = SM5_init;
      else state = SM5_letGo;
      break;
    
    default:
      state = -1;
      break;
  } // Transitions

  switch(state) { // State actions
    case SM5_init:
      break;
    
    case SM5_letGo:
      if (prev != (p1_input & SNES_DPAD)) {
        move_player1();
        prev = p1_input & SNES_DPAD;
      }
      break;
    
    default:
      break;
  } // State actions

  SM5_State = state;
  return state;
}

enum SM6_States { SM6_init, SM6_letGo} SM6_State;
int TickFct_movePlayer2(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  
  static unsigned short prev;
  
  switch(state) { //Transitions
    case -1:
      state = SM6_init;
      prev = 0;
      break;
    
    case SM6_init:
      if (p2_input & SNES_DPAD) {
         state = SM6_letGo;
         move_player2();
         prev = p2_input & SNES_DPAD;
      }
      else state = SM6_init;
      break;
    
    case SM6_letGo:
      if (p2_input & SNES_DPAD)
        state = SM6_letGo;
      else {
        state = SM6_init;
      }
      break; 
    
    default:
      state = -1;
      break;
  } // Transitions

  switch(state) { // State actions
    case SM6_init:
      break;
    
    case SM6_letGo:
    if (prev != (p2_input & SNES_DPAD)) {
      move_player2();
      prev = p2_input & SNES_DPAD;
    }
    break;
    
    default:
    break;
  } // State actions

  SM6_State = state;
  return state;
}

//updates the shockwave 2d vector
enum SM7_States { SM7_init, SM7_wait} SM7_State;
int TickFct_updateShockwave(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  
  switch(state) { //Transitions
    case -1:
    state = SM7_init;
    break;
    
    case SM7_init:
      if (1) {
        state = SM7_wait;
      }
      break;
    
    case SM7_wait:
      if (1) {
        state = SM7_wait;
      }
      break;
    
    default:
      state = -1;
      break;
  } // Transitions

  switch(state) { // State actions
    case SM7_init:
      init_shockwave();
      break;
    
    case SM7_wait:
      update_hprojectile(shockwave1, RIGHT); //moves right
      update_hprojectile(shockwave2, LEFT); //moves left
      break;
    
    default:
      break;
  } // State actions

  SM7_State = state;
  return state;
}

//Checks for collisions with attacks
enum SM8_States { SM8_init, SM8_wait} SM8_State;
int TickFct_checkCollisions(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  
  switch(state) { //Transitions
    case -1:
    state = SM8_init;
    break;
    
    case SM8_init:
    if (1) {
      state = SM8_wait;
    }
    break;
    
    case SM8_wait:
    if (1) {
      state = SM8_wait;
    }
    
    default:
    state = -1;
    break;
  } // Transitions

  switch(state) { // State actions
    case SM8_init:
    break;
    
    case SM8_wait:
      //check player 2 collision with player1 shock wave
      if (check_collision(shockwave1, 1)) {
        player2.hp -= 10;
        destroy_projectile(shockwave1, player2.xpos, player2.ypos);
      }
      //check player 1 collision with player 2 shock wave
      if (check_collision(shockwave2, 0)) {
        player1.hp -= 10;
        destroy_projectile(shockwave2, player1.xpos, player1.ypos);
      }
      break;
    
    default:
      break;
  } // State actions

  SM8_State = state;
  return state;
}

enum SM9_States { SM9_init, SM9_wait} SM9_State;
int TickFct_p1ShockWave(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  static unsigned char i;
  switch(state) { //Transitions
    case -1:
    state = SM9_init;
    i = 0;
    break;
    
    case SM9_init:
    if (p1_input & SNES_A) {
      state = SM9_wait;
      create_projectile(shockwave1, 0);
    }
    else state = SM9_init;
    break;
    
    case SM9_wait:
    if (i == 61) {
      state = SM9_init;
      i = 0;
    }
    else state = SM9_wait;
    break;
    
    default:
    state = -1;
    break;
  } // Transitions

  switch(state) { // State actions
    case SM9_init:
    break;
    
    case SM9_wait:
    i++;
    break;
    
    default:
    break;
  } // State actions

  SM9_State = state;
  return state;
}

enum SM10_States { SM10_init, SM10_wait} SM10_State;
int TickFct_p2ShockWave(int state) {
  /*VARIABLES MUST BE DECLARED STATIC*/
  /*e.g., static int x = 0;*/
  static unsigned char i;
  switch(state) { //Transitions
    case -1:
    state = SM10_init;
    i = 0;
    break;
    
    case SM10_init:
    if (p2_input & SNES_A) {
      state = SM10_wait;
      create_projectile(shockwave2, 1);
    }
    else state = SM10_init;
    break;
    
    case SM10_wait:
    if (i == 61) {
      state = SM10_init;
      i = 0;
    }
    else state = SM10_wait;
    break;
    
    default:
    state = -1;
    break;
  } // Transitions

  switch(state) { // State actions
    case SM10_init:
    break;
    
    case SM10_wait:
    i++;
    break;
    
    default:
    break;
  } // State actions

  SM10_State = state;
  return state;
}

void TickFct_reset() {
  if (p1_input & SNES_SL || p2_input & SNES_SL) {
    SM1_State = -1;
    SM2_State = -1;
    SM3_State = -1;
    SM4_State = -1;
    SM5_State = -1;
    SM6_State = -1;
    SM7_State = -1;
    SM8_State = -1;
    SM9_State = -1;
    SM10_State = -1;
    init_players();
    init_field();
  }    
}