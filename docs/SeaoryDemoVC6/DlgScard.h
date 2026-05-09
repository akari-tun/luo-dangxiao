#if !defined(AFX_DLGSCARD_H__38474383_F799_41BE_B643_E5B23B011C8E__INCLUDED_)
#define AFX_DLGSCARD_H__38474383_F799_41BE_B643_E5B23B011C8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgScard.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlgScard dialog

class CDlgScard : public CDialog
{
// Construction
public:
	CDlgScard(CWnd* pParent=NULL);   // standard constructor

	void		SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName){lstrcpy(m_szPrinterName, pPrinterName);lstrcpy(m_szDriverName, pDriverName);};
	void		ShowResult(LPCTSTR szText);
	void		ShowResultA(LPCSTR szText);
	void		ShowResult(int nStringID);
	void		ShowScardResult(LPCTSTR szFuncName, int nRet, BYTE* lpData, int nLen);
	void		ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

	bool		OpenScardReaderPort();
	void		CloseScardReaderPort();

	void		DoScardPositionCalibration();
	bool		DetectChipCard();


	HANDLE					m_hDev;

	TCHAR		m_szPrinterName[128];
	TCHAR		m_szDriverName[128];

// Dialog Data
	//{{AFX_DATA(CDlgScard)
	enum { IDD = IDD_DLG_SCARD };
	CEdit	m_EditResult;
	int		m_nPort;
	int		m_nBaudRate;
	int		m_nScardType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScard)
	afx_msg void OnBtnCpuTypeA();
	afx_msg void OnBtnCpuTypeB();
	afx_msg void OnBtnMifareDesfire();
	afx_msg void OnBtnMifarePlus();
	afx_msg void OnBtnUltraLight();
	afx_msg void OnBtnUltraLightC();
	afx_msg void OnBtn4442();
	afx_msg void OnBtn4428();
	afx_msg void OnBtn24cxx();
	afx_msg void OnBtnCpu();
	afx_msg void OnBtnM1WriteRead();
	afx_msg void OnBtnM1Value();
	afx_msg void OnBtnGetScardReaderVersion();
	afx_msg void OnBtnCalibrateScardPosition();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCARD_H__38474383_F799_41BE_B643_E5B23B011C8E__INCLUDED_)
