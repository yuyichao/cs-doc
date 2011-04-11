/*C**************************************************************************
* NAME:         timer_soft.c
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5131-usb-kbd-light-1_0_2      
* REVISION:     1.8     
*----------------------------------------------------------------------------
* PURPOSE: 
* This file controls the temporizations of the application. 
* The tempo value is a define symbol in c51_time.h file.
* The oscillator value is a parameter in c51_time.h file.
*****************************************************************************/

/*_____ I N C L U D E S ____________________________________________________*/
#include "config.h"
#include "timer_soft.h"
#include "usb_drv.h"
long int buf2[4];
//int i;

/*_____ M A C R O S ________________________________________________________*/


/*_____ D E F I N I T I O N S ______________________________________________*/
/*V**************************************************************************
* NAME: gl_soft_timer_tick
*----------------------------------------------------------------------------
* PURPOSE: 
* Number of ms since mcu reset.
* This variable is used by soft timers, it must be updated periodically (usually
* by timer interrupt routine).
*****************************************************************************/
volatile Uint32 data gl_soft_timer_tick;

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*F**************************************************************************
* NAME: tempo
*----------------------------------------------------------------------------
* PARAMS:
* delay: tempo value in ms
* return:   none
*----------------------------------------------------------------------------
* PURPOSE: 
* This function manages the temporization.
*----------------------------------------------------------------------------
* EXAMPLE:
* tempo(TIMER_1_S); // Wait 1 sec.
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void tempo(Uint16 delay)
{
register Timer16s timer;

timer=set_timer16s(delay);
while (!std_timeout16s(timer));
}


#ifndef I_HAVE_SETUP_SOFT_TIMER_TICK
Uint16 gl_reminder_inc_ms;

#ifdef SOFT_TIMER_CUSTOM_FOSC
Uint16  REMINDER_INC_MS;

/*F**************************************************************************
* NAME: reload_TH0
*----------------------------------------------------------------------------
* PARAMS:
* return:   value to load on TH0
*----------------------------------------------------------------------------
* PURPOSE: 
* This function calculate the value to load in TH0 when FOSC is custom (FOSC
* is not a precomputed value).
* This function update REMINDER_INC_MS to keep accuracy of timers.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
Byte reload_TH0(void)
{
Byte b;
Byte n;

REMINDER_INC_MS=((Uint16)FOSC/(Uint16)12*(Uint16)INC_MS);
b=(REMINDER_INC_MS>>8);
     if (b&128) b=0;
else if (b&64)  b=128;
else if (b&32)  b=192;
else if (b&16)  b=224;
else if (b&8)   b=240;
else if (b&4)   b=248;
else if (b&2)   b=252;
else if (b&1)   b=254;
else            b=255;
REMINDER_INC_MS=((Uint16)b<<8)-REMINDER_INC_MS;
// Do REMINDER_INC_MS*12*65536/FOSC with the maximum accuracy<=>REMINDER_INC_MS*3*2^18/FOSC
for (n=0;n<18;n++)
  {
  if (REMINDER_INC_MS&32768)
    {
    REMINDER_INC_MS/=FOSC;
    REMINDER_INC_MS*=3;
    }
  else REMINDER_INC_MS<<=1;
  }
return b;
}
#endif


/*F**************************************************************************
* NAME: init_soft_timers
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* This function initialise the soft timer library.  
*----------------------------------------------------------------------------
* EXAMPLE:
* init_soft_timer();
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
void init_soft_timers (void)
{
gl_soft_timer_tick=0;
gl_reminder_inc_ms=0;
TL0 = 0;
TH0 = RELOAD_TH0;
TMOD |= 0x01; // timer0 is mode1 : 16 bits
Set_timer0_x1_mode(); // timer0 always in x1 mode, even if x2 mode is set for std clk.
TR0=1;
Enable_interrupt(); //Enable global interrupt
ET0=1;



}
/*
void timer1_init(void)
{
TMOD&=0x0F;
TL1=0;
TH1=0;
ET1=1;
EA=1;
TR1=1;
}
*/
void data_init(void)
{
buf2[0]=0;
buf2[1]=0;
buf2[2]=0;
buf2[3]=0;
}
void data_deal(void)
{
buf2[0]=buf2[0]+2;
if(buf2[0]>10)buf2[0]=0;
buf2[1]=buf2[1]+2;
if(buf2[1]>10)buf2[1]=0;
buf2[2]=buf2[2]+2;
if(buf2[2]>10)buf2[2]=0;
buf2[3]=buf2[3]+2;
if(buf2[3]>10)buf2[3]=0;
Usb_select_ep(1);
Usb_write_byte(buf2[0]);
Usb_write_byte(buf2[1]);
Usb_write_byte(buf2[2]);
Usb_write_byte(buf2[3]);
  Usb_set_tx_ready();
//WriteEp(1,4,buf2);
}
/*void timer1_interrupt(void) interrupt 3
{
buf[0]=buf[0]+1;
buf[1]=buf[1]+1;
buf[2]=buf[2]+1;
buf[3]=buf[3]+1;
WriteEp(1,4,buf);
}
*?
/*F**************************************************************************
* NAME: Timer0_interrupt
*----------------------------------------------------------------------------
* PARAMS:
* return:   none
*----------------------------------------------------------------------------
* PURPOSE: 
* This function is the interrupt program for the timer 0.
*----------------------------------------------------------------------------
* EXAMPLE:
*----------------------------------------------------------------------------
* NOTE: 
*----------------------------------------------------------------------------
* REQUIREMENTS: 
*****************************************************************************/
Interrupt (void Timer0_interrupt(void),IRQ_T0)
{
// TIMER0_OVF, interrupt cleared by hardware
TH0|=RELOAD_TH0; // Update Physical Clock ( logical OR to keep accuracy after long IT )
// Update gl_soft_timer_tick : number of ms till Reset, REMINDER_INC_MS to keep accuracy
gl_soft_timer_tick+=(((gl_reminder_inc_ms+=REMINDER_INC_MS)<REMINDER_INC_MS)?INC_MS+1:INC_MS);



}

#endif
