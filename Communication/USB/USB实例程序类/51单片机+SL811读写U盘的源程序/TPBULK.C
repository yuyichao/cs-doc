#include "common.h"
#include "SL811.H"
#include "TPBULK.H"
#include "HAL.H"
#include "HPI.H"
//#include "common.h"

//////////////////////////////////
extern unsigned char xdata DBUF[BUFFER_LENGTH];
extern XXGPKG usbstack;
extern SYS_INFO_BLOCK xdata DeviceInfo;
extern FILE_INFO xdata ThisFile;

TPBLK_STRUC idata TPBulk_Block;
#define     TPBulk_CBW			TPBulk_Block.TPBulk_CommandBlock
#define	    CBW_wXferLen		TPBulk_CBW.dCBW_DataXferLen
#define	    RBC_CDB			TPBulk_CBW.cdbRBC
#define     RBC_LUN			TPBulk_CBW.bCBW_LUN
#define     TPBulk_CSW			TPBulk_Block.TPBulk_CommandStatus

///////////////////////////////////////////////////////////////////////////
unsigned char EnumMassDev(void)
{
	
	PMBR_BLOCK pMBR;
	PBPB_BLOCK pBPB;
	////////////////////////////////////////////////////
	if(!SPC_Inquiry())
		return FALSE;
	//if(!SPC_READLONG())
	//	return FALSE;
	if(!SPC_TestUnit())
		return FALSE;
	if(!SPC_LockMedia())
		return FALSE;
	if(!SPC_RequestSense())
		return FALSE;
	if(!SPC_TestUnit())
		return FALSE;
	if(!RBC_ReadCapacity())
		return FALSE;
	////////////////////////////////////////////////////
	pMBR=(PMBR_BLOCK)DBUF;
	DeviceInfo.BPB_BytesPerSec=512; //ÔÝ¼ÙÉèÎª512
	//if(!RBC_Read(0x0,1,DBUF))
	//	return FALSE;
	
	if(!SPC_RequestSense())
		return FALSE;
	if(!SPC_TestUnit())
		return FALSE;
	if(!RBC_ReadCapacity())
		return FALSE;
	if(!RBC_Read(0x0,1,DBUF))
		return FALSE;
	//////////////////////////////////
	if(DBUF[0]==0xeb||DBUF[0]==0xe9)
		{
		DeviceInfo.StartSector=0;
		//DeviceInfo.TotalSector=SwapINT32(pMBR->TotalSector);
		}
	else
		{
	//////////////////////////////////
		DeviceInfo.StartSector=SwapINT32(pMBR->StartSector);
		//DeviceInfo.TotalSector=SwapINT32(pMBR->TotalSector);
		}
	///////////////////////////////////////////////////////
	pBPB=(PBPB_BLOCK)DBUF;
	if(!RBC_Read(DeviceInfo.StartSector,1,DBUF))
		return FALSE;
	DeviceInfo.BPB_BytesPerSec=SwapINT16(pBPB->BPB_BytesPerSec);
	DeviceInfo.BPB_SecPerClus=pBPB->BPB_SecPerClus;
	DeviceInfo.BPB_NumFATs=pBPB->BPB_NumFATs;
	DeviceInfo.BPB_RootEntCnt=SwapINT16(pBPB->BPB_RootEntCnt);
	DeviceInfo.BPB_TotSec16=SwapINT16(pBPB->BPB_TotSec16);
	DeviceInfo.BPB_FATSz16=SwapINT16(pBPB->BPB_FATSz16);
	DeviceInfo.BPB_TotSec32=SwapINT32(pBPB->BPB_TotSec32);
	DeviceInfo.FatStartSector=DeviceInfo.StartSector+1;
	DeviceInfo.RootStartSector=DeviceInfo.StartSector+2*DeviceInfo.BPB_FATSz16+1;
	//DeviceInfo.DataStartSector=DeviceInfo.RootStartSector+DeviceInfo.BPB_RootEntCnt;
	DeviceInfo.FirstDataSector=DeviceInfo.FatStartSector+2*DeviceInfo.BPB_FATSz16+32;
	//DeviceInfo.FirstSectorofCluster=;
	///////////////////////////////////////////////////////
	ThisFile.bFileOpen=0;	
	///////////////////////////////////////////////////////
	return TRUE;
}

