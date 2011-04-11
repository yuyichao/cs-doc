// MyEMS.h: interface for the MyEMS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYEMS_H__9276B02F_74FA_46A8_AF8C_D4EEEB27D25A__INCLUDED_)
#define AFX_MYEMS_H__9276B02F_74FA_46A8_AF8C_D4EEEB27D25A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MyEMS  
{
public:
	bool Distribute(int arithmetic, long spaceneed);
	void Init(long totleEMS);
	int num;
	MyEMS();
	virtual ~MyEMS();
	struct EMSTable * EMST, * BestEMST;
	struct WorkList * worklist, *wlend;
};

#endif // !defined(AFX_MYEMS_H__9276B02F_74FA_46A8_AF8C_D4EEEB27D25A__INCLUDED_)
