// DlgConcave.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgConcave.h"

#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgConcave dialog


CDlgConcave::CDlgConcave(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConcave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConcave)
	m_dwConcaveDistance = 200;
	m_dwConcaveSpace = 10;
	m_csConcaveDigits = _T("123");
	m_shDigit0 = 0;
	m_shDigit1 = 0;
	m_shDigit2 = 0;
	m_shDigit3 = 0;
	m_shDigit4 = 0;
	m_shDigit5 = 0;
	m_shDigit6 = 0;
	m_shDigit7 = 0;
	m_shDigit8 = 0;
	m_shDigit9 = 0;
	m_shDigit10 = 0;
	m_shDigit11 = 0;
	m_shDigit12 = 0;
	m_shDigit13 = 0;
	m_shDigit14 = 0;
	m_shDigit15 = 0;
	m_shDigit16 = 0;
	m_shDigit17 = 0;
	m_shDigit18 = 0;
	m_shDigit19 = 0;
	m_csIndentFwFile = _T("");
	//}}AFX_DATA_INIT
}


void CDlgConcave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConcave)
	DDX_Control(pDX, IDC_SPIN_DIGIT19, m_SpinDigit19);
	DDX_Control(pDX, IDC_SPIN_DIGIT18, m_SpinDigit18);
	DDX_Control(pDX, IDC_SPIN_DIGIT17, m_SpinDigit17);
	DDX_Control(pDX, IDC_SPIN_DIGIT16, m_SpinDigit16);
	DDX_Control(pDX, IDC_SPIN_DIGIT15, m_SpinDigit15);
	DDX_Control(pDX, IDC_SPIN_DIGIT14, m_SpinDigit14);
	DDX_Control(pDX, IDC_SPIN_DIGIT13, m_SpinDigit13);
	DDX_Control(pDX, IDC_SPIN_DIGIT12, m_SpinDigit12);
	DDX_Control(pDX, IDC_SPIN_DIGIT11, m_SpinDigit11);
	DDX_Control(pDX, IDC_SPIN_DIGIT10, m_SpinDigit10);
	DDX_Control(pDX, IDC_EDIT_DIGIT19, m_EditDigit19);
	DDX_Control(pDX, IDC_EDIT_DIGIT18, m_EditDigit18);
	DDX_Control(pDX, IDC_EDIT_DIGIT17, m_EditDigit17);
	DDX_Control(pDX, IDC_EDIT_DIGIT16, m_EditDigit16);
	DDX_Control(pDX, IDC_EDIT_DIGIT15, m_EditDigit15);
	DDX_Control(pDX, IDC_EDIT_DIGIT14, m_EditDigit14);
	DDX_Control(pDX, IDC_EDIT_DIGIT13, m_EditDigit13);
	DDX_Control(pDX, IDC_EDIT_DIGIT12, m_EditDigit12);
	DDX_Control(pDX, IDC_EDIT_DIGIT11, m_EditDigit11);
	DDX_Control(pDX, IDC_EDIT_DIGIT10, m_EditDigit10);
	DDX_Control(pDX, IDC_SPIN_DIGIT9, m_SpinDigit9);
	DDX_Control(pDX, IDC_SPIN_DIGIT8, m_SpinDigit8);
	DDX_Control(pDX, IDC_SPIN_DIGIT7, m_SpinDigit7);
	DDX_Control(pDX, IDC_SPIN_DIGIT6, m_SpinDigit6);
	DDX_Control(pDX, IDC_SPIN_DIGIT5, m_SpinDigit5);
	DDX_Control(pDX, IDC_SPIN_DIGIT4, m_SpinDigit4);
	DDX_Control(pDX, IDC_SPIN_DIGIT3, m_SpinDigit3);
	DDX_Control(pDX, IDC_SPIN_DIGIT2, m_SpinDigit2);
	DDX_Control(pDX, IDC_SPIN_DIGIT1, m_SpinDigit1);
	DDX_Control(pDX, IDC_SPIN_DIGIT0, m_SpinDigit0);
	DDX_Control(pDX, IDC_EDIT_DIGIT9, m_EditDigit9);
	DDX_Control(pDX, IDC_EDIT_DIGIT8, m_EditDigit8);
	DDX_Control(pDX, IDC_EDIT_DIGIT7, m_EditDigit7);
	DDX_Control(pDX, IDC_EDIT_DIGIT6, m_EditDigit6);
	DDX_Control(pDX, IDC_EDIT_DIGIT5, m_EditDigit5);
	DDX_Control(pDX, IDC_EDIT_DIGIT4, m_EditDigit4);
	DDX_Control(pDX, IDC_EDIT_DIGIT3, m_EditDigit3);
	DDX_Control(pDX, IDC_EDIT_DIGIT2, m_EditDigit2);
	DDX_Control(pDX, IDC_EDIT_DIGIT1, m_EditDigit1);
	DDX_Control(pDX, IDC_EDIT_DIGIT0, m_EditDigit0);
	DDX_Control(pDX, IDC_EDIT_CONCAVE_REMAIN_RIBBON, m_EditConcaveRemainRibbon);
	DDX_Control(pDX, IDC_EDIT_CONCAVE_SERIAL, m_EditConcaveSerial);
	DDX_Control(pDX, IDC_EDIT_CONCAVE_FW_VER, m_EditConcaveFwVer);
	DDX_Control(pDX, IDC_SPIN_CONCAVE_SPACE, m_SpinConcaveSpace);
	DDX_Control(pDX, IDC_SPIN_CONCAVE_DISTANCE, m_SpinConcaveDistance);
	DDX_Text(pDX, IDC_EDIT_CONCAVE_DISTANCE, m_dwConcaveDistance);
	DDX_Text(pDX, IDC_EDIT_CONCAVE_SPACE, m_dwConcaveSpace);
	DDX_Text(pDX, IDC_EDIT_CONCAVE_DIGITS, m_csConcaveDigits);
	DDV_MaxChars(pDX, m_csConcaveDigits, 16);
	DDX_Text(pDX, IDC_EDIT_DIGIT0, m_shDigit0);
	DDV_MinMaxInt(pDX, m_shDigit0, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT1, m_shDigit1);
	DDV_MinMaxInt(pDX, m_shDigit1, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT2, m_shDigit2);
	DDV_MinMaxInt(pDX, m_shDigit2, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT3, m_shDigit3);
	DDV_MinMaxInt(pDX, m_shDigit3, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT4, m_shDigit4);
	DDV_MinMaxInt(pDX, m_shDigit4, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT5, m_shDigit5);
	DDV_MinMaxInt(pDX, m_shDigit5, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT6, m_shDigit6);
	DDV_MinMaxInt(pDX, m_shDigit6, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT7, m_shDigit7);
	DDV_MinMaxInt(pDX, m_shDigit7, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT8, m_shDigit8);
	DDV_MinMaxInt(pDX, m_shDigit8, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT9, m_shDigit9);
	DDV_MinMaxInt(pDX, m_shDigit9, 0, 360);
	DDX_Text(pDX, IDC_EDIT_DIGIT10, m_shDigit10);
	DDV_MinMaxInt(pDX, m_shDigit10, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT11, m_shDigit11);
	DDV_MinMaxInt(pDX, m_shDigit11, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT12, m_shDigit12);
	DDV_MinMaxInt(pDX, m_shDigit12, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT13, m_shDigit13);
	DDV_MinMaxInt(pDX, m_shDigit13, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT14, m_shDigit14);
	DDV_MinMaxInt(pDX, m_shDigit14, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT15, m_shDigit15);
	DDV_MinMaxInt(pDX, m_shDigit15, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT16, m_shDigit16);
	DDV_MinMaxInt(pDX, m_shDigit16, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT17, m_shDigit17);
	DDV_MinMaxInt(pDX, m_shDigit17, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT18, m_shDigit18);
	DDV_MinMaxInt(pDX, m_shDigit18, -50, 50);
	DDX_Text(pDX, IDC_EDIT_DIGIT19, m_shDigit19);
	DDV_MinMaxInt(pDX, m_shDigit19, -50, 50);
	DDX_Text(pDX, IDC_EDIT_INDENT_FW_FILE, m_csIndentFwFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConcave, CDialog)
	//{{AFX_MSG_MAP(CDlgConcave)
	ON_BN_CLICKED(IDC_BTN_CONCAVE_PRINT, OnBtnConcavePrint)
	ON_BN_CLICKED(IDC_BTN_GET_CONCAVE_FW_VER, OnBtnGetConcaveFwVer)
	ON_BN_CLICKED(IDC_BTN_GET_CONCAVE_SERIAL, OnBtnGetConcaveSerial)
	ON_BN_CLICKED(IDC_BTN_GET_CONCAVE_REMAIN_RIBBON, OnBtnGetConcaveRemainRibbon)
	ON_BN_CLICKED(IDC_BTN_GET_HAMMER_PARAM, OnBtnGetHammerParam)
	ON_BN_CLICKED(IDC_BTN_SET_HAMMER_PARAM, OnBtnSetHammerParam)
	ON_BN_CLICKED(IDC_BTN_GET_DISK_COMP_PARAM, OnBtnGetDiskCompParam)
	ON_BN_CLICKED(IDC_BTN_SET_DISK_COMP_PARAM, OnBtnSetDiskCompParam)
	ON_BN_CLICKED(IDC_BTN_BROWSE_INDENT_FW_BIN, OnBtnBrowseIndentFwBin)
	ON_BN_CLICKED(IDC_BTN_UPDATE_INDENT_FIRMWARE, OnBtnUpdateIndentFirmware)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConcave message handlers

