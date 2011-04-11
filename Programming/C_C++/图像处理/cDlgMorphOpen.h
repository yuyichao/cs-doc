#if !defined(AFX_CDLGMORPHOPEN_H__00270D81_D74D_11D4_9AF2_0000B4C23285__INCLUDED_)
#define AFX_CDLGMORPHOPEN_H__00270D81_D74D_11D4_9AF2_0000B4C23285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDlgMorphOpen.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cDlgMorphOpen dialog

class cDlgMorphOpen : public CDialog
{
// Construction
public:
	cDlgMorphOpen(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cDlgMorphOpen)
	enum { IDD = IDD_DLG_MORPHOpen };
	CButton	m_Control9;
	CButton	m_Control8;
	CButton	m_Control7;
	CButton	m_Control6;
	CButton	m_Control5;
	CButton	m_Control4;
	CButton	m_Control3;
	CButton	m_Control2;
	CButton	m_Control1;
	int		m_nMode;
	int		m_nStructure1;
	int		m_nStructure2;
	int		m_nStructure3;
	int		m_nStructure4;
	int		m_nStructure5;
	int		m_nStructure6;
	int		m_nStructure7;
	int		m_nStructure8;
	int		m_nStructure9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDlgMorphOpen)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cDlgMorphOpen)
	afx_msg void OnHori();
	afx_msg void OnVert();
	afx_msg void Oncustom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGMORPHOPEN_H__00270D81_D74D_11D4_9AF2_0000B4C23285__INCLUDED_)
