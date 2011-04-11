// FreTrans.h


#ifndef _INC_FreTransAPI
#define _INC_FreTransAPI

#include <complex>
using namespace std;

// º¯ÊýÔ­ÐÍ
VOID WINAPI FFT(complex<double> * TD, complex<double> * FD, int r);
VOID WINAPI IFFT(complex<double> * FD, complex<double> * TD, int r);
VOID WINAPI DCT(double *f, double *F, int power);
VOID WINAPI IDCT(double *F, double *f, int power);
VOID WINAPI WALSH(double *f, double *F, int r);
VOID WINAPI IWALSH(double *F, double *f, int r);

BOOL WINAPI Fourier(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI DIBDct(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI DIBWalsh(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI DIBWalsh1(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);


#endif //!_INC_FreTransAPI