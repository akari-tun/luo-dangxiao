// DlgPrint2.cpp : implementation file
//

#include "stdafx.h"
#include "seaorydemo.h"
#include "DlgPrint2.h"

#include "SeaoryPrinter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void WINAPI   ShowResultMessage(DWORD dwError, TCHAR* szFuncName);

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint2 dialog


CDlgPrint2::CDlgPrint2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrint2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPrint2)
	m_csBmpYMC1 = _T("");
	m_csBmpK1 = _T("");
	m_csBmpO1 = _T("");
	m_csBmpErase = _T("");
	m_csBmpYMC2 = _T("");
	m_csBmpK2 = _T("");
	m_csBmpO2 = _T("");
	m_nColor1 = 0;
	m_nColor2 = 0;
	m_nColor3 = 0;
	m_nColor4 = 0;
	m_nColor5 = 0;
	m_nColor6 = 0;
	m_nColor7 = 0;
	m_nColor8 = 100;
	m_nHeatY1 = 0;
	m_nHeatK1 = 0;
	m_nHeatO1 = 0;
	m_nHeatR1 = 0;
	m_nHeatErase = 0;
	m_nHeatWrite = 0;
	m_nHeatY2 = 0;
	m_nHeatK2 = 0;
	m_nHeatO2 = 0;
	m_nHeatR2 = 0;
	m_nErasePass = 0;
	m_csBmpErase2 = _T("");
	m_bColor1 = TRUE;
	m_bK1 = TRUE;
	m_bO1 = FALSE;
	m_bErase1 = FALSE;
	m_bColor2 = FALSE;
	m_bK2 = FALSE;
	m_bO2 = FALSE;
	m_bErase2 = FALSE;
	m_dwConcaveDistance = 290;
	m_dwConcaveSpace = 18;
	m_csConcaveDigits = _T("1234 567");
	m_bPrintConcaveDigits = FALSE;
	//}}AFX_DATA_INIT

	memset(m_szPrinterName, 0, sizeof(TCHAR)*128);
	memset(m_szDriverName, 0, sizeof(TCHAR)*128);
	m_bModelRxxx = FALSE;
}


