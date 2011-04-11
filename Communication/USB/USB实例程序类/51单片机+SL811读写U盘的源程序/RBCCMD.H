
/*
// RBC commands
*/
#define RBC_CMD_FORMAT					0x04
#define RBC_CMD_READ10					0x28
#define RBC_CMD_READCAPACITY				0x25
#define RBC_CMD_STARTSTOPUNIT				0x1B
#define RBC_CMD_SYNCCACHE				0x35
#define RBC_CMD_VERIFY10				0x2F
#define RBC_CMD_WRITE10					0x2A

/*
// SPC-2 commands
*/
#define SPC_CMD_INQUIRY					0x12
#define SPC_CMD_MODESELECT6				0x15
#define SPC_CMD_MODESENSE6				0x1A
#define SPC_CMD_PERSISTANTRESERVIN			0x5E
#define SPC_CMD_PERSISTANTRESERVOUT			0x5F
#define SPC_CMD_PRVENTALLOWMEDIUMREMOVAL		0x1E
#define SPC_CMD_RELEASE6				0x17
#define SPC_CMD_REQUESTSENSE				0x03
#define SPC_CMD_RESERVE6				0x16
#define SPC_CMD_TESTUNITREADY				0x00
#define SPC_CMD_WRITEBUFFER				0x3B
#define SPC_CMD_READLONG				0x23


/*
// ATAPI Command Descriptor Block
*/

typedef struct _READ_10 {
		INT8 OperationCode;
		INT8 Reserved1;
		INT8 LBA_3;
		INT8 LBA_2;
        INT8 LBA_1;
		INT8 LBA_0;
		INT8 Reserved2;
		INT8 XferLen_1;
		INT8 XferLen_0;
		INT8 Reserved3[3];
} READ_10, * PREAD_10;



typedef struct _WRITE_10 {
		INT8 OperationCode;
		INT8 Reserved1;
		INT8 LBA_3;
		INT8 LBA_2;
        INT8 LBA_1;
		INT8 LBA_0;
		INT8 Reserved2;
		INT8 XferLen_1;
		INT8 XferLen_0;
		INT8 Reserved3[3];
} WRITE_10, *PWRITE_10;

typedef struct _MODE_SENSE_10 {
		INT8 OperationCode;
		INT8 Reserved1;
		INT8 PageCode : 6;
		INT8 Pc : 2;
		INT8 Reserved2[4];
		INT8 ParameterListLengthMsb;
		INT8 ParameterListLengthLsb;
		INT8 Reserved3[3];
} MODE_SENSE_10, *PMODE_SENSE_10;

typedef struct _MODE_SELECT_10 {
		INT8 OperationCode;
		INT8 Reserved1 : 4;
		INT8 PFBit : 1;
		INT8 Reserved2 : 3;
		INT8 Reserved3[5];
		INT8 ParameterListLengthMsb;
		INT8 ParameterListLengthLsb;
		INT8 Reserved4[3];
} MODE_SELECT_10, *PMODE_SELECT_10;
/*
////////////////////////////////////////////////////////////////////////////////////
// Command Descriptor Block
//      _RBC : Reduced Block Command
//      _SPC : SPC-2 SCSI primary Command - 2
////////////////////////////////////////////////////////////////////////////////////
*/

/*
// Generic
*/
/*
// Generic
*/
typedef struct _GENERIC_CDB {
	INT8 OperationCode;
    INT8 Reserved[15];
} GENERIC_CDB,*PGENERIC_CDB;

typedef struct _GENERIC_RBC {
	INT8 OperationCode;
    INT8 Reserved[8];
	INT8 Control;
} GENERIC_RBC,*PGENERIC_RBC;

/*
// format unit
*/
typedef struct _FORMAT_RBC {
	INT8 OperationCode;	/* 04H */
	INT8 VendorSpecific;
	INT8 Increment : 1;
	INT8 PercentorTime : 1;
	INT8 Progress : 1;
    INT8 Immediate : 1;
	INT8 VendorSpecific1 : 4;
	INT8 Reserved2[2];
	INT8 Control;
} FORMAT_RBC, *PFORMAT_RBC;


