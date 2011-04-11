// thread2Dlg.h : header file
//

#if !defined(AFX_THREAD2DLG_H__2D7A14E6_505A_4F6E_9C51_E5CAAE510121__INCLUDED_)
#define AFX_THREAD2DLG_H__2D7A14E6_505A_4F6E_9C51_E5CAAE510121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
UINT ThreadPoc1(LPVOID pParam);
UINT ThreadPoc2(LPVOID pParam);
UINT caijipoc(LPVOID pParam);
/////////////////////////////////////////////////////////////////////////////
// CThread2Dlg dialog

class CThread2Dlg : public CDialog
{
// Construction
public:
	CThread2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CThread2Dlg)
	enum { IDD = IDD_THREAD2_DIALOG };
	int		m_data1;
	int		m_data2;
	double	m_data3;
	int		m_data4;
	int		m_gap;
	//}}AFX_DATA

    void DrawWave(CDC *pDC);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThread2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
private:
		CDC *pDC;             //屏幕绘图设备
		CDC memDC;            //内存绘图设备
		int m_High;           //绘图起点
		int m_Low;            //绘图终点
     	double m_lCount[1024];   //数据存储数组
		int m_now;            //记录波形当前点
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CThread2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnCAIJI();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREAD2DLG_H__2D7A14E6_505A_4F6E_9C51_E5CAAE510121__INCLUDED_)