BOOL CDlgConcave::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_SpinConcaveDistance.SetRange(0, 600);
	m_SpinConcaveSpace.SetRange(0, 100);

	m_SpinDigit0.SetRange(0, 360);
	m_SpinDigit1.SetRange(0, 360);
	m_SpinDigit2.SetRange(0, 360);
	m_SpinDigit3.SetRange(0, 360);
	m_SpinDigit4.SetRange(0, 360);
	m_SpinDigit5.SetRange(0, 360);
	m_SpinDigit6.SetRange(0, 360);
	m_SpinDigit7.SetRange(0, 360);
	m_SpinDigit8.SetRange(0, 360);
	m_SpinDigit9.SetRange(0, 360);

	m_EditDigit0.SetLimitText(3);
	m_EditDigit1.SetLimitText(3);
	m_EditDigit2.SetLimitText(3);
	m_EditDigit3.SetLimitText(3);
	m_EditDigit4.SetLimitText(3);
	m_EditDigit5.SetLimitText(3);
	m_EditDigit6.SetLimitText(3);
	m_EditDigit7.SetLimitText(3);
	m_EditDigit8.SetLimitText(3);
    m_EditDigit9.SetLimitText(3);

	m_SpinDigit10.SetRange(-50, 50);
	m_SpinDigit11.SetRange(-50, 50);
	m_SpinDigit12.SetRange(-50, 50);
	m_SpinDigit13.SetRange(-50, 50);
	m_SpinDigit14.SetRange(-50, 50);
	m_SpinDigit15.SetRange(-50, 50);
	m_SpinDigit16.SetRange(-50, 50);
	m_SpinDigit17.SetRange(-50, 50);
	m_SpinDigit18.SetRange(-50, 50);
	m_SpinDigit19.SetRange(-50, 50);

	m_EditDigit10.SetLimitText(3);
	m_EditDigit11.SetLimitText(3);
	m_EditDigit12.SetLimitText(3);
	m_EditDigit13.SetLimitText(3);
	m_EditDigit14.SetLimitText(3);
	m_EditDigit15.SetLimitText(3);
	m_EditDigit16.SetLimitText(3);
	m_EditDigit17.SetLimitText(3);
	m_EditDigit18.SetLimitText(3);
    m_EditDigit19.SetLimitText(3);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConcave::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
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

