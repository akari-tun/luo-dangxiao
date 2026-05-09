#if !defined(AFX_DLGRETRANSFER_H__7CB02B4A_2C10_41EE_82C4_FA08A346A418__INCLUDED_)
#define AFX_DLGRETRANSFER_H__7CB02B4A_2C10_41EE_82C4_FA08A346A418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRetransfer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRetransfer dialog

class CDlgRetransfer : public CDialog
{
// Construction
public:
	CDlgRetransfer(CWnd* pParent = NULL);   // standard constructor

	void			SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

	TCHAR			m_szPrinterName[128];
	TCHAR			m_szDriverName[128];
	BOOL			m_bModelRxxx;

// Dialog Data
	//{{AFX_DATA(CDlgRetransfer)
	enum { IDD = IDD_DLG_RETRANSFER };
	CComboBox	m_ComboPrintActionSeq;
	CComboBox	m_ComboRibbonEraseMode;
	CComboBox	m_ComboEjectCardSide;
	CComboBox	m_ComboFlatMode3;
	CComboBox	m_ComboFlatMode2;
	CComboBox	m_ComboFlatMode1;
	CComboBox	m_ComboCardType;
	CSpinButtonCtrl	m_SpinFlatSpeed;
	CSpinButtonCtrl	m_SpinHeatTemp3;
	CSpinButtonCtrl	m_SpinHeatSpeed3;
	CSpinButtonCtrl	m_SpinHeatTemp2;
	CSpinButtonCtrl	m_SpinHeatSpeed2;
	CSpinButtonCtrl	m_SpinHeatTemp1;
	CSpinButtonCtrl	m_SpinHeatSpeed1;
	CSpinButtonCtrl	m_SpinHeatPos1;
	CSpinButtonCtrl	m_SpinHeatPos2;
	CSpinButtonCtrl	m_SpinHeatPos3;
	int		m_nCardType;
	int		m_nFlatMode1;
	int		m_nFlatMode2;
	int		m_nFlatMode3;
	int		m_nHeatTemp1;
	int		m_nHeatSpeed1;
	int		m_nHeatTemp2;
	int		m_nHeatSpeed2;
	int		m_nHeatTemp3;
	int		m_nHeatSpeed3;
	int		m_nHeatPos1;
	int		m_nHeatPos2;
	int		m_nHeatPos3;
	int		m_nFlatSpeed;
	int		m_nEjectCardSide;
	int		m_nRibbonEraseMode;
	int		m_nPrintActionSeq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRetransfer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRetransfer)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReadCurrentConfig();
	afx_msg void OnBtnApplyNewConfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRETRANSFER_H__7CB02B4A_2C10_41EE_82C4_FA08A346A418__INCLUDED_)
