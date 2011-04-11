
#include "UDISK_DEF.H"
#define io_port1 *(char xdata *)0x8011
unsigned char Device_Descriptor[18] = {
				    	 0x12,			   //0x12
						 0x01,             //设备描述符类型
						 0x10, 0x02,       //spec rev level (BCD) 1.0
						 0x0,              //设备类型代码
						 0x0,              //设备子类型代码
						 0x0,              //设备协议
						 0x20,             //max packet size
						 0x11, 0x11,       
						 0x22, 0x22,         
						 0x33, 0x33,        
						 0,                 
						 0,               
						 0,                 
						 0x01              //配置数量 	
						};

unsigned char Configuration_Descriptor_All[32] = {
                       /*配置描述符*/
		        	    9,                 //描述符长度
					    2,                 //配置描述符 (0x02)
					    0x20,              
						0x00,              
						1,                 //设备接口数量
						1,                 
					    0,                
						0x80,
						0xfa,
              		    /*接口描述符*/
					    9,                 //描述符长度
					    4,                  //接口描述符
					    0,                 
					    0,                 
						2,                 //设备中用到的端口数目
						8,                 //设备类代码--海量存储设备
						6,	               //6=SCSI
						0x50,              //bulk 0nly 传输
						0,                 
						/*端口描述符*/                 
						0x07,             //端口描述符长度.   
						0x05,             //描述符类型---端口
						0x81,             //IN端口
						0x02,             //BULK端口   
						0x20, 0x00,       //最大传输量 
						0x0,              
						                  
						0x07,             //端口描述符长度.   
						0x05,             //描述符类型---端口
						0x02,             //OUT端口
						0x02,             //BULK端口
						0x20, 0x00,       //最大传输量
						0x0               
					};

unsigned char B_InquiryData[] = {
							0x00,	                       //Direct Access Device
							0x80,	                       //RMB
							0x00,	                       //ISO/ECMA/ANSI
							0x01,	                       //Response Data Format
							0x1f,	                       //Additional Length
							0x00,	                       //Reserved
							0x00,	                       //Reserved
							0x00,	                       //Reserved 
							'X', 'I', 'N', 'Z', 'X', ' ', ' ', ' ',	       //Vendor Information
							'U', 'S', 'B', '-', 'M', 'A', 'S', 'S', 'S', 'T', 'O', 'R', 'A', 'G', 'E', ' ',//Product Identification
							0, 0, 0, 0					   
							};	               
/*SCSI-Read_Format_capacities命令的返回数据	*/
code unsigned char B_Read_Format_capacities[] = {0x00, 0x00, 0x00, 0x10,	        //capacity list header
									0x00, 0x00, 0x07, 0xf5,	0x01, 0x00, 0x02, 0x00,	//capacity descriptor
								    //Number of Blocks =2037,unformatted media,blocklength = 512Bytes
									0x00, 0x00, 0x07, 0xfd,	0x00, 0x00, 0x02, 0x00  //Formattable Capacity Descriptors
									};
//SCSI-Read_Capacity命令的返回数据
code unsigned char B_Read_Capacity[] = {
								0x00, 0x00, 0xfe, 0xa0,	   //Last  Logical Block Address for 32MB 
								0x00, 0x00, 0x02, 0x00	   //block length in bytes
								};
//SCSI-Mode_Sense命令的返回数据    	   		
code unsigned char B_Mode_Sense_ALL[] = {0x0b, 0x00,       //Mode Data Length
							0x00, 0x08, 0x00, 0x00,
							0x7d, 0, 0, 0, 0x02, 0x00
							};
//SCSI-Mode_Sense命令的返回数据			
code unsigned char B_Mode_Sense_TPP[] = {0xf0, 0x00,       //Mode Data Length
							05, 00, 00, 00, 00, 0x0b, 00, 00, 00, 00, 0x24, 00, 00, 00, 00, 00
							};
