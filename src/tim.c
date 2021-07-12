//*****************************************************************
// 
// 
// Timers functions
// todo: 
// Alex Svetlichnyy 2020
//*****************************************************************
#include "tim.h"

/**
* brief  Basic Timer
* param  TIMx_PSC 
* param  TIMx_ARR, timer number (TIM1, TIM2, TIM3, TIM4)
* retval 0 - OK  1 - Err
*/

void StartBasicTimer(uint16_t prescaler, uint16_t reloadValue, uint32_t timNumber){
	if(timNumber==TIMER1){
		TIM1->PSC = prescaler;
		TIM1->ARR = reloadValue;
    TIM1->CNT = 0;		
		TIM1->CR1 |= TIM_CR1_ARPE; // register is buffered. Updated with UEV event   **Not Important
		TIM1->CR1 |= TIM_CR1_URS;  // Only counter overflow/underflow generates an update interrupt **Not Important
		TIM1->CR1 &= ~TIM_CR1_UDIS;// UEV enabled. The Update (UEV) event is enabled
		TIM1->CR2 |= TIM_CR2_MMS_1; //MMS = 010   Trigger output - update
		
		TIM1->EGR |=TIM_EGR_UG;
		
		TIM1->DIER |= TIM_DIER_UIE; // Enable Interrupts
		
		TIM1->CR1 |= TIM_CR1_CEN; // Start
	}

}



/**
* brief  Timer1 GPIO setup
* param  
* param  Remap
* retval 0 - OK  1 - Err
*/

uint32_t Tim1GpioPwmConfigure (uint16_t isRemapped){
	if(isRemapped==NOREMAP){ 													// TIM1-PA8
		GPIOA->CRH	&= ~GPIO_CRH_CNF8;
		GPIOA->CRH	&= ~GPIO_CRH_MODE8; 			// 
		GPIOA->CRH	|= GPIO_CRH_CNF8_1;				// 10 AF Push-Pull
		GPIOA->CRH	|= GPIO_CRH_MODE8_0;			// 01 10MHz	
		return 0;
	}
	return 1;
}

/**
* brief  Timer2 GPIO setup
* param  
* param  Remap
* retval 0 - OK  1 - Err
*/

uint32_t Tim2GpioPwmConfigure (uint16_t isRemapped){
	if(isRemapped==REMAP){ 													// TIM2 PB3 CH3	
		AFIO->MAPR |= AFIO_MAPR_TIM2_REMAP_PARTIALREMAP1;
		GPIOB->CRL	&= ~GPIO_CRL_CNF3;
		GPIOB->CRL	&= ~GPIO_CRL_MODE3; // reseting GPIO Config bits for PA15
		GPIOB->CRL	|= GPIO_CRL_CNF3_1;				// 10 AF Push-Pull
		GPIOB->CRL	|= GPIO_CRL_MODE3_0;			// 01 10MHz	
		return 0;
	}
	return 1;
}

/**
* brief  Timer3 GPIO setup
* param  
* param  Remap
* retval 0 - OK  1 - Err
*/

uint32_t Tim3GpioPwmConfigure (uint16_t isRemapped){
	if(isRemapped==NOREMAP){ 													// TIM2 PB3 CH3	
		GPIOA->CRL	&= ~GPIO_CRL_CNF6;
		GPIOA->CRL	&= ~GPIO_CRL_MODE6; // reseting GPIO Config bits for PA15
		GPIOA->CRL	|= GPIO_CRL_CNF6_1;				// 10 AF Push-Pull
		GPIOA->CRL	|= GPIO_CRL_MODE6_0;			// 01 10MHz	
		return 0;
	}
	return 1;
}





