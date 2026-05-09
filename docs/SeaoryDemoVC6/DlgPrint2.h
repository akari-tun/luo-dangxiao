#if !defined(AFX_DLGPRINT2_H__FDFEC932_362F_4833_A25A_21044FF75419__INCLUDED_)
#define AFX_DLGPRINT2_H__FDFEC932_362F_4833_A25A_21044FF75419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPrint2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint2 dialog

class CDlgPrint2 : public CDialog
{
// Construction
public:
	CDlgPrint2(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];
	BOOL			m_bModelRxxx;

// Dialog Data
	//{{AFX_DATA(CDlgPrint2)
	enum { IDD = IDD_DLG_PRINT2 };
	CComboBox	m_ComboErasePass;
	CSpinButtonCtrl	m_SpinHeatR2;
	CSpinButtonCtrl	m_SpinHeatO2;
	CSpinButtonCtrl	m_SpinHeatK2;
	CSpinButtonCtrl	m_SpinHeatY2;
	CSpinButtonCtrl	m_SpinHeatWrite;
	CSpinButtonCtrl	m_SpinHeatErase;
	CSpinButtonCtrl	m_SpinHeatR1;
	CSpinButtonCtrl	m_SpinHeatO1;
	CSpinButtonCtrl	m_SpinHeatK1;
	CSpinButtonCtrl	m_SpinHeatY1;
	CSpinButtonCtrl	m_SpinColor8;
	CSpinButtonCtrl	m_SpinColor7;
	CSpinButtonCtrl	m_SpinColor6;
	CSpinButtonCtrl	m_SpinColor5;
	CSpinButtonCtrl	m_SpinColor4;
	CSpinButtonCtrl	m_SpinColor3;
	CSpinButtonCtrl	m_SpinColor2;
	CSpinButtonCtrl	m_SpinColor1;
	CEdit	m_EditColor8;
	CEdit	m_EditColor7;
	CEdit	m_EditColor6;
	CEdit	m_EditColor5;
	CEdit	m_EditColor4;
	CEdit	m_EditColor3;
	CEdit	m_EditColor2;
	CEdit	m_EditColor1;
	CString	m_csBmpYMC1;
	CString	m_csBmpK1;
	CString	m_csBmpO1;
	CString	m_csBmpErase;
	CString	m_csBmpYMC2;
	CString	m_csBmpK2;
	CString	m_csBmpO2;
	int		m_nColor1;
	int		m_nColor2;
	int		m_nColor3;
	int		m_nColor4;
	int		m_nColor5;
	int		m_nColor6;
	int		m_nColor7;
	int		m_nColor8;
	int		m_nHeatY1;
	int		m_nHeatK1;
	int		m_nHeatO1;
	int		m_nHeatR1;
	int		m_nHeatErase;
	int		m_nHeatWrite;
	int		m_nHeatY2;
	int		m_nHeatK2;
	int		m_nHeatO2;
	int		m_nHeatR2;
	int		m_nErasePass;
	CString	m_csBmpErase2;
	BOOL	m_bColor1;
	BOOL	m_bK1;
	BOOL	m_bO1;
	BOOL	m_bErase1;
	BOOL	m_bColor2;
	BOOL	m_bK2;
	BOOL	m_bO2;
	BOOL	m_bErase2;
	CSpinButtonCtrl	m_SpinConcaveSpace;
	CSpinButtonCtrl	m_SpinConcaveDistance;
	DWORD	m_dwConcaveDistance;
	DWORD	m_dwConcaveSpace;
	CString	m_csConcaveDigits;
	BOOL	m_bPrintConcaveDigits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPrint2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPrint2)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnPrintBypassDriver();
	//}}AFX_MSG
	afx_msg void OnBtnBrowseImage(UINT);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRINT2_H__FDFEC932_362F_4833_A25A_21044FF75419__INCLUDED_)
