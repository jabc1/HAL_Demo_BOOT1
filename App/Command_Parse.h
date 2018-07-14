#ifndef _command_h
#define _command_h
#include "stm32f1xx.h"
#include "485_function.h"

#define		HEAD					0X7E//包头
#define 	TAIL 					0xE7//包未

#define 	SOFT_VER				0X01//软件版本
#define		HARD_VER				0X0A//设备类型


#define	 	MAX_PROTOCOL_EX_LEN	 	21			/*One pack's Max Length(exclude data region)*/
#define 	DATALEN_FIX_LENGTH   	15          /*Fixed length of data fields*/
#define 	CRC_REGION_OFFSET		(MAX_PROTOCOL_EX_LEN - 3)//crc位置
#define 	CRC_OFFSET           	2        //crc宽度
#define 	PACK_TAIL_REGION_OFFSET (MAX_PROTOCOL_EX_LEN - 1)//结尾

#pragma pack(push,1)
typedef struct{
	u8 PackHead;//头
	u16 DataLength;//数据长度
	u8 SoftVer;//软件版本
	u8 HardType;//设备类型
	u8 CmdCode;//命令类型
	u32 SrcAddr;//源地址
	u32 DesAddr;//目标地址
	u16 SerialNO;//序列号
	u8 TotalPackNum;//总报数
	u8 CurrentPackNO;//当前报数
	u8 CmdData[1000];//内容
//	u8 * CmdData;
}_Cmd_data;
typedef struct
{
//	u8 GetBuff[1000];
	u8 Buff[1000];
}_Data;
#pragma pack(pop)
extern _Cmd_data Cmd_data;
extern _Data Data;
extern struct fifo_data TransmitFIFO;
extern struct fifo_data ReceiveFIFO;
void Init_FIFO(void);
void Receive_function(void);
void Transport_function(void);
bool Analysis(u32 *add,u8 *cmd,u8 *Data,u32 *DataLen,u8 *Content,u32 ContentLen);
bool unpack(u8 cmd,u32 targetadd,u8 *data,u32 datalen);
//void Command_Parse(u8 *command_str);


#endif



