// detect.h


#ifndef _INC_DetectAPI
#define _INC_DetectAPI


// º¯ÊýÔ­ÐÍ
BOOL WINAPI ThresholdDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI AddMinusDIB (LPSTR lpDIBBits, LPSTR lpDIBBitsBK, LONG lWidth, LONG lHeight, bool bAddMinus);
BOOL WINAPI HprojectDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI VprojectDIB (LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI TemplateMatchDIB (LPSTR lpDIBBits, LPSTR lpTemplateDIBBits, 
							  LONG lWidth, LONG lHeight,LONG lTemplateWidth,LONG lTemplateHeight);

#endif //!_INC_DetectAPI