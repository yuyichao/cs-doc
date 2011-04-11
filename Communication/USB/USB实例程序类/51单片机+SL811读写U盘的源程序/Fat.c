#include "common.h"
#include "Fat.h"
#include "SL811.H"
#include "TPBULK.H"
#include "HAL.H"
////////////////////////////////////////
extern SYS_INFO_BLOCK xdata DeviceInfo;
extern FILE_INFO xdata ThisFile;
extern unsigned char xdata DBUF[BUFFER_LENGTH];
unsigned char xdata FATBUF[512];
////////////////////////////////////////
unsigned long FirstSectorofCluster(unsigned int clusterNum)
{
	unsigned long temp;
	temp=clusterNum-2;
	temp=temp*DeviceInfo.BPB_SecPerClus;
	temp=temp+DeviceInfo.FirstDataSector;
	return temp;
}

unsigned int ThisFatSecNum(unsigned int clusterNum)
{
   unsigned int temp;
   temp=clusterNum*2;
   temp=temp/DeviceInfo.BPB_BytesPerSec;
   temp=temp+DeviceInfo.FatStartSector;
   return temp;
}

unsigned int ThisFatEntOffset(unsigned int clusterNum)
{
	unsigned int temp1,temp2;
	temp1=2*clusterNum;
	temp2=temp1/DeviceInfo.BPB_BytesPerSec;
	temp1=temp1-temp2*DeviceInfo.BPB_BytesPerSec;
	return temp1;
}

unsigned int GetNextClusterNum(unsigned int clusterNum)
{
	unsigned int xxgFatSecNum,xxgFatEntOffset;
	
	xxgFatSecNum=ThisFatSecNum(clusterNum);
	xxgFatEntOffset=ThisFatEntOffset(clusterNum);
	//ThisFile.FatSectorPointer=xxgFatSecNum;
	if(ThisFile.FatSectorPointer!=xxgFatSecNum)
	{	
		
		if(!RBC_Read(xxgFatSecNum,1,FATBUF))
			return 0xFFFF;
		ThisFile.FatSectorPointer=xxgFatSecNum;
	}
	
	///////////////////////////////////////////////////
	clusterNum=FATBUF[xxgFatEntOffset+1];
	clusterNum=clusterNum<<8;
	clusterNum+=FATBUF[xxgFatEntOffset];	
	return clusterNum;
}

unsigned char DeleteClusterLink(unsigned int clusterNum)
{
	//unsigned int nextClusterNum;
	unsigned int xxgFatSecNum,xxgFatEntOffset;
	//nextClusterNum=GetNextClusterNum(clusterNum);
	////////////////////////////////////////////
	//nextClusterNum=clusterNum;
	while((clusterNum>1)&&(clusterNum<0xfff0))
	{
	xxgFatSecNum=ThisFatSecNum(clusterNum);
	xxgFatEntOffset=ThisFatEntOffset(clusterNum);
	if(RBC_Read(xxgFatSecNum,1,DBUF))
		{
		clusterNum=DBUF[xxgFatEntOffset+1];
		clusterNum=clusterNum<<8;
		clusterNum+=DBUF[xxgFatEntOffset];	
		//return clusterNum;
		}
	else
		return FALSE;
	DBUF[xxgFatEntOffset]=0x00;
	DBUF[xxgFatEntOffset+1]=0x00;	
	//DelayMs(5);
	if(!RBC_Write(xxgFatSecNum,1,DBUF))
		return FALSE;
	//DelayMs(5);
	if(!RBC_Write(xxgFatSecNum+DeviceInfo.BPB_FATSz16,1,DBUF))
		return FALSE;
	////////////////////////////////////////////
	}
	return TRUE;
}

