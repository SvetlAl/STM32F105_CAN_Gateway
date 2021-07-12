//*****************************************************************
// Some string functions to work with CanHacker
// Since all the commands are processed in an ascii format, some conversion is neccessary
// July 2019 std id messages Recieve/transmit command supported
// todo: add timestamp, extended id support
// Alex Svetlichnyy 2019
//*****************************************************************
#ifndef CAN_LAWICEL_H_INCLUDED
#define CAN_LAWICEL_H_INCLUDED

#include "stm32f10x.h"
#include "can.h"
#include "usart.h"

#define LAWICEL_COMMAND_BUFFER_SIZE 21
#define LAWICEL_DESTINATION_CANHACKER 0
#define LAWICEL_DESTINATION_HANDOVER 1
#define GATEWAY_MODE_OFF 0
#define GATEWAY_MODE_ON 1

void Send_Std_LawicelMessage(Std_Can_Message_Type CanMessage, uint32_t lawicelDestination);
void Lawicel_SendInstruction(uint8_t *data);
void Lawicel_ProcessCommand(uint8_t *data, uint32_t gatewayMode);
uint32_t Lawicel_ProcessTxCommand(void);
Std_Can_Message_Type Lawicel_ConvertString_to_CanMessage(uint8_t *pLawicelString);


#endif /* CAN_LAWICEL_H_INCLUDED*/
