// SeaoryDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeaoryDemo.h"
#include "SeaoryDemoDlg.h"

#include <winspool.h>
#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern WORD				g_wLangID;
extern bool				g_bChangeLangRestart;
extern CSeaoryDemoApp	theApp;


//
#include <setupapi.h>

static DWORD SUPPORT_VIDPID[] =
{
	0x2FDD0003,		//Seaory S20,S20M,S20N
	0x2FDD0201,		//Seaory S20R

	0x2FDD0002,		//Seaory S21,S21F,S21H,S21N,S16
	0x2FDD0602,		//Seaory S21A
	0x2FDD0642,		//Seaory S21E
	0x2FDD0082,		//Seaory S21N-B

	0x2FDD0001,		//Seaory S22,S22F,S18
	0x2FDD0601,		//Seaory S22A,S22T
	0x2FDD0641,		//Seaory S22C,S22V
	0x2FDD0681,		//Seaory S22E
	0x2FDD06C1,		//Seaory S22G
	0x2FDD0103,		//Seaory S22J
	0x2FDD0101,		//Seaory S22K,S22B,S22D,S22N
	0x2FDD0102,		//Seaory S22M,S22MB
	0x2FDD0142,		//Seaory S22MC
	0x2FDD06A1,		//Seaory S22O,S22H
	0x2FDD06E1,		//Seaory S22P
	0x2FDD0202,		//Seaory S22R

	0x2FDD0006,		//Seaory S25
	0x2FDD0007,		//Seaory S26
	0x2FDD0008,		//Seaory S28

	0x2FDD0301,		//Seaory R300,R330,E330
	0x2FDD0302,		//Seaory R600,R660,E600
	0x2FDD0402,		//Seaory R600M
	0x2FDD0005,		//Seaory E30

	0x2FDD1002,		//Goodcard GS200

	0x2FDD2002,		//Goldpac DCE160
	0x2FDD2106,		//Goldpac DCE160K
	0x2FDD2082,		//Goldpac DCE160N-B
	0x2FDD2001,		//Goldpac DCE800
	0x2FDD2601,		//Goldpac DCE800A
	0x2FDD2101,		//Goldpac DCE800K
	0x2FDD2302,		//Goldpac DCE905

	0x2FDD3002,		//Prealway R35

	0x2FDD4004,		//CTD-C-E30
	0x2FDD4302,		//CTD DR600
	0x2FDD4002,		//CTD DS300,DS300N,CTD-C-S21
	0x2FDD4641,		//CTD DS300C,DS300(V)
	0x2FDD4001,		//CTD DS300D
	0x2FDD4041,		//CTD DS300(D)
	0x2FDD4042,		//CTD DS300N-A
	0x2FDD4082,		//CTD DS300N-B
	0x2FDD40C2,		//CTD DS300(W)
	0x2FDD4601,		//CTD DS300T

	0x2FDD5002,		//Fagoo P320E
	0x2FDD7002,		//Kingaotech KT-56S
	0x2FDD7302,		//Kingaotech KT-R86

	0x2FDD8302,		//TSZ-T1 D600
	0x2FDD8002,		//TSZ-T1 D300

	0x2FDD9002,		//Silone DP300
	0x2FDD9082,		//Silone DP300N-B

	0x2FDDA082,		//Jielika SJ21M-B

	0x2FDDB002,		//Kalicnp KD80
	0x2FDDB001,		//Kalicnp KD82

	0x2FDDC002,		//GS Series
	0x2FDD8301,		//TSZ-T1 D600-Q
	0x2FDDD301,		//Imagedec R5000
	0x2FDDE006,		//Volty ID X100
	0x2FDDE007,		//Volty ID X200
	0x2FDDE008,		//Volty ID X300
	0x2FDDF007,		//AUTHENTYS PRO 200
	0x2FDD1042,		//Gudecard GS200PRO
	0x2FDD5302,		//Fagoo P63XX
	0x2FDD6006,		//Bodno A10
	0x2FDD6007,		//Bodno A20
	0x2FDD6008,		//Bodno A30
	0x2FDD6028,		//Bodno A40 PRO

	0
};



