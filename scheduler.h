
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef SCHEDULER_H
#define SCHEDULER_H

#define TASKSGCD 2
#define TASKSNUM 11

#include "attack.h"

//Struct for Tasks represent a running process in our simple real-time operating system
typedef struct task{
  // Tasks should have members that include: state, period,
  //a measurement of elapsed time, and a function pointer.
  int           state;        //Task's current state
  unsigned long period;       //Task period
  unsigned long elapsedTime;  //Time elapsed since last task tick
  int (*TickFct)(int);        //Task tick function
} task;

task tasks[TASKSNUM];

//used to initialize tasks
unsigned char tasks_init();

//tick function prototypes
int TickFct_GetInput(int state);
int TickFct_Example(int state);
int TickFct_printMatrix(int state);
int TickFct_updateMatrix(int state);

int TickFct_movePlayer1(int state);
int TickFct_movePlayer2(int state);

int TickFct_updateShockwave(int state);
int TickFct_p1ShockWave(int state);
int TickFct_p2ShockWave(int state);

int TickFct_checkCollisions(int state);

void TickFct_reset();

#endif //SCHEDULER_H