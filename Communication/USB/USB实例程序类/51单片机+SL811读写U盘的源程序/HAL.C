#include "common.h"
#include "HAL.H"

//unsigned char idata ttt[20];
extern unsigned char xdata DBUF[BUFFER_LENGTH];

unsigned char SL811Read(unsigned char a)
{  
	unsigned char nVal;
	unsigned char xdata *exAddress;
	exAddress = SL811_ADDR_PORT;
	SL811_CS=0;//xxg
	*exAddress=a;
	exAddress=SL811_DATA_PORT;
	nVal = *exAddress;
	SL811_CS=1;//xxg
	return nVal;
}

void SL811Write(unsigned char a, unsigned char d)
{  
	unsigned char xdata *exAddress;
	exAddress = SL811_ADDR_PORT;
	SL811_CS=0;
	*exAddress=a;
	exAddress=SL811_DATA_PORT;
	*exAddress = d;
	SL811_CS=1;
}

void SL811BufRead(unsigned char addr, unsigned char *s, unsigned char c)
{	
	unsigned char i;
	unsigned char xdata *exAddress;
	exAddress=SL811_ADDR_PORT;
	SL811_CS=0;
	*exAddress = addr;
	exAddress=SL811_DATA_PORT;
	for(i=0;i<c;i++)
		{
		*s++ = *exAddress;
		}
	SL811_CS=1;
}

void SL811BufWrite(unsigned char addr, unsigned char *s, unsigned char c)
{	
	//unsigned char temp;
	unsigned char xdata *exAddress;
	exAddress = SL811_ADDR_PORT;
	
	SL811_CS=0;
	*exAddress = addr;
	exAddress=SL811_DATA_PORT;
	while (c--) 
		{
		*exAddress = *s++;
		}
	SL811_CS=1;
	
}

void ComSendByte(unsigned char c)
{
	SBUF=c;
	while(!TI);
	TI=0;
}
void ComErrRsp(unsigned char c)
{
	ComSendByte(0xaa);
	ComSendByte(0xbb);
	ComSendByte(c);
}
unsigned short WordSwap(unsigned short input)
{
	return(((input&0x00FF)<<8)|((input&0xFF00)>>8));
}

void DelayMs(unsigned char nFactor)
{
	unsigned char i;
	unsigned int j;

	for(i=0; i<nFactor; i++)
		{
		MCU_LED2=0;
		for(j=0;j<1000;j++)
		          j=j;
		MCU_LED2=1;
		}
}
void DelayUs(unsigned char nFactor)
{
	unsigned char i;
	unsigned int j;

	for(i=0; i<nFactor; i++)
		for(j=0;j<10;j++)
		          j=j;
}
unsigned long SwapINT32(unsigned long dData)
{
    dData = (dData&0xff)<<24|(dData&0xff00)<<8|(dData&0xff000000)>>24|(dData&0xff0000)>>8;
	return dData;
}

unsigned int SwapINT16(unsigned int dData)
{
    dData = (dData&0xff00)>>8|(dData&0x00ff)<<8;
	return dData;
}

