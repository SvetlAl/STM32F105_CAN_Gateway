#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

#include "stm32f10x.h"
#include "dma.h"


void Adc3ChConfigure(void);
void Dma1GetAdc1Data3Channels(uint16_t *pdata);

#endif /* ADC_H_INCLUDED*/
