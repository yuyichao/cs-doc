/*H***************************************************************************
* NAME:             ext_5131.h
*-----------------------------------------------------------------------------
* $AUTHOR:          $
* $REVISION:        1.0 $
* $DATE:            $
*-----------------------------------------------------------------------------
* PURPOSE:          This file is an extension to the reg_5131.h file.
*                   It defines mask for registers.
*-----------------------------------------------------------------------------
* CHANGES:
*
******************************************************************************/

#ifndef _EXT5131_H_
#define _EXT5131_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/

/* INTERRUPT NUMBER */

#define IRQ_INT0    0
#define IRQ_TIMER0  1
#define IRQ_T0      1
#define IRQ_INT1    2
#define IRQ_TIMER1  3
#define IRQ_T1      3
#define IRQ_UART    4
#define IRQ_TIMER2  5
#define IRQ_T2      5
#define IRQ_PCA     6
#define IRQ_KBD     7
#define IRQ_TWI     8
#define IRQ_SPI     9
#define IRQ_USB    13

/* USB */

#define MSK_TXCMPL    0x01  /* UEPSTAX */
#define MSK_RXOUTB0   0x02
#define MSK_RXOUT     0x02
#define MSK_RXSETUP   0x04
#define MSK_STALLED   0x08
#define MSK_TXRDY     0x10
#define MSK_STALLRQ   0x20
#define MSK_RXOUTB1   0x40
#define MSK_DIR       0x80
#define MSK_RXOUTB0B1 0x42
#define MSK_EP_DIR    0x7F


#define MSK_SPINT     0x01  /* USBINT */
#define MSK_SOFINT    0x08
#define MSK_EORINT    0x10
#define MSK_WUPCPU    0x20

#define MSK_ESPINT    0x01  /* USBIEN */
#define MSK_ESOFINT   0x08
#define MSK_EEORINT   0x10
#define MSK_EWUPCPU   0x20

#define MSK_USBE      0x80  /* USBCON */
#define MSK_SUSPCLK   0x40
#define MSK_SDRMWUP   0x20
#define MSK_DETACH    0x10
#define MSK_UPRSM     0x08
#define MSK_RMWUPE    0x04
#define MSK_CONFG     0x02
#define MSK_FADDEN    0x01

#define MSK_FEN       0x80  /* USBADDR */

#define MSK_EPEN      0x80  /* UEPCONX */
#define MSK_DTGL      0x08
#define MSK_EPDIR     0x04
#define MSK_EPTYPE1   0x02
#define MSK_EPTYPE0   0x01

#define MSK_EP6RST    0x40  /* UEPRST */
#define MSK_EP5RST    0x20
#define MSK_EP4RST    0x10
#define MSK_EP3RST    0x08
#define MSK_EP2RST    0x04
#define MSK_EP1RST    0x02
#define MSK_EP0RST    0x01

#define MSK_EP6INTE    0x40  /* UEPIEN */
#define MSK_EP5INTE    0x20
#define MSK_EP4INTE    0x10
#define MSK_EP3INTE    0x08
#define MSK_EP2INTE    0x04
#define MSK_EP1INTE    0x02
#define MSK_EP0INTE    0x01

#define MSK_CRCOK      0x20  /* UFNUMH */
#define MSK_CRCERR     0x10

/* SYSTEM MANAGEMENT */

#define MSK_SMOD1   0x80    /* PCON */
#define MSK_SMOD0   0x40
#define MSK_POF     0x10
#define MSK_GF1     0x08
#define MSK_GF0     0x04
#define MSK_PD      0x02
#define MSK_IDL     0x01

#define MSK_DPU     0x80    /* AUXR0 */
#define MSK_M0      0x20
#define MSK_DPHDIS  0x10
#define MSK_XRS     0x0C
#define MSK_EXTRAM  0x02
#define MSK_AO      0x01

#define MSK_ENBOOT  0x20    /* AUXR1 */
#define MSK_GF3     0x08
#define MSK_DPS     0x01


/* PLL & CLOCK */

#define MSK_TWIX2   0x80    /* CKCON0 */
#define MSK_WDX2    0x40
#define MSK_PCAX2   0x20
#define MSK_SIX2    0x10
#define MSK_T2X2    0x08
#define MSK_T1X2    0x04
#define MSK_T0X2    0x02
#define MSK_X2      0x01

#define MSK_SPIX2   0x01    /* CKCON1 */

#define MSK_PLOCK   0x01    /* PLLCON */
#define MSK_PLLEN   0x02
#define MSK_EXT48   0x04

#define MSK_R       0xF0    /* PLLDIV */
#define MSK_N       0x0F


/* INTERRUPT */

#define MSK_EC      0x40    /* IEN0 */
#define MSK_ET2     0x20
#define MSK_ES      0x10
#define MSK_ET1     0x08
#define MSK_EX1     0x04
#define MSK_ET0     0x02
#define MSK_EX0     0x01

#define MSK_EUSB    0x40    /* IEN1 */
#define MSK_ESPI    0x04
#define MSK_ETWI    0x02
#define MSK_EKB     0x01


/* TIMERS */

#define MSK_GATE1   0x80    /* TMOD */
#define MSK_C_T1    0x40
#define MSK_MO1     0x30
#define MSK_GATE0   0x08
#define MSK_C_T0    0x04
#define MSK_MO0     0x03


/* WATCHDOG */

#define MSK_WTO     0x07    /* WDTPRG*/




/* SPI CONTROLLER */

#define MSK_SPR     0x83    /* SPCON */
#define MSK_SPEN    0x40
#define MSK_SSDIS   0x20
#define MSK_MSTR    0x10
#define MSK_MODE    0x0C
#define MSK_CPOL    0x08
#define MSK_CPHA    0x04

#define MSK_SPIF    0x80    /* SPSTA */
#define MSK_WCOL    0x40
#define MSK_MODF    0x10


/* TWI CONTROLLER */

#define MSK_SSCR    0x83    /* SSCON */
#define MSK_SSPE    0x40
#define MSK_SSSTA   0x20
#define MSK_SSSTO   0x10
#define MSK_SSSI    0x08
#define MSK_SSAA    0x04


/* FLASH */

#define MSK_FCON_FBUSY   0x01



#endif  /* _EXT5131_H_ */
