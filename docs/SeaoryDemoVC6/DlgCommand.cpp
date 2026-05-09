// DlgCommand.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgCommand.h"

#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TCHAR* WINAPI GetErrorString(DWORD dwError, TCHAR* szErrorString);
extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgCommand dialog


CDlgCommand::CDlgCommand(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCommand::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCommand)
	m_nCommand = 0;
	m_nInfoType = 0;
	m_nStandbySide = 0;
	m_nWaitPos = 0;
	m_nWaitTime = 0;
	m_csBmpErase = _T("");
	m_csFwFile = _T("");
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
	m_bModelRxxx = FALSE;
}


void CDlgCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCommand)
	DDX_Control(pDX, IDC_EDIT_RIBBON_LED_VALUE, m_EditRbnLed);
	DDX_Control(pDX, IDC_COMBO_STANDBY_SIDE, m_ComboStandbySide);
	DDX_Control(pDX, IDC_COMBO_INFO_TYPE, m_ComboPrinterInfo);
	DDX_Control(pDX, IDC_COMBO_COMMAND, m_ComboCommand);
	DDX_CBIndex(pDX, IDC_COMBO_COMMAND, m_nCommand);
	DDX_CBIndex(pDX, IDC_COMBO_INFO_TYPE, m_nInfoType);
	DDX_CBIndex(pDX, IDC_COMBO_STANDBY_SIDE, m_nStandbySide);
	DDX_Text(pDX, IDC_EDIT_STANDBY_POS, m_nWaitPos);
	DDX_Text(pDX, IDC_EDIT_STANDBY_TIME, m_nWaitTime);
	DDX_Text(pDX, IDC_EDIT_ERASE_IMAGE, m_csBmpErase);
	DDX_Text(pDX, IDC_EDIT_FW_FILE, m_csFwFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCommand, CDialog)
	//{{AFX_MSG_MAP(CDlgCommand)
	ON_BN_CLICKED(IDC_BTN_GET_PRINTER_INFO, OnBtnGetPrinterInfo)
	ON_BN_CLICKED(IDC_BTN_GET_PRINTER_STAUTS, OnBtnGetPrinterStauts)
	ON_BN_CLICKED(IDC_BTN_GET_PRINTER_WARNING, OnBtnGetPrinterWarning)
	ON_BN_CLICKED(IDC_BTN_SET_STANDBY_PARAM, OnBtnSetStandbyParam)
	ON_BN_CLICKED(IDC_BTN_BROWSE_ERASE_IMAGE, OnBtnBrowseEraseImage)
	ON_BN_CLICKED(IDC_BTN_ERASE_PARTIAL_CARD, OnBtnErasePartialCard)
	ON_BN_CLICKED(IDC_BTN_EXECUTE_COMMAND, OnBtnExecuteCommand)
	ON_BN_CLICKED(IDC_BTN_BROWSE_FW_FILE, OnBtnBrowseFwFile)
	ON_BN_CLICKED(IDC_BTN_UPDATE_FIRMWARE, OnBtnUpdateFirmware)
	ON_BN_CLICKED(IDC_BTN_GET_BIN_FIRMWARE_VERSION, OnBtnGetBinFirmwareVersion)
	ON_BN_CLICKED(IDC_BTN_CALIBRATE_RIBBON_LED, OnBtnCalibrateRibbonLed)
	ON_BN_CLICKED(IDC_BTN_READ_RIBBON_LED_VALUE, OnBtnReadRibbonLedValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCommand message handlers

BOOL CDlgCommand::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString			temp;
	int				i = 0;

	for(i=IDS_INFO_1;i<=IDS_INFO_24;i++)
	{
		temp.LoadString(i);
		m_ComboPrinterInfo.AddString(temp);
	}
	m_ComboPrinterInfo.SetCurSel(0);

	for(i=IDS_COMMAND_1;i<=IDS_COMMAND_9;i++)
	{
		temp.LoadString(i);
		m_ComboCommand.AddString(temp);
	}
	m_ComboCommand.SetCurSel(0);

	for(i=IDS_STANDBY_SIDE1;i<=IDS_STANDBY_SIDE2;i++)
	{
		temp.LoadString(i);
		m_ComboStandbySide.AddString(temp);
	}
	m_ComboStandbySide.SetCurSel(0);

	//
	TCHAR				szPath[512] = {0};
	GetModuleFileName(NULL, szPath, 512);
	*_tcsrchr(szPath, '\\') = '\0';

	m_csBmpErase  = szPath;
	m_csBmpErase += _T("\\1012x648_erase.bmp");
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCommand::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
{
	uint32_t			dwRet = 0;
	TCHAR				szModel[256] = {0};

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
	m_bModelRxxx = FALSE;

	if ( pPrinterName )
		lstrcpy(m_szPrinterName, pPrinterName);
	if ( pDriverName )
		lstrcpy(m_szDriverName, pDriverName);

	if ( m_szDriverName[0] == 0 )
	{
		dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_MODEL_NAME, szModel);
		if ( dwRet == 0 )
		{
			if ( !lstrcmp(szModel, _T("R300")) || !lstrcmp(szModel, _T("R600")) || !lstrcmp(szModel, _T("R600M")) || !lstrcmp(szModel, _T("EPT-W3(AX)")) || !lstrcmp(szModel, _T("MS-DC600G")) || !lstrcmp(szModel, _T("DCE905")) || !lstrcmp(szModel, _T("DR600")) || !lstrcmp(szModel, _T("D600")) || !lstrcmp(szModel, _T("KT-R86")) || !lstrcmp(szModel, _T("D600-Q")) || !lstrcmp(szModel, _T("R5000")) || !lstrcmp(szModel, _T("R330")) || !lstrcmp(szModel, _T("R660")) || !lstrcmp(szModel, _T("E600")) || !lstrcmp(szModel, _T("E330")) || !lstrcmp(szModel, _T("P63XX")) )
			{
				m_bModelRxxx = TRUE;
			}
		}
	}
	else
	{
		if ( !lstrcmp(m_szDriverName, _T("Seaory R300")) || !lstrcmp(m_szDriverName, _T("Seaory R600")) || !lstrcmp(m_szDriverName, _T("Seaory R600M")) || !lstrcmp(m_szDriverName, _T("EPT-W3(AX)")) || !lstrcmp(m_szDriverName, _T("MS-DC600G")) || !lstrcmp(m_szDriverName, _T("Goldpac DCE905")) || !lstrcmp(m_szDriverName, _T("CTD DR600")) || !lstrcmp(m_szDriverName, _T("TSZ-T1 D600")) || !lstrcmp(m_szDriverName, _T("Kingaotech KT-R86")) || !lstrcmp(m_szDriverName, _T("TSZ-T1 D600-Q")) || !lstrcmp(m_szDriverName, _T("Imagedec R5000")) || !lstrcmp(m_szDriverName, _T("Seaory R330")) || !lstrcmp(m_szDriverName, _T("Seaory R660")) || !lstrcmp(m_szDriverName, _T("Seaory E600")) || !lstrcmp(m_szDriverName, _T("Seaory E330")) || !lstrcmp(m_szDriverName, _T("Fagoo P63XX")) )
		{
			m_bModelRxxx = TRUE;
		}
	}

	int					nShow = SW_SHOW;
	if ( !lstrcmp(m_szDriverName, _T("Seaory S20R")) || !lstrcmp(m_szDriverName, _T("Seaory S22R"))
		|| !lstrcmp(szModel, _T("S20R")) || !lstrcmp(szModel, _T("S22R"))
	)
		nShow = SW_SHOW;
	else
		nShow = SW_HIDE;

	for(int i=IDC_STATIC_ERASE_PARTIAL_CARD;i<=IDC_BTN_ERASE_PARTIAL_CARD;i++)
		GetDlgItem(i)->ShowWindow(nShow);
}