/**
* brief  Timer1 PWM start. Reading value CCR1 and comparing with ARP. Out of this computing PWM value.
* param  
* param  
* retval 0 - OK  1 - Err
// OC1REF (выходной сигнал пина) активен при CNT<CCR1 вот и весь ШИМ
*/
uint32_t Tim1Ch1PwmStart(){
	TIM1->SMCR &= ~TIM_SMCR_SMS; //SMS==000 No slave mode
	
	TIM1->CCR1 = 0;
	// Main module PSC, ARR
	TIM1->PSC = 0; 				// Counting from 0 up to PWM_MAX (255) 
	TIM1->ARR = PWM_MAX; 	// As soon as we use PWM to handle RGB-diode, this would be more convenient to use 0-255 counter
	
	// MODE (in our case - PWM)
	
	TIM1->CR1   |= TIM_CR1_ARPE; 		// register is buffered. Updated with UEV event   **Not Important
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE; // ** CCR1 updated at each update event
	//*********** по инструкции так, но попробуем и по-другому*************
	TIM1->CR2   |= TIM_CR2_MMS_2;  // OC1REF signal is used as trigger output (TRGO)
	TIM1->CCER  |= TIM_CCER_CC1E;   // Enable Capture/COMPARE (CCR1->CCR1) register 
	TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;  //CCMR1->OC2M == 110   PWM mode 1 ** Not Important
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2; 	
	// START
	TIM1->EGR |= TIM_EGR_UG;  // First toggle UG bit to synchronize Buffer ARR and register ARR before a timer starts
	TIM1->CR1 |= TIM_CR1_CEN;
	TIM1->BDTR |= TIM_BDTR_MOE;  // Как оказалось, без этого не работает
	return 0;
}








/**
* brief  Timer2 PWM start. Reading value CCR1 and comparing with ARP. Out of this computing PWM value.
* param  
* param  
* retval 0 - OK  1 - Err

*/
uint32_t Tim2Ch2PwmStart(){
	// TIM clock
	TIM2->SMCR &= ~TIM_SMCR_SMS; //SMS==000 disabled
	
	// Main module PSC, ARR
	TIM2->CCR2 = 0; 
	TIM2->PSC = 0; //Setting TIM_PSC for a certain frequence
	TIM2->ARR = PWM_MAX; // As soon as we use PWM to handle RGB-diode, this would be more convenient to use 0-255 counter	
	// MODE (in our case - PWM)	
	//*********** по инструкции так, но попробуем и по-другому*************
	TIM2->CR1   |= TIM_CR1_ARPE;
	TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
	//*********** по инструкции так, но попробуем и по-другому*************
	TIM2->CR2   |= TIM_CR2_MMS_2;
	TIM2->CCER  |= TIM_CCER_CC2E;   // Enable Capture/COMPARE (CCR1->CCR1) register
	TIM2->CCMR1 &= ~TIM_CCMR1_OC2M;  //CCMR1->OC2M == 110   PWM mode 1
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_1;
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_2; 	
	// START
	TIM2->EGR |= TIM_EGR_UG;  // First toggle UG bit to synchronize Buffer ARR and register ARR before a timer starts
	TIM2->CR1 |= TIM_CR1_CEN;
	return 0;	
}








/**
* brief  Timer3 PWM start. Reading value CCR1 and comparing with ARP. Out of this computing PWM value.
* param  
* param  
* retval 0 - OK  1 - Err

*/
uint32_t Tim3Ch1PwmStart(){
	// TIM clock
	TIM3->SMCR &= ~TIM_SMCR_SMS; //SMS==000 disabled
	
	// Main module PSC, ARR
	TIM3->CCR1 = 0; 
	TIM3->PSC = 0; //Setting TIM_PSC for a certain frequence
	TIM3->ARR = PWM_MAX; // As soon as we use PWM to handle RGB-diode, this would be more convenient to use 0-255 counter
	
	
	// MODE (in our case - PWM)
	
	//*********** по инструкции так, но попробуем и по-другому*************
	TIM3->CR1   |= TIM_CR1_ARPE;
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
	//*********** по инструкции так, но попробуем и по-другому*************
	TIM3->CR2   |= TIM_CR2_MMS_2;
	TIM3->CCER  |= TIM_CCER_CC1E;   // Enable Capture/COMPARE (CCR1->CCR1) register
	TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;  //CCMR1->OC2M == 110   PWM mode 1
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_2; 
	
	// START
	TIM3->EGR |= TIM_EGR_UG;  // First toggle UG bit to synchronize Buffer ARR and register ARR before a timer starts
	TIM3->CR1 |= TIM_CR1_CEN;
	return 0;			
}


