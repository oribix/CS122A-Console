#include "attack.h"

//instant linear collision
bool linear_collide (unsigned char range) {
  if ((player2.xpos - player1.xpos) <= range) return true;
  else return false;
}

/* direction: 0 = left, 1 = right 
   note: right = from 0 to 7
         left  = from 7 to 0 
   ONLY PASS IN ARRAYS OF SIZE 3 */
void update_hprojectile (unsigned char * attackv,
                         bool direction) {
  unsigned char i;
  for (i = 0; i < 3; i++) {
    if (direction) attackv[i] <<= 1; //right
    else attackv[i] >>= 1; //left
  }
  return;
}

/* fix me*/
//pNum: 0 = player 1, 1 = player2
bool check_collision(unsigned char * fieldv, bool pNum) {
  bool collided = false;
  if (pNum) { //player 2
    collided = (fieldv[player2.ypos] & (0x01 << player2.xpos))
               ? true : false;
  }
  else { //player 1
    collided = (fieldv[player1.ypos] & (0x01 << player1.xpos))
               ? true : false;
  }
  return collided;
}


//pNum: 0 = player1, 1 = player2
void create_projectile(unsigned char * fieldv, bool pNum) {
  if (pNum) { //player 2
    fieldv[player2.ypos] |= (0x01 << player2.xpos); 
  }
  else { //player 1
    fieldv[player1.ypos] |= (0x01 << player1.xpos);
  }
  return;
}

void destroy_projectile (unsigned char * fieldv,
                         unsigned char xpos,
                         unsigned char ypos) {
  fieldv[ypos] &= ~(0x01 << xpos);
  return;
}

void init_shockwave() {
  unsigned char i;
  for (i = 0; i < 3 ; i++) {
    shockwave1[i] = 0x00;
    shockwave2[i] = 0x00;
  }
  return;
}