/*
// Read Command
*/
typedef struct _READ_RBC {
	INT8 OperationCode;	/* 10H */
	INT8 VendorSpecific;
    	union{
        	struct
        	{
	        INT8 LBA_3;
	        INT8 LBA_2;
	        INT8 LBA_1;
	        INT8 LBA_0;
        	} LBA_W8 ;

       		INT32 LBA_W32;
    	      }LBA;
	INT8 Reserved;
    	//INT8 XferLength_1;
	//INT8 XferLength_0;
	INT16 XferLength;
    	INT8 Control;
    	//INT8 Reserved1[3];
} READ_RBC, *PREAD_RBC;


/*
// Read Capacity Data - returned in Big Endian format
*/
typedef struct _READ_CAPACITY_DATA {

    INT8 LBA_3;
    INT8 LBA_2;
    INT8 LBA_1;
    INT8 LBA_0;

    INT8 BlockLen_3;
    INT8 BlockLen_2;
    INT8 BlockLen_1;
    INT8 BlockLen_0;
} READ_CAPACITY_DATA, *PREAD_CAPACITY_DATA;
//////////////////////////////////////////////////
typedef struct _READ_LONG_CMD{
    INT8 OperationCode;
    INT8 LogicalUnitNum	:3;
    INT8 RES_1		:5;
    INT8 RES_2;
    INT8 RES_3;
    INT8 RES_4;
    INT8 RES_5;
    INT8 RES_6;
    INT16 AllocationLen;
    INT8 RES_7;
    INT8 RES_8;
    INT8 RES_9;
}READ_LONG_CMD, *PREAD_LONG_CMD;
typedef struct _READ_LONG {

    INT8 RES_1;
    INT8 RES_2;
    INT8 RES_3;
    INT8 CAP_LIST_LEN;
    
    INT8 LBA_3;
    INT8 LBA_2;
    INT8 LBA_1;
    INT8 LBA_0;

    INT8 Descripter;
    INT8 BlockLen_2;
    INT8 BlockLen_1;
    INT8 BlockLen_0;
} READ_LONG, *PREAD_LONG;
/*
// Read Capacity command
*/
typedef struct _READ_CAPACITY_RBC {
	INT8                OperationCode;	/* 10H */
   	union  {
		FLEXI_INT32		l0[2];
        	INT32               l[2];
        	READ_CAPACITY_DATA  CapData;       /* Reserved area, here is used as temp*/
    		} tmpVar;

	INT8                Control;
} READ_CAPACITY_RBC, *PREAD_CAPACITY_RBC;

typedef struct _READ_CAPACITY_RSP {
	INT32 LastLBA;
	INT32 BlockSize;

} READ_CAPACITY_RSP, *PREAD_CAPACITY_RSP;
/*
// START_STOP_UNIT
*/
typedef struct _START_STOP_RBC {
    INT8 OperationCode;    /*1BH*/
    INT8 Immediate: 1;
    INT8 Reserved1 : 7;
    INT8 Reserved2[2];
	union _START_STOP_FLAGS
    {
        struct
        {
            INT8 Start          : 1;
            INT8 LoadEject      : 1;
            INT8 Reserved3      : 2;
            INT8 PowerConditions: 4;
        } bits0;

        struct
        {
            INT8 MediumState    : 2;
            INT8 Reserved3      : 2;
            INT8 PowerConditions: 4;
        } bits1;
    } Flags;
    INT8 Control;
} START_STOP_RBC, *PSTART_STOP_RBC;

/*
// Synchronize Cache
*/
typedef struct _SYNCHRONIZE_CACHE_RBC {

	INT8 OperationCode;    /* 0x35 */
	INT8 Reserved[8];
	INT8 Control;

} SYNCHRONIZE_CACHE_RBC, *PSYNCHRONIZE_CACHE_RBC;

/*
// Write Command
*/
typedef struct _WRITE_RBC {
    INT8 OperationCode;	/* 2AH      */
    //INT8 Reserved0 : 3;
    //INT8 FUA : 1;
    //INT8 Reserved1 : 4;
    INT8 VendorSpecific;
    union{
	 struct
         {
	        INT8 LBA_3;
	        INT8 LBA_2;
	        INT8 LBA_1;
	        INT8 LBA_0;
          } LBA_W8 ;

	 INT32 LBA_W32;
	}   LBA;
    INT8 Reserved2;
    INT16 XferLength;
    INT8 Control;
} WRITE_RBC, *PWRITE_RBC;

