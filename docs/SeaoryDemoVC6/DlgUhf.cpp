// DlgUhf.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgUhf.h"

#include <afxmt.h>
#include <afxtempl.h>

#include "SeaoryUhf.h"
#pragma comment(lib,"UhfReader_API.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUhf dialog


CDlgUhf::CDlgUhf(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUhf::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUhf)
	m_csPasswdRead = _T("00000000");
	m_csPasswdWrite = _T("00000000");
	m_nAddrRead = 0;
	m_nAddrWrite = 0;
	m_nLenRead = 4;
	m_nPort = 2;
	m_nSectionRead = 0;
	m_nSectionWrite = 0;
	m_nAddrTID = 0;
	m_nLenTID = 12;
	//}}AFX_DATA_INIT
	m_hCom = 0;
}

void CDlgUhf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUhf)
	DDX_Control(pDX, IDC_EDIT_TID, m_EditTID);
	DDX_Control(pDX, IDC_COMBO_SECTION_WRITE, m_ComboSectionWrite);
	DDX_Control(pDX, IDC_COMBO_SECTION_READ, m_ComboSectionRead);
	DDX_Control(pDX, IDC_EDIT_DATA_WRITE, m_EditDataWrite);
	DDX_Control(pDX, IDC_EDIT_DATA_READ, m_EditDataRead);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_EditResult);
	DDX_Control(pDX, IDC_EDIT_EPC, m_EditEPC);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_READ, m_csPasswdRead);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_WRITE, m_csPasswdWrite);
	DDX_Text(pDX, IDC_EDIT_START_ADDR_READ, m_nAddrRead);
	DDX_Text(pDX, IDC_EDIT_START_ADDR_WRITE, m_nAddrWrite);
	DDX_Text(pDX, IDC_EDIT_LENGTH_READ, m_nLenRead);
	DDX_CBIndex(pDX, IDC_COMBO_PORT_UHF, m_nPort);
	DDX_CBIndex(pDX, IDC_COMBO_SECTION_READ, m_nSectionRead);
	DDX_CBIndex(pDX, IDC_COMBO_SECTION_WRITE, m_nSectionWrite);
	DDX_Text(pDX, IDC_EDIT_START_ADDR_TID, m_nAddrTID);
	DDX_Text(pDX, IDC_EDIT_LENGTH_TID, m_nLenTID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUhf, CDialog)
	//{{AFX_MSG_MAP(CDlgUhf)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_READ_DATA, OnBtnReadData)
	ON_BN_CLICKED(IDC_BTN_WRITE_DATA, OnBtnWriteData)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_READ_TID, OnBtnReadTID)
	ON_BN_CLICKED(IDC_BTN_READ_EPC, OnBtnReadEPC)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION_READ, OnSelchangeComboSectionRead)
	ON_CBN_SELCHANGE(IDC_COMBO_SECTION_WRITE, OnSelchangeComboSectionWrite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUhf message handlers

BOOL CDlgUhf::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString			temp;
	int				i = 0;

	for(i=IDS_UHF_SECTION1;i<=IDS_UHF_SECTION4;i++)
	{
		temp.LoadString(i);
		m_ComboSectionRead.AddString(temp);
	}

	temp.LoadString(IDS_UHF_SECTION1);
	m_ComboSectionWrite.AddString(temp);
	temp.LoadString(IDS_UHF_SECTION2);
	m_ComboSectionWrite.AddString(temp);
	temp.LoadString(IDS_UHF_SECTION4);
	m_ComboSectionWrite.AddString(temp);

	m_ComboSectionRead.SetCurSel(0);
	m_ComboSectionWrite.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUhf::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if ( m_hCom > 0 )
		UhfReaderDisconnect(m_hCom, 0);
}

void CDlgUhf::ShowResult(LPCTSTR szText)
{
	int nTxtLen = m_EditResult.GetWindowTextLength();
	m_EditResult.SetSel(nTxtLen, nTxtLen);
	m_EditResult.ReplaceSel(szText);
	m_EditResult.SetSel(nTxtLen, nTxtLen);
	m_EditResult.ReplaceSel(_T("\r\n"));
}