void CDlgCommand::OnBtnGetPrinterInfo()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;


	TCHAR				szMsg[256] = {0};
	TCHAR				szTmp[256] = {0};

	CString				temp;

	int					nInfoType = 0;
	int					nInfoValue = 0;

	UpdateData(TRUE);



	nInfoType = m_nInfoType+1;
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, nInfoType, szMsg);

	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnGetPrinterInfo"));
		return;
	}
	else if ( nInfoType == INFO_CARD_POSITION )
	{
		nInfoValue = _ttoi(szMsg);

		switch(nInfoValue)
		{
			case 0:		lstrcpy(szTmp, _T("Out of printer"));	break;
			case 3:		lstrcpy(szTmp, _T("Front standby"));	break;
			case 4:		lstrcpy(szTmp, _T("Flipper"));			break;
			case 5:		lstrcpy(szTmp, _T("Mag in"));			break;
			case 6:		lstrcpy(szTmp, _T("Mag out"));			break;
			case 7:		lstrcpy(szTmp, _T("Start printing"));	break;
			case 8:		lstrcpy(szTmp, _T("Print end"));		break;
			case 9:		lstrcpy(szTmp, _T("Contact"));			break;
			case 10:	lstrcpy(szTmp, _T("Contactless"));		break;
			case 11:	lstrcpy(szTmp, _T("Back standby"));		break;
			case 12:	lstrcpy(szTmp, _T("Card jam pos"));		break;
			case 13:	lstrcpy(szTmp, _T("Prepare pos"));		break;
			case 15:	lstrcpy(szTmp, _T("Start printing 2"));	break;
			case 17:	lstrcpy(szTmp, _T("Down standby"));		break;
			case 18:	lstrcpy(szTmp, _T("Wait emboss"));		break;
		}

		_stprintf(szMsg, _T("%d  => %s"), nInfoValue, szTmp);
	}
	else if ( nInfoType == INFO_INSTALLED_MODULE )
	{
		nInfoValue = _ttoi(szMsg);

		szMsg[0] = 0;

		_stprintf(szTmp, _T("Flipper             => %s\n"), (nInfoValue&MODULE_FLIPPER)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("Magnetic Stripe     => %s\n"), (nInfoValue&MODULE_MAG_STRIPE)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("Contact Encoder     => %s\n"), (nInfoValue&MODULE_CONTACT_IC)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("Contactless Encoder => %s\n"), (nInfoValue&MODULE_RFID)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("Ethernet            => %s\n"), (nInfoValue&MODULE_ETHERNET)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("600 DPI             => %s\n"), (nInfoValue&MODULE_600DPI)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("Ribbon security     => %s\n"), (nInfoValue&MODULE_RIBBON_SECURITY)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("Printer security    => %s\n"), (nInfoValue&MODULE_PRINTER_SECURITY)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);
	}
	else if ( nInfoType == INFO_REJECT_BOX_STATUS )
	{
		nInfoValue = _ttoi(szMsg);

		szMsg[0] = 0;

		_stprintf(szTmp, _T("Reject box cover open => %s\n"), (nInfoValue&0x00000001)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);

		_stprintf(szTmp, _T("Reject box full       => %s\n"), (nInfoValue&0x00000002)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);
	}
	else if ( nInfoType == INFO_CARD_OUT_STATUS )
	{
		nInfoValue = _ttoi(szMsg);

		szMsg[0] = 0;

		_stprintf(szTmp, _T("Card out => %s\n"), (nInfoValue&0x00000001)?_T("Yes"):_T("NO"));
		lstrcat(szMsg, szTmp);
	}

	MessageBox(szMsg, _T("OnBtnGetPrinterInfo"), MB_OK|MB_ICONINFORMATION);
}