void CDlgConcave::OnBtnConcavePrint()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_PrintConcaveDigits(m_szPrinterName, 0, m_dwConcaveDistance, m_dwConcaveSpace, (LPCTSTR)m_csConcaveDigits);

	ShowResultMessage(dwRet, _T("OnBtnConcavePrint"));
}

void CDlgConcave::OnBtnGetConcaveFwVer()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	TCHAR				szInfo[256] = {0};

	UpdateData(TRUE);

	m_EditConcaveFwVer.SetWindowText(_T(""));

	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_INDENT_PRINTER_FW_VERSION, szInfo);
	if ( dwRet == 0 )
		m_EditConcaveFwVer.SetWindowText(szInfo);
	else
		ShowResultMessage(dwRet, _T("OnBtnGetConcaveFwVer"));
}

void CDlgConcave::OnBtnGetConcaveSerial()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	TCHAR				szInfo[256] = {0};

	UpdateData(TRUE);

	m_EditConcaveSerial.SetWindowText(_T(""));

	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_INDENT_PRINTER_SERIAL, szInfo);
	if ( dwRet == 0 )
		m_EditConcaveSerial.SetWindowText(szInfo);
	else
		ShowResultMessage(dwRet, _T("OnBtnGetConcaveSerial"));
}

void CDlgConcave::OnBtnGetConcaveRemainRibbon()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	TCHAR				szInfo[256] = {0};

	UpdateData(TRUE);

	m_EditConcaveRemainRibbon.SetWindowText(_T(""));

	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_INDENT_PRINTER_RIBBON_REMAIN, szInfo);
	if ( dwRet == 0 )
		m_EditConcaveRemainRibbon.SetWindowText(szInfo);
	else
		ShowResultMessage(dwRet, _T("OnBtnGetConcaveRemainRibbon"));
}

