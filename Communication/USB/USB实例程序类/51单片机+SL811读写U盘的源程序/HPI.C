#include "common.h"
#include "TPBULK.H"
#include "HPI.H"
#include "HAL.H"
#include "Fat.h"

extern XXGFLAGS bdata bXXGFlags;
extern unsigned char xdata DBUF[BUFFER_LENGTH];
//////////////////////////////////////////
//UART_CMD_BLOCK xdata inblock;
unsigned char xdata UARTBUF[UARTBUF_LENGTH];
SYS_INFO_BLOCK xdata DeviceInfo;
UART_CMD_BLOCK xdata UartCmdBlock;
UART_RSP_BLOCK xdata UartRspBlock;
FILE_INFO xdata ThisFile;
//////////////////////////////////////////
void UartSendRsp(void)
{
	unsigned int i;
	unsigned char *pBuf=(unsigned char *)&UartRspBlock;
	for(i=0;i<64;i++)
		ComSendByte(*(pBuf+i));
	if(UartRspBlock.len!=0){
		for(i=0;i<UartRspBlock.len;i++)
			ComSendByte(*(UartRspBlock.pbuffer+i));
	   }
}

unsigned char UartHandler(void)
{
  unsigned char retStatus;
  unsigned int len;
  unsigned long pointer;
  //unsigned char *pBuf=(unsigned char *)&UartRspBlock;

  //for(i=0;i<64;i++)
	//*(pBuf+i)=0;
   //UartRspBlock.result=1;
   UartRspBlock.errcode=ERC_OK;
   UartRspBlock.len=0;
   ///////////////////////////
  switch(UartCmdBlock.cmd)
  {
    case CMD_RESET:
    
    	break;
    case CMD_DETECT_DEVICE:
    	ThisFile.bFileOpen=0;	
    	retStatus=DetectDevice();
    	UartRspBlock.cmd=CMD_DETECT_DEVICE_RSP;
    	break;
    case CMD_OPEN_FILE:
    	retStatus=OpenFile(UartCmdBlock.CmdBlock.Cmd_OpenFile.filename);
    	UartRspBlock.cmd=CMD_OPEN_FILE_RSP;
    	break;
    case CMD_CREATE_FILE:
    	
    	retStatus=OpenFile(UartCmdBlock.CmdBlock.Cmd_OpenFile.filename);
    	if(UartRspBlock.errcode==ERC_FILENOTFOUND)
    		{	
	    	retStatus=CreateFile(UartCmdBlock.CmdBlock.Cmd_CreateFile.item);
    		}
    	UartRspBlock.cmd=CMD_CREATE_FILE_RSP;
    	break;
    case CMD_READ_FILE:
    	len=SwapINT16(UartCmdBlock.CmdBlock.Cmd_ReadFile.readLength);
	retStatus=ReadFile(len,UARTBUF);
    	UartRspBlock.cmd=CMD_READ_FILE_RSP;
    	break;
    case CMD_WRITE_FILE:
    	len=SwapINT16(UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength);
    	retStatus=WriteFile(len,UARTBUF);
    	UartRspBlock.cmd=CMD_WRITE_FILE_RSP;
    	break;
    case CMD_LIST:
    	ThisFile.bFileOpen=0;	
    	retStatus=List();
    	UartRspBlock.cmd=CMD_LIST_RSP;
    	break;
    case CMD_REMOVE_FILE:
    	ThisFile.bFileOpen=0;	
    	retStatus=RemoveFile(UartCmdBlock.CmdBlock.Cmd_RemoveFile.filename);
    	UartRspBlock.cmd=CMD_REMOVE_FILE_RSP;
    	break;
    case CMD_GET_CAPACITY:
    	ThisFile.bFileOpen=0;	
    	retStatus=GetCapacity();
    	UartRspBlock.cmd=CMD_GET_CAPACITY_RSP;
    	break;
    case CMD_GET_FREE_CAPACITY:
    	ThisFile.bFileOpen=0;	
    	retStatus=GetFreeCapacity();
    	UartRspBlock.cmd=CMD_GET_FREE_CAPACITY_RSP;
    	break;
    case CMD_SET_FILE_POINTER:
	pointer=SwapINT32(UartCmdBlock.CmdBlock.Cmd_SetFilePointer.pointer);
	retStatus=SetFilePointer(pointer);  
	UartRspBlock.cmd=CMD_SET_FILE_POINTER_RSP;  	
    	break;
    case CMD_GET_VERSION:
	ThisFile.bFileOpen=0;
	retStatus=GetFirmwareVersion();   
	UartRspBlock.cmd=CMD_GET_VERSION_RSP; 
    	break;
    default:
    	bXXGFlags.bits.bCOM_ERR=1;
    	return FALSE;
  }
  
  ///////////////////////////////
  UartRspBlock.result=retStatus;
  UartRspBlock.pbuffer=UARTBUF;
  UartSendRsp();
  return TRUE;
  //return retStatus;
}
unsigned char DetectDevice(void)
{
#define RspBlockDetectDevice UartRspBlock.RspBlock.Rsp_DetectDevice
	return bXXGFlags.bits.SLAVE_IS_ATTACHED;
	
#undef RspBlockDetectDevice
}

