
#include "usb_task.h"
#include "usb_kbd_enum.h"
#include "ext_5131.h"
#include "reg_5131.h"
//////////////////////////////
code struct usb_st_device_descriptor usb_device_descriptor =
{
  sizeof(usb_device_descriptor), 0x01, 0x1001, 0,
  0, 0, 32, 0xeb03, 0x0320,
  0x0001, 0x01, 0x02, 0x03, 1
};
/*STRINGÃèÊö·û*/
code struct usb_st_manufacturer usb_manufacturer =
{ sizeof(usb_manufacturer),  0x03, {'X'<<8, 'I'<<8, 'N'<<8, 'Z'<<8, 'X'<<8} };

code struct usb_st_product usb_product =
{ sizeof(usb_product),       0x03, {'A'<<8, 'T'<<8, '8'<<8, '9'<<8, 'C'<<8, '5'<<8, '1'<<8, \
                               '3'<<8, '1'<<8, ' '<<8, \
                               'H'<<8, 'I'<<8, 'D'<<8, ' '<<8, \
                               'K'<<8, 'e'<<8, 'y'<<8, 'b'<<8, 'o'<<8, 'a'<<8, 'r'<<8,'d'<<8} };

code struct usb_st_serial_number usb_serial_number =
{ sizeof(usb_serial_number), 0x03, {'1'<<8, '.'<<8, '2'<<8, '.'<<8, '3'<<8} };

code struct usb_st_language_descriptor usb_language =
{ sizeof(usb_language),      STRING, 0x0904 };
/*ÅäÖÃÃèÊö·û¼¯ºÏ*/
code struct  
{ struct usb_st_configuration_descriptor  cfg;
  struct usb_st_interface_descriptor      ifc;
  struct usb_st_hid_descriptor            hid ;
  struct usb_st_endpoint_descriptor       ep1 ;
  unsigned char                           rep[0x3b] ;
}
  usb_configuration =
  {
    { 9, 0x02, 0x2200, 1, 1,0, 0x80, 50},
    { 9, 0x04, 1, 0, 1, 0x03, 0, 0, 0 },
    { 9, 0x21, 0x1101, 8, 1, 0x22, 0x3B00 },
    { 7, 0x05, 0x81, 0x03, 32, 0x20 },
    { 0x05,0x01,          /* Usage Page (Generic Desktop)      */        
      0x09,0x06,          /* Usage (Keyboard)                  */
      0xA1,0x01,          /* Collection (Application)          */
      0x05,0x07,          /* Usage Page (Keyboard)             */
      0x19,224,           /* Usage Minimum (224)               */
      0x29,231,           /* Usage Maximum (231)               */
      0x15,0x00,          /* Logical Minimum (0)               */
      0x25,0x01,          /* Logical Maximum (1)               */
      0x75,0x01,          /* Report Size (1)                   */
      0x95,0x08,          /* Report Count (8)                  */
      0x81,0x02,          /* Input (Data, Variable, Absolute)  */
      0x81,0x01,          /* Input (Constant)                  */
      0x19,0x00,          /* Usage Minimum (0)                 */
      0x29,101,           /* Usage Maximum (101)               */
      0x15,0x00,          /* Logical Minimum (0)               */
      0x25,101,           /* Logical Maximum (101)             */
      0x75,0x08,          /* Report Size (8)                   */
      0x95,0x06,          /* Report Count (6)                  */
      0x81,0x00,          /* Input (Data, Array)               */
      0x05,0x08,          /* Usage Page (LED)                  */
      0x19,0x01,          /* Usage Minimum (1)                 */
      0x29,0x05,          /* Usage Maximum (5)                 */
      0x15,0x00,          /* Logical Minimum (0)               */
      0x25,0x01,          /* Logical Maximum (1)               */
      0x75,0x01,          /* Report Size (1)                   */
      0x95,0x05,          /* Report Count (5)                  */
      0x91,0x02,          /* Output (Data, Variable, Absolute) */
      0x95,0x03,          /* Report Count (3)                  */
      0x91,0x01,          /* Output (Constant)                 */
      0xC0                /* End Collection                    */  
      }
  };
