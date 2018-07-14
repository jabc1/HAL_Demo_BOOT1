#ifndef _flash_h
#define _flash_h
#include "stm32f1xx.h" 

#define NVIC_VectTab_FLASH				0X08000000
#define Bootloader_flag_ADD				0X08006000//flag data
#define Flash_APP_ADDR					0x08006400//app start add


#define PROGRAM_UPGRADE_MODE      		0x55FFFF55 //upgrade mode，run in bootload 
#define PROGRAM_NORMAL_MODE      		0xAA0000AA //normal mode，run to app


/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/************************** STM32 内部 FLASH 配置 *****************************/
#define STM32_FLASH_SIZE        64  // 所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN        1    // stm32芯片内容FLASH 写入使能(0，禁用;1，使能)

/* 扩展变量 ------------------------------------------------------------------*/

/* 函数声明 ------------------------------------------------------------------*/
uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //读出半字

void STMFLASH_WriteLenByte(uint32_t WriteAddr, uint32_t DataToWrite, uint16_t Len );	      //指定地址开始写入指定长度的数据
uint32_t STMFLASH_ReadLenByte(uint32_t ReadAddr, uint16_t Len );					                    	//指定地址开始读取指定长度数据
void STMFLASH_Write( uint32_t WriteAddr, uint16_t * pBuffer, uint16_t NumToWrite );		//从指定地址开始写入指定长度的数据
void STMFLASH_Read( uint32_t ReadAddr, uint16_t * pBuffer, uint16_t NumToRead );   	//从指定地址开始读出指定长度的数据
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);
void  ProgramReady(uint16_t _lenth);

void CPU_FLASH_TEST(void);

#endif

