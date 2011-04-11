// DlgPointThre.cpp : implementation file
//

#include "stdafx.h"
#include "ch1_1.h"
#include "DlgPointThre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgPointThre dialog


CDlgPointThre::CDlgPointThre(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPointThre::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgPointThre)
	m_bThre = 0;
	//}}AFX_DATA_INIT
}


void CDlgPointThre::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgPointThre)
	DDX_Text(pDX, IDC_EDIT_Thre, m_bThre);
	DDV_MinMaxByte(pDX, m_bThre, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPointThre, CDialog)
	//{{AFX_MSG_MAP(DlgPointThre)
	ON_EN_KILLFOCUS(IDC_EDIT_Thre, OnKillfocusEDITThre)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPointThre message handlers

BOOL CDlgPointThre::OnInitDialog() 
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
	m_bIsDraging = FALSE;
	
	// 返回TRUE
	return TRUE;
}

void CDlgPointThre::OnKillfocusEDITThre() 
{
	// 更新
	UpdateData(TRUE);
	
	// 重绘
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgPointThre::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 当用户单击鼠标左键开始拖动
	
	// 判断是否在有效区域中
	if(m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_bThre))
		{
			
			// 设置拖动状态
			m_bIsDraging = TRUE;
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// 默认单击鼠标左键处理事件
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgPointThre::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 当用户释放鼠标左键停止拖动
	if (m_bIsDraging)
	{
		// 重置拖动状态
		m_bIsDraging = FALSE;
	}
	
	// 默认释放鼠标左键处理事件
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgPointThre::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 判断当前光标是否在绘制区域
	if(m_MouseRect.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_bIsDraging)
		{
			// 更改阈值
			m_bThre = (BYTE) (point.x - m_MouseRect.left);
			
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			
			// 更新
			UpdateData(FALSE);
			
			// 重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_bThre))
		{
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgPointThre::OnPaint() 
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
	
	// 绘制窗口阈值线
	pDC->MoveTo(m_bThre + 10, 25);
	pDC->LineTo(m_bThre + 10, 280);
	
	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);	
	
	// 绘制坐标值
	str.Format("%d", m_bThre);
	pDC->TextOut(m_bThre + 10, 281, str);
	
	// 绘制用户指定的窗口（注意转换坐标系）
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bThre + 10, 280);
	pDC->LineTo(m_bThre + 10, 25);
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
