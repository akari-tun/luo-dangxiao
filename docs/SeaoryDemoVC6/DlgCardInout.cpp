// DlgCardInout.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgCardInout.h"

#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgCardInout dialog


CDlgCardInout::CDlgCardInout(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCardInout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCardInout)
	m_bCardOutSensor = FALSE;
	m_bHookMode = FALSE;
	m_bRetryByHook = FALSE;
	m_nOutputBin1 = -1;
	m_nOutputBin2 = -1;
	m_nOutputBin3 = -1;
	m_nOutputBin4 = -1;
	m_bAutoFeedM3 = FALSE;
	m_bAutoFeedM2 = FALSE;
	m_bAutoFeedM1 = FALSE;
	m_nInputBin = -1;
	m_nRejectBin = -1;
	m_bWaitRemoval1 = FALSE;
	m_bWaitRemoval2 = FALSE;
	m_bWaitRemoval3 = FALSE;
	m_nWaitPos1 = 0;
	m_nWaitPos2 = 0;
	m_nWaitPos3 = 0;
	m_nWaitTime1 = 0;
	m_nWaitTime2 = 0;
	m_nWaitTime3 = 0;
	m_bAutoEject = FALSE;
	m_nOutputBin5 = -1;
	m_nInputClean = -1;
	m_nOutputClean = -1;
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
	m_bModelRxxx = FALSE;
}


