//*****************************************************************
// 
// STM32F105
// bxCAN controller functions
// todo: Add Extended id support, RTR
// Alex Svetlichnyy 2019
//*****************************************************************

#include "can.h"

/**
* brief  Can1_Initializate_Pins : Initializes pins for CAN1        
* param  Remap configuration
* param  
* retval 0 - OK  1 - Err
*/

uint32_t Can1_Initializate_Pins(uint32_t remapConfig){
	if(remapConfig==REMAP_CAN1){
		GPIOB->CRH	&= ~GPIO_CRH_CNF8;	//  PA11--RX  PA12--TX  for stm32f103  B8--RX  B9--TX  for stm32f105
		GPIOB->CRH 	&= ~GPIO_CRH_MODE8;	 		
		GPIOB->CRH	&= ~GPIO_CRH_CNF9;	 		
		GPIOB->CRH	|= GPIO_CRH_CNF8_0;	 
		GPIOB->CRH	|= GPIO_CRH_CNF9_1;	 
		GPIOB->CRH 	|= GPIO_CRH_MODE9;	 
		AFIO->MAPR  |= AFIO_MAPR_CAN_REMAP_REMAP2;

	}
	else if(remapConfig==NO_REMAP_CAN1){
		//todo: add pin initialization if aint remapped
	}			
	return 0;
}

/**
* brief  Can2_Initializate_Pins : Initializes pins for CAN2        
* param  Remap configuration
* param  
* retval 0 - OK  1 - Err
*/

uint32_t Can2_Initializate_Pins(uint32_t remapConfig){
	if(remapConfig==REMAP_CAN2){
		GPIOB->CRL	&= ~GPIO_CRL_CNF5;	// B5--RX 8  B6--TX  for stm32f105
		GPIOB->CRL 	&= ~GPIO_CRL_MODE5;	 		
		GPIOB->CRL	&= ~GPIO_CRL_CNF6;	 		
		GPIOB->CRL	|= GPIO_CRL_CNF5_0;	 
		GPIOB->CRL	|= GPIO_CRL_CNF6_1;	 
		GPIOB->CRL 	|= GPIO_CRL_MODE6;	
		AFIO->MAPR  |= AFIO_MAPR_CAN2_REMAP;
	}
	else if(remapConfig==NO_REMAP_CAN2){
		//todo: add pin initialization if aint remapped  		
	}			
	return 0;
}

/**
* brief  Can1_Configure_BaudRate : COnfiguring BaudRate for CAN1        
* param  BaudRate configuration
* param  
* retval 0 - OK  1 - Err
*/

uint32_t Can1_Configure_BaudRate(uint32_t baudRate){	
	if(baudRate==CAN_BAUDRATE_500KB){// configuring baudRate
		CAN1->BTR &= ~CAN_BTR_TS1; // reset value
		CAN1->BTR &= ~CAN_BTR_TS2; // reset value
		CAN1->BTR &= ~CAN_BTR_SJW; // reset value

		CAN1->BTR |= 0x0007;       //Prescaler	
		CAN1->BTR |= (0x03 << 16); //BS1
		CAN1->BTR |= (0x02 << 20); //BS2
		return 0;	
	}
	//todo: add some more baudRate configurations
	else return 1;
}

/**
* brief  Can2_Configure_BaudRate : COnfiguring BaudRate for CAN2        
* param  BaudRate configuration
* param  
* retval 0 - OK  1 - Err
*/

uint32_t Can2_Configure_BaudRate(uint32_t baudRate){	
	if(baudRate==CAN_BAUDRATE_500KB){// configuring baudRate
		CAN2->BTR &= ~CAN_BTR_TS1; // reset value
		CAN2->BTR &= ~CAN_BTR_TS2; // reset value
		CAN2->BTR &= ~CAN_BTR_SJW; // reset value

		CAN2->BTR |= 0x0007;       //Prescaler	
		CAN2->BTR |= (0x03 << 16); //BS1
		CAN2->BTR |= (0x02 << 20); //BS2
		return 0;	
	}
	//todo: add some more baudRate configurations
	else return 1;
}

