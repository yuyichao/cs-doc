// ReInitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMSDSD.h"
#include "ReInitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReInitDlg dialog


CReInitDlg::CReInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReInitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReInitDlg)
	m_lTotleEMSSpace = 640;
	//}}AFX_DATA_INIT
}


void CReInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReInitDlg)
	DDX_Text(pDX, IDC_TOTLEEMSSPACE, m_lTotleEMSSpace);
	DDV_MinMaxLong(pDX, m_lTotleEMSSpace, 640, 4194304);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReInitDlg, CDialog)
	//{{AFX_MSG_MAP(CReInitDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReInitDlg message handlers
