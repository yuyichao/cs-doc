/************************************************************************************************
*                           
*                                 USB118 C51测试程序
*                                                               Version 1.1
*                西安达泰电子有限责任公司（Xi'an DATA Electronic Co,Ltd）      
*   网址： http://www.dataie.com 
*   电话： 029-85272421 , 88022900
*  
*                                             西安达泰电子有限责任公司        2008.05
*************************************************************************************************/


/************************************************************************************************
*
*  U盘文件读写模块 以字节为单位进行U盘文件读写。
*  MCS-51单片机C语言示例程序 
*  硬件连接方式: 4线制串口（电源，地，输入，输出） 
*
*************************************************************************************************/

#include <AT89X52.h>

#define uchar  unsigned  char
#define uint   unsigned  int 
	
uchar  commd[66]={0x01,0x02,0x43,0x44,0x45,0x46}; //send and receive command buffer
uchar  idata dat1[32]={0x61,0x62,0x63,0x64,0x65,0x66}; //send  and receive databuffer
long int sm; 
uchar Command_status=0;	  //receive status 

/*---------------------------------
函数功能；send data 
入口参数：
---------------------------------*/
void int_data1(uchar da)
{
 uchar dat;
 for(dat=0; dat<32; dat+=2)
   {
    dat1[dat]=da+dat;
   }
}
 /*
void int_data2(uchar da)
{
 uchar dat;
 for(dat=1; dat<32; dat+=2)
  {
   dat1[dat]=da;
  }
}
/*-------------------------------------------
            数组初始化函数
函数功能：初始化函数
入口参数：ia是要给commd[]数组初始化值
出口参数：
-------------------------------------------*/
void intia(uchar ia)
{
  uchar t;
  for(t=0; t<66; t++)
   {
    commd[t]=ia;
   }
}


/*--------------------------------------------------
               延时函数  延时sec*01ms
函数功能：延时 function ,when frenquency 24M , 0.1ms
入口参数：c 是延时多少个0.1ms
-----------------------------------------------------*
void delay(uint sec) 
{
 uint i;
 uchar j;
 for(i=0; i<sec; i++)
   for(j=0; j<100; j++)
  	;	 	   
}

 /*------------------------------------------------------
               延时函数  延时=sec*15.1ms
函数功能：延时 function ,when frenquency 24M , 15.1ms
入口参数：c 是延时多少个15.1ms
--------------------------------------------------------*/
void Del(int sec) 
{
 uint i;
 uchar j, b; 
 for(i=0; i<sec; i++)
   for(j=0; j<100; j++)
     for(b=0; b<99; b++)
	   ;	   
}

/*----------------------------------------
函数功能：initialize serial function

-------------------------------------------
void serial_int()
{
  EA=0;		 //总中断禁止
  ES=0;		 //serial interrupt forbid
  TMOD=0x20; //定时器T1使用工作方式2
  TH1=0xF3;	//设置初值
  TL1=0xF3;
  TR1=1;
  PCON=0x80;	//SMOD=1
  SCON=0x50;	//工作方式1，9600bit/s 
  TI=1;
  REN=1;
}
*/

/*---------------------------------------------------
                串行通讯口初始化函数 
函数功能：初始化uart   
入口参数：波特率（9600， 19200， 57600）
----------------------------------------------------*/ 
void Init_COMM(uint sp)
{
	     SCON = 0x53;
	     PCON = 0x80 | PCON;
         T2CON=0x30;
	     switch(sp)
	     {
	       case 9600:{// 9600 @ 24MHz: 24000000/(32*(65536-(RCAP2H,RCAP2L)))
                        RCAP2H=0xFF;		
	                    RCAP2L=0xB2;
					  }break;
		   case 19200:{// 19200 @ 24MHz: 24000000/(32*(65536-(RCAP2H,RCAP2L)))
			            RCAP2H=0xFF;		
	                    RCAP2L=0xD9;
					  }break;
           case 57600:{// 57600 @ 24MHz: 24000000/(32*(65536-(RCAP2H,RCAP2L)))
			            RCAP2H=0xFF;		
	                    RCAP2L=0xF3;
					  }break;
          }
          TI=1;
	      RI=0;
	      TR2=1;
		  
}
/*-----------------------------------------------------
                URAT命令格式0xaa+0xbb+command+data
函数功能：是发送同步码0xaa+0xbb
参数说明：syn is send string 
-------------------------------------------------------*/
void serial_syn(  )
{
  uchar i;
  uchar syn;
  i=0;
  syn=0xaa;
  TI=0;
  while(i<2)
  { 
    SBUF=syn;
    while(!TI);
	TI=0;		
	syn=0xbb;
	i++;   
  }
}

