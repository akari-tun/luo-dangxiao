// DlgPrint.cpp : implementation file
//

#include "stdafx.h"
#include "SeaoryDemo.h"
#include "DlgPrint.h"


#include <winspool.h>
#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

LONG WINAPI GetSecurityBmpFileName(LPTSTR szPrinterName, BYTE bySide, LPTSTR szFileNameOut)
{
	TCHAR				szFileName[512] = {0};

	LONG				lRet = 0;
	TCHAR				szRootKey[MAX_PATH] = {0};
	HKEY				hRootKey = 0;
	DWORD				dwType = 0,
						dwSize = 0;
	TCHAR				szValueName[256] = {0};

	_stprintf(szRootKey, _T("Printers\\%s"), szPrinterName);

	lRet = RegCreateKeyEx(HKEY_CURRENT_USER, (LPCTSTR)szRootKey, (DWORD)0, NULL, 0, (REGSAM)KEY_READ, NULL, (PHKEY)&hRootKey, NULL);

	if ( lRet == 0 )
	{
	lstrcpy(szValueName, _T("RbnSecurity"));
	if ( bySide == 0 )
		lstrcat(szValueName, _T("_Front"));
	else
		lstrcat(szValueName, _T("_Back"));

	dwType = REG_SZ;
	dwSize = 256*sizeof(TCHAR);

	lRet = RegQueryValueEx(hRootKey, szValueName, NULL, &dwType, (BYTE*)szFileName, &dwSize);

	RegCloseKey(hRootKey);
	}

	lstrcpy(szFileNameOut, szFileName);

	return lRet;
}

LONG WINAPI SetSecurityBmpFileName(LPTSTR szPrinterName, BYTE bySide, LPTSTR szFileNameIn)
{
	TCHAR				szFileName[512] = {0};

	LONG				lRet = 0;
	TCHAR				szRootKey[MAX_PATH] = {0};
	HKEY				hRootKey = 0;
	DWORD				dwType = 0,
						dwSize = 0;
	TCHAR				szValueName[256] = {0};

	_stprintf(szRootKey, _T("Printers\\%s"), szPrinterName);

	lRet = RegCreateKeyEx(HKEY_CURRENT_USER, (LPCTSTR)szRootKey, (DWORD)0, NULL, 0, (REGSAM)KEY_WRITE, NULL, (PHKEY)&hRootKey, NULL);

	if ( lRet == 0 )
	{
	lstrcpy(szValueName, _T("RbnSecurity"));
	if ( bySide == 0 )
		lstrcat(szValueName, _T("_Front"));
	else
		lstrcat(szValueName, _T("_Back"));

	dwType = REG_SZ;
	dwSize = 256*sizeof(TCHAR);

	lRet = RegSetValueEx(hRootKey, szValueName, 0, REG_SZ, (CONST uint8_t*)szFileNameIn, (lstrlen(szFileNameIn)+1)*sizeof(TCHAR));

	RegCloseKey(hRootKey);
	}

	return lRet;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint dialog


CDlgPrint::CDlgPrint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPrint)
	m_nOrientation = 1;
	m_nRibbonType = 0;
	m_bPrintFront = TRUE;
	m_bPrintBack = FALSE;
	m_nFrontPanel = 0;
	m_nFrontK = 0;
	m_nBackPanel = 0;
	m_nBackK = 0;
	m_csBmpFront = _T("./1012x648_bgr1.bmp");
	m_csBmpBack = _T("./1012x648_bgr2.bmp");
	m_nImgX = 0;
	m_nImgY = 0;
	m_nTxtX = 100;
	m_nTxtY = 20;
	m_csText = _T("This is front side.");
	m_nWidth = 0;
	m_nHeight = 0;
	m_csFontName = _T("Arial");
	m_nFontSize = 12;
	m_nImgX2 = 0;
	m_nImgY2 = 0;
	m_nTxtX2 = 200;
	m_nTxtY2 = 20;
	m_nWidth2 = 0;
	m_nHeight2 = 0;
	m_csText2 = _T("This is back side.");
	m_csFontName2 = _T("Arial");
	m_nFontSize2 = 12;
	m_bRotate180Front = FALSE;
	m_bRotate180Back = FALSE;
	m_bHookMode = FALSE;
	m_bHookRetry = FALSE;
	m_bCardInOutByDev = FALSE;
	m_nResolution = 0;
	m_bEraseCard = FALSE;
	m_csBmpFrontK = _T("./barcode1D.bmp");
	m_csBmpBackK = _T("./barcode2D.bmp");
	m_bMirrorFront = FALSE;
	m_bMirrorBack = FALSE;
	m_bAutoDetectRibbon = FALSE;
	m_bMonoSpeedMode = FALSE;
	m_nImgKX = 100;
	m_nImgKY = 500;
	m_nImgKWidth = 0;
	m_nImgKHeight = 0;
	m_nImgKX2 = 700;
	m_nImgKY2 = 400;
	m_nImgKWidth2 = 0;
	m_nImgKHeight2 = 0;
	m_bGrayYMC = FALSE;
	m_nPaperSize = 0;
	m_b300x1200Mode = FALSE;
	m_bSecurityEraseFront = FALSE;
	m_bSecurityEraseBack = FALSE;
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
	m_bModelRxxx = FALSE;
}


void CDlgPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPrint)
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_ComboResolution);
	DDX_Control(pDX, IDC_COMBO_PAPER_SIZE, m_ComboPaperSize);
	DDX_Control(pDX, IDC_COMBO_FRONT_AREA_REWRITE, m_ComboAreaFrontRewrite);
	DDX_Control(pDX, IDC_COMBO_BACK_AREA_K, m_ComboAreaBackK);
	DDX_Control(pDX, IDC_COMBO_BACK_AREA_YMCO, m_ComboAreaBackYMCO);
	DDX_Control(pDX, IDC_COMBO_FRONT_AREA_K, m_ComboAreaFrontK);
	DDX_Control(pDX, IDC_COMBO_FRONT_AREA_YMCO, m_ComboAreaFrontYMCO);
	DDX_Control(pDX, IDC_COMBO_BACK_PANEL, m_ComboBackPanel);
	DDX_Control(pDX, IDC_COMBO_FRONT_PANEL, m_ComboFrontPanel);
	DDX_Control(pDX, IDC_COMBO_RIBBON_TYPE, m_ComboRibbon);
	DDX_Control(pDX, IDC_COMBO_FEED_CARD, m_ComboFeedCard);
	DDX_Control(pDX, IDC_COMBO_EJECT_CARD, m_ComboEjectCard);
	DDX_Control(pDX, IDC_COMBO_BACK_K_METHOD, m_ComboKback);
	DDX_Control(pDX, IDC_COMBO_FRONT_K_METHOD, m_ComboKfront);
	DDX_Control(pDX, IDC_COMBO_ORIENTATION, m_ComboOrientation);
	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nOrientation);
	DDX_CBIndex(pDX, IDC_COMBO_RIBBON_TYPE, m_nRibbonType);
	DDX_Check(pDX, IDC_CHECK_PRINT_FRONT, m_bPrintFront);
	DDX_Check(pDX, IDC_CHECK_PRINT_BACK, m_bPrintBack);
	DDX_CBIndex(pDX, IDC_COMBO_FRONT_PANEL, m_nFrontPanel);
	DDX_CBIndex(pDX, IDC_COMBO_FRONT_K_METHOD, m_nFrontK);
	DDX_CBIndex(pDX, IDC_COMBO_BACK_PANEL, m_nBackPanel);
	DDX_CBIndex(pDX, IDC_COMBO_BACK_K_METHOD, m_nBackK);
	DDX_Text(pDX, IDC_EDIT_IMG_FRONT, m_csBmpFront);
	DDX_Text(pDX, IDC_EDIT_IMG_BACK, m_csBmpBack);
	DDX_Text(pDX, IDC_EDIT_IMG_X, m_nImgX);
	DDX_Text(pDX, IDC_EDIT_IMG_Y, m_nImgY);
	DDX_Text(pDX, IDC_EDIT_TEXT_X, m_nTxtX);
	DDX_Text(pDX, IDC_EDIT_TEXT_Y, m_nTxtY);
	DDX_Text(pDX, IDC_EDIT_TEXT_FRONT, m_csText);
	DDX_Text(pDX, IDC_EDIT_IMG_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDIT_IMG_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_EDIT_FONT_NAME, m_csFontName);
	DDX_Text(pDX, IDC_EDIT_FONT_SIZE, m_nFontSize);
	DDX_Text(pDX, IDC_EDIT_IMG_X2, m_nImgX2);
	DDX_Text(pDX, IDC_EDIT_IMG_Y2, m_nImgY2);
	DDX_Text(pDX, IDC_EDIT_TEXT_X2, m_nTxtX2);
	DDX_Text(pDX, IDC_EDIT_TEXT_Y2, m_nTxtY2);
	DDX_Text(pDX, IDC_EDIT_IMG_WIDTH2, m_nWidth2);
	DDX_Text(pDX, IDC_EDIT_IMG_HEIGHT2, m_nHeight2);
	DDX_Text(pDX, IDC_EDIT_TEXT_BACK, m_csText2);
	DDX_Text(pDX, IDC_EDIT_FONT_NAME2, m_csFontName2);
	DDX_Text(pDX, IDC_EDIT_FONT_SIZE2, m_nFontSize2);
	DDX_CBIndex(pDX, IDC_COMBO_FEED_CARD, m_nFeedCard);
	DDX_CBIndex(pDX, IDC_COMBO_EJECT_CARD, m_nEjectCard);
	DDX_Check(pDX, IDC_CHECK_WAIT_FOR_REMOVAL, m_bWaitRemoval);
	DDX_Check(pDX, IDC_CHECK_ROTATE180_FRONT, m_bRotate180Front);
	DDX_Check(pDX, IDC_CHECK_ROTATE180_BACK, m_bRotate180Back);
	DDX_Check(pDX, IDC_CHECK_HOOK_MODE, m_bHookMode);
	DDX_Check(pDX, IDC_CHECK_RETRY_BY_HOOK_MODE, m_bHookRetry);
	DDX_Check(pDX, IDC_CHECK_USE_DEVICE_DEFAULT_CARD_IN_OUT, m_bCardInOutByDev);
	DDX_CBIndex(pDX, IDC_COMBO_RESOLUTION, m_nResolution);
	DDX_Check(pDX, IDC_CHECK_ERASE_BEFORE_PRINT, m_bEraseCard);
	DDX_Text(pDX, IDC_EDIT_IMG_FRONT_K, m_csBmpFrontK);
	DDX_Text(pDX, IDC_EDIT_IMG_BACK_K, m_csBmpBackK);
	DDX_Check(pDX, IDC_CHECK_MIRROR_FRONT, m_bMirrorFront);
	DDX_Check(pDX, IDC_CHECK_MIRROR_BACK, m_bMirrorBack);
	DDX_Check(pDX, IDC_CHECK_AUTO_DETECT_RIBBON, m_bAutoDetectRibbon);
	DDX_Check(pDX, IDC_CHECK_MONO_SPEED, m_bMonoSpeedMode);
	DDX_Text(pDX, IDC_EDIT_IMG_K_X, m_nImgKX);
	DDX_Text(pDX, IDC_EDIT_IMG_K_Y, m_nImgKY);
	DDX_Text(pDX, IDC_EDIT_IMG_K_WIDTH, m_nImgKWidth);
	DDX_Text(pDX, IDC_EDIT_IMG_K_HEIGHT, m_nImgKHeight);
	DDX_Text(pDX, IDC_EDIT_IMG_K_X2, m_nImgKX2);
	DDX_Text(pDX, IDC_EDIT_IMG_K_Y2, m_nImgKY2);
	DDX_Text(pDX, IDC_EDIT_IMG_K_WIDTH2, m_nImgKWidth2);
	DDX_Text(pDX, IDC_EDIT_IMG_K_HEIGHT2, m_nImgKHeight2);
	DDX_Check(pDX, IDC_CHECK_GRAY_YMC, m_bGrayYMC);
	DDX_CBIndex(pDX, IDC_COMBO_PAPER_SIZE, m_nPaperSize);
	DDX_Check(pDX, IDC_CHECK_300X1200, m_b300x1200Mode);
	DDX_Check(pDX, IDC_CHECK_SECURITY_BMP_FRONT, m_bSecurityEraseFront);
	DDX_Check(pDX, IDC_CHECK_SECURITY_BMP_BACK, m_bSecurityEraseBack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPrint, CDialog)
	//{{AFX_MSG_MAP(CDlgPrint)
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_BROWSE2, OnBtnBrowse2)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnGeneralPrint)
	ON_BN_CLICKED(IDC_BTN_SIMPLE_PRINT, OnBtnSimplePrint)
	ON_BN_CLICKED(IDC_CHECK_USE_DEVICE_DEFAULT_CARD_IN_OUT, OnCheckUseDeviceDefaultCardInOut)
	ON_BN_CLICKED(IDC_BTN_BROWSE_K, OnBtnBrowseK)
	ON_BN_CLICKED(IDC_BTN_BROWSE_K2, OnBtnBrowseK2)
	ON_BN_CLICKED(IDC_BTN_GET_PRINTER_SETTING, OnBtnGetPrinterSetting)
	ON_BN_CLICKED(IDC_BTN_SET_PRINTER_SETTING, OnBtnSetPrinterSetting)
	ON_CBN_SELCHANGE(IDC_COMBO_RIBBON_TYPE, OnSelchangeComboRibbonType)
	ON_BN_CLICKED(IDC_CHECK_AUTO_DETECT_RIBBON, OnCheckAutoDetectRibbon)
	ON_CBN_SELCHANGE(IDC_COMBO_FRONT_PANEL, OnSelchangeComboFrontPanel)
	ON_CBN_SELCHANGE(IDC_COMBO_BACK_PANEL, OnSelchangeComboBackPanel)
	ON_CBN_SELCHANGE(IDC_COMBO_FEED_CARD, OnSelchangeComboFeedCard)
	ON_CBN_SELCHANGE(IDC_COMBO_EJECT_CARD, OnSelchangeComboEjectCard)
	ON_BN_CLICKED(IDC_BTN_GET_PRINT_AREA, OnBtnGetPrintArea)
	ON_BN_CLICKED(IDC_BTN_SET_PRINT_AREA, OnBtnSetPrintArea)
	ON_CBN_SELCHANGE(IDC_COMBO_PAPER_SIZE, OnSelchangeComboPaperSize)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, OnSelchangeComboResolution)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL_JOBS, OnBtnDeleteAllJobs)
	ON_BN_CLICKED(IDC_BTN_BROWSE_SECURITY_BMP_BACK, OnBtnBrowseSecurityBmpBack)
	ON_BN_CLICKED(IDC_BTN_BROWSE_SECURITY_BMP_FRONT, OnBtnBrowseSecurityBmpFront)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint message handlers

BOOL CDlgPrint::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	int				k = 0;
	CString			temp;

	for(k=IDS_PORTRAIT;k<=IDS_LANDSCAPE;k++)
	{
		temp.LoadString(k);
		m_ComboOrientation.AddString(temp);
	}
	m_ComboOrientation.SetCurSel(1);

	for(k=IDS_K_OF_YMCKO1;k<=IDS_K_OF_YMCKO3;k++)
	{
		temp.LoadString(k);
		m_ComboKfront.AddString(temp);
		m_ComboKback.AddString(temp);
	}
	m_ComboKfront.SetCurSel(0);
	m_ComboKback.SetCurSel(0);

	for(k=IDS_FEED_CARD1;k<=IDS_FEED_CARD4;k++)
	{
		temp.LoadString(k);
		m_ComboFeedCard.AddString(temp);
	}
	m_ComboFeedCard.SetCurSel(0);

	for(k=IDS_EJECT_CARD1;k<=IDS_EJECT_CARD6;k++)
	{
		temp.LoadString(k);
		m_ComboEjectCard.AddString(temp);
	}
	m_ComboEjectCard.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPrint::FillDocPropertyFromUI(SEAORY_DOC_PROP* lpDocProp)
{
	memset(lpDocProp, 0, sizeof(SEAORY_DOC_PROP));

	lpDocProp->byOrientation		= (BYTE)m_nOrientation + 1;

	if ( m_bModelRxxx )
	{
		if ( m_bSecurityEraseFront )		lpDocProp->bySecurityErase		|= 0x01;
		if ( m_bSecurityEraseBack )			lpDocProp->bySecurityErase		|= 0x10;
	}
	else
	{
		lpDocProp->byResolution			= (BYTE)m_nResolution;
		lpDocProp->byPaperSize			= (BYTE)m_nPaperSize;

		if ( m_nResolution == 1 )//300x600
			lpDocProp->by300x1200Mode = m_b300x1200Mode;
	}

	if ( m_bAutoDetectRibbon )
		lpDocProp->byAutoDetectRibbon	= 1;
	else
	{
		if ( m_bModelRxxx )
		{
			switch(m_nRibbonType)
			{
				case 0:		lpDocProp->byRibbonType = 100;		break;//YMCK
				case 1:		lpDocProp->byRibbonType = 101;		break;//YMCKK
				case 2:		lpDocProp->byRibbonType = 1;		break;//K
			}
		}
		else
			lpDocProp->byRibbonType			= (BYTE)m_nRibbonType;

		lpDocProp->byPrintPanelFront	= (BYTE)m_nFrontPanel;
		lpDocProp->byPrintPanelBack		= (BYTE)m_nBackPanel;
		lpDocProp->byResinKfront		= (BYTE)m_nFrontK;
		lpDocProp->byResinKback			= (BYTE)m_nBackK;
	}

	if ( m_bPrintFront )		lpDocProp->byPrintSide		|= 0x01;
	if ( m_bPrintBack )			lpDocProp->byPrintSide		|= 0x10;

	if ( m_bRotate180Front )	lpDocProp->byRotate180		|= 0x01;
	if ( m_bRotate180Back )		lpDocProp->byRotate180		|= 0x10;

	if ( m_bMirrorFront )		lpDocProp->byMirror		|= 0x01;
	if ( m_bMirrorBack )		lpDocProp->byMirror		|= 0x10;

	if ( m_bEraseCard )
		lpDocProp->byEraseCard		= 0x01;

	if ( m_bMonoSpeedMode )
		lpDocProp->byMonoSpeedMode	= 0x01;

	if ( m_bGrayYMC )
		lpDocProp->byGrayYMC		= 0x01;

	if ( m_bCardInOutByDev )
		lpDocProp->byCardInOutByDev	= 1;
	else
	{
		lpDocProp->byInputBin			= (BYTE)m_nFeedCard;

		lpDocProp->byFeedCardMode = 0;
		if ( m_bHookMode )
			lpDocProp->byFeedCardMode |= 0x01;
		if ( m_bHookRetry )
			lpDocProp->byFeedCardMode |= 0x02;

		lpDocProp->byOutputBin			= (BYTE)m_nEjectCard;
		lpDocProp->byEjectCardMode		= (BYTE)m_bWaitRemoval;
	}
}

