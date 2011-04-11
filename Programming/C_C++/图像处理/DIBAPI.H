// dibapi.h

#ifndef _INC_DIBAPI
#define _INC_DIBAPI

// DIB句柄
DECLARE_HANDLE(HDIB);

// DIB常量
#define PALVERSION   0x300

/* DIB宏 */

// 判断是否是Win 3.0的DIB
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

// 计算矩形区域的宽度
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)

// 计算矩形区域的高度
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// 在计算图像大小时，采用公式：biSizeImage = biWidth' × biHeight。
// 是biWidth'，而不是biWidth，这里的biWidth'必须是4的整倍数，表示
// 大于或等于biWidth的，离4最近的整倍数。WIDTHBYTES就是用来计算
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

// PCX文件头结构
typedef struct{
		 BYTE bManufacturer;
		 BYTE bVersion;
		 BYTE bEncoding;
		 BYTE bBpp;
		 WORD wLeft;
		 WORD wTop;
		 WORD wRight;
		 WORD wBottom;
		 WORD wXResolution;
		 WORD wYResolution;
		 BYTE bPalette[48];
		 BYTE bReserved;
		 BYTE bPlanes;
		 WORD wLineBytes;
		 WORD wPaletteType;
		 WORD wSrcWidth;
		 WORD wSrcDepth;
		 BYTE bFiller[54];
} PCXHEADER;

// 函数原型
BOOL      WINAPI  PaintDIB (HDC, LPRECT, HDIB, LPRECT, CPalette* pPal);
BOOL      WINAPI  CreateDIBPalette(HDIB hDIB, CPalette* cPal);
LPSTR     WINAPI  FindDIBBits (LPSTR lpbi);
DWORD     WINAPI  DIBWidth (LPSTR lpDIB);
DWORD     WINAPI  DIBHeight (LPSTR lpDIB);
WORD      WINAPI  PaletteSize (LPSTR lpbi);
WORD      WINAPI  DIBNumColors (LPSTR lpbi);
WORD	  WINAPI  DIBBitCount(LPSTR lpbi);
HGLOBAL   WINAPI  CopyHandle (HGLOBAL h);

BOOL      WINAPI  SaveDIB (HDIB hDib, CFile& file);
HDIB      WINAPI  ReadDIBFile(CFile& file);

BOOL      WINAPI  DIBToPCX256(LPSTR lpDIB, CFile& file);
HDIB      WINAPI  ReadPCX256(CFile& file);

#endif //!_INC_DIBAPI
