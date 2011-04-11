// restore.h


#ifndef _INC_RestoreAPI
#define _INC_RestoreAPI

// º¯ÊýÔ­ÐÍ
BOOL fourn(double * data, unsigned long nn[], int ndim, int isign);
BOOL WINAPI BlurDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI RestoreDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI NoiseBlurDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI WienerDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI RandomNoiseDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI SaltNoiseDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);

#endif //!_INC_RestoreAPI