unsigned char List(void)
{
#define RspBlockList UartRspBlock.RspBlock.Rsp_List
	unsigned int item,i;
	unsigned char j,k,bstop,sector;
	//RspBlockList.errcode=ERC_NODEVICE;
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	item=0;
	bstop=0;
	//RspBlockList.result=0x1;
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt;sector++)
	    {   
		//////////////////////////////////////////////////
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			//item=0;
			UartRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;			
			}
		///////////////////////////////////////////////////
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if(DBUF[i]==0x00)
				{bstop=1;
				break;}
			else if(DBUF[i]==0xE5)
				continue;
			else if((DBUF[i]&0x40==0x40)&&(DBUF[i+11]==0xff))
				{
				j=DBUF[i]&0x0F;
				j=j+1;
				for(k=0;k<j*32;k++)
					UARTBUF[item*32+k]=DBUF[i+k];
				i=i+(j-1)*32;
				item=item+j;
				}
			else
				{
				for(k=0;k<32;k++)
					UARTBUF[item*32+k]=DBUF[i+k];
				item=item+1;
				}
			}
		///////////////////////////////////////////////////////
		if(bstop==1)break;
		
	    }
		
	//pBuf=(PREAD_CAPACITY_RSP)DBUF;
	///////////////////////////////////////////
	RspBlockList.len=SwapINT16(item*32);
	UartRspBlock.len=item*32;
	return TRUE;
	
#undef RspBlockList

}
unsigned char OpenFile(unsigned char *pBuffer)
{
#define RspBlockOpenFile UartRspBlock.RspBlock.Rsp_OpenFile
	unsigned int i;
	unsigned char j,bstop,sector;
	PDIR_INFO pDirInfo;
	
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	
	ThisFile.bFileOpen=0;
	//RspBlockOpenFile.result=0x0;
	//RspBlockOpenFile.errcode=ERC_OK;
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt;sector++)
	    {   
		//////////////////////////////////////////////////
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			UartRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;	
			
			}
		///////////////////////////////////////////////////
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if(DBUF[i]==0x00)
				{
				UartRspBlock.errcode=ERC_FILENOTFOUND;
				//UartRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;	
				}
			///////////////////////////////////////////
			j=0;
			while(DBUF[i+j]==*(pBuffer+j))
				{
				 j=j+1;
				 if(j>10)
				 	break;
				}
			
			if(j>10)
			    {
			    for(j=0;j<32;j++)
			    	RspBlockOpenFile.item[j]=DBUF[i+j];
			    //RspBlockOpenFile.result=0x1;
			    ThisFile.bFileOpen=1;
			    bstop=1;
			     break;}
			
			}
		///////////////////////////////////////////////////////
		if(bstop==1)break;
		///////////////////////////////////////////////////////
		//if(DeviceInfo.BPB_RootEntCnt)
		
	    }
	    
	    if(sector>=DeviceInfo.BPB_RootEntCnt)
	    	{
	    	UartRspBlock.errcode=ERC_FILENOTFOUND;
	    	//UartRspBlock.errcode=ERC_DEVICEERR;
		return FALSE;		
	    	}
	////////////////////////////////////////////
	pDirInfo=(PDIR_INFO)RspBlockOpenFile.item;
	ThisFile.StartCluster=SwapINT16(pDirInfo->startCluster);
	ThisFile.LengthInByte=SwapINT32(pDirInfo->length);
	ThisFile.ClusterPointer=ThisFile.StartCluster;
	ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.StartCluster);
	ThisFile.OffsetofSector=0;
	ThisFile.SectorofCluster=0;
	//=ThisFatSecNum(clusterNum);
	//xxgFatEntOffset=ThisFatEntOffset(clusterNum);
	ThisFile.FatSectorPointer=0;
	//ThisFile.bFileOpen=1;
	ThisFile.pointer=0;
	///////////////////////////////////////////
	return TRUE;
