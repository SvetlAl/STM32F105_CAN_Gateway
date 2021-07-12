/**
  ******************************************************************************
  * @file		keypad4x4-scanning.h
  * @author	Alex Svetlichnyy
  * @date		Jan 2020
  ******************************************************************************
  */

#ifndef __KEYPAD4X4_H
#define __KEYPAD4X4_H

#include "stm32f10x.h"
/********************************* Active output ports ********************************/
#define KEYPAD_PORT_1  0xA10
#define KEYPAD_PORT_2  0xA9
#define KEYPAD_PORT_3  0xA8
#define KEYPAD_PORT_4  0xB15
/********************************* Active output ports ********************************/

/********************************* Input scanned ports ********************************/
#define KEYPAD_PORT_5  0xB5
#define KEYPAD_PORT_6  0xB13
#define KEYPAD_PORT_7  0xB12
#define KEYPAD_PORT_8  0xB4
/********************************* Input scanned ports ********************************/





void initKeypadPorts(void);
uint8_t scanRow(uint8_t rowNumber);
uint8_t scan4b4KeyPad(void);
uint8_t translateKeyNumbertoAscii(uint8_t key);



#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
