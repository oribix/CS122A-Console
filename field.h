#ifndef FIELD_H
#define FIELD_H

//Field Boundaries
#define FIELD_TOP   2
#define FIELD_BOT   0
#define FIELD_LEFT  0
#define FIELD_RIGHT 5

/*
enum panel_status {normal};
enum panel_owner {player1, player2};

struct panel
{
  enum panel_owner owner;
  //panel_status status;
};
*/

void init_field();
void update_gun();
void update_cannon();
void update_lance();

// bit vector
  // bit 0: 0 = player1 is owner; 1 = player2 is owner 
  // bit 1: player1 cannon attack
  // bit 2: player2 cannon attack
  // bit 3: player1 lance attack
  // bit 4: player2 lance attack
  // bit 5: player1 shield
  // bit 6: player2 shield
  // bit 7: RESERVED
unsigned char field[6][3];

#endif
