//============================================================================================================
//
//
//		主函数单元
//
//
//============================================================================================================
//#include	<LPC21xx.h>

#include	"config.h"
#include	"IRQ_Unit.h"
#include	"ClockUnit.h"
#include	"Command_Unit.h"
#include	"SPI_Unit.h"
#include	<String.h>
#include	<Stdio.h>

uint32	vMain_MyLED	=	1;


//============================================================================================================
//		初始化芯片的端口
//============================================================================================================
void	Init_IO(void)
{
	//使用高速口
	//SCS		=	SCS	|	0x01;

	//复位所有的选择功能；
		//P0.0=TxD0,	P0.1=RxD0;	P0.4=SCK0;	P0.5=MISO0;	P0.6=MOSI0;
		//P0.8=TxD1,	P0.9=RxD1;	P0.14=SCK1;	
	PINSEL0		=	0x20051505;
		//P0.19=MISO1;	P0.20=MOSI1;	P0.27~P0.31=JTAG
	PINSEL1		=	0x55400140;

	//设置所有的输出管脚
	IOSET	=	cPB_OutPort;
	IODIR	=	cPB_OutPort;
}
//============================================================================================================

//=====================================================================================================================
void	Main_BlinkLED(void)
{
	if	(vMain_MyLED)
	{
		IOSET	=	cPB_LED0;
	}
	else
	{
		IOCLR	=	cPB_LED0;
	}
	vMain_MyLED	=	!vMain_MyLED;
}
//=====================================================================================================================

//=====================================================================================================================
void	DoTest1(	BYTE*	pReadBuffer,	BYTE*	pWriteBuffer	)
{
	DWORD	TranLen;
	char	WR_Str[32]	=	"This is a test.";

		//测试第一步
		//测试目的：创建目录，创建文件，写入字符串。
		//等待But0按下，则打开文件，准备写数据
		while	(IOPIN	&	cPB_BUT0)
		{}
		//延迟半秒钟，防抖
		CLK_Delay(cCLK_TimerDelay,	500);

		IOSET	=	cPB_LED2;

		if	(!CMD_RootDir())
		{	return;	}

		if	(!CMD_MakeDir("TEMP1"))
		{	return;	}

		if	(!CMD_CreateFile("ABC","TXT"))
		{	return;	}

		memcpy(pWriteBuffer,	WR_Str,	16);
		if	(!CMD_WriteFile(16))
		{	return;	}

		if	(!CMD_OpenFile("ABC","TXT"))
		{	return;	}

		TranLen	=	16;
		if	(!CMD_ReadFile(&TranLen))
		{	return;	}
		if	(TranLen	<	15)
		{	return;	}
		if	(memcmp(pReadBuffer,	WR_Str,	15)	!=0)
		{	return;	}

		IOCLR	=	cPB_LED2;
}
//=====================================================================================================================

//=====================================================================================================================
//测试第二步
//测试目的：测试传输速度
void	DoTest2(	BYTE*	pReadBuffer,	BYTE*	pWriteBuffer	)
{
	BYTE*	pData;
	WORD*	pCounter;
	WORD	i,	SectorCounter;
	char	FileName[8];

	if	(!CMD_RootDir())
	{	return;	}

	if	(!CMD_MakeDir("TEMP1"))
	{	return;	}

	//找到一个没有使用的文件名
	for	(i=0;i<200;i++)
	{
		sprintf(FileName,	"TEST%.3d",	i);
		if	(!CMD_OpenFile(FileName,	"DAT"))
			break;
	}
	//如果文件太多，则退出。
	if	(i>=200)
		return;

	//准备传输的数据
	pCounter	=	(WORD*)pWriteBuffer;
	pData		=	pWriteBuffer+2;
	for	(i=0;	i<510;	i++)
		{
			*pData	=	i;
			pData++;
		}

	//等待But0按下，则开始写数据
	while	(IOPIN	&	cPB_BUT0)
	{}
	//延迟半秒钟，防抖
	CLK_Delay(cCLK_TimerDelay,	500);
	IOSET	=	cPB_LED3;

	//打开文件。
	if	(!CMD_CreateFile(FileName,	"DAT"))
		return;

	//循环写数据，每次写4KB，最多写
	for	(SectorCounter=0;	SectorCounter<2560;	SectorCounter++)
	{
		//生成要写的数据
		*pCounter	=	SectorCounter;
		//写文件
		if	(!CMD_WriteFile(4096))
		{	break;	}

		//如果有按键But1，则退出循环。
		if	(!(IOPIN	&	cPB_BUT1))
		{	break;	}
	}
	//返回根目录，实际上起到了关闭文件的作用。
	CMD_RootDir();

	IOCLR	=	cPB_LED3;
}
//=====================================================================================================================


//=====================================================================================================================
int main (void)
{
	BYTE*	pReadBuffer;
	BYTE*	pWriteBuffer;
	DWORD	Version;

	PINSEL0		=	0;
	PINSEL1		=	0;

	Init_IO();
	CLK_Init();
	CLK_SetupTimer(cCLK_TimerBlink,	500,	Main_BlinkLED);

	CMD_Init((DWORD*)&pWriteBuffer,	(DWORD*)&pReadBuffer);


	IOCLR	=	cPB_LED1	+	cPB_LED2	+	cPB_LED3;
	while(1)
	{
		IOCLR	=	cPB_LED1;
		if	(!CMD_GetVersion((WORD*)(&Version)))
		{
			continue;
		}
		if	(!(CMD_Detect()))
		{	continue;	}
		IOSET	=	cPB_LED1;


		//等待But0按下，则开始写数据
		if	(IOPIN	&	cPB_BUT0)
		{	continue;	}

		DoTest2(pReadBuffer,	pWriteBuffer);

	}

}
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
