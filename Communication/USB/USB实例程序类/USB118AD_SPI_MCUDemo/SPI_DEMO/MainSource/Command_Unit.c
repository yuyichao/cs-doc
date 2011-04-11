//============================================================================================================
//
//
//	本单元的功能描述：
//
//			实现的功能。
//
//
//	输出函数列表：
//			CMD_IdleSetEvent		被	Idle	任务调用，用于当接收到数据时把Command任务从睡眠中唤醒。
//
//===========================================================================================================
//	日期	|	动作	|   	单位       	|	描述	  |联系方式
//===========================================================================================================
//2008-07-28|	创建 	|	 西安达泰电子  	|	创建文件  |029-85272421  http://www.dataie.com
//============================================================================================================

//============================================================================================================
//		引入头文件
//============================================================================================================
#include	"Config.h"
#include	"ClockUnit.h"
#include	"SPI_Unit.h"
#include	"Command_Unit.h"


//============================================================================================================
//定义一些内部的全局变量
volatile	TCMD_CommandBlock	vCMD_CmdStaBuffer;
PCMD_CommandBlock	vCMD_pCommand;
PCMD_StatusBlock	vCMD_pStatus;
//============================================================================================================



//============================================================================================================
//************************************************************************************************************
//*
//*
//*			开始定义	内部函数	函数。
//*
//*
//************************************************************************************************************
//============================================================================================================
//函数名：		CMD_Init
//
//调用关系：	被	Command_Task	函数调用。
//
//功能描述：	初始化本单元。
//
//出口参数：	WriteBuffer		上层程序可用的写文件缓冲区。
//				ReadBuffer		上层程序可用的读文件缓冲区。
//
//返回值：		无。
//============================================================================================================
void	CMD_Init(DWORD*	WriteBuffer,	DWORD*	ReadBuffer)
{
	//根据设置，初始化通讯模块	Mem_AllocateUSBRAM
	SPI_Init();

	vCMD_pCommand	=	(PCMD_CommandBlock)&vCMD_CmdStaBuffer;
	vCMD_pStatus	=	(PCMD_StatusBlock)&vCMD_CmdStaBuffer;

	*WriteBuffer	=	(DWORD)&(vCMD_pCommand->Data[0]);
	*ReadBuffer		=	(DWORD)&(vCMD_pStatus->Data[0]);
}
//============================================================================================================
//函数名：		CMD_DoCommand
//
//调用关系：	被	具体的Command	函数调用。
//
//功能描述：	执行一次命令。
//
//入口参数：	无。
//
//返回值：		是否成功。
//============================================================================================================
BOOL	CMD_DoCommand(DWORD	CommandDataLength,	DWORD	ResDataLength,	DWORD	ResponseCode)
{
	WORD	LastCounter,	ThisCounter;

	vCMD_pCommand->Signature	=	cCMD_Signature;

	//首先，发送命令
	if	(!SPI_Send(vCMD_pCommand,	CommandDataLength))
		return	FALSE;
 	
	//等待命令发送完。
	while	(!SPI_CanSendNextPacket())
	{}

	//然后，启动接收应答的过程
	SPI_StartRead(vCMD_pStatus,	ResDataLength,	ResponseCode);

	//等待接受应答数据成功
	LastCounter	=	0;
	CLK_SetupTimer(cCLK_TimerCommand,	5000,	NULL);
	while	(1)
	{
		ThisCounter	=	SPI_GetReadCounter();
		if	(ThisCounter	>	LastCounter)
		{
			LastCounter	=	ThisCounter;
			CLK_SetupTimer(cCLK_TimerCommand,	500,	NULL);
			//如果读到的数据大于等于需要的应答数据长度，则说明应答成功
			if	(ThisCounter	>=	ResDataLength)
				return	TRUE;
		}
		else
			if	(CLK_OnTimer(cCLK_TimerCommand))
			{
				return	FALSE;
			}
	}
}
//============================================================================================================
//函数名：		CMD_WriteFile
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	写文件。
//
//入口参数：	DataLength	要写的数据长度
//
//返回值：		是否写成功。
//============================================================================================================
BOOL	CMD_WriteFile(	DWORD	DataLength	)
{
	vCMD_pCommand->Parameter[1]	=	DataLength;
	vCMD_pCommand->Parameter[2]	=	DataLength>>8;

	vCMD_pCommand->Command	=	cCMD_CMD_WRITE_FILE;

	if	(!CMD_DoCommand(66+	DataLength,	64,	cCMD_CMD_WRITE_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_WRITE_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_ReadFile
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	读文件。
//
//入口参数：	DataLength	要读的数据长度
//
//返回值：		是否读成功。
//============================================================================================================
BOOL	CMD_ReadFile(	DWORD*	DataLength	)
{
	WORD	i,	j;

	vCMD_pCommand->Parameter[0]	=	*DataLength;
	vCMD_pCommand->Parameter[1]	=	*DataLength>>8;

	vCMD_pCommand->Command	=	cCMD_CMD_READ_FILE;

	CMD_DoCommand(66,	64	+	*DataLength,	cCMD_CMD_READ_FILE_RSP);
	i	=	SPI_GetReadCounter();
	if	(i	<	63)
	{	return	FALSE;	}
	else
	{
		if	((vCMD_pStatus->Command	==	cCMD_CMD_READ_FILE_RSP)	&&	(vCMD_pStatus->Result))
		{
			j	=	vCMD_pStatus->Parameter[0]	+	vCMD_pStatus->Parameter[1]	*	256;
			if	(i	>=	(j+64))
			{
				*DataLength	=	j;
				return	TRUE;
			}
			else
			{	return	FALSE;	}
		}
		else
		{	return	FALSE;	}
	}
}
//============================================================================================================
//函数名：		CMD_CreateFile
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	创建文件。
//
//入口参数：	pFileName	文件名，必须8个字节
//				pExtName	扩展名，必须3个字节
//
//返回值：		是否创建成功。
//============================================================================================================
BOOL	CMD_CreateFile(	char*	pFileName,	char*	pExtName	)
{
	PFS_DirItem	pItem;
	DWORD	i;

	pItem	=	(PFS_DirItem)&(vCMD_pCommand->Parameter[0]);

	//清空
	for	(i=0;	i<11;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0x20;
	}
	for	(i=12;	i<63;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0;
	}

	//复制文件名和扩展名
	for	(i=0;	i<8;	i++)
	{
		if	(pFileName[i]	==	0)
			break;
		pItem->FileName[i]	=	pFileName[i];
	}
	for	(i=0;	i<3;	i++)
	{
		pItem->ExtName[i]	=	pExtName[i];
		if	(pExtName[i]	==	0)
			break;
	}

	pItem->Attribute		=	cFS_ATTR_ARCHIVE;
	pItem->ResForNT			=	0x0;
	pItem->CreateTimeTenth	=	88;
	pItem->CreateTime		=	0x4104;
	pItem->CreateDate		=	0x3908;
	pItem->LastAccDate		=	0x3908;
	pItem->StartClusterHI	=	0;
	pItem->Time				=	0x4104;
	pItem->Data				=	0x3908;
	pItem->StartClusterLO	=	0;
	pItem->FileLength		=	0;


	vCMD_pCommand->Command	=	cCMD_CMD_CREATE_FILE;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_CREATE_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_CREATE_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_OpenFile
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	打开文件。
//
//入口参数：	pFileName	文件名，必须8个字节
//				pExtName	扩展名，必须3个字节
//
//返回值：		是否打开成功。
//============================================================================================================
BOOL	CMD_OpenFile(	char*	pFileName,	char*	pExtName	)
{
	DWORD	i;

	//复制文件名和扩展名
	for	(i=0;	i<8;	i++)
		vCMD_pCommand->Parameter[i]	=	pFileName[i];
	for	(i=0;	i<3;	i++)
		vCMD_pCommand->Parameter[i+8]	=	pExtName[i];

	vCMD_pCommand->Command	=	cCMD_CMD_OPEN_FILE;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_OPEN_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_OPEN_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_SetPointer
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	写文件。
//
//入口参数：	FilePointer	文件指针
//
//返回值：		是否写成功。
//============================================================================================================
BOOL	CMD_SetPointer(	DWORD	FilePointer	)
{
	vCMD_pCommand->Parameter[1]	=	FilePointer;
	vCMD_pCommand->Parameter[2]	=	FilePointer>>8;
	vCMD_pCommand->Parameter[3]	=	FilePointer>>16;
	vCMD_pCommand->Parameter[4]	=	FilePointer>>24;

	vCMD_pCommand->Command	=	cCMD_CMD_SET_FILE_POINTER;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_SET_FILE_POINTER_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_SET_FILE_POINTER_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_Detect
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	检查，是否有U盘。
//
//入口参数：	无
//
//返回值：		是否有U盘。
//============================================================================================================
BOOL	CMD_Detect(	 void	)
{
	DWORD	i;

	vCMD_pCommand->Command	=	cCMD_CMD_DETECT_DEVICE;
	for	(i=0;i<63;i++)
	{
		vCMD_pCommand->Parameter[i]	=	i;
	}

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_DETECT_DEVICE_RSP))
	{	return	FALSE;	}
	else
	{
		//return	vCMD_pStatus->Result;
		/*if	(vCMD_pStatus->Result)
		{
			if	((vCMD_pStatus->Result)	!=	1)
			{
				vCMD_pStatus->Result	=	0;
			}
			//vCMD_pStatus->Result	=	1;
		}
		else
		{
			vCMD_pCommand->Parameter[0]	==	cCMD_CMD_DETECT_DEVICE_RSP;	
		}*/
		return	((vCMD_pStatus->Command	==	cCMD_CMD_DETECT_DEVICE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_GetVersion
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	得到版本号。
//
//出口参数：	无
//
//返回值：		是否有U盘。
//============================================================================================================
BOOL	CMD_GetVersion(	WORD*	pVersion	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_GET_VERSION;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_GET_VERSION_RSP))
	{	return	FALSE;	}
	else
	{
		if	(((vCMD_pStatus->Command	==	cCMD_CMD_GET_VERSION_RSP)	&&	(vCMD_pStatus->Result)))
		{
			*pVersion	=	(vCMD_pStatus->Parameter[0])<<8	+	vCMD_pStatus->Parameter[1];
			return	TRUE;
		}
		else
		{	return	FALSE;	}
	}
}
//============================================================================================================
//函数名：		CMD_InDir
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	进入子目录。
//
//入口参数：	pDirName	目录名，必须8个字节
//
//返回值：		是否成功。
//============================================================================================================
BOOL	CMD_InDir(	char*	pDirName	)
{
	DWORD	i;

	//复制文件名和扩展名
	for	(i=0;	i<8;	i++)
		vCMD_pCommand->Parameter[i]	=	pDirName[i];

	vCMD_pCommand->Command	=	cCMD_CMD_IN_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_IN_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_IN_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_OutDir
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	返回上级目录。
//
//入口参数：	无
//
//返回值：		是否成功。
//============================================================================================================
BOOL	CMD_OutDir(	 void	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_OUT_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_OUT_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_OUT_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_RootDir
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	返回根目录。
//
//入口参数：	无
//
//返回值：		是否成功。
//============================================================================================================
BOOL	CMD_RootDir(	 void	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_ROOT_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_ROOT_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_ROOT_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_MakeDir
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	创建目录。
//
//入口参数：	pDirName	目录名，必须8个字节
//
//返回值：		是否创建成功。
//============================================================================================================
BOOL	CMD_MakeDir(	char*	pDirName	)
{
	PFS_DirItem	pItem;
	DWORD	i;

	pItem	=	(PFS_DirItem)&(vCMD_pCommand->Parameter[0]);

	//清空
	for	(i=0;	i<11;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0x20;
	}
	for	(i=12;	i<63;	i++)
	{
		vCMD_pCommand->Parameter[i]	=	0;
	}

	//复制文件名和扩展名
	for	(i=0;	i<8;	i++)
	{
		if	(pDirName[i]	==	0)
			break;
		pItem->FileName[i]	=	pDirName[i];
	}
	pItem->Attribute		=	cFS_ATTR_DIRECTORY;
	pItem->ResForNT			=	0;
	pItem->CreateTimeTenth	=	88;
	pItem->CreateTime		=	0x4104;
	pItem->CreateDate		=	0x3908;
	pItem->LastAccDate		=	0x3908;
	pItem->StartClusterHI	=	0;
	pItem->Time				=	0x4104;
	pItem->Data				=	0x3908;
	pItem->StartClusterLO	=	0;
	pItem->FileLength		=	0;


	vCMD_pCommand->Command	=	cCMD_CMD_MAKE_DIR;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_MAKE_DIR_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_MAKE_DIR_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_Remove
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	删除文件。
//
//入口参数：	pFileName	文件名，必须8个字节
//				pExtName	扩展名，必须3个字节
//
//返回值：		是否删除成功。
//============================================================================================================
BOOL	CMD_Remove(	char*	pFileName,	char*	pExtName	)
{
	DWORD	i;

	//复制文件名和扩展名
	for	(i=0;	i<8;	i++)
		vCMD_pCommand->Parameter[i]	=	pFileName[i];
	for	(i=0;	i<3;	i++)
		vCMD_pCommand->Parameter[i+8]	=	pExtName[i];

	vCMD_pCommand->Command	=	cCMD_CMD_REMOVE_FILE;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_REMOVE_FILE_RSP))
	{	return	FALSE;	}
	else
	{
		return	((vCMD_pStatus->Command	==	cCMD_CMD_REMOVE_FILE_RSP)	&&	(vCMD_pStatus->Result));
	}
}
//============================================================================================================
//函数名：		CMD_GetCapacity
//
//调用关系：	被	外部函数	调用。
//
//功能描述：	得到U盘容量和剩余空间。
//
//出口参数：	无
//
//返回值：		U盘容量和剩余空间。
//============================================================================================================
BOOL	CMD_GetCapacity(	DWORD*	AvailableSpace,	DWORD*	FreeSpace	)
{

	vCMD_pCommand->Command	=	cCMD_CMD_GET_CAPACITY;

	if	(!CMD_DoCommand(66,	64,	cCMD_CMD_GET_CAPACITY_RSP))
	{	return	FALSE;	}
	else
	{
		if	(((vCMD_pStatus->Command	==	cCMD_CMD_GET_CAPACITY_RSP)	&&	(vCMD_pStatus->Result)))
		{
			*AvailableSpace	=	(vCMD_pStatus->Parameter[0])	+	(vCMD_pStatus->Parameter[1])<<8
							+	(vCMD_pStatus->Parameter[2])<<16	+	(vCMD_pStatus->Parameter[3])<<24;
			*FreeSpace		=	(vCMD_pStatus->Parameter[4])	+	(vCMD_pStatus->Parameter[5])<<8
							+	(vCMD_pStatus->Parameter[6])<<16	+	(vCMD_pStatus->Parameter[7])<<24;
			return	TRUE;
		}
		else
		{	return	FALSE;	}
	}
}
//============================================================================================================



//============================================================================================================
//
//			End	of	File
//
//============================================================================================================