unsigned char TPBulk_GetMaxLUN(void)
{
	
	usbstack.setup.bmRequest=0xa1;
	usbstack.setup.bRequest=0xfe;
	usbstack.setup.wValue=0;
	usbstack.setup.wIndex=0;
	usbstack.setup.wLength=1;
	usbstack.buffer=DBUF;
	return ep0Xfer();

}

unsigned char SPC_Inquiry(void)
{
#define cdbInquirySPC RBC_CDB.SpcCdb_Inquiry
	//unsigned char len;
	//unsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0x60a624de;
	TPBulk_CBW.dCBW_DataXferLen=0x24000000;
	TPBulk_CBW.bCBW_Flag=0x80;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.bCBW_CDBLen=sizeof(INQUIRY_SPC);
	
	/////////////////////////////////
	cdbInquirySPC.OperationCode=SPC_CMD_INQUIRY;
	cdbInquirySPC.EnableVPD=0;
	cdbInquirySPC.CmdSupportData=0;
	cdbInquirySPC.PageCode=0;
	cdbInquirySPC.AllocationLen=0x24;
	cdbInquirySPC.Control=0;
	////////////////////////////////
	//if(!epBulkRcv(DBUF,5))
	//	return FALSE;
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(150);
	//len=36;
	if(!epBulkRcv(DBUF,38))
		return FALSE;
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
	////////////////////////////////
	return TRUE;	
#undef cdbInquirySPC
}

unsigned char SPC_READLONG(void)
{
#define cdbReadLong RBC_CDB.SpcCdb_ReadLong	
	//nsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0x60a624de;
	TPBulk_CBW.dCBW_DataXferLen=0xfc000000;
	TPBulk_CBW.bCBW_Flag=0x80;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.bCBW_CDBLen=sizeof(READ_LONG_CMD);
	
	/////////////////////////////////////
	cdbReadLong.OperationCode=SPC_CMD_READLONG;
	cdbReadLong.LogicalUnitNum=0;
	cdbReadLong.AllocationLen=0xfc;
	//////////////////////////////////////
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(5);
	//len=36;
	if(!epBulkRcv(DBUF,0xfc))
		return FALSE;
	
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
  ////////////////////////////
  return TRUE;
#undef cdbReadLong
}
unsigned char SPC_RequestSense(void)
{
#define cdbRequestSenseSPC RBC_CDB.SpcCdb_RequestSense	
	//unsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0x60a624de;
	TPBulk_CBW.dCBW_DataXferLen=0x0e000000;
	TPBulk_CBW.bCBW_Flag=0x80;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.bCBW_CDBLen=sizeof(REQUEST_SENSE_SPC);
		
	/////////////////////////////////////
	cdbRequestSenseSPC.OperationCode=SPC_CMD_REQUESTSENSE;
	cdbRequestSenseSPC.AllocationLen=0x0e;
	//////////////////////////////////////
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(5);
	//len=36;
	if(!epBulkRcv(DBUF,18))
		return FALSE;
	
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
/////////////////////////////
	return TRUE;
	
#undef cdbRequestSenseSPC
}
unsigned char SPC_TestUnit(void)
{
#define cdbTestUnit RBC_CDB.SpcCdb_TestUnit	
	//unsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0x60a624de;
	TPBulk_CBW.dCBW_DataXferLen=0x00000000;
	TPBulk_CBW.bCBW_Flag=0x00;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.bCBW_CDBLen=sizeof(TEST_UNIT_SPC);
	/////////////////////////////////////
	cdbTestUnit.OperationCode=SPC_CMD_TESTUNITREADY;
	//////////////////////////////////////
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(5);
	
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
#undef cdbTestUnit
////////////////////////////
	return TRUE;
}
unsigned char SPC_LockMedia(void)
{
#define cdbLockSPC RBC_CDB.SpcCdb_Remove	
	//unsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0x60a624de;
	TPBulk_CBW.dCBW_DataXferLen=0x00000000;
	TPBulk_CBW.bCBW_Flag=0x00;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.bCBW_CDBLen=sizeof(MEDIA_REMOVAL_SPC);
	///////////////////////////////////////////
	cdbLockSPC.OperationCode=SPC_CMD_PRVENTALLOWMEDIUMREMOVAL;
	cdbLockSPC.Prevent=1;
	///////////////////////////////////////////
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(5);
	
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
#undef cdbLockSPC
/////////////////////////////
	return TRUE;
}
unsigned char RBC_ReadCapacity(void)
{
#define cdbReadCap RBC_CDB.RbcCdb_ReadCapacity	
	//unsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0x60a624de;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.dCBW_DataXferLen=0x08000000;
	TPBulk_CBW.bCBW_Flag=0x80;
	
	TPBulk_CBW.bCBW_CDBLen=sizeof(READ_CAPACITY_RBC);
	/////////////////////////////////////
	cdbReadCap.OperationCode=RBC_CMD_READCAPACITY;
	/////////////////////////////////////
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(10);
	//len=36;
	if(!epBulkRcv(DBUF,8))
		return FALSE;
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
#undef cdbReadCap
/////////////////////////////
	return TRUE;
}
unsigned char RBC_Read(unsigned long lba,unsigned char len,unsigned char *pBuffer)
{
#define cdbRead RBC_CDB.RbcCdb_Read	
	//unsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0x60a624de;
	TPBulk_CBW.dCBW_DataXferLen=SwapINT32(len*DeviceInfo.BPB_BytesPerSec);
	TPBulk_CBW.bCBW_Flag=0x80;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.bCBW_CDBLen=sizeof(READ_RBC);
	/////////////////////////////////////
	cdbRead.OperationCode=RBC_CMD_READ10;
	cdbRead.VendorSpecific=0;
	cdbRead.LBA.LBA_W32=lba;
	cdbRead.XferLength=len;
	//cdbRead.Reserved1[0]=0;
	//cdbRead.Reserved1[1]=0;
	//cdbRead.Reserved1[2]=0x40;
	//////////////////////////////////////
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(5);
	//len=36;
	if(!epBulkRcv(pBuffer,len*DeviceInfo.BPB_BytesPerSec))
		return FALSE;
	//DelayMs(1);
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
#undef cdbRead
/////////////////////////////
	return TRUE;
}

