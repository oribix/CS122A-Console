#ifndef ATTACK_H 
#define ATTACK_H

#include "player.h"
#include "field.h"

#define MAX_LINEAR 5

#define LEFT 0
#define RIGHT 1

//instant linear collision
bool linear_collide(unsigned char range);
//update linear horizontal projectiles
void update_hprojectile (unsigned char * attackv, 
                         bool direction);
void create_projectile (unsigned char * fieldv, bool pNum);
void destroy_projectile (unsigned char * fieldv,
                         unsigned char xpos,
                         unsigned char ypos);

//checks collisions
bool check_collision(unsigned char * fieldv, bool pNum);

//shockwave attack
void init_shockwave();
unsigned char shockwave1[3]; //player1
unsigned char shockwave2[3]; //player2

#endif