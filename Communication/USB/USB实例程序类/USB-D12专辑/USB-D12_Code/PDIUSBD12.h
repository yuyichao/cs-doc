/******************************************************************
   本程序只供学习使用，未经作者许可，不得用于其它任何用途
      我的邮箱：computer-lov@tom.com
        欢迎访问我的blog：  http://computer00.21ic.org

pdiusbd12.h  file

Created by Computer-lov
Date: 2004.7.13

Edit date:2006.3.2

Version V1.1
Copyright(C) Computer-lov 2004-2014
All rigths reserved
             
*******************************************************************/


#ifndef __PDIUSBD12_H__
#define __PDIUSBD12_H__


//PDIUSBD12.h
//PDIUSBD12的命令和数据定义


//*************************************************
//端点号定义
#define CONTROL_POINT_OUT        0x00
#define CONTROL_POINT_IN         0x80
#define POINT_1_OUT              0x01
#define POINT_1_IN               0x81
#define MAIN_POINT_OUT           0x02
#define MAIN_POINT_IN            0x82
//*************************************************
//*************************************************
//初始化命令
#define Set_Address              0xD0
#define Set_Endpoint_Enable      0xD8
#define Set_Mode                 0xF3
#define Set_DMA                  0xFB
//*************************************************


//*************************************************
//数据流命令
#define Read_Interrupt_Register  0xF4
#define Select_EndPoint          0X00
#define Select_Endpoint_C_OUT    0X00
#define Select_Endpoint_C_IN     0X01
#define Select_Endpoint_1_OUT    0X02
#define Select_Endpoint_1_IN     0X03
#define Select_Endpoint_2_OUT    0X04
#define Select_Endpoint_2_IN     0X05
#define Read_Last_Status         0X40
#define Read_Last_Status_C_OUT   0X40
#define Read_Last_Status_C_IN    0X41
#define Read_Last_Status_1_OUT   0X42
#define Read_Last_Status_1_IN    0X43
#define Read_Last_Status_2_OUT   0X44
#define Read_Last_Status_2_IN    0X45
#define Read_Buffer              0XF0
#define Write_Buffer              0XF0
#define Set_Endpoint_Status_C_OUT 0X40
#define Set_Endpoint_Status_C_IN  0X41
#define Set_Endpoint_Status_1_OUT 0X42
#define Set_Endpoint_Status_1_IN  0X43
#define Set_Endpoint_Status_2_OUT 0X44
#define Set_Endpoint_Status_2_IN  0X45
#define Ack_Setup                 0XF1
#define Clear_Buffer              0XF2
#define Validate_Buffer           0XFA
//*************************************************

//*************************************************
//普通命令
#define Send_Resume               0XF6
#define Read_Current_Frame_Number 0XF5
//*************************************************

//*************************************************
//一些数据的定义
#define Endpoint_Enable           0X01
#define Endpoint_Disenable        0X00

#define Mode0_and_no_connect      0X04
#define Mode1_and_no_connect      0X44
#define Mode2_and_no_connect      0X84
#define Mode3_and_no_connect      0XC4
#define Mode0_and_connect         0X14
#define Mode1_and_connect         0X54
#define Mode2_and_connect         0X94
#define Mode3_and_connect         0XD4
#define Mode_Set_secend_byte      0X8B



#endif