void CDlgConcave::OnBtnGetHammerParam()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	INDENT_HAMMER_PARAM	hammerParam;

	memset(&hammerParam, 0, sizeof(INDENT_HAMMER_PARAM));

	dwRet = SOY_PR_GetIndentConfig(m_szPrinterName, CONFIG_INDENT_HAMMER_PARAM, (void*)&hammerParam);
	if ( dwRet == 0 )
	{
		m_shDigit0 = hammerParam.shDigits[0];
		m_shDigit1 = hammerParam.shDigits[1];
		m_shDigit2 = hammerParam.shDigits[2];
		m_shDigit3 = hammerParam.shDigits[3];
		m_shDigit4 = hammerParam.shDigits[4];
		m_shDigit5 = hammerParam.shDigits[5];
		m_shDigit6 = hammerParam.shDigits[6];
		m_shDigit7 = hammerParam.shDigits[7];
		m_shDigit8 = hammerParam.shDigits[8];
		m_shDigit9 = hammerParam.shDigits[9];
	}
	else
		ShowResultMessage(dwRet, _T("OnBtnGetHammerParam"));

	UpdateData(FALSE);
}

void CDlgConcave::OnBtnSetHammerParam()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	INDENT_HAMMER_PARAM	hammerParam;

	memset(&hammerParam, 0, sizeof(INDENT_HAMMER_PARAM));

	UpdateData(TRUE);

	hammerParam.shDigits[0] = m_shDigit0;
	hammerParam.shDigits[1] = m_shDigit1;
	hammerParam.shDigits[2] = m_shDigit2;
	hammerParam.shDigits[3] = m_shDigit3;
	hammerParam.shDigits[4] = m_shDigit4;
	hammerParam.shDigits[5] = m_shDigit5;
	hammerParam.shDigits[6] = m_shDigit6;
	hammerParam.shDigits[7] = m_shDigit7;
	hammerParam.shDigits[8] = m_shDigit8;
	hammerParam.shDigits[9] = m_shDigit9;

	dwRet = SOY_PR_SetIndentConfig(m_szPrinterName, CONFIG_INDENT_HAMMER_PARAM, (void*)&hammerParam);

	ShowResultMessage(dwRet, _T("OnBtnSetHammerParam"));
}

