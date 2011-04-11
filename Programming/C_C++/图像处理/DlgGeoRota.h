#if !defined(AFX_DLGGEOROTA_H__E1B988B8_88E8_4140_BC0C_EEDB0E8E4125__INCLUDED_)
#define AFX_DLGGEOROTA_H__E1B988B8_88E8_4140_BC0C_EEDB0E8E4125__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGeoRota.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGeoRota dialog

class CDlgGeoRota : public CDialog
{
// Construction
public:
	CDlgGeoRota(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGeoRota)
	enum { IDD = IDD_DLG_GEORota };
	int		m_iRotateAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGeoRota)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGeoRota)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGEOROTA_H__E1B988B8_88E8_4140_BC0C_EEDB0E8E4125__INCLUDED_)