BOOL EnumUSBDevice(HWND hComboBox, IN HDEVINFO HardwareDeviceInfo, IN PSP_INTERFACE_DEVICE_DATA InterfaceInfoData, IN PSP_DEVINFO_DATA DeviceInfoData, DWORD dwVidPid)
{
	ULONG				ulRequiredLength = 0;
	ULONG				ulPredictedLength = 0;
	BOOL				bOK = FALSE;
	BOOL				bRet = FALSE;

	TCHAR				szVidPid[32] = {0};

	DWORD				dwLastError = 0;

	PSP_INTERFACE_DEVICE_DETAIL_DATA	interfaceDetailData = 0;

	TCHAR				szDeviceID[MAX_PATH] = {0};
	DWORD				dwDeviceIDSize = 0;
	TCHAR				szDeviceDESC[MAX_PATH] = {0};
	DWORD				dwDeviceDESCSize = 0;
	TCHAR				szDEVID[MAX_PATH] = {0};


	_stprintf(szVidPid, _T("vid_%04x&pid_%04x"), HIWORD(dwVidPid), LOWORD(dwVidPid));

	bRet = SetupDiGetDeviceInterfaceDetail(HardwareDeviceInfo, InterfaceInfoData, NULL, 0, &ulRequiredLength, NULL);
	if ( ulRequiredLength == 0 )
		return FALSE;

	ulPredictedLength		= ulRequiredLength;

	interfaceDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA) malloc(ulPredictedLength);
	if ( interfaceDetailData == NULL )
	{
		::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return FALSE;
	}

	interfaceDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
	bRet = SetupDiGetDeviceInterfaceDetail(HardwareDeviceInfo, InterfaceInfoData, interfaceDetailData, ulPredictedLength, &ulRequiredLength, NULL);
	if ( bRet )
	{
		//get hardware ID
		bRet = SetupDiGetDeviceRegistryProperty(HardwareDeviceInfo, DeviceInfoData, SPDRP_HARDWAREID, NULL, (unsigned char*)szDeviceID, MAX_PATH*sizeof(TCHAR), &dwDeviceIDSize);
		if ( bRet )
		{
			bRet = SetupDiGetDeviceRegistryProperty(HardwareDeviceInfo, DeviceInfoData, SPDRP_DEVICEDESC, NULL, (unsigned char*)szDeviceDESC, MAX_PATH*sizeof(TCHAR), &dwDeviceDESCSize);
			if ( bRet )
			{
				lstrcpy(szDEVID, szDeviceID);
				_tcslwr(szDEVID);

				if ( _tcsstr(szDEVID,szVidPid) != NULL )
				{
					::SendMessage(hComboBox, CB_ADDSTRING, NULL, (LPARAM)interfaceDetailData->DevicePath);
				}
				bOK = TRUE;
			}
			else
			{
				dwLastError = ::GetLastError();
			}
		}
		else
		{
			dwLastError = ::GetLastError();
		}
	}
	else
	{
		dwLastError = ::GetLastError();
	}

	free(interfaceDetailData);
	::SetLastError(dwLastError);

	return bOK;
}