void CDlgPrint::OnSelchangeComboPaperSize()
{
	// TODO: Add your control notification handler code here
	int					i = 0, k = 0;
	CString				temp;

	HWND				hDlg = this->GetSafeHwnd();

	i = m_ComboPaperSize.GetCurSel();

	if ( i == 0 )//CR-80
	{
		OnSelchangeComboFrontPanel();
		OnSelchangeComboBackPanel();

		GetDlgItem(IDC_CHECK_AUTO_DETECT_RIBBON)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RIBBON_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RESOLUTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PRINT_BACK)->EnableWindow(TRUE);
	}
	else if ( i == 1 )//54mm X 114mm
	{
		((CButton*)GetDlgItem(IDC_CHECK_AUTO_DETECT_RIBBON))->SetCheck(BST_UNCHECKED);
		m_ComboRibbon.SetCurSel(1);
		m_ComboResolution.SetCurSel(0);

		OnCheckAutoDetectRibbon();
		OnSelchangeComboRibbonType();

		if ( !m_bModelRxxx )
			OnSelchangeComboResolution();

		OnSelchangeComboFrontPanel();
		OnSelchangeComboBackPanel();

		((CButton*)GetDlgItem(IDC_CHECK_PRINT_BACK))->SetCheck(BST_UNCHECKED);

		GetDlgItem(IDC_CHECK_AUTO_DETECT_RIBBON)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RIBBON_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RESOLUTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PRINT_BACK)->EnableWindow(FALSE);
	}
	else if ( i == 2 )//A4
	{
		m_ComboResolution.SetCurSel(0);

		OnSelchangeComboResolution();
		OnSelchangeComboFrontPanel();
		OnSelchangeComboBackPanel();

		GetDlgItem(IDC_CHECK_AUTO_DETECT_RIBBON)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RIBBON_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_RESOLUTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PRINT_BACK)->EnableWindow(TRUE);
	}
}

void CDlgPrint::OnCheckAutoDetectRibbon()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL				bEnable = !m_bAutoDetectRibbon;

	GetDlgItem(IDC_COMBO_RIBBON_TYPE)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_FRONT_PANEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_BACK_PANEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_BACK_K_METHOD)->EnableWindow(bEnable);

	GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->EnableWindow(bEnable);
}

void CDlgPrint::OnSelchangeComboRibbonType()
{
	// TODO: Add your control notification handler code here
	int					m = 0, k = 0;
	CString				temp;

	m = m_ComboRibbon.GetCurSel();

	if ( m_bModelRxxx )
	{
		switch(m)
		{
			case 0:		m = 100;	break;//YMCK
			case 1:		m = 101;	break;//YMCKK
			case 2:		m = 1;		break;//K
		}
	}

	if ( m == 1 || m == 4 || m == 5 || m == 6 || m == 7 || m == 11 || m == 12 || m == 13 )//K,KO,Gold,Silver,White,UV,Red,Blue
	{
		GetDlgItem(IDC_COMBO_FRONT_PANEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_BACK_PANEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FRONT_PANEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_BACK_PANEL)->ShowWindow(SW_HIDE);

		m_ComboKfront.ResetContent();
		m_ComboKback.ResetContent();

		for(k=IDS_DITHER;k<=IDS_BLACK_WHITE;k++)
		{
			temp.LoadString(k);
			m_ComboKfront.AddString(temp);
			m_ComboKback.AddString(temp);
		}

		m_ComboKfront.SetCurSel(0);
		m_ComboKback.SetCurSel(0);
	}
	else
	{
		if ( !m_bAutoDetectRibbon )
		{
		GetDlgItem(IDC_COMBO_FRONT_PANEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_BACK_PANEL)->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_COMBO_FRONT_PANEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_BACK_PANEL)->ShowWindow(SW_SHOW);

		m_ComboKfront.ResetContent();
		m_ComboKback.ResetContent();

		for(k=IDS_K_OF_YMCKO1;k<=IDS_K_OF_YMCKO3;k++)
		{
			temp.LoadString(k);
			m_ComboKfront.AddString(temp);
			m_ComboKback.AddString(temp);
		}

		m_ComboKfront.SetCurSel(0);
		m_ComboKback.SetCurSel(0);
	}

	GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_SHOW);
	if ( m_bModelRxxx )
	{
	GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
	}
	else
	{
	GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
	}

	m_ComboFrontPanel.ResetContent();
	m_ComboBackPanel.ResetContent();
	if ( m == 10 )//RYMCK
	{
		m_ComboFrontPanel.AddString(_T("RYMCK"));
		m_ComboFrontPanel.AddString(_T("RYMC"));
		m_ComboFrontPanel.AddString(_T("K"));
		m_ComboFrontPanel.AddString(_T("RK"));
		m_ComboFrontPanel.AddString(_T("RYMCK+R"));
		m_ComboFrontPanel.AddString(_T("YMCK+R"));

		m_ComboBackPanel.AddString(_T("RYMCK"));
		m_ComboBackPanel.AddString(_T("RYMC"));
		m_ComboBackPanel.AddString(_T("K"));
		m_ComboBackPanel.AddString(_T("RK"));
		m_ComboBackPanel.AddString(_T("RYMCK+R"));
		m_ComboBackPanel.AddString(_T("YMCK+R"));
	}
	else if ( m == 100 || m == 101 )//YMCK,YMCKK
	{
		m_ComboFrontPanel.AddString(_T("YMCK"));
		m_ComboFrontPanel.AddString(_T("YMC"));
		m_ComboFrontPanel.AddString(_T("K"));

		m_ComboBackPanel.AddString(_T("YMCK"));
		m_ComboBackPanel.AddString(_T("YMC"));
		m_ComboBackPanel.AddString(_T("K"));
	}
	else
	{
		m_ComboFrontPanel.AddString(_T("YMCKO"));
		m_ComboFrontPanel.AddString(_T("YMCO"));
		m_ComboFrontPanel.AddString(_T("K"));
		m_ComboFrontPanel.AddString(_T("KO"));
		m_ComboFrontPanel.AddString(_T("YMCK"));
		m_ComboFrontPanel.AddString(_T("YMC"));

		m_ComboBackPanel.AddString(_T("YMCKO"));
		m_ComboBackPanel.AddString(_T("YMCO"));
		m_ComboBackPanel.AddString(_T("K"));
		m_ComboBackPanel.AddString(_T("KO"));
		m_ComboBackPanel.AddString(_T("YMCK"));
		m_ComboBackPanel.AddString(_T("YMC"));
	}

	m_ComboFrontPanel.SetCurSel(0);
	m_ComboBackPanel.SetCurSel(0);

	if ( m == 3 || m == 101 )//YMCKOK,YMCKK
	{
		((CButton*)GetDlgItem(IDC_CHECK_PRINT_BACK))->SetCheck(BST_CHECKED);
		m_ComboBackPanel.SetCurSel(2);

		m_ComboKback.ResetContent();
		for(k=IDS_DITHER;k<=IDS_BLACK_WHITE;k++)
		{
			temp.LoadString(k);
			m_ComboKfront.AddString(temp);
			m_ComboKback.AddString(temp);
		}
		m_ComboKback.SetCurSel(0);
	}
	else if ( m == 8 )//1/2ymcKOKO
	{
		((CButton*)GetDlgItem(IDC_CHECK_PRINT_BACK))->SetCheck(BST_CHECKED);
		m_ComboBackPanel.SetCurSel(3);

		m_ComboKback.ResetContent();
		for(k=IDS_DITHER;k<=IDS_BLACK_WHITE;k++)
		{
			temp.LoadString(k);
			m_ComboKfront.AddString(temp);
			m_ComboKback.AddString(temp);
		}
		m_ComboKback.SetCurSel(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_PRINT_BACK))->SetCheck(BST_UNCHECKED);
	}

	OnSelchangeComboFrontPanel();
	OnSelchangeComboBackPanel();
}

void CDlgPrint::OnSelchangeComboResolution()
{
	// TODO: Add your control notification handler code here
	int					i = 0, k = 0;
	CString				temp;

	i = m_ComboResolution.GetCurSel();

	if ( i == 0 )//300x300 DPI
	{
		GetDlgItem(IDC_CHECK_MONO_SPEED)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_300X1200)->ShowWindow(SW_HIDE);
	}
	else//300x600 DPI
	{
		((CButton*)GetDlgItem(IDC_CHECK_MONO_SPEED))->SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_CHECK_MONO_SPEED)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_300X1200)->ShowWindow(SW_SHOW);
	}
}

