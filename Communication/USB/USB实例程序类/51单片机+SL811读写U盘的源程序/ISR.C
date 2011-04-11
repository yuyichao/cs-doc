#include "common.h"
#include "HAL.H"
#include "SL811.H"
#include "HPI.H"

extern XXGFLAGS bdata bXXGFlags;
//extern UART_CMD_BLOCK xdata inblock;
//extern unsigned char xdata UartInBuffer[2112];
extern UART_CMD_BLOCK xdata UartCmdBlock;
extern unsigned char xdata UARTBUF[UARTBUF_LENGTH];
//unsigned char nCount;
/////////////////////////////////////////
void ISRx_FN_USB(void);
void Reset_Timer0(void);
//////////////////////////////////////////
void ISR_COMM(void) interrupt 4
{
	unsigned char *pBuf=(unsigned char *)&UartCmdBlock;
	unsigned int cnt;
	unsigned char uartState,i;
	uartState=0;
	if(RI==0)
		return;
	/////// CMD and PARA phase/////////////////
	//for(cnt=0;cnt<2;cnt++)
	{
		while(!RI);
		i=SBUF;
		RI=0;
		if(i==0xaa)
			uartState=1;
		else
			{
			bXXGFlags.bits.bCOM_ERR=1;
			uartState=0;
			return;
			}
		Reset_Timer0();
		while(!RI && !TF0);
		if(TF0)
			{
			bXXGFlags.bits.bCOM_ERR=1;
			uartState=0;
			return;
			}
		i=SBUF;
		RI=0;
		if((i==0xbb)&&(uartState==1))
			uartState=2;
		else
			{
			bXXGFlags.bits.bCOM_ERR=1;
			uartState=0;
			return;
			}
		
	}
	
	for(cnt=0;cnt<64;cnt++)
	{
		Reset_Timer0();
		while(!RI && !TF0);
		if(TF0)
			{
			bXXGFlags.bits.bCOM_ERR=1;
			uartState=0;
			return;
			}
		*(pBuf+cnt)=SBUF;
		RI=0;
	}	
	/////// data phase /////////////////////
	if(UartCmdBlock.cmd==CMD_WRITE_FILE)
		{
		UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength=SwapINT16(UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength);
		if(UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength>MAX_WRITE_LENGTH)
		{
		bXXGFlags.bits.bCOM_ERR=1;
		uartState=0;
		return;
		}
		for(cnt=0;cnt<UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength;cnt++)
			{
			Reset_Timer0();
			while(!RI && !TF0);
			if(TF0)
			{
			bXXGFlags.bits.bCOM_ERR=1;
			uartState=0;
			return;
			}
			UARTBUF[cnt]=SBUF;
			RI=0;
			}
		UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength=SwapINT16(UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength);
		
		}	
	
	
	//////////////////////////
	bXXGFlags.bits.bUartInDone=1;
	RI=0;	
}

void ISR_Timer0(void) interrupt 1
{
	DISABLE_INTERRUPTS;
	//lClockTicks ++;
	bXXGFlags.bits.bTimer = 1;
	
	//if(bZBoardFlags.bits.bLED == LED_FLASH)
	//	IO_LED ^= 1;
	ENABLE_INTERRUPTS;
}

void ISR_Timer1(void) interrupt 3
{
}

void ISR_Timer2(void) interrupt 5
{
}

void ISR_INT1(void) interrupt 2
{
}

void ISR_INT0(void) interrupt 0
{
	//unsigned char intr;
	DISABLE_INTERRUPTS;
	//ISRx_FN_USB();
	//intr=SL811Read(IntStatus);
	SL811Write(IntStatus,INT_CLEAR);
	ENABLE_INTERRUPTS;
}

void Reset_Timer0(void)
{
	TR0=0;
	TF0=0;
	TL0 = 0x0;         /* value set by user    */
	TH0 = 0x0;         /* value set by user  */
	//ET0 = 1;           /* IE.1*/
	TR0 = 1;           /* TCON.4 start timer  */
}
