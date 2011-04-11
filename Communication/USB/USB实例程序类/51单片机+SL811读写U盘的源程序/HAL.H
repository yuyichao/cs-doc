#include "AT89X52.H"
//#include "common.h"

sbit MCU_LED0=P1^0;
sbit MCU_LED1=P1^1;
sbit MCU_LED2=P1^2;
sbit MCU_LED3=P1^3;

sbit SWM0=P1^4;
sbit SWM1=P1^5;
sbit SWM2=P1^6;
sbit SWM3=P1^7;

sbit SL811_CS=P3^4;

#define SL811_ADDR_PORT		0x0ff00
#define SL811_DATA_PORT		0x0ff01

unsigned char SL811Read(unsigned char a);
void SL811Write(unsigned char a, unsigned char d);
void SL811BufRead(unsigned char addr, unsigned char *s, unsigned char c);
void SL811BufWrite(unsigned char addr, unsigned char *s, unsigned char c);
unsigned char RamRead(unsigned short a);
void RamWrite(unsigned short a, unsigned char d);
void ComSendByte(unsigned char c);
void ComErrRsp(unsigned char c);
unsigned short WordSwap(unsigned short input);
void DelayMs(unsigned char nFactor);
void DelayUs(unsigned char nFactor);
unsigned long SwapINT32(unsigned long dData);
unsigned int SwapINT16(unsigned int dData);