void CDlgPrint::OnSelchangeComboFrontPanel()
{
	// TODO: Add your control notification handler code here
	int					i = 0, k = 0, m = 0, n = 0;
	CString				temp;

	m = m_ComboRibbon.GetCurSel();
	n = m_ComboPaperSize.GetCurSel();
	i = m_ComboFrontPanel.GetCurSel();

	if ( m_bModelRxxx )
	{
		switch(m)
		{
			case 0:		m = 100;	break;//YMCK
			case 1:		m = 101;	break;//YMCKK
			case 2:		m = 1;		break;//K
		}
	}

	if ( m == 10 )//RYMCK
	{
		if ( i == 1 )//RYMC
		{
			GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
		}
		else if ( i == 2 || i == 3 )//K,RK
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKfront.ResetContent();
			for(k=IDS_DITHER;k<=IDS_BLACK_WHITE;k++)
			{
				temp.LoadString(k);
				m_ComboKfront.AddString(temp);
			}
			m_ComboKfront.SetCurSel(0);
		}
		else
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKfront.ResetContent();
			for(k=IDS_K_OF_YMCKO1;k<=IDS_K_OF_YMCKO3;k++)
			{
				temp.LoadString(k);
				m_ComboKfront.AddString(temp);
			}
			m_ComboKfront.SetCurSel(0);

			if ( n == 2 )//A4
			{
				m_ComboKfront.SetCurSel(2);
				GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
			}
		}
	}
	else if ( m == 1 || m == 4 || m == 5 || m == 6 || m == 7 || m == 11 || m == 12 || m == 13 )//K,KO,Gold,Silver,White,UV,Red,Blue
	{


	}
	else
	{
		if ( i == 1 || i == 5 )//YMCO,YMC
		{
			GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
		}
		else if ( i == 2 || i == 3 )//K,KO
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKfront.ResetContent();
			for(k=IDS_DITHER;k<=IDS_BLACK_WHITE;k++)
			{
				temp.LoadString(k);
				m_ComboKfront.AddString(temp);
			}
			m_ComboKfront.SetCurSel(0);
		}
		else
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKfront.ResetContent();
			for(k=IDS_K_OF_YMCKO1;k<=IDS_K_OF_YMCKO3;k++)
			{
				temp.LoadString(k);
				m_ComboKfront.AddString(temp);
			}
			m_ComboKfront.SetCurSel(0);

			if ( n == 2 )//A4
			{
				m_ComboKfront.SetCurSel(2);
				GetDlgItem(IDC_COMBO_FRONT_K_METHOD)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHECK_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_FRONT)->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CDlgPrint::OnSelchangeComboBackPanel()
{
	// TODO: Add your control notification handler code here
	int					i = 0, k = 0, m = 0, n = 0;
	CString				temp;

	m = m_ComboRibbon.GetCurSel();
	n = m_ComboPaperSize.GetCurSel();
	i = m_ComboBackPanel.GetCurSel();

	if ( m_bModelRxxx )
	{
		switch(m)
		{
			case 0:		m = 100;	break;//YMCK
			case 1:		m = 101;	break;//YMCKK
			case 2:		m = 1;		break;//K
		}
	}

	if ( m == 10 )//RYMCK
	{
		if ( i == 1 )//RYMC
		{
			GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
		}
		else if ( i == 2 || i == 3 )//K,RK
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKback.ResetContent();
			for(k=IDS_DITHER;k<=IDS_BLACK_WHITE;k++)
			{
				temp.LoadString(k);
				m_ComboKback.AddString(temp);
			}
			m_ComboKback.SetCurSel(0);
		}
		else
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKback.ResetContent();
			for(k=IDS_K_OF_YMCKO1;k<=IDS_K_OF_YMCKO3;k++)
			{
				temp.LoadString(k);
				m_ComboKback.AddString(temp);
			}
			m_ComboKback.SetCurSel(0);

			if ( n == 2 )//A4
			{
				m_ComboKback.SetCurSel(2);
				GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
			}
		}
	}
	else if ( m == 1 || m == 4 || m == 5 || m == 6 || m == 7 || m == 11 || m == 12 || m == 13 )//K,KO,Gold,Silver,White,UV,Red,Blue
	{


	}
	else
	{
		if ( i == 1 || i == 5 )//YMCO,YMC
		{
			GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
		}
		else if ( i == 2 || i == 3 )//K,KO
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKback.ResetContent();
			for(k=IDS_DITHER;k<=IDS_BLACK_WHITE;k++)
			{
				temp.LoadString(k);
				m_ComboKback.AddString(temp);
			}
			m_ComboKback.SetCurSel(0);
		}
		else
		{
			if ( !m_bAutoDetectRibbon )
			{
				GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_SHOW);
				if ( m_bModelRxxx )
				{
				GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_SHOW);
				}
			}

			m_ComboKback.ResetContent();
			for(k=IDS_K_OF_YMCKO1;k<=IDS_K_OF_YMCKO3;k++)
			{
				temp.LoadString(k);
				m_ComboKback.AddString(temp);
			}
			m_ComboKback.SetCurSel(0);

			if ( n == 2 )//A4
			{
				m_ComboKback.SetCurSel(2);
				GetDlgItem(IDC_COMBO_BACK_K_METHOD)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_CHECK_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_BTN_BROWSE_SECURITY_BMP_BACK)->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CDlgPrint::OnCheckUseDeviceDefaultCardInOut()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL		bEnable = !m_bCardInOutByDev;

	GetDlgItem(IDC_COMBO_FEED_CARD)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_HOOK_MODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_RETRY_BY_HOOK_MODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_EJECT_CARD)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_WAIT_FOR_REMOVAL)->EnableWindow(bEnable);
}

void CDlgPrint::OnSelchangeComboFeedCard()
{
	// TODO: Add your control notification handler code here
	int					i = 0;
	int					nShow = SW_SHOW;

	if ( m_bModelRxxx )
		return;

	i = m_ComboFeedCard.GetCurSel();

	if ( i != 0 )
		nShow = SW_HIDE;

	GetDlgItem(IDC_CHECK_HOOK_MODE)->ShowWindow(nShow);
	GetDlgItem(IDC_CHECK_RETRY_BY_HOOK_MODE)->ShowWindow(nShow);
}

void CDlgPrint::OnSelchangeComboEjectCard()
{
	// TODO: Add your control notification handler code here
	int					i = 0;
	int					nShow = SW_SHOW;

	i = m_ComboEjectCard.GetCurSel();

	if ( i == 3 || i == 4 )
		nShow = SW_HIDE;

	GetDlgItem(IDC_CHECK_WAIT_FOR_REMOVAL)->ShowWindow(nShow);
}

