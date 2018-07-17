#include "update.h"
#include "crc.h"
#include "flash.h"

#pragma pack(push,1)
typedef struct
{
	u32 JumpFlag;				//Bootloader_0程序跳转标记
	u32 bootloader_Version;		//Bootloader版本
	u8 bootloader0_Flag;		//bootloader1软件版本
	u8 bootloader1_Flag;		//bootloader1软件版本大小
}_UpdateF;
#pragma pack(pop)
_UpdateF UpdateF;

#pragma pack(push,2)
typedef struct
{
	u16 update_crc;
	u32 update_len;
	u16 segnum;
}_Update_info;
#pragma pack(pop)
_Update_info Update_info;


void update_function(u8 *data,u32 len)
{
	static u16 count=0;
	u16 crc1=0;
	uint32_t update_addr;
	uint8_t *paddr;

	count++;
	if(count <= ((Update_info.segnum) - 1))//第一包到倒数第二包
	{
		update_addr = Flash_APP_ADDR + (count-1)*len;			  
		IAP_Write_App_Bin(update_addr,data,len);
	}
	else if(count == Update_info.segnum)//最后一包
	{
		update_addr = Flash_APP_ADDR + (count-1)*960;			  
		IAP_Write_App_Bin(update_addr,data,960);
		paddr = (uint8_t *)(Flash_APP_ADDR);
		crc1 = ChkCrcValueEx(paddr,Update_info.update_len,0xFFFF);
		if(Update_info.update_crc == crc1) //check正确
		{
			//更新flash标志并软件复位
			UpdateAPP_succ();//改变标志位软件复位跳转到app部分
		}
		else
		{
			count = 0;//升级失败，变量清零重新升级
		}
	}
	
}
void UpdateAPP_succ(void)
{
	UpdateF.JumpFlag = JUMP_AAP;//标志跳转到APP程序
//	UpdateF.boot1_Version = Boot1Ver;
//	UpdateF.boot1_Flag = 0;
//	UpdateF.bootloader0Flag = 0;
	STMFLASH_Write(Bootloader_flag_ADD, (uint16_t *)&UpdateF,2);
	NVIC_SystemReset();// 复位
}
void UpdateBOOT1_restart()
{
	UpdateF.JumpFlag = JUMP_AAP;//更改标志位软件复位跳入APP准备升级boot1
	UpdateF.bootloader_Version = Boot1Ver;
//	UpdateF.bootloader1Flag = 0;
//	UpdateF.bootloader0Flag = 0;
	STMFLASH_Write(Bootloader_flag_ADD, (uint16_t *)&UpdateF,4);
	NVIC_SystemReset();// 复位
}
void update_parameter(u8 *data,u32 len)//第一包加载前16个字节
{
	_Update *pData;
	pData = (_Update *)data;
	if(pData->boardType == BoardType)
	{
		Update_info.update_crc = pData->crc16;
		Update_info.update_len = pData->upDataLen;
		Update_info.segnum = Update_info.update_len % 960 \
							  ? (Update_info.update_len/960) + 1 \
							  : (Update_info.update_len/960); //拆分512个字节，记录总的升级包数量
		ProgramReady(Update_info.update_len);//擦除APP flash
	}
}
static u16 ulBufFlashApp[1024];
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength )
{
	uint16_t us, usCtr=0, usTemp;
	uint32_t ulAdd_Write = ulStartAddr;                                //当前写入的地址
	uint8_t * pData = pBin_DataBuf;
	
	for ( us = 0; us < ulBufLength; us += 2 )
	{						    
		usTemp =  ( uint16_t ) pData[1]<<8;
		usTemp += ( uint16_t ) pData[0];	  
		pData += 2;                                                      //偏移2个字节
		ulBufFlashApp [ usCtr ++ ] = usTemp;	    
		if ( usCtr == 1024 )
		{
			usCtr = 0;
			STMFLASH_Write ( ulAdd_Write, ulBufFlashApp, 1024 );	
			ulAdd_Write += 2048;                                           //偏移2048  16=2*8.所以要乘以2.
		}
	}
	if ( usCtr ) 
    STMFLASH_Write ( ulAdd_Write, ulBufFlashApp, usCtr );//将最后的一些内容字节写进去.  
}

