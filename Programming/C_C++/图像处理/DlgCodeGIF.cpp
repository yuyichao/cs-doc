// DlgCodeGIF.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgCodeGIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCodeGIF dialog


CDlgCodeGIF::CDlgCodeGIF(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCodeGIF::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCodeGIF)
	m_strFilePath = _T("");
	m_bInterlace = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgCodeGIF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCodeGIF)
	DDX_Text(pDX, IDC_EDIT_FileName, m_strFilePath);
	DDX_Radio(pDX, IDC_RADIO1, m_bInterlace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCodeGIF, CDialog)
	//{{AFX_MSG_MAP(CDlgCodeGIF)
	ON_BN_CLICKED(IDC_btnSaveAs, OnbtnSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCodeGIF message handlers

void CDlgCodeGIF::OnbtnSaveAs() 
{
	// 提示选择保存文件路径
	
	CFileDialog dlg(FALSE, "GIF", m_strFilePath, OFN_HIDEREADONLY, 
		"GIF图像文件 (*.GIF) | *.GIF|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户指定的文件路径
	m_strFilePath = dlg.GetPathName();
	
	// 更新
	UpdateData(FALSE);
	
}

void CDlgCodeGIF::OnOK() 
{
	CFileFind fFind;
	
	// 更新
	UpdateData(TRUE);
	
	// 判断用户指定的文件是否存在
	if (fFind.FindFile(m_strFilePath, 0) != 0)
	{
		// 询问用户是否覆盖
		if (MessageBox("指定的文件已经存在，是否覆盖？", "系统提示", 
			MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) != IDYES)
		{
			// 退出
			return;
		}
	}
	
	// 调用默认的OnOK事件
	CDialog::OnOK();
}