void CDlgPrint::OnBtnGetPrinterSetting()
{
	// TODO: Add your control notification handler code here
	SEAORY_DOC_PROP		docProp = {0};

	DWORD				dwRet = 0;

	int					i = 0;

	BOOL				bEnable = TRUE;

	TCHAR				szTemp[256] = {0};

	//UpdateData(TRUE);

	if ( m_szPrinterName[0] == 0 )
	{
		MessageBox(_T("Please select a printer."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	dwRet = SOY_PR_GetPrinterSetting(m_szPrinterName, (BYTE*)&docProp);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnGetPrinterSetting"));
		return;
	}

	if ( docProp.byAutoDetectRibbon == 0 )
		m_bAutoDetectRibbon	= FALSE;
	else
		m_bAutoDetectRibbon	= TRUE;

	m_nOrientation			= docProp.byOrientation - 1;

	if ( m_bModelRxxx )
	{
		switch(docProp.byRibbonType)
		{
			case 100:		m_nRibbonType = 0;		break;//YMCK
			case 101:		m_nRibbonType = 1;		break;//YMCKK
			case 1:			m_nRibbonType = 2;		break;//K
		}

		if ( docProp.bySecurityErase & 0x01 )
			m_bSecurityEraseFront = TRUE;
		else
			m_bSecurityEraseFront = FALSE;

		if ( docProp.bySecurityErase & 0x10 )
			m_bSecurityEraseBack = TRUE;
		else
			m_bSecurityEraseBack = FALSE;

		memset(szTemp, 0, sizeof(TCHAR)*256);
		GetSecurityBmpFileName(m_szPrinterName, 0, szTemp);
		GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->SetWindowText(szTemp);

		memset(szTemp, 0, sizeof(TCHAR)*256);
		GetSecurityBmpFileName(m_szPrinterName, 1, szTemp);
		GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->SetWindowText(szTemp);
	}
	else
	{
		m_nResolution		= docProp.byResolution;
		m_nRibbonType		= docProp.byRibbonType;
		m_nPaperSize		= docProp.byPaperSize;

		if ( m_nResolution == 1 )//300x600
			m_b300x1200Mode = docProp.by300x1200Mode;
	}

	if ( docProp.byPrintSide & 0x01 )
		m_bPrintFront = TRUE;
	else
		m_bPrintFront = FALSE;

	if ( docProp.byPrintSide & 0x10 )
		m_bPrintBack = TRUE;
	else
		m_bPrintBack = FALSE;

	m_nFrontPanel			= docProp.byPrintPanelFront;
	m_nBackPanel			= docProp.byPrintPanelBack;
	m_nFrontK				= docProp.byResinKfront;
	m_nBackK				= docProp.byResinKback;

	if ( docProp.byRotate180 & 0x01 )
		m_bRotate180Front = TRUE;
	else
		m_bRotate180Front = FALSE;

	if ( docProp.byRotate180 & 0x10 )
		m_bRotate180Back = TRUE;
	else
		m_bRotate180Back = FALSE;

	if ( docProp.byMirror & 0x01 )
		m_bMirrorFront = TRUE;
	else
		m_bMirrorFront = FALSE;

	if ( docProp.byMirror & 0x10 )
		m_bMirrorBack = TRUE;
	else
		m_bMirrorBack = FALSE;

	if ( docProp.byEraseCard & 0x01 )
		m_bEraseCard = TRUE;
	else
		m_bEraseCard = FALSE;

	if ( docProp.byMonoSpeedMode == 0 )
		m_bMonoSpeedMode	= FALSE;
	else
		m_bMonoSpeedMode	= TRUE;

	if ( docProp.byGrayYMC == 0 )
		m_bGrayYMC	= FALSE;
	else
		m_bGrayYMC	= TRUE;

	m_bCardInOutByDev		= docProp.byCardInOutByDev;

	m_nFeedCard				= docProp.byInputBin;

	if ( docProp.byFeedCardMode & 0x01 )
		m_bHookMode = TRUE;
	else
		m_bHookMode = FALSE;

	if ( docProp.byFeedCardMode & 0x02 )
		m_bHookRetry = TRUE;
	else
		m_bHookRetry = FALSE;

	m_nEjectCard			= docProp.byOutputBin;
	m_bWaitRemoval			= docProp.byEjectCardMode;

	UpdateData(FALSE);

	OnSelchangeComboRibbonType();
	if ( !m_bModelRxxx )
		OnSelchangeComboPaperSize();
	OnCheckAutoDetectRibbon();

	m_ComboFrontPanel.SetCurSel(docProp.byPrintPanelFront);
	m_ComboBackPanel.SetCurSel(docProp.byPrintPanelBack);

	OnSelchangeComboFrontPanel();
	OnSelchangeComboBackPanel();

	m_ComboKfront.SetCurSel(docProp.byResinKfront);
	m_ComboKback.SetCurSel(docProp.byResinKback);

	if ( m_bPrintBack )
		((CButton*)GetDlgItem(IDC_CHECK_PRINT_BACK))->SetCheck(BST_CHECKED);
	else
		((CButton*)GetDlgItem(IDC_CHECK_PRINT_BACK))->SetCheck(BST_UNCHECKED);

	OnCheckUseDeviceDefaultCardInOut();

	if ( dwRet != 0 )
	ShowResultMessage(dwRet, _T("OnBtnGetPrinterSetting"));
}

void CDlgPrint::OnBtnSetPrinterSetting()
{
	// TODO: Add your control notification handler code here
	SEAORY_DOC_PROP		docProp = {0};

	DWORD				dwRet = 0;
	TCHAR				szTemp[256] = {0};

	UpdateData(TRUE);

	if ( m_szPrinterName[0] == 0 )
	{
		MessageBox(_T("Please select a printer."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	FillDocPropertyFromUI(&docProp);

	if ( m_bModelRxxx )
	{
		memset(szTemp, 0, sizeof(TCHAR)*256);
		GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->GetWindowText(szTemp, 256);
		SetSecurityBmpFileName(m_szPrinterName, 0, szTemp);

		memset(szTemp, 0, sizeof(TCHAR)*256);
		GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->GetWindowText(szTemp, 256);
		SetSecurityBmpFileName(m_szPrinterName, 1, szTemp);
	}

	dwRet = SOY_PR_SetPrinterSetting(m_szPrinterName, (BYTE*)&docProp);

	if ( dwRet != 0 )
	ShowResultMessage(dwRet, _T("OnBtnSetPrinterSetting"));
}

void CDlgPrint::OnBtnBrowse()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("Image Files(*.BMP;*.JPG;*.GIF;*.PNG)\0*.BMP;*.JPG;*.GIF;*.PNG\0\0");
	//SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP(*.bmp)\0*.bmp\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_IMG_FRONT)->SetWindowText(OpenFileName);
	}
}

void CDlgPrint::OnBtnBrowse2()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("Image Files(*.BMP;*.JPG;*.GIF;*.PNG)\0*.BMP;*.JPG;*.GIF;*.PNG\0\0");
	//SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP(*.bmp)\0*.bmp\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_IMG_BACK)->SetWindowText(OpenFileName);
	}
}

void CDlgPrint::OnBtnBrowseK()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("Image Files(*.BMP;*.JPG;*.GIF;*.PNG)\0*.BMP;*.JPG;*.GIF;*.PNG\0\0");
	//SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP(*.bmp)\0*.bmp\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_IMG_FRONT_K)->SetWindowText(OpenFileName);
	}
}

void CDlgPrint::OnBtnBrowseK2()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("Image Files(*.BMP;*.JPG;*.GIF;*.PNG)\0*.BMP;*.JPG;*.GIF;*.PNG\0\0");
	//SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP(*.bmp)\0*.bmp\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_IMG_BACK_K)->SetWindowText(OpenFileName);
	}
}

