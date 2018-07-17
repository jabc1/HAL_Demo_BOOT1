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
    u8   boardType;//�豸����
    u8   ver;//����汾
    u16  crc16;//������У��
    u16  upDataLen;//����������
    u8*  binFile;//һ������������
	#else
    u8 boardType;                        //�������
    u8 object;                           //0x01����APP����־ 0x00 ����IAP��־ 
//    u16 ver;                             //�汾��Ϣ
    u16 crc16;                           //crc16У��
    u32 upDataLen;                       //����������
    u16 noceUpDateLen;                  //���θ��µĳ���
    u32 updateAddr;                     //���µ�ַ
    u8*  binFile;
    u8 upDataFalg_IAP;                //����IAP�����־
    u8 upDateFalg_APP;                //����IAP�����־	
	#endif
}_Update;
#pragma pack(pop)
void UpdateAPP_succ(void);
void UpdateBOOT1_restart(void);
void update_function(u8 *data,u32 len);
void update_parameter(u8 *data,u32 len);
void IAP_Write_App_Bin ( uint32_t ulStartAddr, uint8_t * pBin_DataBuf, uint32_t ulBufLength );
#endif