void CDlgCardInout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCardInout)
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD_CLEAN, m_ComboEjectClean);
	DDX_Control(pDX, IDC_COMBO_FEED_CARD_CLEAN, m_ComboInputClean);
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD5, m_ComboEjectCard5);
	DDX_Control(pDX, IDC_COMBO_REJECT_CARD_DEFAULT, m_ComboRejectBin);
	DDX_Control(pDX, IDC_COMBO_FEED_CARD_DEFAULT, m_ComboInputBin);
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD4, m_ComboEjectCard4);
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD3, m_ComboEjectCard3);
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD2, m_ComboEjectCard2);
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD1, m_ComboEjectCard1);
	DDX_Check(pDX, IDC_CHECK_CARD_OUT_SENSOR, m_bCardOutSensor);
	DDX_Check(pDX, IDC_CHECK_HOOK_MODE2, m_bHookMode);
	DDX_Check(pDX, IDC_CHECK_RETRY_BY_HOOK_MODE2, m_bRetryByHook);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD1, m_nOutputBin1);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD2, m_nOutputBin2);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD3, m_nOutputBin3);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD4, m_nOutputBin4);
	DDX_Check(pDX, IDC_CHECK_AUTO_FEED_M3, m_bAutoFeedM3);
	DDX_Check(pDX, IDC_CHECK_AUTO_FEED_M2, m_bAutoFeedM2);
	DDX_Check(pDX, IDC_CHECK_AUTO_FEED_M1, m_bAutoFeedM1);
	DDX_CBIndex(pDX, IDC_COMBO_FEED_CARD_DEFAULT, m_nInputBin);
	DDX_CBIndex(pDX, IDC_COMBO_REJECT_CARD_DEFAULT, m_nRejectBin);
	DDX_Check(pDX, IDC_CHECK_WAIT_FOR_REMOVAL_E1, m_bWaitRemoval1);
	DDX_Check(pDX, IDC_CHECK_WAIT_FOR_REMOVAL_E2, m_bWaitRemoval2);
	DDX_Check(pDX, IDC_CHECK_WAIT_FOR_REMOVAL_E3, m_bWaitRemoval3);
	DDX_Text(pDX, IDC_EDIT_WAIT_CARD_POS, m_nWaitPos1);
	DDV_MinMaxInt(pDX, m_nWaitPos1, 0, 80);
	DDX_Text(pDX, IDC_EDIT_WAIT_CARD_POS2, m_nWaitPos2);
	DDV_MinMaxInt(pDX, m_nWaitPos2, 0, 80);
	DDX_Text(pDX, IDC_EDIT_WAIT_CARD_POS3, m_nWaitPos3);
	DDV_MinMaxInt(pDX, m_nWaitPos3, 0, 80);
	DDX_Text(pDX, IDC_EDIT_WAIT_CARD_TIME, m_nWaitTime1);
	DDV_MinMaxInt(pDX, m_nWaitTime1, 0, 120000);
	DDX_Text(pDX, IDC_EDIT_WAIT_CARD_TIME2, m_nWaitTime2);
	DDV_MinMaxInt(pDX, m_nWaitTime2, 0, 120000);
	DDX_Text(pDX, IDC_EDIT_WAIT_CARD_TIME3, m_nWaitTime3);
	DDV_MinMaxInt(pDX, m_nWaitTime3, 0, 120000);
	DDX_Check(pDX, IDC_CHECK_AUTO_EJECT_CARD, m_bAutoEject);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD5, m_nOutputBin5);
	DDX_CBIndex(pDX, IDC_COMBO_FEED_CARD_CLEAN, m_nInputClean);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD_CLEAN, m_nOutputClean);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCardInout, CDialog)
	//{{AFX_MSG_MAP(CDlgCardInout)
	ON_BN_CLICKED(IDC_BTN_GET_CUR_CONFIG, OnBtnGetCurConfig)
	ON_BN_CLICKED(IDC_BTN_SET_NEW_CONFIG, OnBtnSetNewConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCardInout message handlers

BOOL CDlgCardInout::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString			temp;
	int				i = 0;

	for(i=IDS_EJECT_CARD1;i<=IDS_EJECT_CARD6;i++)
	{
		temp.LoadString(i);
		m_ComboEjectCard1.AddString(temp);
	}
	m_ComboEjectCard1.SetCurSel(-1);

	for(i=IDS_EJECT_CARD1;i<=IDS_EJECT_CARD6;i++)
	{
		temp.LoadString(i);
		m_ComboEjectCard2.AddString(temp);
	}
	m_ComboEjectCard2.SetCurSel(-1);

	for(i=IDS_EJECT_CARD1;i<=IDS_EJECT_CARD6;i++)
	{
		temp.LoadString(i);
		m_ComboEjectCard3.AddString(temp);
	}
	m_ComboEjectCard3.SetCurSel(-1);

	for(i=IDS_EJECT_CARD1;i<=IDS_EJECT_CARD6;i++)
	{
		temp.LoadString(i);
		m_ComboEjectCard4.AddString(temp);
	}
	m_ComboEjectCard4.SetCurSel(-1);

	for(i=IDS_EJECT_CARD1;i<=IDS_EJECT_CARD4;i++)
	{
		temp.LoadString(i);
		m_ComboEjectCard5.AddString(temp);
	}
	temp.LoadString(IDS_EJECT_CARD6);
	m_ComboEjectCard5.AddString(temp);
	m_ComboEjectCard5.SetCurSel(-1);

	for(i=IDS_FEED_CARD1;i<=IDS_FEED_CARD4;i++)
	{
		temp.LoadString(i);
		m_ComboInputBin.AddString(temp);
	}
	m_ComboInputBin.SetCurSel(-1);

	for(i=IDS_EJECT_CARD1;i<=IDS_EJECT_CARD4;i++)
	{
		temp.LoadString(i);
		m_ComboRejectBin.AddString(temp);
	}
	temp.LoadString(IDS_EJECT_CARD6);
	m_ComboRejectBin.AddString(temp);
	m_ComboRejectBin.SetCurSel(-1);

	temp.LoadString(IDS_DEFAULT);
	m_ComboInputClean.AddString(temp);
	for(i=IDS_FEED_CARD1;i<=IDS_FEED_CARD4;i++)
	{
		temp.LoadString(i);
		m_ComboInputClean.AddString(temp);
	}
	m_ComboInputClean.SetCurSel(-1);

	temp.LoadString(IDS_DEFAULT);
	m_ComboEjectClean.AddString(temp);
	for(i=IDS_EJECT_CARD1;i<=IDS_EJECT_CARD4;i++)
	{
		temp.LoadString(i);
		m_ComboEjectClean.AddString(temp);
	}
	temp.LoadString(IDS_EJECT_CARD6);
	m_ComboEjectClean.AddString(temp);
	m_ComboEjectClean.SetCurSel(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCardInout::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
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

void CDlgCardInout::InitialControls()
{
	int					i = 0;
	for(i=IDC_STATIC_ADF_TAG;i<=IDC_STATIC_WAIT_CARD_MS3;i++)
		GetDlgItem(i)->ShowWindow(SW_SHOW);

	m_bCardOutSensor = FALSE;
	m_bHookMode = FALSE;
	m_bRetryByHook = FALSE;
	m_nOutputBin1 = -1;

	m_bAutoFeedM1 = FALSE;
	m_nOutputBin2 = -1;

	m_bAutoFeedM2 = FALSE;
	m_nOutputBin3 = -1;

	m_bAutoFeedM3 = FALSE;
	m_nOutputBin4 = -1;

	m_bAutoEject = FALSE;
	m_nOutputBin5 = -1;

	m_bWaitRemoval1 = FALSE;
	m_bWaitRemoval2 = FALSE;
	m_nWaitPos1 = 0;
	m_nWaitPos2 = 0;
	m_nWaitTime1 = 0;
	m_nWaitTime2 = 0;

	m_bWaitRemoval3 = FALSE;
	m_nWaitPos3 = 0;
	m_nWaitTime3 = 0;

	m_nInputBin = -1;
	m_nRejectBin = -1;

	m_nInputClean = -1;
	m_nOutputClean = -1;

	UpdateData(FALSE);
}

void CDlgCardInout::OnBtnGetCurConfig()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;
	int32_t				nValue = 0;

    int32_t             nWaitPos = 0, nWaitTime = 0;

	int					i = 0;
	int					nShow = SW_SHOW;

	uint32_t			dwStatus = 0;

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwStatus == 1167 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnGetCurConfig"));
		return;
	}

	for(i=IDC_STATIC_ADF_TAG;i<=IDC_STATIC_WAIT_CARD_MS3;i++)
		GetDlgItem(i)->ShowWindow(SW_HIDE);

	m_bCardOutSensor = FALSE;
	m_bHookMode = FALSE;
	m_bRetryByHook = FALSE;
	m_nOutputBin1 = -1;

	m_bAutoFeedM1 = FALSE;
	m_nOutputBin2 = -1;

	m_bAutoFeedM2 = FALSE;
	m_nOutputBin3 = -1;

	m_bAutoFeedM3 = FALSE;
	m_nOutputBin4 = -1;

	m_bAutoEject = FALSE;
	m_nOutputBin5 = -1;

	m_bWaitRemoval1 = FALSE;
	m_bWaitRemoval2 = FALSE;
	m_nWaitPos1 = 0;
	m_nWaitPos2 = 0;
	m_nWaitTime1 = 0;
	m_nWaitTime2 = 0;

	m_bWaitRemoval3 = FALSE;
	m_nWaitPos3 = 0;
	m_nWaitTime3 = 0;

	m_nInputBin = -1;
	m_nRejectBin = -1;

	m_nInputClean = -1;
	m_nOutputClean = -1;

	//feeder ------------------------------------
	nValue = 0;
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, &nValue);
	if ( dwRet == 0 )
	{
		m_nOutputBin1 = nValue;

		GetDlgItem(IDC_STATIC_ADF_TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EJECT_CARD1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EJECT_CARD1)->ShowWindow(SW_SHOW);

		nValue = 0;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_CARD_OUT_SENSOR, &nValue);
		if ( dwRet == 0 )
		{
			GetDlgItem(IDC_CHECK_CARD_OUT_SENSOR)->ShowWindow(SW_SHOW);
			if ( nValue != 0 )
				m_bCardOutSensor = TRUE;
		}

		nValue = 0;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_ADF_HOOK_MODE, &nValue);
		if ( dwRet == 0 )
		{
			GetDlgItem(IDC_CHECK_HOOK_MODE2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CHECK_RETRY_BY_HOOK_MODE2)->ShowWindow(SW_SHOW);

			if ( nValue & 0x00000001 )
				m_bHookMode = TRUE;

			if ( nValue & 0x00000002 )
				m_bRetryByHook = TRUE;
		}
	}

	//back manual -------------------------------
	nValue = 2;
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, &nValue);
	if ( dwRet == 0 )
	{
		m_nOutputBin2 = nValue;

		GetDlgItem(IDC_STATIC_BACK_END_TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EJECT_CARD2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EJECT_CARD2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_AUTO_FEED_M1)->ShowWindow(SW_SHOW);

		nValue = 0;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_MANUAL_FEED_BACK, &nValue);
		if ( nValue != 0 )
			m_bAutoFeedM1 = TRUE;

		//
		nValue = 2;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_MODE, &nValue);
		if ( dwRet == 0 )
		{
			for(i=IDC_STATIC_EXIT1;i<=IDC_STATIC_WAIT_CARD_MS;i++)
				GetDlgItem(i)->ShowWindow(SW_SHOW);

			if ( nValue != 0 )
				m_bWaitRemoval1 = TRUE;

            m_nWaitPos1 = 0;
            m_nWaitTime1 = 0;
            dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_STANDBY_BACK_END, &nValue);
            if ( dwRet == 0 )
            {
                m_nWaitPos1 = (short)(nValue >> 16);
                m_nWaitTime1 = (short)nValue;
            }
		}
	}

	GetDlgItem(IDC_STATIC_EJECT_CARD_TAG)->ShowWindow(SW_SHOW);

	//front manual ------------------------------
	nValue = 1;
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, &nValue);
	if ( dwRet == 0 )
	{
		m_nOutputBin3 = nValue;

		GetDlgItem(IDC_STATIC_FRONT_END_TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EJECT_CARD3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EJECT_CARD3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_AUTO_FEED_M2)->ShowWindow(SW_SHOW);

		nValue = 0;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_MANUAL_FEED_FRONT, &nValue);
		if ( nValue != 0 )
			m_bAutoFeedM2 = TRUE;

		//
		nValue = 1;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_MODE, &nValue);
		if ( dwRet == 0 )
		{
			for(i=IDC_STATIC_EXIT2;i<=IDC_STATIC_WAIT_CARD_MS2;i++)
				GetDlgItem(i)->ShowWindow(SW_SHOW);

			if ( nValue != 0 )
				m_bWaitRemoval2 = TRUE;

            m_nWaitPos2 = 0;
            m_nWaitTime2 = 0;
            dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_STANDBY_FRONT_END, &nValue);
            if ( dwRet == 0 )
            {
                m_nWaitPos2 = (short)(nValue >> 16);
                m_nWaitTime2 = (short)nValue;
            }
		}
	}

	//down manual ------------------------------
	nValue = 3;
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, &nValue);
	if ( dwRet == 0 )
	{
		m_nOutputBin4 = nValue;

		GetDlgItem(IDC_STATIC_DOWN_END_TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EJECT_CARD4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EJECT_CARD4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_AUTO_FEED_M3)->ShowWindow(SW_SHOW);

		nValue = 0;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_MANUAL_FEED_DOWN, &nValue);
		if ( nValue != 0 )
			m_bAutoFeedM3 = TRUE;

		//
		nValue = 3;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_MODE, &nValue);
		if ( dwRet == 0 )
		{
			for(i=IDC_STATIC_EXIT3;i<=IDC_STATIC_WAIT_CARD_MS3;i++)
				GetDlgItem(i)->ShowWindow(SW_SHOW);

			if ( nValue != 0 )
				m_bWaitRemoval3 = TRUE;

            m_nWaitPos3 = 0;
            m_nWaitTime3 = 0;
            dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_STANDBY_DOWN_END, &nValue);
            if ( dwRet == 0 )
            {
                m_nWaitPos3 = (short)(nValue >> 16);
                m_nWaitTime3 = (short)nValue;
            }
		}
	}

	//Eject card when card jam in initial process ---------
	nValue = 0;
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_AUTO_EJECT_OUTPUT_BIN, &nValue);
	if ( dwRet == 0 )
	{
		m_nOutputBin5 = nValue;

		GetDlgItem(IDC_STATIC_AUTO_EJECT_CARD_TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_AUTO_EJECT_CARD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EJECT_CARD5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EJECT_CARD5)->ShowWindow(SW_SHOW);

		nValue = 0;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_AUTO_EJECT_AT_INIT, &nValue);
		if ( nValue != 0 )
			m_bAutoEject = TRUE;
	}
	else
	{
		GetDlgItem(IDC_STATIC_AUTO_EJECT_CARD_TAG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_AUTO_EJECT_CARD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_EJECT_CARD5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EJECT_CARD5)->ShowWindow(SW_HIDE);
	}

	//input bin
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_INPUT_BIN, &m_nInputBin);

	//reject bin
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_REJECT_BIN, &m_nRejectBin);

	//input and output for clean tool -------------------------------
	nValue = 0;
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_INPUT_BIN_FOR_CLEAN, &nValue);
	if ( dwRet == 0 )
	{
		m_nInputClean = nValue;

		GetDlgItem(IDC_STATIC_DEFAULT_USE_CLEAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_FEED_CARD_CLEAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_FEED_CARD_CLEAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_EJECT_CARD_CLEAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_EJECT_CARD_CLEAN)->ShowWindow(SW_SHOW);

		nValue = 0;
		dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_FOR_CLEAN, &nValue);
		m_nOutputClean = nValue;
	}
	else
	{
		GetDlgItem(IDC_STATIC_DEFAULT_USE_CLEAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_FEED_CARD_CLEAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_FEED_CARD_CLEAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_EJECT_CARD_CLEAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_EJECT_CARD_CLEAN)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);
}

