// DlgSharpThre.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgSharpThre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSharpThre dialog


CDlgSharpThre::CDlgSharpThre(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSharpThre::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSharpThre)
	m_bThre = 0;
	//}}AFX_DATA_INIT
}


void CDlgSharpThre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSharpThre)
	DDX_Text(pDX, IDC_EDIT_Thre, m_bThre);
	DDV_MinMaxByte(pDX, m_bThre, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSharpThre, CDialog)
	//{{AFX_MSG_MAP(CDlgSharpThre)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSharpThre message handlers
