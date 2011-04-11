// thread2.h : main header file for the THREAD2 application
//

#if !defined(AFX_THREAD2_H__ED9CBAAA_089D_4553_9074_C65C182A8C54__INCLUDED_)
#define AFX_THREAD2_H__ED9CBAAA_089D_4553_9074_C65C182A8C54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CThread2App:
// See thread2.cpp for the implementation of this class
//

class CThread2App : public CWinApp
{
public:
	CThread2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThread2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CThread2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREAD2_H__ED9CBAAA_089D_4553_9074_C65C182A8C54__INCLUDED_)
