//*****************************************************************
// 
// 
// ADC functions
// todo: 
// Alex Svetlichnyy 2020
//*****************************************************************

#include "adc.h"

//*************** USED PORTS PA0 PA1 PA2 for regular channels ***************

/**
* brief  PA0 PA1 PA2 Configured for ADC
* param   
* param  
* retval 
*/

void Adc3ChConfigure()
{
	//*************** PORTS PA0 PA1 PA2 Configuration ***************
	GPIOA->CRL	&= ~0xFFF; //Setting pins PA0 PA1 PA2 all to "0000", to have them with Analog Input mode
  
	// number of used channels 
	ADC1->SQR1 &= ~0xFFFFFF;
	ADC1->SQR2 &= ~0xFFFFFFFF;
	ADC1->SQR3 &= ~0xFFFFFFFF;    //Reset all the ADC1_SQRx bits
	ADC1->SQR1 |= 0x200000;       //Set ADC_SQR1 L Channel numbers to 3
	ADC1->SQR3 |= 0x820; 					//Set ADC_SQR3 SQ1=0 SQ1=1 SQ2=2
	
	ADC1->SMPR1 &= ~0xFFFFFF;
	ADC1->SMPR2 &= ~0x3FFFFFFF;   // Reset all the ADC1_SMPRx bits
	ADC1->SMPR2 |= 0x92;          // Set SMP2=010 SMP1=010 SMP0=010    010==13.5 CYCLES
	
	ADC1->CR1 &= ~0x3FFFFFF;
	ADC1->CR1 |= ADC_CR1_SCAN; // SCAN mode ON,          /////End of Conversion Inerrupt 
	
	ADC1->CR2 &= ~0x3FFFFFF;
	ADC1->CR2 |= ADC_CR2_DMA | ADC_CR2_CONT;   // DMA mode enabled, Continious mode enabled
	
	ADC1->CR2 |= ADC_CR2_ADON; // Enable ADC1
	
__NOP();	// Two dummy states before ADC Calibration
__NOP();
	
	ADC1->CR2 |= ADC_CR2_RSTCAL;					// Start the ADC Cal.rst procedure
	while(ADC1->CR2 & ADC_CR2_RSTCAL)  // and wait until it`s completed
	{
	}
	ADC1->CR2 |= ADC_CR2_CAL; 							// Start the ADC Cal procedure
	while(ADC1->CR2 & ADC_CR2_CAL)  // and wait until it`s completed
	{
	}
	
	ADC1->CR2 |= ADC_CR2_ADON; // Start conversion		
}







/**
* brief  Get data from 3 channels
* param   
* param  
* retval 
*/

void Dma1GetAdc1Data3Channels(uint16_t *pdata){
	while(!(ADC1->SR & ADC_SR_EOC)){}; 			  //wait until last transmission complete	
	DMA_Disable(DMA1_Channel1); 				 				// ADC is linked to DMA1 Ch1
	DMA_DeInit(DMA1_Channel1);
	DMA_Init( DMA1_Channel1,										// Channel4 is used for USART1
              (uint32_t)&(ADC1->DR),				// Data register (peripherals)
              (uint32_t)pdata,								// Buffer address(memory)
              0x3,														// Buffer size 
              TransCompl_Int_Disable      +		// Transmission complete interrupt disabled 
              HalfCompl_Int_Disable       +		// Half data completed interrupt disabled 
              TransError_Int_Disable      +		// Error interrupt disabled
              ReadPerif                  +		// 
              CircularMode_Disable        +		// Circular mode disabled ******IMPORTANT
              PeripheralInc_Disable       +		// Peripheral address not increased 
              MemoryInc_Enable           +		// Memory address increased 
              PDataSize_W                 +		// One-Byte sized data in the peripherals
              MDataSize_W                 +		// One-Byte sized data in the memory
              DMA_Priority_Hi             +		// Priority
              M2M_Disable                 );	// Not from memory to memory
 
	DMA1->IFCR = DMA_IFCR_CTCIF1;								// Reset an empty register flag to prevent zero-byte transmission
  DMA_Enable(DMA1_Channel1);									// Start
}




