#ifndef TIM_H_INCLUDED
#define TIM_H_INCLUDED
#include "stm32f10x.h"

#define CK_CNT_FREQUENCY_1MHZ 71 
#define PWM_MAX 255
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3
#define TIMER4 4

#define NOREMAP 0
#define REMAP 1

#define SLAVE_MODE_OFF 0
#define SLAVE_MODE_GATED 1
#define SLAVE_MODE_TRIGGER 2

#define FREE_INPUT								0
#define ACTIVE_FALLING_PULLUP			1
#define ACTIVE_RISING_PULLDOWN		2







void StartBasicTimer(uint16_t prescaler, uint16_t reloadValue, uint32_t timNumber);

uint32_t Tim1GpioPwmConfigure (uint16_t isRemapped);
uint32_t Tim2GpioPwmConfigure (uint16_t isRemapped);
uint32_t Tim3GpioPwmConfigure (uint16_t isRemapped);



uint32_t Tim1Ch1PwmStart(void);
uint32_t Tim2Ch2PwmStart(void);
uint32_t Tim3Ch1PwmStart(void);

void Tim1Ch1PwmUpdate(uint16_t updateValue);
void Tim2Ch2PwmUpdate(uint16_t updateValue);
void Tim3Ch1PwmUpdate(uint16_t updateValue);


uint32_t Tim1GpioCaptureConfigure (uint16_t isRemapped, uint16_t mode);
uint32_t Tim1Ch1CaptureButtonStart(uint16_t prescaler, uint16_t preloadValue, uint32_t smsMode);

#endif /* TIM_H_INCLUDED*/
