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
        for(j=0;j<800;j++) {
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

void ds3Rumble(bool rumble){
    USART_Send(rumble, 0);
    while(!USART_HasTransmitted(0));
}

//gets the controller vector for dual shock 3 clone
unsigned long long getDS3Vector(bool rumble){
    long long ds3Vector = 0;
    
    ds3Rumble(rumble);
    
    unsigned char i;
    for(i = 0; i < 8 ; i++){
        ds3Vector <<= 8;
        ds3Vector |= USART_Receive(0);
    }
    
    return ds3Vector;
}

//receives matrix from USART1 and returns it
//matrix is a reference to the matrix array to store the received values
//size is the number of elements in the array
unsigned short * receiveMatrix(unsigned short * matrix, unsigned char size){
    unsigned char i;
    for(i = 0; i < size; i++){
        unsigned char highByte = USART_Receive(1);
        unsigned char  lowByte = USART_Receive(1);
        unsigned short row = (highByte << 8) | lowByte;
        matrix[i] = row;
    }
    
    return matrix;
}

enum Fetcher {FETCH_Init, FETCH_Wait} fetcher;
unsigned long long ds3;
unsigned short snesVector;
unsigned char request;

void fetcherInit(){
    fetcher = FETCH_Init;
}

void fetcherTick(){
    //Actions
    switch(fetcher){
        case FETCH_Init:
            ds3 = 0;
            snesVector = 0;
            request = 0;
        break;
        
        case FETCH_Wait:
            request = 0;
            //if a request was made
            if(USART_HasReceived(1)){
                disableMatrix(); //prevents output bug
                
                //read the request
                request = USART_Receive(1);
                
                if(GetBit(request, 0)){ //controller vectors requested
                    //rumbles if requested
                    bool rumble = GetBit(request, 7);
                    
                    //get ds3 controller vector
                    ds3 = getDS3Vector(rumble);
                    
                    snesVector = getSNESVector();
                    
                    //send 8 byte ds3Vector from MSByte to LSByte
                    unsigned char i = 8;
                    while(i-->0){
                        unsigned char byte = (ds3 >> (i * 8)) & 0xFF;
                        USART_Send(byte, 1);
                    }
                    
                    //send 2 byte SNESVector from MSByte to LSByte
                    unsigned char SNEShigh = snesVector >> 8;
                    unsigned char SNESlow = snesVector & 0xFF;
                    USART_Send(SNEShigh, 1);
                    USART_Send(SNESlow, 1);
                }
                //time to receive the matrix data
                else if(GetBit(request, 1)){
                    //informs cartridge that the console is ready
                    USART_Send(request, 1);
                    while(!USART_HasTransmitted(1));
                    
                    //receive red matrix
                    receiveMatrix(matrixR, 8);
                    
                    //receive green matrix
                    receiveMatrix(matrixG, 8);
                }
                
                enableMatrix();
            }
            
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
        vTaskDelay(10);
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
        vTaskDelay(2);
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
   DDRC = 0x00; PORTC = 0xFF;
   DDRD = 0xFF; PORTD = 0x00;
   
   //init UARTS
   initUSART(0);
   initUSART(1);
   
   SNES_init();
   
   //wait for Bluetooth to sync
   delay_ms(5000);
   
   //Start Tasks  
   StartMOS(1);
   StartFetcher(2);
   //RunSchedular 
   vTaskStartScheduler(); 
 
   return 0; 
}