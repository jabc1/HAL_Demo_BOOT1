#include "485_function.h"
#include <stdarg.h>
#include "gpio.h"
void USART1_Printf(const char *fmt, ...)
{
	char SendBuff[256];
	va_list ap;      
	va_start(ap, fmt);  
	vsprintf(SendBuff, fmt, ap);
	va_end(ap);
	SendBuff[sizeof(SendBuff) - 1] = '\0';
	RS485_Send_Data(SendBuff,strlen(SendBuff));
}
void RS485_Send_Data(char *buf,unsigned char len)
{
	u8 t;
	DW_485_Send();        //设置为发送模式
	for(t=0;t<len;t++)               //循环发送数据
	{
		while((USART1->SR&0X40)==0);  //等待发送结束             
		USART1->DR=buf[t];
	}    
	while((USART1->SR&0X40)==0);     //等待发送结束    
	DW_485_Receive();        //设置为接收模式 	

}



