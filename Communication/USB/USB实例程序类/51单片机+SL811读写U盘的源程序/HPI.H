//#include "common.h"
///////////////////////////////////////
#define CMD_RESET		0x00

#define CMD_DETECT_DEVICE	0x01
#define CMD_DETECT_DEVICE_RSP	0x04

#define CMD_OPEN_FILE		0x06
#define CMD_CREATE_FILE		0x07
#define CMD_READ_FILE		0x08
#define CMD_WRITE_FILE		0x09

#define CMD_CREATE_FILE_RSP	0x0A
#define CMD_READ_FILE_RSP	0x0B
#define CMD_WRITE_FILE_RSP	0x0C
#define CMD_OPEN_FILE_RSP	0x0D

#define CMD_LIST		0x10
#define CMD_REMOVE_FILE		0x11
#define CMD_GET_CAPACITY	0x12
#define CMD_GET_FREE_CAPACITY	0x13
#define CMD_FORMAT_DISK		0x14
#define CMD_SET_FILE_POINTER	0x15

#define CMD_LIST_RSP		0x20
#define CMD_REMOVE_FILE_RSP	0x21
#define CMD_GET_CAPACITY_RSP	0x22
#define CMD_GET_FREE_CAPACITY_RSP	0x23
#define CMD_FORMAT_DISK_RSP	0x24
#define CMD_SET_FILE_POINTER_RSP	0x25

#define CMD_GET_VERSION		0x30
#define CMD_GET_VERSION_RSP	0x40

#define CMD_DIR_DOWN		0x70
#define CMD_DIR_DOWN_RSP	0x80
#define CMD_DIR_UP		0x71
#define CMD_DIR_UP_RSP		0x81		
///////////////////////////////////////////////
#define ERC_OK			0x0000

#define ERC_NODEVICE		0x0001
#define ERC_DEVICEFULL		0x0002
#define ERC_DEVICEERR		0x000a
#define ERC_FILEEXIST		0x0010
#define ERC_FILENOTFOUND	0x0011
#define ERC_LENGTHEXCEED	0x0012
#define ERC_REACHEND		0x0013
#define ERC_FILENOTOPENED	0x0014
#define ERC_STATEERR		0x0050
#define ERC_SYSERR		0x00fa

#define COMERC_CMDERR		0x01
#define COMERC_TIMEOUT		0x02
////////////////////////////////////////////////


typedef struct _UART_CMD_GETVERSION{
	//unsigned char cmd;
	unsigned char para;
	unsigned char Reserved[62];
}UART_CMD_GETVERSION,*PUART_CMD_GETVERSION;

typedef struct _UART_CMD_OPENFILE{
	//unsigned char cmd;
	unsigned char filename[11];
	//unsigned char fileext[3]
	unsigned char Reserved[52];
}UART_CMD_OPENFILE,*PUART_CMD_OPENFILE;

typedef struct _UART_CMD_READFILE{
	unsigned int readLength;
	unsigned char Reserved[61];
}UART_CMD_READFILE,*PUART_CMD_READFILE;

typedef struct _UART_CMD_CREATEFILE{
	//unsigned char filename[11];
	unsigned char item[32];
	unsigned char para;
	unsigned char Reserved[30];
}UART_CMD_CREATEFILE,*PUART_CMD_CREATEFILE;

typedef struct _UART_CMD_WRITEFILE{
	//unsigned char filename[11];
	//unsigned char item[32];
	unsigned char para;
	unsigned int writeLength;
	unsigned char Reserved[60];
}UART_CMD_WRITEFILE,*PUART_CMD_WRITEFILE;

typedef struct _UART_CMD_REMOVEFILE{
	//unsigned char cmd;
	unsigned char para;
	unsigned char filename[11];
	//unsigned char fileext[3]
	unsigned char Reserved[51];
}UART_CMD_REMOVEFILE,*PUART_CMD_REMOVEFILE;

typedef struct _UART_CMD_SETFILEPOINTER{
	//unsigned char cmd;
	unsigned char para;
	unsigned long pointer;
	unsigned char Reserved[58];
}UART_CMD_SETFILEPOINTER,*PUART_CMD_SETFILEPOINTER;

typedef union _CMD_BLOCK {
	UART_CMD_OPENFILE Cmd_OpenFile;
	UART_CMD_READFILE Cmd_ReadFile;
	UART_CMD_CREATEFILE Cmd_CreateFile;
	UART_CMD_WRITEFILE	Cmd_WriteFile;
	UART_CMD_REMOVEFILE	Cmd_RemoveFile;
	UART_CMD_SETFILEPOINTER Cmd_SetFilePointer;
	UART_CMD_GETVERSION Cmd_GetVersion;
}CMD_BLOCK, * PCMD_BLOCK;