#undef RspBlockOpenFile
}

unsigned char ReadFile(unsigned int readLength,unsigned char *pBuffer)
{
#define RspBlockReadFile UartRspBlock.RspBlock.Rsp_ReadFile
	unsigned int idata len,i;
	unsigned char bSuccess;
	//unsigned char sector;
	//unsigned long lba;
	
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	if(!ThisFile.bFileOpen)
	{
	UartRspBlock.errcode=ERC_FILENOTOPENED;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	ThisFile.bFileOpen=0;
	bSuccess=1;
	UartRspBlock.len=0;
	//lba=GetSecNumFromPointer();
	//cluster=GetClusterNumFromSectorNum(lba);
	//cluster=ThisFile.StartCluster;
	//lba=FirstSectorofCluster(ThisFile.StartCluster);
	//readLength=SwapINT16(UartCmdBlock.CmdBlock.Cmd_ReadFile.readLength);
	if(readLength>MAX_READ_LENGTH)
		{
		UartRspBlock.errcode=ERC_LENGTHEXCEED;
		return FALSE;	
		}
	if(readLength+ThisFile.pointer>ThisFile.LengthInByte)
		{
		UartRspBlock.errcode=ERC_LENGTHEXCEED;
		return FALSE;	
		}
	////////////////////////////////////////////
		
		///////////////////////////////////////////////////
		while(readLength>0)
		{
		   if(readLength+ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec)
		   	len=DeviceInfo.BPB_BytesPerSec;
		   else
		   	len=readLength+ThisFile.OffsetofSector;
		   
		   //////////////////////////////////////////////////////
		   if(ThisFile.OffsetofSector>0)
		   	{
		   	if(RBC_Read(ThisFile.SectorPointer,1,DBUF))
		   		{
		   		//ThisFile.OffsetofSector=len;
		   		len=len-ThisFile.OffsetofSector;
		   		for(i=0;i<len;i++)
		   			//UARTBUF[i]=DBUF[ThisFile.OffsetofSector+i];
		   			*(pBuffer+i)=DBUF[ThisFile.OffsetofSector+i];
		   		ThisFile.OffsetofSector=ThisFile.OffsetofSector+len;
		   		}
		   	else
		   		{
		   		UartRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;	
		   		}
		   	}
		   else
		   	{
		   		if(!RBC_Read(ThisFile.SectorPointer,1,pBuffer+UartRspBlock.len))
		   		{
		   		UartRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;	
		   		}
		   		ThisFile.OffsetofSector=len;
		   	}
		   ////////////////////////////////////////////////////////////
		  // if(ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec-1)
		  // 	ThisFile.OffsetofSector-=DeviceInfo.BPB_BytesPerSec;
		   readLength-=len;
		   UartRspBlock.len+=len;
		  // ThisFile.OffsetofSector=;
		   /////////////////////////////////////////////////////////
		   if(ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec-1)
		   {	
		   	ThisFile.OffsetofSector-=DeviceInfo.BPB_BytesPerSec;
		   	ThisFile.SectorofCluster+=1;
		   	if(ThisFile.SectorofCluster>DeviceInfo.BPB_SecPerClus-1)
		   	{
		   		ThisFile.SectorofCluster=0;
		 		 ThisFile.ClusterPointer=GetNextClusterNum(ThisFile.ClusterPointer);
		 		 if(ThisFile.ClusterPointer>0xffef)
		 		 	{
		 		 	   //RspBlockReadFile.errcode=ERC_REACHEND;
		   			   //RspBlockReadFile.result=0x0;
		   			   UartRspBlock.errcode=ERC_REACHEND;
					   return FALSE;	
		 		 	}
		 		 ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.ClusterPointer); 	
		   	}
		   	else
		   		ThisFile.SectorPointer=ThisFile.SectorPointer+1;
		    }
		   //////////////////////////////////////////////////////////////////
		}//end while
	
	
	ThisFile.bFileOpen=1;
	ThisFile.pointer+=UartRspBlock.len;
	//////////////////////////////////////////////
	RspBlockReadFile.readLength=SwapINT16(UartRspBlock.len);
	return TRUE;
#undef RspBlockReadFile
}

