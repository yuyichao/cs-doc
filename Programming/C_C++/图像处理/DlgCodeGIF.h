#if !defined(AFX_DLGCODEGIF_H__EB85EF8B_B6B0_4533_A858_DDFF632DFBA7__INCLUDED_)
#define AFX_DLGCODEGIF_H__EB85EF8B_B6B0_4533_A858_DDFF632DFBA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCodeGIF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCodeGIF dialog

class CDlgCodeGIF : public CDialog
{
// Construction
public:
	CDlgCodeGIF(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCodeGIF)
	enum { IDD = IDD_DLG_CodeGIF };
	CString	m_strFilePath;
	BOOL	m_bInterlace;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCodeGIF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCodeGIF)
	afx_msg void OnbtnSaveAs();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCODEGIF_H__EB85EF8B_B6B0_4533_A858_DDFF632DFBA7__INCLUDED_)
