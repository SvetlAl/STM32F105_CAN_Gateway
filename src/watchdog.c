#include "watchdog.h"


void Init_IWDG(u16 tw){
// Для IWDG_PR=7 Tmin=6,4мс RLR=Tмс*40/256
	IWDG->KR=0x5555; // Ключ для доступа к таймеру
	IWDG->PR=7; // Обновление IWDG_PR
	IWDG->RLR=tw*40/256; // Загрузить регистр перезагрузки
	IWDG->KR=0xAAAA; // Перезагрузка
	IWDG->KR=0xCCCC; // Пуск таймера
	}

	
// Функция перезагрузки сторожевого таймера IWDG
void IWDG_reset(void){
	IWDG->KR=0xAAAA; // Перезагрузка
	}