/**
* brief  Can1_Initializate : Initializes registers for CAN1 and starts controller        
* param  remapConfig, baudRate
* param  
* retval 0 - OK  1 - Err
*/
uint32_t Can1_Initializate(uint32_t remapConfig, uint32_t baudRate){
	uint32_t timeout;											// Timeout counter	
	Can1_Initializate_Pins(remapConfig);	// Configure pins	
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;		// Enable peripheral clock
	CAN1->MCR |= CAN_MCR_INRQ;   					// Entering Initialization mode
	CAN1->MCR &= ~CAN_MCR_SLEEP;   				// Quitting Sleep mode
	
	timeout=0;
	while(!(CAN1->MSR & CAN_MSR_INAK) && (timeout < CAN_INIT_TIMEOUT_MAX)){
		timeout++;	// Waiting for a flag CAN_MSR_INAK (This means, Initialization mode started)			
	}; 
	if(!(CAN1->MSR & CAN_MSR_INAK)){ // If a timeout passed away and CAN_MSR_INAK had not come up - Error
		return 1;
	}	
	Can1_Configure_BaudRate(baudRate);	//Configuring BaudRate
	CAN1->IER |= CAN_IER_FMPIE0; 				// Enable FIFO-0 Pending interrupt
	CAN1->IER |= CAN_IER_FMPIE1; 				// Enable FIFO-1 Pending interrupt
	CAN1->MCR |= CAN_MCR_ABOM;					// Enable auto recovery out from Buss-off	
	
	CAN1->MCR &= ~CAN_MCR_INRQ;    			// Quit Initialization mode
	
	timeout=0;
	while((CAN1->MSR & CAN_MSR_INAK) && (timeout < CAN_INIT_TIMEOUT_MAX)){
		timeout++;			
	}; // Waiting for both CAN_MSR_SLAK and CAN_MSR_INAK flags down(This means, Normal mode started) 
	
	if((!(CAN1->MSR & CAN_MSR_INAK)) && (!(CAN1->MSR & CAN_MSR_SLAK))){ 
	//Check SLAK and INAK flags. If they are reset, Controller has entered normal mode
		return 0;
	}
	else return 1;		
}

/**
* brief  Can2_Initializate : Initializes registers for CAN2 and starts controller        
* param  remapConfig, baudRate
* param  
* retval 0 - OK  1 - Err
*/
uint32_t Can2_Initializate(uint32_t remapConfig, uint32_t baudRate){
	uint32_t timeout;											// Timeout counter	
	Can2_Initializate_Pins(remapConfig);	// Configure pins	
	RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;		// Enable peripheral clock
	CAN1->MCR |= CAN_MCR_INRQ;   					// Entering Initialization mode
	CAN2->MCR |= CAN_MCR_INRQ;   					// Entering Initialization mode
	CAN2->MCR &= ~CAN_MCR_SLEEP;   				// Quitting Sleep mode
	
	timeout=0;
	while(!(CAN2->MSR & CAN_MSR_INAK) && (timeout < CAN_INIT_TIMEOUT_MAX)){
		timeout++;	// Waiting for a flag CAN_MSR_INAK (This means, Initialization mode started)			
	}; 
	if(!(CAN2->MSR & CAN_MSR_INAK)){ // If a timeout passed away and CAN_MSR_INAK had not come up - Error
		return 1;
	}	
	Can2_Configure_BaudRate(baudRate);	//Configuring BaudRate
	CAN2->IER |= CAN_IER_FMPIE0; 				// Enable FIFO-0 Pending interrupt
	CAN2->MCR |= CAN_MCR_ABOM;					// Enable auto recovery out from Buss-off	
	
	CAN2->MCR &= ~CAN_MCR_INRQ;    			// Quit Initialization mode
	CAN1->MCR &= ~CAN_MCR_INRQ;    			// Quit Initialization mode

	timeout=0;
	while((CAN2->MSR & CAN_MSR_INAK) && (timeout < CAN_INIT_TIMEOUT_MAX)){
		timeout++;			
	}; // Waiting for both CAN_MSR_SLAK and CAN_MSR_INAK flags down(This means, Normal mode started) 
	
	if((!(CAN2->MSR & CAN_MSR_INAK)) && (!(CAN2->MSR & CAN_MSR_SLAK))){ 
	//Check SLAK and INAK flags. If they are reset, Controller has entered normal mode
		CAN2->RF1R|=CAN_RF0R_RFOM0;
		return 0;

	}
	else return 1;		
}


