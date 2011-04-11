// EMSDSD.h : main header file for the EMSDSD application
//

#if !defined(AFX_EMSDSD_H__25BDB6CA_C6B4_4507_AF55_8D1B45ED94B7__INCLUDED_)
#define AFX_EMSDSD_H__25BDB6CA_C6B4_4507_AF55_8D1B45ED94B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEMSDSDApp:
// See EMSDSD.cpp for the implementation of this class
//

class CEMSDSDApp : public CWinApp
{
public:
	CEMSDSDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEMSDSDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEMSDSDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMSDSD_H__25BDB6CA_C6B4_4507_AF55_8D1B45ED94B7__INCLUDED_)
