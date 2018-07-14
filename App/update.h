#ifndef _update_h
#define _update_h
#include "stm32f1xx.h"
#define		BoardType			0x3a
#define		Boot1Ver			0x01

#define		JUMP_IAP			   0xAABBBBAA
#define		JUMP_AAP			   0xBBAAAABB

#pragma pack(push,1)
typedef struct
{
    u8   boardType;//设备类型
    u8   ver;//软件版本
    u16  crc16;//升级包校验
    u16  upDataLen;//升级包长度
    u8*  binFile;//一包升级包内容
}_Update;
#pragma pack(pop)
void UpdateAPP_succ(void);
void UpdateBOOT1_restart(void);
void update_function(u8 *data,u32 len);
void update_parameter(u8 *data,u32 len);
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength );
#endif