unsigned char CreateFile(unsigned char *pBuffer)
{
#define RspBlockCreateFile UartRspBlock.RspBlock.Rsp_CreateFile
	//unsigned long sectorNum;
	unsigned int sector,i,j;
	unsigned char bstop;
	PDIR_INFO pDirInfo;
	
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	//RspBlockCreateFile.result=0x1;
	//RspBlockCreateFile.errcode=ERC_OK;
	pDirInfo=(PDIR_INFO)pBuffer;
	///////// Search the file of the same name  ///////////
	//UartCmdBlock.CmdBlock.Cmd_CreateFile.filename[j]
	//if(!SPC_TestUnit())
	//	return FALSE;
	//if(!RBC_Read(1,1,DBUF))
	//	return FALSE;
	//if(!RBC_Write(1,1,DBUF))
	//	return FALSE;
	//////// Search the fat for a free cluster  ////////////
	pDirInfo->startCluster=SwapINT16(GetFreeCusterNum());
	
	if(pDirInfo->startCluster<0x2)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	pDirInfo->length=0;
	/////// Search a free space in the root dir space and build the item ///
	ThisFile.bFileOpen=0;
	bstop=0;
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt;sector++)
	    {   
		//////////////////////////////////////////////////
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			
			UartRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;	
			}
		///////////////////////////////////////////////////
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if((DBUF[i]==0x00)||(DBUF[i]==0xE5))
				{
				for(j=0;j<32;j++)
					//DBUF[i+j]=UartCmdBlock.CmdBlock.Cmd_CreateFile.item[j];
					DBUF[i+j]=*(pBuffer+j);
				if(!RBC_Write(DeviceInfo.RootStartSector+sector,1,DBUF))
		  	 		{
		  	 		UartRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;	
		  	 		}
				bstop=1;
				break;
				}
			}
		///////////////////////////////////////////////////////
		if(bstop==1)break;
		
	    }
	/////////////////////////////////////////////
	//pDirInfo=(PDIR_INFO)RspBlockOpenFile.item;
	//ThisFile.FatSectorPointer=ThisFatSecNum(ThisFile.StartCluster);
	ThisFile.StartCluster=SwapINT16(pDirInfo->startCluster);
	ThisFile.LengthInByte=0;
	ThisFile.ClusterPointer=ThisFile.StartCluster;
	ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.StartCluster);
	ThisFile.OffsetofSector=0;
	ThisFile.SectorofCluster=0;
	ThisFile.bFileOpen=1;
	ThisFile.pointer=0;
	ThisFile.FatSectorPointer=0;
	//////////////////////////////////////////////
	return TRUE;
#undef RspBlockCreateFile
}

