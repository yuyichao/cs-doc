#include "config.h"


unsigned char Device_Descriptor[18] = {0x12,0x01, 0x10, 0x01,0xDC,0x0,0x0,0x20,       
						               0x05, 0x80,0x00, 0x10,0x00,0x00,0,0,0,0x01          	
						              };
unsigned char Configuration_Descriptor_All[32] = {
		        	    9,2,0x20,0x00,1,1,0,0x80,0xfa,//ÅäÖÃÃèÊö·û
                        9,4,0,0,2,0xDC,0xAC,0xBC,0, //½Ó¿ÚÃèÊö·û                  
					    7,5,0x81,0x02,0x20,0x00,0x0,//¶Ë¿ÚÃèÊö·û            
						7,5,0x02,0x02,0x20,0x00,0x0  //¶Ë¿ÚÃèÊö·û         
				     	};
bit caiji_start,usb_connected,usb_connected_stored,usb_configured_stored;
int bufout[256],gaptime,gl_reminder_inc_ms,i,j;
unsigned char   bmRequestType,usb_sof_counter,usb_configuration_nb,*pbuffer,endpoint_status[2];
////////////////////////////
void main (void)//Ö÷³ÌÐò
{
  usb_task_init();//USB³õÊ¼»¯
  EpEnable();//¶Ë¿ÚÊ¹ÄÜ
  while(1)
  {  
  usb_task();//USB´¦Àíº¯Êý 
  for(i=0;i<100;i++);
  caiji();//Êý¾Ý²É¼¯´¦Àíº¯Êý
  }

}
////////////////////////////
void usb_task_init(void)
{ 
  USBCON |= 0x80; //Ê¹ÄÜUSB¿ØÖÆÆ÷
  USBCON |= 0x10; /*USBÈí¼þ²å°Î*/
  delay(100);
  USBCON &= ~0x10;
  PLLDIV = 32; //ÅäÖÃ¿ØÖÆÆ÷Ê±ÖÓ
  PLLCON |= 0x02;//Ê¹ÄÜPLL
  UEPNUM = 0;//¸´Î»¿ØÖÆ¶Ë¿Ú
  UEPCONX = 0;
  /*³õÊ¼»¯×´Ì¬±äÁ¿*/
  usb_connected = 0;
  usb_connected_stored = 1;
  usb_configured_stored = 0;
  endpoint_status[0] = 0x00;
  endpoint_status[1] = 0x00;
  usb_connected = 0;
  usb_configuration_nb = 0x00;
}
///////////////////////////////////
//////////////////////////////////////
void EpEnable(void)
{
	UEPNUM=0x00;	UEPCONX=0x80;//¶Ë¿Ú0
	UEPNUM=0x01;	UEPCONX=0x86;//¶Ë¿Ú1
	UEPNUM=0x02;	UEPCONX=0x82;//¶Ë¿Ú2
	UEPRST=0x07;	UEPRST= 0x00;//¶Ë¿Ú¸´Î»
	UEPIEN=0x07;	USBIEN|=0x01<<4;
	USBADDR=0x01<<7;
}
////////////////////////////////////////////////
void usb_task(void){
      if (UEPINT & EP0)usb_enumeration_process();
      if (UEPINT & EP1)   
	    {
	     Usb_clear_tx_complete();	//UEPINTµØÖ·0xF8È´²»ÄÜÎ»Ñ°Ö·	
	    }
	   if (UEPINT & EP2)   
		 {              
         unsigned char  Bufin[64];  
         i = ReadEp(2,Bufin);
		 if(Bufin[1]&0x80!=0)
		  {
          caiji_start=1;//ÉèÖÃ²É¼¯±êÖ¾Î»
          gaptime= Bufin[0];//²É¼¯ÆµÂÊ	
          if(Bufin[1]&0x7f==1)
            {
             P1_4=1;
             }
          if(Bufin[1]&0x7f==2)
             {
             P1_5=1;
             }
          if(Bufin[1]&0x7f==3)
             {
             P1_6=1;
             }
           if(Bufin[1]&0x7f==4)
             {
             P1_7=1;
             }                			
		   }
           else
           {
             caiji_start=0;//ÉèÖÃ²É¼¯×´Ì¬Î»
           }
    /*Êý¾Ý»º³åÇøÇåÁã*/             
	for(i=0;i<64;i++)
		{
		Bufin[i]=0;
		}
   }           
  }
