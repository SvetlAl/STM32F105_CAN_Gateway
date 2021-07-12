//*****************************************************************
// CAN-Gateway based on Starline 2CAN 2LIN device
// 
// 
// Alex Svetlichnyy 2021
//*****************************************************************
#include "main.h"


volatile uint32_t TimingDelay;    														 // The value for Systick delay

/* Setting up a systick interrupt. It counts from a delay value to zero*/
void SysTick_Handler(void) {   
	if (TimingDelay){
		TimingDelay--;
  }      
}
/* Setting up a delay value*/
void Delay_mS(uint32_t nTime){
	TimingDelay = nTime;
	while (TimingDelay);
 }

 
 	typedef struct speed{	
	uint8_t data[2];			// Data 0 - first 1- second
} speed; //creating new type
	
	
	uint32_t convertSpeedToUint(speed sp){
		uint32_t convertedSpeed;
		uint32_t A = (uint32_t)sp.data[1]*100/128;
		uint32_t B = (uint32_t)sp.data[0]*200;
		convertedSpeed = (B+A);
		return convertedSpeed;
	}

	speed convertUint32toSpeed(uint32_t sp){
		uint32_t counter=0;
		while(sp>200){
			sp=sp-200;
			counter++;
		}
		speed result;
		result.data[0]=(uint8_t)counter;
		result.data[1]=(uint8_t)(sp*128/100);
		return result;
	}
		
	speed calculate_NewSpeed(speed value, uint32_t mod)
	{
		uint32_t sp = convertSpeedToUint(value); 
		sp=sp*mod/100;

		speed result = convertUint32toSpeed(sp);
		return result;
	}
 
 
 
 
 
 // bypass channel
 void CAN1_RX0_IRQHandler(void) {
	if(CAN1->RF0R & CAN_RF0R_FMP0){
		// Recieve a new message
		Std_Can_Message_Type newMessage = Can1_Recieve_StdMessage(CAN_FIFO_0);
		//				newMessage.data[7]=0xF0;
			if(Can2_Transmit_StdMessage(newMessage)!=ERR_CAN1_NO_EMPTY_MAILBOX){
						IWDG_reset();
			}
// CAN_RF0R_FMP0 bit is cleared by hardware. 
	}
}
 

// Filtered channel
 void CAN1_RX1_IRQHandler(void) {		
	if(CAN1->RF1R & CAN_RF1R_FMP1){

		// Recieve a new message
	Std_Can_Message_Type newMessage = Can1_Recieve_StdMessage(CAN_FIFO_1);
		speed curS;
		curS.data[0]=newMessage.data[0];
		curS.data[1]=newMessage.data[1];
		
		
		speed newS;
		newS = calculate_NewSpeed(curS, 110);
		newMessage.data[0]=newS.data[0];
		newMessage.data[1]=newS.data[1];
		
	//	newMessage.data[7]=0xF1;
		if(Can2_Transmit_StdMessage(newMessage)!=ERR_CAN1_NO_EMPTY_MAILBOX){
						IWDG_reset();
			}
			
// CAN_RF0R_FMP0 bit is cleared by hardware. 
	}
}
 
 void CAN2_RX0_IRQHandler(void) {
//	 	GPIOA->BSRR |= GPIO_BSRR_BS10;
	if(CAN2->RF0R & CAN_RF0R_FMP0){
		// Recieve a new message
		Std_Can_Message_Type newMessage = Can2_Recieve_StdMessage(CAN_FIFO_0);

			if(Can1_Transmit_StdMessage(newMessage)!=ERR_CAN1_NO_EMPTY_MAILBOX){
						IWDG_reset();
					 
			}
// CAN_RF0R_FMP0 bit is cleared by hardware. 
	}
}
 