unsigned char key[4][4]={HID_W,HID_E,HID_L,HID_C,
                 HID_O,HID_M,HID_E,HID_U,
                 HID_S,HID_B,HID_R,HID_D,
                 HID_C,HID_A,HID_B,HID_SPACEBAR};
unsigned char usb_kbd_state;
unsigned char  transmit_no_key;
typedef union 
{
 unsigned int w;
  unsigned char  b[2];
} Union16;

Union16 scan_key;
unsigned char   usb_key;
unsigned char  key_hit;
unsigned char  shift_key;
static  unsigned char   endpoint_status[2];
static  unsigned char   hid_idle_duration;
static  unsigned char   *pbuffer;
static  unsigned char   bmRequestType;
unsigned char   usb_configuration_nb;
extern  bit     usb_connected;
bit   usb_connected_stored;
bit   usb_configured_stored;
/////////////////////////////
void main (void)
{
  EA = 1;                 //Ê¹ÄÜÖÐ¶Ï
  init_timer0();         //¶¨Ê±Æ÷³õÊ¼»¯
  delay(10);
  usb_task_init();       //usb¿ØÖÆÆ÷³õÊ¼»¯
  while(1)               //ÈÎÎñÑ­»·
  {
  usb_task();           //usbÈÎÎñ´¦Àí×Ó³ÌÐò  
  usb_kbd_task();        //¼üÅÌÈÎÎñ´¦Àí
  delay(256);            //ÑÓÊ±
  }
}

void init_timer0()
{
  TMOD=0x05;
  TH0=0;
  TL0=0;
}
void delay(int n)
{
  TH0=n;
  TL0=0;
  TR0=1;
  if(TH0>1);
  return;
}

