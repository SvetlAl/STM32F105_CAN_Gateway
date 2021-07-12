//*****************************************************************
// 
// 
// 
// 
// Alex Svetlichnyy 2019
//*****************************************************************
#ifndef WATCHDOG_H_INCLUDED
#define WATCHDOG_H_INCLUDED

#include "stm32f10x.h"


void Init_IWDG(u16 tw);
void IWDG_reset(void);



#endif /* WATCHDOG_H_INCLUDED */
