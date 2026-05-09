// DlgScard.cpp : implementation file
//

#include "stdafx.h"
#include "SeaoryDemo.h"
#include "DlgScard.h"

#include "SeaoryScard.h"
#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void WINAPI UDoCalibrationThread(CDlgScard *cpDlg);


/////////////////////////////////////////////////////////////////////////////
// CDlgScard dialog


CDlgScard::CDlgScard(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgScard)
	m_nPort = 0;
	m_nBaudRate = 0;
	m_nScardType = 0;
	//}}AFX_DATA_INIT
	m_hDev = 0;
}


void CDlgScard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgScard)
	DDX_Control(pDX, IDC_EDIT_RESULT, m_EditResult);
	DDX_CBIndex(pDX, IDC_COMBO_PORT, m_nPort);
	DDX_CBIndex(pDX, IDC_COMBO_BAUD_RATE, m_nBaudRate);
	DDX_CBIndex(pDX, IDC_COMBO_SCARD_TYPE, m_nScardType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgScard, CDialog)
	//{{AFX_MSG_MAP(CDlgScard)
	ON_BN_CLICKED(IDC_BTN_CPU_TYPE_A, OnBtnCpuTypeA)
	ON_BN_CLICKED(IDC_BTN_CPU_TYPE_B, OnBtnCpuTypeB)
	ON_BN_CLICKED(IDC_BTN_MIFARE_DESFIRE, OnBtnMifareDesfire)
	ON_BN_CLICKED(IDC_BTN_MIFARE_PLUS, OnBtnMifarePlus)
	ON_BN_CLICKED(IDC_BTN_ULTRA_LIGHT, OnBtnUltraLight)
	ON_BN_CLICKED(IDC_BTN_ULTRA_LIGHT_C, OnBtnUltraLightC)
	ON_BN_CLICKED(IDC_BTN_4442, OnBtn4442)
	ON_BN_CLICKED(IDC_BTN_4428, OnBtn4428)
	ON_BN_CLICKED(IDC_BTN_24CXX, OnBtn24cxx)
	ON_BN_CLICKED(IDC_BTN_CPU, OnBtnCpu)
	ON_BN_CLICKED(IDC_BTN_M1_WRITE_READ, OnBtnM1WriteRead)
	ON_BN_CLICKED(IDC_BTN_M1_VALUE, OnBtnM1Value)
	ON_BN_CLICKED(IDC_BTN_GET_VER, OnBtnGetScardReaderVersion)
	ON_BN_CLICKED(IDC_BTN_CALIBRATE_SCARD_POS, OnBtnCalibrateScardPosition)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgScard message handlers

void CDlgScard::ShowResult(LPCTSTR szText)
{
	int nTxtLen = m_EditResult.GetWindowTextLength();
	m_EditResult.SetSel(nTxtLen, nTxtLen);
	m_EditResult.ReplaceSel(szText);
	m_EditResult.SetSel(nTxtLen, nTxtLen);
	m_EditResult.ReplaceSel(_T("\r\n"));
}

void CDlgScard::ShowResultA(LPCSTR szText)
{
	TCHAR				szWideStr[512] = {0};
	int					nBytes = 0;

#ifdef _UNICODE
	// Get the length of the converted string
	nBytes = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, 0);

	if ( nBytes == 0 )
		return;

	// Convert the string
	nBytes = MultiByteToWideChar(CP_ACP, 0, szText, -1, szWideStr, nBytes);
#else
	strcpy(szWideStr, szText);
#endif

	ShowResult(szWideStr);
}

void CDlgScard::ShowResult(int nStringID)
{
	CString			temp;
	temp.LoadString(nStringID);

	ShowResult(temp);
}

void CDlgScard::ShowScardResult(LPCTSTR szFuncName, int nRet, BYTE* lpData, int nLen)
{
	TCHAR				szMsg[512] = {0};

	_stprintf(szMsg, _T("%s() return %d => %s"), szFuncName, nRet, (nRet==0)?_T("OK"):_T("NG"));

	ShowResult(szMsg);

	char			szHex[512] = {0};

	if ( nRet == 0 && lpData != NULL )
	{
		SOY_SC_Hex_A(lpData, (BYTE*)szHex, nLen);
		ShowResultA(szHex);
	}
}

