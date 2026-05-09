// DlgFm1208.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgFm1208.h"

#include "SeaoryScard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct MyPerson
{
	UINT m_userID;
	UINT m_reserved;
	CHAR m_userName[128];
};

/////////////////////////////////////////////////////////////////////////////
// CDlgFm1208 dialog


CDlgFm1208::CDlgFm1208(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFm1208::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFm1208)
	m_nPort = 0;
	//}}AFX_DATA_INIT
	m_hDev = 0;

	m_byApduProtocol = 0;

	memset(m_lpApdu, 0, 2048);
	m_nApduLen = 0;

	memset(m_lpResp, 0, 2048);
	m_nRespLen = 0;
	m_wStatusCode = 0;
}


void CDlgFm1208::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFm1208)
	DDX_Control(pDX, IDC_EDIT_RESULT, m_EditResult);
	DDX_Control(pDX, IDC_EDIT_NAME_R, m_EditNameR);
	DDX_Control(pDX, IDC_EDIT_ID_R, m_EditIdR);
	DDX_Control(pDX, IDC_EDIT_ID_W, m_EditIdW);
	DDX_Control(pDX, IDC_EDIT_NAME_W, m_EditNameW);
	DDX_CBIndex(pDX, IDC_COMBO_PORT_FM1208, m_nPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFm1208, CDialog)
	//{{AFX_MSG_MAP(CDlgFm1208)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_INIT_CARD, OnBtnInitCard)
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnBtnRecover)
	ON_BN_CLICKED(IDC_BTN_WRITE_CARD, OnBtnWriteCard)
	ON_BN_CLICKED(IDC_BTN_READ_CARD, OnBtnReadCard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFm1208 message handlers

BOOL CDlgFm1208::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFm1208::ShowResult(LPCTSTR szText)
{
	int nTxtLen = m_EditResult.GetWindowTextLength();
	m_EditResult.SetSel(nTxtLen, nTxtLen);
	m_EditResult.ReplaceSel(szText);
	m_EditResult.SetSel(nTxtLen, nTxtLen);
	m_EditResult.ReplaceSel(_T("\r\n"));
}

bool CDlgFm1208::InitialScardFunction()
{
	short				nRet = 0;

	short				nPort = 0;

	UpdateData(TRUE);

	if ( m_nPort == 0 )//USB
		nPort = 100;

	m_hDev = SOY_SC_Init(nPort);

	if ( m_hDev == 0 || m_hDev == INVALID_HANDLE_VALUE )
	{
		ShowResult(_T("Init Com Error!"));
		return false;
	}
	ShowResult(_T("Init Com OK!"));

	//nRet = SOY_SC_ResetDevice(m_hDev);

	SOY_SC_Beep(m_hDev, 10);

	return true;
}

void CDlgFm1208::ReleaseScardFunction()
{
	short				nRet = 0;
	if ( m_hDev > 0 )
	{
		//SOY_SC_Beep(m_hDev, 10);
		nRet = SOY_SC_Exit(m_hDev);
		if ( nRet != 0 )
		{
			ShowResult(_T("SOY_SC_Exit Error!"));
		}
		else
		{
			ShowResult(_T("SOY_SC_Exit OK!"));
			m_hDev = (HANDLE)-1;
		}
	}

}

void CDlgFm1208::OnBtnConnect()
{
	// TODO: Add your control notification handler code here
	bool				bRet = false;
	bRet = InitialScardFunction();
	if ( !bRet )
		return;

	for(int i=IDC_BTN_INIT_CARD;i<=IDC_BTN_READ_CARD;i++)
		GetDlgItem(i)->EnableWindow(TRUE);

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(TRUE);
}

void CDlgFm1208::OnBtnDisconnect()
{
	// TODO: Add your control notification handler code here
	ReleaseScardFunction();

		for(int i=IDC_BTN_INIT_CARD;i<=IDC_BTN_READ_CARD;i++)
			GetDlgItem(i)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(FALSE);
}

void CDlgFm1208::OnBtnInitCard()
{
	// TODO: Add your control notification handler code here
	short				nRet = 0;
	UINT				ulSN = 0;
	TCHAR				szLog[512] = {0};

	BYTE				key[16] = {0};

	DWORD				sw = 0;
	BYTE				lpRsp[1024] = { 0 };
	DWORD				dwRspLen = 0;

	nRet = SOY_SC_Reset(m_hDev, 20);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Reset OK."));
	}

	nRet = SOY_SC_Card(m_hDev, 0, &ulSN);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Card Error!"));
		return;
	}
	else
	{
		_stprintf(szLog, _T("SOY_SC_Card() return SN = %ld."), ulSN);
		ShowResult(szLog);
	}

	ShowResult(_T("Starting initialize card\n"));
	BYTE				byLen = 256;
	BYTE				atsBuf[256] = {0};

	nRet = SOY_SC_Pro_Reset(m_hDev, &byLen, atsBuf);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Pro_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Pro_Reset OK."));
	}

	//选择 MF
	sw = SelectMF(lpRsp, &dwRspLen);
	_stprintf(szLog, _T("[%04X] Select MF"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
		return;

	//擦除MF
	memset(key, 0xFF, 16);
	sw = ExternalAuthenticate(0x00, key, 16);
	_stprintf(szLog, _T("[%04X] External Authenticate"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	sw = EraseMF();
	_stprintf(szLog, _T("[%04X] Erase MF"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}	

	//创建密钥文件
	sw = CreateKeyFile(0x0101, 1024, 1, 0xF0);
	_stprintf(szLog, _T("[%04X] Create Key File"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}


	// 添加外部认证密钥
	sw = AddKey(0x00, 0x39, 0xf0, 0xf0, 0xff, 0xff, key, 16);
	_stprintf(szLog, _T("[%04X] Add External Authenticate Key"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	//进行外部认证
	memset(key, 0xFF, 16);
	sw = ExternalAuthenticate(0x00, key, 16);
	_stprintf(szLog, _T("[%04X] External Authenticate"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	// 创建二进制文件
	sw = CreateEF(0x0105, 0x28, 2048, 0xf0, 0xf0, 0xff, 0xff);
	_stprintf(szLog, _T("[%04X] Create EF "), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	ShowResult(_T("Initialize Card Successfully"));
}

void CDlgFm1208::OnBtnRecover()
{
	short				nRet = 0;
	UINT				ulSN = 0;
	TCHAR				szLog[512] = {0};

	BYTE				key[16] = {0xFF};

	DWORD				sw = 0;
	BYTE				lpRsp[1024] = { 0 };
	DWORD				dwRspLen = 0;

	nRet = SOY_SC_Reset(m_hDev, 20);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Reset OK."));
	}

	nRet = SOY_SC_Card(m_hDev, 0, &ulSN);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Card Error!"));
		return;
	}
	else
	{
		_stprintf(szLog, _T("SOY_SC_Card() return SN = %ld."), ulSN);
		ShowResult(szLog);
	}

	ShowResult(_T("Starting Recover Card\n"));
	BYTE				byLen = 256;
	BYTE				atsBuf[256] = {0};

	nRet = SOY_SC_Pro_Reset(m_hDev, &byLen, atsBuf);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Pro_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Pro_Reset OK."));
	}

	//选择 MF
	sw = SelectMF(lpRsp, &dwRspLen);
	_stprintf(szLog, _T("[%04X] Select MF"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
		return;

	//外部认证
	memset(key, 0xFF, 16);
	sw = ExternalAuthenticate(0x00, key, 16);
	_stprintf(szLog, _T("[%04X] External Authenticate"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	//擦除MF
	sw = EraseMF();
	_stprintf(szLog, _T("[%04X] Erase MF"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	//创建密钥文件
	sw = CreateKeyFile(0x0101, 1024, 1, 0xF0);
	_stprintf(szLog, _T("[%04X] Create Key File"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	// 添加外部认证密钥
	//sw = AddKey(0x00, 0x39, 0xf0, 0xf0, 0xaa, 0x88, key, 16);
	sw = AddKey(0x00, 0x39, 0xf0, 0xf0, 0xff, 0xff, key, 16);
	_stprintf(szLog, _T("[%04X] Add External Authenticate Key"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	ShowResult(_T("Recover Card Successfully"));
}

void CDlgFm1208::OnBtnWriteCard()
{
	// TODO: Add your control notification handler code here
	short				nRet = 0;
	UINT				ulSN = 0;
	TCHAR				szLog[512] = {0};

	BYTE				key[16] = {0xFF};

	DWORD				sw = 0;
	BYTE				lpRsp[1024] = { 0 };
	DWORD				dwRspLen = 0;

	nRet = SOY_SC_Reset(m_hDev, 20);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Reset OK."));
	}

	nRet = SOY_SC_Card(m_hDev, 0, &ulSN);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Card Error!"));
		return;
	}
	else
	{
		_stprintf(szLog, _T("SOY_SC_Card() return SN = %ld."), ulSN);
		ShowResult(szLog);
	}

	ShowResult(_T("Starting Write Card\n"));
	BYTE				byLen = 256;
	BYTE				atsBuf[256] = {0};

	nRet = SOY_SC_Pro_Reset(m_hDev, &byLen, atsBuf);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Pro_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Pro_Reset OK."));
	}

	//选择 EF 0x0105
	sw = SelectEF(0x0105, lpRsp, &dwRspLen);
	_stprintf(szLog, _T("[%04X] Select 0x0105"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
		return;

	//外部认证
	memset(key, 0xFF, 16);
	sw = ExternalAuthenticate(0x00, key, 16);
	_stprintf(szLog, _T("[%04X] External Authenticate"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	//写二进制数据
	MyPerson person = {0};

	CString userName;
	person.m_userID = GetDlgItemInt(IDC_EDIT_ID_W);

	GetDlgItemText(IDC_EDIT_NAME_W, userName);

	TCHAR			szNameT[256] = {0};
	char			szNameA[256] = {0};

	lstrcpy(szNameT, userName);
#ifdef UNICODE
	int			nBytes = 0;
	nBytes = WideCharToMultiByte(CP_ACP, 0, szNameT, -1, NULL, 0, NULL, NULL);
	nBytes = WideCharToMultiByte(CP_ACP, 0, szNameT, -1, szNameA, nBytes, NULL, NULL);
#else
	strcpy(szNameA, szNameT);
#endif

	::CopyMemory(person.m_userName, szNameA, min(128, strlen(szNameA)));
	person.m_userName[127] = '\0';

	sw = WriteBinary(0, (BYTE*)&person, sizeof(person));
	_stprintf(szLog, _T("[%04X] Write binary data\n"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	ShowResult(_T("Write Card Successfully"));
}


void CDlgFm1208::OnBtnReadCard()
{
	// TODO: Add your control notification handler code here
	short				nRet = 0;
	UINT				ulSN = 0;
	TCHAR				szLog[512] = {0};

	BYTE				key[16] = {0xFF};

	DWORD				sw = 0;
	BYTE				lpRsp[1024] = { 0 };
	DWORD				dwRspLen = 0;

	nRet = SOY_SC_Reset(m_hDev, 20);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Reset OK."));
	}

	nRet = SOY_SC_Card(m_hDev, 0, &ulSN);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Card Error!"));
		return;
	}
	else
	{
		_stprintf(szLog, _T("SOY_SC_Card() return SN = %ld."), ulSN);
		ShowResult(szLog);
	}

	ShowResult(_T("Starting Read Card\n"));
	BYTE				byLen = 256;
	BYTE				atsBuf[256] = {0};

	nRet = SOY_SC_Pro_Reset(m_hDev, &byLen, atsBuf);
	if ( nRet != 0 )
	{
		ShowResult(_T("SOY_SC_Pro_Reset Error!"));
		return;
	}
	else
	{
		ShowResult(_T("SOY_SC_Pro_Reset OK."));
	}

	//选择 EF 0x0105
	sw = SelectEF(0x0105, lpRsp, &dwRspLen);
	_stprintf(szLog, _T("[%04X] Select 0x0105"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
		return;

	//外部认证
	memset(key, 0xFF, 16);
	sw = ExternalAuthenticate(0x00, key, 16);
	_stprintf(szLog, _T("[%04X] External Authenticate"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	//读二进制数据
	MyPerson person = { 0 };
	sw = ReadBinary(0, (BYTE*)&person, sizeof(MyPerson));
	person.m_userName[127] = '\0';
	_stprintf(szLog, _T("[%04X] Read binary data\n"), sw);
	ShowResult(szLog);
	if (sw != 0x9000)
	{
		return;
	}

	TCHAR			szNameT[256] = {0};
	char			szNameA[256] = {0};

	strcpy(szNameA, person.m_userName);

#ifdef UNICODE
	int			nBytes = 0;
	nBytes = MultiByteToWideChar(CP_ACP, 0, szNameA, -1, NULL, 0);
	nBytes = MultiByteToWideChar(CP_ACP, 0, szNameA, -1, szNameT, nBytes);
#else
	strcpy(szNameT, szNameA);
#endif

	SetDlgItemInt(IDC_EDIT_ID_R, person.m_userID);
	SetDlgItemText(IDC_EDIT_NAME_R, szNameT);

	ShowResult(_T("Read Card Successfully"));
}


void CDlgFm1208::BuildApdu(BYTE byCls, BYTE byIns, BYTE byP1, BYTE byP2, BYTE byLen, BYTE* pData, BYTE byLe)
{
	int				nLen = 5;

	memset(m_lpApdu, 0, 2048);
	memset(m_lpResp, 0, 2048);

	if (byLen > 0)
		nLen += 1 + byLen;
	m_nApduLen = nLen;

	m_lpApdu[0] = byCls;
	m_lpApdu[1] = byIns;
	m_lpApdu[2] = byP1;
	m_lpApdu[3] = byP2;

	if (byLen > 0) {
		m_lpApdu[4] = byLen;
		memcpy(m_lpApdu + 5, pData, byLen);
	}

	m_lpApdu[nLen-1] = byLe;
}

long CDlgFm1208::DoApdu(BYTE byCls, BYTE byIns, BYTE byP1, BYTE byP2, BYTE byLen, BYTE* pData, BYTE byLe)
{
	long			nRet = 0;
//	BYTE			byRsp = 0;
	unsigned int		byRsp = 0;

	BuildApdu(byCls, byIns, byP1, byP2, byLen, pData, byLe);

//	nRet = SOY_SC_Pro_CommandLink(m_hDev, (BYTE)m_nApduLen, m_lpApdu, &byRsp, m_lpResp, 64, 64);
	nRet = SOY_SC_Pro_CommandLinkInt(m_hDev, m_nApduLen, m_lpApdu, &byRsp, m_lpResp, 64);

	m_nRespLen = byRsp;
	m_wStatusCode = MAKEWORD(m_lpResp[byRsp - 1], m_lpResp[byRsp - 2]);

	return nRet;
}

WORD CDlgFm1208::GetChallenge(BYTE* outChallenge, BYTE byLen)
{
	long			nRet = 0;
	nRet = DoApdu(0x00, 0x84, 0x00, 0x00, 0, NULL, byLen);
	if (m_wStatusCode == 0x9000)
	{
		memcpy(outChallenge, m_lpResp, byLen);
	}

	return m_wStatusCode;
}

WORD CDlgFm1208::ExternalAuthenticate(BYTE keyID, const BYTE* key, BYTE KeyLen)
{

	long			nRet = 0;

	//ATLASSERT(KeyLen == 8 || KeyLen == 16);
	BYTE			ch[32] = { 0 };
	BYTE			ec[32] = { 0 };
	WORD			sw = 0;
	BYTE			aKey[64] = {0};
	memcpy(aKey, key, KeyLen);

	TCHAR				szLog[512] = {0};

	sw = GetChallenge(ch, 8);
	_stprintf(szLog, _T("[%04X] GetChallenge"), sw);
	ShowResult(szLog);

	if (sw != 0x9000)
		return sw;

	if (KeyLen == 8)
	{
		nRet = SOY_SC_Des(aKey, ch, ec, TRUE);
		_stprintf(szLog, _T("SOY_SC_Des() return %d"), nRet);
	}
	else
	{
		nRet = SOY_SC_TripleDes(aKey, ch, ec, TRUE);
		_stprintf(szLog, _T("SOY_SC_TripleDes() return %d"), nRet);
	}
	ShowResult(szLog);

	nRet = DoApdu(0x00, 0x82, 0x00, keyID, 8, ec, 0);

	return m_wStatusCode;
}

WORD CDlgFm1208::SelectDF(WORD fileID, char *dfName, BYTE* lpOutData, DWORD* lpdwLen)
{
	long			nRet = 0;

	if (!dfName || !dfName[0])
	{
		//按文件ID选择
		fileID = MAKEWORD(BYTE(fileID >> 8), BYTE(fileID & 0xff));
		nRet = DoApdu(0x00, 0xa4, 0x00, 0x00, 2, (BYTE*)&fileID, 0);
	}
	else
	{
		//按文件名选择
		//ATLASSERT(dfName.GetLength() >= 5 && dfName.GetLength() <= 22);  	//DF 名长度为 5~21字节
		//BuildApdu(0x00, 0xa4, 0x04, 0x00, strlen(dfName), dfName, 0);
	}

	if ( m_wStatusCode == 0x9000 )
	{
		memcpy(lpOutData, m_lpResp, m_nRespLen - 2);
		*lpdwLen = m_nRespLen - 2;
	}

	return m_wStatusCode;
}

WORD CDlgFm1208::SelectMF(BYTE* lpOutData, DWORD* lpdwLen)
{
	return SelectDF(0x3F00, 0, lpOutData, lpdwLen);
}

WORD CDlgFm1208::SelectEF(WORD fileID, BYTE* lpOutData, DWORD* lpdwLen)
{
	long			nRet = 0;

	fileID = (fileID >> 8) | (fileID << 8);
	nRet = DoApdu(0x00, 0xa4, 0x00, 0x00, 2, (BYTE*)&fileID, 0);

	if (m_wStatusCode == 0x9000)
	{
		if ( lpOutData )
			memcpy(lpOutData, m_lpResp, m_nRespLen - 2);
		if ( lpdwLen )
			*lpdwLen = m_nRespLen - 2;
	}

	return m_wStatusCode;
}

/*创建 MF
参数1, 0 正在建立, 1 建立结束, 3 删除 MF
参数2, 建立权限
参数3, 短ID
参数4, 文件名, 银行应用必须为 "1PAY.SYS.DDF01"
返回值 , 状态字
*/
WORD CDlgFm1208::CreateMF(BYTE createPermission, BYTE shortID, char* mfName)// = "1PAY.SYS.DDF01")
{
	long			nRet = 0;
	BYTE			lpData[64] = { 0 };
	int				mfNameLen = strlen(mfName);

	// MF 名称长度应该在 5~16 字节
	if (mfNameLen < 5 || mfNameLen > 16)
		return -1;

	memset(lpData, 0xFF, 8);//8字节传输字节
	lpData[8] = createPermission;
	lpData[9] = shortID;
	memcpy(lpData + 10, mfName, mfNameLen);

	nRet = DoApdu(0x80, 0xe0, 0x00, 0x00, 10 + mfNameLen, lpData, 0);
	return m_wStatusCode;
}

WORD CDlgFm1208::CreateEndMF()
{
	long			nRet = 0;
	BYTE			fileID[] = { 0x3F, 0x00 };

	nRet = DoApdu(0x80, 0xe0, 0x00, 0x01, 2, fileID, 0);
	return m_wStatusCode;
}

WORD CDlgFm1208::EraseMF()
{
	long			nRet = 0;

	nRet = DoApdu(0x80, 0x0E, 0x00, 0x00, 0, 0, 0);
	return m_wStatusCode;
}

WORD CDlgFm1208::DeleteMF()
{
	long			nRet = 0;
	BYTE			fileID[] = { 0x3F, 0x00 };

	nRet = DoApdu(0x80, 0xe0, 0x00, 0x03, 2, fileID, 0);
	return m_wStatusCode;
}

WORD CDlgFm1208::CreateDF(WORD fileID, BYTE createPermission, char* dfName)
{
	long			nRet = 0;
	BYTE			lpData[64] = { 0 };
	int				dfNameLen = strlen(dfName);

	// MF 名称长度应该在 5~16 字节
	if (dfNameLen < 5 || dfNameLen > 16)
		return -1;

	lpData[0] = HIBYTE(fileID);
	lpData[1] = LOBYTE(fileID);
	lpData[2] = createPermission;
	lpData[3] = 0;
	memcpy(lpData + 4, dfName, dfNameLen);

	nRet = DoApdu(0x80, 0xe0, 0x01, 0x00, 4+dfNameLen, lpData, 0);
	return m_wStatusCode;
}

WORD CDlgFm1208::CreateEF(WORD fileID, BYTE byFileType, WORD wFileSpaceSize, BYTE byCreatePermission, BYTE byWritePermission, BYTE B6, BYTE B7)
{
	long			nRet = 0;
	BYTE			lpData[64] = { 0 };

	lpData[0] = byFileType;
	lpData[1] = HIBYTE(wFileSpaceSize);
	lpData[2] = LOBYTE(wFileSpaceSize);
	lpData[3] = byCreatePermission;
	lpData[4] = byWritePermission;
	lpData[5] = B6;
	lpData[6] = B7;

	nRet = DoApdu(0x80, 0xe0, HIBYTE(fileID), LOBYTE(fileID), 7, lpData, 0);
	return m_wStatusCode;
}

WORD CDlgFm1208::CreateKeyFile(WORD fileID, WORD wFileSpaceSize, BYTE shortID, BYTE byAddPermission)
{
	return  CreateEF(fileID, 0x3f, wFileSpaceSize, shortID, byAddPermission, 0xFF, 0xFF);
}

WORD CDlgFm1208::AddKey(BYTE keyID, BYTE keyType, BYTE usesPermission, BYTE motifyPermission, BYTE status, BYTE errorCount, LPCVOID key, BYTE keyLen)
{
/*
	if (keyLen < 2 || keyLen > 16)
	{
		//ATLASSERT(FALSE);    //密钥长度不正确
		return -1;
	}

	//ATLASSERT(keyType != 0x08 || !(status & 0xf0));   // 外部认证密钥的后继状态高半字节最好是0
*/
	long			nRet = 0;
	BYTE			lpData[64] = { 0 };

	lpData[0] = keyType;
	lpData[1] = usesPermission;
	lpData[2] = motifyPermission;
	lpData[3] = status;
	lpData[4] = errorCount;
	memcpy(lpData + 5, key, keyLen);

	nRet = DoApdu(0x80, 0xd4, 0x01, keyID, 5+ keyLen, lpData, 0);
	return m_wStatusCode;
}

WORD CDlgFm1208::ReadBinary(WORD offset, BYTE* lpBuf, WORD wReadBytes)
{
	long			nRet = 0;
	WORD			wBytesToRead = 0, wReadLen = 0, wNewOffset = 0;
	BYTE			*lpTmp = 0;

	lpTmp = lpBuf;
	wNewOffset = offset;
	wBytesToRead = wReadBytes;
	while (wBytesToRead > 0)
	{
		if (wBytesToRead > 100)
			wReadLen = 100;
		else
			wReadLen = wBytesToRead;

		nRet = DoApdu(0x00, 0xb0, HIBYTE(wNewOffset), LOBYTE(wNewOffset), 0, NULL, (BYTE)wReadLen);
		if (m_wStatusCode != 0x9000)
			return m_wStatusCode;

		memcpy(lpTmp, m_lpResp, m_nRespLen - 2);

		lpTmp += wReadLen;
		wNewOffset += wReadLen;
		wBytesToRead -= wReadLen;
	}

	return m_wStatusCode;
}

WORD CDlgFm1208::WriteBinary(WORD offset, BYTE* lpBuf, WORD wWriteBytes)
{
	long			nRet = 0;
	WORD			wBytesToWrite = 0, wWriteLen = 0, wNewOffset = 0;
	BYTE			*lpTmp = 0;

	lpTmp = lpBuf;
	wNewOffset = offset;
	wBytesToWrite = wWriteBytes;
	while (wBytesToWrite > 0)
	{
		if (wBytesToWrite > 128)
			wWriteLen = 128;
		else
			wWriteLen = wBytesToWrite;

		nRet = DoApdu(0x00, 0xd6, HIBYTE(wNewOffset), LOBYTE(wNewOffset), wWriteLen, lpTmp, 0);
		if (m_wStatusCode != 0x9000)
			return m_wStatusCode;

		lpTmp += wWriteLen;
		wNewOffset += wWriteLen;
		wBytesToWrite -= wWriteLen;
	}

	return m_wStatusCode;
}