/**
* brief  Can1_InitializateFilters : Initializes CAN1 filters        
* param  Remap configuration
* param  
* retval
*/
void Can1_InitializateFilters(void){	
	//todo: add some more features 	
	CAN1->FMR &= ~0x3F00; // 11111100000000  reset CAN2SB
	CAN1->FMR |= CAN_FMR_FINIT; // This bit starts Filter Initialization
	CAN1->FMR |= 0xE00; // CAN2SB   CAN2 starts from 14
	CAN1->FM1R &= ~0xFFFFFFFF;
	CAN1->FS1R &= ~0xFFFFFFFF;
	CAN1->FFA1R &= ~0xFFFFFFFF;
	CAN1->FA1R	&= ~0xFFFFFFFF;
	CAN1->sFilterRegister[0].FR1 &= ~0xFFFFFFFF; // CAN->F0R1
	CAN1->sFilterRegister[0].FR2 &= ~0xFFFFFFFF; // CAN->F0R2
	


	CAN1->sFilterRegister[14].FR1 &= ~0xFFFFFFFF; // CAN->F14R1
	CAN1->sFilterRegister[14].FR2 &= ~0xFFFFFFFF; // CAN->F14R2
	
	
	CAN1->FS1R |= CAN_FS1R_FSC0; // 1 - 32-bit
	CAN1->FS1R |= 0x4000; // 100000000000000  14 bit		

	CAN1->FFA1R |=  0x00; // 0 - FIFO 0
	CAN1->FA1R	|=  0x01;	// 1 - Active		
	CAN1->FA1R	|= 0x4000; // 100000000000000  14 bit		
	
	CAN1->sFilterRegister[0].FR1 |=  0x00; // As long as we accept all the traffic filter value is 0
	CAN1->sFilterRegister[0].FR2 |=  0x00;		
	//Filter Initialization completed		
	CAN1->FMR &= ~CAN_FMR_FINIT;	
}



void Can1_InitializateFilters2(void){	
	//todo: add some more features 	
	CAN1->FMR |= CAN_FMR_FINIT; // This bit starts Filter Initialization
	CAN1->FM1R &= ~0xFFFFFFFF;
	CAN1->FS1R &= ~0xFFFFFFFF;
	CAN1->FFA1R &= ~0xFFFFFFFF;
	CAN1->FA1R	&= ~0xFFFFFFFF;
	CAN1->sFilterRegister[0].FR1 &= ~0xFFFFFFFF; // CAN->F0R1
	CAN1->sFilterRegister[0].FR2 &= ~0xFFFFFFFF;
			
	CAN1->FM1R  |=  CAN_FM1R_FBM0; 			// 1 - Id mode
	CAN1->FS1R  &=  ~CAN_FS1R_FSC0; 		// 0 - 16-bit
	CAN1->FFA1R &=  ~CAN_FFA1R_FFA0; 		// 0 - FIFO 0
	CAN1->FA1R	|=  CAN_FA1R_FACT0;			// 1 - Active	
	CAN1->sFilterRegister[0].FR1 |=  	0x215 << 5 | 0x215 << (16+5); 
	CAN1->sFilterRegister[0].FR2 |=  	0x215 << 5 | 0x215 << (16+5);	
	

	//Filter Initialization completed		
	CAN1->FMR &= ~CAN_FMR_FINIT;	
}

