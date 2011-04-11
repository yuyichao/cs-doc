#if !defined(AFX_DLGPOINTTHRE_H__4CF9C804_C248_4119_B6AD_905FB6AF4D89__INCLUDED_)
#define AFX_DLGPOINTTHRE_H__4CF9C804_C248_4119_B6AD_905FB6AF4D89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPointThre.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPointThre dialog

class CDlgPointThre : public CDialog
{
// Construction
public:
	
	// 当前鼠标拖动状态，TRUE表示正在拖动
	BOOL	m_bIsDraging;
	
	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	CDlgPointThre(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgPointThre)
	enum { IDD = IDD_DLG_PointThre };
	
	// 阈值
	BYTE	m_bThre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgPointThre)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgPointThre)
	afx_msg void OnKillfocusEDITThre();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOINTTHRE_H__4CF9C804_C248_4119_B6AD_905FB6AF4D89__INCLUDED_)
