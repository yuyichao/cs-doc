// DlgIntensity.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgIntensity.h"
#include "DIBAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity dialog


CDlgIntensity::CDlgIntensity(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIntensity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIntensity)
	m_iLowGray = 0;
	m_iUpGray = 0;
	//}}AFX_DATA_INIT
}


void CDlgIntensity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIntensity)
	DDX_Text(pDX, IDC_EDIT_LowGray, m_iLowGray);
	DDV_MinMaxInt(pDX, m_iLowGray, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UpGray, m_iUpGray);
	DDV_MinMaxInt(pDX, m_iUpGray, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIntensity, CDialog)
	//{{AFX_MSG_MAP(CDlgIntensity)
	ON_WM_PAINT()
	ON_EN_KILLFOCUS(IDC_EDIT_LowGray, OnKillfocusEDITLowGray)
	ON_EN_KILLFOCUS(IDC_EDIT_UpGray, OnKillfocusEDITUpGray)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIntensity message handlers

BOOL CDlgIntensity::OnInitDialog() 
{
	// 指向源图像象素的指针
	unsigned char * lpSrc;
	
	// 循环变量
	LONG i;
	LONG j;
	
	// 调用默认OnInitDialog函数
	CDialog::OnInitDialog();
	
	// 获取绘制直方图的标签
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// 设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	// 重置计数为0
	for (i = 0; i < 256; i ++)
	{
		// 清零
		m_lCount[i] = 0;
	}
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(m_lWidth * 8);
	
	// 计算各个灰度值的计数
	for (i = 0; i < m_lHeight; i ++)
	{
		for (j = 0; j < m_lWidth; j ++)
		{
			lpSrc = (unsigned char *)m_lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			m_lCount[*(lpSrc)]++;
		}
	}
	
	// 初始化拖动状态
	m_iIsDraging = 0;
	
	// 返回TRUE
	return TRUE;
}

void CDlgIntensity::OnKillfocusEDITLowGray() 
{
	// 保存变动（控件中数值保存到成员变量中）
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		// 更新（成员变量中数值保存到控件中）
		UpdateData(FALSE);
	}
	
	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgIntensity::OnKillfocusEDITUpGray() 
{
	// 保存变动
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘直方图
	InvalidateRect(m_MouseRect, TRUE);
	
}

void CDlgIntensity::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 当用户单击鼠标左键开始拖动
	
	// 判断是否在接受鼠标事件的有效区域中
	if(m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_iLowGray))
		{
			
			// 设置拖动状态1，拖动下限
			m_iIsDraging = 1;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_iUpGray))
		{
			
			// 设置拖动状态为2，拖动上限
			m_iIsDraging = 2;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// 默认单击鼠标左键处理事件
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgIntensity::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	// 判断是否在接受鼠标事件的有效区域中
	if(m_MouseRect.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_iIsDraging != 0)
		{
			// 判断正在拖动上限还是下限
			if (m_iIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_MouseRect.left < m_iUpGray)
				{
					// 更改下限
					m_iLowGray = point.x - m_MouseRect.left;
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_iLowGray = m_iUpGray - 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_iUpGray - 1;
				}
			}
			else
			{
				// 正在拖动上限
				
				// 判断是否上限>下限
				if (point.x - m_MouseRect.left > m_iLowGray)
				{
					// 更改下限
					m_iUpGray = point.x - m_MouseRect.left;
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_iUpGray = m_iLowGray + 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_iLowGray + 1;
				}
			}
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘直方图
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_iLowGray) || point.x == (m_MouseRect.left + m_iUpGray))
		{
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgIntensity::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 当用户释放鼠标左键停止拖动
	if (m_iIsDraging != 0)
	{
		// 重置拖动状态
		m_iIsDraging = 0;
	}
	
	// 默认释放鼠标左键处理事件
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgIntensity::OnPaint() 
{
	// 字符串
	CString str;
	
	// 循环变量
	LONG i;
	
	// 最大计数
	LONG lMaxCount = 0;
	
	// 设备上下文
	CPaintDC dc(this);
	
	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
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
	pDC->MoveTo(10,10);
	
	// 垂直轴
	pDC->LineTo(10,280);
	
	// 水平轴
	pDC->LineTo(320,280);
	
	// 写X轴刻度值
	str.Format("0");
	pDC->TextOut(10, 283, str);
	str.Format("50");
	pDC->TextOut(60, 283, str);
	str.Format("100");
	pDC->TextOut(110, 283, str);
	str.Format("150");
	pDC->TextOut(160, 283, str);
	str.Format("200");
	pDC->TextOut(210, 283, str);
	str.Format("255");
	pDC->TextOut(265, 283, str);
	
	// 绘制X轴刻度
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 284);
		}
		else
		{
			// 10的倍数
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 282);
		}
	}
	
	// 绘制X轴箭头
	pDC->MoveTo(315,275);
	pDC->LineTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// 计算最大计数值
	for (i = m_iLowGray; i <= m_iUpGray; i ++)
	{
		// 判断是否大于当前最大值
		if (m_lCount[i] > lMaxCount)
		{
			// 更新最大值
			lMaxCount = m_lCount[i];
		}
	}
	
	// 输出最大计数值
	pDC->MoveTo(10, 25);
	pDC->LineTo(14, 25);
	str.Format("%d", lMaxCount);
	pDC->TextOut(11, 26, str);
	
	// 更改成绿色画笔
	pDC->SelectObject(pPenGreen);	
	
	// 绘制窗口上下限
	pDC->MoveTo(m_iLowGray + 10, 25);
	pDC->LineTo(m_iLowGray + 10, 280);
	
	pDC->MoveTo(m_iUpGray + 10, 25);
	pDC->LineTo(m_iUpGray + 10, 280);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 判断是否有计数
	if (lMaxCount > 0)
	{
		// 绘制直方图
		for (i = m_iLowGray; i <= m_iUpGray; i ++)
		{
			pDC->MoveTo(i + 10, 280);
			pDC->LineTo(i + 10, 281 - (int) (m_lCount[i] * 256 / lMaxCount));
		}
	}
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	
}

void CDlgIntensity::OnOK() 
{
	
	// 判断是否下限超过上限
	if (m_iLowGray > m_iUpGray)
	{
		// 互换
		int iTemp = m_iLowGray;
		m_iLowGray = m_iUpGray;
		m_iUpGray = iTemp;
	}
	
	// 返回
	CDialog::OnOK();
}
