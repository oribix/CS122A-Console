#DUALSHOCK3 Clone
This is a console to go with the Dualshock 3 clone.
It uses [FreeRTOS](http://www.freertos.org/) to power it.

The console uses a bluetooth module to connect to the controllers.
It has 2 8x8 Red-Green LED matrixes for output.
The matrices are driven by 4 shift registers.

The console is currently able to accept 2 controllers.
The first controller is the [DUALSHOCK3 clone](https://github.com/oribix/DualShock3-Clone-ATMEGA1284).
The second controller is a SNES controller because making a second DS3 would be expensive.

You can also see a video deomonstration.

##Hardware
- ATMEGA1284
- Bluetooth module: HC-05.
- Shift Registers: SN74HC595N
- 8x8 LED matrix: 1588ASRG

For wiring see the pictures in the google doc.

##File Descriptions
- main.c
  - This has the main function and core logic.
  The state machines here drive the matrix and handle all communications.
- bit.h
  - BInary manipulation helper functions
- led_matrix.h
  - LED matrix Driver
- SNES_controller.h
  - SNES controller driver
- usart_ATmega1284.h
  - ATMEGA1284 USART driver

###FreeRTOS Files
The rest of the files are for FreeRTOS.
They are:
- croutine.c
- heap_1.c
- list.c
- port.c
- queue.c
- tasks.c
- timer.h
- timers.c
