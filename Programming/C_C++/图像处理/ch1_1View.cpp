// ch1_1View.cpp : implementation of the CCh1_1View class
//

#include "stdafx.h"
#include "ch1_1.h"

#include "ch1_1Doc.h"
#include "ch1_1View.h"
#include "mainfrm.h"
#include "DlgIntensity.h"
#include "DlgLinerPara.h"
#include "DlgPointThre.h"
#include "DlgPointWin.h"
#include "DlgPointStre.h"
#include "DlgGeoTran.h"
#include "DlgGeoZoom.h"
#include "DlgGeoRota.h"
#include "DlgSmooth.h"
#include "DlgMidFilter.h"
#include "DlgSharpThre.h"
#include "DlgColor.h"
#include "ColorTable.h"

#include "cDlgMorphErosion.h"
#include "cDlgMorphDilation.h"
#include "cDlgMorphOpen.h"
#include "cDlgMorphClose.h"

#include "DlgHuffman.h"
#include "DlgShannon.h"
#include "DlgCodeGIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCh1_1View

IMPLEMENT_DYNCREATE(CCh1_1View, CScrollView)

BEGIN_MESSAGE_MAP(CCh1_1View, CScrollView)
	//{{AFX_MSG_MAP(CCh1_1View)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_GEOM_TRAN, OnGeomTran)
	ON_COMMAND(ID_GEOM_MIRV, OnGeomMirv)
	ON_COMMAND(ID_GEOM_MIRH, OnGeomMirh)
	ON_COMMAND(ID_GEOM_ROTA, OnGeomRota)
	ON_COMMAND(ID_GEOM_TRPO, OnGeomTrpo)
	ON_COMMAND(ID_GEOM_ZOOM, OnGeomZoom)
	ON_COMMAND(ID_POINT_INVERT, OnPointInvert)
	ON_COMMAND(ID_POINT_EQUA, OnPointEqua)
	ON_COMMAND(ID_POINT_LINER, OnPointLiner)
	ON_COMMAND(ID_POINT_WIND, OnPointWind)
	ON_COMMAND(ID_VIEW_INTENSITY, OnViewIntensity)
	ON_COMMAND(ID_POINT_STRE, OnPointStre)
	ON_COMMAND(ID_FREQ_FOUR, OnFreqFour)
	ON_COMMAND(ID_FREQ_DCT, OnFreqDct)
	ON_COMMAND(ID_POINT_THRE, OnPointThre)
	ON_COMMAND(ID_ENHA_SMOOTH, OnEnhaSmooth)
	ON_COMMAND(ID_ENHA_MidianF, OnENHAMidianF)
	ON_COMMAND(ID_ENHA_SHARP, OnEnhaSharp)
	ON_COMMAND(ID_ENHA_COLOR, OnEnhaColor)
	ON_COMMAND(ID_FILE_256ToGray, OnFILE256ToGray)
	ON_COMMAND(ID_ENHA_GRADSHARP, OnEnhaGradsharp)
	ON_COMMAND(ID_FREQ_WALH, OnFreqWalh)
	ON_COMMAND(ID_CODE_HUFFMAN, OnCodeHuffman)
	ON_COMMAND(ID_CODE_RLE, OnCodeRLE)
	ON_COMMAND(ID_CODE_IRLE, OnCodeIRLE)
	ON_COMMAND(ID_CODE_JEPG, OnCodeJEPG)
	ON_COMMAND(ID_CODE_IJEPG, OnCodeIJEPG)
	ON_COMMAND(ID_CODE_SHANNON, OnCodeShannon)
	ON_COMMAND(ID_MORPH_EROSION, OnMorphErosion)
	ON_COMMAND(ID_MORPH_DILATION, OnMorphDilation)
	ON_COMMAND(ID_MORPH_OPEN, OnMorphOpen)
	ON_COMMAND(ID_MORPH_CLOSE, OnMorphClose)
	ON_COMMAND(ID_MORPH_THINING, OnMorphThining)
	ON_COMMAND(ID_EDGE_FILL, OnEdgeFill)
	ON_COMMAND(ID_EDGE_GAUSS, OnEdgeGauss)
	ON_COMMAND(ID_EDGE_HOUGH, OnEdgeHough)
	ON_COMMAND(ID_EDGE_KIRSCH, OnEdgeKirsch)
	ON_COMMAND(ID_EDGE_PREWITT, OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_ROBERT, OnEdgeRobert)
	ON_COMMAND(ID_EDGE_SOBEL, OnEdgeSobel)
	ON_COMMAND(ID_EDGE_TRACE, OnEdgeTrace)
	ON_COMMAND(ID_DETECT_HPROJECTION, OnDetectHprojection)
	ON_COMMAND(ID_DETECT_MINUS, OnDetectMinus)
	ON_COMMAND(ID_DETECT_TEMPLATE, OnDetectTemplate)
	ON_COMMAND(ID_DETECT_THRESHOLD, OnDetectThreshold)
	ON_COMMAND(ID_DETECT_VPROJECTION, OnDetectVprojection)
	ON_COMMAND(ID_RESTORE_BLUR, OnRestoreBlur)
	ON_COMMAND(ID_RESTORE_INVERSE, OnRestoreInverse)
	ON_COMMAND(ID_RESTORE_NOISEBLUR, OnRestoreNoiseblur)
	ON_COMMAND(ID_RESTORE_RANDOMNOISE, OnRestoreRandomnoise)
	ON_COMMAND(ID_RESTORE_SALTNOISE, OnRestoreSaltnoise)
	ON_COMMAND(ID_RESTORE_WIENER, OnRestoreWiener)
	ON_COMMAND(ID_EDGE_CONTOUR, OnEdgeContour)
	ON_COMMAND(ID_CODE_LZW, OnCodeLzw)
	ON_COMMAND(ID_CODE_ILZW, OnCodeIlzw)
	ON_COMMAND(ID_EDGE_FILL2, OnEdgeFill2)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCh1_1View construction/destruction

CCh1_1View::CCh1_1View()
{
	// TODO: add construction code here

}

CCh1_1View::~CCh1_1View()
{
}

BOOL CCh1_1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCh1_1View drawing