void CDlgConcave::OnBtnGetDiskCompParam()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	INDENT_DISK_COMP_PARAM	diskCompParam;

	memset(&diskCompParam, 0, sizeof(INDENT_DISK_COMP_PARAM));

	dwRet = SOY_PR_GetIndentConfig(m_szPrinterName, CONFIG_INDENT_DISK_COMP_PARAM, (void*)&diskCompParam);
	if ( dwRet == 0 )
	{
		m_shDigit10 = diskCompParam.shDigits[0];
		m_shDigit11 = diskCompParam.shDigits[1];
		m_shDigit12 = diskCompParam.shDigits[2];
		m_shDigit13 = diskCompParam.shDigits[3];
		m_shDigit14 = diskCompParam.shDigits[4];
		m_shDigit15 = diskCompParam.shDigits[5];
		m_shDigit16 = diskCompParam.shDigits[6];
		m_shDigit17 = diskCompParam.shDigits[7];
		m_shDigit18 = diskCompParam.shDigits[8];
		m_shDigit19 = diskCompParam.shDigits[9];
	}
	else
		ShowResultMessage(dwRet, _T("OnBtnGetDiskCompParam"));

	UpdateData(FALSE);
}

void CDlgConcave::OnBtnSetDiskCompParam()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	INDENT_DISK_COMP_PARAM	diskCompParam;

	memset(&diskCompParam, 0, sizeof(INDENT_DISK_COMP_PARAM));

	UpdateData(TRUE);

	diskCompParam.shDigits[0] = m_shDigit10;
	diskCompParam.shDigits[1] = m_shDigit11;
	diskCompParam.shDigits[2] = m_shDigit12;
	diskCompParam.shDigits[3] = m_shDigit13;
	diskCompParam.shDigits[4] = m_shDigit14;
	diskCompParam.shDigits[5] = m_shDigit15;
	diskCompParam.shDigits[6] = m_shDigit16;
	diskCompParam.shDigits[7] = m_shDigit17;
	diskCompParam.shDigits[8] = m_shDigit18;
	diskCompParam.shDigits[9] = m_shDigit19;

	dwRet = SOY_PR_SetIndentConfig(m_szPrinterName, CONFIG_INDENT_DISK_COMP_PARAM, (void*)&diskCompParam);

	ShowResultMessage(dwRet, _T("OnBtnSetDiskCompParam"));
}

void CDlgConcave::OnBtnBrowseIndentFwBin()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("BIN(*.bin)\0*.bin\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_INDENT_FW_FILE)->SetWindowText(OpenFileName);
	}
}

void CDlgConcave::OnBtnUpdateIndentFirmware()
{
	// TODO: Add your control notification handler code here
	DWORD				dwStatus = 0,
						dwError = 0;

	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	if ( m_szPrinterName[0] == 0 )
	{
		MessageBox(_T("Please select a printer."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( m_csIndentFwFile.IsEmpty() )
	{
		MessageBox(_T("Please select a BIN file."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	//check if indent-printing module is exist
	TCHAR				szTemp[32] = {0};
	DWORD				dwModuleFlag = 0;
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_INSTALLED_MODULE, szTemp);
	if ( dwRet == 0 )
	{
		dwModuleFlag = _tstoi(szTemp);
		if ( (dwModuleFlag & MODULE_INDENT) == 0 )
		{
			ShowResultMessage(0x00011043, _T("OnBtnUpdateFirmware"));//Indent-printing module is not attached.
			return;
		}
	}

	dwRet = SOY_PR_UpdateIndentFirmware(m_szPrinterName, m_csIndentFwFile);

	ShowResultMessage(dwRet, _T("OnBtnUpdateFirmware"));
}

