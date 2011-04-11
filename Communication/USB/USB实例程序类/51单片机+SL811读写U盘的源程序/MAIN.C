#include "common.h"
#include "AT89X52.H"
#include "SL811.H"
#include "TPBULK.H"
#include "HAL.H"
#include "HPI.H"
//#include "common.h"

XXGFLAGS bdata bXXGFlags;
extern unsigned char xdata DBUF[BUFFER_LENGTH];
/////////////////////////////////////////////////

void Init_Timer0(void)
{
	TMOD &= 0xF0;      /* clear Timer 0   */
	TMOD  |= 0x1;
	TL0 = 0x0;         /* value set by user    */
	TH0 = 0x0;         /* value set by user  */
	ET0 = 1;           /* IE.1*/
	TR0 = 1;           /* TCON.4 start timer  */
//	PT0 = 1;
}

void Init_SpecialInterrupts(void)
{
	IT0 = 0;
	EX0 = 1;
//	PX0 = 0;
}

void Init_Port()
{
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
}

/*Serial Port */
/*Mode            = 1  /8-bit UART
  Serial Port Interrupt    = Disabled         */
/*Receive         = Enabled   */
/*Auto Addressing    = Disabled   */
void Init_COMM(void)
{
	SCON = 0x53;
	PCON = 0x80 | PCON;
	//TMOD = 0x21;
//	TCON = 0x69;    /* TCON */
	//TH1 = 0xfa;		// 9600bps @ 11.0592MHz
	//TR1 = 0;
	T2CON=0x30;
	RCAP2H=0xFF;		// 57600 @ 24MHz: 24000000/(32*(65536-(RCAP2H,RCAP2L)))
	RCAP2L=0xF3;
	TI=0;
	RI=0;
	TR2=1;
	ES = 1;
}

void main(void)
{
	unsigned char temp;
	
	Init_Timer0();
	Init_SpecialInterrupts();
	Init_Port();
	Init_COMM();
	
	MCU_LED0=1;
	MCU_LED1=1;
	MCU_LED2=1;
	MCU_LED3=0;
	
	//temp=SL811_GetRev();
	//for(temp=0;temp<2;temp++)
		DelayMs(254);
	//temp=SL811Read(CtrlReg);
	//SL811Write(CtrlReg,0);
	//temp=SL811Read(CtrlReg);
	
	//temp=SL811Read(IntStatus);
	//SL811Write(IntStatus,INT_CLEAR);
	//temp=SL811Read(IntStatus);
	bXXGFlags.bits.bUartInDone=0;	
		
	SL811_Init();
	
	for(temp=0;temp<64;temp++)
		DBUF[temp]=0;
	
	ENABLE_INTERRUPTS;
	
	while(TRUE)
	{
		if (bXXGFlags.bits.bTimer){
			DISABLE_INTERRUPTS;
			bXXGFlags.bits.bTimer = 0;
			ENABLE_INTERRUPTS;

			//if(bXXGFlags.bits.bConfiguration)
			check_key_LED();
			}
		if (bXXGFlags.bits.bCOM_ERR){
			
			bXXGFlags.bits.bCOM_ERR=0;
			ComErrRsp(COMERC_CMDERR);
			}
		if(bXXGFlags.bits.SLAVE_FOUND){
			DISABLE_INTERRUPTS;
			bXXGFlags.bits.SLAVE_FOUND=FALSE;
			for(temp=0;temp<4;temp++)
				DelayMs(250);
			//DelayMs(50);
			if(EnumUsbDev(1))				// enumerate USB device, assign USB address = #1
			{
			   	bXXGFlags.bits.SLAVE_ENUMERATED = TRUE;	// Set slave USB device enumerated flag
			   	MCU_LED0=0;
			}	
			ENABLE_INTERRUPTS;
			}
		if(bXXGFlags.bits.SLAVE_REMOVED){
			DISABLE_INTERRUPTS;
			bXXGFlags.bits.SLAVE_REMOVED=FALSE;
			MCU_LED0=1;
			MCU_LED1=1;
			bXXGFlags.bits.SLAVE_ENUMERATED = FALSE;
			bXXGFlags.bits.SLAVE_IS_ATTACHED = FALSE;
			//bXXGFlags.bits.bMassDevice=TRUE;
			ENABLE_INTERRUPTS;
			}
		if(bXXGFlags.bits.bMassDevice){
			DISABLE_INTERRUPTS;
			bXXGFlags.bits.bMassDevice=FALSE;
			if(EnumMassDev())
				{
				bXXGFlags.bits.SLAVE_IS_ATTACHED = TRUE;
				MCU_LED1=0;
				}
			else
				{
				MCU_LED1=1;
				bXXGFlags.bits.SLAVE_IS_ATTACHED = FALSE;
				}		
			ENABLE_INTERRUPTS;
			}
		if(bXXGFlags.bits.bUartInDone){
			DISABLE_INTERRUPTS;
			bXXGFlags.bits.bUartInDone=0;	
			UartHandler();
			ENABLE_INTERRUPTS;	
			}
		
	}
	
}
