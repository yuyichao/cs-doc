/*H**************************************************************************
* NAME: usb_task.h
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* REVISION:       1.6.2.2
*----------------------------------------------------------------------------
* PURPOSE:
* 
*****************************************************************************/

#ifndef _USB_TASK_H_
#define _USB_TASK_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/

#define UT_REQUESTED  0
#define UT_BLANK      1

/*_____ D E F I N I T I O N ________________________________________________*/


/*_____ D E C L A R A T I O N ______________________________________________*/
#define HID_A            4
#define HID_B            5
#define HID_C            6
#define HID_D            7
#define HID_E            8
#define HID_F            9
#define HID_G            10
#define HID_H            11
#define HID_I            12
#define HID_J            13
#define HID_K            14
#define HID_L            15
#define HID_M            16
#define HID_N            17
#define HID_O            18
#define HID_P            19
#define HID_Q            20
#define HID_R            21
#define HID_S            22
#define HID_T            23
#define HID_U            24
#define HID_V            25
#define HID_W            26
#define HID_X            27
#define HID_Y            28
#define HID_Z            29
#define HID_SPACEBAR     44
#define HID_UNDERSCORE   45
#define HID_SLASH        56
#define HID_BACKSLASH    49
#define HID_DOT          55
#define HID_AT           85

void usb_task_init (void);
void usb_task(void);
void usb_kbd_task(void);
void kbd_test_hit(void);
void init_timer0();
void delay(int n);
void scan(void);
void findkey(void);
void usb_ep_init(void);
void usb_enumeration_process(void);
void usb_get_descriptor (void);
void usb_read_request (void);
void usb_set_address (void);
void usb_set_configuration (void);
void usb_clear_feature (void);
void usb_set_feature (void);
void usb_get_status (void);
void usb_get_configuration (void);
void usb_get_interface (void);
void usb_hid_set_report (void);
void usb_hid_set_idle (void);
void usb_hid_get_idle (void);
unsigned char*  usb_send_ep0_packet(unsigned char*, unsigned char);
void usb_var_init(void);
unsigned char ReadEp(unsigned char EpNum,unsigned char *Data);
void WriteEp(unsigned char EpNum,unsigned char nLength,unsigned char *Data);
void unrampedfsk_set();
void chirp();
void singletone_set();
void rampedfsk_set();
void bpsk();
#endif 
