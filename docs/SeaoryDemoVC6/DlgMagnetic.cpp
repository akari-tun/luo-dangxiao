// DlgMagnetic.cpp : implementation file
//

#include "stdafx.h"
#include "SeaoryDemo.h"
#include "DlgMagnetic.h"

#include <winspool.h>
#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgMagnetic dialog


CDlgMagnetic::CDlgMagnetic(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMagnetic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMagnetic)
	m_nCoercivity = 1;
	m_bT1 = TRUE;
	m_bT2 = TRUE;
	m_bT3 = TRUE;
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
}


void CDlgMagnetic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMagnetic)
	DDX_Control(pDX, IDC_COMBO_COERCIVITY, m_ComboCoercitivity);
	DDX_CBIndex(pDX, IDC_COMBO_COERCIVITY, m_nCoercivity);
	DDX_Check(pDX, IDC_CHECK_T1, m_bT1);
	DDX_Check(pDX, IDC_CHECK_T2, m_bT2);
	DDX_Check(pDX, IDC_CHECK_T3, m_bT3);
	DDX_Control(pDX, IDC_EDIT_T1, m_EditT1W);
	DDX_Control(pDX, IDC_EDIT_T2, m_EditT2W);
	DDX_Control(pDX, IDC_EDIT_T3, m_EditT3W);
	DDX_Control(pDX, IDC_EDIT_T1R, m_EditT1R);
	DDX_Control(pDX, IDC_EDIT_T2R, m_EditT2R);
	DDX_Control(pDX, IDC_EDIT_T3R, m_EditT3R);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMagnetic, CDialog)
	//{{AFX_MSG_MAP(CDlgMagnetic)
	ON_BN_CLICKED(IDC_BTN_READ_TRACKS, OnBtnReadTracks)
	ON_BN_CLICKED(IDC_BTN_ENCODE_TRACKS, OnBtnEncodeTracks)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMagnetic message handlers