unsigned char RBC_Write(unsigned long lba,unsigned char len,unsigned char *pBuffer)
{
#define cdbWrite RBC_CDB.RbcCdb_Write	
	//len=2;
	//unsigned int idata temp;
	//unsigned char i;
	//len=1;
	//SPC_TestUnit();
	//unsigned char retStatus=FALSE;
	TPBulk_CBW.dCBW_Signature=CBW_SIGNATURE;
	TPBulk_CBW.dCBW_Tag=0xb4D977c1;
	TPBulk_CBW.dCBW_DataXferLen=SwapINT32(len*DeviceInfo.BPB_BytesPerSec);
	TPBulk_CBW.bCBW_Flag=0x0;
	TPBulk_CBW.bCBW_LUN=0;
	TPBulk_CBW.bCBW_CDBLen=sizeof(WRITE_RBC);
	/////////////////////////////////////
	cdbWrite.OperationCode=RBC_CMD_WRITE10;
	cdbWrite.VendorSpecific=0;
	cdbWrite.LBA.LBA_W32=lba;
	cdbWrite.XferLength=len;
	cdbWrite.Reserved2=0;
	cdbWrite.Control=0;
	//////////////////////////////////////
	if(!epBulkSend((unsigned char *)&TPBulk_CBW,sizeof(TPBulk_CBW)))
		return FALSE;
	DelayMs(15);
	
	if(!epBulkSend(pBuffer,DeviceInfo.BPB_BytesPerSec))
		return FALSE;
	//DelayMs(10);
	if(!epBulkRcv((unsigned char *)&TPBulk_CSW,13))
		return FALSE;
	
#undef cdbWrite

/////////////////////////////
	return TRUE;
}
