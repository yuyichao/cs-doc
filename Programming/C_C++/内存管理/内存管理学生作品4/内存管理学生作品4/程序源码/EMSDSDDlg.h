// EMSDSDDlg.h : header file
//

#if !defined(AFX_EMSDSDDLG_H__305B2C50_4773_4EB2_BA2A_4BE17EE130AE__INCLUDED_)
#define AFX_EMSDSDDLG_H__305B2C50_4773_4EB2_BA2A_4BE17EE130AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyEMS.h"

/////////////////////////////////////////////////////////////////////////////
// CEMSDSDDlg dialog

class CEMSDSDDlg : public CDialog
{
// Construction
public:
	int m_iArithmetic;
	MyEMS* m_myEMS;
	CEMSDSDDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEMSDSDDlg)
	enum { IDD = IDD_EMSDSD_DIALOG };
	CComboBox	m_ccReleaseWork;
	long	m_lTotleEMSSpace;
	long	m_lApplyEMSSpace;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEMSDSDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEMSDSDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnReinit();
	afx_msg void OnApply();
	afx_msg void OnRelease();
	afx_msg void OnFirstarithmetic();
	afx_msg void OnBestarithmetic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMSDSDDLG_H__305B2C50_4773_4EB2_BA2A_4BE17EE130AE__INCLUDED_)