BOOL CDlgMagnetic::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString			temp;

	temp.LoadString(IDS_LOCO);
	m_ComboCoercitivity.AddString(temp);
	temp.LoadString(IDS_HICO);
	m_ComboCoercitivity.AddString(temp);
	m_ComboCoercitivity.SetCurSel(1);

	m_EditT1W.SetLimitText(76);
	m_EditT2W.SetLimitText(37);
	m_EditT3W.SetLimitText(104);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMagnetic::OnBtnReadTracks()
{
	// TODO: Add your control notification handler code here
	unsigned long		dwRet = 0;

	int					i = 0;

	TCHAR				szMsg[256] = {0};

	TCHAR				szT1[256] = {0}, szT2[256] = {0}, szT3[256] = {0};
	TCHAR				*lpT1 = 0, *lpT2 = 0, *lpT3 = 0;

	CString				temp;

	UpdateData(TRUE);


	if ( !m_bT1 && !m_bT2 && !m_bT3 )
	{
		temp.LoadString(IDS_SELECT_TRACK);
		MessageBox(temp, _T("OnBtnReadTracks"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( m_bT1 )
	{
		lpT1 = szT1;
	}

	if ( m_bT2 )
	{
		lpT2 = szT2;
	}

	if ( m_bT3 )
	{
		lpT3 = szT3;
	}

	//check if printer is locked
	TCHAR				szTemp[32] = {0};
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_SECURITY_MODE, szTemp);
	if ( dwRet == 0 && lstrcmp(szTemp, _T("1")) == 0 )
	{
		ShowResultMessage(0x00013005, _T("OnBtnReadTracks"));//Printer is locked.
		return;
	}

	dwRet = SOY_PR_ReadTrack(m_szPrinterName, 0, lpT1, lpT2, lpT3);
	if ( m_bT1 )	m_EditT1R.SetWindowText(szT1);
	if ( m_bT2 )	m_EditT2R.SetWindowText(szT2);
	if ( m_bT3 )	m_EditT3R.SetWindowText(szT3);

	ShowResultMessage(dwRet, _T("OnBtnReadTracks"));
}

void CDlgMagnetic::OnBtnEncodeTracks()
{
	// TODO: Add your control notification handler code here
	unsigned long		dwRet = 0;

	int					i = 0;

	TCHAR				szMsg[256] = {0};

	DWORD				dwMode = 0;
	TCHAR				szT1[256] = {0}, szT2[256] = {0}, szT3[256] = {0};
	TCHAR				*lpT1 = 0, *lpT2 = 0, *lpT3 = 0;

	CString				temp;

	UpdateData(TRUE);



	if ( !m_bT1 && !m_bT2 && !m_bT3 )
	{
		temp.LoadString(IDS_SELECT_TRACK_WRITE);
		MessageBox(temp, _T("OnBtnEncodeTracks"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( m_nCoercivity == 1 )
		dwMode = 0x00000001;

	if ( m_bT1 )
	{
		m_EditT1W.GetWindowText(szT1, 256);
		if ( szT1[0] == 0 )
		{
			temp.LoadString(IDS_INPUT_TRACK_1);
			MessageBox(temp, _T("OnBtnEncodeTracks"), MB_OK|MB_ICONERROR);
			return;
		}

		//check track data
		for(i=0;i<lstrlen(szT1);i++)
		{
			if ( !(szT1[i] >= ' ' && szT1[i] <= '_' && szT1[i] != ';' && szT1[i] != '?') )
			{
				temp.LoadString(IDS_DATA_WRONG1);
				MessageBox(temp, _T("OnBtnEncodeTracks"), MB_OK|MB_ICONERROR);
				return;
			}
		}

		lpT1 = szT1;
	}

	if ( m_bT2 )
	{
		m_EditT2W.GetWindowText(szT2, 256);
		if ( szT2[0] == 0 )
		{
			temp.LoadString(IDS_INPUT_TRACK_2);
			MessageBox(temp, _T("OnBtnEncodeTracks"), MB_OK|MB_ICONERROR);
			return;
		}

		//check track data
		for(i=0;i<lstrlen(szT2);i++)
		{
			if ( !((szT2[i] >= '0' && szT2[i] <= ':') || (szT2[i] >= '<' && szT2[i] <= '>')) )
			{
				temp.LoadString(IDS_DATA_WRONG2);
				MessageBox(temp, _T("OnBtnEncodeTracks"), MB_OK|MB_ICONERROR);
				return;
			}
		}

		lpT2 = szT2;
	}

	if ( m_bT3 )
	{
		m_EditT3W.GetWindowText(szT3, 256);
		if ( szT3[0] == 0 )
		{
			temp.LoadString(IDS_INPUT_TRACK_3);
			MessageBox(temp, _T("OnBtnEncodeTracks"), MB_OK|MB_ICONERROR);
			return;
		}

		//check track data
		for(i=0;i<lstrlen(szT3);i++)
		{
			if ( !((szT3[i] >= '0' && szT3[i] <= ':') || (szT3[i] >= '<' && szT3[i] <= '>')) )
			{
				temp.LoadString(IDS_DATA_WRONG3);
				MessageBox(temp, _T("OnBtnEncodeTracks"), MB_OK|MB_ICONERROR);
				return;
			}
		}

		lpT3 = szT3;
	}

	//check if printer is locked
	TCHAR				szTemp[32] = {0};
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_SECURITY_MODE, szTemp);
	if ( dwRet == 0 && lstrcmp(szTemp, _T("1")) == 0 )
	{
		ShowResultMessage(0x00013005, _T("OnBtnEncodeTracks"));//Printer is locked.
		return;
	}

	dwRet = SOY_PR_EncodeTrack(m_szPrinterName, dwMode, lpT1, lpT2, lpT3, 1);
	ShowResultMessage(dwRet, _T("OnBtnEncodeTracks"));
}
