// ch1_1View.h : interface of the CCh1_1View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CH1_1VIEW_H__60AAD957_ED0B_48FF_834E_78C547411B15__INCLUDED_)
#define AFX_CH1_1VIEW_H__60AAD957_ED0B_48FF_834E_78C547411B15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCh1_1View : public CScrollView
{
protected: // create from serialization only
	CCh1_1View();
	DECLARE_DYNCREATE(CCh1_1View)

// Attributes
public:
	CCh1_1Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCh1_1View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCh1_1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCh1_1View)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDoRealize(WPARAM wParam, LPARAM lParam);  // user message
	afx_msg void OnGeomTran();
	afx_msg void OnGeomMirv();
	afx_msg void OnGeomMirh();
	afx_msg void OnGeomRota();
	afx_msg void OnGeomTrpo();
	afx_msg void OnGeomZoom();
	afx_msg void OnTempSharp();
	afx_msg void OnTempSmooth();
	afx_msg void OnTEMPMidianF();
	afx_msg void OnPointInvert();
	afx_msg void OnPointEqua();
	afx_msg void OnPointLiner();
	afx_msg void OnPointWind();
	afx_msg void OnViewIntensity();
	afx_msg void OnPointStre();
	afx_msg void OnFreqFour();
	afx_msg void OnFreqDct();
	afx_msg void OnPointThre();
	afx_msg void OnEnhaSmooth();
	afx_msg void OnENHAMidianF();
	afx_msg void OnEnhaSharp();
	afx_msg void OnEnhaColor();
	afx_msg void OnFILE256ToGray();
	afx_msg void OnEnhaGradsharp();
	afx_msg void OnFreqWalh();
	afx_msg void OnCodeHuffman();
	afx_msg void OnCodeRLE();
	afx_msg void OnCodeIRLE();
	afx_msg void OnCodeJEPG();
	afx_msg void OnCodeIJEPG();
	afx_msg void OnCodeShannon();
	afx_msg void OnMorphErosion();
	afx_msg void OnMorphDilation();
	afx_msg void OnMorphOpen();
	afx_msg void OnMorphClose();
	afx_msg void OnMorphThining();
	afx_msg void OnEdgeFill();
	afx_msg void OnEdgeGauss();
	afx_msg void OnEdgeHough();
	afx_msg void OnEdgeKirsch();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeRobert();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgeTrace();
	afx_msg void OnDetectHprojection();
	afx_msg void OnDetectMinus();
	afx_msg void OnDetectTemplate();
	afx_msg void OnDetectThreshold();
	afx_msg void OnDetectVprojection();
	afx_msg void OnRestoreBlur();
	afx_msg void OnRestoreInverse();
	afx_msg void OnRestoreNoiseblur();
	afx_msg void OnRestoreRandomnoise();
	afx_msg void OnRestoreSaltnoise();
	afx_msg void OnRestoreWiener();
	afx_msg void OnEdgeContour();
	afx_msg void OnCodeLzw();
	afx_msg void OnCodeIlzw();
	afx_msg void OnEdgeFill2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ch1_1View.cpp
inline CCh1_1Doc* CCh1_1View::GetDocument()
   { return (CCh1_1Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CH1_1VIEW_H__60AAD957_ED0B_48FF_834E_78C547411B15__INCLUDED_)