/*
// VERIFY Command
*/
typedef struct _VERIFY_RBC {
    INT8 OperationCode;	/* 2FH */
    INT8 Reserved0;
	INT8 LBA_3;			/* Big Endian */
	INT8 LBA_2;
	INT8 LBA_1;
	INT8 LBA_0;
	INT8 Reserved1;
    INT8 VerifyLength_1;		/* Big Endian */
	INT8 VerifyLength_0;
	INT8 Control;
} VERIFY_RBC, *PVERIFY_RBC;


/*
//***********************************************************************************
// SPC-2 of SCSI-3 commands
//***********************************************************************************
*/

/*
// INQUIRY Command
*/
typedef struct _INQUIRY_SPC {
	INT8 OperationCode;	/* 12H */
	INT8 EnableVPD:1 ;
    	INT8 CmdSupportData:1 ;
	INT8 Reserved0:6 ;
	INT8 PageCode;
	INT8 Reserved1;
	INT8 AllocationLen;
    	INT8 Control;
} INQUIRY_SPC, *PINQUIRY_SPC;


typedef struct _STD_INQUIRYDATA {
    INT8 DeviceType : 5;
    INT8 Reserved0 : 3;

    INT8 Reserved1 : 7;
    INT8 RemovableMedia : 1;

    INT8 Reserved2;

    INT8 Reserved3 : 5;
    INT8 NormACA : 1;
    INT8 Obsolete0 : 1;
    INT8 AERC : 1;

    INT8 Reserved4[3];

    INT8 SoftReset : 1;
    INT8 CommandQueue : 1;
	INT8 Reserved5 : 1;
	INT8 LinkedCommands : 1;
	INT8 Synchronous : 1;
	INT8 Wide16Bit : 1;
	INT8 Wide32Bit : 1;
	INT8 RelativeAddressing : 1;

	INT8 VendorId[8];

	INT8 ProductId[16];

	INT8 ProductRevisionLevel[4];

/*
//  Above is 36 bytes
//  can be tranmitted by Bulk
*/

    INT8 VendorSpecific[20];
    INT8 InfoUnitSupport : 1;
    INT8 QuickArbitSupport : 1;
    INT8 Clocking : 2;
    INT8 Reserved6 : 4;

    INT8  Reserved7 ;
    INT16 VersionDescriptor[8] ;

    INT8 Reserved8[22];
} STD_INQUIRYDATA, *PSTD_INQUIRYDATA;

typedef struct _SERIALNUMBER_PAGE {
    INT8 DeviceType : 5;
    INT8 DeviceTypeQualifier : 3;

    INT8 PageCode ;
    INT8 Reserved0 ;

    INT8 PageLength ;
    INT8 SerialNumber[24] ;

}VPD_SERIAL_PAGE,* PVPD_SERIAL_PAGE;

#define ASCII_ID_STRING 32
typedef struct _ID_DESCRIPTOR {
	INT8   CodeSet : 4;
	INT8   Reserved0 : 4;

	INT8   IDType : 4;
    INT8   Association : 2;
    INT8   Reserved1 : 2;

    INT8   Reserved2;

	INT8   IDLength ;
	INT8   AsciiID[ASCII_ID_STRING];
} ASCII_ID_DESCRIPTOR,* PASCII_ID_DESCRIPTOR;

typedef struct _DEVICE_ID_PAGE
{
    INT8 DeviceType : 5;
    INT8 DeviceTypeQualifier : 3;

    INT8 PageCode ;
    INT8 Reserved0 ;

    INT8 PageLength ;

    ASCII_ID_DESCRIPTOR   AsciiIdDescriptor[1];
} VPD_DEVICE_ID_PAGE, * PVPD_DEVICE_ID_PAGE;



/*
// Mode Select
*/
typedef struct _MODE_SELECT_SPC {
	INT8 OperationCode;	/* 15H */
	INT8 SavePage : 1 ;
	INT8 Reseved0 : 3 ;
	INT8 PageFormat : 1 ;
	INT8 Reserved1 : 3 ;
	INT8 Reserved2[2];
	INT8 ParameterLen;
	INT8 Control;
} MODE_SELECT_SPC, * PMODE_SELECT_SPC;

typedef struct _MBR_BLOCK {
  INT8 Res[454];
  unsigned long StartSector;
  unsigned long TotalSector;
  INT8 Res1[50];
} MBR_BLOCK,* PMBR_BLOCK;

