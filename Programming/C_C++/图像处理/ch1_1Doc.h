// ch1_1Doc.h : interface of the CCh1_1Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CH1_1DOC_H__766DD4EA_BF18_426A_BA52_B747D78F541C__INCLUDED_)
#define AFX_CH1_1DOC_H__766DD4EA_BF18_426A_BA52_B747D78F541C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dibapi.h"
#include "PointTrans.h"
#include "geotrans.h"
#include "TemplateTrans.h"
#include "FreTrans.h"
#include "morph.h"
#include "edgecontour.h"
#include "detect.h"
#include "restore.h"
#include "GIFAPI.h"

class CCh1_1Doc : public CDocument
{
protected: // create from serialization only
	CCh1_1Doc();
	DECLARE_DYNCREATE(CCh1_1Doc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCh1_1Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nColorIndex;
	void ReplaceHDIB(HDIB hDIB);
	void InitDIBData();
	virtual ~CCh1_1Doc();
	COLORREF m_refColorBKG;		// ±³¾°É«
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCh1_1Doc)
	afx_msg void OnFileReopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CH1_1DOC_H__766DD4EA_BF18_426A_BA52_B747D78F541C__INCLUDED_)
