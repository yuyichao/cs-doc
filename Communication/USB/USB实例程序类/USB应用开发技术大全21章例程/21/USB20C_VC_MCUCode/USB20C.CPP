
#include "stdafx.h"
#include "USB20C.h"

typedef	int		__stdcall	tdUSB20C_EnumDeviceCount(void);
typedef	int		__stdcall	tdUSB20C_Init(int DevNum, int I2CChip);
typedef	void	__stdcall	tdUSB20C_Done(void);

typedef	BOOL	__stdcall	tdUSB20C_SetAddress(int Addr);
typedef	int		__stdcall	tdUSB20C_Input(int Addr);
typedef	BOOL	__stdcall	tdUSB20C_MultInput(int Num, void * Buf);
//typedef	BOOL	__stdcall	tdUSB20C_MultInput(int Num, char * Buf);
typedef	BOOL	__stdcall	tdUSB20C_Output(byte Addr, byte Data);
typedef	BOOL	__stdcall	tdUSB20C_MultOutput(int Num,void * Buf);
//typedef	BOOL	__stdcall	tdUSB20C_MultOutput(int Num, char * Buf);
typedef	BOOL	__stdcall	tdUSB20C_MixedIO(int Num, char * Buf);

typedef	BOOL	__stdcall	tdUSB20C_StartDMARead(void);
typedef	BOOL	__stdcall	tdUSB20C_StartDMAWrite(void);
typedef	BOOL	__stdcall	tdUSB20C_EndDMA(void);
typedef	BOOL	__stdcall	tdUSB20C_DMARead(void * Buf, int DataSize, void * DataRead, int OverTime);

typedef	BOOL	__stdcall	tdUSB20C_DMAWrite(void * Buf, int DataSize, int * DataWritten, int OverTime);

typedef	BOOL	__stdcall	tdUSB20C_SetI2CFrequency(BOOL WorkAt400KHz);
typedef	BOOL	__stdcall	tdUSB20C_ReadI2C(int I2CChip, int Addr, int Num, int StartAddr, void * Buf);
typedef	BOOL	__stdcall	tdUSB20C_WriteI2C(int I2CChip, int Addr, int Num, int StartAddr, void * Buf);

typedef	BOOL	__stdcall	tdUSB20C_SetCPUCS(int CPUSpeed, BOOL ClkOutEnable, BOOL ClkInvert);
typedef	BOOL	__stdcall	tdUSB20C_SetIFCFG(BOOL IntIFCLK, BOOL IntClk48MHz, BOOL IntClkOutEn, BOOL IFCLKInvert, BOOL AsyncFIFO, BOOL OnePulseEarly);
typedef	BOOL	__stdcall	tdUSB20C_WorkAtHighSpeed(void);

typedef	int		__stdcall	tdUSB20C_GetLastError(void);
typedef	BOOL	__stdcall	tdUSB20C_GetLastErrorStrC(int * Size, PCHAR ErrStr);


HINSTANCE hLibrary;

tdUSB20C_EnumDeviceCount *		fUSB20C_EnumDeviceCount;
tdUSB20C_Init *					fUSB20C_Init;
tdUSB20C_Done *					fUSB20C_Done;

tdUSB20C_SetAddress *			fUSB20C_SetAddress;
tdUSB20C_Input *				fUSB20C_Input;
tdUSB20C_MultInput *			fUSB20C_MultInput;
tdUSB20C_Output *				fUSB20C_Output;
tdUSB20C_MultOutput *			fUSB20C_MultOutput;
tdUSB20C_MixedIO *				fUSB20C_MixedIO;

tdUSB20C_StartDMARead *			fUSB20C_StartDMARead;
tdUSB20C_StartDMAWrite *		fUSB20C_StartDMAWrite;
tdUSB20C_EndDMA *				fUSB20C_EndDMA;
tdUSB20C_DMARead *				fUSB20C_DMARead;
tdUSB20C_DMAWrite *				fUSB20C_DMAWrite;

