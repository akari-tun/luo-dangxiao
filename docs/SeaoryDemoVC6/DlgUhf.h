#if !defined(AFX_DLGUHF_H__7D76A436_B20E_4D3B_BB00_E26A0EE8E254__INCLUDED_)
#define AFX_DLGUHF_H__7D76A436_B20E_4D3B_BB00_E26A0EE8E254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUhf.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlgUhf dialog

class CDlgUhf : public CDialog
{
// Construction
public:
	CDlgUhf(CWnd* pParent = NULL);   // standard constructor

	void		SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName){lstrcpy(m_szPrinterName, pPrinterName);lstrcpy(m_szDriverName, pDriverName);};
	void		ShowResult(LPCTSTR szText);

	int			HexStringToByteArray(LPCTSTR sourceStr, BYTE* buf);
	CString		ByteArrayToHexString(BYTE* buf, int startIndex, int length);

	//
	HANDLE		m_hCom;
	TCHAR		m_szPrinterName[128];
	TCHAR		m_szDriverName[128];


// Dialog Data
	//{{AFX_DATA(CDlgUhf)
	enum { IDD = IDD_DLG_UHF };
	CEdit	m_EditTID;
	CComboBox	m_ComboSectionWrite;
	CComboBox	m_ComboSectionRead;
	CEdit	m_EditDataWrite;
	CEdit	m_EditDataRead;
	CEdit	m_EditResult;
	CEdit	m_EditEPC;
	CString	m_csPasswdRead;
	CString	m_csPasswdWrite;
	int		m_nAddrRead;
	int		m_nAddrWrite;
	int		m_nLenRead;
	int		m_nPort;
	int		m_nSectionRead;
	int		m_nSectionWrite;
	int		m_nAddrTID;
	int		m_nLenTID;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUhf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUhf)
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnDisconnect();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReadData();
	afx_msg void OnBtnWriteData();
	afx_msg void OnDestroy();
	afx_msg void OnBtnReadTID();
	afx_msg void OnBtnReadEPC();
	afx_msg void OnSelchangeComboSectionRead();
	afx_msg void OnSelchangeComboSectionWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUHF_H__7D76A436_B20E_4D3B_BB00_E26A0EE8E254__INCLUDED_)
