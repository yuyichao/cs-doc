// edgecontour.h

#define pi 3.1415927
#ifndef _INC_EdgeContourAPI
#define _INC_EdgeContourAPI
// º¯ÊýÔ­ÐÍ

BOOL WINAPI RobertDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI SobelDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI PrewittDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI KirschDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI GaussDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI HoughDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI FillDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI Fill2DIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI ContourDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI TraceDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);

#endif //!_INC_EdgeContourAPI

typedef struct{
	int Value;
	int Dist;
	int AngleNumber;
}	MaxValue;

typedef struct{
	int Height;
	int Width;
}	Seed;

typedef struct{
	int Height;
	int Width;
}	Point;