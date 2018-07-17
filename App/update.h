#ifndef _update_h
#define _update_h
#include "stm32f1xx.h"
#define		BoardType			0x0a
#define		Boot1Ver			0x01

#define		JUMP_IAP			   0xAABBBBAA
#define		JUMP_AAP			   0xBBAAAABB

#pragma pack(push,1)
typedef struct
{
	#if 0
    u8   boardType;//设备类型
    u8   ver;//软件版本
    u16  crc16;//升级包校验
    u16  upDataLen;//升级包长度
    u8*  binFile;//一包升级包内容
	#else
    u8 boardType;                        //板的类型
    u8 object;                           //0x01升级APP区标志 0x00 升级IAP标志 
//    u16 ver;                             //版本信息
    u16 crc16;                           //crc16校验
    u32 upDataLen;                       //升级包长度
    u16 noceUpDateLen;                  //单次更新的长度
    u32 updateAddr;                     //更新地址
    u8*  binFile;
    u8 upDataFalg_IAP;                //更新IAP程序标志
    u8 upDateFalg_APP;                //更新IAP程序标志	
	#endif
}_Update;
#pragma pack(pop)
void UpdateAPP_succ(void);
void UpdateBOOT1_restart(void);
void update_function(u8 *data,u32 len);
void update_parameter(u8 *data,u32 len);
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength );
#endif

