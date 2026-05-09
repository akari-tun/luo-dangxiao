#if !defined(AFX_DLGFM1208_H__6DEB3301_3E72_412E_8F94_3C6AC1650116__INCLUDED_)
#define AFX_DLGFM1208_H__6DEB3301_3E72_412E_8F94_3C6AC1650116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFm1208.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlgFm1208 dialog

class CDlgFm1208 : public CDialog
{
// Construction
public:
	CDlgFm1208(CWnd* pParent = NULL);   // standard constructor

	void		SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName){lstrcpy(m_szPrinterName, pPrinterName);lstrcpy(m_szDriverName, pDriverName);};
	void		ShowResult(LPCTSTR szText);
	void		ShowResult(int nStringID);
	void		ShowFunctionOK(LPCTSTR szFuncName);
	void		ShowFunctionNG(LPCTSTR szFuncName);


	bool		InitialScardFunction();
	void		ReleaseScardFunction();

	void		BuildApdu(BYTE byCls, BYTE byIns, BYTE byP1, BYTE byP2, BYTE byLen, BYTE* pData, BYTE byLe);
	long		DoApdu(BYTE byCls, BYTE byIns, BYTE byP1, BYTE byP2, BYTE byLen, BYTE* pData, BYTE byLe);
	WORD		GetChallenge(BYTE* outChallenge, BYTE byLen);
	WORD		ExternalAuthenticate(BYTE keyID, const BYTE* key, BYTE KeyLen);
	WORD		SelectDF(WORD fileID, char *dfName, BYTE* lpOutData, DWORD* lpdwLen);
	WORD		SelectMF(BYTE* lpOutData, DWORD* lpdwLen);
	WORD		SelectEF(WORD fileID, BYTE* lpOutData, DWORD* lpdwLen);
	WORD		CreateMF(BYTE createPermission, BYTE shortID, char* mfName);// = "1PAY.SYS.DDF01")
	WORD		CreateEndMF();
	WORD		EraseMF();
	WORD		DeleteMF();
	WORD		CreateDF(WORD fileID, BYTE createPermission, char* dfName);
	WORD		CreateEF(WORD fileID, BYTE byFileType, WORD wFileSpaceSize, BYTE byCreatePermission, BYTE byWritePermission, BYTE B6, BYTE B7);
	WORD		CreateKeyFile(WORD fileID, WORD wFileSpaceSize, BYTE shortID, BYTE byAddPermission);
	WORD		AddKey(BYTE keyID, BYTE keyType, BYTE usesPermission, BYTE motifyPermission, BYTE status, BYTE errorCount, LPCVOID key, BYTE keyLen);
	WORD		ReadBinary(WORD offset, BYTE* lpBuf, WORD wReadBytes);
	WORD		WriteBinary(WORD offset, BYTE* lpBuf, WORD wWriteBytes);

	HANDLE		m_hDev;

	BYTE		m_byApduProtocol;

	BYTE		m_lpApdu[2048];
	int			m_nApduLen;

	BYTE		m_lpResp[2048];
	int			m_nRespLen;
	WORD		m_wStatusCode;

	TCHAR		m_szPrinterName[128];
	TCHAR		m_szDriverName[128];

// Dialog Data
	//{{AFX_DATA(CDlgFm1208)
	enum { IDD = IDD_DLG_FM1208 };
	CEdit	m_EditResult;
	CEdit	m_EditNameR;
	CEdit	m_EditIdR;
	CEdit	m_EditIdW;
	CEdit	m_EditNameW;
	int		m_nPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFm1208)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFm1208)
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnDisconnect();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInitCard();
	afx_msg void OnBtnRecover();
	afx_msg void OnBtnWriteCard();
	afx_msg void OnBtnReadCard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFM1208_H__6DEB3301_3E72_412E_8F94_3C6AC1650116__INCLUDED_)
