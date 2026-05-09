#if !defined(AFX_DLGSECURITY_H__F7873F1D_7FE3_484C_83FF_C0E8A76D0CA6__INCLUDED_)
#define AFX_DLGSECURITY_H__F7873F1D_7FE3_484C_83FF_C0E8A76D0CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSecurity.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSecurity dialog

class CDlgSecurity : public CDialog
{
// Construction
public:
	CDlgSecurity(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];

// Dialog Data
	//{{AFX_DATA(CDlgSecurity)
	enum { IDD = IDD_DLG_SECURITY };
	CComboBox	m_ComboSecurityMode;
	CEdit	m_EditNewPasswd;
	CEdit	m_EditOldPasswd;
	CEdit	m_EditCurPasswd;
	int		m_nSecurityMode;
	CString	m_CurPasswd;
	CString	m_OldPasswd;
	CString	m_NewPasswd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSecurity)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSecurity)
	afx_msg void OnBtnSetSecurityMode();
	afx_msg void OnBtnSetSecurityPassword();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSECURITY_H__F7873F1D_7FE3_484C_83FF_C0E8A76D0CA6__INCLUDED_)
