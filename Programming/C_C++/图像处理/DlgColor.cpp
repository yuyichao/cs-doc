// DlgColor.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgColor dialog


CDlgColor::CDlgColor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgColor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgColor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgColor)
	DDX_Control(pDX, IDC_COLOR_LIST, m_lstColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgColor, CDialog)
	//{{AFX_MSG_MAP(CDlgColor)
	ON_LBN_DBLCLK(IDC_COLOR_LIST, OnDblclkColorList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgColor message handlers

BOOL CDlgColor::OnInitDialog() 
{
	// 循环变量
	int		i;
	
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();
	
	// 添加伪彩色编码
	for (i = 0; i < m_nColorCount; i++)
	{
		m_lstColor.AddString(m_lpColorName + i * m_nNameLen);
	}

	// 选中初始编码表
	m_lstColor.SetCurSel(m_nColor);
	
	// 返回TRUE
	return TRUE;
}

void CDlgColor::OnDblclkColorList() 
{
	// 双击事件，直接调用OnOK()成员函数
	OnOK();
	
}

void CDlgColor::OnOK() 
{
	// 用户单击确定按钮
	m_nColor = m_lstColor.GetCurSel();
	
	// 调用默认的OnOK()函数
	CDialog::OnOK();
}
