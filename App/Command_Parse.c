#include "Command_Parse.h"
#include "usart1.h"
#include "fifo.h"
#include "CRC.h"
#include "gpio.h"
#include "update.h"

#define MAX_TRANSMIT_QUEUE_LENGTH 	1000
#define MAX_RECEIVE_QUEUE_LENGTH  	1000

#pragma pack(push,1)
typedef struct{
	u32 add;
	u8 cmd;
	u32 datalen;
	u8 data[1000];
//	u8 *data;
}_Cmd_Analysis;
#pragma pack(pop)

u8 TransmitQueue[MAX_TRANSMIT_QUEUE_LENGTH];
u8 ReceiveQueue[MAX_RECEIVE_QUEUE_LENGTH];
struct fifo_data TransmitFIFO;
struct fifo_data ReceiveFIFO;
_Cmd_data Cmd_data;
_Data Data;
_Cmd_Analysis Cmd_Analysis;
void Init_FIFO()
{
	fifo_Init(&ReceiveFIFO, ReceiveQueue, MAX_RECEIVE_QUEUE_LENGTH);
	fifo_Init(&TransmitFIFO, TransmitQueue, MAX_TRANSMIT_QUEUE_LENGTH);
}

void Receive_function()
{
	static u32 Length = 0;
	if (!fifo_empty(&ReceiveFIFO))
	{
		memset(Data.Buff,0,sizeof(Data.Buff));
		if (fifo_get_frame(&ReceiveFIFO,&Data.Buff[0],&Length))
		{
			if(true == Analysis(&Cmd_Analysis.add,&Cmd_Analysis.cmd,Cmd_Analysis.data,\
								&Cmd_Analysis.datalen,Data.Buff,Length))
			{
//				if(Msg(Data.Buff,Length) == false)
//					return;				

				if(Cmd_Analysis.cmd == 0xa0)//正常命令
				{
					USART1_Printf("Rcmd=%s,%d\r\n",Cmd_Analysis.data,Cmd_Analysis.datalen);
					//unpack(0x0a,0x00,Cmd_Analysis.data,Cmd_Analysis.datalen);
				}
				if(Cmd_Analysis.cmd == 0xb0)//APP升级包信息crc,len擦除app程序
				{
					update_parameter(Cmd_Analysis.data,Cmd_Analysis.datalen);
				}				
				if(Cmd_Analysis.cmd == 0xb1)//APP升级包内容
				{
					update_function(Cmd_Analysis.data,Cmd_Analysis.datalen);
				}
				if(Cmd_Analysis.cmd == 0xb2)//升级boot1改变升级boot1标志位,软件复位
				{
					UpdateBOOT1_restart();
				}
			}			
		}
	}
}
void Transport_function()
{
	static u32 Len,i;
	if (!fifo_empty(&TransmitFIFO))
	{
		memset(Data.Buff,0,sizeof(Data.Buff));
		if (fifo_get_frame(&TransmitFIFO,&Data.Buff[0], &Len))
        {
			#if 1
			DW_485_Send();        //设置为发送模式
			for(i=0;i<Len;i++)               //循环发送数据
			{
				while((USART1->SR&0X40)==0);  //等待发送结束             
				USART1->DR=Data.Buff[i];
			}    
			while((USART1->SR&0X40)==0);     //等待发送结束    
			DW_485_Receive();        //设置为接收模式 
			#else
//			for(i=0;i<Len;i++)
//			{
//				if(Data.Buff[i]==0x00)
//					Data.Buff[i] = 0x30;
//			}
//			USART1_Printf("%s",&Data.Buff[0]);
			#endif
        }
	}
}
/*
 *解析命令函数作用是带出指针变量
 */
