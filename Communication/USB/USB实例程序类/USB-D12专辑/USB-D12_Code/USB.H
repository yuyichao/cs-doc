/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
      我的邮箱：computer-lov@tom.com
        欢迎访问我的blog：  http://computer00.21ic.org

USB.H  file

Created by Computer-lov
Date: 2004.7.13

Edit date:2006.3.2

Version V1.1
Copyright(C) Computer-lov 2004-2014
All rigths reserved
             
*******************************************************************/

#ifndef __USB_H__
#define __USB_H__

#define BYTE unsigned char
#define WORD unsigned int

// 定义标准的描述符类型，也就是USB分配的各种描述符的编号，即bDescriptorType 字段的值

#define DEVICE_DESCRIPTOR               0x01  //设备描述符
#define CONFIGURATION_DESCRIPTOR        0x02  //配置描述符
#define STRING_DESCRIPTOR               0x03  //字符串描述符
#define INTERFACE_DESCRIPTOR            0x04  //接口描述符
#define ENDPOINT_DESCRIPTOR             0x05  //端点描述符


//定义标准的USB请求命令，即USB分配的各种请求的编号，即bRequest字段的值

#define GET_STATUS                      0x00  //获取状态请求
#define CLEAR_FEATURE                   0x01  //清除特性请求
#define SET_FEATURE                     0x03  //设置特性请求
#define SET_ADDRESS                     0x05  //设置地址请求
#define GET_DESCRIPTOR                  0x06  //获取描述符请求
#define SET_DESCRIPTOR                  0x07  //设置描述符请求
#define GET_CONFIGURATION               0x08  //获取配置请求
#define SET_CONFIGURATION               0x09  //设置配置请求
#define GET_INTERFACE                   0x0A  //获取接口请求
#define SET_INTERFACE                   0x0B  //设置接口请求
#define SYNCH_FRAME                     0x0C  //同步帧请求


//定义特性选择器

#define ENDPOINT_HALT                   0x00  //端点的禁止特性
#define DEVICE_REMOTE_WAKEUP            0x01  //设备的远程唤醒特性


//使用的端点数

//#define Number_Endpoint               0x05  //端点数量为5


//定义的端点类型
#define ENDPOINT_TYPE_CONTROL           0x00  //控制传输
#define ENDPOINT_TYPE_ISOCHRONOUS       0x01  //同步传输
#define ENDPOINT_TYPE_BULK              0x02  //批量传输
#define ENDPOINT_TYPE_INTERRUPT         0x03  //中断传输



//定义标准的设备描述符结构

typedef struct _DEVICE_DCESCRIPTOR_STRUCT
{
 BYTE blength;                               //设备描述符的字节数大小
 BYTE bDescriptorType;	                      //设备描述符类型编号
 WORD bcdUSB;                                //USB版本号
 BYTE bDeviceClass;                          //USB分配的设备类代码
 BYTE bDeviceSubClass;                       //USB分配的子类代码
 BYTE bDeviceProtocol;                       //USB分配的设备协议代码
 BYTE bMaxPacketSize0;                       //端点0的最大包大小
 WORD idVendor;                              //厂商编号
 WORD idProduct;                             //产品编号
 WORD bcdDevice;                             //设备出厂编号
 BYTE iManufacturer;                         //设备厂商字符串的索引
 BYTE iProduct;                              //描述产品字符串的索引
 BYTE iSerialNumber;                         //描述设备序列号字符串的索引
 BYTE bNumConfigurations;                    //可能的配置数量
}
DEVICE_DESCRIPTOR_STRUCT, * pDEVICE_DESCRIPTOR_STRUCT;


//定义标准的配置描述符结构

typedef struct _CONFIGURATION_DESCRIPTOR_STRUCT
{
 BYTE bLength;                               //配置描述符的字节数大小
 BYTE bDescriptorType;                       //配置描述符类型编号
 WORD wTotalLength;                          //此配置返回的所有数据大小
 BYTE bNumInterfaces;                        //此配置所支持的接口数量
 BYTE bConfigurationValue;                   //Set_Configuration命令所需要的参数值
 BYTE iConfiguration;                        //描述该配置的字符串的索引值
 BYTE bmAttributes;                          //供电模式的选择
 BYTE MaxPower;                              //设备从总线提取的最大电流
}
CONFIGURATION_DESCRIPTOR_STRUCT, * pCONFIGURATION_DESCRIPTOR_STRUCT;


//定义标准的接口描述符结构

typedef struct _INTERFACE_DESCRIPTOR_STRUCT
{
 BYTE bLength;                               //接口描述符的字节数大小
 BYTE bDescriptorType;                       //接口描述符的类型编号
 BYTE bInterfaceNumber;                      //该接口的编号
 BYTE bAlternateSetting;                     //备用的接口描述符编号
 BYTE bNumEndpoints;                         //该接口使用的端点数，不包括端点0
 BYTE bInterfaceClass;                       //接口类型
 BYTE bInterfaceSubClass;                    //接口子类型
 BYTE bInterfaceProtocol;                    //接口遵循的协议
 BYTE iInterface;                            //描述该接口的字符串索引值
}
INTERFACE_DESCRIPTOR_STRUCT, * pINTERFACE_DESCRIPTOR_STRUCT;


//定义标准的端点描述符结构

typedef struct _ENDPOINT_DESCRIPTOR_STRUCT
{
 BYTE bLegth;                                //端点描述符字节数大小
 BYTE bDescriptorType;                       //端点描述符类型编号
 BYTE bEndpointAddress;                      //端点地址及输入输出属性
 BYTE bmAttributes;                          //端点的传输类型属性
 WORD wMaxPacketSize;                        //端点收、发的最大包大小
 BYTE bInterval;                             //主机查询端点的时间间隔
}
ENDPOINT_DESCRIPTOR_STRUCT, * pENDPOINT_DESCRIPTOR_STRUCT;


//一个字符串描述符结构

typedef struct _STRING_DESCRIPTOR_STRUCT
{
 BYTE bLength;                               //字符串描述符的字节数大小
 BYTE bDescriptorType;                        //字符串描述符类型编号
 BYTE SomeDesriptor[36];                     //UNICODE编码的字符串
}
STRING_DESCRIPTOR_STRUCT, * pSTRING_DESCRIPTOR_STRUCT;


//标准设备请求命令结构

typedef struct _REQUESTCMD
{
 BYTE bmRequestType;                         //请求命令类型
 BYTE bRequest;                              //请求命令代码
 WORD wValue;                                //命令信息
 WORD wIndex;                                //索引信息
 WORD wLength;                               //传输数据大小
}
REQUESTCMD, * pREQUESTCMD;
   

//setup_packet缓冲区

#define MAX_CONTROL_DATA_SIZE 16

typedef struct _control_data_buff
{
 REQUESTCMD DeviceRequest;                    //USB设备请求
 unsigned int wLength;
 unsigned int wCount;
 unsigned char * pData;
 unsigned char dataBuffer[MAX_CONTROL_DATA_SIZE];
}CONTROL_DATA_BUFF;
 

#endif
