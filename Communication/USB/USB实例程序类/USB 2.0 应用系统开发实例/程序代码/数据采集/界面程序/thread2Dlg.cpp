// thread2Dlg.cpp : implementation file
//

#define NOCRYPT			// prevent attempt to include missing files
#define _INC_EXCPT
#include "stdafx.h"
#include <stdlib.h>
#include "thread2.h"
#include "thread2Dlg.h"
#include <math.h>
#include <winioctl.h>
#include <windows.h>
#include "device.h"	// DriverWorks
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CEvent g_event1Start; // creates autoreset events
CEvent g_event2Start;
CEvent g_caijistart;
double buf[1024];
char *bufread;
char *bufread1;
char *bufread2;
char *bufread3;
char *bufread4;
int data1;
int count1=0,count2=0,count3=0,gaptime,channel;
#define Ds8Device_CLASS_GUID \
 { 0x665ec980, 0xaa46, 0x49ed, { 0xbc, 0xbf, 0xc7, 0xa4, 0xa2, 0x48, 0x5d, 0x15 } }

HANDLE	hDevice = INVALID_HANDLE_VALUE;
GUID ClassGuid = Ds8Device_CLASS_GUID;
HANDLE OpenByInterface(GUID* pClassGuid, DWORD instance, PDWORD pError);
void CloseIfOpen(void);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
UINT ThreadPoc2(LPVOID pParam)
{
	::WaitForSingleObject(g_event2Start,INFINITE);
      int j;
	  for(j=0;j<5000;j++)
	  {
		  count2=count2+1;
		  Sleep(1);
	  }
	  return 0;
}

UINT ThreadPoc1(LPVOID pParam)
{	
	::WaitForSingleObject(g_event1Start,INFINITE);
	/////////////find
	DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
    if(hDevice==NULL)
	{
		return 0;
	}
	//////////////write
    char	*bufwrite;
	ULONG	nWritten;
	bufwrite = (char *) malloc(2);
	bufwrite[0]=gaptime;
   	bufwrite[1]=0x80|channel;
 
	WriteFile(hDevice, bufwrite, 2, &nWritten, NULL);  
	free(bufwrite);

	ULONG	nRead;
	bufread = (char *) malloc(256); 

	int i=0;
	
	for(i=0;i<4;i++)
	{
		ReadFile(hDevice, bufread+i*64, 64, &nRead, NULL);
		count1=count1+1;
	}

	free(bufread);
	return 0;

}
UINT caijipoc(LPVOID pParam)
{
    ::WaitForSingleObject(g_caijistart,INFINITE);
    int i;
	for(i=0;i<1024;i++)
	{
		buf[i]=sin(i/40)*100;
		count1=count1+1;	
	}	
	
	//g_eventshow.SetEvent();
	return 0;
	
}

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
// CThread2Dlg dialog

CThread2Dlg::CThread2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThread2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThread2Dlg)
	m_data4 = 2;
	m_gap = 100;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThread2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThread2Dlg)