int CDlgUhf::HexStringToByteArray(LPCTSTR szBuf, BYTE* lpbyValue)
{
	const TCHAR				*lpChar = 0;
	BYTE				byValue = 0;
	int					nLen = 0;

	lpChar = szBuf;

	while ( *lpChar != 0 )
	{
		while ( lpChar[0] == ' ' || lpChar[0] == '-' || lpChar[0] == ':' )
			lpChar++;

		if ( *lpChar == 0 )
			return nLen;

		if ( lpChar[0] >= 'A' && lpChar[0] <= 'F' )
			byValue = (BYTE)(lpChar[0] - 'A' + 10);
		else if ( lpChar[0] >= 'a' && lpChar[0] <= 'f' )
			byValue += (BYTE)(lpChar[1] - 'a' + 10);
		else if ( lpChar[0] >= '0' && lpChar[0] <= '9' )
			byValue = (BYTE)(lpChar[0] - '0');
		else
			return nLen;

		byValue <<= 4;
		if ( lpChar[1] >= 'A' && lpChar[1] <= 'F' )
			byValue += (BYTE)(lpChar[1] - 'A' + 10);
		else if ( lpChar[1] >= 'a' && lpChar[1] <= 'f' )
			byValue += (BYTE)(lpChar[1] - 'a' + 10);
    	else if ( lpChar[1] >= '0' && lpChar[1] <= '9' )
			byValue += (BYTE)(lpChar[1] - '0');
		else if ( lpChar[1] == 0 || lpChar[1] == ' ' )
			byValue >>= 4;
		else
			return nLen;

		*lpbyValue = byValue;
		lpbyValue++;
		lpChar+=2;
		nLen++;
	}//end while

	return nLen;
}

CString CDlgUhf::ByteArrayToHexString(BYTE* buf, int startIndex, int length)
{
	CString result;
	CString tmp;

	BYTE	*lpTmp = 0;
	int		i = 0;

	if(length<=0 || buf==NULL)
		return result;

	lpTmp = buf + startIndex;
	for(i=0;i<length;i++,lpTmp++)
	{
		tmp.Format(_T("%02X"), *lpTmp);
		result += tmp;
	}

	return result;
}

void CDlgUhf::OnBtnConnect()
{
	// TODO: Add your control notification handler code here
	int					nRet = 0;
	TCHAR				szLog[512] = {0};
	char				szComPort[16] = {0};

	UpdateData(TRUE);

	sprintf(szComPort, "COM%d", m_nPort+1);
	nRet = UhfReaderConnect(m_hCom, szComPort, 0x06);
	_stprintf(szLog, _T("UhfReaderConnect(COM%d) return %d"), m_nPort+1, nRet);

	if ( m_hCom == 0 || m_hCom == INVALID_HANDLE_VALUE )
	{
		lstrcat(szLog, _T(" => NG"));
		ShowResult(szLog);
		return;
	}
	lstrcat(szLog, _T(" => OK"));
	ShowResult(szLog);

	for(int i=IDC_BTN_DISCONNECT;i<=IDC_BTN_READ_EPC;i++)
		GetDlgItem(i)->EnableWindow(TRUE);

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(FALSE);
	m_EditEPC.SetWindowText(_T(""));
	m_EditTID.SetWindowText(_T(""));
	m_EditDataRead.SetWindowText(_T(""));
	m_EditDataWrite.SetWindowText(_T(""));
}

void CDlgUhf::OnBtnDisconnect()
{
	// TODO: Add your control notification handler code here
	int				nRet = 0;
	if ( m_hCom > 0 )
	{
		nRet = UhfReaderDisconnect(m_hCom, 0);
		ShowResult(_T("UhfReaderDisconnect() => OK"));
		m_hCom = 0;
	}

	for(int i=IDC_BTN_DISCONNECT;i<=IDC_BTN_WRITE_DATA;i++)
		GetDlgItem(i)->EnableWindow(FALSE);

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
}