void Can1_InitializateFilters_GatewayTemplate(void){	
	//todo: add some more features 	

	CAN1->FMR |= CAN_FMR_FINIT; // This bit starts Filter Initialization
		CAN2->FMR |= CAN_FMR_FINIT; // This bit starts Filter Initialization
	CAN1->FMR &= ~0x3F00; // 11111100000000  reset CAN2SB
	CAN1->FMR |= 0x300; // CAN2SB   CAN2 starts from 14
	
	CAN1->FM1R &= ~0xFFFFFFFF;
	CAN1->FS1R &= ~0xFFFFFFFF;
	CAN1->FFA1R &= ~0xFFFFFFFF;
	CAN1->FA1R	&= ~0xFFFFFFFF;
	
	//CAN1
	CAN1->sFilterRegister[0].FR1 &= ~0xFFFFFFFF; // CAN->F0R1
	CAN1->sFilterRegister[0].FR2 &= ~0xFFFFFFFF; // CAN->F0R2
	
	CAN1->FM1R  &=  ~ CAN_FM1R_FBM0; 			// 0 - Mask mode
	CAN1->FS1R  &=  ~CAN_FS1R_FSC0; 		// 0 - 16-bit
	CAN1->FFA1R |=  CAN_FFA1R_FFA0; 		// 0 - FIFO 1
	CAN1->FA1R	|=  CAN_FA1R_FACT0;			// 1 - Active	
	

	CAN1->sFilterRegister[0].FR1 |=  	0x0214 << 5 | 0x1FFF << (16); // from 214 to 215  // ID and mask
	CAN1->sFilterRegister[0].FR2 |=  	0x0215 << 5 | 0x1FFF << (16); // from 214 to 215  // ID and mask


	CAN1->sFilterRegister[1].FR1 &= ~0xFFFFFFFF; // CAN->F14R1
	CAN1->sFilterRegister[1].FR2 &= ~0xFFFFFFFF; // CAN->F14R2

	CAN1->FM1R  &=  ~  CAN_FM1R_FBM1; 	// 0 - Mask mode
	CAN1->FS1R  &=  ~CAN_FS1R_FSC1; 		// 0 - 16-bit
	CAN1->FFA1R &=  ~CAN_FFA1R_FFA1; 		// 0 - FIFO 0
	CAN1->FA1R	|=  CAN_FA1R_FACT1;			// 1 - Active	
	
	CAN1->sFilterRegister[1].FR1 |=  	0x000 << 5 | 0x00 << (16); // else than 214 or 215  // ID and mask
	CAN1->sFilterRegister[1].FR2 |=  	0x000 << 5 | 0x00 << (16); // else than 214 or 215  // ID and mask
	
	//CAN2
	CAN1->sFilterRegister[3].FR1 &= ~0xFFFFFFFF; // CAN->F14R1
	CAN1->sFilterRegister[3].FR2 &= ~0xFFFFFFFF; // CAN->F14R2
	
	CAN1->FM1R  &=  ~ CAN_FM1R_FBM3; 			// 0 - Mask mode
	CAN1->FS1R  &=  ~CAN_FS1R_FSC3; 				// 0 - 16-bit
	CAN1->FFA1R &=  ~CAN_FFA1R_FFA3; 		// 0 - FIFO 0
	CAN1->FA1R	|=  CAN_FA1R_FACT3;			// 1 - Active	
		
	
	CAN1->sFilterRegister[3].FR1 |=  0x00; // As long as we accept all the traffic filter value is 0
	CAN1->sFilterRegister[3].FR2 |=  0x00;		
	
	
	//Filter Initialization completed		
	CAN1->FMR &= ~CAN_FMR_FINIT;	
}


/**
 * brief  Can1_SetTxMessage: set up an empty mailbox with a single message (with dlc, id and data) 
 * param  CanMessage, Mailbox number
 * param  
 * retval 0 - OK, 1 - Err
 */	
uint32_t Can1_SetTxMessage(Std_Can_Message_Type canMessage, uint32_t mailboxNo){
	CAN1->sTxMailBox[mailboxNo].TIR	  &= ~ 0xFFFFFFFF;  // Clean up
	CAN1->sTxMailBox[mailboxNo].TDTR  &= ~ 0xFFFFFFFF;	// Clean up
	CAN1->sTxMailBox[mailboxNo].TDLR  &= ~ 0xFFFFFFFF;  // Clean up
	CAN1->sTxMailBox[mailboxNo].TDHR  &= ~ 0xFFFFFFFF;  // Clean up
	
// ID value is set within 31-21 bit range. First set bits 28-21
	CAN1->sTxMailBox[mailboxNo].TIR  |= (canMessage.id_lowbyte << 21);
	canMessage.id_highbyte &=0x1F;	// Discard bits 7-3 of the second identificator part
//set bits 31-28		
	CAN1->sTxMailBox[mailboxNo].TIR  |= (canMessage.id_highbyte << 29);			
	CAN1->sTxMailBox[mailboxNo].TDTR |= canMessage.dlc; 
	
	uint8_t MessageSize;
	MessageSize=canMessage.dlc;	// Data fields assignation is based on the message size																				
	if(MessageSize>8){		
		return 1;  //make sure DLC is not greater, than 8
	}	
	if(MessageSize>=1){
		CAN1->sTxMailBox[mailboxNo].TDLR |= canMessage.data[0];}					  	 // DATA0
	if(MessageSize>=2){
		CAN1->sTxMailBox[mailboxNo].TDLR |= (canMessage.data[1] << 8);} 				// DATA1	
	if(MessageSize>=3){
		CAN1->sTxMailBox[mailboxNo].TDLR |= (canMessage.data[2] << 16);} 				// DATA2
	if(MessageSize>=4){	
		CAN1->sTxMailBox[mailboxNo].TDLR |= (canMessage.data[3] << 24);} 				// DATA3
	if(MessageSize>=5){
		CAN1->sTxMailBox[mailboxNo].TDHR |= canMessage.data[4];} 								// DATA4	
	if(MessageSize>=6){
		CAN1->sTxMailBox[mailboxNo].TDHR |= (canMessage.data[5] << 8);} 				// DATA5
	if(MessageSize>=7){
		CAN1->sTxMailBox[mailboxNo].TDHR |= (canMessage.data[6] << 16);} 				// DATA6		
	if(MessageSize==8){
		CAN1->sTxMailBox[mailboxNo].TDHR |= (canMessage.data[7] << 24);} 				// DATA7 	
	return 0;			
	}
	

	
	/**
 * brief  Can2_SetTxMessage: set up an empty mailbox with a single message (with dlc, id and data) 
 * param  CanMessage, Mailbox number
 * param  
 * retval 0 - OK, 1 - Err
 */	