typedef struct _BPB_BLOCK {
  INT8 BS_jmpBoo[3];
  INT8 BS_OEMName[8];
  INT16 BPB_BytesPerSec;
  INT8 BPB_SecPerClus;
  INT8 BPB_RsvdSecCn[2];
  INT8 BPB_NumFATs;
  INT16 BPB_RootEntCnt;
  INT16 BPB_TotSec16;
  INT8 BPB_Media;
  INT16 BPB_FATSz16;
  INT16 BPB_SecPerTrk;
  INT16 BPB_NumHeads;
  unsigned long BPB_HiddSec;
  unsigned long BPB_TotSec32;
  INT8 BS_DrvNum;
  INT8 BS_Reserved1;
  INT8 BS_BootSig;
  INT8 BS_VolID[4];
  INT8 BS_VolLab[11];
  INT8 BS_FilSysType[8];
  INT8 ExecutableCode[448];
  INT8 Marker[2];
} BPB_BLOCK,* PBPB_BLOCK;

typedef struct _SYS_INFO_BLOCK{
  unsigned long StartSector;
  unsigned long TotalSector;
  //INT8 BS_jmpBoo[3];
  //INT8 BS_OEMName[8];
  INT16 BPB_BytesPerSec;
  INT8 BPB_SecPerClus;
  //INT8 BPB_RsvdSecCn[2];
  INT8 BPB_NumFATs;
  INT16 BPB_RootEntCnt;
  INT16 BPB_TotSec16;
  INT8 BPB_Media;
  INT16 BPB_FATSz16;
  INT16 BPB_SecPerTrk;
  INT16 BPB_NumHeads;
  unsigned long BPB_HiddSec;
  unsigned long BPB_TotSec32;
  INT8 BS_DrvNum;
  //INT8 BS_Reserved1;
  INT8 BS_BootSig;
  INT8 BS_VolID[4];
  INT8 BS_VolLab[11];
  INT8 BS_FilSysType[8];
  //INT8 ExecutableCode[448];
  //INT8 Marker[2];
  ///////////////////////////////
  unsigned long FatStartSector;
  unsigned long RootStartSector;
  //unsigned long DataStartSector;
  unsigned long FirstDataSector;
  //unsigned long FirstSectorofCluster;
} SYS_INFO_BLOCK,* PSYS_INFO_BLOCK;

typedef struct _FILE_INFO{
  unsigned char bFileOpen;
  unsigned int StartCluster;
  unsigned long LengthInByte;
  unsigned int ClusterPointer;
  unsigned long SectorPointer;
  unsigned int OffsetofSector;
  unsigned char SectorofCluster;
  unsigned long pointer;
  unsigned int	FatSectorPointer;
  

} FILE_INFO, * PFILE_INFO;

typedef struct _DIR_INFO{
	unsigned char name[8];
	unsigned char extension[3];
	unsigned char attribute;
	unsigned char Reserved[10];
	unsigned int lastUpdateDate;
	unsigned int lastUpdateTime;
	unsigned int startCluster;
	unsigned long length;
} DIR_INFO,* PDIR_INFO;
/*
// Mode Sense
*/
typedef struct _MODE_SENSE_SPC {
    INT8 OperationCode;	/* 1AH */
    INT8 Reseved0 : 3 ;
    INT8 DisableBlockDescriptor : 1 ;
    INT8 Reserved0 : 4 ;
    INT8 PageCode:6 ;
    INT8 PageControl : 2 ;
    INT8 Reserved1;
    INT8 ParameterLen;
    INT8 Control;
} MODE_SENSE_SPC, * PMODE_SENSE_SPC;

typedef struct _MODE_PARAMETER_HEAD {
    INT8 DataLen;
    INT8 MediumType;
    INT8 DeviceParameter;
    INT8 BlockDescriptorLen;
} MODE_PARAMETER_HEAD, * PMODE_PARAMETER_HEAD;

/*
// Define Device Capabilities page.
*/
typedef struct _MODE_RBC_DEVICE_PARAMETERS_PAGE {
    INT8 PageCode : 6;
	INT8 Reserved : 1;
    INT8 PageSavable : 1;
    INT8 PageLength;
    INT8 WriteCacheDisable : 1;
    INT8 Reserved1 : 7;
    INT8 LogicalBlockSize[2];
    INT8 NumberOfLogicalBlocks[5];
    INT8 PowerPerformance;
    INT8 Lockable : 1;
    INT8 Formattable : 1;
    INT8 Writable : 1;
    INT8 Readable : 1;
    INT8 Reserved2 : 4;
    INT8 Reserved3;
}MODE_RBC_DEVICE_PARAMETERS_PAGE, *PMODE_RBC_DEVICE_PARAMETERS_PAGE;

