// DlgRetransfer.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgRetransfer.h"

#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgRetransfer dialog


CDlgRetransfer::CDlgRetransfer(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRetransfer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRetransfer)
	m_nCardType = -1;
	m_nFlatMode1 = -1;
	m_nFlatMode2 = -1;
	m_nFlatMode3 = -1;
	m_nHeatTemp1 = 0;
	m_nHeatSpeed1 = 0;
	m_nHeatTemp2 = 0;
	m_nHeatSpeed2 = 0;
	m_nHeatTemp3 = 0;
	m_nHeatSpeed3 = 0;
	m_nHeatPos1 = 0;
	m_nHeatPos2 = 0;
	m_nHeatPos3 = 0;
	m_nFlatSpeed = 0;
	m_nEjectCardSide = -1;
	m_nRibbonEraseMode = -1;
	m_nPrintActionSeq = -1;
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
	m_bModelRxxx = FALSE;
}


void CDlgRetransfer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRetransfer)
	DDX_Control(pDX, IDC_COMBO_PRINT_ACTION_SEQUENCE, m_ComboPrintActionSeq);
	DDX_Control(pDX, IDC_COMBO_RIBBON_ERASE_MODE, m_ComboRibbonEraseMode);
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD_SIDE, m_ComboEjectCardSide);
	DDX_Control(pDX, IDC_COMBO_FLAT_MODE3, m_ComboFlatMode3);
	DDX_Control(pDX, IDC_COMBO_FLAT_MODE2, m_ComboFlatMode2);
	DDX_Control(pDX, IDC_COMBO_FLAT_MODE1, m_ComboFlatMode1);
	DDX_Control(pDX, IDC_COMBO_CARD_TYPE, m_ComboCardType);
	DDX_Control(pDX, IDC_SPIN_FLAT_SPEED, m_SpinFlatSpeed);
	DDX_Control(pDX, IDC_SPIN_HEAT_TEMP3, m_SpinHeatTemp3);
	DDX_Control(pDX, IDC_SPIN_HEAT_SPEED3, m_SpinHeatSpeed3);
	DDX_Control(pDX, IDC_SPIN_HEAT_TEMP2, m_SpinHeatTemp2);
	DDX_Control(pDX, IDC_SPIN_HEAT_SPEED2, m_SpinHeatSpeed2);
	DDX_Control(pDX, IDC_SPIN_HEAT_TEMP1, m_SpinHeatTemp1);
	DDX_Control(pDX, IDC_SPIN_HEAT_SPEED1, m_SpinHeatSpeed1);
	DDX_Control(pDX, IDC_SPIN_HEAT_POS1, m_SpinHeatPos1);
	DDX_Control(pDX, IDC_SPIN_HEAT_POS2, m_SpinHeatPos2);
	DDX_Control(pDX, IDC_SPIN_HEAT_POS3, m_SpinHeatPos3);
	DDX_CBIndex(pDX, IDC_COMBO_CARD_TYPE, m_nCardType);
	DDX_CBIndex(pDX, IDC_COMBO_FLAT_MODE1, m_nFlatMode1);
	DDX_CBIndex(pDX, IDC_COMBO_FLAT_MODE2, m_nFlatMode2);
	DDX_CBIndex(pDX, IDC_COMBO_FLAT_MODE3, m_nFlatMode3);
	DDX_Text(pDX, IDC_EDIT_HEAT_TEMP1, m_nHeatTemp1);
	DDV_MinMaxInt(pDX, m_nHeatTemp1, -11, 7);
	DDX_Text(pDX, IDC_EDIT_HEAT_SPEED1, m_nHeatSpeed1);
	DDV_MinMaxInt(pDX, m_nHeatSpeed1, -10, 10);
	DDX_Text(pDX, IDC_EDIT_HEAT_TEMP2, m_nHeatTemp2);
	DDV_MinMaxInt(pDX, m_nHeatTemp2, -11, 7);
	DDX_Text(pDX, IDC_EDIT_HEAT_SPEED2, m_nHeatSpeed2);
	DDV_MinMaxInt(pDX, m_nHeatSpeed2, -10, 10);
	DDX_Text(pDX, IDC_EDIT_HEAT_TEMP3, m_nHeatTemp3);
	DDV_MinMaxInt(pDX, m_nHeatTemp3, -11, 7);
	DDX_Text(pDX, IDC_EDIT_HEAT_SPEED3, m_nHeatSpeed3);
	DDV_MinMaxInt(pDX, m_nHeatSpeed3, -10, 10);
	DDX_Text(pDX, IDC_EDIT_HEAT_POS1, m_nHeatPos1);
	DDV_MinMaxInt(pDX, m_nHeatPos1, 0, 10);
	DDX_Text(pDX, IDC_EDIT_HEAT_POS2, m_nHeatPos2);
	DDV_MinMaxInt(pDX, m_nHeatPos2, 0, 10);
	DDX_Text(pDX, IDC_EDIT_HEAT_POS3, m_nHeatPos3);
	DDV_MinMaxInt(pDX, m_nHeatPos3, 0, 10);
	DDX_Text(pDX, IDC_EDIT_FLAT_SPEED, m_nFlatSpeed);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD_SIDE, m_nEjectCardSide);
	DDX_CBIndex(pDX, IDC_COMBO_RIBBON_ERASE_MODE, m_nRibbonEraseMode);
	DDX_CBIndex(pDX, IDC_COMBO_PRINT_ACTION_SEQUENCE, m_nPrintActionSeq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRetransfer, CDialog)
	//{{AFX_MSG_MAP(CDlgRetransfer)
	ON_BN_CLICKED(IDC_BTN_READ_CURRENT_CONFIG, OnBtnReadCurrentConfig)
	ON_BN_CLICKED(IDC_BTN_APPLY_NEW_CONFIG, OnBtnApplyNewConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRetransfer message handlers

BOOL CDlgRetransfer::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_SpinHeatTemp1.SetRange(-11, 7);
	m_SpinHeatTemp2.SetRange(-11, 7);
	m_SpinHeatTemp3.SetRange(-11, 7);
	m_SpinHeatSpeed1.SetRange(-10, 10);
	m_SpinHeatSpeed2.SetRange(-10, 10);
	m_SpinHeatSpeed3.SetRange(-10, 10);
	m_SpinHeatPos1.SetRange(0, 10);
	m_SpinHeatPos2.SetRange(0, 10);
	m_SpinHeatPos3.SetRange(0, 10);

	m_SpinFlatSpeed.SetRange(0, 200);

	CString			temp;
	int				i = 0;

	for(i=IDS_RETRANSFER_CARD_TYPE1;i<=IDS_RETRANSFER_CARD_TYPE4;i++)
	{
		temp.LoadString(i);
		m_ComboCardType.AddString(temp);
	}
	m_ComboCardType.SetCurSel(-1);

	for(i=IDS_FLAT_MODE_OPT1;i<=IDS_FLAT_MODE_OPT5;i++)
	{
		temp.LoadString(i);
		m_ComboFlatMode1.AddString(temp);
	}
	m_ComboFlatMode1.SetCurSel(-1);

	for(i=IDS_FLAT_MODE_OPT1;i<=IDS_FLAT_MODE_OPT5;i++)
	{
		temp.LoadString(i);
		m_ComboFlatMode2.AddString(temp);
	}
	m_ComboFlatMode2.SetCurSel(-1);

	for(i=IDS_FLAT_MODE_OPT1;i<=IDS_FLAT_MODE_OPT5;i++)
	{
		temp.LoadString(i);
		m_ComboFlatMode3.AddString(temp);
	}
	m_ComboFlatMode3.SetCurSel(-1);

	for(i=IDS_EJECT_CARD_SIDE_OPT1;i<=IDS_EJECT_CARD_SIDE_OPT3;i++)
	{
		temp.LoadString(i);
		m_ComboEjectCardSide.AddString(temp);
	}
	m_ComboEjectCardSide.SetCurSel(-1);

	for(i=IDS_RIBBON_ERASE_MODE_OPT1;i<=IDS_RIBBON_ERASE_MODE_OPT3;i++)
	{
		temp.LoadString(i);
		m_ComboRibbonEraseMode.AddString(temp);
	}
	m_ComboRibbonEraseMode.SetCurSel(-1);

	for(i=IDS_PRINT_ACTION_SEQUENCE_OPT1;i<=IDS_PRINT_ACTION_SEQUENCE_OPT2;i++)
	{
		temp.LoadString(i);
		m_ComboPrintActionSeq.AddString(temp);
	}
	m_ComboPrintActionSeq.SetCurSel(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgRetransfer::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
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

	m_ComboCardType.SetCurSel(-1);
	m_ComboFlatMode1.SetCurSel(-1);
	m_ComboFlatMode2.SetCurSel(-1);
	m_ComboFlatMode3.SetCurSel(-1);
	m_ComboEjectCardSide.SetCurSel(-1);

	int					nEnable = FALSE;
	if ( m_bModelRxxx )
		nEnable = TRUE;

	for(int i=IDC_BTN_READ_CURRENT_CONFIG;i<=IDC_COMBO_PRINT_ACTION_SEQUENCE;i++)
		GetDlgItem(i)->EnableWindow(nEnable);
}

void CDlgRetransfer::OnBtnReadCurrentConfig()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	TCHAR				szModel[256] = {0};

	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_MODEL_NAME, szModel);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnReadCurrentConfig"));
		return;
	}

	if ( !lstrcmp(szModel, _T("R300")) || !lstrcmp(szModel, _T("R600")) || !lstrcmp(szModel, _T("R600M")) || !lstrcmp(szModel, _T("EPT-W3(AX)")) || !lstrcmp(szModel, _T("MS-DC600G")) || !lstrcmp(szModel, _T("DCE905")) || !lstrcmp(szModel, _T("DR600")) || !lstrcmp(szModel, _T("D600")) || !lstrcmp(szModel, _T("KT-R86")) || !lstrcmp(szModel, _T("D600-Q")) || !lstrcmp(szModel, _T("R5000")) || !lstrcmp(szModel, _T("R330")) || !lstrcmp(szModel, _T("R660")) || !lstrcmp(szModel, _T("E600")) || !lstrcmp(szModel, _T("E330")) || !lstrcmp(szModel, _T("P63XX")) )
	{

	}
	else
	{
		ShowResultMessage(50, _T("OnBtnReadCurrentConfig"));
		return;
	}

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_CARD_TYPE, &m_nCardType);

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER1_SPEED, &m_nHeatSpeed1);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER1_TEMPERATURE, &m_nHeatTemp1);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER1_OFFSET, &m_nHeatPos1);

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER2_SPEED, &m_nHeatSpeed2);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER2_TEMPERATURE, &m_nHeatTemp2);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER2_OFFSET, &m_nHeatPos2);

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER3_SPEED, &m_nHeatSpeed3);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER3_TEMPERATURE, &m_nHeatTemp3);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER3_OFFSET, &m_nHeatPos3);

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_SINGLE_SIDE, &m_nFlatMode1);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_FRONT_SIDE, &m_nFlatMode2);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_BACK_SIDE, &m_nFlatMode3);

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_SPEED, &m_nFlatSpeed);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_EJECT_CARD_SIDE, &m_nEjectCardSide);

	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_RIBBON_ERASE_MODE, &m_nRibbonEraseMode);
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_RTR_PRINT_ACTION_SEQ, &m_nPrintActionSeq);

	UpdateData(FALSE);
	ShowResultMessage(dwRet, _T("OnBtnReadCurrentConfig"));
}

