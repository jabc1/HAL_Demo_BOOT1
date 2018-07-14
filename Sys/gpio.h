#ifndef	 _gpio_h
#define  _gpio_h
#include "stm32f1xx_hal.h"
#define 	DE_WD_PIN		GPIO_PIN_12		
#define		DE_WD_PROE		GPIOA
#define		DE_WD_CLK()		__HAL_RCC_GPIOA_CLK_ENABLE()

#define 	RE_WD_PIN		GPIO_PIN_11		
#define		RE_WD_PROE		GPIOA
#define		RE_WD_CLK()		__HAL_RCC_GPIOA_CLK_ENABLE()

void GPIO_init(void);
void DW_485_Receive(void);
void DW_485_Send(void);
#endif

