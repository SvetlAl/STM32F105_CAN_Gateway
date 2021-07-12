//*****************************************************************
// Some string functions to work with CanHacker
// Since all the commands are processed in an ascii format, some conversion is neccessary
// July 2019 std id messages Recieve/transmit command supported
// todo: add timestamp, extended id support
// Alex Svetlichnyy 2019
//*****************************************************************

#include "can_lawicel.h"

extern uint8_t LawicelCommandCounter;													// Byte counter for lawicel command buffer
extern uint8_t LawicelBuffer[LAWICEL_COMMAND_BUFFER_SIZE];		// Lawicel command recieved from the host marked  
//******* GATEWAY ***********
extern uint8_t GateWayLawicelCommandCounter;													// Byte counter for lawicel command buffer
extern uint8_t GateWayLawicelBuffer[LAWICEL_COMMAND_BUFFER_SIZE];		// Lawicel command recieved from the host marked 
extern void GateWayCanRxBufferWrite(Std_Can_Message_Type rxData);
//******* GATEWAY ***********

/**
* brief  Send_StdLawicelMessage: Send a CAN message to the host in lawicel format
* brief  tIIILD'\r'  where 't' - message command mark III - id numbers L - length D-DDDDDDDD data bytes(0-8)
* param  Std_Can_Message_Type
* param  
* retval 
*/
/* Message can take up from 5 to 21 (+'\r') bytes depending on data size  */
void Send_Std_LawicelMessage(Std_Can_Message_Type CanMessage, uint32_t lawicelDestination){
	static uint8_t lawicelMessage_toSend[22];
	uint8_t commandSize;
		lawicelMessage_toSend[0]='t';																								// Lawicel 'message' mark
		lawicelMessage_toSend[1]=halfbyte_to_hexascii(CanMessage.id_highbyte);			// BXX byte of a std id
		lawicelMessage_toSend[2]=halfbyte_to_hexascii(CanMessage.id_lowbyte >> 4);	// XBX byte of a std id
		lawicelMessage_toSend[3]=halfbyte_to_hexascii(CanMessage.id_lowbyte);				// XXB byte of a std id
		lawicelMessage_toSend[4]=halfbyte_to_hexascii(CanMessage.dlc);							// DLC data length
		commandSize=CanMessage.dlc;																									// Number of message data bytes	
		commandSize=commandSize*2;																									// Each byte is split into 2 'ascii' format bytes
		commandSize=commandSize+4;																									// Skip first 0-4 bytes before count
		uint8_t lastByte=commandSize+1;																							// Mark the last byte position
		uint32_t messageSize=lastByte+1;																						// Message size calculation (lastbyte position + zerobyte)
	
		uint8_t messageByteCounter=0;
		if(commandSize==20){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);	         
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}   
		if(commandSize==18){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);										
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}	      			
		if(commandSize==16){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);										
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}							
		if(commandSize==14){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);										
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}							
		if(commandSize==12){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);											
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}										
		if(commandSize==10){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);											
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}									
		if(commandSize==8){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);											
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}								
		if(commandSize==6){
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter]);											
			lawicelMessage_toSend[commandSize--]=halfbyte_to_hexascii(CanMessage.data[messageByteCounter++] >> 4);}
			
			lawicelMessage_toSend[lastByte]='\r';		
			
			if(lawicelDestination==LAWICEL_DESTINATION_CANHACKER){
				Dma1Usart1_SendByteArray(lawicelMessage_toSend, messageSize);}
			
			else if(lawicelDestination==LAWICEL_DESTINATION_HANDOVER){
				Dma1Usart2_SendByteArray(lawicelMessage_toSend, messageSize);}
}
/**
* brief  Lawicel_Ititialize: CanHacker initializes connection with a version request 'V','v'
* brief  answer format is VXXXX, vXXXXX. All the other requests are responded with '\r'
* param  recieved data
* param  
* retval 
*/
/* Message can take up from 5 to 21 (+'\r') bytes depending on data size  */
void Lawicel_SendInstruction(uint8_t *data){ 
	uint8_t Buf[7];
	if(data[0]=='V'){
	 Buf[0]='V';
	 Buf[1]='1';
	 Buf[2]='0';
	 Buf[3]='1';
	 Buf[4]='0';
	 Buf[5]='\r';	
   Dma1Usart1_SendByteArray(Buf,6);
		}
	else if(data[0]=='v'){
	 Buf[0]='v';
	 Buf[1]='S';
	 Buf[2]='T';
	 Buf[3]='M';
	 Buf[4]='3';
	 Buf[5]='2';	
	 Buf[6]='\r';	
   Dma1Usart1_SendByteArray(Buf,7);			
		} 
	else {
	 Buf[0]='\r';
	 Dma1Usart1_SendByte(&Buf[0]);}	
}
/**
* brief  Lawicel_ProcessCommand: this function decides what to do with the data recieved from the host
* brief  
* param  data recieved from the host
* param  
* retval 
*/
/*   */
void Lawicel_ProcessCommand(uint8_t *data, uint32_t gatewayMode){
	if(gatewayMode==GATEWAY_MODE_OFF){
		Lawicel_SendInstruction(data);
	}
	
//****************************** TODO ***************************
	//****************************** TODO ***************************
	//****************************** TODO ***************************
	else if(gatewayMode==GATEWAY_MODE_ON){				
			if(data[0]=='t'){															
			GateWayLawicelCommandCounter=1;																
			GateWayLawicelBuffer[0]='t';				
				}		
			else if(GateWayLawicelCommandCounter!=0){												// If a message counter is running, continue recieving data into the buffer
					if(data[0]=='\r' && GateWayLawicelCommandCounter>=5){			// If a message contains less than 8 bytes data (recieved data is less than								
					Std_Can_Message_Type newCanMessage;
					newCanMessage=Lawicel_ConvertString_to_CanMessage(GateWayLawicelBuffer);
					GateWayCanRxBufferWrite(newCanMessage);
					GateWayLawicelCommandCounter=0;	
					}
			
		GateWayLawicelBuffer[GateWayLawicelCommandCounter]=data[0];		// If '\r' is not met, write put the byte into the buffer
		GateWayLawicelCommandCounter++;																// 

		if(GateWayLawicelCommandCounter>LAWICEL_COMMAND_BUFFER_SIZE){	// 
			GateWayLawicelCommandCounter=0;															// 
//			Lawicel_ProcessTxCommand(GATEWAY_MODE_ON);								// 
				}				
	//****************************** TODO ***************************		
				//****************************** TODO ***************************
				//****************************** TODO ***************************
		}
	}
}

