// ad9854Dlg.h : header file
//

#if !defined(AFX_AD9854DLG_H__F2184ECF_5F05_495F_9EFC_05DAAB886E35__INCLUDED_)
#define AFX_AD9854DLG_H__F2184ECF_5F05_495F_9EFC_05DAAB886E35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAd9854Dlg dialog

class CAd9854Dlg : public CDialog
{
// Construction
public:
	CAd9854Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAd9854Dlg)
	enum { IDD = IDD_AD9854_DIALOG };
	DWORD	m_phase1;
	DWORD	m_phase2;
	DWORD	m_freq1;
	DWORD	m_deltfreq;
	DWORD	m_upclock;
	DWORD	m_vfreq;
	int		m_amp;
	int		m_gatetime;
	long	m_freq2;
	//}}AFX_DATA
    void singletone(void);
	void unrampedfsk(void);
	void rampedfsk(void);
	void chirp(void);
	void bpsk(void);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAd9854Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAd9854Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Ontest();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AD9854DLG_H__F2184ECF_5F05_495F_9EFC_05DAAB886E35__INCLUDED_)
