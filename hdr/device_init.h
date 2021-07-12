#ifndef DEVICE_INIT_H_INCLUDED
#define DEVICE_INIT_H_INCLUDED

//*****************************************************************
// Header
// STM32F105 device inintialization
// 
// 
// Alexander Svetlichnyy 2021
//*****************************************************************


#include "stm32f10x.h"


#define INDICATOR_LED_ON     GPIOA->BSRR = GPIO_BSRR_BS10		// Toggle evaluation board LED on
#define INDICATOR_LED_OFF    GPIOA->BSRR = GPIO_BSRR_BR10		// Toggle evaluation board LED off

 

	uint32_t ClockInit(void);														// Configuring initial clocks and frequences
	void EnablePeripherals(void);												// Choose the peripherals to be used in the project and set clocking
	
	uint32_t GetProgrammStatus(uint16_t StatusNumber);	// Diagnostics function (status)
	uint32_t HandleProgrammError(uint16_t ErrorNumber); // Diagnostics function (error)

#endif /* DEVICE_INIT_H_INCLUDED*/
