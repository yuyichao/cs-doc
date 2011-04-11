// DlgPointWin.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgPointWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPointWin dialog


CDlgPointWin::CDlgPointWin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPointWin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPointWin)
	m_bLow = 0;
	m_bUp = 0;
	//}}AFX_DATA_INIT
}


void CDlgPointWin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPointWin)
	DDX_Text(pDX, IDC_EDIT_Low, m_bLow);
	DDV_MinMaxByte(pDX, m_bLow, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Up, m_bUp);
	DDV_MinMaxByte(pDX, m_bUp, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPointWin, CDialog)
	//{{AFX_MSG_MAP(CDlgPointWin)
	ON_EN_KILLFOCUS(IDC_EDIT_Low, OnKillfocusEDITLow)
	ON_EN_KILLFOCUS(IDC_EDIT_Up, OnKillfocusEDITUp)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPointWin message handlers

BOOL CDlgPointWin::OnInitDialog() 
{
	
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
	
	// 初始化拖动状态
	m_iIsDraging = 0;
	
	// 返回TRUE
	return TRUE;
}

void CDlgPointWin::OnKillfocusEDITLow() 
{
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_bLow > m_bUp)
	{
		// 互换
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgPointWin::OnKillfocusEDITUp() 
{
	// 更新
	UpdateData(TRUE);
	
	// 判断是否下限超过上限
	if (m_bLow > m_bUp)
	{
		// 互换
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;
		
		// 更新
		UpdateData(FALSE);
	}
	
	// 重绘
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgPointWin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 当用户单击鼠标左键开始拖动
	if(m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_bLow))
		{
			
			// 设置拖动状态1，拖动下限
			m_iIsDraging = 1;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_bUp))
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

void CDlgPointWin::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 判断当前光标是否在绘制区域
	if(m_MouseRect.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_iIsDraging != 0)
		{
			// 判断正在拖动上限还是下限
			if (m_iIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_MouseRect.left < m_bUp)
				{
					// 更改下限
					m_bLow = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_bLow = m_bUp - 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_bUp - 1;
				}
			}
			else
			{
				// 正在拖动上限
				
				// 判断是否上限>下限
				if (point.x - m_MouseRect.left > m_bLow)
				{
					// 更改下限
					m_bUp = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_bUp = m_bLow + 1;
					
					// 重设鼠标位置
					point.x = m_MouseRect.left + m_bLow + 1;
				}
			}
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_bLow) || point.x == (m_MouseRect.left + m_bUp))
		{
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgPointWin::OnLButtonUp(UINT nFlags, CPoint point) 
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

void CDlgPointWin::OnPaint() 
{
	// 字符串
	CString str;
	
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
	pPenRed->CreatePen(PS_SOLID,2,RGB(255,0,0));
	
	// 创建画笔对象
	CPen* pPenBlue = new CPen;
	
	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID,2,RGB(0,0, 255));
	
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
	
	// 写坐标
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// 绘制X轴箭头
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// 绘制X轴箭头
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// 更改成绿色画笔
	pDC->SelectObject(pPenGreen);	
	
	// 绘制窗口上下限
	pDC->MoveTo(m_bLow + 10, 25);
	pDC->LineTo(m_bLow + 10, 280);
	
	pDC->MoveTo(m_bUp + 10, 25);
	pDC->LineTo(m_bUp + 10, 280);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 绘制坐标值
	str.Format("(%d, %d)", m_bLow, m_bLow);
	pDC->TextOut(m_bLow + 10, 281 - m_bLow, str);
	str.Format("(%d, %d)", m_bUp, m_bUp);
	pDC->TextOut(m_bUp + 10, 281 - m_bUp, str);
	
	// 绘制用户指定的窗口（注意转换坐标系）
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bLow + 10, 280);
	pDC->LineTo(m_bLow + 10, 280 - m_bLow);
	pDC->LineTo(m_bUp + 10, 280 - m_bUp);
	pDC->LineTo(m_bUp + 10, 25);
	pDC->LineTo(265, 25);
	
	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);	
	
	// 绘制边缘
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	
}

void CDlgPointWin::OnOK() 
{
	
	// 判断是否下限超过上限
	if (m_bLow > m_bUp)
	{
		// 互换
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;
	}
	
	// 默认处理事件
	CDialog::OnOK();
}

