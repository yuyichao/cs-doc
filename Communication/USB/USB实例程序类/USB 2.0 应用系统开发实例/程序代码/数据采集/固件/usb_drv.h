/*H**************************************************************************
* NAME:         usb_drv.h         
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5131-usb-kbd-light-1_0_2      
* REVISION:     1.4     
*----------------------------------------------------------------------------
* PURPOSE: 
* This file contains the USB low level driver definition                                      
*****************************************************************************/

#ifndef _USB_DRV_H_
#define _USB_DRV_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/


/*F**************************************************************************
* NAME: CONTROL / BULK / INTERRUPT / ISOCHRONOUS
*----------------------------------------------------------------------------
* PURPOSE: 
* These define are the values used to enable and configure an endpoint.
* They are written in the UEPCONX register.
*****************************************************************************/

#define CONTROL              0x80
#define BULK_IN              0x86
#define BULK_OUT             0x82
#define INTERRUPT_IN         0x87
#define INTERRUPT_OUT        0x83
#define ISOCHRONOUS_IN       0x85
#define ISOCHRONOUS_OUT      0x81
#define BULK_MAX_SIZE        ((Uchar)64)

/*M**************************************************************************
* NAME: wSWAP 
*----------------------------------------------------------------------------
* PARAMS:
* x     : (Uint16) the 16 bit word to swap 
* return: (Uint16) the 16 bit word x with the 2 bytes swaped  
*----------------------------------------------------------------------------
* PURPOSE: 
* This macro swaps the Uchar order in words.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define wSWAP(x) ((((x)>>8)&0x00FF)|(((x)<<8)&0xFF00)) ;


/*M**************************************************************************
* NAME: dwSWAP 
*----------------------------------------------------------------------------
* PARAMS:
* x     : (Uint32) the 32 bit double word to swap 
* return: (Uint32) the 32 bit double word x with the 4 bytes swaped  
*----------------------------------------------------------------------------
* PURPOSE: 
* This macro swaps the Uchar order in double words.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define dwSWAP(x) ((((x)>>24)&0x000000FF)|(((x)>>8)&0x0000FF00)|(((x)<<24)&0xFF000000)|(((x)<<8)&0x00FF0000))


/*M**************************************************************************
* NAME: General endpoint management 
*----------------------------------------------------------------------------
* PARAMS:
*----------------------------------------------------------------------------
* PURPOSE: 
* These macros manage the common features of the endpoints
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define Usb_select_ep(e)              (UEPNUM = e)
#define Usb_configure_ep_type(x)      (UEPCONX = x)
#define Usb_set_stall_request()       (UEPSTAX |= MSK_STALLRQ)
#define Usb_clear_stall_request()     (UEPSTAX &= ~MSK_STALLRQ)
#define Usb_clear_stalled()           (UEPSTAX &= ~MSK_STALLED)
#define Usb_stall_requested()         (UEPSTAX & MSK_STALLRQ)
#define Usb_stall_sent()              (UEPSTAX & MSK_STALLED)
#define Usb_read_byte()               (UEPDATX)
#define Usb_write_byte(x)             (UEPDATX = x)
#define Usb_endpoint_interrupt()      (UEPINT != 0 )


/*M**************************************************************************
* NAME: OUT endpoint management 
*----------------------------------------------------------------------------
* PARAMS:
*----------------------------------------------------------------------------
* PURPOSE: 
* These macros manage the OUT endpoints.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define Usb_clear_rx()                (UEPSTAX &= ~MSK_RXOUT)
#define Usb_clear_rx_bank0()          (UEPSTAX &= ~MSK_RXOUTB0)
#define Usb_clear_rx_bank1()          (UEPSTAX &= ~MSK_RXOUTB1)
#define Usb_rx_complete()             (UEPSTAX & MSK_RXOUTB0B1)
unsigned char ReadEp(unsigned char EpNum,unsigned char *Data);
void WriteEp(unsigned char EpNum,unsigned char nLength,unsigned char *Data);
void EpEnable(void);
/*M**************************************************************************
* NAME: IN endpoint management 
*----------------------------------------------------------------------------
* PARAMS:
*----------------------------------------------------------------------------
* PURPOSE: 
* These macros manage the IN endpoints.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define Usb_set_tx_ready()            (UEPSTAX |= MSK_TXRDY)
#define Usb_clear_tx_ready()          (UEPSTAX &= ~MSK_TXRDY)
#define Usb_clear_tx_complete()       (UEPSTAX &= ~MSK_TXCMPL)
#define Usb_tx_complete()             (UEPSTAX & MSK_TXCMPL)
#define Usb_tx_ready()                (UEPSTAX & MSK_TXRDY)


/*M**************************************************************************
* NAME: CONTROL endpoint management 
*----------------------------------------------------------------------------
* PARAMS:
*----------------------------------------------------------------------------
* PURPOSE: 
* These macros manage the Control endpoints.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define Usb_clear_rx_setup()          (UEPSTAX &= ~MSK_RXSETUP)
#define Usb_setup_received()          (UEPSTAX & MSK_RXSETUP)
#define Usb_clear_DIR()               (UEPSTAX &= ~MSK_DIR)
#define Usb_set_DIR()                 (UEPSTAX |= MSK_DIR)


/*M**************************************************************************
* NAME: General USB management 
*----------------------------------------------------------------------------
* PARAMS:
*----------------------------------------------------------------------------
* PURPOSE: 
* These macros manage the USB controller.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define Usb_enable()                  (USBCON |= MSK_USBE)
#define Usb_disable()                 (USBCON &= ~MSK_USBE)
#define Usb_detach()                  (USBCON |= MSK_DETACH)
#define Usb_attach()                  (USBCON &= ~MSK_DETACH)
#define Usb_clear_reset()             (USBINT &= ~MSK_EORINT)
#define Usb_clear_resume()            (USBINT &= ~MSK_WUPCPU)
#define Usb_clear_sof()               (USBINT &= ~MSK_SOFINT)
#define Usb_clear_suspend()           (USBINT &= ~MSK_SPINT)
#define Usb_suspend()                 (USBINT & MSK_SPINT)
#define Usb_resume()                  (USBINT & MSK_WUPCPU)
#define Usb_reset()                   (USBINT & MSK_EORINT)
#define Usb_sof()                     (USBINT & MSK_SOFINT)
#define Usb_configure_address(x)      (USBADDR = (0x80 | x))
#define Usb_set_CONFG()               (USBCON |= MSK_CONFG)
#define Usb_clear_CONFG()             (USBCON &= ~MSK_CONFG)
#define Usb_set_FADDEN()              (USBCON |= MSK_FADDEN)
#define Usb_clear_FADDEN()            (USBCON &= ~MSK_FADDEN)

/*M**************************************************************************
* NAME: USB interrupt management 
*----------------------------------------------------------------------------
* PARAMS:
*----------------------------------------------------------------------------
* PURPOSE: 
* These macros manage the USB controller.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/

#define Usb_enable_int()              (IEN1 |= MSK_EUSB)
#define Usb_disable_int()             (IEN1 &= ~MSK_EUSB)

#define Usb_enable_reset_int()        (USBIEN |= MSK_EEORINT)
#define Usb_enable_resume_int()       (USBIEN |= MSK_EWUPCPU)
#define Usb_enable_sof_int()          (USBIEN |= MSK_ESOFINT)
#define Usb_enable_suspend_int()      (USBIEN |= MSK_ESPINT)
#define Usb_disable_reset_int()       (USBIEN &= ~MSK_EEORINT)
#define Usb_disable_resume_int()      (USBIEN &= ~MSK_EWUPCPU)
#define Usb_disable_sof_int()         (USBIEN &= ~MSK_ESOFINT)
#define Usb_disable_suspend_int()     (USBIEN &= ~MSK_ESPINT)

#define Usb_enable_ep_int(e)          {UEPIEN |= (0x01 << e))
#define Usb_disable_ep_int(e)         {UEPIEN &= ~(0x01 << e))



/*M**************************************************************************
* NAME: USB clock management 
*----------------------------------------------------------------------------
* PARAMS:
*----------------------------------------------------------------------------
* PURPOSE: 
* These macros manage the USB clock.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
#define Usb_set_EXT48()               (PLLCON |= MSK_EXT48)
#define Usb_clear_EXT48()             (PLLCON &= ~MSK_EXT48)

#define Pll_stop()                    (PLLCON &= ~MSK_PLLEN)
#define Pll_set_div(n)                (PLLDIV = n)
#define Pll_enable()                  (PLLCON |= MSK_PLLEN) 



/*_____ D E C L A R A T I O N ______________________________________________*/

void    usb_configure_endpoint       (Uchar , Uchar);
Uchar   usb_get_nb_byte              (void);
Uint16  usb_get_nb_byte_epw          (void);
Uchar*  usb_send_ep0_packet          (Uchar*, Uchar);
Uchar*  usb_send_packet              (Uchar , Uchar*, Uchar);
Uchar*  usb_read_packet              (Uchar , Uchar*, Uchar);
void    usb_reset_endpoint           (Uchar);
Uchar   usb_select_enpoint_interrupt (void);
void    usb_halt_endpoint            (Uchar);
void    configure_usb_clock          (void);



#endif  /* _USB_DRV_H_ */