void CCh1_1View::OnDraw(CDC* pDC)
{
	
	// 显示等待光标
	BeginWaitCursor();
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 获取DIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		
		// 获取DIB宽度
		int cxDIB = (int) ::DIBWidth(lpDIB);
		
		// 获取DIB高度
		int cyDIB = (int) ::DIBHeight(lpDIB);

		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// 判断是否是打印
		if (pDC->IsPrinting())
		{
			// 是打印，计算输出图像的位置和大小，以便符合页面
			
			// 获取打印页面的水平宽度(象素)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// 获取打印页面的垂直高度(象素)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// 获取打印机每英寸象素数
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// 计算打印图像大小（缩放，根据页面宽度调整图像大小）
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// 计算打印图像位置（垂直居中）
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;

		}
		else   
		// 非打印
		{
			// 不必缩放图像
			rcDest = rcDIB;
		}
		
		// 输出DIB
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	
	// 恢复正常光标
	EndWaitCursor();
	
}

/////////////////////////////////////////////////////////////////////////////
// CCh1_1View printing

BOOL CCh1_1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 设置总页数为一。
	pInfo->SetMaxPage(1);

	return DoPreparePrinting(pInfo);
}

void CCh1_1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCh1_1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCh1_1View diagnostics

#ifdef _DEBUG
void CCh1_1View::AssertValid() const
{
	CView::AssertValid();
}

void CCh1_1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCh1_1Doc* CCh1_1View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCh1_1Doc)));
	return (CCh1_1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCh1_1View message handlers


BOOL CCh1_1View::OnEraseBkgnd(CDC* pDC) 
{
	// 主要是为了设置子窗体默认的背景色
	// 背景色由文档成员变量m_refColorBKG指定

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();

	// 创建一个Brush
	CBrush brush(pDoc->m_refColorBKG);                                              
                                                                                  
	// 保存以前的Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// 获取重绘区域
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// 重绘
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);

	// 恢复以前的Brush
	pDC->SelectObject(pOldBrush);                                                  

	// 返回
	return TRUE;

}

LRESULT CCh1_1View::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 判断DIB是否为空
	if (pDoc->GetHDIB() == NULL)
	{
		// 直接返回
		return 0L;
	}
	
	// 获取Palette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// 获取MainFrame
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);
		
		CClientDC appDC(pAppFrame);

		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		
		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tCCh1_1View::OnPaletteChanged中调用SelectPalette()失败！\n");
		}
	}
	
	return 0L;
}

void CCh1_1View::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

void CCh1_1View::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	CScrollView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);
	
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}

void CCh1_1View::OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView)
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		ASSERT(pActivateView == this);
		OnDoRealize((WPARAM)m_hWnd, 0);   // same as SendMessage(WM_DOREALIZE);
	}
}

void CCh1_1View::OnEditCopy() 
{
	// 复制当前图像

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 打开剪贴板
	if (OpenClipboard())
	{
		// 更改光标形状
		BeginWaitCursor();
		
		// 清空剪贴板
		EmptyClipboard();
		
		// 复制当前图像到剪贴板
		SetClipboardData (CF_DIB, CopyHandle((HANDLE) pDoc->GetHDIB()) );
		
		// 关闭剪贴板
		CloseClipboard();
		
		// 恢复光标
		EndWaitCursor();
	}
}

void CCh1_1View::OnEditPaste() 
{
	// 粘贴图像
	
	// 创建新DIB
	HDIB hNewDIB = NULL;
	
	// 打开剪贴板
	if (OpenClipboard())
	{
		// 更改光标形状
		BeginWaitCursor();

		// 读取剪贴板中的图像
		hNewDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));

		// 关闭剪贴板
		CloseClipboard();
		
		// 判断是否读取成功
		if (hNewDIB != NULL)
		{
			// 获取文档
			CCh1_1Doc* pDoc = GetDocument();

			// 替换DIB，同时释放旧DIB对象
			pDoc->ReplaceHDIB(hNewDIB);

			// 更新DIB大小和调色板
			pDoc->InitDIBData();

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);
			
			// 重新设置滚动视图大小
			SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

			// 实现新的调色板
			OnDoRealize((WPARAM)m_hWnd,0);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		// 恢复光标
		EndWaitCursor();
	}
}

void CCh1_1View::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	// 如果当前DIB对象不空，复制菜单项有效
	pCmdUI->Enable(GetDocument()->GetHDIB() != NULL);
}

void CCh1_1View::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// 如果当前剪贴板中有DIB对象，粘贴菜单项有效
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_DIB));
}

