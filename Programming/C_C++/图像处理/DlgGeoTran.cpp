// DlgGeoTran.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgGeoTran.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoTran dialog


CDlgGeoTran::CDlgGeoTran(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGeoTran::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGeoTran)
	m_XOffset = 0;
	m_YOffset = 0;
	//}}AFX_DATA_INIT
}


void CDlgGeoTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGeoTran)
	DDX_Text(pDX, IDC_EDIT_XOffset, m_XOffset);
	DDX_Text(pDX, IDC_EDIT_YOffset, m_YOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGeoTran, CDialog)
	//{{AFX_MSG_MAP(CDlgGeoTran)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoTran message handlers
