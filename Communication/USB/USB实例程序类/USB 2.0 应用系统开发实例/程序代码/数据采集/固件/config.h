/*H**************************************************************************
* NAME:         config.h         
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5131-usb-kbd-light-1_0_2      
* FILE_REV:     1.5.2.5     
*----------------------------------------------------------------------------
* PURPOSE: 
* Describes the system dependant software configuration.
* This file is included by all source files in order to access to system wide
* configuration.
*****************************************************************************/

#ifndef _CONFIG_H_
#define _CONFIG_H_

/*_____ I N C L U D E S ____________________________________________________*/
#include "reg_5131.h"
#include "ext_5131.h"
#include "5131_drv.h"
#define FOSC                    16000
#define FILE_BOARD_H            "c5131_evab.h"
#define CPUB_VERSION            0x0102

#define TWI_SCAL                120 //120, 160, 60 //dont work with 256, 224, 192, 960
#define SDA_SOFT                GENEB_SDA
#define SCL_SOFT                GENEB_SCL

#define ENABLE_SPLASH_SCREEN      // Splash screen is first screen show after reset
#define LOGO_ATMEL                // ASCII art of Logo ATMEL
/* USB Configuration */
                                    /* DEVICE DESCRIPTOR */
#define USB_SPECIFICATION     0x1001
#define DEVICE_CLASS          0
#define DEVICE_SUB_CLASS      0
#define DEVICE_PROTOCOL       0
#define EP_CONTROL_LENGTH     32
#define VENDOR_ID             0xEB03        /* Atmel vendor ID = 03EBh */
#define PRODUCT_ID            0x0320        /* Product ID: 2003h = HID keyboard */
#define RELEASE_NUMBER        0x0001
#define MAN_INDEX             0x01
#define PROD_INDEX            0x02
#define SN_INDEX              0x03
#define NB_CONFIGURATION      1

                                    /* CONFIGURATION DESCRIPTOR */
#define CONF_LENGTH           0x2200
#define NB_INTERFACE          1
#define CONF_NB               1
#define CONF_INDEX            0
#define CONF_ATTRIBUTES       USB_CONFIG_BUSPOWERED
#define MAX_POWER             50          /* 100 mA */
                                    /* INTERFACE DESCRIPTOR */
#define INTERFACE_NB          0
#define ALTERNATE             0
#define NB_ENDPOINT           1
#define INTERFACE_CLASS       0x03
#define INTERFACE_SUB_CLASS   0
#define INTERFACE_PROTOCOL    0
#define INTERFACE_INDEX       0
                                    /* ENDPOINT 1 DESCRIPTOR */
#define ENDPOINT_NB_1         ENDPOINT_1
#define EP_ATTRIBUTES_1       0x03
#define EP_SIZE_1             ((Uint16)EP_IN_LENGTH) << 8 
#define EP_INTERVAL_1         0x20

#define EP_CONTROL            0x00
#define EP_IN                 0x01
#define EP_KBD_IN             EP_IN
#define EP_IN_LENGTH          8
#define ENDPOINT_0            0x00
#define ENDPOINT_1            0x81

#endif /* _CONFIG_H_ */


#define GET_STATUS            0x00
#define GET_DEVICE            0x01
#define CLEAR_FEATURE         0x01        /* see FEATURES below */
#define GET_STRING            0x03
#define SET_FEATURE           0x03        /* see FEATURES below */
#define SET_ADDRESS           0x05
#define GET_DESCRIPTOR        0x06
#define SET_DESCRIPTOR        0x07
#define GET_CONFIGURATION     0x08
#define SET_CONFIGURATION     0x09
#define GET_INTERFACE         0x0A
#define SET_INTERFACE         0x0B
#define SYNCH_FRAME           0x0C

#define GET_DEVICE_DESCRIPTOR           1
#define GET_CONFIGURATION_DESCRIPTOR    4
#define REQUEST_DEVICE_STATUS         0x80
#define REQUEST_INTERFACE_STATUS      0x81
#define REQUEST_ENDPOINT_STATUS       0x82
#define ZERO_TYPE                     0x00
#define INTERFACE_TYPE                0x01
#define ENDPOINT_TYPE                 0x02
#define DEVICE                0x01
#define CONFIGURATION         0x02
#define STRING                0x03
#define INTERFACE             0x04
#define ENDPOINT              0x05
#define Usb_set_tx_ready()            (UEPSTAX |= MSK_TXRDY)
#define Usb_clear_tx_ready()          (UEPSTAX &= ~MSK_TXRDY)
#define Usb_clear_tx_complete()       (UEPSTAX &= ~MSK_TXCMPL)
#define Usb_tx_complete()             (UEPSTAX & MSK_TXCMPL)
#define Usb_tx_ready()                (UEPSTAX & MSK_TXRDY)
#define Usb_write_byte(x)             (UEPDATX = x)
unsigned char ReadEp(unsigned char EpNum,unsigned char *Data);
void WriteEp(unsigned char EpNum,unsigned char nLength,unsigned char *Data);
void EpEnable(void);
unsigned char*  usb_send_ep0_packet          (unsigned char*, unsigned char);

void    usb_var_init(void);
void    usb_ep_init(void);
void    usb_enumeration_process(void);
void usb_task_init     (void);
void usb_task          (void);
void caiji();
void    usb_read_request (void);
void    usb_get_descriptor (void);
void    usb_set_address (void);
void    usb_set_configuration (void);
void    usb_get_configuration (void);
void data_deal(void);
void EpEnable(void);
void data_init(void);
unsigned int caiji_single();
void delay(int mm);
void senddata();