void CCh1_1View::OnViewIntensity() 
{
	// 查看当前图像灰度直方图
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持查看256色位图灰度直方图！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 创建对话框
	CDlgIntensity dlgPara;
	
	// 初始化变量值
	dlgPara.m_lpDIBBits = lpDIBBits;
	dlgPara.m_lWidth = ::DIBWidth(lpDIB);
	dlgPara.m_lHeight = ::DIBHeight(lpDIB);
	dlgPara.m_iLowGray = 0;
	dlgPara.m_iUpGray = 255;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

//////////////////////////////////////////////////////////////////////////////////////
//  图像点运算
//
void CCh1_1View::OnPointInvert() 
{
	// 图像反色
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 线性变换的斜率
	FLOAT fA;
	
	// 线性变换的截距
	FLOAT fB;
	
	// 反色操作的线性变换的方程是-x + 255
	fA = -1.0;
	fB = 255.0;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的反色，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的反色！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用LinerTrans()函数反色
	LinerTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), fA, fB);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnPointLiner() 
{
	// 线性变换
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 创建对话框
	CDlgLinerPara dlgPara;
	
	// 线性变换的斜率
	FLOAT fA;
	
	// 线性变换的截距
	FLOAT fB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的线性变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的线性变换！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 初始化变量值
	dlgPara.m_fA = 2.0;
	dlgPara.m_fB = -128.0;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	fA = dlgPara.m_fA;
	fB = dlgPara.m_fB;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用LinerTrans()函数进行线性变换
	LinerTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), fA, fB);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnPointThre() 
{
	// 阈值变换
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 参数对话框
	CDlgPointThre  dlgPara;
	
	// 阈值
	BYTE	bThre;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的阈值变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的阈值变换！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 初始化变量值
	dlgPara.m_bThre = 128;
	
	// 显示对话框，提示用户设定阈值
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的阈值
	bThre = dlgPara.m_bThre;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用ThresholdTrans()函数进行阈值变换
	ThresholdTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bThre);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnPointWind() 
{
	// 窗口变换
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 创建对话框
	CDlgPointWin  dlgPara;
	
	// 窗口下限
	BYTE	bLow;
	
	// 窗口上限
	BYTE	bUp;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的窗口变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的窗口变换！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 初始化变量值
	dlgPara.m_bLow = 0;
	dlgPara.m_bUp = 255;
	
	// 显示对话框，提示用户设定窗口上下限
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的窗口上下限
	bLow = dlgPara.m_bLow;
	bUp = dlgPara.m_bUp;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用WindowTrans()函数进行窗口变换
	WindowTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bLow, bUp);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnPointStre() 
{
	// 灰度拉伸
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 创建对话框
	CDlgPointStre dlgPara;
	
	// 点1坐标
	BYTE	bX1;
	BYTE	bY1;
	
	// 点2坐标
	BYTE	bX2;
	BYTE	bY2;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的灰度拉伸，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的灰度拉伸！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 初始化变量值
	dlgPara.m_bX1 = 50;
	dlgPara.m_bY1 = 30;
	dlgPara.m_bX2 = 200;
	dlgPara.m_bY2 = 220;
	
	// 显示对话框，提示用户设定拉伸位置
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	bX1 = dlgPara.m_bX1;
	bY1 = dlgPara.m_bY1;
	bX2 = dlgPara.m_bX2;
	bY2 = dlgPara.m_bY2;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用GrayStretch()函数进行灰度拉伸
	GrayStretch(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bX1, bY1, bX2, bY2);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnPointEqua() 
{
	// 灰度均衡
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的直方图均衡，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的直方图均衡！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用InteEqualize()函数进行直方图均衡
	InteEqualize(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB));
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

//////////////////////////////////////////////////////////////////////////////////////
//  图像几何变换
//
void CCh1_1View::OnGeomTran() 
{
	// 平移位图

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平移，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的平移！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	LONG lXOffset;
	LONG lYOffset;
	
	// 创建对话框
	CDlgGeoTran dlgPara;
	
	// 初始化变量值
	dlgPara.m_XOffset = 100;
	dlgPara.m_YOffset = 100;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	lXOffset = dlgPara.m_XOffset;
	lYOffset = dlgPara.m_YOffset;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用TranslationDIB()函数平移DIB
	if (TranslationDIB1(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), lXOffset, lYOffset))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnGeomMirv() 
{
	// 垂直镜像
	

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的垂直镜像，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的垂直镜像！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用MirrorDIB()函数垂直镜像DIB
	if (MirrorDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), FALSE))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();

}

void CCh1_1View::OnGeomMirh() 
{
	// 水平镜像

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的水平镜像，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的水平镜像！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用MirrorDIB()函数水平镜像DIB
	if (MirrorDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), TRUE))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();

	
}

void CCh1_1View::OnGeomTrpo() 
{
	// 图像转置

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的转置，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的转置！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用TransposeDIB()函数转置DIB
	if (TransposeDIB(lpDIB))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();

}