void CDlgCommand::OnBtnExecuteCommand()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	TCHAR				szMsg[256] = {0};
	TCHAR				szTmp[256] = {0};

	CString				temp;

	DWORD				dwCommand = 0;

	UpdateData(TRUE);

	switch(m_nCommand)
	{
        case 0: dwCommand = CMD_RESET_PRINTER_HARD;			break;
        case 1: dwCommand = CMD_RESET_PRINTER_JAM;			break;
        case 2: dwCommand = CMD_CLEAN_CARD_PATH;			break;
        case 3: dwCommand = CMD_FLIP_CARD;					break;
        case 4: dwCommand = CMD_CLEAN_CARD_PATH_NO_EJECT;	break;
        case 5: dwCommand = CMD_INITIALIZE_FLIPPER;			break;
		case 6: dwCommand = CMD_ERASE_FULL_CARD;			break;
		case 7: dwCommand = CMD_CANCEL_SUSPENDED_TASK;		break;
		case 8: dwCommand = CMD_CLEAN_CARD_PATH_ADJ_RBN_MTR;break;
   }

	dwRet = SOY_PR_ExecCommand(m_szPrinterName, dwCommand);

	ShowResultMessage(dwRet, _T("OnBtnExecuteCommand"));
}

void CDlgCommand::OnBtnGetPrinterStauts()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	int					i = 0;

	TCHAR				szMsg[256] = {0};

	uint32_t			dwStatus = 0;

	CString				temp;

	UpdateData(TRUE);

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnGetPrinterStauts"));
	}
	else
	{
		if ( dwStatus == 0 )
		{
			temp.LoadString(IDS_READY);
			lstrcpy(szMsg, temp);
			MessageBox(szMsg, _T("OnBtnGetPrinterStatus"), MB_OK);
		}
		else if ( dwStatus == ERROR_BUSY )
		{
			temp.LoadString(IDS_PRINTER_BUSY);
			lstrcpy(szMsg, temp);
			MessageBox(szMsg, _T("OnBtnGetPrinterStatus"), MB_OK);
		}
		else
		{
			ShowResultMessage(dwStatus, _T("OnBtnGetPrinterStauts"));
		}
	}
}