void CDlgPrint2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPrint2)
	DDX_Control(pDX, IDC_COMBO_ERASE_PASS, m_ComboErasePass);
	DDX_Control(pDX, IDC_SPIN_HEAT_R2, m_SpinHeatR2);
	DDX_Control(pDX, IDC_SPIN_HEAT_O2, m_SpinHeatO2);
	DDX_Control(pDX, IDC_SPIN_HEAT_K2, m_SpinHeatK2);
	DDX_Control(pDX, IDC_SPIN_HEAT_Y2, m_SpinHeatY2);
	DDX_Control(pDX, IDC_SPIN_HEAT_WRITE, m_SpinHeatWrite);
	DDX_Control(pDX, IDC_SPIN_HEAT_ERASE, m_SpinHeatErase);
	DDX_Control(pDX, IDC_SPIN_HEAT_R1, m_SpinHeatR1);
	DDX_Control(pDX, IDC_SPIN_HEAT_O1, m_SpinHeatO1);
	DDX_Control(pDX, IDC_SPIN_HEAT_K1, m_SpinHeatK1);
	DDX_Control(pDX, IDC_SPIN_HEAT_Y1, m_SpinHeatY1);
	DDX_Control(pDX, IDC_SPIN_COLOR8, m_SpinColor8);
	DDX_Control(pDX, IDC_SPIN_COLOR7, m_SpinColor7);
	DDX_Control(pDX, IDC_SPIN_COLOR6, m_SpinColor6);
	DDX_Control(pDX, IDC_SPIN_COLOR5, m_SpinColor5);
	DDX_Control(pDX, IDC_SPIN_COLOR4, m_SpinColor4);
	DDX_Control(pDX, IDC_SPIN_COLOR3, m_SpinColor3);
	DDX_Control(pDX, IDC_SPIN_COLOR2, m_SpinColor2);
	DDX_Control(pDX, IDC_SPIN_COLOR1, m_SpinColor1);
	DDX_Control(pDX, IDC_EDIT_COLOR8, m_EditColor8);
	DDX_Control(pDX, IDC_EDIT_COLOR7, m_EditColor7);
	DDX_Control(pDX, IDC_EDIT_COLOR6, m_EditColor6);
	DDX_Control(pDX, IDC_EDIT_COLOR5, m_EditColor5);
	DDX_Control(pDX, IDC_EDIT_COLOR4, m_EditColor4);
	DDX_Control(pDX, IDC_EDIT_COLOR3, m_EditColor3);
	DDX_Control(pDX, IDC_EDIT_COLOR2, m_EditColor2);
	DDX_Control(pDX, IDC_EDIT_COLOR1, m_EditColor1);
	DDX_Text(pDX, IDC_EDIT_PRINT2_COLOR1, m_csBmpYMC1);
	DDX_Text(pDX, IDC_EDIT_PRINT2_K1, m_csBmpK1);
	DDX_Text(pDX, IDC_EDIT_PRINT2_O1, m_csBmpO1);
	DDX_Text(pDX, IDC_EDIT_PRINT2_ERASE, m_csBmpErase);
	DDX_Text(pDX, IDC_EDIT_PRINT2_COLOR2, m_csBmpYMC2);
	DDX_Text(pDX, IDC_EDIT_PRINT2_K2, m_csBmpK2);
	DDX_Text(pDX, IDC_EDIT_PRINT2_O2, m_csBmpO2);
	DDX_Text(pDX, IDC_EDIT_COLOR1, m_nColor1);
	DDV_MinMaxInt(pDX, m_nColor1, -100, 100);
	DDX_Text(pDX, IDC_EDIT_COLOR2, m_nColor2);
	DDV_MinMaxInt(pDX, m_nColor2, -100, 100);
	DDX_Text(pDX, IDC_EDIT_COLOR3, m_nColor3);
	DDV_MinMaxInt(pDX, m_nColor3, 0, 100);
	DDX_Text(pDX, IDC_EDIT_COLOR4, m_nColor4);
	DDV_MinMaxInt(pDX, m_nColor4, -100, 100);
	DDX_Text(pDX, IDC_EDIT_COLOR5, m_nColor5);
	DDV_MinMaxInt(pDX, m_nColor5, -100, 100);
	DDX_Text(pDX, IDC_EDIT_COLOR6, m_nColor6);
	DDV_MinMaxInt(pDX, m_nColor6, -100, 100);
	DDX_Text(pDX, IDC_EDIT_COLOR7, m_nColor7);
	DDV_MinMaxInt(pDX, m_nColor7, -100, 100);
	DDX_Text(pDX, IDC_EDIT_COLOR8, m_nColor8);
	DDV_MinMaxInt(pDX, m_nColor8, 10, 999);
	DDX_Text(pDX, IDC_EDIT_HEAT_Y1, m_nHeatY1);
	DDX_Text(pDX, IDC_EDIT_HEAT_K1, m_nHeatK1);
	DDX_Text(pDX, IDC_EDIT_HEAT_O1, m_nHeatO1);
	DDX_Text(pDX, IDC_EDIT_HEAT_R1, m_nHeatR1);
	DDX_Text(pDX, IDC_EDIT_HEAT_ERASE, m_nHeatErase);
	DDX_Text(pDX, IDC_EDIT_HEAT_WRITE, m_nHeatWrite);
	DDX_Text(pDX, IDC_EDIT_HEAT_Y2, m_nHeatY2);
	DDX_Text(pDX, IDC_EDIT_HEAT_K2, m_nHeatK2);
	DDX_Text(pDX, IDC_EDIT_HEAT_O2, m_nHeatO2);
	DDX_Text(pDX, IDC_EDIT_HEAT_R2, m_nHeatR2);
	DDX_CBIndex(pDX, IDC_COMBO_ERASE_PASS, m_nErasePass);
	DDX_Text(pDX, IDC_EDIT_PRINT2_ERASE2, m_csBmpErase2);
	DDX_Check(pDX, IDC_CHECK_COLOR1, m_bColor1);
	DDX_Check(pDX, IDC_CHECK_K1, m_bK1);
	DDX_Check(pDX, IDC_CHECK_O1, m_bO1);
	DDX_Check(pDX, IDC_CHECK_ERASE1, m_bErase1);
	DDX_Check(pDX, IDC_CHECK_COLOR2, m_bColor2);
	DDX_Check(pDX, IDC_CHECK_K2, m_bK2);
	DDX_Check(pDX, IDC_CHECK_O2, m_bO2);
	DDX_Check(pDX, IDC_CHECK_ERASE2, m_bErase2);
	DDX_Control(pDX, IDC_SPIN_CONCAVE_SPACE, m_SpinConcaveSpace);
	DDX_Control(pDX, IDC_SPIN_CONCAVE_DISTANCE, m_SpinConcaveDistance);
	DDX_Text(pDX, IDC_EDIT_CONCAVE_DISTANCE, m_dwConcaveDistance);
	DDX_Text(pDX, IDC_EDIT_CONCAVE_SPACE, m_dwConcaveSpace);
	DDX_Text(pDX, IDC_EDIT_CONCAVE_DIGITS, m_csConcaveDigits);
	DDV_MaxChars(pDX, m_csConcaveDigits, 16);
	DDX_Check(pDX, IDC_CHECK_PRINT_CONCAVE, m_bPrintConcaveDigits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPrint2, CDialog)
	//{{AFX_MSG_MAP(CDlgPrint2)
	ON_BN_CLICKED(IDC_BTN_PRINT_BYPASS_DRIVER, OnBtnPrintBypassDriver)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_PRINT2_BROWSE_COLOR1, IDC_BTN_PRINT2_BROWSE_O2, OnBtnBrowseImage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPrint2 message handlers