/**
* brief  Lawicel_ProcessTxCommand: construct a CAN message from ascii-string and send it to a CAN-Network
* brief  
* param  
* param  
* retval 0 OK 1 Err
*/
/* tIIILD'\r'  where 't' - message command mark III - id numbers L - length D-DDDDDDDD data bytes(0-8) */

uint32_t Lawicel_ProcessTxCommand(){
		Std_Can_Message_Type hostMessageToSend;		 					// Construct a CAN message to send using the data from the host
		uint8_t tmp=0;
																												// Zero byte is already set as 't'
		tmp=hexascii_to_halfbyte(LawicelBuffer[1]);					// Set a Byte1 as BXX byte of a std id	
		hostMessageToSend.id_highbyte=tmp;                  // Set a Byte1 as BXX byte of a std id            
		
		tmp=hexascii_to_halfbyte(LawicelBuffer[2]);					// Set a Byte2 as XBX byte of a std id
		tmp=(tmp << 4);
		tmp=tmp | hexascii_to_halfbyte(LawicelBuffer[3]);		// Set a Byte3 as XXB byte of a std id
		hostMessageToSend.id_lowbyte=tmp;
		
		tmp=hexascii_to_halfbyte(LawicelBuffer[4]);					// Set a Byte4 as a dlc
		hostMessageToSend.dlc=tmp;                        
		uint8_t messageLength = (hostMessageToSend.dlc*2);	// Calculate a message length: every data byte was split into 2 ascii
		messageLength = messageLength+4;										// Skip 0-4 bytes of an ascii string
		uint8_t dataByteCounter=0;
	
		if(messageLength==20){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);													
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );			
			hostMessageToSend.data[dataByteCounter++]=tmp;														
		}			
		if(messageLength==18){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);								
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}			
		if(messageLength==16){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}		
		if(messageLength==14){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}	
		if(messageLength==12){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}		
		if(messageLength==10){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}		
		if(messageLength==8){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);									
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );				
			hostMessageToSend.data[dataByteCounter++]=tmp;															
		}		
		if(messageLength==6){			
			tmp = hexascii_to_halfbyte(LawicelBuffer[messageLength--]);									
			tmp |= (hexascii_to_halfbyte(LawicelBuffer[messageLength--]) << 4 );				
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}
		uint8_t MessageSize=hostMessageToSend.dlc;		 																						
		for(dataByteCounter = 8; dataByteCounter > MessageSize;){					// Set all unused fields with 0xff	
			hostMessageToSend.data[--dataByteCounter]=0xff;
		}		
		
		uint32_t transmissionResult;
		transmissionResult= Can1_Transmit_StdMessage(hostMessageToSend);
		return transmissionResult;		
}
	