void CDlgCommand::OnBtnGetPrinterWarning()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	int					i = 0;

	TCHAR				szMsg[256] = {0};

	uint32_t		dwStatus = 0;
	const TCHAR			*lpDesc = 0;

	CString				temp;

	UpdateData(TRUE);



	dwRet = SOY_PR_GetPrinterWarning(m_szPrinterName, &dwStatus);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnGetPrinterWarning"));
	}
	else
	{
		while ( WarningMap[i].dwErrCode != 0xFFFFFFFF )
		{
			if ( dwStatus == WarningMap[i].dwErrCode )
			{
				lpDesc = WarningMap[i].pDesc;
				break;
			}
			i++;
		}

		if ( lpDesc )
		{
			lstrcpy(szMsg, lpDesc);
			MessageBox(szMsg, _T("OnBtnGetPrinterWarning"), MB_OK);
		}
		else
		{
			ShowResultMessage(dwStatus, _T("OnBtnGetPrinterWarning"));
		}
	}
}

void CDlgCommand::OnBtnSetStandbyParam()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_SetStandbyParameters(m_szPrinterName, m_nStandbySide+1, m_nWaitPos, m_nWaitTime);

	ShowResultMessage(dwRet, _T("OnBtnSetStandbyParam"));
}

void CDlgCommand::OnBtnBrowseEraseImage()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP(*.bmp)\0*.bmp\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_ERASE_IMAGE)->SetWindowText(OpenFileName);
	}
}

