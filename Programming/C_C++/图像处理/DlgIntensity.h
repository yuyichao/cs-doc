#if !defined(AFX_DLGINTENSITY_H__504BB8CE_7CF6_4D13_B5B7_DCC1BC84FEA5__INCLUDED_)
#define AFX_DLGINTENSITY_H__504BB8CE_7CF6_4D13_B5B7_DCC1BC84FEA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIntensity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity dialog

class CDlgIntensity : public CDialog
{
// Construction
public:
	
	// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动下限，2表示正在拖动上限。
	int		m_iIsDraging;
	
	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;
	
	// DIB的高度
	LONG	m_lHeight;
	
	// DIB的宽度
	LONG	m_lWidth;
	
	// 指向当前DIB象素的指针
	char *	m_lpDIBBits;
	
	// 各个灰度值的计数
	LONG	m_lCount[256];
	
	CDlgIntensity(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIntensity)
	enum { IDD = IDD_DLG_Intensity };
	
	// 显示灰度区间的下限
	int		m_iLowGray;
	
	// 显示灰度区间的上限
	int		m_iUpGray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIntensity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIntensity)
	afx_msg void OnPaint();
	afx_msg void OnKillfocusEDITLowGray();
	afx_msg void OnKillfocusEDITUpGray();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINTENSITY_H__504BB8CE_7CF6_4D13_B5B7_DCC1BC84FEA5__INCLUDED_)
