// DlgGeoZoom.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgGeoZoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoZoom dialog


CDlgGeoZoom::CDlgGeoZoom(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGeoZoom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGeoZoom)
	m_XZoom = 0.0f;
	m_YZoom = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgGeoZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGeoZoom)
	DDX_Text(pDX, IDC_EDIT_XZoom, m_XZoom);
	DDV_MinMaxFloat(pDX, m_XZoom, 0.25f, 4.f);
	DDX_Text(pDX, IDC_EDIT_YZoom, m_YZoom);
	DDV_MinMaxFloat(pDX, m_YZoom, 0.25f, 4.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGeoZoom, CDialog)
	//{{AFX_MSG_MAP(CDlgGeoZoom)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoZoom message handlers
