#if !defined(AFX_DLGPOINTWIN_H__E76A76B4_7F31_421A_9EB3_2AD952C0F009__INCLUDED_)
#define AFX_DLGPOINTWIN_H__E76A76B4_7F31_421A_9EB3_2AD952C0F009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPointWin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPointWin dialog

class CDlgPointWin : public CDialog
{
// Construction
public:
	
	// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动下限，2表示正在拖动上限。
	int		m_iIsDraging;
	
	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	CDlgPointWin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPointWin)
	enum { IDD = IDD_DLG_PointWin };
	
	// 窗口的下限
	BYTE	m_bLow;
	
	// 窗口的上限
	BYTE	m_bUp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPointWin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPointWin)
	afx_msg void OnKillfocusEDITLow();
	afx_msg void OnKillfocusEDITUp();
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOINTWIN_H__E76A76B4_7F31_421A_9EB3_2AD952C0F009__INCLUDED_)
