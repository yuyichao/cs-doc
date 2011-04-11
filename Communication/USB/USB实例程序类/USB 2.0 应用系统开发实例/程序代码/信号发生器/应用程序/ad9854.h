// ad9854.h : main header file for the AD9854 application
//

#if !defined(AFX_AD9854_H__0FEE67FA_58A5_4B99_A910_1751ED2F877D__INCLUDED_)
#define AFX_AD9854_H__0FEE67FA_58A5_4B99_A910_1751ED2F877D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAd9854App:
// See ad9854.cpp for the implementation of this class
//

class CAd9854App : public CWinApp
{
public:
	CAd9854App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAd9854App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAd9854App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AD9854_H__0FEE67FA_58A5_4B99_A910_1751ED2F877D__INCLUDED_)