uint32_t Can2_SetTxMessage(Std_Can_Message_Type canMessage, uint32_t mailboxNo){
	CAN2->sTxMailBox[mailboxNo].TIR	  &= ~ 0xFFFFFFFF;  // Clean up
	CAN2->sTxMailBox[mailboxNo].TDTR  &= ~ 0xFFFFFFFF;	// Clean up
	CAN2->sTxMailBox[mailboxNo].TDLR  &= ~ 0xFFFFFFFF;  // Clean up
	CAN2->sTxMailBox[mailboxNo].TDHR  &= ~ 0xFFFFFFFF;  // Clean up
	
// ID value is set within 31-21 bit range. First set bits 28-21
	CAN2->sTxMailBox[mailboxNo].TIR  |= (canMessage.id_lowbyte << 21);
	canMessage.id_highbyte &=0x1F;	// Discard bits 7-3 of the second identificator part
//set bits 31-28		
	CAN2->sTxMailBox[mailboxNo].TIR  |= (canMessage.id_highbyte << 29);			
	CAN2->sTxMailBox[mailboxNo].TDTR |= canMessage.dlc; 
	
	uint8_t MessageSize;
	MessageSize=canMessage.dlc;	// Data fields assignation is based on the message size																				
	if(MessageSize>8){		
		return 1;  //make sure DLC is not greater, than 8
	}	
	if(MessageSize>=1){
		CAN2->sTxMailBox[mailboxNo].TDLR |= canMessage.data[0];}					  	 // DATA0
	if(MessageSize>=2){
		CAN2->sTxMailBox[mailboxNo].TDLR |= (canMessage.data[1] << 8);} 				// DATA1	
	if(MessageSize>=3){
		CAN2->sTxMailBox[mailboxNo].TDLR |= (canMessage.data[2] << 16);} 				// DATA2
	if(MessageSize>=4){	
		CAN2->sTxMailBox[mailboxNo].TDLR |= (canMessage.data[3] << 24);} 				// DATA3
	if(MessageSize>=5){
		CAN2->sTxMailBox[mailboxNo].TDHR |= canMessage.data[4];} 								// DATA4	
	if(MessageSize>=6){
		CAN2->sTxMailBox[mailboxNo].TDHR |= (canMessage.data[5] << 8);} 				// DATA5
	if(MessageSize>=7){
		CAN2->sTxMailBox[mailboxNo].TDHR |= (canMessage.data[6] << 16);} 				// DATA6		
	if(MessageSize==8){
		CAN2->sTxMailBox[mailboxNo].TDHR |= (canMessage.data[7] << 24);} 				// DATA7 	
	return 0;			
	}

	
	
	
	
	
	/**
* brief  Can1_GetEmptyMailbox: find an empty mailbox
* param  CurrentMailBox
* param  
* retval Empty mailbox number otherwise - Error number
*/
/* This function is designed to be able to work with a continuous CAN-traffic flow */
/* With every new transmission in the first place this function checks for the next mailbox free*/
/* E.g. If mailbox0 was used for a previous transmission, this function looks for 	*/
/* an empty mailbox starting from mailbox1,then mailbox2, then mailbox0	*/
uint32_t Can1_GetEmptyMailbox(){		
	uint32_t EmptyMailBoxNo;
  if(CAN1->TSR == 0x1C000000){ //The very first message after reset
		EmptyMailBoxNo=CAN_TX_MAILBOX_0;
		return EmptyMailBoxNo;
	}			
	for (int i=0; i < CAN_EMPTY_MAILBOX_TIMEOUT; i++){
		if((CAN1->TSR & CAN_TSR_TME0) && (CAN1->TSR & CAN_TSR_RQCP2)  && (CAN1->TSR & CAN_TSR_TXOK2)) {EmptyMailBoxNo= CAN_TX_MAILBOX_0;
		CAN1->TSR |= CAN_TSR_RQCP2;
			CAN1->TSR |= CAN_TSR_TXOK2;
			return EmptyMailBoxNo;
			}
		if((CAN1->TSR & CAN_TSR_TME1) && (CAN1->TSR & CAN_TSR_RQCP0)  && (CAN1->TSR & CAN_TSR_TXOK0)) {EmptyMailBoxNo= CAN_TX_MAILBOX_1; 
			CAN1->TSR |= CAN_TSR_RQCP0;
			CAN1->TSR |= CAN_TSR_TXOK0;
			return EmptyMailBoxNo;
			}
		if((CAN1->TSR & CAN_TSR_TME2) && (CAN1->TSR & CAN_TSR_RQCP1)  && (CAN1->TSR & CAN_TSR_TXOK1)) {EmptyMailBoxNo= CAN_TX_MAILBOX_2; 
			CAN1->TSR |= CAN_TSR_TXOK1;
			CAN1->TSR |= CAN_TSR_RQCP1;
			return EmptyMailBoxNo;
			}			
	}
	
	if(!(CAN1->TSR & CAN_TSR_TME0) && !(CAN1->TSR & CAN_TSR_TME1) && !(CAN1->TSR & CAN_TSR_TME2)){
		EmptyMailBoxNo=ERR_CAN1_NO_EMPTY_MAILBOX;//no empty mailbox found
	}			
	return EmptyMailBoxNo;
}



	/**
* brief  Can2_GetEmptyMailbox: find an empty mailbox
* param  CurrentMailBox
* param  
* retval Empty mailbox number otherwise - Error number
*/
/* This function is designed to be able to work with a continuous CAN-traffic flow */
/* With every new transmission in the first place this function checks for the next mailbox free*/
/* E.g. If mailbox0 was used for a previous transmission, this function looks for 	*/
/* an empty mailbox starting from mailbox1,then mailbox2, then mailbox0	*/
uint32_t Can2_GetEmptyMailbox(){		
	uint32_t EmptyMailBoxNo;
  if(CAN2->TSR == 0x1C000000){ //The very first message after reset
		EmptyMailBoxNo=CAN_TX_MAILBOX_0;
		return EmptyMailBoxNo;
	}			
	for (int i=0; i < CAN_EMPTY_MAILBOX_TIMEOUT; i++){
		if((CAN2->TSR & CAN_TSR_TME0) && (CAN2->TSR & CAN_TSR_RQCP2)  && (CAN2->TSR & CAN_TSR_TXOK2)) {EmptyMailBoxNo= CAN_TX_MAILBOX_0;
		CAN2->TSR |= CAN_TSR_RQCP2;
			CAN2->TSR |= CAN_TSR_TXOK2;
			return EmptyMailBoxNo;
			}
		if((CAN2->TSR & CAN_TSR_TME1) && (CAN2->TSR & CAN_TSR_RQCP0)  && (CAN2->TSR & CAN_TSR_TXOK0)) {EmptyMailBoxNo= CAN_TX_MAILBOX_1; 
			CAN2->TSR |= CAN_TSR_RQCP0;
			CAN2->TSR |= CAN_TSR_TXOK0;
			return EmptyMailBoxNo;
			}
		if((CAN2->TSR & CAN_TSR_TME2) && (CAN2->TSR & CAN_TSR_RQCP1)  && (CAN2->TSR & CAN_TSR_TXOK1)) {EmptyMailBoxNo= CAN_TX_MAILBOX_2; 
			CAN2->TSR |= CAN_TSR_TXOK1;
			CAN2->TSR |= CAN_TSR_RQCP1;
			return EmptyMailBoxNo;
			}			
	}
	
	if(!(CAN2->TSR & CAN_TSR_TME0) && !(CAN2->TSR & CAN_TSR_TME1) && !(CAN2->TSR & CAN_TSR_TME2)){
		EmptyMailBoxNo=ERR_CAN1_NO_EMPTY_MAILBOX;//no empty mailbox found
	}			
	return EmptyMailBoxNo;
}




	
	
