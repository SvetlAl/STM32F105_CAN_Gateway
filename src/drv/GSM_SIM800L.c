//*****************************************************************
// GSM800L functions
// PIN assignation: B12 controls Gate of a N-Mosfet, that powers SIM800L
// RX/TX ports were configured for A2-A3 of USART2
// Re-assign pins within configuration functions If you need/
// 
//  Alex Svetlichnyy 2020
//*****************************************************************

#include "gsm_sim800l.h"
#include "usart.h"
#include "string_convertor.h"

extern volatile uint32_t sim800l_status_buffer_counter;
/**
* brief  Configure Power Up pin (B12)
* param  
* param  
* retval 
*/
void sim800l_PwrPinConfigure(void){
	GPIOB->CRH	&= ~GPIO_CRH_CNF12;	
	GPIOB->CRH 	|=  GPIO_CRH_MODE12_0;
	sim800l_PwrDown();	
}
/**
* brief  Power Up
* param  
* param  
* retval 
*/
void sim800l_PwrUp(void){
	GPIOB->BSRR |= GPIO_BSRR_BS12;
}
/**
* brief  Power down
* param  
* param  
* retval 
*/
void sim800l_PwrDown(void){
	GPIOB->BSRR |= GPIO_BSRR_BR12;
}
/**
* brief  Reset
* param  
* param  
* retval 
*/
void sim800l_Reset(void){
	sim800l_PwrDown();
	uint32_t timeout=SIM800L_RESET_TIMEOUT;
	while(timeout--);
	sim800l_PwrUp();
}


/**
* brief  Initiate the module
* param  
* param  
* retval 
*/
uint32_t sim800l_Initiate(uint8_t *pdata){
	sim800l_PwrPinConfigure();
	sim800l_PwrUp();
	
	uint32_t timeout=SIM800L_RESPONSE_TIMEOUT;	
	while(timeout--);
	
	uint32_t result=0;
	for(int i=0; i<SIM800L_INIT_ATTEMPTS;i++){
		result=sim800l_GetPowerStatus(pdata);
		if(result==SIM800L_STATUS_OK) break;
		uint32_t timeout=SIM800L_RESPONSE_TIMEOUT;	
		while(timeout--);		
	}
	if(result!=SIM800L_STATUS_OK)return SIM800L_STATUS_NG;

	timeout=SIM800L_RESPONSE_TIMEOUT;	
	while(timeout--);


	
	for(int i=0; i<SIM800L_INIT_ATTEMPTS;i++){
	result=sim800l_GetConnectionStatus(pdata);
	if(result==SIM800L_STATUS_OK) return SIM800L_STATUS_OK;
	uint32_t timeout=SIM800L_RESPONSE_TIMEOUT;	
	while(timeout--);		
	}
	
 return SIM800L_STATUS_NG;	
}