/*--------------------------------------------------------
                   串口接收一个字节数据
函数功能：接收一个字节数据
---------------------------------------------------------*/
void Send_Data(uchar send)
{
   
   SBUF=send;
   while(!TI);   
   TI=0;	              			            
}
/*---------------------------------------------------------------------------------------------------
                     串口发送函数command(64)+data(0-2048)
函数功能: serial send data
入口参数: *s_command 是命令指针，按协议要求发送64个字节为一帧，*s_dat是数据指针，
          s_num是要发送的数据字节数（0-2048）
 ----------------------------------------------------------------------------------------------------*/
void serial_send(uchar *s_command,  uchar *s_dat, uint s_num)
{
  uint s;
  for(s=0; s<64; s++)   	 
   {
     //SBUF=*s_command;
	// while(!TI);
   	// TI=0;
	 Send_Data(*s_command);
	 s_command++;  
    }
   for(s=0; s<s_num; s++)
	{
 	 //SBUF=*s_dat;
	 //while(!TI);
   	 //TI=0;
	 Send_Data(*s_dat);
	 s_dat++; 	
   	}
   TI=0;
   RI=0;

   EA=1;
   ES=1;    
}

/*--------------------------------------------------------
                   串口接收一个字节数据
函数功能：接收一个字节数据
---------------------------------------------------------*/
unsigned char Receive_Data()
{
         uchar receive;
		 while(!RI);
		 receive=SBUF;
		 RI=0;	              			  
         return (receive);           
}
/*-------------------------------------------------------------------------------------------------
                    串口接收一帧数据：数据格式 command(64)+data(0-16384)
函数功能：接收一帧数据
入口参数：*r_command是接收响应命令帧数据指针，*r_dat是接收数据指针，r_num是在命令指针后的数据个数。
出口参数：return 0 表示错误命令, 1 表示接收一帧完成 
---------------------------------------------------------------------------------------------------*/
unsigned char Receive_One(uchar *r_command,  uchar *r_dat)
{
         
  uchar i;
  uint r_num;
  r_num=0;
  for(i=0; i<3; i++)
   {
    *r_command=Receive_Data();
	 r_command++;
   }
  if(commd[0]==0xaa)	 //如果接收到0xaa+0xbb+0x01 错误信息格式是： 0xaa + 0xbb +0x01
	  return 0;
  for(i=3;i<63;i++)      //不是错误信息时，继续接收完一帧（64byte）    
   {
    *r_command=Receive_Data();
   	r_command++;
    }
 		 																		 
  if((commd[0]==0xB0))	 //如果是read 命令则有数据返回
    {
    	r_num=commd[6];	 //data for r_num BYTE    
		r_num<<=8;
		r_num|=commd[5];
     }
   if(commd[0]==0x20) 	 //如果是 list 命令则有数据返回
	 {
       r_num=commd[5];	 //data for r_num BYTE    
       r_num<<=8;
	   r_num|=commd[4];
	  }    		   
   for(i=0;i<r_num;i++)    //receive data,   r_num(0-16384)
     {
      *(r_dat+i)=Receive_Data();
     }		 
    return 1;
}
/*----------------------------------------------
函数功能：serial receive  data
参数说明：同serial_send()
------------------------------------------------*
unsigned char serial_rec(unsigned char *r_command,  unsigned char *r_dat,  unsigned int r_num)
{
 unsigned int r, receive_data;
 //serial_int();
 
 for(r=0; r<3; r++)
   {
    while(!RI);
	receive_data=SBUF;
	RI=0;
    *r_command=receive_data;		 		 		 
	r_command++;	
   	  
   }
   delay(10);
  if(commd[0]==0xaa)
      return 0;

 for(r=3; r<64; r++)	 
   {
	while(!RI);
	receive_data=SBUF;
	RI=0;
	*r_command=receive_data;		 		 		 
	r_command++;
	
   	  
   }

 for(r=0; r<r_num; r++)
   {
	while(!RI);
	receive_data=SBUF;
	RI=0; 
	*r_dat=receive_data;
    r_dat++;
	
   	 
   }
    return 1;	 
  
}
 
/*-------------------------------------------------------------------------------------------
            检测设备是否正常
函数功能: USB118 detect u
出口参数: USB118T_Detect commd[0]= 0x04, commd[1] 是检测结果 0x01有磁盘，0x00没有磁盘。 
------------------------------------------------------------------------------------------- */
void USB118_Detect()
{
  intia(0x20);		//intialize	command data group

  commd[0]=0x01;	 //USB118R_Detect command 0x01
  commd[1]=0x00;  
    
  serial_syn();
  serial_send(commd, 0, 0);
}