unsigned char WriteFile(unsigned int writeLength,unsigned char *pBuffer)
{
#define RspBlockWriteFile UartRspBlock.RspBlock.Rsp_WriteFile
	unsigned int len,sector,i;
	PDIR_INFO pDirInfo;
	unsigned char bSuccess,bStop;
	
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	if(!ThisFile.bFileOpen)
	{
	UartRspBlock.errcode=ERC_FILENOTOPENED;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	//if(UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength>0)
	//	{
	//	for(i=UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength;i<MAX_WRITE_LENGTH;i++)
	//	UARTBUF[i]=0;
	//	}
	///////////////////////////////////////////////////////////
	ThisFile.bFileOpen=0;
	bSuccess=1;
	bStop=0;

	UartRspBlock.len=0;
	//if(UartCmdBlock.CmdBlock.Cmd_WriteFile.writeLength)
	while(writeLength>0)
	{
		if(writeLength+ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec)
		   	len=DeviceInfo.BPB_BytesPerSec;
		else
		   	len=writeLength+ThisFile.OffsetofSector;
		   
		 //////////////////////////////////////////////////////
		 if(ThisFile.OffsetofSector>0)
		 	{
		 	if(RBC_Read(ThisFile.SectorPointer,1,DBUF))
		   		{
		   		//ThisFile.OffsetofSector=len;
		   		len=len-ThisFile.OffsetofSector;
		   		for(i=0;i<len;i++)
		   			//DBUF[ThisFile.OffsetofSector+i]=UARTBUF[i];
		   			DBUF[ThisFile.OffsetofSector+i]=*(pBuffer+i);
		   		if(!RBC_Write(ThisFile.SectorPointer,1,DBUF))
		   			{
		   			UartRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;	
		   			}
		   		ThisFile.OffsetofSector=ThisFile.OffsetofSector+len;
		   		}
		   	else
		   		{
		   		UartRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;	
		   		}
		 	}
		 else
		 	{
		 	if(!RBC_Write(ThisFile.SectorPointer,1,pBuffer+UartRspBlock.len))
		   		{
		   		UartRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;	
		   		}
		   	ThisFile.OffsetofSector=len;
		 	}
		 /////////////////////////////////////////////////////
		 //if(ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec-1)
		 //  	ThisFile.OffsetofSector-=DeviceInfo.BPB_BytesPerSec;
		   writeLength-=len;
		   UartRspBlock.len+=len;
		   //ThisFile.LengthInByte+=len;
		/////////////更新文件指针 //////////////////////////////
		  if(ThisFile.OffsetofSector>DeviceInfo.BPB_BytesPerSec-1)
		   {	
		   	ThisFile.OffsetofSector-=DeviceInfo.BPB_BytesPerSec;
		   	ThisFile.SectorofCluster+=1;
		   	if(ThisFile.SectorofCluster>DeviceInfo.BPB_SecPerClus-1)
		   	{
		   		ThisFile.SectorofCluster=0;
		 		 ThisFile.ClusterPointer=CreateClusterLink(ThisFile.ClusterPointer);//GetNextClusterNum(ThisFile.ClusterPointer);
		 		 if(ThisFile.ClusterPointer==0x00)
		 		 	{
		 		 //	   RspBlockReadFile.errcode=ERC_REACHEND;
		   			//   RspBlockReadFile.result=0x0;
		   			UartRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;	
		 			}
		 		 ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.ClusterPointer); 	
		   	}
		   	else
		   		ThisFile.SectorPointer=ThisFile.SectorPointer+1;
		    }
		
	
	}//end while
	ThisFile.pointer+=UartRspBlock.len;
	///////////更新文件目录信息/////////////////////////////
	if(bSuccess==1)
	{
		
		for(sector=0;sector<DeviceInfo.BPB_RootEntCnt;sector++)
	    	{   
		//////////////////////////////////////////////////
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			UartRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;	
			}
		///////////////////////////////////////////////////
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			pDirInfo=(PDIR_INFO)(DBUF+i);
			
			if(pDirInfo->startCluster==SwapINT16(ThisFile.StartCluster))
				{
				if(ThisFile.pointer>ThisFile.LengthInByte)
					ThisFile.LengthInByte=ThisFile.pointer;
				//else
				//	ThisFile.pointer=;
				
				pDirInfo->length=SwapINT32(ThisFile.LengthInByte);
				if(!RBC_Write(DeviceInfo.RootStartSector+sector,1,DBUF))
		   		{
		   		UartRspBlock.errcode=ERC_DEVICEERR;
				return FALSE;	
		   		}
				 bStop=1;
				 break;
				}
			}
		if(bStop==1)
			break;
		////////////////////////////////////////////////////
	       }
	
	}
	/*
	if(bSuccess==0)
	{
	RspBlockWriteFile.errcode=ERC_DEVICEERR;
	RspBlockWriteFile.result=0x0;
	}
	*/
	UartRspBlock.len=0;
	ThisFile.bFileOpen=1;
	//////////////////////////////////////////////
	return TRUE;
#undef RspBlockWriteFile
}

unsigned char RemoveFile(unsigned char *pBuffer)
{
#define RspBlockRemoveFile UartRspBlock.RspBlock.Rsp_RemoveFile
	unsigned int sector,i;
	unsigned char bStop,j;
	PDIR_INFO pDirInfo;
	
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	//RspBlockRemoveFile.result=0x1;
	////////////// 清除目录/////////////////////////////////////
	for(sector=0;sector<DeviceInfo.BPB_RootEntCnt;sector++)
	    	{   
		//////////////////////////////////////////////////
		if(!RBC_Read(DeviceInfo.RootStartSector+sector,1,DBUF))
			{
			UartRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;	
			}
		///////////////////////////////////////////////////
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+32)
			{
			if(DBUF[i]==0x00)
				{
				UartRspBlock.errcode=ERC_FILENOTFOUND;
				return FALSE;	
				}
			///////////////////////////////////////////
			j=0;
			while(DBUF[i+j]==*(pBuffer+j))
				{
				 j=j+1;
				 if(j>10) break;
				 }//end while
			
			if(j>10)
			 	{	
			 	DBUF[i]=0xE5;
			 	pDirInfo=(PDIR_INFO)(DBUF+i);
			 	ThisFile.StartCluster=SwapINT16(pDirInfo->startCluster);
			 	DelayMs(15);
			 	if(!RBC_Write(DeviceInfo.RootStartSector+sector,1,DBUF))
					{
					UartRspBlock.errcode=ERC_DEVICEERR;
					return FALSE;	
					}
				//////////////////// 清除FAT中的纪录////////////////////////
				DelayMs(10);
				if(!DeleteClusterLink(ThisFile.StartCluster))
						{
						UartRspBlock.errcode=ERC_DEVICEERR;
						return FALSE;	
						}
			 	bStop=1;
			 	break;
			 	}
			
			}//end for
		if(bStop==1)
			break;
		
	       }//end search
	if(sector>=DeviceInfo.BPB_RootEntCnt)
		{
		UartRspBlock.errcode=ERC_FILENOTFOUND;
			return FALSE;	
		}
	//////////////////////////////////////////////
	return TRUE;