/**
* brief  Get power status
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_GetPowerStatus(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	uint8_t ATcmd[]="AT\r";
	Dma1Usart2_SendByteArray(ATcmd,3);
		
	uint32_t timeout=SIM800L_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	sim800l_status_buffer_counter=0;
	
	uint32_t result;
	uint8_t correct_response[2]="OK";
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_OK_RESPONSE_LENGTH,2);

	if(result==0) return SIM800L_STATUS_NG;
	else return SIM800L_STATUS_OK;	
}


/**
* brief  Get connection status
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_GetConnectionStatus(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	
	uint8_t ATcmd[]="AT+CREG?\r";
	Dma1Usart2_SendByteArray(ATcmd,9);
		
	uint32_t timeout=SIM800L_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	sim800l_status_buffer_counter=0;
	
	uint32_t result;
	uint8_t correct_response[9]="CREG: 0,1";
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_ATCREG_RESPONSE_LENGTH,9);

	if(result==0) return SIM800L_STATUS_NG;
	else return SIM800L_STATUS_OK;	
}


/**
* brief  Call Host number
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_CallHost(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	
	uint8_t ATcmd[]="ATD+79069100938;\r";
	Dma1Usart2_SendByteArray(ATcmd,18);
	uint32_t timeout=SIM800L_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	sim800l_status_buffer_counter=0;
	
	uint32_t result;
	uint8_t correct_response[2]="OK";
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_OK_RESPONSE_LENGTH,2);

	if(result==0) return SIM800L_STATUS_NG;
	else return SIM800L_STATUS_OK;			
}


/**
* brief  Stop dialing any number
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_StopOutcomingCall(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	uint8_t ATcmd[]="ATH0\r";
	Dma1Usart2_SendByteArray(ATcmd,5);
	uint32_t timeout=SIM800L_LONG_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	sim800l_status_buffer_counter=0;
	
	uint32_t result;
	uint8_t correct_response[2]="OK";
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_OK_RESPONSE_LENGTH,2);

	if(result==0) return SIM800L_STATUS_NG;
	else return SIM800L_STATUS_OK;	
}

/**
* brief  Check incoming ring
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_CheckIncomingRing(uint8_t *pdata, uint8_t *cid){
	
	uint32_t result;
	uint8_t correct_response[6]={'R','I','N','G',13,10};
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_RING_RESPONSE_LENGTH,6);
	
	if(result==0){ return SIM800L_STATUS_NG;};  // If the word RING was not found - no incoming call detected
	
		uint32_t pos=19;			// If incoming call detected, extract the caller ID into 	uint8_t *cid								
	for(int i=0;i<11;i++){
		cid[i]=pdata[pos]; 
		pos++;
		}	
	return SIM800L_STATUS_OK;	
}



/**
* brief  Hang up the reciever
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_HangUp(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	uint8_t ATcmd[4]="ATH\r";
	Dma1Usart2_SendByteArray(ATcmd,4);
	uint32_t timeout=SIM800L_LONG_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	sim800l_status_buffer_counter=0;

	uint32_t result;
	uint8_t correct_response[2]="OK";
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_OK_RESPONSE_LENGTH,2);

	if(result==0) return SIM800L_STATUS_NG;
	else return SIM800L_STATUS_OK;	
}


/**
* brief Answer
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_Answer(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	uint8_t ATcmd[4]="ATA\r";
	Dma1Usart2_SendByteArray(ATcmd,4);
	uint32_t timeout=SIM800L_LONG_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	sim800l_status_buffer_counter=0;
	
	uint32_t result;
	uint8_t correct_response[2]="OK";
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_OK_RESPONSE_LENGTH,2);

	if(result==0) return SIM800L_STATUS_NG;
	else return SIM800L_STATUS_OK;	
}





/**
* brief Caller Id On
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_CallIdOn(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	uint8_t ATcmd[10]="AT+CLIP=1\r";
	Dma1Usart2_SendByteArray(ATcmd,10);
	uint32_t timeout=SIM800L_LONG_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	sim800l_status_buffer_counter=0;
	
	uint32_t result;
	uint8_t correct_response[2]="OK";
	
  result=find_first_Word_in_a_String(pdata,correct_response,SIM800L_OK_RESPONSE_LENGTH,2);

	if(result==0) return SIM800L_STATUS_NG;
	else return SIM800L_STATUS_OK;	
}



/**
* brief Is incoming number accepted?
* param  
* param  
* retval OK/ERROR
*/
uint32_t sim800l_isNumberAccepted(uint8_t *pdata){
	uint8_t hostNumber[11]="79069100938";
		for(int i=0; i<11;i++){
			if(pdata[i]!=hostNumber[i]) return SIM800L_STATUS_NG;
		}
	return SIM800L_STATUS_OK;	
}






















uint32_t sim800l_ErrorTest(uint8_t *pdata){
	sim800l_status_buffer_counter=0;
	uint8_t ATcmd[]={'A','T','\r'};
	Dma1Usart2_SendByteArray(ATcmd,3);
		
	uint32_t timeout=SIM800L_RESPONSE_TIMEOUT;
	// at this point response is recieved through USART2 IRQ and written into a status buffer (*pdata)
	while(timeout--);
	
	uint8_t correct_response[9]={0x41, 0x54, 0x0D, 0x0D, 0x0A, 0x4F, 0x4B, 0x0D, 0x00};
	for(int i=0;i<9;i++){
		if(pdata[i]!=correct_response[i])return SIM800L_STATUS_NG;
	}
	return SIM800L_STATUS_OK;	
}