bool Analysis(u32 *add,u8 *cmd,u8 *Data,u32 *DataLen,u8 *Content,u32 ContentLen)
{
	_Cmd_data *pPack;
	u16 CRCValue;
	u16 *pCRCValueRegion;
	
	if(ContentLen < MAX_PROTOCOL_EX_LEN)
	{
		return false;
	}
	pPack = (_Cmd_data *)Content;//结构化内容
	
	if( (HEAD != pPack->PackHead) || \
		(SOFT_VER != pPack->SoftVer)|| \
		(HARD_VER != pPack->HardType)|| \
		(TAIL != *((u8 *)((u8 *)pPack + PACK_TAIL_REGION_OFFSET \
					+  pPack->DataLength - DATALEN_FIX_LENGTH))))
	{
		return false;
	}
	
	CRCValue = ChkCrcValueEx(&(pPack->SoftVer), (pPack->DataLength),0xFFFF);
	pCRCValueRegion = (u16 *)((u8*)pPack + CRC_REGION_OFFSET \
								+(pPack->DataLength - DATALEN_FIX_LENGTH));
	
	if (*pCRCValueRegion != CRCValue)
	{
		return false;
	}	
	
	memcpy(Data,&(pPack->CmdData), pPack->DataLength - DATALEN_FIX_LENGTH);//拷贝内容带出
	*DataLen = pPack->DataLength - DATALEN_FIX_LENGTH;//带出内容长度
	*cmd = pPack->CmdCode;//带出命令
    *add = pPack->DesAddr;//带出地址
	
	return true;
}

/*
*用于打包程序要发送的数据
 */
bool unpack(u8 cmd,u32 targetadd,u8 *data,u32 datalen)
{
	_Cmd_data *pPack;
	u16 CRCValue;
	u16 *pCRCValueRegion;
	u8 *pPackTail;
	pPack = (_Cmd_data *)malloc(datalen + MAX_PROTOCOL_EX_LEN);//申请内存长度
	if(pPack == NULL)
	{
		return false;
	}

	pPack->PackHead = HEAD;
	pPack->DataLength = datalen + MAX_PROTOCOL_EX_LEN - 6;
	pPack->SoftVer = SOFT_VER;
	pPack->HardType = HARD_VER;
	pPack->CmdCode = cmd;
	pPack->SrcAddr = 0x12000000;//自身ID号
	pPack->DesAddr = targetadd;
	pPack->SerialNO = 0;
	pPack->TotalPackNum = 1;
	pPack->CurrentPackNO = 1;
	
	memcpy(&(pPack->CmdData), data, datalen);
	
	CRCValue = ChkCrcPack(&(pPack->SoftVer), (pPack->DataLength),0xFFFF);
	pCRCValueRegion = (u16 *)((u8 *)pPack + datalen + CRC_REGION_OFFSET); 
	*pCRCValueRegion = CRCValue;
	
	pPackTail = (u8 *)((u8 *)pPack + datalen + PACK_TAIL_REGION_OFFSET);
	*pPackTail = TAIL;	
	/*Add to Transmit FIFO*/   
    if (!fifo_puts(&TransmitFIFO, (u8 *)pPack, datalen + MAX_PROTOCOL_EX_LEN))
    {
        free(pPack);
        return false;
    }
	free(pPack);
	return true;	
}









//void Command_Parse(u8 *command_str)
//{
//	char *token,*segment_ptr[12],i;
//	// 清空命令段指针
//	for(i = 0; i < 12; i++)
//	{
//		segment_ptr[i] = NULL;
//	}
//	if(command_str[0] == 'W' || command_str[0] == 'R' || command_str[0] == 'C' \
//		|| command_str[0] == 'S' || command_str[0] == 'Q')
//	{
//		memcpy(Uart1.BackBuff,command_str,strlen((char *)command_str));
//		i = 0;
//		token = strtok((char *)command_str, ",=");
//		while(token != NULL && i < 12)
//		{
//			segment_ptr[i] = token;	
//			USART1_Printf("%s\r\n",segment_ptr[i]);
//			i++;
//			token = strtok(NULL,",=");			
//		}	
//		USART1_Printf("%s",(u8 *)Uart1.BackBuff);
//		memset((u8 *)Uart1.BackBuff,0,sizeof(Uart1.BackBuff));
//	}
//}


