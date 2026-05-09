#if !defined(AFX_DLGCONCAVE_H__CE32876B_0CBD_4CEB_880B_CDADF509B516__INCLUDED_)
#define AFX_DLGCONCAVE_H__CE32876B_0CBD_4CEB_880B_CDADF509B516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConcave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConcave dialog

class CDlgConcave : public CDialog
{
// Construction
public:
	CDlgConcave(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];

// Dialog Data
	//{{AFX_DATA(CDlgConcave)
	enum { IDD = IDD_DLG_CONCAVE };
	CSpinButtonCtrl	m_SpinDigit19;
	CSpinButtonCtrl	m_SpinDigit18;
	CSpinButtonCtrl	m_SpinDigit17;
	CSpinButtonCtrl	m_SpinDigit16;
	CSpinButtonCtrl	m_SpinDigit15;
	CSpinButtonCtrl	m_SpinDigit14;
	CSpinButtonCtrl	m_SpinDigit13;
	CSpinButtonCtrl	m_SpinDigit12;
	CSpinButtonCtrl	m_SpinDigit11;
	CSpinButtonCtrl	m_SpinDigit10;
	CEdit	m_EditDigit19;
	CEdit	m_EditDigit18;
	CEdit	m_EditDigit17;
	CEdit	m_EditDigit16;
	CEdit	m_EditDigit15;
	CEdit	m_EditDigit14;
	CEdit	m_EditDigit13;
	CEdit	m_EditDigit12;
	CEdit	m_EditDigit11;
	CEdit	m_EditDigit10;
	CSpinButtonCtrl	m_SpinDigit9;
	CSpinButtonCtrl	m_SpinDigit8;
	CSpinButtonCtrl	m_SpinDigit7;
	CSpinButtonCtrl	m_SpinDigit6;
	CSpinButtonCtrl	m_SpinDigit5;
	CSpinButtonCtrl	m_SpinDigit4;
	CSpinButtonCtrl	m_SpinDigit3;
	CSpinButtonCtrl	m_SpinDigit2;
	CSpinButtonCtrl	m_SpinDigit1;
	CSpinButtonCtrl	m_SpinDigit0;
	CEdit	m_EditDigit9;
	CEdit	m_EditDigit8;
	CEdit	m_EditDigit7;
	CEdit	m_EditDigit6;
	CEdit	m_EditDigit5;
	CEdit	m_EditDigit4;
	CEdit	m_EditDigit3;
	CEdit	m_EditDigit2;
	CEdit	m_EditDigit1;
	CEdit	m_EditDigit0;
	CEdit	m_EditConcaveRemainRibbon;
	CEdit	m_EditConcaveSerial;
	CEdit	m_EditConcaveFwVer;
	CSpinButtonCtrl	m_SpinConcaveSpace;
	CSpinButtonCtrl	m_SpinConcaveDistance;
	DWORD	m_dwConcaveDistance;
	DWORD	m_dwConcaveSpace;
	CString	m_csConcaveDigits;
	short	m_shDigit0;
	short	m_shDigit1;
	short	m_shDigit2;
	short	m_shDigit3;
	short	m_shDigit4;
	short	m_shDigit5;
	short	m_shDigit6;
	short	m_shDigit7;
	short	m_shDigit8;
	short	m_shDigit9;
	short	m_shDigit10;
	short	m_shDigit11;
	short	m_shDigit12;
	short	m_shDigit13;
	short	m_shDigit14;
	short	m_shDigit15;
	short	m_shDigit16;
	short	m_shDigit17;
	short	m_shDigit18;
	short	m_shDigit19;
	CString	m_csIndentFwFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConcave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConcave)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnConcavePrint();
	afx_msg void OnBtnGetConcaveFwVer();
	afx_msg void OnBtnGetConcaveSerial();
	afx_msg void OnBtnGetConcaveRemainRibbon();
	afx_msg void OnBtnGetHammerParam();
	afx_msg void OnBtnSetHammerParam();
	afx_msg void OnBtnGetDiskCompParam();
	afx_msg void OnBtnSetDiskCompParam();
	afx_msg void OnBtnBrowseIndentFwBin();
	afx_msg void OnBtnUpdateIndentFirmware();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONCAVE_H__CE32876B_0CBD_4CEB_880B_CDADF509B516__INCLUDED_)