void CDlgUhf::OnBtnReadEPC()
{
	// TODO: Add your control notification handler code here
	int					nRet = 0;
	TCHAR				szLog[512] = {0};

	BYTE				inventorySingleLen = 0xFF;
	BYTE				inventorySingleBuf[256] = {0};

	TCHAR				szEPC[512] = {0};
	char				szEPCa[512] = {0};
	CString				hex;

	UpdateData(TRUE);

	memset(inventorySingleBuf, 0, inventorySingleLen);//初始化
	nRet = UhfInventorySingleTag(m_hCom, &inventorySingleLen, inventorySingleBuf, 0);
	_stprintf(szLog, _T("UhfInventorySingleTag() return %d"), nRet);

	if ( nRet > 0 )
	{
		lstrcat(szLog, _T(" => OK"));
		ShowResult(szLog);

		hex = ByteArrayToHexString(inventorySingleBuf, 0, inventorySingleLen);
		_stprintf(szLog, _T("Received tag data => %s"), hex);
		ShowResult(szLog);
		//ShowResult(_T("返回的数据含义：PC(2字节)、EPC(len-5字节)、CRC(2字节)、RSSI(1字节)"));

		lstrcpy(szEPC, ByteArrayToHexString(inventorySingleBuf, 2,inventorySingleLen-5));
		m_EditEPC.SetWindowText(szEPC);
	}
	else
	{
		m_EditEPC.SetWindowText(_T(""));
		lstrcat(szLog, _T(" => NG"));
		ShowResult(szLog);
		return;
	}

	memset(szEPCa, 0, 512);
#ifdef _UNICODE
	int			nBytes = 0;
	nBytes = WideCharToMultiByte(CP_ACP, 0, szEPC, -1, NULL, 0, NULL, NULL);
	nBytes = WideCharToMultiByte(CP_ACP, 0, szEPC, -1, szEPCa, nBytes, NULL, NULL);
#else
	strcpy(szEPCa, szEPC);
#endif

	nRet = UhfAddFilter(m_hCom, 0, 0, 1, 32, 32, 0, szEPCa, 0);//使用上面读到的标签数据，进行选中
	_stprintf(szLog, _T("UhfAddFilter() return %d"), nRet);
	if ( nRet == 0 )
	{
		lstrcat(szLog, _T(" => NG"));
		ShowResult(szLog);
		return;
	}

	lstrcat(szLog, _T(" => OK"));
	ShowResult(szLog);

	for(int i=IDC_BTN_READ_TID;i<=IDC_BTN_WRITE_DATA;i++)
		GetDlgItem(i)->EnableWindow(TRUE);
}

void CDlgUhf::OnBtnReadTID()
{
	// TODO: Add your control notification handler code here
	int					nRet = 0;
	TCHAR				szLog[512] = {0};

	TCHAR				szTID[256] = {0};

	BYTE				uDataReturn[256] = {0};
	BYTE				uErrorCode = 0;

	int					nLenEPC = 0;
	int					nOffset = 0;

	UpdateData(TRUE);

	memset(uDataReturn, 0, 256);
	nRet = UhfReadDataByTID(m_hCom, m_nAddrTID, m_nLenTID, uDataReturn, &uErrorCode, 0);
	_stprintf(szLog, _T("UhfReadDataByTID(addr=%d, len=%d) return %d"), m_nAddrTID, m_nLenTID, nRet);

	if ( nRet > 0 )
	{
		lstrcat(szLog, _T(" => OK"));
		ShowResult(szLog);

		lstrcpy(szTID, ByteArrayToHexString(uDataReturn, 0, nRet));
		_stprintf(szLog, _T("+++ TID = %s"), szTID);
		ShowResult(szLog);

		memset(szTID, 0, 256);
		nLenEPC = uDataReturn[0];
		nOffset = 1 + nLenEPC;
		lstrcpy(szTID, ByteArrayToHexString(uDataReturn, nOffset, nRet-nOffset));

		m_EditTID.SetWindowText(szTID);
	}
	else
	{
		m_EditTID.SetWindowText(_T(""));
		lstrcat(szLog, _T(" => NG"));
		ShowResult(szLog);
		return;
	}
}