void CCh1_1View::OnGeomZoom() 
{
	// 图像缩放

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的缩放，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的缩放！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 缩放比率
	float fXZoomRatio;
	float fYZoomRatio;
	
	// 创建对话框
	CDlgGeoZoom dlgPara;
	
	// 初始化变量值
	dlgPara.m_XZoom = 0.5;
	dlgPara.m_YZoom = 0.5;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	fXZoomRatio = dlgPara.m_XZoom;
	fYZoomRatio = dlgPara.m_YZoom;
	
	// 删除对话框
	delete dlgPara;	
	
	// 创建新DIB
	HDIB hNewDIB = NULL;
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用ZoomDIB()函数转置DIB
	hNewDIB = (HDIB) ZoomDIB(lpDIB, fXZoomRatio, fYZoomRatio);
	
	// 判断缩放是否成功
	if (hNewDIB != NULL)
	{
		
		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hNewDIB);

		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnGeomRota() 
{
	// 图像旋转

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的旋转，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的旋转！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 缩放比率
	int iRotateAngle;
	
	// 创建对话框
	CDlgGeoRota dlgPara;
	
	// 初始化变量值
	dlgPara.m_iRotateAngle = 90;
	
	// 显示对话框，提示用户设定旋转角度
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	iRotateAngle = dlgPara.m_iRotateAngle;
	
	// 删除对话框
	delete dlgPara;	
	
	
	// 创建新DIB
	HDIB hNewDIB = NULL;
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用RotateDIB()函数旋转DIB
	hNewDIB = (HDIB) RotateDIB(lpDIB, iRotateAngle);
	
	// 判断旋转是否成功
	if (hNewDIB != NULL)
	{
		
		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hNewDIB);

		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

//////////////////////////////////////////////////////////////////////////////////////
//  图像增强
//
void CCh1_1View::OnEnhaSmooth() 
{
	// 图像平滑
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	// 模板元素数组
	FLOAT	aValue[25];
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的平滑！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 创建对话框
	CDlgSmooth dlgPara;
	
	// 给模板数组赋初值（为平均模板）
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;
	
	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = (FLOAT) (1.0 / 9.0);
	dlgPara.m_fpArray = aValue;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的平移量
	iTempH  = dlgPara.m_iTempH;
	iTempW  = dlgPara.m_iTempW;
	iTempMX = dlgPara.m_iTempMX;
	iTempMY = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用Template()函数平滑DIB
	if (::Template(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		  iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnENHAMidianF() 
{
	// 中值滤波
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 滤波器的高度
	int iFilterH;
	
	// 滤波器的宽度
	int iFilterW;
	
	// 中心元素的X坐标
	int iFilterMX;
	
	// 中心元素的Y坐标
	int iFilterMY;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的中值滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 创建对话框
	CDlgMidFilter dlgPara;
	
	// 初始化变量值
	dlgPara.m_iFilterType = 0;
	dlgPara.m_iFilterH = 3;
	dlgPara.m_iFilterW = 1;
	dlgPara.m_iFilterMX = 0;
	dlgPara.m_iFilterMY = 1;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	iFilterH = dlgPara.m_iFilterH;
	iFilterW = dlgPara.m_iFilterW;
	iFilterMX = dlgPara.m_iFilterMX;
	iFilterMY = dlgPara.m_iFilterMY;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用MedianFilter()函数中值滤波
	if (::MedianFilter(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		  iFilterH, iFilterW, iFilterMX, iFilterMY))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnEnhaGradsharp() 
{
	// 梯度锐化
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的梯度锐化，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的梯度锐化！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 阈值
	BYTE	bThre;
	
	// 创建对话框
	CDlgSharpThre dlgPara;
	
	// 初始化变量值
	dlgPara.m_bThre = 10;
	
	// 提示用户输入阈值
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	bThre = dlgPara.m_bThre;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用GradSharp()函数进行梯度板锐化
	if (::GradSharp(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bThre))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnEnhaSharp() 
{
	// 图像锐化
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 模板高度
	int		iTempH;
	
	// 模板宽度
	int		iTempW;
	
	// 模板系数
	FLOAT	fTempC;
	
	// 模板中心元素X坐标
	int		iTempMX;
	
	// 模板中心元素Y坐标
	int		iTempMY;
	
	// 模板元素数组
	FLOAT	aValue[9];
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的锐化，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的锐化！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 设置拉普拉斯模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aValue[0] = -1.0;
	aValue[1] = -1.0;
	aValue[2] = -1.0;
	aValue[3] = -1.0;
	aValue[4] =  9.0;
	aValue[5] = -1.0;
	aValue[6] = -1.0;
	aValue[7] = -1.0;
	aValue[8] = -1.0;
	
	// 调用Template()函数用拉普拉斯模板锐化DIB
	if (::Template(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		  iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnEnhaColor() 
{
	// 伪彩色编码
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 保存用户选择的伪彩色编码表索引
	int		nColor;
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（只处理256色位图的伪彩色变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的伪彩色变换！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 参数对话框
	CDlgColor dlgPara;
	
	// 初始化变量值
	if (pDoc->m_nColorIndex >= 0)
	{
		// 初始选中当前的伪彩色
		dlgPara.m_nColor = pDoc->m_nColorIndex;
	}
	else
	{
		// 初始选中灰度伪彩色编码表
		dlgPara.m_nColor = 0;
	}
	
	// 指向名称数组的指针
	dlgPara.m_lpColorName = (LPSTR) ColorScaleName;
	
	// 伪彩色编码数目
	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
	
	// 名称字符串长度
	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
	
	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户的设定
	nColor = dlgPara.m_nColor;
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 判断伪彩色编码是否改动
	if (pDoc->m_nColorIndex != nColor)
	{
		// 调用ReplaceColorPal()函数变换调色板
		::ReplaceColorPal(lpDIB, (BYTE*) ColorsTable[nColor]);
		
		// 替换当前文档调色板
		pDoc->GetDocPalette()->SetPaletteEntries(0, 256, (LPPALETTEENTRY) ColorsTable[nColor]);
		
		// 更新类成员变量
		pDoc->m_nColorIndex = nColor;
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 实现新的调色板
		OnDoRealize((WPARAM)m_hWnd,0);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnFILE256ToGray() 
{
	// 将256色位图转换成灰度图
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 颜色表中的颜色数目
	WORD wNumColors;
	
	// 获取DIB中颜色表中的颜色数目
	wNumColors = ::DIBNumColors(lpDIB);
	
	// 判断是否是8-bpp位图
	if (wNumColors != 256)
	{
		// 提示用户
		MessageBox("非256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 指向DIB象素的指针
	BYTE *	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像宽度
	LONG	lWidth;
	
	// 图像高度
	LONG	lHeight;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO lpbmc;
	
	// 表明是否是Win3.0 DIB的标记
	BOOL bWinStyleDIB;
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// 灰度映射表
	BYTE bMap[256];
	
	// 判断是否是WIN3.0的DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);
	
	// 计算灰度映射表（保存各个颜色的灰度值），并更新DIB调色板
	for (i = 0; i < 256; i ++)
	{
		if (bWinStyleDIB)
		{
			// 计算该颜色对应的灰度值
			bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +
						     0.587 * lpbmi->bmiColors[i].rgbGreen +
					         0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);
			
			// 更新DIB调色板红色分量
			lpbmi->bmiColors[i].rgbRed = i;
			
			// 更新DIB调色板绿色分量
			lpbmi->bmiColors[i].rgbGreen = i;
			
			// 更新DIB调色板蓝色分量
			lpbmi->bmiColors[i].rgbBlue = i;
			
			// 更新DIB调色板保留位
			lpbmi->bmiColors[i].rgbReserved = 0;
		}
		else
		{
			// 计算该颜色对应的灰度值
			bMap[i] = (BYTE)(0.299 * lpbmc->bmciColors[i].rgbtRed +
						     0.587 * lpbmc->bmciColors[i].rgbtGreen +
					         0.114 * lpbmc->bmciColors[i].rgbtBlue + 0.5);
			
			// 更新DIB调色板红色分量
			lpbmc->bmciColors[i].rgbtRed = i;
			
			// 更新DIB调色板绿色分量
			lpbmc->bmciColors[i].rgbtGreen = i;
			
			// 更新DIB调色板蓝色分量
			lpbmc->bmciColors[i].rgbtBlue = i;
		}
	}

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// 获取图像宽度
	lWidth = ::DIBWidth(lpDIB);
	
	// 获取图像高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 更换每个象素的颜色索引（即按照灰度映射表换成灰度值）
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 变换
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// 替换当前调色板为灰度调色板
	pDoc->GetDocPalette()->SetPaletteEntries(0, 256, (LPPALETTEENTRY) ColorsTable[0]);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 实现新的调色板
	OnDoRealize((WPARAM)m_hWnd,0);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	
}

//////////////////////////////////////////////////////////////////////////////////////
//  图像正交变换
//

void CCh1_1View::OnFreqFour() 
{
	// 图像付立叶变换
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的付立叶变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的付立叶变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用Fourier()函数进行付立叶变换
	if (::Fourier(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnFreqDct() 
{
	// 图像离散余弦变换
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散余弦变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的离散余弦变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用DIBDct()函数进行离散余弦变换
	if (::DIBDct(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnFreqWalh() 
{
	// 图像沃尔什-哈达玛变换
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的沃尔什-哈达玛变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的沃尔什-哈达玛变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用DIBWalsh()或者DIBWalsh1()函数进行变换
	if (::DIBWalsh1(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
	
}

//////////////////////////////////////////////////////////////////////////////////////
//  形态学变换
//
void CCh1_1View::OnMorphErosion() 
{
	//腐蚀运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的腐蚀，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的腐蚀！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	int nMode;
	
	// 创建对话框
	cDlgMorphErosion dlgPara;
	
	// 初始化变量值
	dlgPara.m_nMode = 0;
	
	// 显示对话框，提示用户设定腐蚀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的腐蚀方向
	nMode = dlgPara.m_nMode;

	int structure[3][3];
	if (nMode == 2)
	{
		structure[0][0]=dlgPara.m_nStructure1;
		structure[0][1]=dlgPara.m_nStructure2;
		structure[0][2]=dlgPara.m_nStructure3;
		structure[1][0]=dlgPara.m_nStructure4;
		structure[1][1]=dlgPara.m_nStructure5;
		structure[1][2]=dlgPara.m_nStructure6;
		structure[2][0]=dlgPara.m_nStructure7;
		structure[2][1]=dlgPara.m_nStructure8;
		structure[2][2]=dlgPara.m_nStructure9;
	}
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用ErosionDIB()函数腐蚀DIB
	if (ErosionDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB), nMode , structure))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnMorphOpen() 
{
	//开运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的开运算，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的开运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	int nMode;
	
	// 创建对话框
	cDlgMorphOpen dlgPara;
	
	// 初始化变量值
	dlgPara.m_nMode = 0;
	
	// 显示对话框，提示用户设定开运算方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的开运算方向
	nMode = dlgPara.m_nMode;

	int structure[3][3];
	if (nMode == 2)
	{
		structure[0][0]=dlgPara.m_nStructure1;
		structure[0][1]=dlgPara.m_nStructure2;
		structure[0][2]=dlgPara.m_nStructure3;
		structure[1][0]=dlgPara.m_nStructure4;
		structure[1][1]=dlgPara.m_nStructure5;
		structure[1][2]=dlgPara.m_nStructure6;
		structure[2][0]=dlgPara.m_nStructure7;
		structure[2][1]=dlgPara.m_nStructure8;
		structure[2][2]=dlgPara.m_nStructure9;
	}
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用ErosionDIB()函数开运算DIB
	//if (OpenDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB), nMode , structure))
	if (OpenDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), nMode , structure))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
	
}

void CCh1_1View::OnMorphThining() 
{
	//闭运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的闭运算，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的细化运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用ThiningDIB()函数对DIB进行闭运算
	if (ThiningDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnMorphClose() 
{
	//闭运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的闭运算，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的闭运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	int nMode;
	
	// 创建对话框
	cDlgMorphClose dlgPara;
	
	// 初始化变量值
	dlgPara.m_nMode = 0;
	
	// 显示对话框，提示用户设定闭运算方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的闭运算方向
	nMode = dlgPara.m_nMode;

	int structure[3][3];
	if (nMode == 2)
	{
		structure[0][0]=dlgPara.m_nStructure1;
		structure[0][1]=dlgPara.m_nStructure2;
		structure[0][2]=dlgPara.m_nStructure3;
		structure[1][0]=dlgPara.m_nStructure4;
		structure[1][1]=dlgPara.m_nStructure5;
		structure[1][2]=dlgPara.m_nStructure6;
		structure[2][0]=dlgPara.m_nStructure7;
		structure[2][1]=dlgPara.m_nStructure8;
		structure[2][2]=dlgPara.m_nStructure9;
	}
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用CloseDIB()函数对DIB进行闭运算
	if (CloseDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB), nMode , structure))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
}

void CCh1_1View::OnMorphDilation() 
{
	//膨胀运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的膨胀，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的膨胀！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	int nMode;
	
	// 创建对话框
	cDlgMorphDilation dlgPara;
	
	// 初始化变量值
	dlgPara.m_nMode = 0;
	
	// 显示对话框，提示用户设定膨胀方向
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户设定的膨胀方向
	nMode = dlgPara.m_nMode;

	int structure[3][3];
	if (nMode == 2)
	{
		structure[0][0]=dlgPara.m_nStructure1;
		structure[0][1]=dlgPara.m_nStructure2;
		structure[0][2]=dlgPara.m_nStructure3;
		structure[1][0]=dlgPara.m_nStructure4;
		structure[1][1]=dlgPara.m_nStructure5;
		structure[1][2]=dlgPara.m_nStructure6;
		structure[2][0]=dlgPara.m_nStructure7;
		structure[2][1]=dlgPara.m_nStructure8;
		structure[2][2]=dlgPara.m_nStructure9;
	}
	
	// 删除对话框
	delete dlgPara;	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用DilationDIB()函数膨胀DIB
	if (DilationDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB), nMode , structure))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

//////////////////////////////////////////////////////////////////////////////////////
//  边缘与轮廓
//
void CCh1_1View::OnEdgeHough() 
{
	//Hough运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的Hough变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用HoughDIB()函数对DIB
	if (HoughDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或者图像中含有0和255之外的像素值！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnEdgeGauss() 
{
	//Gauss边缘检测运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用GaussDIB()函数对DIB进行边缘检测
	if (GaussDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnEdgeKirsch() 
{
	//Kirsch边缘检测运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用KirschDIB()函数对DIB进行边缘检测
	if (KirschDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnEdgePrewitt() 
{
	//Prewitt边缘检测运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用PrewittDIB()函数对DIB进行边缘检测
	if (PrewittDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnEdgeRobert() 
{
	//Robert边缘检测运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用RobertDIB()函数对DIB进行边缘检测
	if (RobertDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
}

void CCh1_1View::OnEdgeSobel() 
{
	//Sobel边缘检测运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用SobelDIB()函数对DIB进行边缘检测
	if (SobelDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
	
}

void CCh1_1View::OnEdgeFill() 
{
	//种子填充运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的种子填充，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用FillDIB()函数对DIB进行种子填充
	if (FillDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
}

void CCh1_1View::OnEdgeContour() 
{
	//轮廓提取运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的轮廓提取，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用FillDIB()函数对DIB进行轮廓提取
	if (ContourDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnEdgeTrace() 
{
	//轮廓跟踪运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的轮廓跟踪，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用TraceDIB()函数对DIB进行轮廓跟踪
	if (TraceDIB(lpDIBBits, ::DIBWidth(lpDIB) , ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
}

//////////////////////////////////////////////////////////////////////////////////////
//  图像分析
//
void CCh1_1View::OnDetectMinus() 
{
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB,lpDIBBK;

	// 指向DIB象素指针
	LPSTR    lpDIBBits,lpDIBBitsBK;
	
	//图像的宽度与高度
	long lWidth,lHeight;

	HDIB hDIBBK;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的水平镜像，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的平移！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	lWidth = ::DIBWidth(lpDIB);
	lHeight = ::DIBHeight(lpDIB);
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	if(dlg.DoModal() == IDOK)
	{
 
	 	CFile file;
	 	CFileException fe;
 
	 	CString strPathName;
 
		strPathName = dlg.GetPathName();
 
		// 打开文件
		VERIFY(file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe));

		// 尝试调用ReadDIBFile()读取图像
		TRY
		{
 			hDIBBK = ::ReadDIBFile(file);
		}
		CATCH (CFileException, eLoad)
		{
			// 读取失败
	 		file.Abort();
 		
			// 恢复光标形状
			EndWaitCursor();
 		
			// 报告失败
			//ReportSaveLoadException(strPathName, eLoad,
//				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			
			// 设置DIB为空
			hDIBBK = NULL;
			
			// 返回
			return;
		}
		END_CATCH
 	
		// 初始化DIB
		//InitDIBData();
 	
		// 判断读取文件是否成功
		if (hDIBBK == NULL)
		{
 			// 失败，可能非BMP格式
 			CString strMsg;
 			strMsg = "读取图像时出错！可能是不支持该类型的图像文件！";
 		
 			// 提示出错
 			MessageBox(strMsg, NULL, MB_ICONINFORMATION | MB_OK);
  		
 			// 恢复光标形状
 			EndWaitCursor();
 		
 			// 返回
 			return;
 		}
 	}
	else
	{
 		// 恢复光标形状
 		EndWaitCursor();
 		
		return;
	}
	// 锁定DIB
	lpDIBBK = (LPSTR) ::GlobalLock((HGLOBAL) hDIBBK);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的差影，其它的可以类推）
	if (::DIBNumColors(lpDIBBK) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) hDIBBK);
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBitsBK = ::FindDIBBits(lpDIBBK);
	
	// 调用AddMinusDIB()函数相减两幅DIB
	if (AddMinusDIB(lpDIBBits,lpDIBBitsBK, lWidth,lHeight,false))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	::GlobalUnlock((HGLOBAL) hDIBBK);

	// 恢复光标
	EndWaitCursor();	
}

void CCh1_1View::OnDetectHprojection() 
{
	//水平投影

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的投影，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用HprojectDIB()函数对DIB进行水平投影
	if (HprojectDIB(lpDIBBits,::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
}

void CCh1_1View::OnDetectVprojection() 
{
	//垂直投影

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的投影，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用VprojectDIB()函数对DIB进行垂直投影
	if (VprojectDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
			
}

void CCh1_1View::OnDetectTemplate() 
{
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR lpDIB,lpTemplateDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits,lpTemplateDIBBits;
	
	//图像的宽度与高度
	long lWidth,lHeight;

	//模板的宽度与高度
	long lTemplateWidth,lTemplateHeight;

	HDIB hTemplateDIB;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的水平镜像，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的平移！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	lWidth = ::DIBWidth(lpDIB);
	lHeight = ::DIBHeight(lpDIB);
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	if(dlg.DoModal() == IDOK)
	{
 
	 	CFile file;
	 	CFileException fe;
 
	 	CString strPathName;
 
		strPathName = dlg.GetPathName();
 
		// 打开文件
		VERIFY(file.Open(strPathName, CFile::modeRead | CFile::shareDenyWrite, &fe));

		// 尝试调用ReadDIBFile()读取图像
		TRY
		{
 			hTemplateDIB = ::ReadDIBFile(file);
		}
		CATCH (CFileException, eLoad)
		{
			// 读取失败
	 		file.Abort();
 		
			// 恢复光标形状
			EndWaitCursor();
 		
			// 报告失败
			//ReportSaveLoadException(strPathName, eLoad,
//				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
			
			// 设置DIB为空
			hTemplateDIB = NULL;
			
			// 返回
			return;
		}
		END_CATCH
 	
		// 初始化DIB
		//InitDIBData();
 	
		// 判断读取文件是否成功
		if (hTemplateDIB == NULL)
		{
 			// 失败，可能非BMP格式
 			CString strMsg;
 			strMsg = "读取图像时出错！可能是不支持该类型的图像文件！";
 		
 			// 提示出错
 			MessageBox(strMsg, NULL, MB_ICONINFORMATION | MB_OK);
  		
 			// 恢复光标形状
 			EndWaitCursor();
 		
 			// 返回
 			return;
 		}
 	}
	else
	{
 		// 恢复光标形状
 		EndWaitCursor();

		return;
	}
	// 锁定DIB
	lpTemplateDIB = (LPSTR) ::GlobalLock((HGLOBAL) hTemplateDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模板匹配，其它的可以类推）
	if (::DIBNumColors(lpTemplateDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) hTemplateDIB);
		
		// 返回
		return;
	}
	
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpTemplateDIBBits = ::FindDIBBits(lpTemplateDIB);
	
	lTemplateWidth = ::DIBWidth(lpTemplateDIB);
	lTemplateHeight = ::DIBHeight(lpTemplateDIB);
	if(lTemplateHeight > lHeight || lTemplateWidth > lWidth )
	{
		// 提示用户
		MessageBox("模板尺寸大于源图像尺寸！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) hTemplateDIB);
		
		// 返回
		return;

	}
	// 调用TemplateMatchDIB()函数进行模板匹配
	if (TemplateMatchDIB(lpDIBBits,lpTemplateDIBBits, lWidth,lHeight, lTemplateWidth,lTemplateHeight))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	::GlobalUnlock((HGLOBAL) hTemplateDIB);

	// 恢复光标
	EndWaitCursor();	
	
}


void CCh1_1View::OnDetectThreshold() 
{
	//阈值分割

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的阈值分割，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用ThresholdDIB()函数对DIB进行阈值分割
	if (ThresholdDIB(lpDIBBits,::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

//////////////////////////////////////////////////////////////////////////////////////
//  图像复原
//
void CCh1_1View::OnRestoreBlur() 
{
	//图像模糊操作，生成一幅待复原的图像

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用Blur1DIB()函数对DIB进行模糊处理
	if (BlurDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或图像尺寸不符合要求！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
}

void CCh1_1View::OnRestoreInverse() 
{
	//图像逆滤波复原操作

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的复原操作，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用RestoreDIB()函数对DIB进行复原
	if (RestoreDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或图像尺寸不符合要求！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
}

void CCh1_1View::OnRestoreNoiseblur() 
{
	//图像模糊操作，生成一幅待复原的图像

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用NoiseBlurDIB()函数对DIB进行模糊加噪处理
	if (NoiseBlurDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或图像尺寸不符合要求！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnRestoreWiener() 
{
	//图像维纳滤波复原操作

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的复原操作，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用WienerDIB()函数对DIB进行复原
	if (WienerDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败或图像尺寸不符合要求！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
			
}

void CCh1_1View::OnRestoreRandomnoise() 
{
	//图像加噪操作，在图像中加入随机噪声

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用RandomNoiseDIB()函数对DIB进行加噪处理
	if (RandomNoiseDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	
	
}

void CCh1_1View::OnRestoreSaltnoise() 
{
	//图像加噪操作，在图像中加入椒盐噪声

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用SaltNoiseDIB()函数对DIB进行加噪处理
	if (SaltNoiseDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();	
	
}


//////////////////////////////////////////////////////////////////////////////////////
//  图像编码
//
void CCh1_1View::OnCodeHuffman() 
{
	// 查看哈夫曼编码表
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向源图像象素的指针
	unsigned char *	lpSrc;
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// DIB的高度
	LONG	lHeight;
	
	// DIB的宽度
	LONG	lWidth;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 图像象素总数
	LONG	lCountSum;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 保存各个灰度值频率的数组指针
	FLOAT * fFreq;
	
	// 获取当前DIB颜色数目
	int		iColorNum;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 获取当前DIB颜色数目
	iColorNum = ::DIBNumColors(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图，其它的可以类推）
	if (iColorNum != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图哈夫曼编码！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	/******************************************************************************
	// 开始计算各个灰度级出现的频率	
	//
	// 如果需要对其它序列进行哈夫曼编码，只需改动此处即可，例如，直接赋值：
	   iColorNum = 8;
	   fFreq = new FLOAT[iColorNum];
	   fFreq[0] = 0.04;
	   fFreq[1] = 0.05;
	   fFreq[2] = 0.06;
	   fFreq[3] = 0.07;
	   fFreq[4] = 0.10;
	   fFreq[5] = 0.10;
	   fFreq[6] = 0.18;
	   fFreq[7] = 0.40;
	// 这样就可以对指定的序列进行哈夫曼编码
	******************************************************************************/
	
	// 分配内存
	fFreq = new FLOAT[iColorNum];
	
	// 计算DIB宽度
	lWidth = ::DIBWidth(lpDIB);
	
	// 计算DIB高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 重置计数为0
	for (i = 0; i < iColorNum; i ++)
	{
		// 清零
		fFreq[i] = 0.0;
	}
	
	// 计算各个灰度值的计数（对于非256色位图，此处给数组fFreq赋值方法将不同）
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// 指向图像指针
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			fFreq[*(lpSrc)] += 1;
		}
	}
	
	// 计算图像象素总数
	lCountSum = lHeight * lWidth;
	
	// 计算各个灰度值出现的概率
	for (i = 0; i < iColorNum; i ++)
	{
		// 计算概率
		fFreq[i] /= (FLOAT)lCountSum;
	}
	
	// 计算各个灰度级出现的频率结束
	/*****************************************************************************/
	
	// 创建对话框
	CDlgHuffman dlgPara;
	
	// 初始化变量值
	dlgPara.m_fFreq = fFreq;
	dlgPara.m_iColorNum = iColorNum;
	
	// 显示对话框
	dlgPara.DoModal();
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnCodeShannon() 
{
	// 查看香农－弗诺编码表
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向源图像象素的指针
	unsigned char *	lpSrc;
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// DIB的高度
	LONG	lHeight;
	
	// DIB的宽度
	LONG	lWidth;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 图像象素总数
	LONG	lCountSum;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 保存各个灰度值频率的数组指针
	FLOAT * fFreq;
	
	// 获取当前DIB颜色数目
	int		iColorNum;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 获取当前DIB颜色数目
	iColorNum = ::DIBNumColors(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图，其它的可以类推）
	if (iColorNum != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图香农－弗诺编码！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	/******************************************************************************
	// 开始计算各个灰度级出现的频率	
	//
	// 如果需要对其它序列进行香农－弗诺编码，只需改动此处即可，例如，直接赋值：
	   iColorNum = 8;
	   fFreq = new FLOAT[iColorNum];
	   fFreq[0] = 0.0625;
	   fFreq[1] = 0.0625;
	   fFreq[2] = 0.0625;
	   fFreq[3] = 0.0625;
	   fFreq[4] = 0.125;
	   fFreq[5] = 0.125;
	   fFreq[6] = 0.25;
	   fFreq[7] = 0.25;
	// 这样就可以对指定的序列进行香农－弗诺编码
	******************************************************************************/
	
	// 分配内存
	fFreq = new FLOAT[iColorNum];
	
	// 计算DIB宽度
	lWidth = ::DIBWidth(lpDIB);
	
	// 计算DIB高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 重置计数为0
	for (i = 0; i < iColorNum; i ++)
	{
		// 清零
		fFreq[i] = 0.0;
	}
	
	// 计算各个灰度值的计数（对于非256色位图，此处给数组fFreq赋值方法将不同）
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// 指向图像指针
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			fFreq[*(lpSrc)] += 1;
		}
	}
	
	// 计算图像象素总数
	lCountSum = lHeight * lWidth;
	
	// 计算各个灰度值出现的概率
	for (i = 0; i < iColorNum; i ++)
	{
		// 计算概率
		fFreq[i] /= (FLOAT)lCountSum;
	}
	
	// 计算各个灰度级出现的频率结束
	/*****************************************************************************/
	
	// 创建对话框
	CDlgShannon dlgPara;
	
	// 初始化变量值
	dlgPara.m_fFreq = fFreq;
	dlgPara.m_iColorNum = iColorNum;
	
	// 显示对话框
	dlgPara.DoModal();
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
	
}

void CCh1_1View::OnCodeRLE() 
{
	// 对当前图像进行行程编码（存为PCX格式文件）
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的行程编码）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的行程编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 文件保存路径
	CString strFilePath;
	
	// 初始化文件名为原始文件名
	strFilePath = pDoc->GetPathName();
	
	// 更改后缀为PCX
	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
	{
		// 更改后缀为PCX
		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "PCX";
	}
	else
	{
		// 直接添加后缀PCX
		strFilePath += ".PCX";
	}
	
	// 创建SaveAs对话框
	CFileDialog dlg(FALSE, "PCX", strFilePath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"PCX图像文件 (*.PCX) | *.PCX|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 恢复光标
		EndWaitCursor();
		
		// 返回
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.GetPathName();
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试创建指定的PCX文件
	if (!file.Open(strFilePath, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// 提示用户
		MessageBox("打开指定PCX文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 调用DIBToPCX256()函数将当前的DIB保存为256色PCX文件
	if (::DIBToPCX256(lpDIB, file))
	{
		// 提示用户
		MessageBox("成功保存为PCX文件！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		// 提示用户
		MessageBox("保存为PCX文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnCodeIRLE() 
{
	// 加载256色PCX文件
	
	// 文件路径
	CString strFilePath;
	
	// 创建Open对话框
	CFileDialog dlg(TRUE, "PCX", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"PCX图像文件 (*.PCX) | *.PCX|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.GetPathName();
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试打开指定的PCX文件
	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 提示用户
		MessageBox("打开指定PCX文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 调用ReadPCX256()函数读取指定的256色PCX文件
	HDIB hDIB = ::ReadPCX256(file);
	
	if (hDIB != NULL)
	{
		// 提示用户
		//MessageBox("成功读取PCX文件！", "系统提示" , 
		//	MB_ICONINFORMATION | MB_OK);
		
		// 获取文档
		CCh1_1Doc* pDoc = GetDocument();
		
		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hDIB);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// 实现新的调色板
		OnDoRealize((WPARAM)m_hWnd,0);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("读取PCX文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	
}

void CCh1_1View::OnCodeLzw() 
{
	// 对当前图像进行GIF-LZW编码（存为GIF格式文件）
	
	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否超过256色
	if (::DIBNumColors(lpDIB) > 256)
	{
		// 提示用户
		MessageBox("目前只支持< 256色位图的GIF-LZW编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 文件保存路径
	CString strFilePath;
	
	// 指定是否以交错方式保存GIF文件
	BOOL	bInterlace;
	
	// 初始化文件名为原始文件名
	strFilePath = pDoc->GetPathName();
	
	// 更改后缀为GIF
	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
	{
		// 更改后缀为GIF
		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "GIF";
	}
	else
	{
		// 直接添加后缀GIF
		strFilePath += ".GIF";
	}
	
	// 创建SaveAs对话框
	CDlgCodeGIF dlg;
	
	dlg.m_strFilePath = strFilePath;
	dlg.m_bInterlace = FALSE;
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 恢复光标
		EndWaitCursor();
		
		// 返回
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.m_strFilePath;
	bInterlace = dlg.m_bInterlace;
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试创建指定的GIF文件
	if (!file.Open(strFilePath, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// 提示用户
		MessageBox("打开指定GIF文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 调用DIBToGIF()函数将当前的DIB保存为GIF文件
	if (::DIBToGIF(lpDIB, file, bInterlace))
	{
		// 提示用户
		MessageBox("成功保存为GIF文件！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		// 提示用户
		MessageBox("保存为GIF文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
}

void CCh1_1View::OnCodeIlzw() 
{
	// 加载GIF文件
	
	// 文件路径
	CString strFilePath;
	
	// 创建Open对话框
	CFileDialog dlg(TRUE, "GIF", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"GIF图像文件 (*.GIF) | *.GIF|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.GetPathName();
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试打开指定的GIF文件
	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 提示用户
		MessageBox("打开指定GIF文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 调用ReadGIF()函数读取指定的GIF文件
	HDIB hDIB = ::ReadGIF(file);
	
	if (hDIB != NULL)
	{
		// 提示用户
		//MessageBox("成功读取GIF文件！", "系统提示" , 
		//	MB_ICONINFORMATION | MB_OK);
		
		// 获取文档
		CCh1_1Doc* pDoc = GetDocument();
		
		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hDIB);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 重新设置滚动视图大小
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
		
		// 实现新的调色板
		OnDoRealize((WPARAM)m_hWnd,0);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("读取GIF文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	
}

void CCh1_1View::OnCodeJEPG() 
{
	// TODO: Add your command handler code here
	
}

void CCh1_1View::OnCodeIJEPG() 
{
	// TODO: Add your command handler code here
	
}


void CCh1_1View::OnEdgeFill2() 
{
	//种子填充运算

	// 获取文档
	CCh1_1Doc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的种子填充，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);

		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 调用FillDIB()函数对DIB进行种子填充
	if (Fill2DIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// 恢复光标
	EndWaitCursor();
		
}