void CDlgPrint::OnBtnGeneralPrint()
{
	DWORD				dwStatus = 0,
						dwError = 0;
	SEAORY_DOC_PROP		docProp = {0};

	TCHAR				szJobName[128] = {0};
	HDC					hDC = 0;
	DOCINFO				DocInfo;
	int					nJobId = 0;
	BITMAPINFO			BmpInfo = {0};
	int					nRet = 0;

	HBITMAP				hBmp24 = 0;
	BITMAP				Bmp24 = {0};

	HBITMAP				hBmp24b = 0;
	BITMAP				Bmp24b = {0};

	HBITMAP				hBmpK = 0;
	BITMAP				BmpK = {0};

	HBITMAP				hBmpKb = 0;
	BITMAP				BmpKb = {0};

	LOGFONT				LogFont = {0};
	HFONT				hFont = 0,
						hDefFont = 0;
	int					nOldMapMode = 0;
	int					nResX = 0, nResY = 0;

	DWORD				dwRet = 0;

	int					i = 0;

	CString				csExt;

	UpdateData(TRUE);

	if ( m_szPrinterName[0] == 0 )
	{
		MessageBox(_T("Please select a printer."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( !m_bPrintFront && !m_bPrintBack )
	{
		MessageBox(_T("Please select at least one side to print."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}


	//check if printer is locked
	TCHAR				szTemp[32] = {0};
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_SECURITY_MODE, szTemp);
	if ( dwRet == 0 && lstrcmp(szTemp, _T("1")) == 0 )
	{
		ShowResultMessage(0x00013005, _T("OnBtnGeneralPrint"));//Printer is locked.
		return;
	}

	//load image from file
	if ( m_bPrintFront )
	{
		if ( !m_csBmpFront.IsEmpty() )
		{
			csExt = m_csBmpFront.Mid(m_csBmpFront.GetLength() - 3, 3);
			if ( csExt.CompareNoCase(_T("BMP")) != 0 )
			{
				MessageBox(_T("Please select a BMP file."), _T("Front Color Image"), MB_OK|MB_ICONERROR);
				return;
			}

			hBmp24 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpFront, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			dwError = GetLastError();
			GetObject(hBmp24, sizeof(BITMAP), &Bmp24);
		}

		if ( !m_csBmpFrontK.IsEmpty() )
		{
			csExt = m_csBmpFrontK.Mid(m_csBmpFrontK.GetLength() - 3, 3);
			if ( csExt.CompareNoCase(_T("BMP")) != 0 )
			{
				MessageBox(_T("Please select a BMP file."), _T("Front K Image"), MB_OK|MB_ICONERROR);
				return;
			}

			hBmpK = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpFrontK, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			dwError = GetLastError();
			GetObject(hBmpK, sizeof(BITMAP), &BmpK);
		}
	}

	if ( m_bPrintBack )
	{
		if ( !m_csBmpBack.IsEmpty() )
		{
			csExt = m_csBmpBack.Mid(m_csBmpBack.GetLength() - 3, 3);
			if ( csExt.CompareNoCase(_T("BMP")) != 0 )
			{
				MessageBox(_T("Please select a BMP file."), _T("Back Color Image"), MB_OK|MB_ICONERROR);
				return;
			}

			hBmp24b = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpBack, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			dwError = GetLastError();
			GetObject(hBmp24b, sizeof(BITMAP), &Bmp24b);
		}

		if ( !m_csBmpBackK.IsEmpty() )
		{
			csExt = m_csBmpBackK.Mid(m_csBmpBackK.GetLength() - 3, 3);
			if ( csExt.CompareNoCase(_T("BMP")) != 0 )
			{
				MessageBox(_T("Please select a BMP file."), _T("Back K Image"), MB_OK|MB_ICONERROR);
				return;
			}

			hBmpKb = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpBackK, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			dwError = GetLastError();
			GetObject(hBmpKb, sizeof(BITMAP), &BmpKb);
		}
	}

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//1. optional printer settings
	FillDocPropertyFromUI(&docProp);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2. CreateDC
	hDC = CreateDC(_T("WINSPOOL"), m_szPrinterName, NULL, 0);

	//3. Optional. Apply the job setting into the DC.
	//   Please notice that this function will get the printer's current setting,
	//   and then combine the settings value with docProp values.
	//   Please do not use this function when you want to use the printer's current Printing Preferences.
	dwError = SOY_PR_ModifyDocumentProperties(m_szPrinterName, hDC, (BYTE*)&docProp, 0);
	if ( dwError )
	{
		::DeleteDC(hDC);
		ShowResultMessage(dwError, _T("OnBtnGeneralPrint"));
		return;
	}

	nResX = GetDeviceCaps(hDC, LOGPIXELSX);
	nResY = GetDeviceCaps(hDC, LOGPIXELSY);
	if ( nResX < nResY )
		nResY = nResX;

	//4. StartDoc
	memset(&DocInfo, 0, sizeof(DOCINFO));
	DocInfo.cbSize = sizeof(DOCINFO);

	lstrcpy(szJobName, _T("SeaoryDemo VC++ General Print Job"));
	DocInfo.lpszDocName = szJobName;

	nJobId = ::StartDoc(hDC, &DocInfo);

	//-------------------------------------------------------------------------
	//front side
	//-------------------------------------------------------------------------
	if ( m_bPrintFront )
	{
		//5. StartPage
		nRet = ::StartPage(hDC);

		//6. Draw anything you want on page between ::StartPage and ::EndPage
		if ( Bmp24.bmBits )
		{
			memset(&BmpInfo, 0, sizeof(BITMAPINFO));
			BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			BmpInfo.bmiHeader.biWidth = Bmp24.bmWidth;
			BmpInfo.bmiHeader.biHeight = Bmp24.bmHeight;
			BmpInfo.bmiHeader.biPlanes = Bmp24.bmPlanes;
			BmpInfo.bmiHeader.biBitCount = Bmp24.bmBitsPixel * Bmp24.bmPlanes;
			BmpInfo.bmiHeader.biCompression = BI_RGB;

			if ( m_nWidth == 0 )	m_nWidth = Bmp24.bmWidth;
			if ( m_nHeight == 0 )	m_nHeight = Bmp24.bmHeight;
			nRet = StretchDIBits(hDC, m_nImgX, m_nImgY, m_nWidth, m_nHeight, 0, 0, Bmp24.bmWidth, Bmp24.bmHeight, Bmp24.bmBits, &BmpInfo, DIB_RGB_COLORS, SRCCOPY);
 		}

		if ( BmpK.bmBits )
		{
			memset(&BmpInfo, 0, sizeof(BITMAPINFO));
			BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			BmpInfo.bmiHeader.biWidth = BmpK.bmWidth;
			BmpInfo.bmiHeader.biHeight = BmpK.bmHeight;
			BmpInfo.bmiHeader.biPlanes = BmpK.bmPlanes;
			BmpInfo.bmiHeader.biBitCount = BmpK.bmBitsPixel * BmpK.bmPlanes;
			BmpInfo.bmiHeader.biCompression = BI_RGB;

			if ( m_nImgKWidth == 0 )	m_nImgKWidth = BmpK.bmWidth;
			if ( m_nImgKHeight == 0 )	m_nImgKHeight = BmpK.bmHeight;
			nRet = StretchDIBits(hDC, m_nImgKX, m_nImgKY, m_nImgKWidth, m_nImgKHeight, 0, 0, BmpK.bmWidth, BmpK.bmHeight, BmpK.bmBits, &BmpInfo, DIB_RGB_COLORS, SRCCOPY);
 		}

		if ( !m_csText.IsEmpty() )
		{
			memset((LPBYTE)&LogFont, 0, sizeof(LOGFONT));

			LogFont.lfHeight = -MulDiv(m_nFontSize, nResY, 72);//unit=pixel
			LogFont.lfWeight = FW_NORMAL;
			LogFont.lfCharSet = DEFAULT_CHARSET;
			//LogFont.lfQuality = ANTIALIASED_QUALITY;
			lstrcpy(LogFont.lfFaceName, (LPCTSTR)m_csFontName);

			hFont = ::CreateFontIndirect(&LogFont);

			hDefFont = (HFONT)::SelectObject(hDC, hFont);

			::SetTextColor(hDC, 0x00000000);
			::TextOut(hDC, m_nTxtX, m_nTxtY, (LPCTSTR)m_csText, m_csText.GetLength());

			::SelectObject(hDC, hDefFont);
			DeleteObject(hFont);
		}

		//7. If page drawing is finished, set to end page
		nRet = ::EndPage(hDC);
	}

	//-------------------------------------------------------------------------
	//back side
	//-------------------------------------------------------------------------
	if ( m_bPrintBack )
	{
		//5. StartPage
		nRet = ::StartPage(hDC);

		//6. Draw anything you want on page between ::StartPage and ::EndPage
		if ( Bmp24b.bmBits )
		{
			memset(&BmpInfo, 0, sizeof(BITMAPINFO));
			BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			BmpInfo.bmiHeader.biWidth = Bmp24b.bmWidth;
			BmpInfo.bmiHeader.biHeight = Bmp24b.bmHeight;
			BmpInfo.bmiHeader.biPlanes = Bmp24b.bmPlanes;
			BmpInfo.bmiHeader.biBitCount = Bmp24b.bmBitsPixel * Bmp24b.bmPlanes;
			BmpInfo.bmiHeader.biCompression = BI_RGB;

			if ( m_nWidth2 == 0 )	m_nWidth2 = Bmp24b.bmWidth;
			if ( m_nHeight2 == 0 )	m_nHeight2 = Bmp24b.bmHeight;
			nRet = StretchDIBits(hDC, m_nImgX2, m_nImgY2, m_nWidth2, m_nHeight2, 0, 0, Bmp24b.bmWidth, Bmp24b.bmHeight, Bmp24b.bmBits, &BmpInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		if ( BmpKb.bmBits )
		{
			memset(&BmpInfo, 0, sizeof(BITMAPINFO));
			BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			BmpInfo.bmiHeader.biWidth = BmpKb.bmWidth;
			BmpInfo.bmiHeader.biHeight = BmpKb.bmHeight;
			BmpInfo.bmiHeader.biPlanes = BmpKb.bmPlanes;
			BmpInfo.bmiHeader.biBitCount = BmpKb.bmBitsPixel * BmpKb.bmPlanes;
			BmpInfo.bmiHeader.biCompression = BI_RGB;

			if ( m_nImgKWidth2 == 0 )	m_nImgKWidth2 = BmpKb.bmWidth;
			if ( m_nImgKHeight2 == 0 )	m_nImgKHeight2 = BmpKb.bmHeight;
			nRet = StretchDIBits(hDC, m_nImgKX2, m_nImgKY2, m_nImgKWidth2, m_nImgKHeight2, 0, 0, BmpKb.bmWidth, BmpKb.bmHeight, BmpKb.bmBits, &BmpInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		if ( !m_csText2.IsEmpty() )
		{
			memset((LPBYTE)&LogFont, 0, sizeof(LOGFONT));
			LogFont.lfHeight = -MulDiv(m_nFontSize2, nResY, 72);//unit=pixel
			LogFont.lfWeight = FW_NORMAL;
			LogFont.lfCharSet = DEFAULT_CHARSET;
			lstrcpy(LogFont.lfFaceName, (LPCTSTR)m_csFontName2);
			hFont = ::CreateFontIndirect(&LogFont);

			hDefFont = (HFONT)::SelectObject(hDC, hFont);

			::SetTextColor(hDC, 0x00000000);
			::TextOut(hDC, m_nTxtX2, m_nTxtY2, (LPCTSTR)m_csText2, m_csText2.GetLength());
			::SelectObject(hDC, hDefFont);
			DeleteObject(hFont);
		}

		//7. If page drawing is finished, set to end page
		nRet = ::EndPage(hDC);
	}

	//8. If job done, set end of the job
	nRet = ::EndDoc(hDC);

	::DeleteDC(hDC);

	//release image object
	if ( hBmp24 )
		DeleteObject(hBmp24);

	if ( hBmp24b )
		DeleteObject(hBmp24b);

	if ( hBmpK )
		DeleteObject(hBmpK);

	if ( hBmpKb )
		DeleteObject(hBmpKb);

	//If you need to do another encoding, please wait printer to print card completely.
//	dwError = WaitPrintingFinished(m_szPrinterName);
//	ShowResultMessage(dwError, _T("OnBtnGeneralPrint"));
}

void CDlgPrint::OnBtnSimplePrint()
{
	DWORD				dwStatus = 0,
						dwError = 0;
	SEAORY_DOC_PROP		docProp = {0};

	HDC					hPrinterDC = 0;
	DWORD				dwRet = 0;
	BOOL				bCancelJob = FALSE;

	int					i = 0;


	UpdateData(TRUE);

	if ( m_szPrinterName[0] == 0 )
	{
		MessageBox(_T("Please select a printer."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( !m_bPrintFront && !m_bPrintBack )
	{
		MessageBox(_T("Please select at least one side to print."), _T("Error"), MB_OK|MB_ICONERROR);
		return;
	}


	//check if printer is locked
	TCHAR				szTemp[32] = {0};
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_SECURITY_MODE, szTemp);
	if ( dwRet == 0 && lstrcmp(szTemp, _T("1")) == 0 )
	{
		ShowResultMessage(0x00013005, _T("OnBtnSimplePrint"));//Printer is locked.
		return;
	}

	FillDocPropertyFromUI(&docProp);

	dwRet = SOY_PR_StartPrinting2(m_szPrinterName, (BYTE*)&docProp, &hPrinterDC);
	if ( dwRet != 0 )
	{
		ShowResultMessage(dwRet, _T("OnBtnSimplePrint"));
		return;
	}

	if ( m_bPrintFront )
	{
		dwRet = SOY_PR_StartPage2(hPrinterDC);

		if ( dwRet == 0 && !m_csBmpFront.IsEmpty() )
			dwRet = SOY_PR_PrintImage2(hPrinterDC, m_nImgX, m_nImgY, m_nWidth, m_nHeight, (LPTSTR)(LPCTSTR)m_csBmpFront);

		if ( dwRet == 0 && !m_csBmpFrontK.IsEmpty() )
			dwRet = SOY_PR_PrintImage2(hPrinterDC, m_nImgKX, m_nImgKY, m_nImgKWidth, m_nImgKHeight, (LPTSTR)(LPCTSTR)m_csBmpFrontK);

		if ( dwRet == 0 && !m_csText.IsEmpty() )
			dwRet = SOY_PR_PrintText2(hPrinterDC, m_nTxtX, m_nTxtY, (LPTSTR)(LPCTSTR)m_csText, (LPTSTR)(LPCTSTR)m_csFontName, m_nFontSize, 400, 0, 0, 0x00000000, FALSE);

		if ( dwRet == 0 )
			dwRet = SOY_PR_EndPage2(hPrinterDC);
	}

	if ( m_bPrintBack )
	{
		if ( dwRet == 0 )
			dwRet = SOY_PR_StartPage2(hPrinterDC);

		if ( dwRet == 0 && !m_csBmpBack.IsEmpty() )
			dwRet = SOY_PR_PrintImage2(hPrinterDC, m_nImgX2, m_nImgY2, m_nWidth2, m_nHeight2, (LPTSTR)(LPCTSTR)m_csBmpBack);

		if ( dwRet == 0 && !m_csBmpBackK.IsEmpty() )
			dwRet = SOY_PR_PrintImage2(hPrinterDC, m_nImgKX2, m_nImgKY2, m_nImgKWidth2, m_nImgKHeight2, (LPTSTR)(LPCTSTR)m_csBmpBackK);

		if ( dwRet == 0 && !m_csText2.IsEmpty() )
			dwRet = SOY_PR_PrintText2(hPrinterDC, m_nTxtX2, m_nTxtY2, (LPTSTR)(LPCTSTR)m_csText2, (LPTSTR)(LPCTSTR)m_csFontName2, m_nFontSize2, 400, 0, 0, 0x00000000, FALSE);

		if ( dwRet == 0 )
			dwRet = SOY_PR_EndPage2(hPrinterDC);
	}

	dwError = dwRet;
	if ( dwRet != 0 )
		bCancelJob = TRUE;
	dwRet = SOY_PR_EndPrinting2(hPrinterDC, bCancelJob);

	if ( dwError != 0 )
	{
		ShowResultMessage(dwError, _T("OnBtnSimplePrint"));
	}
	else
	{
	//If you need to do another encoding, please wait printer to print card completely.
//	dwError = WaitPrintingFinished(m_szPrinterName);
//	ShowResultMessage(dwError, _T("OnBtnSimplePrint"));
	}
}

DWORD CDlgPrint::WaitPrintingFinished(TCHAR *szPrinterName)
{
	DWORD				dwRet = 0;
	uint32_t			dwStatus = 0;

	dwRet = SOY_PR_WaitPrinterBusy(m_szPrinterName, &dwStatus);

/*
	//wait printer to print card completely
	Sleep(5000);
	while ( 1 )
	{
		dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);

		if ( dwRet != 0 )
			dwStatus = dwRet;

		if ( dwStatus != 170 )//busy
			break;

		Sleep(1000);
	}
*/
	return dwStatus;
}

void CDlgPrint::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
{
	lstrcpy(m_szPrinterName, pPrinterName);
	lstrcpy(m_szDriverName, pDriverName);
	m_bModelRxxx = FALSE;

	if ( !pDriverName || pDriverName[0] == 0 )
	{
		for(int i=IDC_CHECK_AUTO_DETECT_RIBBON;i<=IDC_CHECK_300X1200;i++)
			GetDlgItem(i)->EnableWindow(FALSE);

		return;
	}
	else
	{
		for(int i=IDC_CHECK_AUTO_DETECT_RIBBON;i<=IDC_CHECK_300X1200;i++)
			GetDlgItem(i)->EnableWindow(TRUE);
	}

	int					nShow = SW_SHOW;
	if ( !lstrcmp(m_szDriverName, _T("Seaory R300")) || !lstrcmp(m_szDriverName, _T("Seaory R600")) || !lstrcmp(m_szDriverName, _T("Seaory R600M")) || !lstrcmp(m_szDriverName, _T("EPT-W3(AX)")) || !lstrcmp(m_szDriverName, _T("MS-DC600G")) || !lstrcmp(m_szDriverName, _T("Goldpac DCE905")) || !lstrcmp(m_szDriverName, _T("CTD DR600")) || !lstrcmp(m_szDriverName, _T("TSZ-T1 D600")) || !lstrcmp(m_szDriverName, _T("Kingaotech KT-R86")) || !lstrcmp(m_szDriverName, _T("TSZ-T1 D600-Q")) || !lstrcmp(m_szDriverName, _T("Imagedec R5000")) || !lstrcmp(m_szDriverName, _T("Seaory R330")) || !lstrcmp(m_szDriverName, _T("Seaory R660")) || !lstrcmp(m_szDriverName, _T("Seaory E600")) || !lstrcmp(m_szDriverName, _T("Seaory E330")) || !lstrcmp(m_szDriverName, _T("Fagoo P63XX")) )
	{
		nShow = SW_HIDE;
		m_bModelRxxx = TRUE;
	}

	m_ComboRibbon.ResetContent();
	if ( m_bModelRxxx )
	{
		m_ComboRibbon.AddString(_T("YMCK"));
		m_ComboRibbon.AddString(_T("YMCKK"));
		m_ComboRibbon.AddString(_T("K"));
	}
	else
	{
		m_ComboRibbon.AddString(_T("YMCKO"));
		m_ComboRibbon.AddString(_T("K"));
		m_ComboRibbon.AddString(_T("1/2 ymcKO"));
		m_ComboRibbon.AddString(_T("YMCKOK"));
		m_ComboRibbon.AddString(_T("KO"));
		m_ComboRibbon.AddString(_T("Gold"));
		m_ComboRibbon.AddString(_T("Silver"));
		m_ComboRibbon.AddString(_T("White"));
		m_ComboRibbon.AddString(_T("1/2 ymcKOKO"));
		m_ComboRibbon.AddString(_T("1/2 ymcKO-n"));
		m_ComboRibbon.AddString(_T("RYMCK"));
		m_ComboRibbon.AddString(_T("UV"));
		m_ComboRibbon.AddString(_T("Red"));
		m_ComboRibbon.AddString(_T("Blue"));
	}

	m_ComboPaperSize.ShowWindow(nShow);
	m_ComboResolution.ShowWindow(nShow);

	GetDlgItem(IDC_CHECK_HOOK_MODE)->ShowWindow(nShow);
	GetDlgItem(IDC_CHECK_RETRY_BY_HOOK_MODE)->ShowWindow(nShow);
	GetDlgItem(IDC_CHECK_ERASE_BEFORE_PRINT)->ShowWindow(nShow);
	GetDlgItem(IDC_CHECK_MONO_SPEED)->ShowWindow(nShow);

	for(int i=IDC_STATIC_PRINT_AREA;i<=IDC_BTN_SET_PRINT_AREA;i++)
		GetDlgItem(i)->ShowWindow(nShow);


	TCHAR				szPath[512] = {0};
	GetModuleFileName(NULL, szPath, 512);
	*_tcsrchr(szPath, '\\') = '\0';

	m_csBmpFront  = szPath;
	m_csBmpBack   = szPath;
	m_csBmpFrontK = szPath;
	m_csBmpBackK  = szPath;

	if ( !lstrcmp(m_szDriverName, _T("Seaory R300")) || !lstrcmp(m_szDriverName, _T("TSZ-T1 D600-Q")) || !lstrcmp(m_szDriverName, _T("Imagedec R5000")) || !lstrcmp(m_szDriverName, _T("Seaory R330")) || !lstrcmp(m_szDriverName, _T("Seaory E330")) || !lstrcmp(m_szDriverName, _T("Fagoo P63XX")) )
	{
		m_csBmpFront += _T("\\1030x648_bgr1.bmp");
		m_csBmpBack  += _T("\\1030x648_bgr2.bmp");
	}
	else if ( m_bModelRxxx )
	{
		m_csBmpFront += _T("\\2060x1296_bgr1.bmp");
		m_csBmpBack  += _T("\\2060x1296_bgr2.bmp");
	}
	else
	{
		m_csBmpFront += _T("\\1012x648_bgr1.bmp");
		m_csBmpBack  += _T("\\1012x648_bgr2.bmp");
	}

	m_csBmpFrontK += _T("\\barcode1D.bmp");
	m_csBmpBackK  += _T("\\barcode2D.bmp");

	if ( !lstrcmp(m_szDriverName, _T("Seaory S20R")) || !lstrcmp(m_szDriverName, _T("Seaory S22R")) )
	{
		GetDlgItem(IDC_COMBO_RIBBON_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ERASE_BEFORE_PRINT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_COMBO_RIBBON_TYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_ERASE_BEFORE_PRINT)->ShowWindow(SW_HIDE);
	}

	OnBtnGetPrinterSetting();
	OnSelchangeComboResolution();

	if ( !m_bModelRxxx )
	OnBtnGetPrintArea();

}

void CDlgPrint::OnBtnGetPrintArea()
{
	// TODO: Add your control notification handler code here
	m_ComboAreaFrontYMCO.ResetContent();
	m_ComboAreaFrontK.ResetContent();
	m_ComboAreaFrontRewrite.ResetContent();
	m_ComboAreaBackYMCO.ResetContent();
	m_ComboAreaBackK.ResetContent();

	m_ComboAreaFrontYMCO.SetCurSel(-1);
	m_ComboAreaFrontK.SetCurSel(-1);
	m_ComboAreaFrontRewrite.SetCurSel(-1);
	m_ComboAreaBackYMCO.SetCurSel(-1);
	m_ComboAreaBackK.SetCurSel(-1);

	uint32_t			dwRet = 0;

	int					i = 0;


	int					nAreaNum = 0;
	int					nBufBytes = 0;
	TCHAR				szNameBuf[1024] = {0};
	TCHAR				szAreaDefault[256] = {0};
	TCHAR				*lpTmp = 0;
	int					nDefaultIndex = -1;



	//Front side, YMCO
	nAreaNum = 0;
	nBufBytes = 1024 * sizeof(TCHAR);
	nDefaultIndex = -1;
	memset(szNameBuf, 0, nBufBytes);
	memset(szAreaDefault, 0, sizeof(TCHAR)*256);
	lpTmp = 0;
	dwRet = SOY_PR_GetPrintArea(m_szPrinterName, m_nOrientation+1, 0, 0, &nAreaNum, &nBufBytes, szNameBuf, szAreaDefault);
	if ( nAreaNum > 0 )
	{
		lpTmp = szNameBuf;
		for(i=0;i<nAreaNum;i++)
		{
			m_ComboAreaFrontYMCO.AddString(lpTmp);
			if ( lstrcmp(lpTmp, szAreaDefault) == 0 )
				nDefaultIndex = i;
			lpTmp += lstrlen(lpTmp) + 1;
		}

		m_ComboAreaFrontYMCO.SetCurSel(nDefaultIndex);
	}

	//Front side, K
	nAreaNum = 0;
	nBufBytes = 1024 * sizeof(TCHAR);
	nDefaultIndex = -1;
	memset(szNameBuf, 0, nBufBytes);
	memset(szAreaDefault, 0, sizeof(TCHAR)*256);
	lpTmp = 0;
	dwRet = SOY_PR_GetPrintArea(m_szPrinterName, m_nOrientation+1, 0, 1, &nAreaNum, &nBufBytes, szNameBuf, szAreaDefault);
	if ( nAreaNum > 0 )
	{
		lpTmp = szNameBuf;
		for(i=0;i<nAreaNum;i++)
		{
			m_ComboAreaFrontK.AddString(lpTmp);
			if ( lstrcmp(lpTmp, szAreaDefault) == 0 )
				nDefaultIndex = i;
			lpTmp += lstrlen(lpTmp) + 1;
		}

		m_ComboAreaFrontK.SetCurSel(nDefaultIndex);
	}

	//Front side, Rewrite
	nAreaNum = 0;
	nBufBytes = 1024 * sizeof(TCHAR);
	nDefaultIndex = -1;
	memset(szNameBuf, 0, nBufBytes);
	memset(szAreaDefault, 0, sizeof(TCHAR)*256);
	lpTmp = 0;
	dwRet = SOY_PR_GetPrintArea(m_szPrinterName, m_nOrientation+1, 0, 2, &nAreaNum, &nBufBytes, szNameBuf, szAreaDefault);
	if ( nAreaNum > 0 )
	{
		lpTmp = szNameBuf;
		for(i=0;i<nAreaNum;i++)
		{
			m_ComboAreaFrontRewrite.AddString(lpTmp);
			if ( lstrcmp(lpTmp, szAreaDefault) == 0 )
				nDefaultIndex = i;
			lpTmp += lstrlen(lpTmp) + 1;
		}

		m_ComboAreaFrontRewrite.SetCurSel(nDefaultIndex);
	}

	//Back side, YMCO
	nAreaNum = 0;
	nBufBytes = 1024 * sizeof(TCHAR);
	nDefaultIndex = -1;
	memset(szNameBuf, 0, nBufBytes);
	memset(szAreaDefault, 0, sizeof(TCHAR)*256);
	lpTmp = 0;
	dwRet = SOY_PR_GetPrintArea(m_szPrinterName, m_nOrientation+1, 1, 0, &nAreaNum, &nBufBytes, szNameBuf, szAreaDefault);
	if ( nAreaNum > 0 )
	{
		lpTmp = szNameBuf;
		for(i=0;i<nAreaNum;i++)
		{
			m_ComboAreaBackYMCO.AddString(lpTmp);
			if ( lstrcmp(lpTmp, szAreaDefault) == 0 )
				nDefaultIndex = i;
			lpTmp += lstrlen(lpTmp) + 1;
		}

		m_ComboAreaBackYMCO.SetCurSel(nDefaultIndex);
	}

	//Back side, K
	nAreaNum = 0;
	nBufBytes = 1024 * sizeof(TCHAR);
	nDefaultIndex = -1;
	memset(szNameBuf, 0, nBufBytes);
	memset(szAreaDefault, 0, sizeof(TCHAR)*256);
	lpTmp = 0;
	dwRet = SOY_PR_GetPrintArea(m_szPrinterName, m_nOrientation+1, 1, 1, &nAreaNum, &nBufBytes, szNameBuf, szAreaDefault);
	if ( nAreaNum > 0 )
	{
		lpTmp = szNameBuf;
		for(i=0;i<nAreaNum;i++)
		{
			m_ComboAreaBackK.AddString(lpTmp);
			if ( lstrcmp(lpTmp, szAreaDefault) == 0 )
				nDefaultIndex = i;
			lpTmp += lstrlen(lpTmp) + 1;
		}

		m_ComboAreaBackK.SetCurSel(nDefaultIndex);
	}
}

void CDlgPrint::OnBtnSetPrintArea()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	int					i = 0;

	TCHAR				szAreaDefault[256] = {0};



	//Front side, YMCO
	i = m_ComboAreaFrontYMCO.GetCurSel();
	if ( i > -1 )
	{
		m_ComboAreaFrontYMCO.GetLBText(i, szAreaDefault);
		dwRet = SOY_PR_SetPrintArea(m_szPrinterName, m_nOrientation+1, 0, 0, szAreaDefault);
	}

	//Front side, K
	i = m_ComboAreaFrontK.GetCurSel();
	if ( i > -1 )
	{
		m_ComboAreaFrontK.GetLBText(i, szAreaDefault);
		dwRet = SOY_PR_SetPrintArea(m_szPrinterName, m_nOrientation+1, 0, 1, szAreaDefault);
	}

	//Front side, Rewrite
	i = m_ComboAreaFrontRewrite.GetCurSel();
	if ( i > -1 )
	{
		m_ComboAreaFrontRewrite.GetLBText(i, szAreaDefault);
		dwRet = SOY_PR_SetPrintArea(m_szPrinterName, m_nOrientation+1, 0, 2, szAreaDefault);
	}

	//Back side, YMCO
	i = m_ComboAreaBackYMCO.GetCurSel();
	if ( i > -1 )
	{
		m_ComboAreaBackYMCO.GetLBText(i, szAreaDefault);
		dwRet = SOY_PR_SetPrintArea(m_szPrinterName, m_nOrientation+1, 1, 0, szAreaDefault);
	}

	//Back side, K
	i = m_ComboAreaBackK.GetCurSel();
	if ( i > -1 )
	{
		m_ComboAreaBackK.GetLBText(i, szAreaDefault);
		dwRet = SOY_PR_SetPrintArea(m_szPrinterName, m_nOrientation+1, 1, 1, szAreaDefault);
	}
}

void CDlgPrint::OnBtnDeleteAllJobs()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	dwRet = SOY_PR_DeleteAllJobs(m_szPrinterName);
}

void CDlgPrint::OnBtnBrowseSecurityBmpFront()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP Files(*.BMP)\0*.BMP\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_SECURITY_BMP_FRONT)->SetWindowText(OpenFileName);
	}
}

void CDlgPrint::OnBtnBrowseSecurityBmpBack()
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP Files(*.BMP)\0*.BMP\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_SECURITY_BMP_BACK)->SetWindowText(OpenFileName);
	}
}

