#if !defined(AFX_DLGCARDINOUT_H__5C1F73F4_3B5E_4471_B0BE_60D75F626BB1__INCLUDED_)
#define AFX_DLGCARDINOUT_H__5C1F73F4_3B5E_4471_B0BE_60D75F626BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCardInout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCardInout dialog

class CDlgCardInout : public CDialog
{
// Construction
public:
	CDlgCardInout(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	void			InitialControls();

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];
	BOOL			m_bModelRxxx;

// Dialog Data
	//{{AFX_DATA(CDlgCardInout)
	enum { IDD = IDD_DLG_CARD_INOUT };
	CComboBox	m_ComboEjectClean;
	CComboBox	m_ComboInputClean;
	CComboBox	m_ComboEjectCard5;
	CComboBox	m_ComboRejectBin;
	CComboBox	m_ComboInputBin;
	CComboBox	m_ComboEjectCard4;
	CComboBox	m_ComboEjectCard3;
	CComboBox	m_ComboEjectCard2;
	CComboBox	m_ComboEjectCard1;
	BOOL	m_bCardOutSensor;
	BOOL	m_bHookMode;
	BOOL	m_bRetryByHook;
	int		m_nOutputBin1;
	int		m_nOutputBin2;
	int		m_nOutputBin3;
	int		m_nOutputBin4;
	BOOL	m_bAutoFeedM3;
	BOOL	m_bAutoFeedM2;
	BOOL	m_bAutoFeedM1;
	int		m_nInputBin;
	int		m_nRejectBin;
	BOOL	m_bWaitRemoval1;
	BOOL	m_bWaitRemoval2;
	BOOL	m_bWaitRemoval3;
	int		m_nWaitPos1;
	int		m_nWaitPos2;
	int		m_nWaitPos3;
	int		m_nWaitTime1;
	int		m_nWaitTime2;
	int		m_nWaitTime3;
	BOOL	m_bAutoEject;
	int		m_nOutputBin5;
	int		m_nInputClean;
	int		m_nOutputClean;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCardInout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCardInout)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGetCurConfig();
	afx_msg void OnBtnSetNewConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCARDINOUT_H__5C1F73F4_3B5E_4471_B0BE_60D75F626BB1__INCLUDED_)