void CDlgScard::ShowResultMessage(DWORD dwError, TCHAR* szFuncName)
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
			//temp.Format(_T("%s (%d)"), szMsg, dwError);
			//lstrcpy(szMsg, temp);
		}
		//MessageBox(szMsg, szFuncName, MB_OK|MB_ICONERROR);
	}
	else
	{
		temp.LoadString(IDS_SUCCESS);
		lstrcpy(szMsg, temp);
		//MessageBox(szMsg, szFuncName, MB_OK|MB_ICONINFORMATION);
	}

	temp.Format(_T("%s() return %d => %s"), szFuncName, dwError, szMsg);
	ShowResult(temp);
}

bool CDlgScard::OpenScardReaderPort()
{
	short				nPort = 0;

	UpdateData(TRUE);

	nPort = m_nPort;
	if ( m_nPort == 0 )//USB
		nPort = 100;

	m_hDev = SOY_SC_Init(nPort);

	if ( m_hDev == 0 || m_hDev == INVALID_HANDLE_VALUE )
	{
		ShowScardResult(_T("SOY_SC_Init"), -1, NULL, 0);
		//ShowResult(IDS_INIT_COM_ERROR);
		return false;
	}
	//ShowResult(IDS_INIT_COM_OK);
	ShowScardResult(_T("SOY_SC_Init"), 0, NULL, 0);

	//SOY_SC_Beep(m_hDev, 10);

	return true;
}

void CDlgScard::CloseScardReaderPort()
{
	short				nRet = 0;

	if ( m_hDev > 0 )
	{
		SOY_SC_Beep(m_hDev, 10);
		nRet = SOY_SC_Exit(m_hDev);
		ShowScardResult(_T("SOY_SC_Exit"), nRet, NULL, 0);
	}
	m_hDev = 0;
}

