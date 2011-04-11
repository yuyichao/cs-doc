#if !defined(AFX_DLGSHANNON_H__456A32D8_D7EE_4F4B_945B_672AE8258607__INCLUDED_)
#define AFX_DLGSHANNON_H__456A32D8_D7EE_4F4B_945B_672AE8258607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShannon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog

class CDlgShannon : public CDialog
{
// Construction
public:
	
	// 灰度级别数目
	int m_iColorNum;
	
	// 各个灰度值出现频率
	FLOAT *	m_fFreq;
	
	// 香农－弗诺编码表
	CString	* m_strCode;
	
	CDlgShannon(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgShannon)
	enum { IDD = IDD_DLG_Shannon };
	CListCtrl	m_lstTable;
	double	m_dEntropy;
	double	m_dAvgCodeLen;
	double	m_dEfficiency;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShannon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShannon)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHANNON_H__456A32D8_D7EE_4F4B_945B_672AE8258607__INCLUDED_)
