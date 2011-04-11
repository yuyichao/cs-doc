#if !defined(AFX_DLGCOLOR_H__BC36E028_E94D_4AD1_AF17_E8F8E960D8F4__INCLUDED_)
#define AFX_DLGCOLOR_H__BC36E028_E94D_4AD1_AF17_E8F8E960D8F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgColor dialog

class CDlgColor : public CDialog
{
// Construction
public:
	
	// 颜色名称字符串长度
	int m_nNameLen;
	
	// 颜色名称字符串数组指针
	LPSTR m_lpColorName;
	
	// 颜色数目
	int m_nColorCount;
	
	// 当前选择的伪彩色编码表
	int m_nColor;
	
	CDlgColor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgColor)
	enum { IDD = IDD_DLG_Color };
	CListBox	m_lstColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgColor)
	afx_msg void OnDblclkColorList();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOLOR_H__BC36E028_E94D_4AD1_AF17_E8F8E960D8F4__INCLUDED_)