void CDlgScard::OnBtnM1WriteRead()
{
	short				nRet = -1;

	unsigned char		mode = 0;
	unsigned char		cardSN[64] = {0};
	unsigned int		snLen = 0;

	unsigned char		byAuthMode = 0;
	unsigned char		bySectorNum = 1;
	unsigned char		keyA[64] = {0};

	int					i = 0;
	unsigned char		writeBlock = 1 * 4 + 0;
	unsigned char		writeBuf[512] = {0};

	unsigned char		readBlock = 1 * 4 + 0;
	unsigned char		readBuf[512] = {0};

	ShowResult(_T("----- M1 Write Read -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'A');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_n(m_hDev, mode, &snLen, cardSN);
	ShowScardResult(_T("SOY_SC_Card_n"), nRet, cardSN, snLen);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//验证卡密码
	for(i=0;i<6;i++)
	{
		keyA[i] = 0xFF;
	}
	nRet = SOY_SC_Authentication_pass(m_hDev, byAuthMode, bySectorNum, keyA);
	ShowScardResult(_T("SOY_SC_Authentication_pass"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//写扇区
	for(i=0;i<16;i++)
	{
		writeBuf[i] = rand() % 0xFF;
	}
	nRet = SOY_SC_Write(m_hDev, writeBlock, writeBuf);
	ShowScardResult(_T("SOY_SC_Write"), nRet, writeBuf, 16);

	//读扇区
	nRet = SOY_SC_Read(m_hDev, readBlock, readBuf);
	ShowScardResult(_T("SOY_SC_Read"), nRet, readBuf, 16);

	CloseScardReaderPort();
}

void CDlgScard::OnBtnM1Value()
{
	int					nRet = -1;

	unsigned char		mode = 0;
	unsigned char		cardSN[64] = {0};
	unsigned int		snLen = 0;

	unsigned char		byAuthMode = 0;
	unsigned char		bySectorNum = 1;
	unsigned char		keyA[64] = {0};

	int					i = 0;
	unsigned char		byBlock = 1 * 4 + 1;

	unsigned int		uiValue = 0;
	TCHAR				szValue[128] = {0};

	ShowResult(_T("----- M1 Value -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'A');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_n(m_hDev, mode, &snLen, cardSN);
	ShowScardResult(_T("SOY_SC_Card_n"), nRet, cardSN, snLen);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//验证卡密码
	for(i=0;i<6;i++)
	{
		keyA[i] = 0xFF;
	}
	nRet = SOY_SC_Authentication_pass(m_hDev, byAuthMode, bySectorNum, keyA);
	ShowScardResult(_T("SOY_SC_Authentication_pass"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//块值初始化为0
	nRet = SOY_SC_InitVal(m_hDev, byBlock, 0);
	ShowScardResult(_T("SOY_SC_InitVal"), nRet, NULL, 0);

	//读块值
	nRet = SOY_SC_ReadVal(m_hDev, byBlock, &uiValue);
	ShowScardResult(_T("SOY_SC_ReadVal"), nRet, NULL, 0);
	if ( nRet == 0 )
	{
		_itot(uiValue, szValue, 10);
		ShowResult(szValue);
	}

	//块加值
	nRet = SOY_SC_Increment(m_hDev, byBlock, 1000);
	ShowScardResult(_T("SOY_SC_Increment"), nRet, NULL, 0);

	//读块值
	nRet = SOY_SC_ReadVal(m_hDev, byBlock, &uiValue);
	ShowScardResult(_T("SOY_SC_ReadVal"), nRet, NULL, 0);
	if ( nRet == 0 )
	{
		_itot(uiValue, szValue, 10);
		ShowResult(szValue);
	}

	//块减值
	nRet = SOY_SC_Decrement(m_hDev, byBlock, 100);
	ShowScardResult(_T("SOY_SC_Decrement"), nRet, NULL, 0);

	//读块值
	nRet = SOY_SC_ReadVal(m_hDev, byBlock, &uiValue);
	ShowScardResult(_T("SOY_SC_ReadVal"), nRet, NULL, 0);
	if ( nRet == 0 )
	{
		_itot(uiValue, szValue, 10);
		ShowResult(szValue);
	}

	//块值传递
	nRet = SOY_SC_Restore(m_hDev, byBlock);
	ShowScardResult(_T("SOY_SC_Restore"), nRet, NULL, 0);
	if ( nRet == 0 )
	{
		nRet = SOY_SC_Transfer(m_hDev, byBlock+1);
		ShowScardResult(_T("SOY_SC_Transfer"), nRet, NULL, 0);
	}

	//读块值
	nRet = SOY_SC_ReadVal(m_hDev, byBlock+1, &uiValue);
	ShowScardResult(_T("SOY_SC_ReadVal"), nRet, NULL, 0);
	if ( nRet == 0 )
	{
		_itot(uiValue, szValue, 10);
		ShowResult(szValue);
	}

	CloseScardReaderPort();
}

void CDlgScard::OnBtnCpuTypeA()
{
	int					nRet = -1;

	unsigned char		mode = 0;
	unsigned char		cardSN[64] = {0};
	unsigned int		snLen = 0;

	unsigned char		atrLen = 0;
	unsigned char		atrBuf[512] = {0};
	unsigned int		readLen = 0;
	unsigned char		readBuf[512] = {0};

	unsigned int		commandLen = 0;
	unsigned char		commandBuf[512] = {0};

	ShowResult(_T("----- CPU Type A -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'A');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_n(m_hDev, mode, &snLen, cardSN);
	ShowScardResult(_T("SOY_SC_Card_n"), nRet, cardSN, snLen);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//非接触式CPU卡复位
	nRet = SOY_SC_Pro_ResetInt(m_hDev, &atrLen, atrBuf);
	ShowScardResult(_T("SOY_SC_Pro_ResetInt"), nRet, atrBuf, atrLen);

	//非接触式CPU卡指令交互
	if ( nRet == 0 )
	{
		commandBuf[0] = 0x00;
		commandBuf[1] = 0x84;
		commandBuf[2] = 0x00;
		commandBuf[3] = 0x00;
		commandBuf[4] = 0x08;

		commandLen = 5;

		nRet = SOY_SC_Pro_CommandLinkInt(m_hDev, commandLen, commandBuf, &readLen, readBuf, 10);
		ShowScardResult(_T("SOY_SC_Pro_CommandLinkInt"), nRet, readBuf, readLen);
	}

	CloseScardReaderPort();
}

void CDlgScard::OnBtnCpuTypeB()
{
	int					nRet = -1;

	unsigned char		cardSN[64] = {0};

	unsigned int		readLen = 0;
	unsigned char		readBuf[512] = {0};

	unsigned int		commandLen = 0;
	unsigned char		commandBuf[512] = {0};

	ShowResult(_T("----- CPU Type B -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'B');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_B(m_hDev, cardSN);
	ShowScardResult(_T("SOY_SC_Card_B"), nRet, cardSN, 11);

	//非接触式CPU卡指令交互
	if ( nRet == 0 )
	{
		commandBuf[0] = 0x00;
		commandBuf[1] = 0x84;
		commandBuf[2] = 0x00;
		commandBuf[3] = 0x00;
		commandBuf[4] = 0x08;

		commandLen = 5;

		nRet = SOY_SC_Pro_CommandLinkInt(m_hDev, commandLen, commandBuf, &readLen, readBuf, 10);
		ShowScardResult(_T("SOY_SC_Pro_CommandLinkInt"), nRet, readBuf, readLen);
	}

	CloseScardReaderPort();
}

void CDlgScard::OnBtnMifareDesfire()
{
	int					nRet = -1;

	unsigned char		mode = 0;
	unsigned char		cardSN[64] = {0};
	unsigned int		snLen = 0;

	unsigned char		atrLen = 0;
	unsigned char		atrBuf[512] = {0};
	unsigned int		readLen = 0;
	unsigned char		readBuf[512] = {0};

	unsigned int		commandLen = 0;
	unsigned char		commandBuf[512] = {0};

	ShowResult(_T("----- Mifare DESFire -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'A');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_n(m_hDev, mode, &snLen, cardSN);
	ShowScardResult(_T("SOY_SC_Card_n"), nRet, cardSN, snLen);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//非接触式CPU卡复位
	nRet = SOY_SC_Pro_ResetInt(m_hDev, &atrLen, atrBuf);
	ShowScardResult(_T("SOY_SC_Pro_ResetInt"), nRet, atrBuf, atrLen);

	//非接触式CPU卡指令交互
	if ( nRet == 0 )
	{
		commandBuf[0] = 0x00;
		commandBuf[1] = 0x84;
		commandBuf[2] = 0x00;
		commandBuf[3] = 0x00;
		commandBuf[4] = 0x08;

		commandLen = 5;

		nRet = SOY_SC_Pro_CommandLinkInt(m_hDev, commandLen, commandBuf, &readLen, readBuf, 10);
		ShowScardResult(_T("SOY_SC_Pro_CommandLinkInt"), nRet, readBuf, readLen);
	}

	CloseScardReaderPort();
}

void CDlgScard::OnBtnMifarePlus()
{
	int					nRet = -1;

	unsigned char		mode = 0;
	unsigned char		cardSN[64] = {0};
	unsigned int		snLen = 0;

	unsigned char		atrLen = 0;
	unsigned char		atrBuf[512] = {0};

	int					i = 0;
	unsigned char		writeBuf[512] = {0};

	ShowResult(_T("----- Mifare Plus -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'A');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_n(m_hDev, mode, &snLen, cardSN);
	ShowScardResult(_T("SOY_SC_Card_n"), nRet, cardSN, snLen);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//非接触式CPU卡复位
	nRet = SOY_SC_Pro_Reset(m_hDev, &atrLen, atrBuf);
	ShowScardResult(_T("SOY_SC_Pro_Reset"), nRet, atrBuf, atrLen);

	//Mifare Plus卡设置个人化数据（0级）
	if ( nRet == 0 )
	{
		for(i=0;i<16;i++)
		{
			writeBuf[i] = rand() % 0xFF;
		}
		nRet = SOY_SC_MF_PLUS_L0_WritePerson(m_hDev, 0x9000, writeBuf);
		ShowScardResult(_T("SOY_SC_MF_PLUS_L0_WritePerson"), nRet, writeBuf, 16);
	}

	CloseScardReaderPort();
}

void CDlgScard::OnBtnUltraLight()
{
	int					nRet = -1;

	unsigned char		mode = 0;
	unsigned char		cardSN[64] = {0};
	unsigned int		snLen = 0;

	int					i = 0;
	unsigned char		writeBlock = 4;
	unsigned char		writeBuf[512] = {0};

	unsigned char		readBlock = 4;
	unsigned char		readBuf[512] = {0};

	ShowResult(_T("----- UltraLight -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'A');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_n(m_hDev, mode, &snLen, cardSN);
	ShowScardResult(_T("SOY_SC_Card_n"), nRet, cardSN, snLen);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//写
	for(i=0;i<4;i++)
	{
		writeBuf[i] = rand() % 0xFF;
	}
	nRet = SOY_SC_Write(m_hDev, writeBlock, writeBuf);
	ShowScardResult(_T("SOY_SC_Write"), nRet, writeBuf, 4);

	//读
	nRet = SOY_SC_Read(m_hDev, readBlock, readBuf);
	ShowScardResult(_T("SOY_SC_Read"), nRet, readBuf, 4);

	CloseScardReaderPort();
}

void CDlgScard::OnBtnUltraLightC()
{
	short				nRet = -1;

	unsigned char		mode = 0;
	unsigned char		cardSN[64] = {0};
	unsigned int		snLen = 0;

	unsigned char		byAuthMode = 0;
	unsigned char		bySectorNum = 1;
	unsigned char		keyA[64] = {0};

	int					i = 0;
	unsigned char		writeBlock = 1;
	unsigned char		writeBuf[512] = {0};

	unsigned char		readBlock = 1;
	unsigned char		readBuf[512] = {0};

	ShowResult(_T("----- UltraLight C -----"));

	if ( !OpenScardReaderPort() )
		return;

	//射频复位
	nRet = SOY_SC_Reset(m_hDev, 1);
	ShowScardResult(_T("SOY_SC_Reset"), nRet, NULL, 0);

	//设置卡型
	nRet = SOY_SC_ConfigCard(m_hDev, 'A');
	ShowScardResult(_T("SOY_SC_ConfigCard"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//寻卡并返回卡序列号
	nRet = SOY_SC_Card_n(m_hDev, mode, &snLen, cardSN);
	ShowScardResult(_T("SOY_SC_Card_n"), nRet, cardSN, snLen);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//验证卡密码
	for(i=0;i<16;i++)
	{
		keyA[i] = 0xFF;
	}
	nRet = SOY_SC_Auth_Ulc(m_hDev, keyA);
	ShowScardResult(_T("SOY_SC_Auth_Ulc"), nRet, keyA, 16);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//写
	for(i=0;i<4;i++)
	{
		writeBuf[i] = rand() % 0xFF;
	}
	nRet = SOY_SC_Write(m_hDev, writeBlock, writeBuf);
	ShowScardResult(_T("SOY_SC_Write"), nRet, writeBuf, 4);

	//读
	nRet = SOY_SC_Read(m_hDev, readBlock, readBuf);
	ShowScardResult(_T("SOY_SC_Read"), nRet, readBuf, 4);

	CloseScardReaderPort();
}

void CDlgScard::OnBtn4442()
{
	int					nRet = -1;

	unsigned char		keyA[64] = {0};

	int					i = 0;
	unsigned char		writeBuf[512] = {0};
	unsigned char		readBuf[512] = {0};
	short				nOffset = 32;
	short				nLen = 50;

	ShowResult(_T("----- 4442 -----"));

	if ( !OpenScardReaderPort() )
		return;

	//验证卡密码
	for(i=0;i<3;i++)
	{
		keyA[i] = 0xFF;
	}
	nRet = SOY_SC_VerifyPin_4442(m_hDev, keyA);
	ShowScardResult(_T("SOY_SC_VerifyPin_4442"), nRet, keyA, 3);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//写
	for(i=0;i<nLen;i++)
	{
		writeBuf[i] = rand() % 0xFF;
	}
	nRet = SOY_SC_Write_4442(m_hDev, nOffset, nLen, writeBuf);
	ShowScardResult(_T("SOY_SC_Write_4442"), nRet, writeBuf, nLen);

	//读
	nRet = SOY_SC_Read_4442(m_hDev, nOffset, nLen, readBuf);
	ShowScardResult(_T("SOY_SC_Read_4442"), nRet, readBuf, nLen);

	CloseScardReaderPort();
}

void CDlgScard::OnBtn4428()
{
	int					nRet = -1;

	unsigned char		keyA[64] = {0};

	int					i = 0;
	unsigned char		writeBuf[512] = {0};
	unsigned char		readBuf[512] = {0};
	short				nOffset = 32;
	short				nLen = 50;

	ShowResult(_T("----- 4428 -----"));

	if ( !OpenScardReaderPort() )
		return;

	//验证卡密码
	for(i=0;i<2;i++)
	{
		keyA[i] = 0xFF;
	}
	nRet = SOY_SC_VerifyPin_4428(m_hDev, keyA);
	ShowScardResult(_T("SOY_SC_VerifyPin_4428"), nRet, keyA, 2);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//写
	for(i=0;i<nLen;i++)
	{
		writeBuf[i] = rand() % 0xFF;
	}
	nRet = SOY_SC_Write_4428(m_hDev, nOffset, nLen, writeBuf);
	ShowScardResult(_T("SOY_SC_Write_4428"), nRet, writeBuf, nLen);

	//读
	nRet = SOY_SC_Read_4428(m_hDev, nOffset, nLen, readBuf);
	ShowScardResult(_T("SOY_SC_Read_4428"), nRet, readBuf, nLen);

	CloseScardReaderPort();
}

void CDlgScard::OnBtn24cxx()
{
	int					nRet = -1;

	int					i = 0;
	unsigned char		writeBuf[512] = {0};
	unsigned char		readBuf[512] = {0};
	short				nOffset = 32;
	short				nLen = 50;

	ShowResult(_T("----- 24CXX -----"));

	if ( !OpenScardReaderPort() )
		return;

	//写
	for(i=0;i<nLen;i++)
	{
		writeBuf[i] = rand() % 0xFF;
	}
	nRet = SOY_SC_Write_24c(m_hDev, nOffset, nLen, writeBuf);
	ShowScardResult(_T("SOY_SC_Write_24c"), nRet, writeBuf, nLen);

	//读
	nRet = SOY_SC_Read_24c(m_hDev, nOffset, nLen, readBuf);
	ShowScardResult(_T("SOY_SC_Read_24c"), nRet, readBuf, nLen);

	CloseScardReaderPort();
}

void CDlgScard::OnBtnCpu()
{
	int					nRet = -1;
	int					i = 0;

	unsigned char		atrLen = 0;
	unsigned char		atrBuf[512] = {0};
	unsigned int		readLen = 0;
	unsigned char		readBuf[512] = {0};

	unsigned int		commandLen = 0;
	unsigned char		commandBuf[512] = {0};

	ShowResult(_T("----- Contact CPU -----"));

	if ( !OpenScardReaderPort() )
		return;

	//设置当前接触式卡座
	nRet = SOY_SC_Set_CPU(m_hDev, 0x0C);
	ShowScardResult(_T("SOY_SC_Set_CPU"), nRet, NULL, 0);
	if ( nRet != 0 )
	{
		CloseScardReaderPort();
		return;
	}

	//接触式CPU卡复位
	nRet = SOY_SC_CpuReset(m_hDev, &atrLen, atrBuf);
	ShowScardResult(_T("SOY_SC_CpuReset"), nRet, atrBuf, atrLen);

	//接触式CPU卡指令交互
	if ( nRet == 0 )
	{
		commandBuf[0] = 0x00;
		commandBuf[1] = 0x84;
		commandBuf[2] = 0x00;
		commandBuf[3] = 0x00;
		commandBuf[4] = 0x08;

		commandLen = 5;

		nRet = SOY_SC_CpuApduInt(m_hDev, commandLen, commandBuf, &readLen, readBuf);
		ShowScardResult(_T("SOY_SC_CpuApduInt"), nRet, readBuf, readLen);
	}

	nRet = SOY_SC_CpuDown(m_hDev);
	ShowScardResult(_T("SOY_SC_CpuDown"), nRet, NULL, 0);

	CloseScardReaderPort();
}

void CDlgScard::OnBtnGetScardReaderVersion()
{
	// TODO: Add your control notification handler code here
	int					nRet = -1;

	unsigned char		version[256] = {0};

	ShowResult(_T("----- Get Reader Version -----"));

	if ( !OpenScardReaderPort() )
		return;

	//读取设备版本号
	nRet = SOY_SC_GetVer(m_hDev, version);
	ShowScardResult(_T("SOY_SC_GetVer"), nRet, NULL, 0);
	if ( nRet == 0 )
	{
		ShowResultA((LPCSTR)version);
	}

	CloseScardReaderPort();
}
void CDlgScard::OnBtnCalibrateScardPosition()
{
	// TODO: Add your control notification handler code here
	uint32_t			dwRet = 0;

	int					i = 0;
	int					nOldPos = 0;
	int					nNewPos = 0;
	int					nMin = -50, nMax = 50;
	int					nSucc = 0;

	uint32_t			dwStatus = 0;

	bool				bFound = false;
	int					nOkStart = 0, nOkEnd = 0;

	TCHAR				szMsg[256] = {0};
	TCHAR				szInfo[256] = {0};
	int					nInfoValue = 0;

	UpdateData(TRUE);

	CloseScardReaderPort();

	//1. check if contact smart card module is attached
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_INSTALLED_MODULE, szInfo);
	ShowResultMessage(dwRet, _T("SOY_PR_GetPrinterInfo"));
	if ( dwRet != 0 )
		return;

	_stprintf(szMsg, _T("INFO_INSTALLED_MODULE => %s"), szInfo);
	ShowResult(szMsg);
	nInfoValue = _ttoi(szInfo);

	if ( (nInfoValue & MODULE_CONTACT_IC) == 0 )
	{
		dwRet = 0x00011030;//The smart card module is not attached.
		ShowResultMessage(dwRet, _T("OnBtnCalibrateScardPosition"));
		return;
	}

	//2. check if printer is ready
	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	ShowResultMessage(dwRet, _T("SOY_PR_GetPrinterStatus"));
	if ( dwRet != 0 )
		return;

	if ( dwStatus != 0 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnCalibrateScardPosition"));
		return;
	}

	//
	for(int k=IDC_COMBO_PORT;k<IDC_EDIT_RESULT;k++)
		GetDlgItem(k)->EnableWindow(FALSE);

	//
	HANDLE				hThread = 0;
	DWORD				dwThreadId = 0;

	hThread = CreateThread(NULL, 0,
							(LPTHREAD_START_ROUTINE)UDoCalibrationThread,
							this,
							CREATE_SUSPENDED,
							&dwThreadId);
	ResumeThread(hThread);
}

void CDlgScard::DoScardPositionCalibration()
{
	uint32_t			dwRet = 0;

	int					i = 0;
	int					nOldPos = 0;
	int					nNewPos = 0;
	int					nMin = -50, nMax = 50;
	int					nSucc = 0;

	uint32_t			dwStatus = 0;

	bool				bFound = false;
	int					nOkStart = 0, nOkEnd = 0;

	TCHAR				szMsg[256] = {0};
	TCHAR				szInfo[256] = {0};
	int					nInfoValue = 0;


	//3. read old position value
	dwRet = SOY_PR_GetPrinterConfig(m_szPrinterName, CONFIG_IC_CARD_POS, &nOldPos);
	ShowResultMessage(dwRet, _T("SOY_PR_GetPrinterConfig"));
	_stprintf(szMsg, _T("CONFIG_IC_CARD_POS => %d"), nOldPos);
	ShowResult(szMsg);

	//4. loop to test IC card
	bFound = false;
	for(i=nMin;i<=nMax;i+=5)
	{
		_stprintf(szMsg, _T("[%d] -----"), i);
		ShowResult(szMsg);

		bFound = false;

		//a. set new position value
		dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_IC_CARD_POS, i);
		ShowResultMessage(dwRet, _T("SOY_PR_SetPrinterConfig"));
		if ( dwRet )
			break;

		//b. move card to contact encoder
		dwRet = SOY_PR_ExecCommand(m_szPrinterName, CMD_MOVE_CARD_TO_CONTACT);
		ShowResultMessage(dwRet, _T("SOY_PR_ExecCommand"));
		if ( dwRet )
			break;

		//c. check if smart card is connected
		bFound = DetectChipCard();
		if ( bFound )
			nSucc++;

		if ( bFound && nSucc == 1 )
			nOkStart = i;
		else if ( bFound )
			nOkEnd = i;

		if ( !bFound && nSucc > 1 )
			break;
	}

	//5. move card to output hopper
	dwRet = SOY_PR_ExecCommand(m_szPrinterName, CMD_MOVE_CARD_TO_HOPPER);
	ShowResultMessage(dwRet, _T("SOY_PR_ExecCommand"));

	//6. set new position value
	if ( nSucc == 1 )
		nNewPos = nOkStart;
	else if ( nSucc > 1 )
		nNewPos = (nOkStart + nOkEnd) / 2;
	else
		nNewPos = nOldPos;

	dwRet = SOY_PR_SetPrinterConfig(m_szPrinterName, CONFIG_IC_CARD_POS, nNewPos);
	ShowResultMessage(dwRet, _T("SOY_PR_SetPrinterConfig"));
	_stprintf(szMsg, _T("CONFIG_IC_CARD_POS => %d"), nNewPos);
	ShowResult(szMsg);

	if ( nSucc == 0 )
		dwRet = 0x00011031;//Calibrate smart card position fail.

	ShowResultMessage(dwRet, _T("OnBtnCalibrateScardPosition"));

	//
	for(int k=IDC_COMBO_PORT;k<IDC_EDIT_RESULT;k++)
		GetDlgItem(k)->EnableWindow(TRUE);
}

bool CDlgScard::DetectChipCard()
{
	bool				bFound = false;

	short				nPort = 0;
	HANDLE				hDev = 0;
	int					nRet = -1;

	unsigned char		readLen = 0;
	unsigned char		readBuf[512] = {0};

	//1. connect card reader
	nPort = m_nPort;
	if ( m_nPort == 0 )//USB
		nPort = 100;

	hDev = SOY_SC_Init(nPort);
	if ( hDev == 0 || hDev == INVALID_HANDLE_VALUE )
	{
		ShowScardResult(_T("SOY_SC_Init"), -1, NULL, 0);
		return false;
	}
	ShowScardResult(_T("SOY_SC_Init"), 0, NULL, 0);

	//2. detect IC card
	if ( m_nScardType == 0 )//CPU
	{
		nRet = SOY_SC_Set_CPU(hDev, 0x0C);
		ShowScardResult(_T("SOY_SC_Set_CPU"), nRet, NULL, 0);
		nRet = SOY_SC_CpuReset(hDev, &readLen, readBuf);
		ShowScardResult(_T("SOY_SC_CpuReset"), nRet, readBuf, readLen);
	}
	else if ( m_nScardType == 1 )//4442
	{
		nRet = SOY_SC_Check_4442(hDev);
		ShowScardResult(_T("SOY_SC_Check_4442"), nRet, NULL, 0);
	}
	else if ( m_nScardType == 2 )//4428
	{
		nRet = SOY_SC_Check_4428(hDev);
		ShowScardResult(_T("SOY_SC_Check_4428"), nRet, NULL, 0);
	}

	if ( nRet == 0 )
		bFound = true;

	//3. disconnect card reader
	nRet = SOY_SC_Exit(hDev);
	ShowScardResult(_T("SOY_SC_Exit"), nRet, NULL, 0);

	return bFound;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void WINAPI UDoCalibrationThread(CDlgScard *cpDlg)
{
	bool				bSucc = true;

	cpDlg->DoScardPositionCalibration();

	ExitThread(0);
}