tdUSB20C_SetI2CFrequency *		fUSB20C_SetI2CFrequency;
tdUSB20C_ReadI2C *				fUSB20C_ReadI2C;
tdUSB20C_WriteI2C *				fUSB20C_WriteI2C;

tdUSB20C_SetCPUCS *				fUSB20C_SetCPUCS;
tdUSB20C_SetIFCFG *				fUSB20C_SetIFCFG;
tdUSB20C_WorkAtHighSpeed *		fUSB20C_WorkAtHighSpeed;

tdUSB20C_GetLastError *			fUSB20C_GetLastError;
tdUSB20C_GetLastErrorStrC *		fUSB20C_GetLastErrorStrC;

int		USB20C_DLLInit()
{
	hLibrary = LoadLibrary("USB20C.DLL");
	if (hLibrary == NULL)
	{
		return	0;
	}
	fUSB20C_EnumDeviceCount = (tdUSB20C_EnumDeviceCount*)GetProcAddress(hLibrary, "USB20C_EnumDeviceCount");
	fUSB20C_Init = (tdUSB20C_Init*)GetProcAddress(hLibrary,"USB20C_Init");
	fUSB20C_Done = (tdUSB20C_Done*)GetProcAddress(hLibrary,"USB20C_Done");

	fUSB20C_SetAddress = (tdUSB20C_SetAddress*)GetProcAddress(hLibrary,"USB20C_SetAddress");
	fUSB20C_Input = (tdUSB20C_Input*)GetProcAddress(hLibrary,"USB20C_Input");
	fUSB20C_MultInput = (tdUSB20C_MultInput*)GetProcAddress(hLibrary,"USB20C_MultInput");
	fUSB20C_Output = (tdUSB20C_Output*)GetProcAddress(hLibrary,"USB20C_Output");
	fUSB20C_MultOutput = (tdUSB20C_MultOutput*)GetProcAddress(hLibrary,"USB20C_MultOutput");
	fUSB20C_MixedIO = (tdUSB20C_MixedIO*)GetProcAddress(hLibrary,"USB20C_MixedIO");
	
	fUSB20C_StartDMARead = (tdUSB20C_StartDMARead*)GetProcAddress(hLibrary,"USB20C_StartDMARead");
	fUSB20C_StartDMAWrite = (tdUSB20C_StartDMAWrite*)GetProcAddress(hLibrary,"USB20C_StartDMAWrite");
	fUSB20C_EndDMA = (tdUSB20C_EndDMA*)GetProcAddress(hLibrary,"USB20C_EndDMA");
	fUSB20C_DMARead = (tdUSB20C_DMARead*)GetProcAddress(hLibrary,"USB20C_DMARead");
	fUSB20C_DMAWrite = (tdUSB20C_DMAWrite*)GetProcAddress(hLibrary,"USB20C_DMAWrite");
	
	fUSB20C_SetI2CFrequency = (tdUSB20C_SetI2CFrequency*)GetProcAddress(hLibrary,"USB20C_SetI2CFrequency");
	fUSB20C_ReadI2C = (tdUSB20C_ReadI2C*)GetProcAddress(hLibrary,"USB20C_ReadI2C");
	fUSB20C_WriteI2C = (tdUSB20C_WriteI2C*)GetProcAddress(hLibrary,"USB20C_WriteI2C");
	
	fUSB20C_SetCPUCS = (tdUSB20C_SetCPUCS*)GetProcAddress(hLibrary,"USB20C_SetCPUCS");
	fUSB20C_SetIFCFG = (tdUSB20C_SetIFCFG*)GetProcAddress(hLibrary,"USB20C_SetIFCFG");
	fUSB20C_WorkAtHighSpeed = (tdUSB20C_WorkAtHighSpeed*)GetProcAddress(hLibrary,"USB20C_WorkAtHighSpeed");
	
	fUSB20C_GetLastError = (tdUSB20C_GetLastError*)GetProcAddress(hLibrary,"USB20C_GetLastError");
	fUSB20C_GetLastErrorStrC = (tdUSB20C_GetLastErrorStrC*)GetProcAddress(hLibrary,"USB20C_GetLastErrorStrC");

	return 1;
}

