#ifndef _usart1_h
#define _usart1_h
#include "stm32f1xx_hal.h"
//#pragma pack(push, 1)
typedef struct{
	u8 RxBuff[1000];
//	u8 BackBuff[1024];
	u16 Len;
	bool over;
}_Uart1;
//#pragma pack(pop)
typedef enum{
	UART_RX_STATE_READY,
	UART_RX_STATE_START
}_uart1RxState;
extern _uart1RxState uart1RxState;
extern _Uart1 Uart1;
void MX_USART1_UART_Init(void);
void user_Uart1Handler(void); 
void Re_start_uart1_idle(void);
void Uart1_Init(void);
#endif

