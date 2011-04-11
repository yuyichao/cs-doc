typedef struct gifheader
   {
      BYTE bySignature[3];
      BYTE byVersion[3];
   }  GIFHEADER;

typedef struct gifscrdesc
   {
      WORD wWidth;
      WORD wDepth;
      struct globalflag
         {
            BYTE PalBits   : 3;
            BYTE SortFlag  : 1;
            BYTE ColorRes  : 3;
            BYTE GlobalPal : 1;
         }  GlobalFlag;
      BYTE byBackground;
      BYTE byAspect;
   }  GIFSCRDESC;

typedef struct gifimage
   {
      WORD wLeft;
      WORD wTop;
      WORD wWidth;
      WORD wDepth;
      struct localflag
         {
            BYTE PalBits   : 3;
            BYTE Reserved  : 2;
            BYTE SortFlag  : 1;
            BYTE Interlace : 1;
            BYTE LocalPal  : 1;
         }  LocalFlag;
   }  GIFIMAGE;

typedef struct gifcontrol
   {
      BYTE byBlockSize;
      struct flag
         {
            BYTE Transparency   : 1;
            BYTE UserInput      : 1;
            BYTE DisposalMethod : 3;
            BYTE Reserved       : 3;
         }  Flag;
      WORD wDelayTime;
      BYTE byTransparencyIndex;
      BYTE byTerminator;
   }  GIFCONTROL;

typedef struct gifplaintext
   {
      BYTE byBlockSize;
      WORD wTextGridLeft;
      WORD wTextGridTop;
      WORD wTextGridWidth;
      WORD wTextGridDepth;
      BYTE byCharCellWidth;
      BYTE byCharCellDepth;
      BYTE byForeColorIndex;
      BYTE byBackColorIndex;
   }  GIFPLAINTEXT;

typedef struct gifapplication
   {
      BYTE byBlockSize;
      BYTE byIdentifier[8];
      BYTE byAuthentication[3];
   }  GIFAPPLICATION;

typedef struct gifd_var
   {
      LPSTR lpDataBuff;
      LPSTR lpBgnBuff;
      LPSTR lpEndBuff;
      DWORD dwDataLen;
      WORD  wMemLen;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wBits;
      BOOL  bEOF;
      BOOL  bInterlace;
   }  GIFD_VAR;
typedef GIFD_VAR FAR *LPGIFD_VAR;

typedef struct gifc_var
   {
      LPSTR lpDataBuff;
      LPSTR lpEndBuff;
      DWORD dwTempCode;
      WORD  wWidth;
      WORD  wDepth;
      WORD  wLineBytes;
      WORD  wBits;
      WORD  wByteCnt;
      WORD  wBlockNdx;
      BYTE  byLeftBits;
   }  GIFC_VAR;
typedef GIFC_VAR FAR *LPGIFC_VAR;


// 宏运算
#define DWORD_WBYTES(x)         ( (((x) + 31UL) >> 5) << 2 )
#define WORD_WBYTES(x)          ( (((x) + 15UL) >> 4) << 1 )
#define BYTE_WBYTES(x)          (  ((x) +  7UL) >> 3       )

//常量
#define MAX_BUFF_SIZE           32768 /* 32K */
#define MAX_HASH_SIZE            5051
#define MAX_TABLE_SIZE           4096 /* 12-bit */
#define MAX_SUBBLOCK_SIZE         255

// 函数原型
BOOL WINAPI DIBToGIF(LPSTR lpDIB, CFile& file, BOOL bInterlace);
void WINAPI EncodeGIF_LZW(LPSTR lpDIBBits, CFile& file, 
						  LPGIFC_VAR lpGIFCVar,WORD wWidthBytes, BOOL bInterlace);
void WINAPI GIF_LZW_WriteCode(CFile& file, WORD wCode, LPSTR lpSubBlock,
							  LPBYTE lpbyCurrentBits,LPGIFC_VAR lpGIFCVar);
HDIB WINAPI ReadGIF(CFile& file);

void WINAPI ReadSrcData(CFile& file, LPWORD lpwMemLen, LPDWORD lpdwDataLen,
                        LPSTR lpSrcBuff, LPBOOL lpbEOF);
void WINAPI DecodeGIF_LZW(CFile& file, LPSTR lpDIBBits,
						  LPGIFD_VAR lpGIFDVar,WORD wWidthBytes);