/////////////////////////////////////////////////////////
////////////////////////////////////////////////
void usb_enumeration_process (void)//USBÉè±¸Ã¶¾Ù¹ý³Ì
{ 
  UEPNUM = 0;//Ñ¡Ôñ¿ØÖÆ¶Ë¿Ú
  bmRequestType = UEPDATX;          /* µÃµ½ bmRequestType */
  switch (UEPDATX)                  /* ÅÐ¶Ï bRequest µÄÖµ */
  {
    case GET_DESCRIPTOR:                   /*»ñµÃÃèÊö·ûÇëÇó*/
      usb_get_descriptor();
      break;
    case GET_CONFIGURATION:               /*»ñµÃÅäÖÃÇëÇó*/
      usb_get_configuration();
      break;
    case SET_ADDRESS:                    /*ÉèÖÃµØÖ·ÇëÇó*/
      usb_set_address();
      break;
    case SET_CONFIGURATION:              /*ÉèÖÃÅäÖÃÇëÇó»òÉèÖÃHID±¨±íÇëÇó*/
      if (bmRequestType == 0) { usb_set_configuration(); }
         break;
     default:
      UEPSTAX =UEPSTAX & ~0x04;        /*Çå³ýSETUP±êÖ¾*/
      UEPSTAX =UEPSTAX | 0x20;         /*ÖÐÖ¹ÇëÇóÉèÖÃ*/               
      while (!UEPSTAX & 0x08);
      UEPSTAX =UEPSTAX &~ 0x20;
      UEPSTAX =UEPSTAX &~ 0x08;
      break;
    }
    UEPSTAX = UEPSTAX &~0x80;
}
//////////////////////////////////////////////////////
void usb_set_address (void)                 /*ÉèÖÃÉè±¸µØÖ·×Ó³ÌÐò*/
{
  unsigned char address;
  address = UEPDATX;                    /* »ñµÃÏµÍ³·ÖÅäµÄÉè±¸µØÖ· */
  UEPSTAX &= ~0x04;                     /*Çå³ýSETUP±êÖ¾*/
  UEPSTAX |= 0x10;                       /* ·µ»Ø0×Ö½Ú×´Ì¬×Ö */
  USBCON |= 0x01;                         /*ÉèÖÃµØÖ·Ê¹ÄÜ*/
  while (!(UEPSTAX & 0x01));
  UEPSTAX=UEPSTAX & ~0x01;
  USBADDR = (0x80 | address);              /*ÅäÖÃÉè±¸µØÖ·*/
}
////------set_config-------////////
void usb_set_configuration (void)
{
  unsigned char configuration_number;
  configuration_number = UEPDATX;   /* ¶ÁÈ¡ÅäÖÃÊý*/
  UEPSTAX &= ~0x80;
  UEPSTAX &= ~0x04;                   /*Çå³ýSETUP±êÖ¾*/
  if (configuration_number <= 1)
  {
    usb_configuration_nb = configuration_number;
  }
  else
  {
    UEPSTAX |= 0x20;            /*ÖÐÖ¹ÇëÇóÉèÖÃ*/ 
    while (!UEPSTAX & 0x08);
    UEPSTAX &= ~0x20;
    UEPSTAX &= ~0x08;
    return;
  }

  UEPSTAX |= 0x10;        /* ·µ»Ø0×Ö½Ú×´Ì¬×Ö*/
  while (!UEPSTAX & 0x01);
  UEPSTAX &= ~0x01;
  /* Éè±¸¶Ë¿ÚÅäÖÃ */
  UEPNUM = 1;
  UEPCONX = 0x87;
  UEPRST = 0x01;
  UEPRST = 0x00;
}
///////////////////////////////////////////////////////////////
void usb_get_descriptor (void)
{
  unsigned char   data_to_transfer;
  unsigned  int  wLength;
  unsigned char   descriptor_type;
  unsigned char   string_type;                      
  string_type = UEPDATX;            /* ¶ÁÈ¡ wValueµÄµÍÎ» */
  descriptor_type = UEPDATX;        /* ¶ÁÈ¡ wValueµÄ¸ßÎ» */
  switch (descriptor_type)          /*ÅÐ¶ÏÃèÊö·ûÀàÐÍ*/
  {
    case DEVICE:                            /*Éè±¸ÃèÊö·û¸*/                
    {
      data_to_transfer = sizeof (Device_Descriptor);
      pbuffer = &(Device_Descriptor[0]);//Ö¸ÏòÉè±¸ÃèÊö·û½á¹¹Ê×µØÖ·
      break;
    }

    case CONFIGURATION:                       /*ÅäÖÃ*/
    {
      data_to_transfer = sizeof (Configuration_Descriptor_All);
      pbuffer = &(Configuration_Descriptor_All[0]);
      break;
   } 
    default:
    {
      UEPSTAX &= ~0x04;
      UEPSTAX |= 0x20;
      while ((!(UEPSTAX & 0x08)) && (UEPSTAX & 0x04));
      UEPSTAX &= ~0x08;
      UEPSTAX &= ~0x20;
      UEPSTAX &= ~0x80;
      return;
    }
  }

  ACC = UEPDATX;                   
  ACC = UEPDATX;
  ((unsigned char*)&wLength)[1] = UEPDATX;   /* ¶ÁÈ¡Òª´«ÊäµÄ³¤¶È */
  ((unsigned char*)&wLength)[0] = UEPDATX;
  if (wLength > data_to_transfer);     /* ¶ÁÈ¡µÄ³¤¶È´óÓÚÃèÊö·û³¤¶ÈÊ± */      
  else
  {
    data_to_transfer = (unsigned char)wLength;       /*´«ËÍÐèÒªµÄÊý¾Ý³¤¶È */
  }
  UEPSTAX &= ~0x04 ;                    
  UEPSTAX |= 0x80;                            
  while (data_to_transfer > 32)/*´«ËÍµÄ³¤¶È´óÓÚ¿ØÖÆ¶Ë¿ÚµÄ³¤¶ÈÊ±*/
  {
    pbuffer = usb_send_ep0_packet(pbuffer, 32);/*´«ËÍÒ»´Î¶Ë¿Ú³¤¶ÈµÄÊý¾Ý*/
    data_to_transfer -= 32;
    while ((!(UEPSTAX & 0x42)) && (!(UEPSTAX & 0x01)));/*´«ÊäÃ»ÓÐÍê³É*/
    UEPSTAX=UEPSTAX & 0x01;
    if ((UEPSTAX & 0x42))               
    {
      UEPSTAX &= ~0x10;
      UEPSTAX &= ~0x02;
      return;
    }
  }
  /* ´«ËÍ×îºóÒ»´ÎÊý¾Ý */
  pbuffer = usb_send_ep0_packet(pbuffer, data_to_transfer);
  data_to_transfer = 0;
  while ((!(UEPSTAX & 0x42)) && (!(UEPSTAX & 0x01)));
  UEPSTAX=UEPSTAX & 0x01;
  if ((UEPSTAX & 0x42))                  /* if cancel from USB Host */
  {
    UEPSTAX &= ~0x10;
    UEPSTAX &= ~0x02;
    return;
  }
}
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
unsigned int caiji_single()
{//Æô¶¯²ÉÑù
   unsigned int dd;
   P1_0=0;
   P1_1=0;
   for(i=0;i<10;i++);
   P1_0=1;
   P1_1=1;
  //²éÑ¯×ª»»ÊÇ·ñ½áÊø
  if(P1_3==1);
   //¶ÁÈ¡×ª»»Êý¾Ý
  P1_0=0;
  P1_2=0;
  dd=P1;
  return dd;
}
//////////////////////////////////////////////////////
void caiji()
{
if(caiji_start == 1)
{
for(i=0;i<256;i++)
{
bufout[i]=caiji_single();
delay(gaptime);
}
for(i=0;i<100;i++);
senddata();
}
}
/////////////////////////////////

void senddata()
{

for(i=0;i<8;i++)
{
 for(j=0;j<32;j++)
{
  Usb_write_byte(bufout[i*32+j]);
}
Usb_set_tx_ready();
while(!Usb_tx_complete());
Usb_clear_tx_complete();
}
}
/////////////////////////////

void delay(int mm)
{
for(i=0;i<mm;i++);
}
