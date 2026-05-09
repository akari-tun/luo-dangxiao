#if !defined(AFX_DLGCOMMAND_H__7A25EBCB_01E3_4774_B7CF_72EF24274129__INCLUDED_)
#define AFX_DLGCOMMAND_H__7A25EBCB_01E3_4774_B7CF_72EF24274129__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCommand.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCommand dialog

class CDlgCommand : public CDialog
{
// Construction
public:
	CDlgCommand(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];
	BOOL			m_bModelRxxx;

// Dialog Data
	//{{AFX_DATA(CDlgCommand)
	enum { IDD = IDD_DLG_COMMAND };
	CEdit	m_EditRbnLed;
	CComboBox	m_ComboStandbySide;
	CComboBox	m_ComboPrinterInfo;
	CComboBox	m_ComboCommand;
	int		m_nCommand;
	int		m_nInfoType;
	int		m_nStandbySide;
	int		m_nWaitPos;
	int		m_nWaitTime;
	CString	m_csBmpErase;
	CString	m_csFwFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCommand)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCommand)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGetPrinterInfo();
	afx_msg void OnBtnGetPrinterStauts();
	afx_msg void OnBtnGetPrinterWarning();
	afx_msg void OnBtnSetStandbyParam();
	afx_msg void OnBtnBrowseEraseImage();
	afx_msg void OnBtnErasePartialCard();
	afx_msg void OnBtnExecuteCommand();
	afx_msg void OnBtnBrowseFwFile();
	afx_msg void OnBtnUpdateFirmware();
	afx_msg void OnBtnGetBinFirmwareVersion();
	afx_msg void OnBtnCalibrateRibbonLed();
	afx_msg void OnBtnReadRibbonLedValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMMAND_H__7A25EBCB_01E3_4774_B7CF_72EF24274129__INCLUDED_)