/**
* brief  Can1_Transmit_StdMessage_8b : transmit a 8-byte standart id message via CAN1        
* param  CanMessage
* param  
* retval mailbox number, 4,5 - err
*/
	
uint32_t Can1_Transmit_StdMessage(Std_Can_Message_Type canMessage){
	uint32_t timeout=0;
	uint32_t mailboxNo;
	mailboxNo =	Can1_GetEmptyMailbox();	
	if(mailboxNo==ERR_CAN1_NO_EMPTY_MAILBOX){
		return ERR_CAN1_NO_EMPTY_MAILBOX;
	}
	Can1_SetTxMessage(canMessage, mailboxNo);
	CAN1->sTxMailBox[mailboxNo].TIR  |= CAN_TI0R_TXRQ; // Command to send
	

		while (timeout < CAN_TRANSMIT_TIMEOUT){timeout++;};

		
	for (int i=0; i < CAN_EMPTY_MAILBOX_TIMEOUT; i++){	
	if(mailboxNo==CAN_TX_MAILBOX_0 && (CAN1->TSR & CAN_TSR_TXOK0) && (CAN1->TSR & CAN_TSR_TME0) && (CAN1->TSR & CAN_TSR_RQCP0)){
		return CAN_TX_MAILBOX_0;}	
	if(mailboxNo==CAN_TX_MAILBOX_1 && (CAN1->TSR & CAN_TSR_TXOK1) && (CAN1->TSR & CAN_TSR_TME1) && (CAN1->TSR & CAN_TSR_RQCP1)){
		return CAN_TX_MAILBOX_1;}
	if(mailboxNo==CAN_TX_MAILBOX_2 && (CAN1->TSR & CAN_TSR_TXOK2) && (CAN1->TSR & CAN_TSR_TME2) && (CAN1->TSR & CAN_TSR_RQCP2)){
		return CAN_TX_MAILBOX_2;}
	}
	
	return ERR_CAN1_NO_EMPTY_MAILBOX;
}