/*
// prevent/allow medium removal
*/
typedef struct _MEDIA_REMOVAL_SPC {
	INT8 OperationCode;    /* 1EH */
	INT8 Reserved0[3];
	INT8 Prevent;
	//INT8 Reserved1:6 ;
	//INT8 Control;
} MEDIA_REMOVAL_SPC, *PMEDIA_REMOVAL_SPC;

/*
// Request Sense
*/
typedef struct _REQUEST_SENSE_SPC {
    INT8 OperationCode;    /* 03H */
    INT8 Reserved[3];
    INT8 AllocationLen;
    INT8 Control;
} REQUEST_SENSE_SPC, *PREQUEST_SENSE_SPC;

typedef struct _REQUEST_SENSE_DATA {
    INT8 ResponseCode : 7;
    INT8 Valid : 1;

    INT8 SegmentNum;

    INT8 SenseKey : 4;
    INT8 Reserved0 : 1;
    INT8 WrongLenIndicator : 1;
    INT8 EndofMedium : 1;
    INT8 FileMark : 1;

    INT8 Info_0;
    INT8 Info_1;
    INT8 Info_2;
    INT8 Info_3;

    INT8 AdditionalSenseLen;

    INT8 CommandSpecInfo_0;
    INT8 CommandSpecInfo_1;
    INT8 CommandSpecInfo_2;
    INT8 CommandSpecInfo_3;

    INT8 ASC;
    INT8 ASCQ;
    INT8 FieldReplacableUnitCode;
    INT8 SenseKeySpec_0 : 7;
    INT8 SenseKeySpecValid : 1;
    INT8 SenseKeySpec_1;
    INT8 SenseKeySpec_2;

} REQUEST_SENSE_DATA, *PREQUEST_SENSE_DATA;

/*
// Test Unit Ready
*/
typedef struct _TEST_UNIT_SPC {
	INT8 OperationCode;    /* 00H */
	INT8 Reserved[4];
	INT8 Control;
} TEST_UNIT_SPC, *PTEST_UNIT_SPC;

/*
// Write Buffer
*/
typedef struct _WRITE_BUFFER_SPC {
    INT8 OperationCode;    /* 3BH */
    INT8 Mode:4 ;
    INT8 Reserved0:4 ;
	INT8 BufferID;
    INT8 BufferOff_2;
    INT8 BufferOff_1;
    INT8 BufferOff_0;
    INT8 ParameterLen_2;
    INT8 ParameterLen_1;
	INT8 ParameterLen_0;
    INT8 Control;
} WRITE_BUFFER_SPC, *PWRITE_BUFFER_SPC;

typedef union _CDB_RBC {
    GENERIC_CDB             Cdb_Generic;
  
     // RBC commands
    GENERIC_RBC             RbcCdb_Generic;

    FORMAT_RBC              RbcCdb_Format;
    READ_RBC                RbcCdb_Read;
    READ_CAPACITY_RBC       RbcCdb_ReadCapacity;
    START_STOP_RBC          RbcCdb_OnOffUnit;
    SYNCHRONIZE_CACHE_RBC   RbcCdb_SyncCache;
    VERIFY_RBC              RbcCdb_Verify;
    WRITE_RBC               RbcCdb_Write;

   
    // SPC-2 commands
   
    INQUIRY_SPC             SpcCdb_Inquiry;
    MODE_SELECT_SPC         SpcCdb_ModeSelect;
    MODE_SENSE_SPC          SpcCdb_ModeSense;
    MEDIA_REMOVAL_SPC       SpcCdb_Remove;
    REQUEST_SENSE_SPC       SpcCdb_RequestSense;
    TEST_UNIT_SPC           SpcCdb_TestUnit;
    WRITE_BUFFER_SPC        SpcCdb_WriteBuffer;

    // ATAPI Commands
    READ_10         CmdRead10;
    WRITE_10        CmdWrite10;
    MODE_SELECT_10  CmdModeSel10;
    MODE_SENSE_10   CmdModeSen10;
    //////////////////////////////////////
    READ_LONG_CMD	SpcCdb_ReadLong;

} CDB_RBC, *PCDB_RBC;