#undef RspBlockRemoveFile
}

unsigned char GetCapacity(void)
{
	unsigned int sectorNum,freesectorcnt,i;
	
#define RspBlockGetCapacity UartRspBlock.RspBlock.Rsp_GetCapacity
	PREAD_CAPACITY_RSP pBuf;
	
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	if(!RBC_ReadCapacity())
	{
	UartRspBlock.errcode=ERC_DEVICEERR;
	return FALSE;	
	}
	
	pBuf=(PREAD_CAPACITY_RSP)DBUF;
	RspBlockGetCapacity.disksize=SwapINT32((pBuf->LastLBA+1)*pBuf->BlockSize);
	////////////////////////////////////////////////////////////////////////
	sectorNum=DeviceInfo.FatStartSector;
	freesectorcnt=0;
	while(sectorNum<DeviceInfo.BPB_FATSz16+DeviceInfo.FatStartSector)
	{
		
		if(RBC_Read(sectorNum,1,DBUF))
		{
		  for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+2)
		  	{
		  	 //clusterNum++;	
		  	 
		  	 if((DBUF[i]==0xff)&&(DBUF[i+1]==0xff))
		  	 	{	
		  	 	freesectorcnt++;
		  	 	}
		  	// clusterNum++;
		  	}	
		}
		else
			{
			UartRspBlock.errcode=ERC_DEVICEERR;
			return FALSE;	
			}
		sectorNum++;
	}
	
	////////////////////////////////////////////////////////////////////////
	RspBlockGetCapacity.freedisksize=DeviceInfo.BPB_BytesPerSec*DeviceInfo.BPB_SecPerClus;
	RspBlockGetCapacity.freedisksize=freesectorcnt*RspBlockGetCapacity.freedisksize;
	RspBlockGetCapacity.freedisksize=SwapINT32(RspBlockGetCapacity.disksize)-RspBlockGetCapacity.freedisksize;
	RspBlockGetCapacity.freedisksize=SwapINT32(RspBlockGetCapacity.freedisksize);
		
	return TRUE;
#undef RspBlockGetCapacity
}

unsigned char GetFreeCapacity(void)
{
#define RspBlockGetCapacity UartRspBlock.RspBlock.Rsp_GetFreeCapacity
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	//////////////////////////////////////////////
	
	
	return TRUE;
#undef RspBlockGetFreeCapacity
}

unsigned char SetFilePointer(unsigned long pointer)
{
#define RspBlockSetFilePointer UartRspBlock.RspBlock.Rsp_SetFilePointer
	
	//ThisFile.FilePointer=UartCmdBlock.CmdBlock.Cmd_SetFilePointer.pointer;
	if(!bXXGFlags.bits.SLAVE_IS_ATTACHED)
	{
	UartRspBlock.errcode=ERC_NODEVICE;
	return FALSE;		
	}
	if(!ThisFile.bFileOpen)
	{
	UartRspBlock.errcode=ERC_FILENOTOPENED;
	return FALSE;		
	}
	///////////////////////////////////////////////////////////
	ThisFile.pointer=pointer;
	if(ThisFile.pointer>ThisFile.LengthInByte)
	{
	UartRspBlock.errcode=ERC_LENGTHEXCEED;
	return FALSE;	
	}
	
	if(!GoToPointer(ThisFile.pointer))
	{
	ThisFile.bFileOpen=0;
	UartRspBlock.errcode=ERC_DEVICEERR;
	return FALSE;	
	}
	//////////////////////////////////////////////
	return TRUE;

#undef RspBlockSetFilePointer
}

unsigned char GetFirmwareVersion(void)
{
   #define RspBlockGetVersion UartRspBlock.RspBlock.Rsp_GetVersion
   ////////////////////////////////////////////////////////////
   RspBlockGetVersion.version=0x0102;
   return TRUE;
   #undef RspBlockGetVersion
}

