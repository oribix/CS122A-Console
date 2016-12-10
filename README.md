#DUALSHOCK3 Clone
This is a console to go with the Dualshock 3 clone.
It uses [FreeRTOS](http://www.freertos.org/) to power it.

The console uses a bluetooth module to connect to the controllers.
It has 2 8x8 Red-Green LED matrixes for output.
The matrices are driven by 4 shift registers.

The console is currently able to accept 2 controllers.
The first controller is the [DUALSHOCK3 clone](https://github.com/oribix/DualShock3-Clone-ATMEGA1284).
The second controller is a SNES controller because making a second DS3 would be expensive.

The console takes ATMEGA1284s as [game cartridges](https://github.com/oribix/CS122A-Console-Cartridge).

See the [Google Doc](https://drive.google.com/open?id=0B5K5OUHJGcLmLTlsRlRZWVctNUk) for more details.

[Video Demonstration]()

##Hardware
- ATMEGA1284
- Bluetooth module: HC-05.
- Shift Registers: SN74HC595N
- 8x8 LED matrix: 1588ASRG

For wiring see the pictures in the [Google Doc](https://drive.google.com/open?id=0B5K5OUHJGcLmLTlsRlRZWVctNUk).

##File Descriptions

###main.c
This has the main function and core logic.
The state machines here drive the matrix and handle all communications.
Games should not be made using this code as a base.
Instead use the [Game Cartridge API](https://github.com/oribix/CS122A-Console-Cartridge).

###bit.h
Binary manipulation helper functions

###led_matrix.h
LED matrix Driver

###SNES_controller.h
SNES controller driver

###usart_ATmega1284.h
ATMEGA1284 USART driver

###FreeRTOS Files
- croutine.c
- heap_1.c
- list.c
- port.c
- queue.c
- tasks.c
- timer.h
- timers.c
