#if !defined(AFX_DLGLINERPARA_H__CA65EBE3_E61B_4E4B_9C27_716F1FD13500__INCLUDED_)
#define AFX_DLGLINERPARA_H__CA65EBE3_E61B_4E4B_9C27_716F1FD13500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLinerPara.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLinerPara dialog

class CDlgLinerPara : public CDialog
{
// Construction
public:
	
	// 标识是否已经绘制橡皮筋线
	BOOL	m_bDrawed;
	
	// 保存鼠标左键单击时的位置
	CPoint	m_p1;
	
	// 保存鼠标拖动时的位置
	CPoint	m_p2;
	
	// 当前鼠标拖动状态，TRUE表示正在拖动。
	BOOL	m_bIsDraging;
	
	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	CDlgLinerPara(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLinerPara)
	enum { IDD = IDD_DLG_LinerPara };
	
	// 线性变换的斜率
	float	m_fA;
	
	// 线性变换的截距
	float	m_fB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLinerPara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLinerPara)
	afx_msg void OnPaint();
	afx_msg void OnKillfocusEditA();
	afx_msg void OnKillfocusEditB();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLINERPARA_H__CA65EBE3_E61B_4E4B_9C27_716F1FD13500__INCLUDED_)