void CDlgCardInout::OnBtnSetNewConfig()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	int					nValue1 = 0, nValue2 = 0;
	int					nConfigValue = 0;

	uint32_t			dwStatus = 0;

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwStatus == 1167 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnSetNewConfig"));
		return;
	}

	UpdateData(TRUE);

	//feeder ------------------------------------
	if ( m_nOutputBin1 >= 0 )
	{
		nConfigValue = m_bCardOutSensor;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_CARD_OUT_SENSOR, nConfigValue);

		nConfigValue = 0;
		if ( m_bHookMode )
			nConfigValue = 1;
		if ( m_bRetryByHook )
			nConfigValue += 2;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_ADF_HOOK_MODE, nConfigValue);

		nValue1 = 0;
		nConfigValue = (nValue1 << 16) + m_nOutputBin1;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, nConfigValue);
	}

	//back manual -------------------------------
	if ( m_nOutputBin2 >= 0 )
	{
		nConfigValue = m_bAutoFeedM1;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_MANUAL_FEED_BACK, nConfigValue);

		nValue1 = 2;
		nConfigValue = (nValue1 << 16) + m_nOutputBin2;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, nConfigValue);

		nValue1 = 2;
		nConfigValue = (nValue1 << 16) + m_bWaitRemoval1;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_MODE, nConfigValue);

        nConfigValue = (m_nWaitPos1 << 16) + m_nWaitTime1;
        dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_STANDBY_BACK_END, nConfigValue);
	}

	//front manual ------------------------------
	if ( m_nOutputBin3 >= 0 )
	{
		nConfigValue = m_bAutoFeedM2;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_MANUAL_FEED_FRONT, nConfigValue);

		nValue1 = 1;
		nConfigValue = (nValue1 << 16) + m_nOutputBin3;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, nConfigValue);

		nValue1 = 1;
		nConfigValue = (nValue1 << 16) + m_bWaitRemoval2;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_MODE, nConfigValue);

        nConfigValue = (m_nWaitPos2 << 16) + m_nWaitTime2;
        dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_STANDBY_FRONT_END, nConfigValue);
	}

	//down manual ------------------------------
	if ( m_nOutputBin4 >= 0 )
	{
		nConfigValue = m_bAutoFeedM3;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_MANUAL_FEED_DOWN, nConfigValue);

		nValue1 = 3;
		nConfigValue = (nValue1 << 16) + m_nOutputBin4;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_INPUT_OUTPUT_BINDING, nConfigValue);

		nValue1 = 3;
		nConfigValue = (nValue1 << 16) + m_bWaitRemoval3;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_MODE, nConfigValue);

        nConfigValue = (m_nWaitPos3 << 16) + m_nWaitTime3;
        dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_STANDBY_DOWN_END, nConfigValue);
	}

	//Eject card when card jam in initial process ---------
	if ( m_nOutputBin5 >= 0 )
	{
		nConfigValue = m_bAutoEject;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_AUTO_EJECT_AT_INIT, nConfigValue);

		nConfigValue = m_nOutputBin5;
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_AUTO_EJECT_OUTPUT_BIN, nConfigValue);
	}

	//input bin
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_INPUT_BIN, m_nInputBin);

	//reject bin
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_REJECT_BIN, m_nRejectBin);

	//input and output for clean tool -------------------------------
	if ( m_nInputClean >= 0 )
	{
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_INPUT_BIN_FOR_CLEAN, m_nInputClean);
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_OUTPUT_BIN_FOR_CLEAN, m_nOutputClean);
	}

	ShowResultMessage(dwRet, _T("OnBtnSetNewConfig"));
}
