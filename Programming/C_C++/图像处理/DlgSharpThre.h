#if !defined(AFX_DLGSHARPTHRE_H__17339264_0AB6_4CB6_8FEB_397BD5004632__INCLUDED_)
#define AFX_DLGSHARPTHRE_H__17339264_0AB6_4CB6_8FEB_397BD5004632__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSharpThre.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSharpThre dialog

class CDlgSharpThre : public CDialog
{
// Construction
public:
	CDlgSharpThre(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSharpThre)
	enum { IDD = IDD_DLG_THRE };
	BYTE	m_bThre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSharpThre)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSharpThre)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHARPTHRE_H__17339264_0AB6_4CB6_8FEB_397BD5004632__INCLUDED_)