/**
* brief  Can2_Transmit_StdMessage_8b : transmit a 8-byte standart id message via CAN2        
* param  CanMessage
* param  
* retval mailbox number, 4,5 - err
*/
	
uint32_t Can2_Transmit_StdMessage(Std_Can_Message_Type canMessage){
	uint32_t timeout=0;
	uint32_t mailboxNo;
	mailboxNo =	Can2_GetEmptyMailbox();	
	if(mailboxNo==ERR_CAN2_NO_EMPTY_MAILBOX){
		return ERR_CAN2_NO_EMPTY_MAILBOX;
	}
	Can2_SetTxMessage(canMessage, mailboxNo);
	CAN2->sTxMailBox[mailboxNo].TIR  |= CAN_TI0R_TXRQ; // Command to send

	
		while (timeout < CAN_TRANSMIT_TIMEOUT){timeout++;};
		
	for (int i=0; i < CAN_EMPTY_MAILBOX_TIMEOUT; i++){	
	if(mailboxNo==CAN_TX_MAILBOX_0 && (CAN2->TSR & CAN_TSR_TXOK0) && (CAN2->TSR & CAN_TSR_TME0) && (CAN2->TSR & CAN_TSR_RQCP0)){
		return CAN_TX_MAILBOX_0;}	
	if(mailboxNo==CAN_TX_MAILBOX_1 && (CAN2->TSR & CAN_TSR_TXOK1) && (CAN2->TSR & CAN_TSR_TME1) && (CAN2->TSR & CAN_TSR_RQCP1)){
		return CAN_TX_MAILBOX_1;}
	if(mailboxNo==CAN_TX_MAILBOX_2 && (CAN2->TSR & CAN_TSR_TXOK2) && (CAN2->TSR & CAN_TSR_TME2) && (CAN2->TSR & CAN_TSR_RQCP2)){
		return CAN_TX_MAILBOX_2;}
	}
	
	return ERR_CAN1_NO_EMPTY_MAILBOX;
}




	
/**
* brief  Can1_Recieve_StdMessage_8b : transmit a 8-byte standart id message via CAN1        
* param  FifoNo Fifo number 0 or 1
* param  
* retval 0 OK 1 - Err
*/	
	
Std_Can_Message_Type Can1_Recieve_StdMessage(uint32_t FifoNo){
	static Std_Can_Message_Type recievedMessage;  
			
	recievedMessage.id_lowbyte  =(CAN1->sFIFOMailBox[FifoNo].RIR >> 21);
	recievedMessage.id_highbyte =(CAN1->sFIFOMailBox[FifoNo].RIR >> 29);
	recievedMessage.dlc         = CAN1->sFIFOMailBox[FifoNo].RDTR; 
	uint8_t MessageSize;
	MessageSize=recievedMessage.dlc;	// Data fields assignation is based on the message size		

	if(MessageSize>=1){
		recievedMessage.data[0]			= CAN1->sFIFOMailBox[FifoNo].RDLR;}					 		 	 // DATA0
	if(MessageSize>=2){
		recievedMessage.data[1]			=(CAN1->sFIFOMailBox[FifoNo].RDLR >> 8);} 					// DATA1	
	if(MessageSize>=3){
		recievedMessage.data[2]			=(CAN1->sFIFOMailBox[FifoNo].RDLR >> 16);} 					// DATA2
	if(MessageSize>=4){	
		recievedMessage.data[3]			=(CAN1->sFIFOMailBox[FifoNo].RDLR >> 24);} 					// DATA3
	if(MessageSize>=5){
		recievedMessage.data[4]			= CAN1->sFIFOMailBox[FifoNo].RDHR;} 								// DATA4	
	if(MessageSize>=6){
		recievedMessage.data[5]			=(CAN1->sFIFOMailBox[FifoNo].RDHR >> 8);} 					// DATA5
	if(MessageSize>=7){
		recievedMessage.data[6]			=(CAN1->sFIFOMailBox[FifoNo].RDHR >> 16);} 					// DATA6		
	if(MessageSize==8){
		recievedMessage.data[7]			=(CAN1->sFIFOMailBox[FifoNo].RDHR >> 24);} 					// DATA7 	
		
	uint8_t regCounter; 															// Set all unused fields with 0xff		
	for(regCounter = 8; regCounter > MessageSize;){
		recievedMessage.data[--regCounter]=0xff;
	}				
	if(FifoNo==CAN_FIFO_0){
	CAN1->RF0R |= CAN_RF0R_RFOM0; //release		
		}
	else{
	CAN1->RF1R |= CAN_RF1R_RFOM1; //release		
		}
	return recievedMessage;
}
	