BOOL CDlgPrint2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_SpinColor1.SetRange(-100, 100);
	m_SpinColor2.SetRange(-100, 100);
	m_SpinColor3.SetRange(   0, 100);
	m_SpinColor4.SetRange(-100, 100);
	m_SpinColor5.SetRange(-100, 100);
	m_SpinColor6.SetRange(-100, 100);
	m_SpinColor7.SetRange(-100, 100);
	m_SpinColor8.SetRange(  10, 999);
	m_EditColor1.SetLimitText(4);
	m_EditColor2.SetLimitText(4);
	m_EditColor3.SetLimitText(4);
	m_EditColor4.SetLimitText(4);
	m_EditColor5.SetLimitText(4);
	m_EditColor6.SetLimitText(4);
	m_EditColor7.SetLimitText(4);
	m_EditColor8.SetLimitText(4);

	m_SpinHeatY1.SetRange(-127, 127);
	m_SpinHeatK1.SetRange(-127, 127);
	m_SpinHeatO1.SetRange(-127, 127);
	m_SpinHeatR1.SetRange(-127, 127);
	m_SpinHeatWrite.SetRange(-127, 127);
	m_SpinHeatErase.SetRange(-127, 127);
	m_SpinHeatY2.SetRange(-127, 127);
	m_SpinHeatK2.SetRange(-127, 127);
	m_SpinHeatO2.SetRange(-127, 127);
	m_SpinHeatR2.SetRange(-127, 127);

	int				k = 0;
	CString			temp;

	for(k=IDS_ERASE_PASS0;k<=IDS_ERASE_PASS6;k++)
	{
		temp.LoadString(k);
		m_ComboErasePass.AddString(temp);
	}
	m_ComboErasePass.SetCurSel(0);

	m_SpinConcaveDistance.SetRange(0, 600);
	m_SpinConcaveSpace.SetRange(0, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPrint2::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
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
	if ( m_bModelRxxx )
	{
		nShow = SW_HIDE;
	}

	TCHAR				szPath[512] = {0};
	GetModuleFileName(NULL, szPath, 512);
	*_tcsrchr(szPath, '\\') = '\0';

	m_csBmpYMC1   = szPath;
	m_csBmpK1     = szPath;
	m_csBmpO1     = szPath;
	m_csBmpErase  = szPath;

	m_csBmpYMC2   = szPath;
	m_csBmpK2     = szPath;
	m_csBmpO2     = szPath;
	m_csBmpErase2 = szPath;

	GetDlgItem(IDC_COMBO_ERASE_PASS)->EnableWindow(FALSE);

	if ( !lstrcmp(m_szDriverName, _T("Seaory R300")) || !lstrcmp(m_szDriverName, _T("TSZ-T1 D600-Q"))|| !lstrcmp(m_szDriverName, _T("Imagedec R5000")) || !lstrcmp(m_szDriverName, _T("Seaory R330")) || !lstrcmp(m_szDriverName, _T("Seaory E330")) || !lstrcmp(m_szDriverName, _T("Fagoo P63XX"))
		|| !lstrcmp(szModel, _T("R300")) || !lstrcmp(szModel, _T("D600-Q")) || !lstrcmp(szModel, _T("R5000")) || !lstrcmp(szModel, _T("R330")) || !lstrcmp(szModel, _T("E330")) || !lstrcmp(szModel, _T("P63XX"))
		)
	{
		m_csBmpYMC1 += _T("\\1030x648_bgr1.bmp");
		m_csBmpK1   += _T("\\1030x648_k1.bmp");
		m_csBmpO1    = _T("");

		m_csBmpYMC2 += _T("\\1030x648_bgr2.bmp");
		m_csBmpK2   += _T("\\1030x648_k2.bmp");
		m_csBmpO2    = _T("");

		m_csBmpErase   = _T("");
		m_csBmpErase2  = _T("");
	}
	else if ( m_bModelRxxx )
	{
		m_csBmpYMC1 += _T("\\2060x1296_bgr1.bmp");
		m_csBmpK1   += _T("\\2060x1296_k1.bmp");
		m_csBmpO1    = _T("");

		m_csBmpYMC2 += _T("\\2060x1296_bgr2.bmp");
		m_csBmpK2   += _T("\\2060x1296_k2.bmp");
		m_csBmpO2    = _T("");

		m_csBmpErase   += _T("\\2060x1296_erase.bmp");
		m_csBmpErase2  += _T("\\2060x1296_erase.bmp");
	}
	else if ( !lstrcmp(m_szDriverName, _T("Seaory S20R")) || !lstrcmp(m_szDriverName, _T("Seaory S22R")) || !lstrcmp(szModel, _T("S20R")) || !lstrcmp(szModel, _T("S22R")) )
	{
		m_csBmpYMC1  = _T("");
		m_csBmpK1    = szPath;
		m_csBmpK1   += _T("\\1012x648_k1.bmp");
		m_csBmpO1    = _T("");

		m_csBmpYMC2  = _T("");
		m_csBmpK2    = _T("");
		m_csBmpO2    = _T("");

		m_csBmpErase += _T("\\1012x648_erase.bmp");
		m_csBmpErase2 = _T("");

		GetDlgItem(IDC_COMBO_ERASE_PASS)->EnableWindow(TRUE);
	}
	else if ( !lstrcmp(m_szDriverName, _T("Seaory S22P")) )
	{
		m_csBmpYMC1 += _T("\\1012x648_bgr1.bmp");
		m_csBmpK1   += _T("\\1012x648_k1.bmp");
		m_csBmpO1   += _T("\\1012x648_o1.bmp");

		m_csBmpYMC2 += _T("\\1012x648_bgr2.bmp");
		m_csBmpK2   += _T("\\1012x648_k2.bmp");
		m_csBmpO2   += _T("\\1012x648_o2.bmp");

		m_csBmpErase  += _T("\\1012x648_SecurityErase.bmp");
		m_csBmpErase2 += _T("\\1012x648_SecurityErase.bmp");
	}
	else
	{
		m_csBmpYMC1 += _T("\\1012x648_bgr1.bmp");
		m_csBmpK1   += _T("\\1012x648_k1.bmp");
		m_csBmpO1   += _T("\\1012x648_o1.bmp");

		m_csBmpYMC2 += _T("\\1012x648_bgr2.bmp");
		m_csBmpK2   += _T("\\1012x648_k2.bmp");
		m_csBmpO2   += _T("\\1012x648_o2.bmp");

		m_csBmpErase   = _T("");
		m_csBmpErase2  = _T("");
	}

	UpdateData(FALSE);
}

