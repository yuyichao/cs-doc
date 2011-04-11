// TemplateTrans.h


#ifndef _INC_TemplateTransAPI
#define _INC_TemplateTransAPI

// º¯ÊýÔ­ÐÍ
BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
					 int iTempH, int iTempW, 
					 int iTempMX, int iTempMY,
					 FLOAT * fpArray, FLOAT fCoef);
BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int iFilterH, int iFilterW, 
						 int iFilterMX, int iFilterMY);
unsigned char WINAPI GetMedianNum(unsigned char * bArray, int iFilterLen);
BOOL WINAPI GradSharp(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre);
BOOL WINAPI ReplaceColorPal(LPSTR lpDIB, BYTE * bpColorsTable);

#endif //!_INC_TemplateTransAPI