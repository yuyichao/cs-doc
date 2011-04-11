///////////////////////////////////
//#include "common.h"
#include "RBCCMD.H"
///////////////////////////////////
#define CBW_SIGNATURE   0x55534243
#define CSW_SIGNATURE   0x55534253

typedef struct _COMMAND_BLOCK_WRAPPER{
    INT32   dCBW_Signature;
    INT32   dCBW_Tag;
    INT32   dCBW_DataXferLen;
    INT8    bCBW_Flag;
    INT8    bCBW_LUN;
    INT8    bCBW_CDBLen;
    CDB_RBC cdbRBC;
} CBW, *PCBW;

typedef struct _COMMAND_STATUS_WRAPPER{
    INT32   dCSW_Signature;
    INT32   dCSW_Tag;
    INT32   dCSW_DataResidue;
    INT8    bCSW_Status;
} CSW, *PCSW;

typedef union _TPBULK_STRUC {
    CBW     TPBulk_CommandBlock;
    CSW     TPBulk_CommandStatus;

}TPBLK_STRUC, * PTPBLK_STRUC;
///////////////////////////////////
unsigned char EnumMassDev(void);
unsigned char TPBulk_GetMaxLUN(void);
//unsigned char TPBulk_CBWHandler(unsigned char OperationCode);

unsigned char SPC_Inquiry(void);
unsigned char SPC_READLONG(void);
unsigned char SPC_RequestSense(void);
unsigned char SPC_TestUnit(void);
unsigned char SPC_LockMedia(void);
unsigned char RBC_ReadCapacity(void);
unsigned char RBC_Read(unsigned long lba,unsigned char len,unsigned char *pBuffer);
unsigned char RBC_Write(unsigned long lba,unsigned char len,unsigned char *pBuffer);


