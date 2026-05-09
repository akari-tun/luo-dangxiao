#if !defined(AFX_DLGMAGNETIC_H__6671C1C4_B5A8_47AB_ABED_9D36ED8194CD__INCLUDED_)
#define AFX_DLGMAGNETIC_H__6671C1C4_B5A8_47AB_ABED_9D36ED8194CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMagnetic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMagnetic dialog

class CDlgMagnetic : public CDialog
{
// Construction
public:
	CDlgMagnetic(CWnd* pParent = NULL);   // standard constructor

	void		SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName){lstrcpy(m_szPrinterName, pPrinterName);lstrcpy(m_szDriverName, pDriverName);};

	TCHAR		m_szPrinterName[128];
	TCHAR		m_szDriverName[128];

	
// Dialog Data
	//{{AFX_DATA(CDlgMagnetic)
	enum { IDD = IDD_DLG_MAGNETIC };
	CComboBox	m_ComboCoercitivity;
	int		m_nCoercivity;
	BOOL	m_bT1;
	BOOL	m_bT2;
	BOOL	m_bT3;
	CEdit	m_EditT1W;
	CEdit	m_EditT2W;
	CEdit	m_EditT3W;
	CEdit	m_EditT1R;
	CEdit	m_EditT2R;
	CEdit	m_EditT3R;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMagnetic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMagnetic)
	afx_msg void OnBtnReadTracks();
	afx_msg void OnBtnEncodeTracks();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAGNETIC_H__6671C1C4_B5A8_47AB_ABED_9D36ED8194CD__INCLUDED_)
