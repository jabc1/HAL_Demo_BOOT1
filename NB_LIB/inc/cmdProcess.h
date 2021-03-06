/******************************Copyright(C) Nubo*******************************
File name  : cmdProcess.h
Description: process command 
Platform   : MDK V5.23.0.0
Version    : V1.0
Author     : Joey
Create Time: 2018-1-3
Modify     : 
Modify Time: 
******************************************************************************/

#ifndef __CMDPROCESS_H
#define __CMDPROCESS_H
#include "NbType.h"
#include "fifo.h"
#include "NbLibConfig.h"

/*******************************定义数据结构**********************************/
#pragma pack(1)
struct process_data
{
    u8 len;                 //sendBuf处理数据长度
    u8 flag;                //处理标志
    u8 con;                 //处理条件
    u8 mean;                //条件描述
//    u8 level;               //消息等级
    u8 data[CMD_SIZE];           //要处理的实际数据内容
    u8 (*process_msg)(u8 *buf, u32 len);
    u32(*get_msg)(u8 *cmdBuf, struct fifo_data *fifo);
};
#pragma pack()
typedef enum
{
    NORMAL = 0,             //正常模式
    MANY_P = 1,             //多次处理
}CMDcon_t;


typedef enum
{
    COMPLETE                = 0,    //操作正常完成
    FRAME_FORM_ERR          = 1,    //帧格式错误
    FRAME_LEN_ERR           = 2,    //帧长度错误
    FRAME_CHECKSUM_ERR      = 3,    //帧校验错误
    FRAME_REPEAT_ERR        = 4,    //帧重复
    CMD_NOTSUPPORT_ERR      = 5,    //命令不支持
    DATA_AREA_ERR           = 6,    //数据域无效
    DEVICE_BUSY_ERR         = 7,    //设备忙
	UPGRADE_VER_ERR         = 8,    //upgrade version is equal

	NO_CMD                  = 13,   //cmd is not existent
	NO_ACK                  = 14,   //no ack
	NO_EXE                  = 15,   //no execute

} MsgResult_t;                      //消息操作结果枚举量


struct attr_exe_data
{
    u32 cmd;                              //指令号
    MsgResult_t (*exe_attr)(u8*,u16,u8*,u8*); //exe函数入口                      
};

struct attr_exe_data_ex
{
    u32 cmd;                              //指令号
    MsgResult_t (*set_attr)(u8*,u16);         //set函数入口
    MsgResult_t (*get_attr)(u8*,u16,u8*,u8*); //get函数入口                      
};


/**********************************变量声明**********************************/
extern struct process_data uart1rcv_pd;
extern struct process_data uart1send_pd;

/**********************************函数声明**********************************/
void CMDProcessInit(void);
void Process_FIFOData(struct fifo_data *fifo, struct process_data *prc);
u8 Msg_Process(u8 *sdata, u32 len);
MsgResult_t Msg_Execute(u8 *sdata, u32 len);

extern u8 Send_Process(u8 *sdata, u32 len);

#endif
