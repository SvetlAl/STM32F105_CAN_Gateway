#ifndef GSM_SIM800L_H_INCLUDED
#define GSM_SIM800L_H_INCLUDED

#include "stm32f10x.h"

#define SIM800L_RESET_TIMEOUT  						0xFFF
#define SIM800L_RESPONSE_TIMEOUT 					0xFFFFF
#define SIM800L_LONG_RESPONSE_TIMEOUT 		0xFFFFFF
#define SIM800L_STATUS_BUFFER_SIZE 				64
#define SIM800L_OK_RESPONSE_LENGTH				22
#define SIM800L_ATCREG_RESPONSE_LENGTH		42
#define SIM800L_RING_RESPONSE_LENGTH		  32

#define SIM800L_INIT_ATTEMPTS						  64 // Number of attempts to initiate the module


#define SIM800L_STATUS_OK  		0
#define SIM800L_STATUS_NG 	  1

#define SIM800L_CMD_GET_PWR_STATUS 					0x31
#define SIM800L_CMD_GET_CONNECTION_STATUS 	0x32
#define SIM800L_CMD_CALL_HOST							 	0x33
#define SIM800L_CMD_STOP_OUTCOMING_DIAL			0x34
#define SIM800L_CMD_HANG_UP									0x35
#define SIM800L_CMD_ANSWER									0x36
#define SIM800L_CMD_CALL_ID_ON							0x37  // Caller ID ON



void sim800l_PwrPinConfigure(void);
void sim800l_PwrUp(void);
void sim800l_PwrDown(void);
void sim800l_Reset(void);
uint32_t sim800l_Initiate(uint8_t *pdata);


uint32_t sim800l_GetPowerStatus(uint8_t *pdata);
uint32_t sim800l_ErrorTest(uint8_t *pdata);
uint32_t sim800l_GetConnectionStatus(uint8_t *pdata);
uint32_t sim800l_CallHost(uint8_t *pdata);
uint32_t sim800l_StopOutcomingCall(uint8_t *pdata);
uint32_t sim800l_CheckIncomingRing(uint8_t *pdata, uint8_t *cid);
uint32_t sim800l_HangUp(uint8_t *pdata);
uint32_t sim800l_Answer(uint8_t *pdata);
uint32_t sim800l_CallIdOn(uint8_t *pdata);
uint32_t sim800l_isNumberAccepted(uint8_t *pdata);



#endif /* GSM_SIM800L_H_INCLUDED*/