/**
* brief  Can2_Recieve_StdMessage_8b : transmit a 8-byte standart id message via CAN2        
* param  FifoNo Fifo number 0 or 1
* param  
* retval 0 OK 1 - Err
*/	
	
Std_Can_Message_Type Can2_Recieve_StdMessage(uint32_t FifoNo){
	static Std_Can_Message_Type recievedMessage;  
			
	recievedMessage.id_lowbyte  =(CAN2->sFIFOMailBox[FifoNo].RIR >> 21);
	recievedMessage.id_highbyte =(CAN2->sFIFOMailBox[FifoNo].RIR >> 29);
	recievedMessage.dlc         = CAN2->sFIFOMailBox[FifoNo].RDTR; 
	uint8_t MessageSize;
	MessageSize=recievedMessage.dlc;	// Data fields assignation is based on the message size		

	if(MessageSize>=1){
		recievedMessage.data[0]			= CAN2->sFIFOMailBox[FifoNo].RDLR;}					 		 	 // DATA0
	if(MessageSize>=2){
		recievedMessage.data[1]			=(CAN2->sFIFOMailBox[FifoNo].RDLR >> 8);} 					// DATA1	
	if(MessageSize>=3){
		recievedMessage.data[2]			=(CAN2->sFIFOMailBox[FifoNo].RDLR >> 16);} 					// DATA2
	if(MessageSize>=4){	
		recievedMessage.data[3]			=(CAN2->sFIFOMailBox[FifoNo].RDLR >> 24);} 					// DATA3
	if(MessageSize>=5){
		recievedMessage.data[4]			= CAN2->sFIFOMailBox[FifoNo].RDHR;} 								// DATA4	
	if(MessageSize>=6){
		recievedMessage.data[5]			=(CAN2->sFIFOMailBox[FifoNo].RDHR >> 8);} 					// DATA5
	if(MessageSize>=7){
		recievedMessage.data[6]			=(CAN2->sFIFOMailBox[FifoNo].RDHR >> 16);} 					// DATA6		
	if(MessageSize==8){
		recievedMessage.data[7]			=(CAN2->sFIFOMailBox[FifoNo].RDHR >> 24);} 					// DATA7 	
		
	uint8_t regCounter; 															// Set all unused fields with 0xff		
	for(regCounter = 8; regCounter > MessageSize;){
		recievedMessage.data[--regCounter]=0xff;
	}				
	if(FifoNo==CAN_FIFO_0){
	CAN2->RF0R |= CAN_RF0R_RFOM0; //release		
		}
	else{
	CAN2->RF1R |= CAN_RF1R_RFOM1; //release		
		}	
	return recievedMessage;
}



/**
* brief  Can1_Check_Fifo0Pending_StdMessage : check if FIFO 0 empty        
* param  
* param  
* retval 0 empty 1 - not empty
*/	
	

uint32_t Can1_Check_Fifo0Pending_StdMessage(void){		
	if(CAN1->RF0R & CAN_RF0R_FMP0){
		return 1;
	}
	else return 0;
}
	
	void FlushCanTxBuffers(CAN_TypeDef *CAN)	{
 for(int i=0; i<3; i++){
	CAN->sTxMailBox[i].TIR	 = 0; 
	CAN->sTxMailBox[i].TDTR =0; 		
	CAN->sTxMailBox[i].TDLR = 0;  
	CAN->sTxMailBox[i].TDHR = 0;}
}



