/**
  ******************************************************************************
  * @file		keypad4x4-scanning.c
  * @author	Alex Svetlichnyy
  * @date		Jan 2020
  ******************************************************************************
  */

/** Includes ---------------------------------------------------------------- */
#include "keypad4x4-scanning.h"
#include "usart.h"

/**
  ******************************************************************************
  * @brief	Scan keypad
  * @param	None
  * @retval	Pressed key
  ******************************************************************************
  */
uint8_t scan4b4KeyPad(){
	uint8_t result = 0;
	uint8_t scanOut = 0;
	uint8_t rowCounter;
	
	for(rowCounter=0; rowCounter<5;rowCounter++){
		scanOut=scanRow(rowCounter);
		if(scanOut!=0){
			result = 4*(rowCounter-1)+scanOut;
			return result;
		}
	}	
	return 0;
}



/**
  ******************************************************************************
  * @brief	Scan a single row
  * @param	row number
  * @retval	Active Column
  ******************************************************************************
  */
uint8_t scanRow(uint8_t rowNumber){
	uint8_t result=0;
	uint32_t timeout=0xFF;
	if(rowNumber==1){
			GPIOA->BSRR |=  GPIO_BSRR_BS10;		
			timeout=0xFF;
			while(timeout--);  //Power up the row №X 
		
		
			if(GPIOB->IDR & GPIO_IDR_IDR5){				
			result = 1;	
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR13){
			result = 2;
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR12){
			result = 3;
			}			
			else if(GPIOB->IDR & GPIO_IDR_IDR4){
			result = 4;
			}
			GPIOA->BSRR |=  GPIO_BSRR_BR10;		     // Power down the row
			return result;
	}
	
	else	if(rowNumber==2){
			GPIOA->BSRR |=  GPIO_BSRR_BS9;		
			timeout=0xFF;
			while(timeout--);		 //Power up the row №X 
		
		
			if(GPIOB->IDR & GPIO_IDR_IDR5){				
			result = 1;
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR13){
			result = 2;
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR12){
			result = 3;
			}			
			else if(GPIOB->IDR & GPIO_IDR_IDR4){
			result = 4;
			}
			GPIOA->BSRR |=  GPIO_BSRR_BR9;		     // Power down the row
			return result;
	}
	
		else	if(rowNumber==3){
			GPIOA->BSRR |=  GPIO_BSRR_BS8;			
			timeout=0xFF;
			while(timeout--);			//Power up the row №X 
			
			if(GPIOB->IDR & GPIO_IDR_IDR5){				
			result = 1;
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR13){
			result = 2;
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR12){
			result = 3;
			}			
			else if(GPIOB->IDR & GPIO_IDR_IDR4){
			result = 4;
			}
			GPIOA->BSRR |=  GPIO_BSRR_BR8;		     // Power down the row
			return result;
	}
	
			else	if(rowNumber==4){
			GPIOB->BSRR |=  GPIO_BSRR_BS15;
			timeout=0xFF;
			while(timeout--);				//Power up the row №X 
				
			if(GPIOB->IDR & GPIO_IDR_IDR5){				
			result = 1;
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR13){
			result = 2;
			}
			else if(GPIOB->IDR & GPIO_IDR_IDR12){
			result = 3;
			}			
			else if(GPIOB->IDR & GPIO_IDR_IDR4){
			result = 4;
			}
			GPIOB->BSRR |=  GPIO_BSRR_BR15;		     // Power down the row
			return result;
	}
	
	
	else return 0;
}


/**
  ******************************************************************************
  * @brief	InitKeypadPorts
  * @param	
  * @retval	
  ******************************************************************************
  */
void initKeypadPorts(void){
	#if KEYPAD_PORT_1 == 0xA10
	GPIOA->CRH	&= ~GPIO_CRH_CNF10;
	GPIOA->CRH	&= ~GPIO_CRH_MODE10;	
	GPIOA->CRH 	|=  GPIO_CRH_MODE10_0;
	GPIOA->BSRR |=  GPIO_BSRR_BR10;  // output mode, reset
	#endif
	
	#if KEYPAD_PORT_2 == 0xA9
	GPIOA->CRH	&= ~GPIO_CRH_CNF9;
	GPIOA->CRH	&= ~GPIO_CRH_MODE9;	
	GPIOA->CRH 	|=  GPIO_CRH_MODE9_0;
	GPIOA->BSRR |=  GPIO_BSRR_BR9;  // output mode, reset
	#endif	
	
	#if KEYPAD_PORT_3 == 0xA8
	GPIOA->CRH	&= ~GPIO_CRH_CNF8;
	GPIOA->CRH	&= ~GPIO_CRH_MODE8;	
	GPIOA->CRH 	|=  GPIO_CRH_MODE8_0;
	GPIOA->BSRR |=  GPIO_BSRR_BR8;  // output mode, reset
	#endif	
	
	#if KEYPAD_PORT_4 == 0xB15
	GPIOB->CRH	&= ~GPIO_CRH_CNF15;
	GPIOA->CRH	&= ~GPIO_CRH_MODE15;	
	GPIOB->CRH 	|=  GPIO_CRH_MODE15_0;
	GPIOB->BSRR |=  GPIO_BSRR_BR15;  // output mode, reset
	#endif		
	

	#if KEYPAD_PORT_5 == 0xB5
	GPIOB->CRL &= ~GPIO_CRL_MODE5;  
	GPIOB->CRL &= ~GPIO_CRL_CNF5;   
	GPIOB->CRL |=  GPIO_CRL_CNF5_1; 
	GPIOB->BSRR |=  GPIO_BSRR_BR5;   //Pulldown
	#endif		

	#if KEYPAD_PORT_6 == 0xB13
	GPIOB->CRH &= ~GPIO_CRH_MODE13;  
	GPIOB->CRH &= ~GPIO_CRH_CNF13;   
	GPIOB->CRH |=  GPIO_CRH_CNF13_1; 
	GPIOB->BSRR |=  GPIO_BSRR_BR13;   //Pulldown
	#endif		
	
	#if KEYPAD_PORT_7 == 0xB12
	GPIOB->CRH &= ~GPIO_CRH_MODE12;  
	GPIOB->CRH &= ~GPIO_CRH_CNF12;   
	GPIOB->CRH |=  GPIO_CRH_CNF12_1; 
	GPIOB->BSRR |=  GPIO_BSRR_BR12;   //Pulldown
	#endif		
	
	#if KEYPAD_PORT_8 == 0xB4
	GPIOB->CRL &= ~GPIO_CRL_MODE4;   
	GPIOB->CRL &= ~GPIO_CRL_CNF4;    
	GPIOB->CRL |=  GPIO_CRL_CNF4_1;  
	GPIOB->BSRR |=  GPIO_BSRR_BR4;    //Pulldown
	#endif		
	
}

/**
  ******************************************************************************
  * @brief	translate 0-16 into 1*#ABCD
  * @param	
  * @retval	
  ******************************************************************************
  */
uint8_t translateKeyNumbertoAscii(uint8_t key){
	if(key==1)return '1';
	else if(key==2)return '2';
	else if(key==3)return '3';
	else if(key==4)return 'A';
	else if(key==5)return '4';
	else if(key==6)return '5';
	else if(key==7)return '6';
	else if(key==8)return 'B';
	else if(key==9)return '7';
	else if(key==10)return '8';
	else if(key==11)return '9';
	else if(key==12)return 'C';
	else if(key==13)return '*';
	else if(key==14)return '0';
	else if(key==15)return '#';
	else if(key==16)return 'D';	
	else return 0;
}









/********************************* END OF FILE ********************************/
/******************************************************************************/
