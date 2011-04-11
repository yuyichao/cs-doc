// ad9854Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ad9854.h"
#include "ad9854Dlg.h"
#include <winioctl.h>
#include <windows.h>
#include "device.h"	
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define Ds8Device_CLASS_GUID \
 { 0x665ec980, 0xaa46, 0x49ed, { 0xbc, 0xbf, 0xc7, 0xa4, 0xa2, 0x48, 0x5d, 0x15 } }

HANDLE	hDevice = INVALID_HANDLE_VALUE;
GUID ClassGuid = Ds8Device_CLASS_GUID;
HANDLE OpenByInterface(GUID* pClassGuid, DWORD instance, PDWORD pError);
void CloseIfOpen(void);

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
// CAd9854Dlg dialog

CAd9854Dlg::CAd9854Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAd9854Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAd9854Dlg)
	m_phase1 = 0;
	m_phase2 = 0;
	m_freq1 = 0;
	m_deltfreq = 0;
	m_upclock = 0;
	m_vfreq = 0;
	m_amp = 0;
	m_gatetime = 0;
	m_freq2 = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAd9854Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAd9854Dlg)
	DDX_Text(pDX, IDC_EDIT1, m_phase1);
	DDX_Text(pDX, IDC_EDIT2, m_phase2);
	DDX_Text(pDX, IDC_EDIT3, m_freq1);
	DDX_Text(pDX, IDC_EDIT5, m_deltfreq);
	DDX_Text(pDX, IDC_EDIT6, m_upclock);
	DDX_Text(pDX, IDC_EDIT7, m_vfreq);
	DDX_Text(pDX, IDC_EDIT8, m_amp);
	DDX_Text(pDX, IDC_EDIT9, m_gatetime);
	DDX_Text(pDX, IDC_EDIT4, m_freq2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAd9854Dlg, CDialog)
	//{{AFX_MSG_MAP(CAd9854Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, Ontest)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAd9854Dlg message handlers

