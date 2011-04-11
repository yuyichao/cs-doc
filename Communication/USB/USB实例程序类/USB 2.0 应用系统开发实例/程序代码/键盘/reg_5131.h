/*H****************************************************************************
* NAME:           reg_5131.h
*------------------------------------------------------------------------------
* $AUTHOR:        Laurent Guilhaumon $
* $REVISION:      0.0 $
* $DATE:          22/02/2001 $
*------------------------------------------------------------------------------
* PURPOSE:
*   This file defines Sfr registers and BIT Registers for AT89C5131
*  (Keil notation is the reference)
******************************************************************************/

#ifndef _REG5131_H_
#define _REG5131_H_

/* 
** _____ H E A D E R S ________________________________________________________
*/

/* 
** _____ F U N C T I O N   D E F I N I T I O N ________________________________
*/
#define Sfr(x,y)      sfr x = y
#define Sfr16(x,y)  sfr16 x = y
#define Sbit(x,y,z)  sbit x = y ^ z


/* 
** _____ B Y T E   R E G I S T E R S __________________________________________
*/

/* _____ C 5 1   C O R E ______________________________________________________
*/
Sfr ( ACC   , 0xE0 ) ;        /* Sfr ( ACC, bit addressable)*/
Sfr ( B     , 0xF0 ) ;        /* Sfr ( B,   bit addressable)*/
Sfr ( PSW   , 0xD0 ) ;        /* Sfr ( PSW, bit addressable)*/
Sfr ( SP    , 0x81 ) ;
Sfr ( DPL   , 0x82 ) ;
Sfr ( DPH   , 0x83 ) ;

Sfr16 (DPTR, 0x82);

#define PM_PD   0x02    /* Power Down Mode */
#define PM_IDL  0x01    /* Idle Mode */

/* _____  I / O    P O R T ____________________________________________________
*/
Sfr ( P0    , 0x80 ) ;        /* Sfr ( P0, bit addressable)*/

Sbit ( P0_7 , P0 , 7 ) ;
Sbit ( P0_6 , P0 , 6 ) ;
Sbit ( P0_5 , P0 , 5 ) ;
Sbit ( P0_4 , P0 , 4 ) ;
Sbit ( P0_3 , P0 , 3 ) ;
Sbit ( P0_2 , P0 , 2 ) ;
Sbit ( P0_1 , P0 , 1 ) ;
Sbit ( P0_0 , P0 , 0 ) ;

Sfr ( P1    , 0x90 ) ;        /* Sfr ( P1, bit addressable)*/

Sbit ( P1_7 , P1 , 7 ) ;
Sbit ( P1_6 , P1 , 6 ) ;
Sbit ( P1_5 , P1 , 5 ) ;
Sbit ( P1_4 , P1 , 4 ) ;
Sbit ( P1_3 , P1 , 3 ) ;
Sbit ( P1_2 , P1 , 2 ) ;
Sbit ( P1_1 , P1 , 1 ) ;
Sbit ( P1_0 , P1 , 0 ) ;

Sfr ( P2    , 0xA0 ) ;        /* Sfr ( P2, bit addressable)*/

Sbit ( P2_7 , P2 , 7 ) ;
Sbit ( P2_6 , P2 , 6 ) ;
Sbit ( P2_5 , P2 , 5 ) ;
Sbit ( P2_4 , P2 , 4 ) ;
Sbit ( P2_3 , P2 , 3 ) ;
Sbit ( P2_2 , P2 , 2 ) ;
Sbit ( P2_1 , P2 , 1 ) ;
Sbit ( P2_0 , P2 , 0 ) ;

Sfr ( P3    , 0xB0 ) ;        /* Sfr ( P3, bit addressable)*/

Sbit ( P3_7 , P3 , 7 ) ;
Sbit ( P3_6 , P3 , 6 ) ;
Sbit ( P3_5 , P3 , 5 ) ;
Sbit ( P3_4 , P3 , 4 ) ;
Sbit ( P3_3 , P3 , 3 ) ;
Sbit ( P3_2 , P3 , 2 ) ;
Sbit ( P3_1 , P3 , 1 ) ;
Sbit ( P3_0 , P3 , 0 ) ;

Sfr ( P4    , 0xC0 ) ;        /* Sfr ( P4, bit addressable)*/

Sbit ( P4_1 , P4 , 1 ) ;
Sbit ( P4_0 , P4 , 0 ) ;