void CDlgRetransfer::OnBtnApplyNewConfig()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	UpdateData(TRUE);

	TCHAR				szModel[256] = {0};

	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_MODEL_NAME, szModel);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnApplyNewConfig"));
		return;
	}

	if ( !lstrcmp(szModel, _T("R300")) || !lstrcmp(szModel, _T("R600")) || !lstrcmp(szModel, _T("R600M")) || !lstrcmp(szModel, _T("EPT-W3(AX)")) || !lstrcmp(szModel, _T("MS-DC600G")) || !lstrcmp(szModel, _T("DCE905")) || !lstrcmp(szModel, _T("DR600")) || !lstrcmp(szModel, _T("D600")) || !lstrcmp(szModel, _T("KT-R86")) || !lstrcmp(szModel, _T("D600-Q")) || !lstrcmp(szModel, _T("R5000")) || !lstrcmp(szModel, _T("R330")) || !lstrcmp(szModel, _T("R660")) || !lstrcmp(szModel, _T("E600")) || !lstrcmp(szModel, _T("E330")) || !lstrcmp(szModel, _T("P63XX")) )
	{

	}
	else
	{
		ShowResultMessage(50, _T("OnBtnApplyNewConfig"));
		return;
	}

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_CARD_TYPE, m_nCardType);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER1_SPEED, m_nHeatSpeed1);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER1_TEMPERATURE, m_nHeatTemp1);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER1_OFFSET, m_nHeatPos1);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER2_SPEED, m_nHeatSpeed2);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER2_TEMPERATURE, m_nHeatTemp2);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER2_OFFSET, m_nHeatPos2);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER3_SPEED, m_nHeatSpeed3);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER3_TEMPERATURE, m_nHeatTemp3);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_USER3_OFFSET, m_nHeatPos3);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_SINGLE_SIDE, m_nFlatMode1);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_FRONT_SIDE, m_nFlatMode2);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_BACK_SIDE, m_nFlatMode3);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_FLAT_SPEED, m_nFlatSpeed);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_EJECT_CARD_SIDE, m_nEjectCardSide);

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_RIBBON_ERASE_MODE, m_nRibbonEraseMode);
	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_RTR_PRINT_ACTION_SEQ, m_nPrintActionSeq);

	ShowResultMessage(dwRet, _T("OnBtnApplyNewConfig"));
}
