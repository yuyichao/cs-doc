//============================================================================================================
//
//
//	本单元的功能描述：
//
//			实现	通过UART1	传输数据的功能。
//
//
//	输出函数列表：
//
//			UART1_Init		初始化
//			UART1_Send		发送数据
//===========================================================================================================
//	日期	|	动作	|	作者	|	描述	
//===========================================================================================================
//2008-07-28|	创建 	|	霍旭阳	|	创建文件 
//============================================================================================================

//============================================================================================================
//		引入头文件
//============================================================================================================
//#include	<LPC23xx.h>                        /* LPC23xx/24xx definitions */

#include	"Config.h"
#include	"IRQ_Unit.h"
#include	"SPI_unit.h"

//============================================================================================================
//定义一些常量
//============================================================================================================

//============================================================================================================
//引入一些全局变量，这些全局变量在Command单元定义。
//============================================================================================================
//定义一些全局变量
BYTE	vSPI_FirstReadData;
BYTE	vSPI_TempData;
WORD	vSPI_ReadCounter;
volatile	BYTE*	vSPI_RD_pBufPos;
volatile	BYTE*	vSPI_RD_pBufStart;
volatile	BYTE*	vSPI_RD_pBufEnd;
volatile	BYTE*	vSPI_WR_pBufPos;
volatile	BYTE*	vSPI_WR_pBufStart;
volatile	BYTE*	vSPI_WR_pBufEnd;
//============================================================================================================