BOOL EnumUSBPrinters(HWND hComboBox)
{
	BOOL						bResult = FALSE;
	HDEVINFO					hardwareDeviceInfo = 0;
	SP_INTERFACE_DEVICE_DATA	interfaceInfoData = {0};
	SP_DEVINFO_DATA				deviceInfo = {0};
	DWORD						dwIndex = 0;
	DWORD						dwDev = 0;
	HANDLE						hDev = INVALID_HANDLE_VALUE;
	GUID						UsbprintGuid;//{28D78FAD-5A12-11D1-AE5B-0000F803A8C2}

	int							i = 0;
	DWORD						dwLastError = 0;

	if ( hComboBox == NULL )
	{
		return(FALSE);
	}

	UsbprintGuid.Data1	 = 0x28D78FAD;
	UsbprintGuid.Data2	 = 0x5A12;
	UsbprintGuid.Data3	 = 0x11D1;
	UsbprintGuid.Data4[0]= 0xAE;
	UsbprintGuid.Data4[1]= 0x5B;
	UsbprintGuid.Data4[2]= 0x00;
	UsbprintGuid.Data4[3]= 0x00;
	UsbprintGuid.Data4[4]= 0xF8;
	UsbprintGuid.Data4[5]= 0x03;
	UsbprintGuid.Data4[6]= 0xA8;
	UsbprintGuid.Data4[7]= 0xC2;

	interfaceInfoData.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);
	deviceInfo.cbSize		 = sizeof(SP_DEVINFO_DATA);

	hardwareDeviceInfo = SetupDiGetClassDevs((LPGUID)&UsbprintGuid, NULL, NULL, (DIGCF_PRESENT|DIGCF_INTERFACEDEVICE));

	bResult = (INVALID_HANDLE_VALUE != hardwareDeviceInfo);

	dwDev = 0;
	while ( bResult && SetupDiEnumDeviceInfo(hardwareDeviceInfo, dwDev, &deviceInfo) )
	{
		dwIndex = 0;
		while ( bResult && SetupDiEnumDeviceInterfaces(hardwareDeviceInfo, &deviceInfo, (LPGUID)&UsbprintGuid, dwIndex, &interfaceInfoData) )
		{
			bResult = FALSE;
			for(i=0;;i++)
			{
				if ( SUPPORT_VIDPID[i] == 0 )
					break;

				bResult = EnumUSBDevice(hComboBox, hardwareDeviceInfo, &interfaceInfoData, &deviceInfo, SUPPORT_VIDPID[i]);
			}

			if ( !bResult )
			{
				dwLastError = ::GetLastError();

				SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);

				::SetLastError(dwLastError);
				return(FALSE);
			}
			++dwIndex;
		}

		++dwDev;
	}

	SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);

	return(TRUE);
}


TCHAR* WINAPI GetErrorString(DWORD dwError, TCHAR* szErrorString)
{
	int					i = 0;
	TCHAR				szMsg[512] = {0};
	DWORD				dwRet = 0;

	const TCHAR			*lpDesc = 0;

	if ( dwError != 0 )
	{
		while ( ErrorMap[i].dwErrCode != 0xFFFFFFFF )
		{
			if ( dwError == ErrorMap[i].dwErrCode )
			{
				lpDesc = ErrorMap[i].pDesc;
				break;
			}
			i++;
		}

		if ( lpDesc )
		{
			//lstrcpy(szMsg, lpDesc);
			_stprintf(szErrorString, _T("%s\n\n(0x%08X)"), lpDesc, dwError);
		}
		else
		{
			dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), szMsg, 512, NULL);
			_stprintf(szErrorString, _T("%s\n\n(%d)"), szMsg, dwError);
		}
	}
	else
	{
		lstrcpy(szErrorString, _T("OK"));
	}

	return szErrorString;
}

void WINAPI ShowResultMessage(DWORD dwError, TCHAR* szFuncName)
{
	int					i = 0;
	TCHAR				szMsg[512] = {0};
	DWORD				dwRet = 0;

	CString				temp;

	if ( dwError != 0 )
	{
		GetErrorString(dwError, szMsg);
		::MessageBox(0, szMsg, szFuncName, MB_OK|MB_ICONERROR);
	}
	else
	{
		temp.LoadString(IDS_SUCCESS);
		lstrcpy(szMsg, temp);
		::MessageBox(0, szMsg, szFuncName, MB_OK|MB_ICONINFORMATION);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeaoryDemoDlg dialog

CSeaoryDemoDlg::CSeaoryDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeaoryDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeaoryDemoDlg)
	m_nConnectType = 0;
	m_nCardPos = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(m_szPrinterNameArray, 0, sizeof(TCHAR)*32*128);
	memset(m_szDriverNameArray, 0, sizeof(TCHAR)*32*128);
}

void CSeaoryDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeaoryDemoDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nConnectType);
	DDX_Control(pDX, IDC_COMBO_SYMLINK, m_ComboSymlink);
	DDX_Control(pDX, IDC_CHECK_LOG, m_EnableLog);
	DDX_Control(pDX, IDC_STATIC_SDK_VERSION, m_StaticSdkVersion);
	DDX_Control(pDX, IDC_COMBO_CARD_POS, m_ComboPos);
	DDX_Control(pDX, IDC_COMBO_PRINTER, m_ComboPrinter);
	DDX_Control(pDX, IDC_TAB1, m_Tab1);
	DDX_CBIndex(pDX, IDC_COMBO_CARD_POS, m_nCardPos);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeaoryDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CSeaoryDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_PRINTER, OnSelchangeComboPrinter)
	ON_BN_CLICKED(IDC_BTN_MOVE_CARD, OnBtnMoveCard)
	ON_BN_CLICKED(IDC_CHECK_LOG, OnCheckLog)
	ON_CBN_SELCHANGE(IDC_COMBO_SYMLINK, OnSelchangeComboSymlink)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeaoryDemoDlg message handlers

BOOL CSeaoryDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);

			pSysMenu->AppendMenu(MF_SEPARATOR);

			pSysMenu->AppendMenu(MF_STRING, IDC_LANG_SYSTEM, _T("System"));
			if ( g_wLangID == 0 )
				pSysMenu->CheckMenuItem(IDC_LANG_SYSTEM, MF_BYCOMMAND | MF_CHECKED);

			pSysMenu->AppendMenu(MF_STRING, IDC_LANG_ENG, _T("English"));
			if ( g_wLangID == 1 )
				pSysMenu->CheckMenuItem(IDC_LANG_ENG, MF_BYCOMMAND | MF_CHECKED);

			pSysMenu->AppendMenu(MF_STRING, IDC_LANG_CHS, _T("Chinese Simplified"));//_T("简体中文 "));
			if ( g_wLangID == 2 )
				pSysMenu->CheckMenuItem(IDC_LANG_CHS, MF_BYCOMMAND | MF_CHECKED);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	if ( g_bChangeLangRestart )
		g_bChangeLangRestart = false;

	FindSupportedPrinters();
	if ( m_ComboPrinter.GetCount() > 0 )
		m_ComboPrinter.SetCurSel(0);

	EnumUSBPrinters(::GetDlgItem(GetSafeHwnd(), IDC_COMBO_SYMLINK));
	if ( m_ComboSymlink.GetCount() > 0 )
		m_ComboSymlink.SetCurSel(0);

	UpdateData(FALSE);

	CString  temp;
	temp.LoadString(IDS_TAB_COMMAND);
	m_Tab1.InsertItem(0, temp);
	temp.LoadString(IDS_TAB_CARD_INOUT);
	m_Tab1.InsertItem(1, temp);
	temp.LoadString(IDS_TAB_CONFIG);
	m_Tab1.InsertItem(2, temp);
	temp.LoadString(IDS_TAB_RETRANSFER);
	m_Tab1.InsertItem(3, temp);
	temp.LoadString(IDS_TAB_PRINT);
	m_Tab1.InsertItem(4, temp);
	temp.LoadString(IDS_TAB_PRINT2);
	m_Tab1.InsertItem(5, temp);
	temp.LoadString(IDS_TAB_MAGNETIC);
	m_Tab1.InsertItem(6, temp);
	temp.LoadString(IDS_TAB_SMART_CARD);
	m_Tab1.InsertItem(7, temp);
	temp.LoadString(IDS_TAB_UHF);
	m_Tab1.InsertItem(8, temp);
	temp.LoadString(IDS_TAB_FM1208);
	m_Tab1.InsertItem(9, temp);
	temp.LoadString(IDS_TAB_CONCAVE);
	m_Tab1.InsertItem(10, temp);
	temp.LoadString(IDS_TAB_SECURITY);
	m_Tab1.InsertItem(11, temp);

	m_Tab1.Init();

	int			k = 0;
	//CString  printerName;
	k = m_ComboPrinter.GetCurSel();
	if ( k >= 0 )
	{
		//m_ComboPrinter.GetLBText(0, printerName);
		//m_Tab1.SetPrinterName(printerName);
		m_Tab1.SetPrinterName(m_szPrinterNameArray[k], m_szDriverNameArray[k]);
	}

	int			i = 0;
	CString		pos;
	for(i=IDS_MOVE_CARD_POS_1;i<=IDS_MOVE_CARD_POS_14;i++)
	{
		pos.LoadString(i);
		m_ComboPos.AddString(pos);
	}

	m_ComboPos.SetCurSel(0);

	TCHAR			szSdkVersion[32] = {0};
	SOY_PR_SdkVersion(szSdkVersion);
	m_StaticSdkVersion.SetWindowText(szSdkVersion);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSeaoryDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ( nID >= IDC_LANG_SYSTEM && nID <= IDC_LANG_CHS )
	{
		WORD			wLangID = 0;

		switch(nID)
		{
			case IDC_LANG_SYSTEM:	wLangID = 0;		break;
			case IDC_LANG_ENG:		wLangID = 1;		break;
			case IDC_LANG_CHS:		wLangID = 2;		break;
		}

		g_wLangID = wLangID;
		g_bChangeLangRestart = true;

		theApp.WriteProfileInt(_T("Settings"), _T("Language"), g_wLangID);

		OnCancel();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSeaoryDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSeaoryDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

DWORD CSeaoryDemoDlg::FindSupportedPrinters()
{
	BOOL			bResult = FALSE;
	DWORD			i = 0, k = 0,
					dwBuf = 0,
					dwNeeded = 0,
					dwPrinterNum = 0;
	BYTE			*lpBuf = NULL;

	DWORD			dwFlags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;

	PRINTER_INFO_2	*lpPrinterInfo2 = NULL;

	bResult = EnumPrinters(dwFlags, NULL, 2, NULL, 0, &dwNeeded, &dwPrinterNum);

	if ( dwNeeded == 0 )
		return 3012;//ERROR_PRINTER_NOT_FOUND;

	dwBuf = dwNeeded;
	lpBuf = new BYTE[dwBuf];

	if ( !lpBuf )
		return 14;//ERROR_OUTOFMEMORY;

	bResult = EnumPrinters(dwFlags, NULL, 2, lpBuf, dwBuf, &dwNeeded, &dwPrinterNum);

	lpPrinterInfo2 = (PRINTER_INFO_2*)lpBuf;
	for(i=0;i<dwPrinterNum;i++,lpPrinterInfo2++)
	{
		if (   _tcsstr(lpPrinterInfo2->pDriverName, _T("Seaory S1"))
			|| _tcsstr(lpPrinterInfo2->pDriverName, _T("Seaory S2"))
			|| _tcsstr(lpPrinterInfo2->pDriverName, _T("Seaory R"))
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Seaory E30")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Seaory E600")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Seaory E330")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("EPT-W3(AX)")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("MS-DC600G")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD-C-E30")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DR600")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300C")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300D")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300(D)")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300N")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300N-A")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300N-B")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300T")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD DS300(V)")) == 0			
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("CTD CTD-C-S21")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Fagoo P320E")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Fagoo P63XX")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE160")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE160K")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE160N")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE160N-B")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE800")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE800A")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE800K")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE800N")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goldpac DCE905")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Goodcard GS200")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("ID CARD PRINTER")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Jielika SJ21M-B")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Kingaotech KT-56S")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Kingaotech KT-R86")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Prealway R35")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Silone DP300")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Silone DP300N")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Silone DP300N-B")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("TSZ-T1 D300")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("TSZ-T1 D600")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("TSZ-T1 D600-Q")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("TECSUN TSZ-F301")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Kalicnp KD80")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Kalicnp KD82")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("GS Series")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Imagedec R5000")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Volty ID X100")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Volty ID X200")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Volty ID X300")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("AUTHENTYS PRO 200")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Gudecard GS200PRO")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Bodno A10")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Bodno A20")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Bodno A30")) == 0
			|| lstrcmp(lpPrinterInfo2->pDriverName, _T("Bodno A40 PRO")) == 0
			)
		{
			m_ComboPrinter.AddString(lpPrinterInfo2->pPrinterName);

			lstrcpy(m_szPrinterNameArray[k], lpPrinterInfo2->pPrinterName);
			lstrcpy(m_szDriverNameArray[k], lpPrinterInfo2->pDriverName);
			k++;
		}
	}

	delete [] lpBuf;

	return 0;
}