/**
* brief  Timer1 PWM updater
* param  
* param  updateValue
* retval 

*/
void Tim1Ch1PwmUpdate(uint16_t updateValue){
	TIM1->CCR1 = updateValue; 
}

/**
* brief  Timer1 PWM updater
* param  
* param  updateValue
* retval 

*/
void Tim2Ch2PwmUpdate(uint16_t updateValue){
	TIM2->CCR2 = updateValue; 
}
/**
* brief  Timer1 PWM updater
* param  
* param  updateValue
* retval 

*/
void Tim3Ch1PwmUpdate(uint16_t updateValue){
	TIM3->CCR1 = updateValue; 
}

/**
* brief  Timer1 configure gpio for capture
* param  
* param REMAP
* retval 

*/

uint32_t Tim1GpioCaptureConfigure (uint16_t isRemapped, uint16_t mode){
	if(isRemapped==NOREMAP){ 	
		if(mode==ACTIVE_FALLING_PULLUP){
		GPIOA->CRH &= ~GPIO_CRH_MODE8;   //очистить разряды MODE
		GPIOA->CRH &= ~GPIO_CRH_CNF8;    //очистить разряды CNF
		GPIOA->CRH |=  GPIO_CRH_CNF8_1;  //дискретный вход, подтяжка к плюсу
//		GPIOA->CRH |=  GPIO_CRH_CNF8_0;  //дискретный вход, третье состояние
		GPIOA->BSRR =  GPIO_BSRR_BS8;    //включить подтягивающий резистор
		return 0;
		}
		if(mode==FREE_INPUT){
		GPIOA->CRH &= ~GPIO_CRH_MODE8;   //очистить разряды MODE
		GPIOA->CRH &= ~GPIO_CRH_CNF8;    //очистить разряды CNF
//		GPIOA->CRH |=  GPIO_CRH_CNF8_1;  //дискретный вход, подтяжка к плюсу
		GPIOA->CRH |=  GPIO_CRH_CNF8_0;  //дискретный вход, третье состояние
//		GPIOA->BSRR =  GPIO_BSRR_BS8;    //включить подтягивающий резистор
		return 0;
		}	
	}	
return 1;
}

/**
* brief  Timer1  capture button edge
* param  
* param 
* retval 

*/

