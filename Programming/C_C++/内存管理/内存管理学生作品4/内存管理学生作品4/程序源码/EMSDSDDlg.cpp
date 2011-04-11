// EMSDSDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EMSDSD.h"
#include "EMSDSDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEMSDSDDlg dialog

CEMSDSDDlg::CEMSDSDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEMSDSDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEMSDSDDlg)
	m_lTotleEMSSpace = 640;
	m_lApplyEMSSpace = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEMSDSDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEMSDSDDlg)
	DDX_Control(pDX, IDC_REALSEWORK, m_ccReleaseWork);
	DDX_Text(pDX, IDC_TOTLEEMSSPACE, m_lTotleEMSSpace);
	DDV_MinMaxLong(pDX, m_lTotleEMSSpace, 640, 4194304);
	DDX_Text(pDX, IDC_APPLYEMSSAPCE, m_lApplyEMSSpace);
	DDV_MinMaxLong(pDX, m_lApplyEMSSpace, 0, 4194304);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEMSDSDDlg, CDialog)
	//{{AFX_MSG_MAP(CEMSDSDDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REINIT, OnReinit)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_RELEASE, OnRelease)
	ON_BN_CLICKED(IDC_FIRSTARITHMETIC, OnFirstarithmetic)
	ON_BN_CLICKED(IDC_BESTARITHMETIC, OnBestarithmetic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEMSDSDDlg message handlers

BOOL CEMSDSDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_myEMS = NULL;
	m_iArithmetic = 1;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	m_ccReleaseWork.AddString(_T("<没有作业>"));

	CheckRadioButton(IDC_FIRSTARITHMETIC,IDC_BESTARITHMETIC,IDC_FIRSTARITHMETIC);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	OnReinit();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEMSDSDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEMSDSDDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	CClientDC clientDC(this);
	CBrush brushGreen, brushOrange;
	brushGreen.CreateSolidBrush(RGB(0,200,0));
	brushOrange.CreateSolidBrush(RGB(240,160,50));
	clientDC.FillRect(CRect(100,190,170,215),&brushGreen);
	clientDC.FillRect(CRect(255,190,325,215),&brushOrange);
	clientDC.FillRect(CRect(20,100,570,150),&brushGreen);
	clientDC.SetBkMode(TRANSPARENT);
	clientDC.TextOut(18,148,_T("^"),1);
	clientDC.TextOut(10,155,_T("0 K"),3);
	CString s1, s2;
	s1.Format("%d",m_lTotleEMSSpace);
	s2.Format("%d",m_lTotleEMSSpace/2);
	int num, temp;
	for(num = 0, temp = m_lTotleEMSSpace; temp > 0; temp /= 10, num++);
	clientDC.TextOut(568-7*num,155,s1.GetBuffer(num),num);
	clientDC.TextOut(567,148,_T("^"),1);
	clientDC.TextOut(575,155,_T("K"),1);
	for(num = 0, temp = m_lTotleEMSSpace/2; temp > 0; temp /= 10, num++);
	clientDC.TextOut(293,148,_T("^"),1);
	clientDC.TextOut(303-7*num,155,s2.GetBuffer(num),num);
	clientDC.TextOut(310,155,_T("K"),1);
	struct EMSTable * EMSTemp;
	for(EMSTemp = m_myEMS->EMST; EMSTemp !=NULL ; EMSTemp = EMSTemp->next)
	{
		if(!EMSTemp->isFree)
		{
			clientDC.FillRect(CRect(20+EMSTemp->start*550/m_lTotleEMSSpace,100,21+EMSTemp->end*550/m_lTotleEMSSpace,150),&brushOrange);
		}
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEMSDSDDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEMSDSDDlg::OnReinit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_myEMS != NULL) delete m_myEMS;
	m_myEMS = new MyEMS();
	m_myEMS->Init(m_lTotleEMSSpace);
	Invalidate();
}

void CEMSDSDDlg::OnApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_lApplyEMSSpace > 0) m_myEMS->Distribute(m_iArithmetic, m_lApplyEMSSpace);
	m_ccReleaseWork.ResetContent();
	struct WorkList * temp = m_myEMS->worklist;
	for(; temp->next != NULL; temp = temp->next)
	{
		CString s, r;
		s.Format("%d",temp->num);
		s+="#作业 大小：";
		r.Format("%d",(int)temp->size);
		s+=r;
		s+="K";
		m_ccReleaseWork.AddString(s);
	}
	Invalidate();
}

void CEMSDSDDlg::OnRelease() 
{
	// TODO: Add your control notification handler code here
	struct WorkList * temp = m_myEMS->worklist;
	struct EMSTable * et, *et1;
	for(int i = 0; temp->next != NULL; temp = temp->next, i++)
	{
		if(i == m_ccReleaseWork.GetCurSel())
		{
			et = temp->position;
			et->isFree = true;
			if(et->before!=NULL&&et->next!=NULL&&et->before->isFree&&et->next->isFree)
			{
				et->before->end=et->next->end;
				et->before->size+=(et->size+et->next->size);
				et->before->next=et->next->next;
				if(et->next->next!=NULL) et->next->next->before=et->before;
				delete et->next;
				delete et;
			}
			else if(et->before!=NULL&&et->before->isFree)
			{
				et->before->end=et->end;
				et->before->size+=et->size;
				et->before->next=et->next;
				et->next->before=et->before;
				delete et;
			}
			else if(et->next!=NULL&&et->next->isFree)
			{
				et->end=et->next->end;
				et->size+=et->next->size;
				et1=et->next;
				if(et->next->next!=NULL) et->next->next->before=et;
				et->next=et->next->next;
				delete et1;
			}
			if(temp == m_myEMS->worklist) m_myEMS->worklist=temp->next;
			else
			{
				temp->before->next = temp->next;
				temp->next->before = temp->before;
			}
			delete temp;
			break;
		}
	}
	m_ccReleaseWork.ResetContent();
	for(temp = m_myEMS->worklist; temp->next != NULL; temp = temp->next)
	{
		CString s, r;
		s.Format("%d",temp->num);
		s+="#作业 大小：";
		r.Format("%d",(int)temp->size);
		s+=r;
		s+="K";
		m_ccReleaseWork.AddString(s);
	}
	Invalidate();
}

void CEMSDSDDlg::OnFirstarithmetic() 
{
	// TODO: Add your control notification handler code here
	m_iArithmetic = 1;	
}

void CEMSDSDDlg::OnBestarithmetic() 
{
	// TODO: Add your control notification handler code here
	m_iArithmetic = 2;	
}
