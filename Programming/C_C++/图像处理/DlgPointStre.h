#if !defined(AFX_DLGPOINTSTRE_H__45B95585_372F_4C49_8928_99D343D2DE00__INCLUDED_)
#define AFX_DLGPOINTSTRE_H__45B95585_372F_4C49_8928_99D343D2DE00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPointStre.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPointStre dialog

class CDlgPointStre : public CDialog
{
// Construction
public:
	
	// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动第一点，2表示正在拖动第二点。
	int		m_iIsDraging;
	
	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	// 标识是否已经绘制橡皮筋线
	BOOL	m_bDrawed;
	
	// 保存鼠标左键单击时的位置
	CPoint	m_p1;
	
	// 保存鼠标拖动时的位置
	CPoint	m_p2;
	
	CDlgPointStre(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPointStre)
	enum { IDD = IDD_DLG_PointStre };
	
	// 两个转折点坐标
	BYTE	m_bX1;
	BYTE	m_bY1;
	BYTE	m_bX2;
	BYTE	m_bY2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPointStre)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPointStre)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnKillfocusEditX1();
	afx_msg void OnKillfocusEditX2();
	afx_msg void OnKillfocusEditY1();
	afx_msg void OnKillfocusEditY2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOINTSTRE_H__45B95585_372F_4C49_8928_99D343D2DE00__INCLUDED_)
