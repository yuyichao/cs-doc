/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
      我的邮箱：computer-lov@tom.com
        欢迎访问我的blog：  http://computer00.21ic.org

mass_storage.h  file

Created by Computer-lov
Date: 2004.7.13

Edit date:2006.3.2

Version V1.1
Copyright(C) Computer-lov 2004-2014
All rigths reserved
             
*******************************************************************/


#ifndef __MASS_STORAGE_H__
#define __MASS_STORAGE_H__


//定义了USB Mass Storage 类的一些命令及数据结构

#define SUCCESS 0x00
#define FAIL    0x01
#define ERROR   0x02
//UFI子类命令
#define Format_Unit                 0x04         //格式化单元
#define Inquiry                     0x12         //查询
#define Star_Stop                   0x1B         //启动/停止
#define Mode_Select                 0x55         //模式选择
#define Mode_Sense                  0x5A         //模式判别
#define Medium_Removal              0x1E         //禁止/允许介质移除
#define Read_10                     0x28         //读10
#define Read_12                     0xA8         //读12
#define Read_Capacity               0x25         //读容量
#define Read_Format_capacity        0x23         //读格式化容量
#define Request_Sense               0x03         //请求判断
#define Rezero_Unit                 0x01         //归0
#define Seek_10                     0x2B         //寻道10
#define Send_Diagnostic             0x1D         //发送诊断
#define Test_Unit_Ready             0x00         //测试单元准备
#define Verify                      0x2F         //验证
#define Write_10                    0x2A         //写10
#define Write_12                    0xAA         //写12
#define Write_and_Verify            0x2E         //写并验证


//定义命令块封包(CBW)
typedef struct _CBW
{
 unsigned long dCBWSignature;                    //CBW标志
 unsigned long dCBWTag;                          //命令块标签
 unsigned long int dCBWDataTransgerLength;        //数据包大小
 unsigned char bmCBWFlags;                       //D7位表示数据方向，0=out，1=in，其它位保留为0
 unsigned char bCBWLUN;                          //接收该命令的逻辑单元号LUN
 unsigned char bCBWCBLength;                     //表示CBWCB的长度
 unsigned char CBWCB[16];                        //子类命令
}
CBW, * pCBW;

//定义命令状态封包(CSW)
typedef struct _CSW
{
 unsigned long dCSWSignature;                    //CSW的标志
 unsigned long dCSWTag;                          //命令状态标志
 unsigned long dCSWDataResidue;                  //表示dCBWDataTransferLenhth字段中主机希望的数据长度与实际发送的数据长度的差额
 unsigned char bCSWStatus;                       //该命令执行的情况       
}
CSW, * pCSW;


#endif