void CDlgUhf::OnBtnReadData()
{
	// TODO: Add your control notification handler code here
	int					nRet = 0;
	TCHAR				szLog[512] = {0};
	TCHAR				szData[512] = {0};

	BYTE				uDataReturn[256] = {0};
	int					nBank = 0;

	char				szPassA[32] = {0};

	UpdateData(TRUE);

	m_EditDataRead.SetWindowText(_T(""));

	switch(m_nSectionRead)
	{
		case 0:		nBank = UHF_DATA_SECTION_RESERVED;		break;
		case 1:		nBank = UHF_DATA_SECTION_EPC;			break;
		case 2:		nBank = UHF_DATA_SECTION_TID;			break;
		case 3:		nBank = UHF_DATA_SECTION_USER;			break;
	}

#ifdef _UNICODE
	int			nBytes = 0;
	nBytes = WideCharToMultiByte(CP_ACP, 0, m_csPasswdRead, -1, NULL, 0, NULL, NULL);
	nBytes = WideCharToMultiByte(CP_ACP, 0, m_csPasswdRead, -1, szPassA, nBytes, NULL, NULL);
#else
	strcpy(szPassA, m_csPasswdRead);
#endif

	memset(uDataReturn, 0, 256);

	//以下的操作都是基于选中标签状态进行的，前提是成功调用UhfAddFilter选中标签
	nRet = UhfReadDataByEPC(m_hCom, szPassA, nBank, m_nAddrRead, m_nLenRead, uDataReturn, 0);
	_stprintf(szLog, _T("UhfReadDataByEPC(bank=%d, addr=%d, len=%d) return %d"), nBank, m_nAddrRead, m_nLenRead, nRet);

	if ( nRet > 0 )
	{
		lstrcat(szLog, _T(" => OK"));
		ShowResult(szLog);

		lstrcpy(szData, ByteArrayToHexString(uDataReturn, 0, nRet));
		switch(nBank)
		{
			case 0:		lstrcpy(szLog, _T("+++ RESERVED => "));		break;
			case 1:		lstrcpy(szLog, _T("+++ EPC => "));			break;
			case 2:		lstrcpy(szLog, _T("+++ TID => "));			break;
			case 3:		lstrcpy(szLog, _T("+++ USER => "));			break;
		}

		lstrcat(szLog, szData);
		ShowResult(szLog);

		m_EditDataRead.SetWindowText(szData);
	}
	else
	{
		lstrcat(szLog, _T(" => NG"));
		ShowResult(szLog);
	}
}

void CDlgUhf::OnBtnWriteData()
{
	// TODO: Add your control notification handler code here
	int					nRet = 0;
	TCHAR				szLog[512] = {0};
	TCHAR				szData[512] = {0};

	char				szPassA[32] = {0};
	char				szAddrA[256] = {0};
	int					nBank = 0;

	BYTE				uDataWrite[256] = {0};
	int					nDataLen = 0;
	BYTE				uErrorCode = 0;

	UpdateData(TRUE);

	switch(m_nSectionWrite)
	{
		case 0:		nBank = UHF_DATA_SECTION_RESERVED;		break;
		case 1:		nBank = UHF_DATA_SECTION_EPC;			break;
		case 2:		nBank = UHF_DATA_SECTION_USER;			break;
	}

#ifdef _UNICODE
	int			nBytes = 0;
	nBytes = WideCharToMultiByte(CP_ACP, 0, m_csPasswdWrite, -1, NULL, 0, NULL, NULL);
	nBytes = WideCharToMultiByte(CP_ACP, 0, m_csPasswdWrite, -1, szPassA, nBytes, NULL, NULL);
#else
	strcpy(szPassA, m_csPasswdWrite);
#endif

	sprintf(szAddrA, "%04X", m_nAddrWrite);

	CString dataW;
	m_EditDataWrite.GetWindowText(dataW);

	nDataLen = HexStringToByteArray(dataW, uDataWrite);
	if ( nDataLen == 0 )
		return;
/*
	//自动选择设备附近标签操作
	nRet = UhfWriteDataByEPCEx(m_hCom, szPassA, nBank, szAddrA, nDataLen/2, uDataWrite, &uErrorCode, 0);
	_stprintf(szLog, _T("UhfWriteDataByEPCEx(bank=%d, addr=%d, len=%d) return %d, error code = %d."), nBank, m_nAddrWrite, nDataLen/2, nRet, uErrorCode);
*/

	//以下的操作都是基于选中标签状态进行的，前提是成功调用UhfAddFilter选中标签
	nRet = UhfWriteDataByEPC(m_hCom, szPassA, nBank, szAddrA, nDataLen/2, uDataWrite, &uErrorCode, 0);
	_stprintf(szLog, _T("UhfWriteDataByEPC(bank=%d, addr=%d, len=%d) return %d, error code = %d."), nBank, m_nAddrWrite, nDataLen/2, nRet, uErrorCode);
	if ( nRet > 0 )
	{
		lstrcat(szLog, _T(" => OK"));
	}
	else
	{
		lstrcat(szLog, _T(" => NG"));
	}

	ShowResult(szLog);
}

void CDlgUhf::OnSelchangeComboSectionRead()
{
	// TODO: Add your control notification handler code here

}

void CDlgUhf::OnSelchangeComboSectionWrite()
{
	// TODO: Add your control notification handler code here

}