BOOL CAd9854Dlg::OnInitDialog()
{
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
	DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
	if(hDevice==INVALID_HANDLE_VALUE)
	{
		MessageBox("找不到指定设备");
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAd9854Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAd9854Dlg::OnPaint() 
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
HCURSOR CAd9854Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAd9854Dlg::Ontest() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int test;
	test=GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO5);
   // if(test==IDC_RADIO1)
	//{
	DWORD DATA1,DATA2,DATA3;
	DATA1=m_freq1;
	DATA2=DATA1&0xFF0;
	DATA3=DATA2>>8;
	//m_freq2=DATA1;
	//m_phase1=DATA2;
	//m_phase2=DATA3;
	LONGLONG freq=0x011418ff,freq2;
	char freq1,freq3,freq4,freq5,freq6;
	//freq=m_freq2*14073748;
	freq1=(freq&0xffff)>>8;
	freq2=char((freq&0xff0000)>>16);
	freq3=char((freq&0x0000ff000000)>>24);
	freq4=char((freq&0x000000ff0000)>>16);
	freq5=(freq&0x00000000ff00)>>8;
	freq6=(freq&0x0000000000ff)>>0;
    m_phase1=DWORD(freq1);
	m_phase2=DWORD(freq2);
	m_amp=freq3;
	m_upclock=freq4;
    m_vfreq=freq5;
	m_gatetime=freq6;
	//}
	UpdateData(FALSE);
	
}
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
void CAd9854Dlg::singletone()
{
	DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
	
	//////////////write
    char	*bufwrite;
	ULONG	nWritten;
	bufwrite = (char *) malloc(8);
	LONGLONG freq,freq1,freq2,freq3,freq4,freq5,freq6;
	 char amp;
	//频率字1
	freq=m_freq1*938249922;
	
	freq1=(freq&0xff0000000000)>>40;
	freq2=(freq&0xff00000000)>>32;
	freq3=(freq&0xff000000)>>24;
	freq4=(freq&0xff0000)>>16;
	freq5=(freq&0xff00)>>8;
	freq6=(freq&0xff)>>0;
	///幅度码
	amp=m_amp;
	bufwrite[0]=1;//标示符，SINGLE-TONE模式为1
	bufwrite[1]=char(freq1);
   	bufwrite[2]=char(freq2);
    bufwrite[3]=char(freq3);
	bufwrite[4]=char(freq4);
	bufwrite[5]=char(freq5);
	bufwrite[6]=char(freq6);
    bufwrite[7]=amp;
	//传送幅度和频率码
	WriteFile(hDevice, bufwrite, 7, &nWritten, NULL);  
	free(bufwrite);

   
}
void CAd9854Dlg::unrampedfsk()
{
    DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
	
    char	*bufwrite;
	ULONG	nWritten;
	bufwrite = (char *) malloc(15);
	LONGLONG freq1,freq2,freq11,freq12,freq13,freq14,freq15,freq16,freq21,freq22,freq23,freq24,freq25,freq26;
	char amp,gaterate;
	///频率字1
	freq1=m_freq1*938249922;
	freq11=(freq1&0xff0000000000)>>40;
	freq12=(freq1&0xff00000000)>>32;
	freq13=(freq1&0xff000000)>>24;
	freq14=(freq1&0xff0000)>>16;
	freq15=(freq1&0xff00)>>8;
	freq16=(freq1&0xff)>>0;
	//频率字2
	freq2=m_freq2*938249922;
	freq21=(freq2&0xff0000000000)>>40;
	freq22=(freq2&0xff00000000)>>32;
	freq23=(freq2&0xff000000)>>24;
	freq24=(freq2&0xff0000)>>16;
	freq25=(freq2&0xff00)>>8;
	freq26=(freq2&0xff)>>0;
	//幅度码
	amp=m_amp;
	//FSK RATE
     gaterate=m_gatetime;

	bufwrite[0]=2;//标示符,unrampedfsk为2;
	bufwrite[1]=char(freq11);
   	bufwrite[2]=char(freq12);
    bufwrite[3]=char(freq13);
	bufwrite[4]=char(freq14);
	bufwrite[5]=char(freq15);
	bufwrite[6]=char(freq16);
	bufwrite[7]=char(freq21);
   	bufwrite[8]=char(freq22);
    bufwrite[9]=char(freq23);
	bufwrite[10]=char(freq24);
	bufwrite[11]=char(freq25);
	bufwrite[12]=char(freq26);
    bufwrite[13]=amp;
    bufwrite[14]=gaterate;
	WriteFile(hDevice, bufwrite, 15, &nWritten, NULL);  
	free(bufwrite);
}
void CAd9854Dlg::bpsk()
{
    DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
	
    char	*bufwrite;
	ULONG	nWritten;
	bufwrite = (char *) malloc(7);
	int phase1,phase2,phase11,phase12,phase21,phase22;
	char amp,gaterate;
	///相位字1
	phase1=m_phase1*5215;
	phase11=(phase1&0xff00)>>8;
	phase12=(phase1&0xff)>>0;
	//相位字2
	phase2=phase2*5215;
	phase21=(phase2&0xff00)>>8;
	phase22=(phase2&0xff)>>0;
	
	//幅度码
	amp=m_amp;
	//FSK RATE
     gaterate=m_gatetime;

	bufwrite[0]=2;//标示符,unrampedfsk为2;
	bufwrite[1]=char(phase11);
   	bufwrite[2]=char(phase12);
    bufwrite[3]=char(phase21);
	bufwrite[4]=char(phase21);
	bufwrite[5]=amp;
    bufwrite[6]=gaterate;
	WriteFile(hDevice, bufwrite, 7, &nWritten, NULL);  
	free(bufwrite);
}
void CAd9854Dlg::rampedfsk()
{
    DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
	UpdateData(TRUE);
	m_freq2=2;
	UpdateData(FALSE);
    char	*bufwrite;
	ULONG	nWritten;
	bufwrite = (char *) malloc(24);
	LONGLONG freq1,freq2,fsw1,fsw2,fsw3,fsw4,fsw5,fsw6,ramp1,ramp2,ramp3,fsw,ramp,freq11,freq12,freq13,freq14,freq15,freq16,freq21,freq22,freq23,freq24,freq25,freq26;
    char amp,gaterate;
	 
	///频率字1
	freq1=m_freq1*938249922;
	freq11=(freq1&0xff0000000000)>>40;
	freq12=(freq1&0xff00000000)>>32;
	freq13=(freq1&0xff000000)>>24;
	freq14=(freq1&0xff0000)>>16;
	freq15=(freq1&0xff00)>>8;
	freq16=(freq1&0xff)>>0;
	//频率字2
	freq2=m_freq2*938249922;
	freq21=(freq2&0xff0000000000)>>40;
	freq22=(freq2&0xff00000000)>>32;
	freq23=(freq2&0xff000000)>>24;
	freq24=(freq2&0xff0000)>>16;
	freq25=(freq2&0xff00)>>8;
	freq26=(freq2&0xff)>>0;
	//幅度码
	amp=m_amp;
	//FSK RATE
     gaterate=m_gatetime;
     //步进频率
    fsw=m_deltfreq*938249922;

	fsw1=(fsw&0xff0000000000)>>40;
	fsw2=(fsw&0xff00000000)>>32;
	fsw3=(fsw&0xff000000)>>24;
	fsw4=(fsw&0xff0000)>>16;
	fsw5=(fsw&0xff00)>>8;
	fsw6=(fsw&0xff)>>0;
    ///ramp
	ramp=m_vfreq;
	ramp1=(ramp&0xff0000)>>16;
	ramp2=(ramp&0xff00)>>8;
	ramp3=(ramp&0xff)>>0;
	//////////
	bufwrite[0]=3;//标示符,rampedfsk为3;
	bufwrite[1]=char(freq11);
   	bufwrite[2]=char(freq12);
    bufwrite[3]=char(freq13);
	bufwrite[4]=char(freq14);
	bufwrite[5]=char(freq15);
	bufwrite[6]=char(freq16);
	bufwrite[7]=char(freq21);
   	bufwrite[8]=char(freq22);
    bufwrite[9]=char(freq23);
	bufwrite[10]=char(freq24);
	bufwrite[11]=char(freq25);
	bufwrite[12]=char(freq26);
    bufwrite[13]=amp;
    bufwrite[14]=gaterate;
	bufwrite[15]=char(fsw1);
   	bufwrite[16]=char(fsw2);
    bufwrite[17]=char(fsw3);
	bufwrite[18]=char(fsw4);
	bufwrite[19]=char(fsw5);
	bufwrite[20]=char(fsw6);
	bufwrite[21]=char(ramp1);
   	bufwrite[22]=char(ramp2);
    bufwrite[23]=char(ramp3);

	WriteFile(hDevice, bufwrite, 24, &nWritten, NULL);  
	free(bufwrite);
}
void CAd9854Dlg::chirp()
{
	UpdateData(TRUE);
	DWORD	Error;
	hDevice = OpenByInterface( &ClassGuid, 0, &Error);
	
    char	*bufwrite;
	ULONG	nWritten;
	bufwrite = (char *) malloc(18);
	LONGLONG freq1,freq11,freq12,freq13,freq14,freq15,freq16,fsw,ramp,fsw1,fsw2,fsw3,fsw4,fsw5,fsw6,ramp1,ramp2,ramp3;
	char amp,upclock;


	///频率字1
	freq1=m_freq1*938249922;
	freq11=(freq1&0xff0000000000)>>40;
	freq12=(freq1&0x00ff00000000)>>32;
	freq13=(freq1&0x0000ff000000)>>24;
	freq14=(freq1&0x000000ff0000)>>16;
	freq15=(freq1&0x00000000ff00)>>8;
	freq16=(freq1&0x0000000000ff)>>0;
	
	//幅度码
	amp=m_amp;
	//upclock
     upclock=m_upclock;
     //步进频率
    fsw=m_deltfreq*938249922;

	fsw1=(fsw&0xff0000000000)>>40;
	fsw2=(fsw&0x00ff00000000)>>32;
	fsw3=(fsw&0x0000ff000000)>>24;
	fsw4=(fsw&0x000000ff0000)>>16;
	fsw5=(fsw&0x00000000ff00)>>8;
	fsw6=(fsw&0x0000000000ff)>>0;
    ///ramp
	ramp=m_vfreq;
	ramp1=(ramp&0x000000ff0000)>>16;
	ramp2=(ramp&0x00000000ff00)>>8;
	ramp3=(ramp&0x0000000000ff)>>0;
	//////////
	bufwrite[0]=3;//标示符,rampedfsk为3;
	bufwrite[1]=char(freq11);
   	bufwrite[2]=char(freq12);
    bufwrite[3]=char(freq13);
	bufwrite[4]=char(freq14);
	bufwrite[5]=char(freq15);
	bufwrite[6]=char(freq16);
	bufwrite[7]=amp;
   	bufwrite[8]=upclock;      
	bufwrite[9]=char(fsw1);
   	bufwrite[10]=char(fsw2);
    bufwrite[11]=char(fsw3);
	bufwrite[12]=char(fsw4);
	bufwrite[13]=char(fsw5);
	bufwrite[14]=char(fsw6);
	bufwrite[15]=char(ramp1);
   	bufwrite[16]=char(ramp2);
    bufwrite[17]=char(ramp3);

	WriteFile(hDevice, bufwrite, 18, &nWritten, NULL);  
	free(bufwrite);
	m_freq1=1;
	UpdateData(FALSE);
}
void CAd9854Dlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	int radiochecked;
	radiochecked=GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO5);
	switch(radiochecked)
	{
	case IDC_RADIO1:	
		singletone();
	     break;
    case IDC_RADIO2:
		unrampedfsk();
         break;
	case IDC_RADIO3:
		rampedfsk();
	    break;
	case IDC_RADIO4:
		chirp();
	    break;
	case IDC_RADIO5:
		bpsk();
        break;
	default:
		break;	
}
}