/*-----------------------------------------------------------------------------------------------------------------------
		  创建文件或者目录
函数功能：create file or makedir
入口参数：tpye is 0x07(file) or 0x32(makedir),na1--an8 feil name, tp1--tp3 feil tpye
出口参数：USB118_Create  ,Create 返回command[0]=0x0A, command [1]  01H表示成功，00H表示失败，command[2]-[3]表示错误码
      and USB118_MakeDir ,MakeDir返回command[0]=0x42，command [1]  01H表示成功，00H表示失败，command[2]-[3]表示错误码 
----------------------------------------------------------------------------------------------------------------------- */
void USB118_Crefiledir( uchar tpye,  uchar na1, uchar na2, uchar na3, uchar na4, 
                         uchar na5,  uchar na6, uchar na7, uchar na8, uchar tp1,
					   	 uchar tp2,  uchar tp3 )
{
 intia(0);
 commd[0]=tpye;		//create file(07H) or dir(32H)
 commd[1]=na1;
 commd[2]=na2;
 commd[3]=na3;
 commd[4]=na4;
 commd[5]=na5;
 commd[6]=na6;
 commd[7]=na7;
 commd[8]=na8;

 commd[9]=tp1;
 commd[10]=tp2;
 commd[11]=tp3;

 commd[12]=0x20;	//create file attribute
 commd[23]=0x38;    //create file time
 commd[24]=0x21;
 commd[33]=0;

 serial_syn();
 serial_send(commd, 0, 0);  
 //serial_rec(commd, 0, 0);
}

/*------------------------------------------------------------------
函数功能： open file
入口参数：The na1-8 is file name, the tp1-3 is file type.
出口参数：USB118_Open  ,Open 返回command[0]=0x0D, command [1]  01H表示成功，00H表示失败，
          command[2]-[3]表示错误码 
------------------------------------------------------------------*/
void  USB118_Open(uchar na1, uchar na2, uchar na3, uchar na4,uchar na5, uchar na6, uchar na7, uchar na8, 
                  uchar tp1, uchar tp2, uchar tp3)
					   	   
{
 intia(0x20);
 commd[0]=0x06;	 //open file 
 commd[1]=na1;
 commd[2]=na2;
 commd[3]=na3;
 commd[4]=na4;
 commd[5]=na5;
 commd[6]=na6;
 commd[7]=na7;
 commd[8]=na8;

 commd[9]=tp1;
 commd[10]=tp2;
 commd[11]=tp3;

 serial_syn();
 serial_send(commd, 0, 0);
 //serial_rec(commd, 0, 0);	
}

/*-----------------------------------------------------------------------------
             写入数据
函数功能：when open  or create file, write data
入口参数：*wdata 将要写入数据组的指针头, wnum是写入数据的长度(0--2048)
-------------------------------------------------------------------------------*/		 
void USB118_Write(uchar *wdata, uint wnum)
{
  
  uchar n;
  uint data_num;
  intia(0);
  data_num=wnum;
  n=wnum&0xff;
  wnum>>=8;
  
  commd[0]=0x09;	 //write data
  commd[1]=0;
  commd[2]=n;
  commd[3]=(uchar)wnum;
  				    
  serial_syn();
  serial_send(commd, wdata, data_num);

  //serial_rec(commd, 0, 0);
}  	  
/*-----------------------------------------------
函数功能:进入子目录
入口参数:na1-8是要进入的目录名
------------------------------------------------*
void USB118_InDir(uchar na1, uchar na2, uchar na3, uchar na4,uchar na5, uchar na6, uchar na7, uchar na8)
{
 intia(0x20);

 commd[0]=0x33;		   //in dir
 commd[1]=na1;
 commd[2]=na2;
 commd[3]=na3;
 commd[4]=na4;
 commd[5]=na5;
 commd[6]=na6;
 commd[7]=na7;
 commd[8]=na8;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*------------------------------       
函数功能:返回当前目录的上一层
-------------------------------*
void USB118_OutDir()
{
 intia(0x20);
 
 commd[0]=0x34;	  // out dir

 serial_syn();
 serial_send(commd, 0, 0);
}

/*------------------------------------
函数功能:返回根目录
--------------------------------------*
void USB118_RootDir()
{
 intia(0x20);

 commd[0]=0x35;	  // return root dir

 serial_syn();
 serial_send(commd, 0, 0);
}

/*----------------------------------
函数功能:read data
入口参数:length读取数据的长度
-----------------------------------*
void USB118_Read(uint length)
{
 uchar leng;
 intia(0x20);

 leng=(uchar)length&0xff;
 length>>=8; 
 commd[0]=0x08;	    //read file
 commd[1]=leng;
 commd[2]=(uchar)length;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*----------------------------------
函数功能:列举目录下文件和文件夹
-----------------------------------*
void USB118_List()
{
 intia(0x20);
 
 commd[0]=0x10;
 commd[1]=0;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*--------------------------------------------------------------
函数功能:删除指定文件或子目录
入口参数:na1-8是要删除的文件名, tp1-3是要删除的文件的扩展名
--------------------------------------------------------------*
void USB118_Remove(uchar na1, uchar na2, uchar na3, uchar na4,uchar na5, uchar na6, uchar na7, uchar na8, 
                  uchar tp1, uchar tp2, uchar tp3)
{
 intia(0x20);

 commd[0]=0x11;
 commd[1]=0;
 commd[2]=na1;
 commd[3]=na2;
 commd[4]=na3;
 commd[5]=na4;
 commd[6]=na5;
 commd[7]=na6;
 commd[8]=na7;
 commd[9]=na8;

 commd[10]=tp1;
 commd[11]=tp2;
 commd[12]=tp3;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*----------------------------------------
函数功能:获取磁盘空间
----------------------------------------*
void USB118_GetCapacity()
{
  intia(0x20);

  commd[0]=0x12;
  commd[1]=0;

  serial_syn();
  serial_send(commd, 0, 0);
}

/*--------------------------------------------------
函数功能:移动指针位置
入口函数:sp 是指针移动到的位置 (0--4294967295)
---------------------------------------------------*/
void USB118_SetFilePointer(long int sp)
{
 //intia(0x20);

 commd[0]=0x15;
 commd[1]=0;
 commd[2]=(uchar)sp;
 sp>>=8;
 commd[3]=(uchar)sp;
 sp>>=8;
 commd[4]=(uchar)sp;
 sp>>=8;
 commd[5]=(uchar)sp;

 serial_syn();
 serial_send(commd, 0, 0);
}