//============================================================================================================
//************************************************************************************************************
//*
//*
//*			开始定义	处理UART中断、实施数据传输的	函数。
//*
//*
//************************************************************************************************************
//============================================================================================================
//函数名：		UART1_ReadBuffer
//
//调用关系：	被	SPI的中断处理程序SPI_Handler	函数调用。
//
//功能描述：	从RBR读取数据，存入接收数据缓冲区。
//
//				在本函数中，修改Command单元中定义个全局变量  （接收数据缓冲区指针）vSPI_RD_pBufPos	的值。
//
//				得到新的数据后，调用Command单元的一个函数，通过Command单元去处理这些接收到的数据。
//
//入口参数：	无。
//
//返回值：		无。
//============================================================================================================
void SPI_ReadBuffer (void)
{
	while	(SSPSR	&	SSPSR_RNE)
	{
		//接收数据
		*vSPI_RD_pBufPos	=	SSPDR;
		if	((vSPI_RD_pBufPos	==	vSPI_RD_pBufEnd)	&&	
			(vSPI_FirstReadData)	&&	(vSPI_FirstReadData	==	*vSPI_RD_pBufPos))
		{
			*vSPI_RD_pBufPos	=	0;
		}
		/*if	((vSPI_ReadCounter	==	0)	&&	(vSPI_RD_pBufPos	!=	vSPI_RD_pBufEnd)	&&	
			(vSPI_FirstReadData)	&&	(vSPI_FirstReadData	!=	*vSPI_RD_pBufPos))
		{
			*vSPI_RD_pBufPos	=	0;
		}*/
		//如果是接受第一个数据，则需要等到第一个数据是我们需要的数据之后，才进行接受
		if	((vSPI_ReadCounter	==	0)	&&	(vSPI_FirstReadData))
		{
			if	(*vSPI_RD_pBufPos	==	vSPI_FirstReadData)
			{
				//启动“发送FIFO半空”和“接受FIFO半满”的中断
				//SSPIMSC	=	SSPIMSC_RORIM | SSPIMSC_RTIM | SSPIMSC_RXIM | SSPIMSC_TXIM;
				//vSPI_FirstReadData++;
				if	(vSPI_RD_pBufPos	==	vSPI_RD_pBufEnd)
				{
					vSPI_FirstReadData++;
					continue;
				}
			}
			else
			{	continue;	}
		}
		//移动接收指针
		vSPI_ReadCounter++;
		vSPI_RD_pBufPos++;
		if	(vSPI_RD_pBufPos	>=	vSPI_RD_pBufEnd)
		{
			vSPI_RD_pBufPos	=	(BYTE*)&vSPI_TempData;
			vSPI_RD_pBufStart	=	vSPI_RD_pBufPos;
			vSPI_RD_pBufEnd	=	vSPI_RD_pBufPos;
			vSPI_FirstReadData	=	0;
			//如果没有读数据需要读取，则停止“接受FIFO半满”的中断
			//SSPIMSC	&=	~SSPIMSC_RXIM;
			//return;
			continue;
		}
	}
}
//============================================================================================================
//函数名：		SPI_WriteBuffer
//
//调用关系：	被	SPI的中断处理程序SPI_Handler	函数调用。
//
//功能描述：	把发送数据缓冲区中的数据发送到THR寄存器。
//	
//				本函数仅当TX的FIFO为空时被调用执行，在函数中，最多写8个字节的数据到TX的FIFO中。
//
//				在本函数中，修改内部全局变量  （发送数据缓冲区指针）vSPI_WR_pLastPos	的值。
//
//入口参数：	无。
//
//返回值：		无。
//============================================================================================================
void SPI_WriteBuffer (void)
{
	DWORD	i;
	i	=	0;
	//循环发送
	while	(SSPSR	&	SSPSR_TNF)
	{
		if	(vSPI_WR_pBufPos	==	0)
		{
		//如果是工作于从模式，则需要象如下的工作
			/*SSP0DR	=	0xFFFF;*/
			
		//如果是工作于主模式，则需要象如下的工作
			//如果所有的数据都已经发送完，那么如果
			if	(vSPI_FirstReadData	==	0)
			{	//如果没有读数据需要读取，则停止“发送FIFO半空”的中断
				SSPIMSC	&=	~SSPIMSC_TXIM;
				return;	
			}
			else
			{	//如果还有数据需要读取，则发送0xFF。好继续执行读取动作。
				SSPDR	=	0xFFFF;
				i++;
				if	(i	>	3)
					return;	
			}
		}
		else
		{
			//如果没有数据需要发送，则将指针指向缺省地址
			if	(vSPI_WR_pBufPos	>=	vSPI_WR_pBufEnd)
			{
				vSPI_WR_pBufPos	=	0;
				SSPIMSC	&=	~SSPIMSC_TXIM;
				//清除接收数据FIFO，准备接受设备的应答
				while	(SSPSR	&	SSPSR_RNE)
				{
					i	=	SSPDR;
				}
				return;	
			}
			//发送数据
			SSPDR	=	*vSPI_WR_pBufPos;
			//移动发送指针
			vSPI_WR_pBufPos++;
			i++;
			if	(i	>	3)
				return;	
		}
	}
}
//============================================================================================================
//函数名：		SPI_Handler
//
//调用关系：	被	硬件中断	调用。
//
//功能描述：	SPI1的中断处理程序。
//
//入口参数：	无。
//
//返回值：		无。
//============================================================================================================
void SPI_Handler (void) __irq 
{
	WORD	IntCond;
	volatile	BYTE Dummy;

	IntCond	=	SSPMIS;
	if ( IntCond & SSPMIS_RORMIS )	/* Receive overrun interrupt */
	{
		SSPICR = SSPICR_RORIC;		/* clear interrupt */
	}
	if ( IntCond & SSPMIS_RTMIS )	/* Receive timeout interrupt */
	{
		SSPICR = SSPICR_RTIC;		/* clear interrupt */
	}

	if ( IntCond & SSPMIS_RXMIS )	/* Rx at least half full */
	{
		SPI_ReadBuffer();		/* receive until it's empty */		
	}

	if ( IntCond & SSPMIS_TXMIS )	/* Tx at least half empty */
	{
		SPI_WriteBuffer();		/* send until it's empty */		
	}

    VICVectAddr = 0;		// Acknowledge Interrupt
}
//============================================================================================================
//************************************************************************************************************
//*
//*
//*			开始定义	本单元的外部接口函数	函数。
//*
//*
//************************************************************************************************************
//============================================================================================================
//函数名：		SPI_Init
//
//调用关系：	被	外部函数	函数调用。
//
//功能描述：	SPI1的初始化程序。
//
//入口参数：	无
//
//返回值：		无。
//============================================================================================================
BOOL	SPI_Init( void )
{
	vSPI_ReadCounter	=	0;
	vSPI_RD_pBufPos	=	(BYTE*)&vSPI_TempData;
	vSPI_RD_pBufStart	=	vSPI_RD_pBufPos;
	vSPI_RD_pBufEnd	=	vSPI_RD_pBufPos;
	vSPI_WR_pBufPos	=	0;
	vSPI_WR_pBufStart	=	vSPI_RD_pBufPos;
	vSPI_WR_pBufEnd	=	vSPI_RD_pBufPos;

	PINSEL0	|=	0x20000000;       /* SCK1 */
	PINSEL1	|=	0x00000540;       /* SSEL1,MOSI1,MOSI1 */
	
	// enable clock to SSP1 for security reason. By default, it's enabled already
	PCONP	|=	(1 << 10);

	//Set DSS data to 8-bit, Frame format SPI, CPOL = 0, CPHA = 0, and SCR is 15
	SSPCR0	=	0x0107;

	// SSPCPSR clock prescale register, minimum divisor is 0x02 in master mode
	SSPCPSR	=	0x2;

	IRQ_SetInt( cIRQ_SPI1, 1, (DWORD)SPI_Handler );
	
	// Device select as master, SSP Enabled
	SSPCR1	=	SSPCR1_SSE;
	//* Set SSPINMS registers to enable interrupts */
	//* enable all error related interrupts */
	SSPIMSC	=	SSPIMSC_RORIM | SSPIMSC_RTIM;

	return	TRUE;
}
//============================================================================================================
//函数名：		SPI_Send
//
//调用关系：	被	外部函数	函数调用。
//
//功能描述：	设置发送数据缓冲区的指针，然后，填写第一次的THR，以后的数据都在中断服务程序中被填写到THR、发送。
//
//入口参数：	pBufStart	发送缓冲区的起始位置。
//				DataLength	需要发送的数据长度。
//
//返回值：		无。
//============================================================================================================
BOOL	SPI_Send(void*	pBufStart,	DWORD	DataLength)
{
	if	((vSPI_WR_pBufPos	<	vSPI_WR_pBufEnd)	&&	(vSPI_WR_pBufPos	!=	0))
	{
		return	FALSE;
	}

	IRQ_Disable(	cIRQ_SPI1	);

	vSPI_WR_pBufStart	=	pBufStart;
	vSPI_WR_pBufEnd	=	(BYTE*)((DWORD)pBufStart	+	DataLength);
	vSPI_WR_pBufPos	=	vSPI_WR_pBufStart;

	SSPIMSC	|=	SSPIMSC_TXIM;
	SPI_WriteBuffer();

	IRQ_Enable(	cIRQ_SPI1	);

	return	TRUE;
}
//============================================================================================================
//函数名：		SPI_CanSendNextPacket
//
//调用关系：	被	外部函数	函数调用。
//
//功能描述：	查询是否可以发送下一个数据包。
//
//入口参数：	无
//
//返回值：		如果写指针等于发送数据传冲区结束位置，则返回真；否则返回假。
//============================================================================================================
BOOL	SPI_CanSendNextPacket(void)
{
	return	(vSPI_WR_pBufPos	>=	vSPI_WR_pBufEnd)	||	(vSPI_WR_pBufPos	==	0);
}
//============================================================================================================
//函数名：		SPI_StartRead
//
//调用关系：	被	外部函数	函数调用。
//
//功能描述：	启动读数据过程。
//
//入口参数：	pBufStart	发送缓冲区的起始位置。
//				DataLength	需要接收的数据长度。
//
//返回值：		无。
//============================================================================================================
void	SPI_StartRead(void*	pBufStart,	DWORD	DataLength,	DWORD	FirstData)
{
	IRQ_Disable(	cIRQ_SPI1	);

	vSPI_RD_pBufStart	=	pBufStart;
	vSPI_RD_pBufEnd	=	(BYTE*)((DWORD)pBufStart	+	DataLength);
	vSPI_RD_pBufPos	=	pBufStart;

	vSPI_FirstReadData	=	FirstData;
	vSPI_ReadCounter	=	0;

	SSPIMSC	|=	SSPIMSC_RXIM;

	IRQ_Enable(	cIRQ_SPI1	);
}
//============================================================================================================
//函数名：		SPI_GetReadCounter
//
//调用关系：	被	外部函数	函数调用。
//
//功能描述：	查询读了多少个数据。
//
//入口参数：	无
//
//返回值：		读了多少个数据。
//============================================================================================================
DWORD	SPI_GetReadCounter(void)
{
	if	(SPI_CanSendNextPacket())
	{
		SPI_ReadBuffer();
		SPI_WriteBuffer();
	}

	return	vSPI_ReadCounter;
}
//============================================================================================================
//函数名：		SPI_CancelRead
//
//调用关系：	被	外部函数	函数调用。
//
//功能描述：	终止读数据过程。
//
//入口参数：	无
//
//返回值：		无。
//============================================================================================================
void	SPI_CancelRead(void)
{
	IRQ_Disable(	cIRQ_SPI1	);

	vSPI_RD_pBufPos	=	(BYTE*)&vSPI_TempData;
	vSPI_RD_pBufStart	=	vSPI_RD_pBufPos;
	vSPI_RD_pBufEnd	=	vSPI_RD_pBufPos;
	vSPI_ReadCounter	=	0;
	vSPI_FirstReadData	=	0;
	SSPIMSC	&=	~SSPIMSC_RXIM;

	IRQ_Enable(	cIRQ_SPI1	);
}
//============================================================================================================




//============================================================================================================
//
//			End	of	File
//
//============================================================================================================