//SCSI-Mode_Sense命令的返回数据			
code unsigned char B_Mode_Sense_ZERO[] = {0x00, 0x06,      //Mode Data Length
							0x00,	                       //Medium Type Code
							0,                             //write enabled
							0, 0, 0, 0                     //reserved	
                            };


unsigned char data bulk_CSW[]={0x55,0x53,0x42,0x53,	//bytes 4 dCSWSignature
					      0x00,0x00,0x00,0x00,	    //bytes 4 dCSWTag
					      0x00,0x00,0x00,0x00,		//bytes 4 dDataResiduce
					      0x00};			        //bCSWStatus  00=good state.
struct_CBW data bulk_CBW;
void main()
{   
	Flash_Reset();											         									         
	USBInit();
	EpEnable();
	while(1)
	{
	if (UEPINT & EP0)   Ep0();//端口0中断处理
	if (UEPINT & EP1)   Ep1();//端口1中断处理			 
	if (UEPINT & EP2)   Ep2();//端口2中断处理 
    }
}/////////////////////////////////////////////////////
void USBInit()
{
	int data i;	
	CKCON|=1;   //X2 Mode
	/*PLL配置*/
	PLLNDIV	=	0x04;
	PLLCON |=	(0x3&11)<<6;
	PLLRDIV	=	(0x3ff&11)>>2;
	USBCLK=0;
	PLLCON&=(~PLLRES);
	PLLCON|=PLLEN;
	USBCON&=(~USBE);
	for(i=0;i<1000;i++);//延时
	USBCON|=USBE;//USB控制器使能

}	
///////////////////////////////////////////////////
void EpEnable(void)
{
	UEPNUM=0x00;	UEPCONX=0x80;
	UEPNUM=0x01;	UEPCONX=0x86;
	UEPNUM=0x02;	UEPCONX=0x82;
	UEPRST=0x07;	UEPRST= 0x00;
	UEPIEN=0x07;	USBIEN|=EEOFINT;
	USBADDR=FEN;
}
///////////////////////////////////////////////////
unsigned char ReadEp(unsigned char EpNum,unsigned char *Data)//读指定端口
{
	unsigned char data i=0;
	UEPNUM=EpNum;
	while(i<UBYCTLX)
	{
		Data[i++]=UEPDATX;
	}	
	UEPSTAX&=~(RXOUTB0|RXOUTB1|RXSETUP);
	return(i);
}
/////////////////////////////////////////////////////
void WriteEp(unsigned char EpNum,unsigned char nLength,unsigned char *Data)//写指定端口
{
	unsigned char data i=0;
	UEPNUM=EpNum;
	UEPSTAX|=DIR;
	while(nLength--) UEPDATX=Data[i++];	
	UEPSTAX|=TXRDY;
	while(!(UEPSTAX&TXCMP)) ;
	UEPSTAX&=(~(TXCMP));
}
///////////////////////////////////////////////////
void Set_Address(unsigned char EpNum)//SET_address请求处理
{
	WriteEp(0,0,0);
	USBADDR|=EpNum;
	USBADDR|=FEN;
	USBCON|=FADDEN;
}
//////////////////////////////////////////////////////
void Get_Descriptor(unsigned char DesType,unsigned char nLength)//获得设备的描述符请求处理函数
{
	if(DesType==0x01)//设备描述符
		WriteEp(0,18,Device_Descriptor);

	if((DesType==0x02)&&(nLength==0x09))//配置描述符
		WriteEp(0,9,Configuration_Descriptor_All);

	if((DesType==0x02)&&(nLength==0xff))//描述符集合
	{
	    WriteEp(0,32,Configuration_Descriptor_All);
	    WriteEp(0,2,&Device_Descriptor[4]);
	}

	if((DesType==0x02)&&(nLength==0x20)) //配置、接口、端口描述符集合
		WriteEp(0,32,Configuration_Descriptor_All);
}
//////////////////////////////////////////////////////////
void Set_Configuration(unsigned char wValue)//使能配置
{
  if(wValue == 0)
  {
		UEPNUM=0x00;	UEPCONX=0x80;
		UEPNUM=0x01;	UEPCONX=0x86;
		UEPNUM=0x02;	UEPCONX=0x82;
		USBCON&=(~CONFG);
		WriteEp(0,0,0);	
  }
  else if(wValue == 1) 
  {
		UEPNUM=0x00;	UEPCONX=0x80;
		UEPNUM=0x01;	UEPCONX=0x86;
		UEPNUM=0x02;	UEPCONX=0x82;
		USBCON|=CONFG;
		WriteEp(0,0,0);	
  }
}
///////////////////////////////////////////////////////////////
void Ep0()//端口0处理函数
{
  unsigned char data DT[32]={0,};
  unsigned char data i;
  i = ReadEp(0,DT);
  if (((DT[0] & 0x60)==0) && i)
  {
    switch (DT[1])
    {
      case set_address				:Set_Address(DT[2]);			break;
      case get_descriptor			:Get_Descriptor(DT[3],DT[6]);	break;
      case set_configuration	    :Set_Configuration(DT[2]);		break;
      default						:;					            break;
	}
  }
  else if(DT[0]==0xa1)
	{
		WriteEp(0,0,0);
	}							  						
}