int main(void){
	ClockInit();   							// Start HSE, PLL, Flash latency, all the RCC configuration
	EnablePeripherals(); 				// Enable all the Peripherial clocks
	
	SysTick_Config(72000-1); 		// interrupt in 10 states
	Init_IWDG(STARTUP_TIMEOUT);
	


	

	
	Can1_Initializate(REMAP_CAN1, CAN_BAUDRATE_500KB);  
	Can2_Initializate(REMAP_CAN2, CAN_BAUDRATE_500KB);  
	Can1_InitializateFilters_GatewayTemplate();
	
	// LED INDICATOR INIT
GPIOA->CRH	&= ~GPIO_CRH_CNF10;						// 
GPIOA->CRH 	|= GPIO_CRH_MODE10_0;					//
GPIOA->BSRR |= GPIO_BSRR_BS10;						// 
	
		// CAN TRANCIEVERS STB INIT
GPIOB->CRL	&= ~GPIO_CRL_CNF7;						//  CAN1
GPIOB->CRL 	|= GPIO_CRL_MODE7_0;					//
GPIOB->CRL 	|= GPIO_CRL_MODE7_1;
GPIOB->BSRR |= GPIO_BSRR_BR7;						// 
	
GPIOB->CRH	&= ~GPIO_CRH_CNF14;						//  CAN2
GPIOB->CRH 	|= GPIO_CRH_MODE14_0;					//
GPIOB->CRH 	|= GPIO_CRH_MODE14_1;
GPIOB->BSRR |= GPIO_BSRR_BR14;						// 

	
	Delay_mS(100);
	IWDG_reset();
	GPIOA->BSRR |= GPIO_BSRR_BR10;;
	Delay_mS(100);
	IWDG_reset();
	GPIOA->BSRR |= GPIO_BSRR_BS10;
	Delay_mS(100);
	IWDG_reset();
	GPIOA->BSRR |= GPIO_BSRR_BR10;
	
	
	
	
	NVIC_EnableIRQ(CAN1_RX1_IRQn);	
	NVIC_SetPriority(CAN1_RX1_IRQn, 3);
	

	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	NVIC_SetPriority(CAN1_RX0_IRQn, 4);
	NVIC_EnableIRQ(CAN2_RX0_IRQn);
	NVIC_SetPriority(CAN2_RX0_IRQn, 5);
	__enable_irq ();
	
		Std_Can_Message_Type Message1;

	Message1.id_highbyte=0xC;
	Message1.id_lowbyte=0x55;
	Message1.dlc=8;
	Message1.data[0]=0x55 ;
	Message1.data[1]=0xFF;
	Message1.data[2]=0xFF;
	Message1.data[3]=0xFF;
	Message1.data[4]=0xFF;
	Message1.data[5]=0xFF;
	Message1.data[6]=0xFF;
	Message1.data[7]=0xFF;
	
	Can1_Transmit_StdMessage(Message1);
	
	
		
	/*static Std_Can_Message_Type Message1;
	static Std_Can_Message_Type Message2;

	Message1.id_highbyte=0x0;
	Message1.id_lowbyte=0x11;
	Message1.dlc=8;
	Message1.data[0]=0x11 ;
	Message1.data[1]=0x11;
	Message1.data[2]=0x11;
	Message1.data[3]=0x11;
	Message1.data[4]=0x11;
	Message1.data[5]=0x11;
	Message1.data[6]=0x11;
	Message1.data[7]=0x11;
	
	Message2.id_highbyte=0x0;
	Message2.id_lowbyte=0x22;
	Message2.dlc=8;
	Message2.data[0]=0x22 ;
	Message2.data[1]=0x22;
	Message2.data[2]=0x22;
	Message2.data[3]=0x22;
	Message2.data[4]=0x22;
	Message2.data[5]=0x22;
	Message2.data[6]=0x22;
	Message2.data[7]=0x22;
*/
	while(1){
/*
		Delay_mS(50);
		Can1_Transmit_StdMessage(Message1);	
		Delay_mS(50);
		Can2_Transmit_StdMessage(Message2);	
		*/

			}						
}