/* _____ T I M E R S __________________________________________________________
*/
Sfr ( TH0   , 0x8C ) ; 
Sfr ( TL0   , 0x8A ) ;
Sfr ( TH1   , 0x8D ) ;
Sfr ( TL1   , 0x8B ) ;
Sfr ( TH2   , 0xCD ) ;
Sfr ( TL2   , 0xCC ) ;
Sfr ( TCON  , 0x88 ) ;        /* Sfr ( TCON,  bit addressable)*/
Sfr ( TMOD  , 0x89 ) ;
Sfr ( T2CON , 0xC8 ) ;        /* Sfr ( T2CON, bit addressable)*/
Sfr ( T2MOD , 0xC9 ) ;
Sfr ( RCAP2H, 0xCB ) ;
Sfr ( RCAP2L, 0xCA ) ;
Sfr ( WDTRST, 0xA6 ) ;
Sfr ( WDTPRG, 0xA7 ) ;

                            /* TCON bits */
Sbit ( TF1   , TCON , 7 ) ;
Sbit ( TR1   , TCON , 6 ) ;
Sbit ( TF0   , TCON , 5 ) ;
Sbit ( TR0   , TCON , 4 ) ;
Sbit ( IE1_  , TCON , 3 ) ;
Sbit ( IT1   , TCON , 2 ) ;
Sbit ( IE0_  , TCON , 1 ) ;
Sbit ( IT0   , TCON , 0 ) ;
                            /* T2CON bits */
Sbit ( TF2   , T2CON , 7 ) ;
Sbit ( EXF2  , T2CON , 6 ) ;
Sbit ( RCLK  , T2CON , 5 ) ;
Sbit ( TCLK  , T2CON , 4 ) ;
Sbit ( EXEN2 , T2CON , 3 ) ;
Sbit ( TR2   , T2CON , 2 ) ;
Sbit ( C_T2  , T2CON , 1 ) ;
Sbit ( CP_RL2, T2CON , 0 ) ;


/* _____ S E R I A L    I / O _________________________________________________
*/
Sfr ( SCON  , 0x98 ) ;
Sfr ( SBUF  , 0x99 ) ;
Sfr ( SADEN , 0xB9 ) ;
Sfr ( SADDR , 0xA9 ) ;

Sbit ( FE_SM0, SCON , 7 ) ;
Sbit ( SM1   , SCON , 6 ) ;
Sbit ( SM2   , SCON , 5 ) ;
Sbit ( REN   , SCON , 4 ) ;
Sbit ( TB8   , SCON , 3 ) ;
Sbit ( RB8   , SCON , 2 ) ;
Sbit ( TI    , SCON , 1 ) ;
Sbit ( RI    , SCON , 0 ) ;

/* _____ B A U D    R A T E    G E N E R A T O R ______________________________
*/
Sfr ( BRL   , 0x9A ) ;
Sfr ( BDRCON, 0x9B ) ;

/* _____ P C A ________________________________________________________________
*/
Sfr ( CCON   , 0xD8 ) ;      /* Sfr ( CCON, bit addressable)*/
Sfr ( CMOD   , 0xD9 ) ;
Sfr ( CL     , 0xE9 ) ;
Sfr ( CH     , 0xF9 ) ;
Sfr ( CCAPM0 , 0xDA ) ;
Sfr ( CCAPM1 , 0xDB ) ;
Sfr ( CCAPM2 , 0xDC ) ;
Sfr ( CCAPM3 , 0xDD ) ;
Sfr ( CCAPM4 , 0xDE ) ;
Sfr ( CCAP0H , 0xFA ) ;
Sfr ( CCAP1H , 0xFB ) ;
Sfr ( CCAP2H , 0xFC ) ;
Sfr ( CCAP3H , 0xFD ) ;
Sfr ( CCAP4H , 0xFE ) ;
Sfr ( CCAP0L , 0xEA ) ;
Sfr ( CCAP1L , 0xEB ) ;
Sfr ( CCAP2L , 0xEC ) ;
Sfr ( CCAP3L , 0xED ) ;
Sfr ( CCAP4L , 0xEE ) ;

                             /* CCON bits */
Sbit ( CF    , CCON , 7 ) ;
Sbit ( CR    , CCON , 6 ) ;
Sbit ( CCF4  , CCON , 4 ) ;
Sbit ( CCF3  , CCON , 3 ) ;
Sbit ( CCF2  , CCON , 2 ) ;
Sbit ( CCF1  , CCON , 1 ) ;
Sbit ( CCF0  , CCON , 0 ) ;