/////////////////////////////////////////////////////////////////////////
void WriteEpBulk(unsigned char EpNum,unsigned char nLength,unsigned char *Data)//写端口
{
	unsigned char data i;
	UEPNUM=EpNum;
	UEPSTAX|=DIR;
	for(i=0;i<nLength;i++) UEPDATX=Data[i];
	UEPSTAX|=TXRDY;
}
//////////////////////////////////////////////////////////////////////////
void TransmitCSW()//传送状态字
{
  WriteEpBulk(1, sizeof(bulk_CSW), bulk_CSW);
  while(!(UEPSTAX&TXCMP)) ;
  UEPSTAX&=(~(TXCMP));
}
///////////////////////////////////////////////////////////////////////////
void Ep1()//端口1处理函数
{
  UEPSTAX&=(~(TXCMP));
  TransmitCSW();
}
//////////////////////////////////////////////////////////////////////////
 void SCSI_Mode_Sense()
{
  if(bulk_CBW.CBWCB[2] == SCSI_MSPGCD_TPP)                 //Page Code=Timer and Potect Page
    {WriteEpBulk(1, sizeof(B_Mode_Sense_TPP), B_Mode_Sense_TPP);}
  else if(bulk_CBW.CBWCB[2] == SCSI_MSPGCD_RETALL)		   //Page Code=All
    {WriteEpBulk(1, sizeof(B_Mode_Sense_ALL), B_Mode_Sense_ALL);}
  else {WriteEpBulk(1, sizeof(B_Mode_Sense_ZERO), B_Mode_Sense_ZERO);}
}
///////////////////////////////////////////////////////////////////////////
void SCSI_Read_Format_Capacities()
{
  if(bulk_CBW.CBWCB[7]==0 && bulk_CBW.CBWCB[8]==0)return;
  WriteEpBulk(1, sizeof(B_Read_Format_capacities), B_Read_Format_capacities);
}
/////////////////////////////////////////////////////////////////////////
void SCSI_Read10()//FLASH读处理函数
{
  unsigned char data i;
  unsigned char	Addr[4];
  unsigned char length;
  /*读操作的起始地址*/
  Addr[2] = bulk_CBW.CBWCB[4];
  Addr[3] = bulk_CBW.CBWCB[5];
  /*读操作的扇区长度*/
  length = bulk_CBW.CBWCB[8];
  while(length>0)
  {
  P5 = COMMAND;
  io_port1 = 0x00;//FLASH读命令码
  P5 = ADDRESS;
  io_port1 = 0;
  io_port1 = Addr[3];
  io_port1 = Addr[2];
  P5 = D_DATA;
  UEPNUM=0x01;
  UEPSTAX|=DIR;
  while(!(P5 & RB));
  /*读取一个扇区的数据*/
  for(i=0;i<8;i++)
  {
  ReadFlash();
  UEPSTAX|=TXRDY;
  while(!(UEPSTAX&TXCMP));
  UEPSTAX&=(~(TXCMP));	  }
  io_port1 = INACTIVE;
  length--;	
  Addr[3]++;
 if(Addr[3]==255)
  {
  Addr[3]=0;
  Addr[2]++;
  }  
  }
  TransmitCSW();
}
/////////////////////////////////////////
void delay()
{
  unsigned char data i=100;
  while(i-->0);
}
/////////////////////////////////////////////
void SCSI_Write10()	//FLASH写操作
{
   unsigned char addr[4];
   unsigned char data i=0,length=0,nBeginPage=0;
   /*开辟一个数据缓冲区*/
    P5 = COMMAND;
    io_port1 = 0x60;
    P5 = ADDRESS;
    io_port1 = BuffBlock;
	io_port1 = 0xff;
    P5 = COMMAND;						  
    io_port1 = 0xd0;
    P5 = D_DATA;
	delay();
    while(!(P5 & RB));
   /*得到写操作的参数--起始扇区号、扇区长度*/
	addr[2] = bulk_CBW.CBWCB[4];
	addr[3] = bulk_CBW.CBWCB[5];
	length = bulk_CBW.CBWCB[8];	
	nBeginPage = addr[3]&0x1f;
   /*把写操作扇区对应的块的数据COPY到缓冲区*/
	UEPNUM = 0x02;	
	delay();
	while(!(P5 & RB)); 
    if(nBeginPage>0)
	  {
	     for(i=0;i<nBeginPage;i++)
		   { 
			  P5 = COMMAND;
			  io_port1 = 0x00;
			  P5 = ADDRESS;
              io_port1 = 0;					 //A0-A7
              io_port1 = (addr[3]&0xe0)|i; 	 //A9-A16
	          io_port1 = addr[2]; 			 //A17-A24 
			  P5 = D_DATA;
              delay();
		      while(!(P5 & RB)); 

			  P5 = COMMAND;
			  io_port1 = 0x8a;
			  P5 = ADDRESS;
              io_port1 = 0;					        
     		  io_port1 = BuffBlock|i;
	          io_port1 = 0xff; 				        
			  P5 = D_DATA;
			  delay();
		      while(!(P5 & RB));
		   }
		 nBeginPage=0;
	  }
	  /*数据先写到缓冲区*/
	while(length>0)
	  {
	  	 P5 = COMMAND;
	     io_port1 = 0x80;//PAGE写操作码
	     P5 = ADDRESS;
	     io_port1 = 0;							  //A0-A7	 
         io_port1 = (addr[3]&0x1f)|BuffBlock;
		 io_port1 = 0xff;				 		  //A17-A24
		 P5 = D_DATA;
		 for(i=0;i<8;i++)
		   {
			  while (!(UEPINT & EP2));	
			  WriteFlash();
			  UEPSTAX &= 0xB9;
		   }
		 P5 = COMMAND;
		 io_port1 = 0x10;
		 P5 = D_DATA;
		 length--;	
		 delay();
		 while(!(P5 & RB));
		 if(((addr[3]&0x1f)==0x1f)||(length==0))
		   {
		   		 /*保存数据到缓冲区*/
	    for(i=((addr[3]&0x1f)+1);i<32;i++)
	       {
	         P5 = COMMAND;
		     io_port1 = 0x00;
		     P5 = ADDRESS;
             io_port1 = 0;						     //A0-A7  
             io_port1 = (addr[3]&0xe0)|i; 	     //A9-A16
	         io_port1 = addr[2]; 			     //A17-A24 
		     P5 = D_DATA;
		     delay();
		     while(!(P5 & RB));
             P5 = COMMAND;
            io_port1 = 0x8a;
             P5 = ADDRESS;
             io_port1 = 0;						        //A0-A7
      	   io_port1 = BuffBlock|i;
	         io_port1 = 0xff; 				            //A17-A24 
	         P5 = D_DATA;
	         delay();
	         while(!(P5 & RB));
	        }
			/*擦除要写的数据块*/
		   	  P5 = COMMAND;
              io_port1 = 0x60;				 //擦除当前block
              P5 = ADDRESS;
              io_port1 = addr[3];          //A9-A16
	          io_port1 = addr[2]; 		 //A17-A24 
              P5 = COMMAND;
              io_port1 = 0xd0;
              P5 = D_DATA;
			  delay();
		      while(!(P5 & RB));        //等待操作完成  
             /*把缓冲区的数据COPY到指定写的扇区所在块*/
			  for(i=0;i<32;i++)	
			    {
				   P5 = COMMAND;
				   io_port1 = 0x00;
			  	   P5 = ADDRESS;
                   io_port1 = 0;				                //A0-A7
				   io_port1 = BuffBlock|i; 
	               io_port1 = 0xff; 			                //A17-A24 
				   P5 = D_DATA;
				   delay();
		           while(!(P5 & RB)); 

				   P5 = COMMAND;
				   io_port1 = 0x8a;  
			  	   P5 = ADDRESS;
                   io_port1 = 0;						   //A0-A7
                   io_port1 = (addr[3]&0xe0)|i;      //A9-A16
	               io_port1 = addr[2]; 			   //A17-A24 
				   P5 = D_DATA;
				   delay();
		           while(!(P5 & RB));
  			    }

			  if(length>0)
			    {
				   P5 = COMMAND;
                   io_port1 = 0x60;
                   P5 = ADDRESS;
   
				   io_port1 = BuffBlock;
	               io_port1 = 0xff; 				           //A17-A24 
                   P5 = COMMAND;
                   io_port1 = 0xD0;
                   P5 = D_DATA;
				   delay();
				   while(!(P5 & RB));
				}
 		   }
		   addr[3]++;
		  if(addr[3]==255)
		  {
		  addr[2]++;
		  }
		  
	  }

	TransmitCSW();	
}	
//////////////////////////////////
void Ep2()//端口2中断处理函数
{
  unsigned char data i;
  unsigned char data Buf[64];
  i = ReadEp(2,Buf);
  bulk_CSW[4] = Buf[4];  bulk_CSW[5] = Buf[5]; bulk_CSW[6] = Buf[6]; bulk_CSW[7] = Buf[7];
 for(i=0;i<12;i++) bulk_CBW.CBWCB[i] = Buf[i+15];
 switch(bulk_CBW.CBWCB[0])
  {
  case Inquiry			         :WriteEpBulk(1,36,B_InquiryData);break;
  case Mode_Sense			     :SCSI_Mode_Sense();              break;
  case Read10				     :SCSI_Read10();                  break;
  case Read_Capacity		     :WriteEpBulk(1, sizeof(B_Read_Capacity), B_Read_Capacity);break;
  case Read_Format_Capacities	 :SCSI_Read_Format_Capacities();  break;
  case Test_Unit_Ready	         :TransmitCSW();                  break;
  case Verify				     :TransmitCSW();                  break;
  case Write10			         :SCSI_Write10();                 break;
  case Medium_Removal		     :TransmitCSW();                  break;
  }
}
/////////////////////////////////////
void Flash_Reset(void)		                                     //flash reset
{ 
	unsigned int data i;
	P5 = COMMAND;                     
	io_port1 = 0xff;	                 //reset command
	for (i=0; i<3000; i++) ;	                                 //delay 	
}
////////////////////////////////////
void ReadFlash()	 //从FLASH读取32个字节数据并保存在端口缓存
{
    int i;
	for(i=0;i<32;i++)
	{
	UEPDATX=io_port1;
	}
}
///////////////////////////////////
void WriteFlash()	 //从端口得到32个字节的数据写到FLASH 
{
int i;
  for(i=0;i<32;i++)
  {
  io_port1=UEPDATX;
  
  } 
} 
