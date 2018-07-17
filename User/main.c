/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"   
#include "stm32f1xx_hal.h"
#include "Sys.h"
#include "gpio.h"
#include "usart1.h"
#include "flash.h"
#include "485_function.h"
#include "Command_Parse.h"

int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000400);
	HAL_Init();
	SystemClock_Config();
	GPIO_init();
	MX_USART1_UART_Init();
	Init_FIFO();
	USART1_Printf("BOOT1 start!\r\n");
	while(1)
	{
		if(Uart1.over == true)
		{
			Uart1.over = false;
//			USART1_Printf("%s\r\n",Uart1.RxBuff);
//			memset(Uart1.RxBuff,0,sizeof(Uart1.RxBuff));
			Re_start_uart1_idle();
		}
		Receive_function();
		Transport_function();
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
