#include <stdint.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <math.h> 
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <avr/eeprom.h> 
#include <avr/portpins.h> 
#include <avr/pgmspace.h> 
 
//FreeRTOS include files 
#include "FreeRTOS.h" 
#include "task.h" 
#include "croutine.h"

#include "usart_ATmega1284.h"
#include "led_matrix.h"
#include "SNES_controller.h"

unsigned short matrixG[8]; //green matrix data
unsigned short matrixR[8]; //red matrix data

void delay_ms(int miliSec) { //for 8 Mhz crystal
    int i,j;
    for(i=0;i<miliSec;i++) {
        for(j=0;j<775;j++) {
            asm("nop");
        }
    }
}

//requests from game cartridge the red matrix data
//must be done according to protocol
void getoutputR(unsigned short matrixRed[]){
    
}

//requests from game cartridge the green matrix data
//must be done according to protocol
void getoutputG(unsigned short matrixGreen[]){
    
}

void ds3Rumble(unsigned char duration){
    USART_Send(duration, 0);
}

//gets the controller vector for dual shock 3 clone
unsigned long long getDS3Vector(){
    long long ds3Vector = 0;
    
    ds3Rumble(0);
    
    unsigned char i;
    for(i = 0; i < 8 ; i++){
        ds3Vector <<= 8;
        ds3Vector |= USART_Receive(0);
    }
    
    return ds3Vector;
}

enum Fetcher {FETCH_Init, FETCH_Wait} fetcher;
unsigned long long ds3;

void fetcherInit(){
    fetcher = FETCH_Init;
}

void fetcherTick(){
    //Actions
    switch(fetcher){
        case FETCH_Init:
            ds3 = 0;
        break;
        
        case FETCH_Wait:
            //get ds3 controller vector
            //note that we disable-enable matrix output
            //this prevents an output bug
            disableMatrix();
            ds3 = getDS3Vector();
            enableMatrix();
        break;
    }
    
    //Transitions
    switch(fetcher){
        case FETCH_Init:
            fetcher = FETCH_Wait;
        break;
        
        case FETCH_Wait:
        break;
    }
}

void fetcherTask()
{
    fetcherInit();
    for(;;)
    {
        fetcherTick();
        vTaskDelay(50);
    }
}

enum MatrixOutputState {MOS_INIT, MOS_Wait} MOS;

unsigned char rowToPrint;

void MOS_Init(){
	MOS = MOS_INIT;
}

void MOS_Tick(){
	//Actions
	switch(MOS){
		case MOS_INIT:
            rowToPrint = 0;
		break;
		case MOS_Wait:
            ;
            int i;
            for(i = 0; i < 8; i++){
                matrixR[i] = ds3 & 0xFFFF;
            }
            displayMatrixRow(rowToPrint, matrixR, matrixG);
            if(rowToPrint < 7) rowToPrint++;
            else rowToPrint = 0;
            
        break;
	}
    
	//Transitions
	switch(MOS){
		case MOS_INIT:
			MOS = MOS_Wait;
		break;
		case MOS_Wait:
        break;
	}
}

void MOSTask()
{
	MOS_Init();
   for(;;) 
   { 	
	MOS_Tick();
	vTaskDelay(3);
   } 
}

void StartMOS(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(MOSTask, (signed portCHAR *)"MOSTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}

void StartFetcher(unsigned portBASE_TYPE Priority)
{
    xTaskCreate(fetcherTask, (signed portCHAR *)"fetcherTask", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
 
int main(void) 
{ 
   DDRA = 0xFF; PORTA = 0x00;
   DDRB = 0xFF; PORTB = 0x00;
   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   
   initUSART(0);
   SNES_init();
   
   delay_ms(5000); //wait for Bluetooth to sync
   
   //Start Tasks  
   StartMOS(1);
   StartFetcher(2);
   //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}