//	DDX_Text(pDX, IDC_EDIT1, m_data1);
//	DDX_Text(pDX, IDC_EDIT2, m_data2);
	//DDX_Text(pDX, IDC_EDIT3, m_data3);
	DDX_Text(pDX, IDC_EDIT4, m_data4);
	DDX_Text(pDX, IDC_EDIT5, m_gap);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThread2Dlg, CDialog)
	//{{AFX_MSG_MAP(CThread2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnCAIJI)
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
//	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
//	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThread2Dlg message handlers
HANDLE OpenByInterface(
		GUID* pClassGuid,	// points to the GUID that identifies the interface class
		DWORD instance,		// specifies which instance of the enumerated devices to open
		PDWORD pError		// address of variable to receive error status
		)
{
	HANDLE hDev;
	CDeviceInterfaceClass DevClass(pClassGuid, pError);

	if (*pError != ERROR_SUCCESS)
		return INVALID_HANDLE_VALUE;

	CDeviceInterface DevInterface(&DevClass, instance, pError);

	if (*pError != ERROR_SUCCESS)
		return INVALID_HANDLE_VALUE;

	hDev = CreateFile(
		DevInterface.DevicePath(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);

	if (hDev == INVALID_HANDLE_VALUE)
		*pError = GetLastError();

	return hDev;
}
BOOL CThread2Dlg::OnInitDialog()
{
    DWORD	Error;
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	AfxBeginThread(ThreadPoc1,GetSafeHwnd());
    AfxBeginThread(ThreadPoc2,GetSafeHwnd());

	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
    if(hDevice== INVALID_HANDLE_VALUE )
	{
		MessageBox("找不到指定硬件");
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThread2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThread2Dlg::OnPaint() 
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
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThread2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CThread2Dlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	  g_event1Start.SetEvent();
	  //m_data1=count1;
	  m_data2=count2;
	 
          m_data1=count1;
	  SetTimer(1,120,NULL);

	  UpdateData(false);
}

void CThread2Dlg::OnCAIJI() 
{
	// TODO: Add your control notification handler code here
	///find device
		int i;
	m_Low = 0;
	m_High = 256;
	m_now =0;
   g_event1Start.SetEvent();
   gaptime=m_gap;
	channel=m_data4;
   while(count1!=4);
    Sleep(10);
   for(i=0;i<256;i++)
	{
	 buf[i]= *(bufread+i);
	 }
    SetTimer(1,120,NULL);

	
    
  
}
///

////
void CThread2Dlg::DrawWave(CDC *pDC)
{
	CRect rect;
	
	CString str;
	int i;
	int m_left,m_top,m_right,m_bottom;
	
    int m_Interval = (m_High - m_Low)/10;//时间间隔
	if (m_Interval < 1)  m_Interval = 1;
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	pWnd->GetClientRect(&rect);
	pDC->Rectangle(&rect);
	
	
    m_left = rect.left+10;
	m_top = rect.top+10;
	m_right = rect.right-10;
	m_bottom = rect.bottom-20;
	
	int m_IntervalPan = (m_right - m_left)/11;
	m_data1=m_bottom;
	m_data2=m_top;
	m_data3=sin(1/2.0);
	//UpdateData(false);
	if (m_IntervalPan < 1 ) m_IntervalPan =1;
	
	// 创建画笔对象
	CPen* pPenRed = new CPen;
	
	// 红色画笔
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// 创建画笔对象
	CPen* pPenGreen = new CPen;
	
	// 绿色画笔
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// 绘制坐标轴
	pDC->MoveTo(m_left,m_top);
	
	// 垂直轴
	pDC->LineTo(m_left,m_bottom);
	
	// 水平轴
	pDC->LineTo(m_right,m_bottom);
	
	
	// 写X轴刻度值
	for(i=0;i<10;i++)
	{
		//str.Format(_T("%d"),m_Min+i*m_Interval);
		str.Format(_T("%d"),m_Low+i*m_Interval);
		pDC->TextOut(m_left+i*m_IntervalPan,m_bottom+3,str);		
	}
	//str.Format(_T("%d"),m_Max);
	str.Format(_T("%d"),m_High);
	pDC->TextOut(m_left+10*m_IntervalPan,m_bottom+3,str);		
	
	
	// 绘制X轴刻度
	for (i = m_left; i < m_right-20; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, m_bottom);
			pDC->LineTo(i + 10, m_bottom+4);
		}
		else
		{
			// 10的倍数
			pDC->MoveTo(i + 10, m_bottom);
			pDC->LineTo(i + 10, m_bottom+2);
		}
	}
	
	// 绘制Y轴箭头
	pDC->MoveTo(m_right-5,m_bottom-5);
	pDC->LineTo(m_right,m_bottom);
	pDC->LineTo(m_right-5,m_bottom+5);
	
	// 绘制X轴箭头	
	pDC->MoveTo(m_left-5,m_top+5);
	pDC->LineTo(m_left,m_top);
	pDC->LineTo(m_left+5,m_top+5);
	

	
	// 绘制Y轴网格 选择绿色画笔
    pDC->SelectObject(pPenGreen);	
    //
	int iTemp = (m_bottom  - m_top)/5;
    for (i = 1 ;i <= 5 ;i++)
	{
		pDC->MoveTo(m_left,m_bottom - i*iTemp);
		pDC->LineTo(m_right,m_bottom - i*iTemp);
	}

    //数组赋值
	if(m_now<1024)
	{
	    m_now++;
	}
	else
	{
		m_now = 0;
	}
    
	for(i = 0;i<256;i=i+1)
	{ 
		//m_lCount[i]=(rand()%10+rand()%100+rand()%1000)%1024;
		
        m_lCount[i] = sin(i/10.0-m_now)*200; 
	}
	//m_data3=m_lCount[1];

	//UpdateData(false);
	int xTemp;
	double yTemp;
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);
	for (i = m_Low; i <= m_High; i=i+3)
	{	xTemp = m_left+(i-m_Low)*m_IntervalPan/m_Interval;
		//yTemp = m_bottom/2 - (int) (m_lCount[i] * m_bottom / 1024);	
		//xTemp = m_left+i;
		yTemp = m_bottom/2 + buf[i];
		if (yTemp < m_top) yTemp = m_top;
		if((xTemp >=m_left)&&(xTemp <=m_right))
		{
			pDC->MoveTo(xTemp, m_bottom/2);
			pDC->LineTo(xTemp, yTemp);
		}
	}
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	return;
}
void CThread2Dlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	pWnd->GetClientRect(&rect);
	// 指针
	pDC = pWnd->GetDC();	
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	//pDC->Rectangle(&rect);

	
    //内存绘图
    CBitmap memBitmap;
	CBitmap* pOldBmp = NULL;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);
	pOldBmp = memDC.SelectObject(&memBitmap);
	memDC.BitBlt(rect.left,rect.top,rect.right,rect.bottom,pDC,0,0,SRCCOPY);

	DrawWave(&memDC);
	
	pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	memBitmap.DeleteObject();

	CDialog::OnTimer(nIDEvent);
}

void CThread2Dlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
	if(hDevice==NULL)
	{
		m_data1=1;
		
	}
	else
	{
		m_data1=2;
	}
	UpdateData(false);
}

void CThread2Dlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	char	*bufwrite;
	ULONG	nWritten;
	bufwrite = (char *) malloc(4);
	bufwrite[0]=1;
   	bufwrite[1]=1;
    bufwrite[2]=1;
	bufwrite[3]=1;	
	WriteFile(hDevice, bufwrite, 4, &nWritten, NULL);  
	free(bufwrite);
}

void CThread2Dlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	//char	*bufread,i;
	int i;
	ULONG	nRead;
	bufread = (char *) malloc(1024);
	  for(i=0;i<1024;i++)
	  {
	  ReadFile(hDevice, bufread+i, 1, &nRead, NULL);
	  //Sleep(100);
	  }	      
	  //free(bufread);
}

void CThread2Dlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	m_Low = 0;
	m_High = 1024;
	m_now =0;  
	for(int i=0;i<1024;i++)
	{
	 count1=count1+1;
	 buf[i]=sin(bufread[i]/10.0)*100;
	 
	 //Sleep(1);
	}
    SetTimer(1,120,NULL);

}