void CDlgPrint2::OnBtnBrowseImage(UINT nBtnId)
{
	// TODO: Add your control notification handler code here
	CString				OpenFileName = _T("");
	CFileDialog			SrcFileDlg(TRUE, _T("*.*"), _T(""));

	int					i = 0;

	i = nBtnId - IDC_BTN_PRINT2_BROWSE_COLOR1;

	SrcFileDlg.m_ofn.lpstrFilter  = _T("BMP(*.bmp)\0*.bmp\0\0");
	SrcFileDlg.m_ofn.nFilterIndex = 1;
	SrcFileDlg.m_ofn.lpstrInitialDir = _T("C:\\");

	if ( SrcFileDlg.DoModal() == IDOK )
	{
		OpenFileName = (LPCTSTR)SrcFileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_PRINT2_COLOR1+i)->SetWindowText(OpenFileName);
	}
}

void CDlgPrint2::OnBtnPrintBypassDriver()
{
	uint32_t				dwStatus = 0,
						dwError = 0;

	int					nRet = 0;

	BITMAP				BmpYMC1 = {0}, BmpK1 = {0}, BmpO1 = {0};
	BITMAP				BmpYMC2 = {0}, BmpK2 = {0}, BmpO2 = {0};
	BITMAP				BmpErase = {0}, BmpErase2 = {0};

	HBITMAP				hBmpYMC1 = 0, hBmpK1 = 0, hBmpO1 = 0;
	HBITMAP				hBmpYMC2 = 0, hBmpK2 = 0, hBmpO2 = 0;
	HBITMAP				hBmpErase = 0, hBmpErase2 = 0;

	DWORD				dwRet = 0;

	BOOL				bWrongSize = FALSE;
	int					i = 0;

	TCHAR				szMsg[256] = {0};
	CString				temp;

	CString				csExt;

	UpdateData(TRUE);

	if ( m_csBmpYMC1.IsEmpty() && m_csBmpK1.IsEmpty() && m_csBmpYMC2.IsEmpty() && m_csBmpK2.IsEmpty() && m_csBmpErase.IsEmpty() )
	{
		MessageBox(_T("Please select an image file."), _T("OnBtnPrintBypassDriver"), MB_OK|MB_ICONERROR);
		return;
	}

	//check if printer is locked
	TCHAR				szTemp[32] = {0};
	dwRet = SOY_PR_GetPrinterInfo(m_szPrinterName, INFO_SECURITY_MODE, szTemp);
	if ( dwRet == 0 && lstrcmp(szTemp, _T("1")) == 0 )
	{
		ShowResultMessage(0x00013005, _T("OnBtnPrintBypassDriver"));//Printer is locked.
		return;
	}

	dwRet = SOY_PR_GetPrinterStatus(m_szPrinterName, &dwStatus);
	if ( dwStatus != 0 )
	{
		ShowResultMessage(dwStatus, _T("OnBtnPrintBypassDriver"));
		return;
	}

	if ( m_bColor1 && !m_csBmpYMC1.IsEmpty() )
	{
		csExt = m_csBmpYMC1.Mid(m_csBmpYMC1.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Front Color Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpYMC1 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpYMC1, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpYMC1, sizeof(BITMAP), &BmpYMC1);

		if ( !m_bModelRxxx )
		{
		if ( BmpYMC1.bmBits && !( (BmpYMC1.bmWidth == 648 && BmpYMC1.bmHeight == 1012) || (BmpYMC1.bmWidth == 1012 && BmpYMC1.bmHeight == 648) ) )
			bWrongSize = TRUE;
		}
	}

	if ( m_bK1 && !m_csBmpK1.IsEmpty() )
	{
		csExt = m_csBmpK1.Mid(m_csBmpK1.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Front K Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpK1 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpK1, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpK1, sizeof(BITMAP), &BmpK1);

		if ( !m_bModelRxxx )
		{
		if ( BmpK1.bmBits && !( (BmpK1.bmWidth == 648 && BmpK1.bmHeight == 1012) || (BmpK1.bmWidth == 1012 && BmpK1.bmHeight == 648) ) )
			bWrongSize = TRUE;
		}
	}

	if ( bWrongSize )
	{
		MessageBox(_T("Please select an image file with width and height as 648x1012 or 1012x648."), _T("OnBtnPrintBypassDriver"), MB_OK|MB_ICONERROR);
		if ( hBmpYMC1 )
			DeleteObject(hBmpYMC1);

		if ( hBmpK1 )
			DeleteObject(hBmpK1);
		return;
	}

	if ( m_bColor2 && !m_csBmpYMC2.IsEmpty() )
	{
		csExt = m_csBmpYMC2.Mid(m_csBmpYMC2.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Back Color Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpYMC2 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpYMC2, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpYMC2, sizeof(BITMAP), &BmpYMC2);

		if ( !m_bModelRxxx )
		{
		if ( BmpYMC2.bmBits && !( (BmpYMC2.bmWidth == 648 && BmpYMC2.bmHeight == 1012) || (BmpYMC2.bmWidth == 1012 && BmpYMC2.bmHeight == 648) ) )
			bWrongSize = TRUE;
		}
	}

	if ( m_bK2 && !m_csBmpK2.IsEmpty() )
	{
		csExt = m_csBmpK2.Mid(m_csBmpK2.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Back K Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpK2 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpK2, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpK2, sizeof(BITMAP), &BmpK2);

		if ( !m_bModelRxxx )
		{
		if ( BmpK2.bmBits && !( (BmpK2.bmWidth == 648 && BmpK2.bmHeight == 1012) || (BmpK2.bmWidth == 1012 && BmpYMC2.bmHeight == 648) ) )
			bWrongSize = TRUE;
		}
	}

	if ( m_bO1 && !m_csBmpO1.IsEmpty() )
	{
		csExt = m_csBmpO1.Mid(m_csBmpO1.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Back K Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpO1 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpO1, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpO1, sizeof(BITMAP), &BmpO1);

		if ( !m_bModelRxxx )
		{
		if ( BmpO1.bmBits && !( (BmpO1.bmWidth == 648 && BmpO1.bmHeight == 1012) || (BmpO1.bmWidth == 1012 && BmpO1.bmHeight == 648) ) )
			bWrongSize = TRUE;
		}
	}

	if ( m_bO2 && !m_csBmpO2.IsEmpty() )
	{
		csExt = m_csBmpO2.Mid(m_csBmpO2.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Back K Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpO2 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpO2, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpO2, sizeof(BITMAP), &BmpO2);

		if ( !m_bModelRxxx )
		{
		if ( BmpO2.bmBits && !( (BmpO2.bmWidth == 648 && BmpO2.bmHeight == 1012) || (BmpO2.bmWidth == 1012 && BmpO2.bmHeight == 648) ) )
			bWrongSize = TRUE;
		}
	}

	if ( m_bErase1 && !m_csBmpErase.IsEmpty() )
	{
		csExt = m_csBmpErase.Mid(m_csBmpErase.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Back K Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpErase = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpErase, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpErase, sizeof(BITMAP), &BmpErase);

		if ( !m_bModelRxxx )
		{
		if ( BmpErase.bmBits && !( (BmpErase.bmWidth == 648 && BmpErase.bmHeight == 1012) || (BmpErase.bmWidth == 1012 && BmpErase.bmHeight == 648) ) )
			bWrongSize = TRUE;
		}
	}

	if ( m_bErase2 && !m_csBmpErase2.IsEmpty() )
	{
		csExt = m_csBmpErase2.Mid(m_csBmpErase2.GetLength() - 3, 3);
		if ( csExt.CompareNoCase(_T("BMP")) != 0 )
		{
			MessageBox(_T("Please select a BMP file."), _T("Back Erase Image"), MB_OK|MB_ICONERROR);
			return;
		}

		hBmpErase2 = (HBITMAP) LoadImage(0, (LPCTSTR)m_csBmpErase2, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		dwError = GetLastError();
		GetObject(hBmpErase2, sizeof(BITMAP), &BmpErase2);
	}

	if ( hBmpYMC1 == 0 && hBmpK1 == 0 && hBmpYMC2 == 0 && hBmpK2 == 0 && hBmpErase == 0 && hBmpErase2 == 0 )
	{
		MessageBox(_T("Please select an image file."), _T("OnBtnPrintBypassDriver"), MB_OK|MB_ICONERROR);
		return;
	}

	if ( bWrongSize )
	{
		MessageBox(_T("Please select an image file with width and height as 648x1012 or 1012x648."), _T("OnBtnPrintBypassDriver"), MB_OK|MB_ICONERROR);

		if ( hBmpYMC1 )			DeleteObject(hBmpYMC1);
		if ( hBmpK1 )			DeleteObject(hBmpK1);
		if ( hBmpO1 )			DeleteObject(hBmpO1);

		if ( hBmpYMC2 )			DeleteObject(hBmpYMC2);
		if ( hBmpK2 )			DeleteObject(hBmpK2);
		if ( hBmpO2 )			DeleteObject(hBmpO2);

		if ( hBmpErase )		DeleteObject(hBmpErase);
		if ( hBmpErase2 )		DeleteObject(hBmpErase2);
		return;
	}

	PRINT_CARD_PARAM		cardParam = {0};
	memset(&cardParam, 0, sizeof(PRINT_CARD_PARAM));

	cardParam.dwSize = sizeof(PRINT_CARD_PARAM);
	cardParam.byErasePass = m_nErasePass;


	//all the data has 648 pixels width and 1012 pixels height
	//if the data pointer is null, than the plane will not be printed.
	cardParam.lpFrontBGR		= &BmpYMC1;
	cardParam.lpFrontK			= &BmpK1;
	cardParam.lpFrontO			= &BmpO1;

	cardParam.lpBackBGR			= &BmpYMC2;
	cardParam.lpBackK			= &BmpK2;
	cardParam.lpBackO			= &BmpO2;

	cardParam.lpFrontErase		= &BmpErase;
	cardParam.lpBackErase		= &BmpErase2;

	cardParam.ColorAdj[0]		= m_nColor1;
	cardParam.ColorAdj[1]		= m_nColor2;
	cardParam.ColorAdj[2]		= m_nColor3;
	cardParam.ColorAdj[3]		= m_nColor4;
	cardParam.ColorAdj[4]		= m_nColor5;
	cardParam.ColorAdj[5]		= m_nColor6;
	cardParam.ColorAdj[6]		= m_nColor7;
	cardParam.ColorAdj[7]		= m_nColor8;

	//Heating Energy adjustment, value range of following fields are -127 ~ 127
	//same as driver UI Heating Energy tab
	//for ribbon YMCKO
	cardParam.chFrontHeatYMC	= m_nHeatY1;
	cardParam.chFrontHeatK		= m_nHeatK1;
	cardParam.chFrontHeatO		= m_nHeatO1;

	cardParam.chBackHeatYMC		= m_nHeatY2;
	cardParam.chBackHeatK		= m_nHeatK2;
	cardParam.chBackHeatO		= m_nHeatO2;

	//for ribbon K
	cardParam.chFrontHeatResinK	= m_nHeatR1;
	cardParam.chBackHeatResinK	= m_nHeatR2;

	//for rewrite card
	cardParam.chFrontHeatWrite	= m_nHeatWrite;
	cardParam.chFrontHeatErase	= m_nHeatErase;

	char		szDigitsA[32];
	if ( m_bPrintConcaveDigits )
	{
		cardParam.byPrintConcaveDigits = 1;
		cardParam.bySpace = m_dwConcaveSpace;
		cardParam.wDistance = m_dwConcaveDistance;

		memset(szDigitsA, 0, 32);

#ifdef _UNICODE
	int nBytes = 0;
	nBytes = WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)m_csConcaveDigits, -1, szDigitsA, 32, NULL, NULL);
#else
	strcpy(szDigitsA, m_csConcaveDigits);
#endif

		strcpy(cardParam.szDigitsA, szDigitsA);
	}

	dwRet = SOY_PR_PrintOneCard(m_szPrinterName, &cardParam);

	//release image object
	if ( hBmpYMC1 )			DeleteObject(hBmpYMC1);
	if ( hBmpK1 )			DeleteObject(hBmpK1);
	if ( hBmpO1 )			DeleteObject(hBmpO1);

	if ( hBmpYMC2 )			DeleteObject(hBmpYMC2);
	if ( hBmpK2 )			DeleteObject(hBmpK2);
	if ( hBmpO2 )			DeleteObject(hBmpO2);

	if ( hBmpErase )		DeleteObject(hBmpErase);
	if ( hBmpErase2 )		DeleteObject(hBmpErase2);

	ShowResultMessage(dwRet, _T("OnBtnPrintBypassDriver"));
}
