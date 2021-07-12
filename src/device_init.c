//*****************************************************************
// Functions for initial STM32F105 configuration
// e.g. clocking
// 
// Alexander Svetlichnyy 2021
//*****************************************************************
#include "device_init.h"

/**
 * brief  ClockInit - Enable clocking of the STM32f105
 * param  
 * param  
 * retval 0 OK 1 Error
 */
uint32_t ClockInit(void)
{
// Inside this function we set configuration bits in RCC_CR and RCC_CFGR
//Our purpose is to configure HSE, PLL and all the Clock Control except certain Peripherials 
	uint32_t HSE_Status = 0;
	uint32_t HSE_TimeoutCounter = 0;
	RCC->CR |= RCC_CR_HSEON;			// Engaging HSE

// Inside this loop we`re waiting for the flag HSERDY to be set within defined timeout period
	while((HSE_TimeoutCounter < HSEStartUp_TimeOut) &&  HSE_Status == 0)
	{
		HSE_TimeoutCounter++;
		if ((RCC->CR & RCC_CR_HSERDY) != 0){
			HSE_Status = 1;
		}		
	}	
// If HSE has successfully started, we continue with RCC configuration
	if(HSE_Status != 0){
//Since we`re going to use 72Mhz we have to set up flash memory access latency to "two wait" mode
//FLASH_ACR_LATENCY_2 was defined as uint8_t, ergo we declare it as uint32_t
		FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;
//Next Step is to set up all the PLL configuration bits BEFORE it started	


			RCC->CFGR |= RCC_CFGR_PLLMULL4;    //PLL *9
			RCC->CFGR |= RCC_CFGR_PLLSRC_PREDIV1;  // PLL source is HSE
			RCC->CFGR2 |= RCC_CFGR2_PREDIV1_0;

// Here we go! Starting PLL!
		RCC->CR |= RCC_CR_PLLON;
		while ((RCC->CR & RCC_CR_PLLRDY) == 0){}; //Make sure PLL has started
			
		RCC->CR |= RCC_CR_PLL3ON;
		while ((RCC->CR & RCC_CR_PLL3RDY) == 0){}; //Make sure PLL has started
			
		RCC->CFGR |= RCC_CFGR_SW_1; // PLL was selected as system clock switch
		while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0){}; //Make sure this worked out	
		return 0; // RCC has started for good!!!
	}
	else{
		return 1; //RCC hasn`t started and we need to sort this out.
	}	
}

/**
 * brief  EnablePeripherals - Enable clocking of the peripherals that will be used
 * param  
 * param  
 * retval
 */
void EnablePeripherals(void)
{
//GPIO	
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC -> APB2ENR |= RCC_APB2ENR_IOPCEN;
//	RCC -> APB2ENR |= RCC_APB2ENR_IOPDEN;
//ADC1
// RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;
//AFIO	//EXT
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
//DAC1
//RCC->APB1ENR |= RCC_APB1ENR_DACEN;
//CAN1
  RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
	//CAN2
  RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
//DMA1	
//	RCC->AHBENR |=RCC_AHBENR_DMA1EN;

//I2C
//	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
//SPI
//	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
//TIM
// RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
// RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
// RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	
//USART1	
//	RCC->APB2ENR	|= RCC_APB2ENR_USART1EN;
//USART2	
//	RCC->APB1ENR	|= RCC_APB1ENR_USART2EN;		
//USB
//RCC->APB1ENR |= RCC_APB1ENR_USBEN;	



AFIO->MAPR|=AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
}



// TODO: Enhance USART procedures
/**
 * brief  GetProgrammStatus - Diagnostics function that sends a certain value
 * brief  associated with a certain event. This function is used wherever you want
 * brief  to make sure a programm runs up to this point.
 * param  
 * param  Status number defined in the header file
 * retval
 */
uint32_t GetProgrammStatus(uint16_t StatusNumber){
/*	unsigned char StatusHDR[6]={' ',' ','S','T','A','_'};
	unsigned char StatusChar[5];	
	Convert_Uint16_into_String5(StatusNumber, StatusChar);

	Dma1Usart1_SendByte(&StatusHDR[0]);
	Dma1Usart1_SendByte(&StatusHDR[1]);
	Dma1Usart1_SendByte(&StatusHDR[2]);	
	Dma1Usart1_SendByte(&StatusHDR[3]);	
	Dma1Usart1_SendByte(&StatusHDR[4]);
	Dma1Usart1_SendByte(&StatusHDR[5]);	

	Dma1Usart1_SendByte(&StatusChar[4]);
	Dma1Usart1_SendByte(&StatusChar[3]);
	Dma1Usart1_SendByte(&StatusChar[2]);	
	Dma1Usart1_SendByte(&StatusChar[1]);	
	Dma1Usart1_SendByte(&StatusChar[0]);		
	*/
	return (uint32_t)StatusNumber;
}

/**
 * brief  HandleProgrammError - Diagnostics function that handles errors
 * param  
 * param  Error number defined in the header file
 * retval
 */
uint32_t HandleProgrammError(uint16_t ErrorNumber){
/*	unsigned char ErrHDR[5]={' ',' ','E','R','_'};
	unsigned char ErrorChar[5];		
	Convert_Uint16_into_String5(ErrorNumber, ErrorChar);
	
	Dma1Usart1_SendByte(&ErrHDR[0]);
	Dma1Usart1_SendByte(&ErrHDR[1]);
	Dma1Usart1_SendByte(&ErrHDR[2]);	
	Dma1Usart1_SendByte(&ErrHDR[3]);	
	Dma1Usart1_SendByte(&ErrHDR[4]);		
		
	Dma1Usart1_SendByte(&ErrorChar[4]);
	Dma1Usart1_SendByte(&ErrorChar[3]);
	Dma1Usart1_SendByte(&ErrorChar[2]);	
	Dma1Usart1_SendByte(&ErrorChar[1]);	
	Dma1Usart1_SendByte(&ErrorChar[0]);
	*/
	return (uint32_t)ErrorNumber;
}