void usb_task_init(void)
{ 
  USBCON |= 0x80; //Ê¹ÄÜUSB¿ØÖÆÆ÷
  USBCON |= 0x10; /*USBÈí¼þ²å°Î*/
  delay(100);
  USBCON &= ~0x10;
  PLLDIV = 32; //ÅäÖÃ¿ØÖÆÆ÷Ê±ÖÓ
  PLLCON |= 0x02;//Ê¹ÄÜPLL
  //usb_configure_endpoint(EP_CONTROL, CONTROL);//ÅäÖÃ¿ØÖÆ¶Ë¿Ú
  UEPNUM = 0;//¸´Î»¿ØÖÆ¶Ë¿Ú
  UEPCONX = 0;
  /*³õÊ¼»¯×´Ì¬±äÁ¿*/
  usb_connected = 0;
  usb_connected_stored = 1;
  usb_configured_stored = 0;
  transmit_no_key = 0;
  key_hit = 0;
  usb_kbd_state = 0;
  shift_key = 0;
  endpoint_status[0] = 0x00;
  endpoint_status[1] = 0x00;
  usb_connected = 0;
  usb_configuration_nb = 0x00;
}
/////////////////////////////////
void usb_task(void)
{

  if (USBINT & 0x01)    //Èç¹û´¦ÓÚ¹ÒÆð×´Ì¬
  {
    if (usb_connected == 1)
    { 
      usb_connected = 0;
     }

    if (USBINT & 0x20)//µ±½ÓÊÜµ½»½ÐÑÖ¸ÁîÊ±
    {
      usb_connected = 1;//ÉèÖÃÁ¬½Ó±êÖ¾
      USBINT =USBINT & ~0x01;//Çå³ý¹ÒÆð±êÖ¾
      USBINT =USBINT & ~0x20;//Çå³ý»½ÐÑ±êÖ¾
      USBINT =USBINT & ~0x08;
      }
  }
  else         
  {
    usb_connected = 1;
    if (USBINT & 0x10) //¸´Î»ÖÐ¶Ï;¶
	{
    endpoint_status[0] = 0x00;
    endpoint_status[1] = 0x00;
    usb_connected = 0;
    usb_configuration_nb = 0x00;  
    USBINT =USBINT & ~0x10;
    }
    if (UEPINT != 0)  //USB¶Ë¿ÚÖÐ¶Ï
    {
      UEPNUM = 0;   //Ñ¡Ôñ¿ØÖÆ¶Ë¿Ú
      if (UEPSTAX & 0x04)usb_enumeration_process();//±ê×¼ÇëÇó´¦Àí
      UEPNUM = 1; //Ñ¡ÔñÖÐ¶Ï¶Ë¿Ú1
      if (UEPSTAX & 0x01)
      { 
	  UEPSTAX =UEPSTAX & ~0x01;//Çå³ý·¢ËÍ½áÊø±êÖ¾Î»
	  key_hit = 0;   //ÉèÖÃkey_hit³õÊ¼ÖµÎª0¡£
      }//IN¶Ë¿Ú´¦Àí³ÌÐò
    }
  }
}
/*usb_enumeration_process ()*/

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
      else { usb_hid_set_report(); }
      break;
    case CLEAR_FEATURE:                 /*Çå³ýÌØÐÔÇëÇó*/
      usb_clear_feature();
      break;
    case SET_FEATURE:                   /*ÉèÖÃÌØÐÔÇëÇó*/
      usb_set_feature();
      break;
    case GET_STATUS:                    /*»ñµÃ×´Ì¬ÇëÇó*/
      usb_get_status();
      break;
    case GET_INTERFACE:                 /*»ñµÃ½Ó¿ÚÐÅÏ¢ÇëÇó»òÉèÖÃÏìÓ¦¼ä¸ôÇëÇó*/
      if (bmRequestType == 0x81) { usb_get_interface(); } 
      else { usb_hid_set_idle(); }
      break;
    case HID_GET_IDLE:                  /*»ñµÃÏìÓ¦¼ä¸ôÇëÇó*/
      usb_hid_get_idle();
      break;
    case SET_DESCRIPTOR:
    case SET_INTERFACE:
    case SYNCH_FRAME:
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
////////////*get_descriptor*///////////////
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
    case DEVICE:                            /*Éè±¸ÃèÊö·û*/                
    {
      data_to_transfer = sizeof (usb_device_descriptor);
      pbuffer = &(usb_device_descriptor.bLength);//Ö¸ÏòÉè±¸ÃèÊö·û½á¹¹Ê×µØÖ·
      break;
    }

    case CONFIGURATION:                       /*ÅäÖÃ*/
    {
      data_to_transfer = sizeof (usb_configuration);
      pbuffer = &(usb_configuration.cfg.bLength);
      break;
    }

    case REPORT:                            /*±¨±í*/
    {
      data_to_transfer = 0x3b;
      pbuffer = &(usb_configuration.rep[0]);
      break;
    }

    case HID:                              /*HID*/
    {
      data_to_transfer = sizeof(usb_configuration.hid);
      pbuffer = &(usb_configuration.hid.bLength);
      break;
    }
    case STRING:                          /*STRING*/
    {
      switch (string_type)
      {
        case 0x00:
        {
          data_to_transfer = sizeof (usb_language);
          pbuffer = &(usb_language.bLength);
          break;
        }
        case 0x01:
        {
          data_to_transfer = sizeof (usb_manufacturer);
          pbuffer = &(usb_manufacturer.bLength);
          break;
        }
        case 0x02:
        {
          data_to_transfer = sizeof (usb_product);
          pbuffer = &(usb_product.bLength);
          break;
        }
        case 0x03:
        {
          data_to_transfer = sizeof (usb_serial_number);
          pbuffer = &(usb_serial_number.bLength);
          break;
        }
        default:
        {
          UEPSTAX =UEPSTAX & ~0x04;
          UEPSTAX=UEPSTAX | 0x20; 
          while ((!(UEPSTAX & 0x08)) && (UEPSTAX & 0x04));
          UEPSTAX &= ~0x08;
          UEPSTAX &= ~0x20;
          UEPSTAX &= ~0x80;
          return;
        }
      }
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
  if (wLength > data_to_transfer)     /* ¶ÁÈ¡µÄ³¤¶È´óÓÚÃèÊö·û³¤¶ÈÊ± */      
  {
                   
  }
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
//////-----void usb_get_configuration (void)------////////
void usb_get_configuration (void)
{
  UEPSTAX &= ~0x04;
  UEPSTAX |= 0x80;
  UEPDATX = 0;
  UEPSTAX |= 0x10;
  while (!(UEPSTAX & 0x01));
  UEPSTAX=UEPSTAX & ~0x01;
  while (!(UEPSTAX & 0x42));
  UEPSTAX &= ~0x02;
  UEPSTAX &= ~0x80;
}
/////////////----set_address-----//////////
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
////-----usb_hid_set_report (void)------//////
void usb_hid_set_report (void)/*ÉèÖÃ±¨±íÇëÇó³ÌÐò*/
{
  UEPSTAX &= ~0x04;
  while(!(UEPSTAX & 0x42));
  UEPSTAX &= ~0x02;
  UEPSTAX |= 0x10;                         
  while(!(UEPSTAX & 0x01));
  UEPSTAX &= ~0x01;
}
//////-----usb_hid_set_idle----///////
void usb_hid_set_idle (void)
{
  ACC = UEPDATX;
  hid_idle_duration = UEPDATX;      /* wValue contains the duration */
  UEPSTAX &= ~0x04;
  UEPSTAX |= 0x10;                          /* send a ZLP for STATUS phase */
  while(!(UEPSTAX & 0x01));
  UEPSTAX &= ~0x01;
}
/////---usb_hid_get_idle--- /////////
void usb_hid_get_idle (void)
{
  UEPSTAX &= ~0x04;
  UEPSTAX |= 0x80;
  UEPDATX = hid_idle_duration;
  UEPSTAX |= 0x10;                          /* send a ZLP for STATUS phase */
  while(!(UEPSTAX & 0x01));
  UEPSTAX &= ~0x01;
  
  while (!(UEPSTAX & 0x42));
  UEPSTAX &= ~0x02;
  UEPSTAX &= ~0x80;
}
////////---usb_kbd_task-----//////
void usb_kbd_task (void)
{
  if ((!key_hit)&&(usb_configuration_nb != 0) && !(USBINT & 0x01))
  {
    kbd_test_hit();//¼üÅÌÉ¨Ãè×Ó³ÌÐò
	if (key_hit == 1)//ÓÐ°´¼ü°´ÏÂ
    {
      transmit_no_key = 1;
	  /*ÐÎ³É¼üÂë±¨±í*/
      UEPNUM = 1;
      UEPDATX = 0;
      UEPDATX = shift_key;
      UEPDATX = usb_key;
      UEPDATX = 0;
      UEPDATX = 0;
      UEPDATX = 0;
      UEPDATX = 0;								
      UEPDATX = 0;
	  //ÉèÖÃ·¢ËÍ×¼±¸ºÃ±êÖ¾Î»
      UEPSTAX |= 0x10;
	  return;
    }

  if (transmit_no_key)//Ã»ÓÐ°´¼ü°´ÏÂÊ±´«ËÍÈ«ÁãÊý¾Ý±¨±í
    {
      key_hit = 1;
      transmit_no_key = 0;
       UEPNUM = 1;
      UEPDATX = 0;
      UEPDATX = 0;
      UEPDATX = 0;
      UEPDATX = 0;
      UEPDATX = 0;
      UEPDATX = 0;
      UEPDATX = 0;								
      UEPDATX = 0;
	  //ÉèÖÃ·¢ËÍ×¼±¸ºÃ±êÖ¾Î»
      UEPSTAX |= 0x10;
    }
	
}
}
////------test kbd------/////
void kbd_test_hit(void)
{
  /*³õÊ¼»¯¼üÅÌ*/
  P1_0=0;
  P1_1=0;
  P1_2=0;
  P1_3=0;
  KBLS = 0x00;
 /*ÅÐ¶ÏÓÐÃ»ÓÐ°´¼ü°´ÏÂ*/
  if((KBF & 0xF0) != 0x00)
 {
  delay(100);//È¥¶¶¶¯
  if(!((KBF & 0xF0) != 0x00))return;//ÔÙÅÐ¶ÏÓÐÃ»ÓÐ°´¼ü°´ÏÂ
  scan();//É¨Ãè¾ØÕó¼üÅÌ
  findkey();//»ñµÃ°´¼ü¶ÔÓ¦µÄHIDÂë
 }
}
//////----scan------//////
void scan(void)//¼üÅÌÉ¨Ãè×Ó³ÌÐò
{  
    unsigned char b,c;
    P1_0 = 1;
    P1_1 = 1;
    P1_2 =1;
    P1_3 = 1;
    KBF = 0x00;
    P1_0 = 0;             // É¨ÃèÁÐ1 
    b = KBF & 0xF0;
    P1_0 =1;
    KBF = 0x00;
    P1_1 = 0;             //É¨ÃèÁÐ2 
    b |= KBF & 0xF0 >> 4;
    P1_1 = 1;
    KBF = 0x00;
  
    P1_2 = 0;              //É¨ÃèÁÐ3 
    c = KBF & 0xF0;
    P1_2 = 1;
    KBF = 0x00;
  
    P1_3 = 0;             //É¨ÃèÁÐ4 
    c |= KBF & 0xF0 >> 4;
    P1_3 = 1;
    KBF = 0x00;
  
    scan_key.b[0]=b;
    scan_key.b[1]=c;
  
    key_hit = 1;
  
    P1_0 = 0;
    P1_1 = 0;
    P1_2 = 0;
    P1_3 = 0; 
}

void findkey()//»ñµÃ¼üÂë
{

if(scan_key.b[0]==0xfe)usb_key=key[1][2];
if(scan_key.b[0]==0xfd)usb_key=key[2][2];
if(scan_key.b[0]==0xfb)usb_key=key[3][2];
if(scan_key.b[0]==0xf7)usb_key=key[4][2];
if(scan_key.b[0]==0xef)usb_key=key[1][1];
if(scan_key.b[0]==0xdf)usb_key=key[2][1];
if(scan_key.b[0]==0xbf)usb_key=key[3][1];
if(scan_key.b[0]==0x7f)usb_key=key[4][1];

if(scan_key.b[1]==0xfe)usb_key=key[1][4];
if(scan_key.b[1]==0xfd)usb_key=key[2][4];
if(scan_key.b[1]==0xfb)usb_key=key[3][4];
if(scan_key.b[1]==0xf7)usb_key=key[4][4];
if(scan_key.b[1]==0xef)usb_key=key[1][3];
if(scan_key.b[1]==0xdf)usb_key=key[2][3];
if(scan_key.b[1]==0xbf)usb_key=key[3][3];
if(scan_key.b[1]==0x7f)shift_key=0x02;
}
////////////////////////
unsigned char * usb_send_ep0_packet (unsigned char* tbuf, unsigned char data_length)
{
unsigned char i;

  UEPNUM=0;
  for (i = data_length; i != 0 ; i--, tbuf++)
  { 
  UEPDATX=*tbuf; 
  }
  UEPSTAX |= 0x10;
  return tbuf;
}