unsigned int GetClusterNumFromSectorNum(unsigned long sectorNum)
{
	unsigned long temp;
	temp=sectorNum-DeviceInfo.FirstDataSector;
	temp=temp/DeviceInfo.BPB_SecPerClus;
	temp=temp+2;
	return (unsigned int)temp;
}
/*
unsigned long GetSecNumFromPointer(void)
{
	unsigned int clusterNum,clusterSize;
	unsigned long temp,pointer;
	pointer=ThisFile.FilePointer;
	clusterSize=DeviceInfo.BPB_SecPerClus*DeviceInfo.BPB_BytesPerSec;
	clusterNum=ThisFile.StartCluster;
	while(pointer>clusterSize)
	{
		pointer-=clusterSize;	
		clusterNum=GetNextClusterNum(clusterNum);
	}
	temp=FirstSectorofCluster(clusterNum)+pointer/DeviceInfo.BPB_BytesPerSec;
	return temp;
}
*/
unsigned char GoToPointer(unsigned long pointer)
{
	//unsigned char temp;
	unsigned int clusterSize;
	//unsigned long temp;
	//pointer=ThisFile.FilePointer;
	clusterSize=DeviceInfo.BPB_SecPerClus*DeviceInfo.BPB_BytesPerSec;
	ThisFile.ClusterPointer=ThisFile.StartCluster;
	while(pointer>clusterSize)
	{
		pointer-=clusterSize;	
		ThisFile.ClusterPointer=GetNextClusterNum(ThisFile.ClusterPointer);
		if(ThisFile.ClusterPointer==0xffff)
		{
		return FALSE;
		}
	}
	ThisFile.SectorofCluster=pointer/DeviceInfo.BPB_BytesPerSec;
	ThisFile.SectorPointer=FirstSectorofCluster(ThisFile.ClusterPointer)+ThisFile.SectorofCluster;
	ThisFile.OffsetofSector=pointer-ThisFile.SectorofCluster*DeviceInfo.BPB_BytesPerSec;
	ThisFile.FatSectorPointer=0;
	return TRUE;
	
}

unsigned int GetFreeCusterNum(void)
{
	unsigned int clusterNum,i;
	unsigned long sectorNum;
	clusterNum=0;
	sectorNum=DeviceInfo.FatStartSector;
	while(sectorNum<DeviceInfo.BPB_FATSz16+DeviceInfo.FatStartSector)
	{
		
		if(!RBC_Read(sectorNum,1,DBUF))
			return 0x0;
		for(i=0;i<DeviceInfo.BPB_BytesPerSec;i=i+2)
		  	{
		  	 //clusterNum++;	
		  	 
		  	 if((DBUF[i]==0)&&(DBUF[i+1]==0))
		  	 	{	
		  	 	DBUF[i]=0xff;
		  	 	DBUF[i+1]=0xff;
		  	 	//DelayMs(10);
		  	 	if(!RBC_Write(sectorNum,1,DBUF))
		  	 		return 0x00;
		  	 	//DelayMs(10);
		  	 	if(!RBC_Write(sectorNum+DeviceInfo.BPB_FATSz16,1,DBUF))
		  	 		return 0x00;
		  	 	
		  	 	return	clusterNum; 
		  	 	}
		  	 clusterNum++;
		  	}	
				
		sectorNum=2*clusterNum/DeviceInfo.BPB_BytesPerSec+DeviceInfo.FatStartSector;	
		//clusterNum+=DeviceInfo.BPB_BytesPerSec;
		//DelayMs(10);
	}
	
	return 0x0;
}

unsigned int CreateClusterLink(unsigned int currentCluster)
{
	unsigned int newCluster;
	unsigned int xxgFatSecNum,xxgFatEntOffset;
	
	newCluster=GetFreeCusterNum();
		
	xxgFatSecNum=ThisFatSecNum(currentCluster);
	xxgFatEntOffset=ThisFatEntOffset(currentCluster);
	if(RBC_Read(xxgFatSecNum,1,DBUF))
		{
		//clusterNum=DBUF[xxgFatEntOffset+1];
		//clusterNum=clusterNum<<8;
		//clusterNum+=DBUF[xxgFatEntOffset];	
		//return clusterNum;
		DBUF[xxgFatEntOffset]=newCluster;
		DBUF[xxgFatEntOffset+1]=newCluster>>8;
		//DelayMs(5);
		if(!RBC_Write(xxgFatSecNum,1,DBUF))
			return 0x00;
		//DelayMs(10);
		if(!RBC_Write(xxgFatSecNum+DeviceInfo.BPB_FATSz16,1,DBUF))
			return 0x00;
		}
	else
		return 0x00;
	
	return newCluster;
}
