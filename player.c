#include "player.h"

/* initializes player attributes. pNum = player number */
char init_players() {
  player1.hp = 80;
  player1.xpos = 1; // 1
  player1.ypos = 1; // 1
  player1.move_en = true;
  
  player2.hp = 80;
  player2.xpos = 4; // 1
  player2.ypos = 1; // 1
  player2.move_en = true;
  
  return 0;
}

void move_player1() {
  //do nothing if control is disabled
  if (!player1.move_en) return;
  
  unsigned char xpos = player1.xpos;
  unsigned char ypos = player1.ypos;
  
  if (player1.move_en) {
    
    switch (p1_input & SNES_DPAD) {
      case SNES_UP:
        if (ypos < FIELD_TOP) player1.ypos += 1;
        break;
      case SNES_DOWN:
        if (ypos > FIELD_BOT) player1.ypos -= 1;
        break;
      case SNES_RIGHT:
        if (!(field[xpos + 1][ypos] & 0x01)) //player1 owner
          player1.xpos += 1;
        break;
      case SNES_LEFT:
        if (xpos > FIELD_LEFT) player1.xpos -= 1;
        break;
      default:
        break;
    }      
  }
  return;
}

/* player 2 controls are inverted so that the player can
     sit across from you*/
void move_player2() {
  //do nothing if control is disabled
  if (!player2.move_en) return;
  
  unsigned char xpos = player2.xpos;
  unsigned char ypos = player2.ypos;
  
  switch (p2_input & SNES_DPAD) {
    case SNES_UP:
      if (ypos > FIELD_BOT) player2.ypos -= 1;
      break;
    
    case SNES_DOWN:
      if (ypos < FIELD_TOP) player2.ypos += 1;
      break;
    
    case SNES_RIGHT:
      if (field[xpos - 1][ypos] & 0x01) //player2 owner
      player2.xpos -= 1;
      break;
    
    case SNES_LEFT:
      if (xpos < FIELD_RIGHT) player2.xpos += 1;
      break;
    
    default:
      break;
  }    
  return;
}