// SeaoryDemoDlg.h : header file
//

#if !defined(AFX_SEAORYDEMODLG_H__72706C55_7573_4FAC_9043_B95CA5F79751__INCLUDED_)
#define AFX_SEAORYDEMODLG_H__72706C55_7573_4FAC_9043_B95CA5F79751__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSeaoryDemoDlg dialog

class CSeaoryDemoDlg : public CDialog
{
// Construction
public:
	CSeaoryDemoDlg(CWnd* pParent = NULL);	// standard constructor
	DWORD		FindSupportedPrinters();

	TCHAR		m_szPrinterNameArray[32][128];
	TCHAR		m_szDriverNameArray[32][128];

// Dialog Data
	//{{AFX_DATA(CSeaoryDemoDlg)
	enum { IDD = IDD_SEAORYDEMO_DIALOG };
	CComboBox	m_ComboSymlink;
	CButton	m_EnableLog;
	CStatic	m_StaticSdkVersion;
	CComboBox	m_ComboPos;
	CComboBox	m_ComboPrinter;
	CMyTabCtrl	m_Tab1;
	int		m_nCardPos;
	int		m_nConnectType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeaoryDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSeaoryDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeComboPrinter();
	afx_msg void OnBtnMoveCard();
	afx_msg void OnCheckLog();
	afx_msg void OnSelchangeComboSymlink();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEAORYDEMODLG_H__72706C55_7573_4FAC_9043_B95CA5F79751__INCLUDED_)
