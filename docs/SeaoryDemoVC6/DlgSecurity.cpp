// DlgSecurity.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgSecurity.h"

#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TCHAR* WINAPI GetErrorString(DWORD dwError, TCHAR* szErrorString);
extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgSecurity dialog


CDlgSecurity::CDlgSecurity(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSecurity::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSecurity)
	m_nSecurityMode = -1;
	m_CurPasswd = _T("");
	m_OldPasswd = _T("");
	m_NewPasswd = _T("");
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
}


void CDlgSecurity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSecurity)
	DDX_Control(pDX, IDC_COMBO_SECURITY_MODE, m_ComboSecurityMode);
	DDX_Control(pDX, IDC_EDIT_NEW_PASSWD, m_EditNewPasswd);
	DDX_Control(pDX, IDC_EDIT_OLD_PASSWD, m_EditOldPasswd);
	DDX_Control(pDX, IDC_EDIT_CUR_PASSWD, m_EditCurPasswd);
	DDX_CBIndex(pDX, IDC_COMBO_SECURITY_MODE, m_nSecurityMode);
	DDX_Text(pDX, IDC_EDIT_CUR_PASSWD, m_CurPasswd);
	DDV_MaxChars(pDX, m_CurPasswd, 8);
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWD, m_OldPasswd);
	DDV_MaxChars(pDX, m_OldPasswd, 8);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWD, m_NewPasswd);
	DDV_MaxChars(pDX, m_NewPasswd, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSecurity, CDialog)
	//{{AFX_MSG_MAP(CDlgSecurity)
	ON_BN_CLICKED(IDC_BTN_SET_SECURITY_MODE, OnBtnSetSecurityMode)
	ON_BN_CLICKED(IDC_BTN_SET_SECURITY_PASSWORD, OnBtnSetSecurityPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSecurity message handlers

BOOL CDlgSecurity::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString			temp;
	int				i = 0;

	for(i=IDS_SECURITY_MODE0;i<=IDS_SECURITY_MODE1;i++)
	{
		temp.LoadString(i);
		m_ComboSecurityMode.AddString(temp);
	}
	m_ComboSecurityMode.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSecurity::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
{
	uint32_t			dwRet = 0;
	TCHAR				szModel[256] = {0};

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);

	if ( pPrinterName )
		lstrcpy(m_szPrinterName, pPrinterName);
	if ( pDriverName )
		lstrcpy(m_szDriverName, pDriverName);
}

void CDlgSecurity::OnBtnSetSecurityMode()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_SetSecurityMode(m_szPrinterName, m_CurPasswd, m_nSecurityMode);

	ShowResultMessage(dwRet, _T("OnBtnSetSecurityMode"));
}

void CDlgSecurity::OnBtnSetSecurityPassword()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_SetSecurityPassword(m_szPrinterName, m_OldPasswd, m_NewPasswd);

	ShowResultMessage(dwRet, _T("OnBtnSetSecurityPassword"));
}