void	USB20C_DLLDone(void)
{
	FreeLibrary(hLibrary);
}


int		USB20C_Init(int DevNum, int I2CChip)
{
	return fUSB20C_Init(DevNum, I2CChip);
}

void	USB20C_Done(void)
{
	fUSB20C_Done();
}

int		USB20C_EnumDeviceCount(void)
{
	return fUSB20C_EnumDeviceCount();
}

BOOL	USB20C_SetAddress(int Addr)
{
	return fUSB20C_SetAddress(Addr);
}

int		USB20C_Input(int Addr)
{
	return fUSB20C_Input(Addr);
}

BOOL	USB20C_MultInput(int Num, void * Buf)
{
	return fUSB20C_MultInput(Num, Buf);
}

BOOL	USB20C_Output(byte Addr, byte Data)
{
	return fUSB20C_Output(Addr, Data);
}

BOOL	USB20C_MultOutput(int Num, void * Buf)
{
	return fUSB20C_MultOutput(Num, Buf);
}

BOOL	USB20C_MixedIO(int Num, char * Buf)
{
	return fUSB20C_MixedIO(Num, Buf);
}


BOOL	USB20C_StartDMARead(void)
{
	return fUSB20C_StartDMARead();
}

BOOL	USB20C_StartDMAWrite(void)
{
	return fUSB20C_StartDMAWrite();
}

BOOL	USB20C_EndDMA(void)
{
	return fUSB20C_EndDMA();
}

BOOL	USB20C_DMARead(void * Buf, int DataSize, void * DataRead, int OverTime)
{
	return fUSB20C_DMARead(Buf, DataSize, DataRead, OverTime);
}

BOOL	USB20C_DMAWrite(void * Buf, int DataSize, int * DataWritten, int OverTime)
{
	return fUSB20C_DMAWrite(Buf, DataSize, DataWritten, OverTime);
}


BOOL	USB20C_SetI2CFrequency(BOOL WorkAt400KHz)
{
	return fUSB20C_SetI2CFrequency(WorkAt400KHz);
}

BOOL	USB20C_ReadI2C(int I2CChip, int Addr, int Num, int StartAddr, void * Buf)
{
	return fUSB20C_ReadI2C(I2CChip, Addr, Num, StartAddr, Buf);
}

BOOL	USB20C_WriteI2C(int I2CChip, int Addr, int Num, int StartAddr, void * Buf)
{
	return fUSB20C_WriteI2C(I2CChip, Addr, Num, StartAddr, Buf);
}


BOOL	USB20C_SetCPUCS(int CPUSpeed, BOOL ClkOutEnable, BOOL ClkInvert)
{
	return fUSB20C_SetCPUCS(CPUSpeed, ClkOutEnable, ClkInvert);
}

BOOL	USB20C_SetIFCFG(BOOL IntIFCLK, BOOL IntClk48MHz, BOOL IntClkOutEn, BOOL IFCLKInvert, BOOL AsyncFIFO, BOOL OnePulseEarly)
{
	return fUSB20C_SetIFCFG(IntIFCLK, IntClk48MHz, IntClkOutEn, IFCLKInvert, AsyncFIFO, OnePulseEarly);
}

BOOL	USB20C_WorkAtHighSpeed(void)
{
	return fUSB20C_WorkAtHighSpeed();
}


int		USB20C_GetLastError(void)
{
	return fUSB20C_GetLastError();
}

BOOL	USB20C_GetLastErrorStrC(int * Size, PCHAR ErrStr)
{
	return fUSB20C_GetLastErrorStrC(Size, ErrStr);
}


