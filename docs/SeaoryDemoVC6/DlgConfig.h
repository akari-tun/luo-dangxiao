#if !defined(AFX_DLGCONFIG_H__76EB959B_07DD_43C9_8950_911CFD721CF2__INCLUDED_)
#define AFX_DLGCONFIG_H__76EB959B_07DD_43C9_8950_911CFD721CF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig dialog

class CDlgConfig : public CDialog
{
// Construction
public:
	CDlgConfig(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	void			InitialControls();

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];
	BOOL			m_bModelRxxx;

// Dialog Data
	//{{AFX_DATA(CDlgConfig)
	enum { IDD = IDD_DLG_CONFIG };
	CComboBox	m_ComboInitRibbonMode;
	CComboBox	m_ComboCardInSignal;
	CSpinButtonCtrl	m_SpinRfPos;
	CSpinButtonCtrl	m_SpinIcPos;
	CEdit	m_EditRfPos;
	CEdit	m_EditIcPos;
	CSpinButtonCtrl	m_SpinS;
	CSpinButtonCtrl	m_SpinT;
	CSpinButtonCtrl	m_SpinL;
	int		m_nL;
	int		m_nT;
	int		m_nS;
	int		m_nIcPos;
	int		m_nRfPos;
	int		m_nCardInSignal;
	int		m_nInitRibbonMode;
	BOOL	m_bRejectBoxCover;
	BOOL	m_bRejectBoxFull;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPrintPosChart();
	afx_msg void OnBtnGetPrintPos();
	afx_msg void OnBtnSetPrintPos();
	afx_msg void OnBtnGetIcPos();
	afx_msg void OnBtnSetIcPos();
	afx_msg void OnBtnGetRfPos();
	afx_msg void OnBtnSetRfPos();
	afx_msg void OnBtnGetOtherConfigValue();
	afx_msg void OnBtnSetOtherConfigValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIG_H__76EB959B_07DD_43C9_8950_911CFD721CF2__INCLUDED_)
