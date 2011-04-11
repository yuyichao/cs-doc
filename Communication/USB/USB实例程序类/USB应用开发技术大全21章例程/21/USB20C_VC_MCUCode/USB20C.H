#if !defined(USB20C_H__5A0F6F1D_E399_4B06_A216_986D73DAE71E__INCLUDED_)
#define USB20C_H__5A0F6F1D_E399_4B06_A216_986D73DAE71E__INCLUDED_

#define	CPU12MHz	#0x00
#define	CPU24MHz	#0x08
#define	CPU48MHz	#0x10

#define	I2C24c01	#1
#define	I2C24c02	#2
#define	I2C24c04	#3
#define	I2C24c16	#4
#define	I2C24c32	#5
#define	I2C24c64	#6
#define	I2C24c128	#7
#define	I2C24c256	#8


int		USB20C_DLLInit();
void	USB20C_DLLDone();
int		USB20C_EnumDeviceCount(void);
int		USB20C_Init(int DevNum, int I2CChip);
void	USB20C_Done(void);

BOOL	USB20C_SetAddress(int Addr);
int		USB20C_Input(int Addr);
//BOOL	USB20C_MultInput(int Num, void * Buf);
BOOL	USB20C_MultInput(int Num, void * Buf);
//BOOL	USB20C_Output(int Addr, int Data);
BOOL	USB20C_Output(byte Addr, byte Data);
BOOL	USB20C_MultOutput(int Num, void * Buf);
//BOOL	USB20C_MultOutput(int Num, char * Buf);
BOOL	USB20C_MixedIO(int Num, char * Buf);

BOOL	USB20C_StartDMARead(void);
BOOL	USB20C_StartDMAWrite(void);
BOOL	USB20C_EndDMA(void);
BOOL	USB20C_DMARead(void * Buf, int DataSize, void * DataRead, int OverTime);
BOOL	USB20C_DMAWrite(void * Buf, int DataSize, int * DataWritten, int OverTime);

BOOL	USB20C_SetI2CFrequency(BOOL WorkAt400KHz);
BOOL	USB20C_ReadI2C(int I2CChip, int Addr, int Num, int StartAddr, void * Buf);
BOOL	USB20C_WriteI2C(int I2CChip, int Addr, int Num, int StartAddr, void * Buf);

BOOL	USB20C_SetCPUCS(int CPUSpeed, BOOL ClkOutEnable, BOOL ClkInvert);
BOOL	USB20C_SetIFCFG(BOOL IntIFCLK, BOOL IntClk48MHz, BOOL IntClkOutEn, BOOL IFCLKInvert, BOOL AsyncFIFO, BOOL OnePulseEarly);
BOOL	USB20C_WorkAtHighSpeed(void);

int		USB20C_GetLastError(void);
BOOL	USB20C_GetLastErrorStrC(int * Size, PCHAR ErrStr);

#endif