void CDlgCommand::OnBtnErasePartialCard()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0,
						dwStatus = 0,
						dwError = 0;

	BITMAP				BmpYMC1 = {0};
	HBITMAP				hBmpYMC1 = 0;

	TCHAR				szModel[256] = {0};
	TCHAR				szPosChart[512] = {0};

	UpdateData(TRUE);

	if ( m_szPrinterName[0] == 0 )
	{
		MessageBox(_T("Please select a printer."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( m_csBmpErase.IsEmpty() )
	{
		MessageBox(_T("Please select a BMP file."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwStatus != 0 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnErasePartialCard"));
		return;
	}

	//load image from file
	hBmpYMC1 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpErase, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	dwError = GetLastError();
	GetObject(hBmpYMC1, sizeof(BITMAP), &BmpYMC1);

	if ( hBmpYMC1 == 0 )
	{
		MessageBox(_T("Please select an image file."), _T("OnBtnErasePartialCard"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( BmpYMC1.bmBits && !( (BmpYMC1.bmWidth == 648 && BmpYMC1.bmHeight == 1012) || (BmpYMC1.bmWidth == 1012 && BmpYMC1.bmHeight == 648) ) )
	{
		MessageBox(_T("Please select an image file with width and height as 648x1012 or 1012x648."), _T("OnBtnErasePartialCard"), MB_OK|MB_ICONERROR);
		DeleteObject(hBmpYMC1);
		return;
	}

	PRINT_CARD_PARAM		cardParam = {0};
	memset(&cardParam, 0, sizeof(PRINT_CARD_PARAM));

	cardParam.dwSize = sizeof(PRINT_CARD_PARAM);

	cardParam.byErasePass	= 2;
	cardParam.lpFrontErase	= &BmpYMC1;

	dwRet = SOY_PR_PrintOneCard(m_szPrinterName, &cardParam);

	//release image object
	if ( hBmpYMC1 )
		DeleteObject(hBmpYMC1);
	ShowResultMessage(dwRet, _T("OnBtnErasePartialCard"));
}

void CDlgCommand::OnBtnBrowseFwFile()
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
		GetDlgItem(IDC_EDIT_FW_FILE)->SetWindowText(OpenFileName);
	}
}

void CDlgCommand::OnBtnUpdateFirmware()
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

	if ( m_csFwFile.IsEmpty() )
	{
		MessageBox(_T("Please select a BIN file."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	//check if printer is locked
	TCHAR				szTemp[32] = {0};
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_SECURITY_MODE, szTemp);
	if ( dwRet == 0 && lstrcmp(szTemp, _T("1")) == 0 )
	{
		ShowResultMessage(0x00013005, _T("OnBtnUpdateFirmware"));//Printer is locked.
		return;
	}

	dwRet = SOY_PR_UpdateFirmware(m_szPrinterName, m_csFwFile);

	ShowResultMessage(dwRet, _T("OnBtnUpdateFirmware"));
}

void CDlgCommand::OnBtnGetBinFirmwareVersion()
{
	// TODO: Add your control notification handler code here
	DWORD				dwStatus = 0,
						dwError = 0;

	uint32_t			dwRet = 0;

	TCHAR				szBinVer[64] ={0};

	TCHAR				szError[256] = {0};
	TCHAR				szTemp[256] = {0};
	TCHAR				szResult[1024] = {0};

	UpdateData(TRUE);

	GetDlgItem(IDC_EDIT_BIN_FW_VER)->SetWindowText(_T(""));

	if ( m_csFwFile.IsEmpty() )
	{
		MessageBox(_T("Please select a BIN file."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	dwRet = SOY_PR_GetBinFirmwareVersion(m_csFwFile, szBinVer);

	if ( dwRet == 0 )
	{
		GetDlgItem(IDC_EDIT_BIN_FW_VER)->SetWindowText(szBinVer);
	}
	else
	{
		_stprintf(szTemp, _T("SOY_PR_GetBinFirmwareVersion() return %d => %s\n"), dwRet, GetErrorString(dwRet, szError));
		lstrcpy(szResult, szTemp);
		MessageBox(szResult, _T("SOY_PR_GetBinFirmwareVersion"), MB_OK);
	}
}

void CDlgCommand::OnBtnCalibrateRibbonLed()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_ExecCommand(m_szPrinterName, CMD_CALIBRATE_RIBBON_LED);

	if ( dwRet == 1627 )
		MessageBox(_T("Calibrate fail!"), _T("OnBtnCalibrateRibbonLed"), MB_OK);
	else
		ShowResultMessage(dwRet, _T("OnBtnCalibrateRibbonLed"));
}

void CDlgCommand::OnBtnReadRibbonLedValue()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	TCHAR				szLedValue[256] ={0};

	m_EditRbnLed.SetWindowText(_T(""));

	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_RIBBON_LED_VALUE, szLedValue);

	if ( dwRet == 0 )
		m_EditRbnLed.SetWindowText(szLedValue);

	ShowResultMessage(dwRet, _T("OnBtnReadRibbonLedValue"));
}