/**
* brief  Lawicel_ConvertString_to_CanMessage: convert a Lawicel string to a Can Message
* brief  
* param  
* param  
* retval CanMessage structure
*/
/* tIIILD'\r'  where 't' - message command mark III - id numbers L - length D-DDDDDDDD data bytes(0-8) */


Std_Can_Message_Type Lawicel_ConvertString_to_CanMessage(uint8_t *pLawicelString){

		Std_Can_Message_Type hostMessageToSend;		 					// Construct a CAN message to send using the data from the host
		uint8_t tmp=0;
																												// Zero byte is already set as 't'
		tmp=hexascii_to_halfbyte(pLawicelString[1]);					// Set a Byte1 as BXX byte of a std id	
		hostMessageToSend.id_highbyte=tmp;                  // Set a Byte1 as BXX byte of a std id            
		
		tmp=hexascii_to_halfbyte(pLawicelString[2]);					// Set a Byte2 as XBX byte of a std id
		tmp=(tmp << 4);
		tmp=tmp | hexascii_to_halfbyte(pLawicelString[3]);		// Set a Byte3 as XXB byte of a std id
		hostMessageToSend.id_lowbyte=tmp;
		
		tmp=hexascii_to_halfbyte(pLawicelString[4]);					// Set a Byte4 as a dlc
		hostMessageToSend.dlc=tmp;                        
		uint8_t messageLength = (hostMessageToSend.dlc*2);	// Calculate a message length: every data byte was split into 2 ascii
		messageLength = messageLength+4;										// Skip 0-4 bytes of an ascii string
		uint8_t dataByteCounter=0;
	
		if(messageLength==20){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);													
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );			
			hostMessageToSend.data[dataByteCounter++]=tmp;														
		}			
		if(messageLength==18){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);								
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}			
		if(messageLength==16){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}		
		if(messageLength==14){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}	
		if(messageLength==12){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}		
		if(messageLength==10){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);							
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}		
		if(messageLength==8){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);									
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );				
			hostMessageToSend.data[dataByteCounter++]=tmp;															
		}		
		if(messageLength==6){			
			tmp = hexascii_to_halfbyte(pLawicelString[messageLength--]);									
			tmp |= (hexascii_to_halfbyte(pLawicelString[messageLength--]) << 4 );				
			hostMessageToSend.data[dataByteCounter++]=tmp;			
		}
		uint8_t MessageSize=hostMessageToSend.dlc;		 																						
		for(dataByteCounter = 8; dataByteCounter > MessageSize;){					// Set all unused fields with 0xff	
			hostMessageToSend.data[--dataByteCounter]=0xff;
		}		
		return hostMessageToSend;		
}	
