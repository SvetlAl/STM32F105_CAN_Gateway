//*****************************************************************
// 
// 
// Ext functions
// todo: 
// Alex Svetlichnyy 2020
//*****************************************************************
#include "ext.h"

void Ext0_pinA_init(void){
	GPIOA->CRL &= ~GPIO_CRL_MODE0;   //очистить разряды MODE
	GPIOA->CRL &= ~GPIO_CRL_CNF0;    //очистить разряды CNF
	GPIOA->CRL |=  GPIO_CRL_CNF0_1;  //дискретный вход, подтяжка к плюсу
	GPIOA->BSRR =  GPIO_BSRR_BS0;    //включить подтягивающий резистор
	AFIO->EXTICR[0] &= ~(AFIO_EXTICR1_EXTI0); //Нулевой канал EXTI подключен к порту PA0
	

  EXTI->PR = EXTI_PR_PR0;      //Сбрасываем флаг прерывания       //перед включением самого прерывания
  EXTI->IMR |= EXTI_IMR_MR0;   //Включаем прерывание 0-го канала EXTI
	
//	EXTI->RTSR |= EXTI_RTSR_TR0; //Прерывание по нарастанию импульса
  EXTI->FTSR |= EXTI_FTSR_TR0; //Прерывание по спаду импульса
	NVIC_EnableIRQ(EXTI0_IRQn);	
}


/*
void EXTI0_IRQHandler(void)
{
  EXTI->PR = EXTI_PR_PR0; //Сбрасываем флаг прерывания 
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
  asm("nop");
}

*/