/*---------------------------------
函数功能:获取系统版本号
-----------------------------------*
void USB118_GetVersion()
{
 intia(0x20);

 commd[0]=0x30;
 commd[1]=0;

 serial_syn();
 serial_send(commd, 0, 0);
} 

/************************************************
主函数
*************************************************/
main()
{

 Del(300);
 //intia(0x20);  
 Init_COMM(9600);

 while(1)
 {
  USB118_Detect();
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x04)&(commd[1]==0x01)) 
	  {
	   break;
	   }
   
 }
 while(1)
 {
  USB118_Crefiledir( 0x07,  0xB4, 0XEF, 0xCC, 0xA9, 'U', 'S', 'B', 0x20, 'T', 'X', 'T');
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x0A)&(commd[1]==0x01)) 
	  {
	   break;
	   }
   
 } 	  
 Del(10);	
   while(1)
 {
  USB118_Open(0xB4, 0XEF, 0xCC, 0xA9, 'U', 'S', 'B', 0x20, 'T', 'X', 'T');
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x0D)&(commd[1]==0x01)) 
	  {
	   sm=(long int)commd[35];
	   sm<<=8;
	   sm|=(long int)commd[34];
	   sm<<=8;							  
	   sm|=(long int)commd[33];
	   sm<<=8;
	   sm|=(long int)commd[32];
	   break;
	   }
   
 }
 Del(10);
  while(1)
 {
  USB118_SetFilePointer(sm);
  Del(50);
 // if(Command_status==0x01)
	if((commd[0]==0x25)&(commd[1]==0x01)) 
	  {
	   break;
	   }
   
 } 		
 Del(10); 

 while(1)
 {
  int_data1(0x41);	 //Tab键的ASCII码09H
  dat1[0]=0xCE;	  //西
  dat1[1]=0xF7;	  

  dat1[2]=0xB0;	  //安
  dat1[3]=0xB2;

  dat1[4]=0xB4;	  //达
  dat1[5]=0xEF;	 

  dat1[6]=0xCC;	  //泰
  dat1[7]=0xA9;

  dat1[8]=0xB5;	  //电
  dat1[9]=0xe7;

  dat1[10]=0xD7;  //子
  dat1[11]=0xd3;  

  dat1[12]=0x20;   //空格键的ASCII码为20H

  dat1[13]='U';
  dat1[14]='S';
  dat1[15]='B';
  dat1[16]='1';
  dat1[17]='1';
  dat1[18]='8';
  dat1[19]='A';

  	
  USB118_Write(dat1, 19);
  Del(70);
 // if(Command_status==0x01)
	if((commd[0]==0x0C)&(commd[1]==0x01)) 
	  {
	   break;
	   }  
 }
 Del(10);
 while(1);
 }

/*---------------------------------------------------
                接收中断函数
函数功能：接收数据
出口参数：Command_status的状态
----------------------------------------------------*/
void serial() interrupt 4 using 0
{
  uint m;
  if(RI)
   {
   ES=0;
   m= Receive_One(commd, 0);
  //m=serial_rec(commd,0, 0);  
   if(m)
     Command_status=0x01;  	  	
  }
 }  