/* _____ I N T E R R U P T ____________________________________________________
*/
Sfr ( IEN0  , 0xA8 ) ;       /* Sfr ( IE0,  bit addressable			*/
Sfr ( IEN1  , 0xB1 ) ;
Sfr ( IPL0  , 0xB8 ) ;       /* Sfr ( IPL0, bit addressable		*/
Sfr ( IPH0  , 0xB7 ) ;
Sfr ( IPL1  , 0xB2 ) ;
Sfr ( IPH1  , 0xB3 ) ;

                             /* IE0 bits */
Sbit ( EA   , IEN0 , 7 ) ;
Sbit ( EC   , IEN0 , 6 ) ;
Sbit ( ET2  , IEN0 , 5 ) ;
Sbit ( ES   , IEN0 , 4 ) ;
Sbit ( ET1  , IEN0 , 3 ) ;
Sbit ( EX1  , IEN0 , 2 ) ;
Sbit ( ET0  , IEN0 , 1 ) ;
Sbit ( EX0  , IEN0 , 0 ) ;
                             /* IPL0 bits */
Sbit ( PPCL , IPL0 , 6 ) ;
Sbit ( PT2L , IPL0 , 5 ) ;
Sbit ( PSL  , IPL0 , 4 ) ;
Sbit ( PTIL , IPL0 , 3 ) ;
Sbit ( PXIL , IPL0 , 2 ) ;
Sbit ( PT0L , IPL0 , 1 ) ;
Sbit ( PX0L , IPL0 , 0 ) ;

/* _____ P L L ________________________________________________________________
*/
Sfr ( PLLCON, 0xA3 ) ;
Sfr ( PLLDIV, 0xA4 ) ;

/* _____ K E Y B O A R D ______________________________________________________
*/
Sfr ( KBF   , 0x9E ) ;
Sfr ( KBE   , 0x9D ) ;
Sfr ( KBLS  , 0x9C ) ;

/* _____ T W I ________________________________________________________________
*/
Sfr ( SSCON , 0x93 ) ;
Sfr ( SSCS  , 0x94 ) ;
Sfr ( SSSTA , 0x94 ) ;
Sfr ( SSDAT , 0x95 ) ;
Sfr ( SSADR , 0x96 ) ;

/* _____ S P I ________________________________________________________________
*/
Sfr ( SPCON , 0xC3 ) ;
Sfr ( SPSTA , 0xC4 ) ;
Sfr ( SPDAT , 0xC5 ) ;

/* _____ U S B ________________________________________________________________
*/
Sfr ( USBCON , 0xBC ) ;
Sfr ( USBADDR, 0xC6 ) ;
Sfr ( USBINT , 0xBD ) ;
Sfr ( USBIEN , 0xBE ) ;
Sfr ( UEPNUM , 0xC7 ) ;
Sfr ( UEPCONX, 0xD4 ) ;
Sfr ( UEPSTAX, 0xCE ) ;
Sfr ( UEPRST , 0xD5 ) ;
Sfr ( UEPINT , 0xF8 ) ;         /* Sfr ( UEPINT, bit addressable) */
Sfr ( UEPIEN , 0xC2 ) ;
Sfr ( UEPDATX, 0xCF ) ;
Sfr ( UBYCTLX, 0xE2 ) ;
Sfr ( UBYCTHX, 0xE3 ) ;
Sfr ( UDPADDL, 0xD6 ) ;
Sfr ( UDPADDH, 0xD7 ) ;
Sfr ( UFNUML , 0xBA ) ;
Sfr ( UFNUMH , 0xBB ) ;
                                /* UEPINT bits */
Sbit ( EP6INT , UEPINT , 6 ) ;
Sbit ( EP5INT , UEPINT , 5 ) ;
Sbit ( EP4INT , UEPINT , 4 ) ;
Sbit ( EP3INT , UEPINT , 3 ) ;
Sbit ( EP2INT , UEPINT , 2 ) ;
Sbit ( EP1INT , UEPINT , 1 ) ;
Sbit ( EP0INT , UEPINT , 0 ) ;

/* _____ M I S C . ____________________________________________________________
*/
Sfr ( PCON   , 0x87 ) ;
Sfr ( AUXR   , 0x8E ) ;
Sfr ( AUXR1  , 0xA2 ) ;
Sfr ( CKCON0 , 0x8F ) ;
Sfr ( CKCON1 , 0xAF ) ;
Sfr ( CKSEL  , 0x85 ) ;
Sfr ( LEDCON , 0xF1 ) ;
Sfr ( FCON   , 0xD1 ) ;
Sfr ( EECON  , 0xD2 ) ;

#endif        /* _REG5131_H_ */


