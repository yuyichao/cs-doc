// morph.h


#ifndef _INC_MorphAPI
#define _INC_MorphAPI


// º¯ÊýÔ­ÐÍ
BOOL WINAPI ErosionDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori , int structure[3][3]);
BOOL WINAPI DilationDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori , int structure[3][3]);
BOOL WINAPI OpenDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori , int structure[3][3]);
BOOL WINAPI CloseDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori , int structure[3][3]);
BOOL WINAPI ThiningDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);

#endif //!_INC_MorphAPI