uint32_t Tim1Ch1CaptureButtonStart(uint16_t prescaler, uint16_t preloadValue, uint32_t smsMode){
	if(smsMode==SLAVE_MODE_OFF){
		TIM1->SMCR &= ~TIM_SMCR_SMS; //SMS==000 No slave mode
	
		TIM1->CCR1 = 0;
		TIM1->PSC = prescaler; 				// 
		TIM1->ARR = preloadValue; 	  // 

		TIM1->CCMR1 |= TIM_CCMR1_CC1S_0; // Setting PA8(TIM1_CH1) as input (CC1S = 01 means IC1=TI1)
		TIM1->CCMR1 |= TIM_CCMR1_IC1F_0;
		TIM1->CCMR1 |= TIM_CCMR1_IC1F_1; // Sampling time. Частота выборок
	
//	TIM1->CCER  |= TIM_CCER_CC1P;  //Active falling edge
		TIM1->CCER  &=~TIM_CCER_CC1P;  //Active rising edge
	
		TIM1->CCMR1 &= ~TIM_CCMR1_IC1PSC; // входной предделитель не нужен, т.к отслеживаем каждый фронт
	
		TIM1->DIER |= TIM_DIER_CC1IE; // Enable Capture/Compare Interrupt	
		TIM1->CCER  |= TIM_CCER_CC1E;	 // Enable Capture/Compare
	
		TIM1->CR1   |= TIM_CR1_ARPE; 		// register is buffered. Updated with UEV event   **Not Important
		TIM1->CCMR1 |= TIM_CCMR1_OC1PE; // ** CCR1 updated at each update event
	//*********** по инструкции так, но попробуем и по-другому*************
		TIM1->CR2 |= TIM_CR2_MMS_1; //MMS = 010   Trigger output - update
	
	// START
		TIM1->EGR |= TIM_EGR_UG;  // First toggle UG bit to synchronize Buffer ARR and register ARR before a timer starts
		TIM1->CR1 |= TIM_CR1_CEN;
		return 0;
	}
	
	
	
	
	
	
	
	
		if(smsMode==SLAVE_MODE_GATED){
		TIM1->SMCR &= ~TIM_SMCR_SMS; 		//Reset SMS
		TIM1->SMCR |=  TIM_SMCR_SMS_0; 
		TIM1->SMCR |=  TIM_SMCR_SMS_2; //SMS==101 Gated Mode

				
		TIM1->CCR1 = 0;
	// 
		TIM1->PSC = prescaler; 				// 
		TIM1->ARR = preloadValue; 	  // 

		TIM1->CCMR1 |= TIM_CCMR1_CC1S_0; // Setting PA8(TIM1_CH1) as input (CC1S = 01 means IC1=TI1)
		TIM1->CCMR1 &= ~TIM_CCMR1_IC1F;
		TIM1->CCMR1 |= TIM_CCMR1_IC1F_0;
		TIM1->CCMR1 |= TIM_CCMR1_IC1F_1; // Sampling time. Частота выборок
		TIM1->CCMR1 |= TIM_CCMR1_IC1F_2;
		TIM1->CCMR1 |= TIM_CCMR1_IC1F_3; // Sample time - maximum
			
			
			
	
		TIM1->CCER  |= TIM_CCER_CC1P;  //Active falling edge
//		TIM1->CCER  &=~TIM_CCER_CC1P;  //Active rising edge
				
		TIM1->SMCR |=  TIM_SMCR_TS_0; // Завязано на сигнал с канала CH1
		TIM1->SMCR |=  TIM_SMCR_TS_2; //TS==101 source is a signal from CH1
				
				
	
		TIM1->CCMR1 &= ~TIM_CCMR1_IC1PSC; // входной предделитель не нужен, т.к отслеживаем каждый фронт
		TIM1->CR1   |= TIM_CR1_ARPE; 		// register is buffered. Updated with UEV event   **Not Important

		TIM1->CR2 |= TIM_CR2_MMS_1; //MMS = 010   Trigger output - update
		
//		TIM1->EGR |=  TIM_EGR_TG; 		//trigger event
		TIM1->DIER |= TIM_DIER_TIE; 	// Trigger Interrupt	
	
	// START
//		TIM1->EGR |= TIM_EGR_UG;  // First toggle UG bit to synchronize Buffer ARR and register ARR before a timer starts
		TIM1->CR1 |= TIM_CR1_CEN;
		return 0;
	}
		
		
		
		
		
		
		
		
		
	return 1;	
}







 /*//TIM1 Update Interrupt
void TIM1_UP_IRQHandler(){
	if(TIM1->SR & TIM_SR_UIF){
		GPIOC->BSRR |= GPIO_BSRR_BS13;		// Сбросили бит. 
	Delay_mS(50);
	TIM1->SR &= ~TIM_SR_UIF; // Flag down
	}
	TIM1->SR &= ~TIM_SR_UIF;
}




read and reset Capture/Compare Register
void TIM1_CC_IRQHandler(){
	if(TIM1->SR & TIM_SR_CC1IF){
			TIM1->CCR1 &= ~ 0xFFFF;
			TIM1->SR &=~ TIM_SR_CC1IF;
	}
		if(TIM1->SR & TIM_SR_CC1OF)
		{
			TIM1->SR &=~ TIM_SR_CC1OF;
		}
	}








*/




