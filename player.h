#ifndef PLAYER_H
#define PLAYER_H

#include "stdbool.h"
#include "field.h"
#include "io.h"

//enum condition {normal, stunned, rooted};
struct player
{
  unsigned char hp;         /* health bar */
  unsigned char xpos;       /* x position on the field */
  unsigned char ypos;       /* y position on the field */
  bool move_en;             /* movement enable  */
  bool attack_en;           /* attack enable */
  //enum condition cond;      /* player condition */
};

struct player player1;
struct player player2;

char init_players();
void move_players();
void move_player1();
void move_player2();
#endif
