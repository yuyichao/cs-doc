#if !defined(AFX_DLGGEOZOOM_H__3B544968_7588_433E_AD96_40D6120A6E79__INCLUDED_)
#define AFX_DLGGEOZOOM_H__3B544968_7588_433E_AD96_40D6120A6E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGeoZoom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoZoom dialog

class CDlgGeoZoom : public CDialog
{
// Construction
public:
	CDlgGeoZoom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGeoZoom)
	enum { IDD = IDD_DLG_GEOZoom };
	float	m_XZoom;
	float	m_YZoom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGeoZoom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGeoZoom)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGEOZOOM_H__3B544968_7588_433E_AD96_40D6120A6E79__INCLUDED_)
