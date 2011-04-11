// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AC62C54D_058E_4CB9_BE9B_BE1173970836__INCLUDED_)
#define AFX_STDAFX_H__AC62C54D_058E_4CB9_BE9B_BE1173970836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

struct EMSTable
{
	long start, end, size;
	bool isFree;
	struct EMSTable * next, * before;
};

struct WorkList
{
	int num;
	long size;
	struct EMSTable * position;
	struct WorkList * next, * before;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AC62C54D_058E_4CB9_BE9B_BE1173970836__INCLUDED_)
