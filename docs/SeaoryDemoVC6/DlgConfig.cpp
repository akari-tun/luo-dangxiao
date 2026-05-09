// DlgConfig.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgConfig.h"

#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig dialog


CDlgConfig::CDlgConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfig)
	m_nL = 0;
	m_nT = 0;
	m_nS = 0;
	m_nIcPos = 0;
	m_nRfPos = 0;
	m_nCardInSignal = -1;
	m_nInitRibbonMode = -1;
	m_bRejectBoxCover = FALSE;
	m_bRejectBoxFull = FALSE;
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
	m_bModelRxxx = FALSE;
}


void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfig)
	DDX_Control(pDX, IDC_COMBO_INIT_RIBBON_MODE, m_ComboInitRibbonMode);
	DDX_Control(pDX, IDC_COMBO_CARD_IN_SIGNAL, m_ComboCardInSignal);
	DDX_Control(pDX, IDC_SPIN_RF_POS, m_SpinRfPos);
	DDX_Control(pDX, IDC_SPIN_IC_POS, m_SpinIcPos);
	DDX_Control(pDX, IDC_EDIT_RF_POS, m_EditRfPos);
	DDX_Control(pDX, IDC_EDIT_IC_POS, m_EditIcPos);
	DDX_Control(pDX, IDC_SPIN_S, m_SpinS);
	DDX_Control(pDX, IDC_SPIN_T, m_SpinT);
	DDX_Control(pDX, IDC_SPIN_L, m_SpinL);
	DDX_Text(pDX, IDC_EDIT_L, m_nL);
	DDX_Text(pDX, IDC_EDIT_T, m_nT);
	DDX_Text(pDX, IDC_EDIT_S, m_nS);
	DDX_Text(pDX, IDC_EDIT_IC_POS, m_nIcPos);
	DDX_Text(pDX, IDC_EDIT_RF_POS, m_nRfPos);
	DDX_CBIndex(pDX, IDC_COMBO_CARD_IN_SIGNAL, m_nCardInSignal);
	DDX_CBIndex(pDX, IDC_COMBO_INIT_RIBBON_MODE, m_nInitRibbonMode);
	DDX_Check(pDX, IDC_CHECK_REJECT_BOX_COVER, m_bRejectBoxCover);
	DDX_Check(pDX, IDC_CHECK_REJECT_BOX_FULL, m_bRejectBoxFull);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgConfig)
	ON_BN_CLICKED(IDC_BTN_PRINT_POS_CHART, OnBtnPrintPosChart)
	ON_BN_CLICKED(IDC_BTN_GET_PRINT_POS, OnBtnGetPrintPos)
	ON_BN_CLICKED(IDC_BTN_SET_PRINT_POS, OnBtnSetPrintPos)
	ON_BN_CLICKED(IDC_BTN_GET_IC_POS, OnBtnGetIcPos)
	ON_BN_CLICKED(IDC_BTN_SET_IC_POS, OnBtnSetIcPos)
	ON_BN_CLICKED(IDC_BTN_GET_RF_POS, OnBtnGetRfPos)
	ON_BN_CLICKED(IDC_BTN_SET_RF_POS, OnBtnSetRfPos)
	ON_BN_CLICKED(IDC_BTN_GET_OTHER_CONFIG_VALUE, OnBtnGetOtherConfigValue)
	ON_BN_CLICKED(IDC_BTN_SET_OTHER_CONFIG_VALUE, OnBtnSetOtherConfigValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig message handlers

BOOL CDlgConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString			temp;
	int				i = 0;

	m_SpinL.SetRange(-15, 15);
	m_SpinT.SetRange(-15, 15);
	m_SpinS.SetRange(-15, 15);

	m_SpinIcPos.SetRange(-100, 100);
	m_EditIcPos.SetLimitText(4);

	m_SpinRfPos.SetRange(-20, 20);
	m_EditRfPos.SetLimitText(3);

	for(i=IDS_CARD_IN_SIGNAL1;i<=IDS_CARD_IN_SIGNAL2;i++)
	{
		temp.LoadString(i);
		m_ComboCardInSignal.AddString(temp);
	}
	m_ComboCardInSignal.SetCurSel(-1);

	for(i=IDS_INIT_RIBBON_MODE1;i<=IDS_INIT_RIBBON_MODE2;i++)
	{
		temp.LoadString(i);
		m_ComboInitRibbonMode.AddString(temp);
	}
	m_ComboInitRibbonMode.SetCurSel(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfig::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
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

	InitialControls();
}

void CDlgConfig::InitialControls()
{
	m_nL = 0;
	m_nT = 0;
	m_nS = 0;

	m_nIcPos = 0;
	m_nRfPos = 0;
	m_nCardInSignal = -1;
	m_nInitRibbonMode = -1;
	m_bRejectBoxCover = FALSE;
	m_bRejectBoxFull = FALSE;

	if ( m_bModelRxxx )
		GetDlgItem(IDC_CHECK_REJECT_BOX_COVER)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_CHECK_REJECT_BOX_COVER)->ShowWindow(SW_SHOW);

	UpdateData(FALSE);
}

void CDlgConfig::OnBtnGetPrintPos()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_PRINT_POS_L, &m_nL);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnGetPrintPos"));
		return;
	}

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_PRINT_POS_T, &m_nT);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_PRINT_POS_S, &m_nS);

	UpdateData(FALSE);

	ShowResultMessage(dwRet, _T("OnBtnGetPrintPos"));
}

