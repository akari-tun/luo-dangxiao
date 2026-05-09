#if !defined(AFX_DLGPRINT_H__A21340A4_D704_459A_A203_B63C682B83AD__INCLUDED_)
#define AFX_DLGPRINT_H__A21340A4_D704_459A_A203_B63C682B83AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPrint.h : header file
//

#include "SeaoryPrinter.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint dialog

class CDlgPrint : public CDialog
{
// Construction
public:
	CDlgPrint(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	DWORD			WaitPrintingFinished(TCHAR *szPrinterName);

	void			FillDocPropertyFromUI(SEAORY_DOC_PROP* lpDocProp);

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];
	BOOL			m_bModelRxxx;

// Dialog Data
	//{{AFX_DATA(CDlgPrint)
	enum { IDD = IDD_DLG_PRINT };
	CComboBox	m_ComboResolution;
	CComboBox	m_ComboPaperSize;
	CComboBox	m_ComboAreaFrontRewrite;
	CComboBox	m_ComboAreaBackK;
	CComboBox	m_ComboAreaBackYMCO;
	CComboBox	m_ComboAreaFrontK;
	CComboBox	m_ComboAreaFrontYMCO;
	CComboBox	m_ComboBackPanel;
	CComboBox	m_ComboFrontPanel;
	CComboBox	m_ComboRibbon;
	CComboBox	m_ComboFeedCard;
	CComboBox	m_ComboEjectCard;
	CComboBox	m_ComboKback;
	CComboBox	m_ComboKfront;
	CComboBox	m_ComboOrientation;
	int		m_nOrientation;
	int		m_nRibbonType;
	BOOL	m_bPrintFront;
	BOOL	m_bPrintBack;
	int		m_nFrontPanel;
	int		m_nFrontK;
	int		m_nBackPanel;
	int		m_nBackK;
	int		m_nInfoType;
	CString	m_csBmpFront;
	CString	m_csBmpBack;
	int		m_nImgX;
	int		m_nImgY;
	int		m_nTxtX;
	int		m_nTxtY;
	CString	m_csText;
	int		m_nWidth;
	int		m_nHeight;
	CString	m_csFontName;
	int		m_nFontSize;
	int		m_nImgX2;
	int		m_nImgY2;
	int		m_nTxtX2;
	int		m_nTxtY2;
	int		m_nWidth2;
	int		m_nHeight2;
	CString	m_csText2;
	CString	m_csFontName2;
	int		m_nFontSize2;
	int		m_nFeedCard;
	int		m_nEjectCard;
	BOOL	m_bWaitRemoval;
	BOOL	m_bRotate180Front;
	BOOL	m_bRotate180Back;
	BOOL	m_bHookMode;
	BOOL	m_bHookRetry;
	BOOL	m_bCardInOutByDev;
	int		m_nResolution;
	BOOL	m_bEraseCard;
	CString	m_csBmpFrontK;
	CString	m_csBmpBackK;
	BOOL	m_bMirrorFront;
	BOOL	m_bMirrorBack;
	BOOL	m_bAutoDetectRibbon;
	BOOL	m_bMonoSpeedMode;
	int		m_nImgKX;
	int		m_nImgKY;
	int		m_nImgKWidth;
	int		m_nImgKHeight;
	int		m_nImgKX2;
	int		m_nImgKY2;
	int		m_nImgKWidth2;
	int		m_nImgKHeight2;
	BOOL	m_bGrayYMC;
	int		m_nPaperSize;
	BOOL	m_b300x1200Mode;
	BOOL	m_bSecurityEraseFront;
	BOOL	m_bSecurityEraseBack;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPrint)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnBrowse();
	afx_msg void OnBtnBrowse2();
	afx_msg void OnBtnGeneralPrint();
	afx_msg void OnBtnSimplePrint();
	afx_msg void OnCheckUseDeviceDefaultCardInOut();
	afx_msg void OnBtnBrowseK();
	afx_msg void OnBtnBrowseK2();
	afx_msg void OnBtnGetPrinterSetting();
	afx_msg void OnBtnSetPrinterSetting();
	afx_msg void OnSelchangeComboRibbonType();
	afx_msg void OnCheckAutoDetectRibbon();
	afx_msg void OnSelchangeComboFrontPanel();
	afx_msg void OnSelchangeComboBackPanel();
	afx_msg void OnSelchangeComboFeedCard();
	afx_msg void OnSelchangeComboEjectCard();
	afx_msg void OnBtnGetPrintArea();
	afx_msg void OnBtnSetPrintArea();
	afx_msg void OnSelchangeComboPaperSize();
	afx_msg void OnSelchangeComboResolution();
	afx_msg void OnBtnDeleteAllJobs();
	afx_msg void OnBtnBrowseSecurityBmpBack();
	afx_msg void OnBtnBrowseSecurityBmpFront();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRINT_H__A21340A4_D704_459A_A203_B63C682B83AD__INCLUDED_)
