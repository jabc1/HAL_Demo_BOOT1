#include "gpio.h"
#include "stm32f1xx_hal.h"

void GPIO_init()
{
	GPIO_InitTypeDef   GPIO_InitStruct;
	DE_WD_CLK();	
	RE_WD_CLK();
	
	HAL_GPIO_WritePin(DE_WD_PROE, DE_WD_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RE_WD_PROE, RE_WD_PIN, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = DE_WD_PIN|RE_WD_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DE_WD_PROE, &GPIO_InitStruct);	
}
void DW_485_Receive()
{
	HAL_GPIO_WritePin(DE_WD_PROE, DE_WD_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RE_WD_PROE, RE_WD_PIN, GPIO_PIN_RESET);
}
void DW_485_Send()
{
	HAL_GPIO_WritePin(DE_WD_PROE, DE_WD_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RE_WD_PROE, RE_WD_PIN, GPIO_PIN_SET);
}

