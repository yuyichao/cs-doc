// DlgGeoRota.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgGeoRota.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoRota dialog


CDlgGeoRota::CDlgGeoRota(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGeoRota::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGeoRota)
	m_iRotateAngle = 0;
	//}}AFX_DATA_INIT
}


void CDlgGeoRota::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGeoRota)
	DDX_Text(pDX, IDC_EDIT_Rotate, m_iRotateAngle);
	DDV_MinMaxInt(pDX, m_iRotateAngle, 0, 360);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGeoRota, CDialog)
	//{{AFX_MSG_MAP(CDlgGeoRota)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoRota message handlers
