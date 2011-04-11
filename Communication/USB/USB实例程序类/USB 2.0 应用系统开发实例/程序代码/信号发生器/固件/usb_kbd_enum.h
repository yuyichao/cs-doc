/*H**************************************************************************
* NAME:         usb_kbd_enum.h
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5131-usb-kbd-light-1_0_2      
* REVISION:     1.2     
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the USB task definition
*****************************************************************************/

#ifndef _USB_ENUM_H_
#define _USB_ENUM_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/


/* HID specific */
#define GET_REPORT_DESCRIPTOR 0x22
/* *** */

/*_____ S T A N D A R D    R E Q U E S T S __________________________________*/

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

/* HID specific */
#define HID_SET_IDLE          0x0A
#define HID_GET_IDLE          0x02
/* *** */

#define REQUEST_DEVICE_STATUS         0x80
#define REQUEST_INTERFACE_STATUS      0x81
#define REQUEST_ENDPOINT_STATUS       0x82
#define ZERO_TYPE                     0x00
#define INTERFACE_TYPE                0x01
#define ENDPOINT_TYPE                 0x02

/*_____ D E S C R I P T O R    T Y P E S ____________________________________*/

#define DEVICE                0x01
#define CONFIGURATION         0x02
#define STRING                0x03
#define INTERFACE             0x04
#define ENDPOINT              0x05

/* HID specific */
#define HID                   0x21
#define REPORT                0x22
/* *** */

/*_____ S T A N D A R D    F E A T U R E S __________________________________*/

#define DEVICE_REMOTE_WAKEUP_FEATURE     0x01
#define ENDPOINT_HALT_FEATURE            0x00

/*_____ D E V I C E   S T A T U S ___________________________________________*/

#define SELF_POWERED       1

/*_____ D E V I C E   S T A T E _____________________________________________*/

#define ATTACHED                  0
#define POWERED                   1
#define DEFAULT                   2
#define ADDRESSED                 3
#define CONFIGURED                4
#define SUSPENDED                 5

#define USB_CONFIG_BUSPOWERED     0x80
#define USB_CONFIG_SELFPOWERED    0x40
#define USB_CONFIG_REMOTEWAKEUP   0x20

/*_________________________________________________________ S T R U C T _____*/
/*_____ U S B   D E V I C E   R E Q U E S T _________________________________*/
struct Endpoint_information_st
{
  unsigned int  fifo_size ;           /* size of the endpoint FIFO */
  unsigned int  fifo_left ;
};

struct USB_request_st
{
  unsigned char   bmRequestType;        /* Characteristics of the request */
  unsigned char   bRequest;             /* Specific request */
  unsigned int  wValue;               /* field that varies according to request */
  unsigned int  wIndex;               /* field that varies according to request */
  unsigned int  wLength;              /* Number of bytes to transfer if Data */
};


/*_____ U S B   D E V I C E   D E S C R I P T O R ___________________________*/

struct usb_st_device_descriptor
{
  unsigned char  bLength;               /* Size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* DEVICE descriptor type */
  unsigned int bscUSB;                /* Binay Coded Decimal Spec. release */
  unsigned char  bDeviceClass;          /* Class code assigned by the USB */
  unsigned char  bDeviceSubClass;       /* Sub-class code assigned by the USB */
  unsigned char  bDeviceProtocol;       /* Protocol code assigned by the USB */
  unsigned char  bMaxPacketSize0;       /* Max packet size for EP0 */
  unsigned int idVendor;              /* Vendor ID. ATMEL = 0x03EB */
  unsigned int idProduct;             /* Product ID assigned by the manufacturer */
  unsigned int bcdDevice;             /* Device release number */
  unsigned char  iManufacturer;         /* Index of manu. string descriptor */
  unsigned char  iProduct;              /* Index of prod. string descriptor */
  unsigned char  iSerialNumber;         /* Index of S.N.  string descriptor */
  unsigned char  bNumConfigurations;    /* Number of possible configurations */
};


/*_____ U S B   C O N F I G U R A T I O N   D E S C R I P T O R _____________*/

struct usb_st_configuration_descriptor
{
  unsigned char  bLength;               /* size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* CONFIGURATION descriptor type */
  unsigned int wTotalLength;          /* total length of data returned */
  unsigned char  bNumInterfaces;        /* number of interfaces for this conf. */
  unsigned char  bConfigurationValue;   /* value for SetConfiguration resquest */
  unsigned char  iConfiguration;        /* index of string descriptor */
  unsigned char  bmAttibutes;           /* Configuration characteristics */
  unsigned char  MaxPower;              /* maximum power consumption */
};


/*_____ U S B   I N T E R F A C E   D E S C R I P T O R _____________________*/

struct usb_st_interface_descriptor
{
  unsigned char bLength;                /* size of this descriptor in bytes */
  unsigned char bDescriptorType;        /* INTERFACE descriptor type */
  unsigned char bInterfaceNumber;       /* Number of interface */
  unsigned char bAlternateSetting;      /* value to select alternate setting */
  unsigned char bNumEndpoints;          /* Number of EP except EP 0 */
  unsigned char bInterfaceClass;        /* Class code assigned by the USB */
  unsigned char bInterfaceSubClass;     /* Sub-class code assigned by the USB */
  unsigned char bInterfaceProtocol;     /* Protocol code assigned by the USB */
  unsigned char iInterface;             /* Index of string descriptor */
};


/*_____ U S B   E N D P O I N T   D E S C R I P T O R _______________________*/

struct usb_st_endpoint_descriptor
{
  unsigned char  bLength;               /* Size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* ENDPOINT descriptor type */
  unsigned char  bEndpointAddress;      /* Address of the endpoint */
  unsigned char  bmAttributes;          /* Endpoint's attributes */
  unsigned int wMaxPacketSize;        /* Maximum packet size for this EP */
  unsigned char  bInterval;             /* Interval for polling EP in ms */
};


/*_____ U S B   M A N U F A C T U R E R   D E S C R I P T O R _______________*/

struct usb_st_manufacturer
{
  unsigned char  bLength;               /* size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* STRING descriptor type */
  unsigned int wstring[5];/* unicode characters */
};


/*_____ U S B   P R O D U C T   D E S C R I P T O R _________________________*/

struct usb_st_product
{
  unsigned char  bLength;               /* size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* STRING descriptor type */
  unsigned int wstring[24];/* unicode characters */
};


/*_____ U S B   S E R I A L   N U M B E R   D E S C R I P T O R _____________*/

struct usb_st_serial_number
{
  unsigned char  bLength;               /* size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* STRING descriptor type */
  unsigned int wstring[5];/* unicode characters */
};


/*_____ U S B   L A N G U A G E    D E S C R I P T O R ______________________*/

struct usb_st_language_descriptor
{
  unsigned char  bLength;               /* size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* STRING descriptor type */
  unsigned int wlangid;               /* language id */
};

/* HID specific */
/*_____ U S B   H I D   D E S C R I P T O R __________________________________*/

struct usb_st_hid_descriptor
{ 
  unsigned char  bLength;               /* Size of this descriptor in bytes */
  unsigned char  bDescriptorType;       /* HID descriptor type */
  unsigned int bscHID;                /* Binay Coded Decimal Spec. release */
  unsigned char  bCountryCode;          /* Hardware target country */
  unsigned char  bNumDescriptors;       /* Number of HID class descriptors to follow */
  unsigned char  bRDescriptorType;      /* Report descriptor type */
  unsigned int wDescriptorLength;     /* Total length of Report descriptor */
};


#endif  /* _USB_ENUM_H_ */