void CSeaoryDemoDlg::OnSelchangeComboPrinter()
{
	// TODO: Add your control notification handler code here
	int					i = 0;
	//CString				printerName;

	i = m_ComboPrinter.GetCurSel();
	//m_ComboPrinter.GetLBText(i, printerName);
	//m_Tab1.SetPrinterName(printerName);

	if ( i >= 0 )
		m_Tab1.SetPrinterName(m_szPrinterNameArray[i], m_szDriverNameArray[i]);
}

void CSeaoryDemoDlg::OnBtnMoveCard()
{
	// TODO: Add your control notification handler code here
	unsigned long		dwRet = 0;

	int					i = 0;
	TCHAR				szPrinterName[128] = {0};

	DWORD				dwCommand = 0;

	UpdateData(TRUE);

	if ( m_nConnectType == 1 )
	{
		i = m_ComboSymlink.GetCurSel();
		m_ComboSymlink.GetLBText(i, szPrinterName);
	}
	else
	{
		i = m_ComboPrinter.GetCurSel();
		m_ComboPrinter.GetLBText(i, szPrinterName);
	}

	dwCommand = m_nCardPos+1;
	dwRet = SOY_PR_ExecCommand(szPrinterName, dwCommand);

	ShowResultMessage(dwRet, _T("OnBtnMoveCard"));
}