//////////////////////////////////////////////////
typedef struct _UART_RSP_DETECTDEVICE{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned char Reserved[60];
}UART_RSP_DETECTDEVICE,*PUART_RSP_DETECTDEVICE;

typedef struct _UART_RSP_GETCAPACITY{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned long disksize;
	unsigned long freedisksize;
	unsigned char Reserved[52];
}UART_RSP_GETCAPACITY,*PUART_RSP_GETCAPACITY;

typedef struct _UART_RSP_GETFREECAPACITY{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned long diskfreesize;
	unsigned char Reserved[56];
}UART_RSP_GETFREECAPACITY,*PUART_RSP_GETFREECAPACITY;

typedef struct _UART_RSP_LIST{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned int len;
	//unsigned int errcode;
	unsigned char Reserved[58];
}UART_RSP_LIST,*PUART_RSP_LIST;

typedef struct _UART_RSP_OPENFILE{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned char item[32];
	//unsigned int errcode;
	unsigned char Reserved[28];
}UART_RSP_OPENFILE,*PUART_RSP_OPENFILE;

typedef struct _UART_RSP_READFILE{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned int readLength;
	//unsigned int errcode;
	unsigned char Reserved[58];
}UART_RSP_READFILE,*PUART_RSP_READFILE;

typedef struct _UART_RSP_CREATEFILE{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned char Reserved[60];
}UART_RSP_CREATEFILE,*PUART_RSP_CREATEFILE;

typedef struct _UART_RSP_WRITEFILE{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned char Reserved[60];
}UART_RSP_WRITEFILE,*PUART_RSP_WRITEFILE;

typedef struct _UART_RSP_REMOVEFILE{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned char Reserved[60];
}UART_RSP_REMOVEFILE,*PUART_RSP_REMOVEFILE;

typedef struct _UART_RSP_SETFILEPOINTER{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned char Reserved[60];
}UART_RSP_SETFILEPOINTER,*PUART_RSP_SETFILEPOINTER;

typedef struct _UART_RSP_GETVERSION{
	//unsigned char cmd;
	//unsigned char result;
	//unsigned int errcode;
	unsigned int version;
	unsigned char Reserved[58];
}UART_RSP_GETVERSION,*PUART_RSP_GETVERSION;

typedef union _RSP_BLOCK {

	UART_RSP_DETECTDEVICE Rsp_DetectDevice;
	UART_RSP_GETCAPACITY Rsp_GetCapacity;
	UART_RSP_GETFREECAPACITY Rsp_GetFreeCapacity;
	UART_RSP_LIST	Rsp_List;
	UART_RSP_OPENFILE Rsp_OpenFile;
	UART_RSP_READFILE Rsp_ReadFile;
	UART_RSP_CREATEFILE Rsp_CreateFile;
	UART_RSP_WRITEFILE Rsp_WriteFile;
	UART_RSP_REMOVEFILE	Rsp_RemoveFile;
	UART_RSP_SETFILEPOINTER Rsp_SetFilePointer;
	UART_RSP_GETVERSION Rsp_GetVersion;
}RSP_BLOCK, * PRSP_BLOCK;

////////////////////////////////////////////////////
typedef struct _UART_CMD_BLOCK {
	unsigned char cmd;
	CMD_BLOCK CmdBlock;
	//unsigned char buffer[2048];
} UART_CMD_BLOCK, * PUART_CMD_BLOCK;

typedef struct _UART_RSP_BLOCK {
	unsigned char cmd;
	unsigned char result;
	unsigned int errcode;
	RSP_BLOCK RspBlock;
	unsigned int len;
	unsigned char *pbuffer;
} UART_RSP_BLOCK, * PUART_RSP_BLOCK;


/////////////////////////////////////////////////
void UartSendRsp(void);
unsigned char UartHandler(void);
unsigned char DetectDevice(void);
unsigned char List(void);
unsigned char OpenFile(unsigned char *pBuffer);
unsigned char ReadFile(unsigned int readLength,unsigned char *pBuffer);
unsigned char CreateFile(unsigned char *pBuffer);
unsigned char WriteFile(unsigned int writeLength,unsigned char *pBuffer);
unsigned char RemoveFile(unsigned char *pBuffer);
unsigned char GetCapacity(void);
unsigned char GetFreeCapacity(void);
unsigned char SetFilePointer(unsigned long pointer);
unsigned char GetFirmwareVersion(void);