void CDlgConfig::OnBtnPrintPosChart()
{
	uint32_t			dwRet = 0,
						dwStatus = 0,
						dwError = 0;

	BITMAP				BmpChart = {0};
	HBITMAP				hBmpChart = 0;

	TCHAR				szModel[256] = {0};
	TCHAR				szPosChart[512] = {0};

	UpdateData(TRUE);

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwStatus != 0 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnPrintPosChart"));
		return;
	}

	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_MODEL_NAME, szModel);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnPrintPosChart"));
		return;
	}

	GetModuleFileName(NULL, szPosChart, 512);
	*_tcsrchr(szPosChart, '\\') = '\0';

	if ( !lstrcmp(szModel, _T("S20R")) || !lstrcmp(szModel, _T("S22R")) )
	{
		lstrcat(szPosChart, _T("\\PosChart_648x1012_rewrite.bmp"));
	}
	else if ( !lstrcmp(szModel, _T("R300")) || !lstrcmp(szModel, _T("D600-Q")) || !lstrcmp(szModel, _T("R5000")) || !lstrcmp(szModel, _T("R330")) || !lstrcmp(szModel, _T("E330")) || !lstrcmp(szModel, _T("P63XX")) )
	{
		lstrcat(szPosChart, _T("\\PosChart_648x1030.bmp"));
	}
	else if ( !lstrcmp(szModel, _T("R600")) || !lstrcmp(szModel, _T("R600M")) || !lstrcmp(szModel, _T("EPT-W3(AX)")) || !lstrcmp(szModel, _T("MS-DC600G")) || !lstrcmp(szModel, _T("DCE905")) || !lstrcmp(szModel, _T("DR600")) || !lstrcmp(szModel, _T("D600")) || !lstrcmp(szModel, _T("KT-R86")) || !lstrcmp(szModel, _T("R660")) || !lstrcmp(szModel, _T("E600")) )
	{
		lstrcat(szPosChart, _T("\\PosChart_1296x2060.bmp"));
	}
	else
	{
		lstrcat(szPosChart, _T("\\PosChart_648x1012.bmp"));
	}

	hBmpChart = (HBITMAP) LoadImage(0, (LPCTSTR)szPosChart, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	dwError = GetLastError();
	GetObject(hBmpChart, sizeof(BITMAP), &BmpChart);

	PRINT_CARD_PARAM		cardParam = {0};
	memset(&cardParam, 0, sizeof(PRINT_CARD_PARAM));

	cardParam.dwSize = sizeof(PRINT_CARD_PARAM);

	cardParam.lpFrontBGR		= &BmpChart;

	dwRet = SOY_PR_PrintOneCard(m_szPrinterName, &cardParam);

	//release image object
	if ( hBmpChart )			DeleteObject(hBmpChart);

	ShowResultMessage(dwRet, _T("OnBtnPrintPosChart"));
}

void CDlgConfig::OnBtnSetPrintPos()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_PRINT_POS_L, m_nL);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnSetPrintPos"));
		return;
	}

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_PRINT_POS_T, m_nT);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_PRINT_POS_S, m_nS);

	ShowResultMessage(dwRet, _T("OnBtnSetPrintPos"));
}

void CDlgConfig::OnBtnGetIcPos()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_IC_CARD_POS, &m_nIcPos);

	UpdateData(FALSE);

	ShowResultMessage(dwRet, _T("OnBtnGetIcPos"));
}

void CDlgConfig::OnBtnSetIcPos()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_IC_CARD_POS, m_nIcPos);

	ShowResultMessage(dwRet, _T("OnBtnSetIcPos"));
}

void CDlgConfig::OnBtnGetRfPos()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RF_CARD_POS, &m_nRfPos);

	UpdateData(FALSE);

	ShowResultMessage(dwRet, _T("OnBtnGetIcPos"));
}

void CDlgConfig::OnBtnSetRfPos()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RF_CARD_POS, m_nRfPos);

	ShowResultMessage(dwRet, _T("OnBtnSetRfPos"));
}

void CDlgConfig::OnBtnGetOtherConfigValue()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	int32_t				nConfigValue = 0;

	uint32_t			dwStatus = 0;

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwStatus == 1167 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnGetOtherConfigValue"));
		return;
	}

	//废卡盒感应器
	m_bRejectBoxCover = FALSE;
	m_bRejectBoxFull = FALSE;

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_REJECT_BOX_SENSOR, &nConfigValue);
	if ( !m_bModelRxxx )
	{
		if ( nConfigValue & 0x00000001 )
			m_bRejectBoxCover = TRUE;
	}
	if ( nConfigValue & 0x00000002 )
		m_bRejectBoxFull = TRUE;

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_CARD_IN_SIGNAL, &nConfigValue);
	if ( dwRet == 0 )
		m_nCardInSignal = nConfigValue;
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_INITIAL_RIBBON_MODE, &nConfigValue);
	if ( dwRet == 0 )
		m_nInitRibbonMode = nConfigValue;

	UpdateData(FALSE);

	ShowResultMessage(dwRet, _T("OnBtnGetOtherConfigValue"));
}

void CDlgConfig::OnBtnSetOtherConfigValue()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	int32_t				nConfigValue = 0;

	uint32_t			dwStatus = 0;

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwStatus == 1167 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnSetOtherConfigValue"));
		return;
	}

	UpdateData(TRUE);

	//废卡盒感应器
	if ( !m_bModelRxxx )
	{
		if ( m_bRejectBoxCover )
			nConfigValue = 1;
	}
	if ( m_bRejectBoxFull )
		nConfigValue += 2;

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_REJECT_BOX_SENSOR, nConfigValue);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_CARD_IN_SIGNAL, m_nCardInSignal);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_INITIAL_RIBBON_MODE, m_nInitRibbonMode);

	ShowResultMessage(dwRet, _T("OnBtnSetOtherConfigValue"));
}