void CSeaoryDemoDlg::OnCheckLog()
{
	// TODO: Add your control notification handler code here
	int		nCheckState = 0;
	DWORD	dwLogLevel = 0;

	UpdateData(TRUE);

	nCheckState = m_EnableLog.GetCheck();
	if ( nCheckState == BST_CHECKED )
		dwLogLevel = 1;

	SOY_PR_SetLogLevel(dwLogLevel);
}
/*
void CSeaoryDemoDlg::ShowResultMessage(DWORD dwError, TCHAR* szFuncName)
{
	int					i = 0;
	TCHAR				szMsg[512] = {0};
	DWORD				dwRet = 0;

	CString				temp;
	const TCHAR			*lpDesc = 0;

	if ( dwError != 0 )
	{
		while ( ErrorMap[i].dwErrCode != 0xFFFFFFFF )
		{
			if ( dwError == ErrorMap[i].dwErrCode )
			{
				lpDesc = ErrorMap[i].pDesc;
				break;
			}
			i++;
		}

		if ( lpDesc )
		{
			lstrcpy(szMsg, lpDesc);
		}
		else
		{
			//temp.LoadString(IDS_STATUS);
			//_stprintf(szMsg, _T("%s = 0x%08X"), temp, dwError);

			dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), szMsg, 512, NULL);
			temp.Format(_T("%s (%d)"), szMsg, dwError);
			lstrcpy(szMsg, temp);
		}
		MessageBox(szMsg, szFuncName, MB_OK|MB_ICONERROR);
	}
	else
	{
		temp.LoadString(IDS_SUCCESS);
		lstrcpy(szMsg, temp);
		MessageBox(szMsg, szFuncName, MB_OK|MB_ICONINFORMATION);
	}
}
*/
void CSeaoryDemoDlg::OnSelchangeComboSymlink()
{
	// TODO: Add your control notification handler code here
	int					i = 0;
	CString				symlink;

	i = m_ComboSymlink.GetCurSel();
	if ( i >= 0 )
	{
	m_ComboSymlink.GetLBText(i, symlink);
	m_Tab1.SetPrinterName((LPTSTR)(LPCTSTR)symlink, _T(""));
	}
}

void CSeaoryDemoDlg::OnRadio1()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_COMBO_PRINTER)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_SYMLINK)->EnableWindow(FALSE);
	OnSelchangeComboPrinter();
}

void CSeaoryDemoDlg::OnRadio2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_COMBO_PRINTER)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SYMLINK)->EnableWindow(TRUE);
	OnSelchangeComboSymlink();
}
