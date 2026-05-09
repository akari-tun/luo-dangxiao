// MyTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SeaoryDemo.h"
#include "MyTabCtrl.h"

#include "DlgCommand.h"
#include "DlgCardInout.h"
#include "DlgConfig.h"
#include "DlgRetransfer.h"
#include "DlgPrint.h"
#include "DlgPrint2.h"
#include "DlgMagnetic.h"
#include "DlgScard.h"
#include "DlgUhf.h"
#include "DlgFm1208.h"
#include "DlgConcave.h"
#include "DlgSecurity.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl

CMyTabCtrl::CMyTabCtrl()
{
	m_cpPages[0] = new CDlgCommand;
	m_cpPages[1] = new CDlgCardInout;
	m_cpPages[2] = new CDlgConfig;
	m_cpPages[3] = new CDlgRetransfer;
	m_cpPages[4] = new CDlgPrint;
	m_cpPages[5] = new CDlgPrint2;
	m_cpPages[6] = new CDlgMagnetic;
	m_cpPages[7] = new CDlgScard;
	m_cpPages[8] = new CDlgUhf;
	m_cpPages[9] = new CDlgFm1208;
	m_cpPages[10] = new CDlgConcave;
	m_cpPages[11] = new CDlgSecurity;

	m_nTotalPages = 12;
}

CMyTabCtrl::~CMyTabCtrl()
{
	for(int i=0; i < m_nTotalPages; i++){
		delete m_cpPages[i];
	}
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CMyTabCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl message handlers


void CMyTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if ( m_nCurTab != GetCurFocus() )
	{
		m_cpPages[m_nCurTab]->ShowWindow(SW_HIDE);
		m_nCurTab = GetCurFocus();

		m_cpPages[m_nCurTab]->ShowWindow(SW_SHOW);
		m_cpPages[m_nCurTab]->SetFocus();
	}
}

void CMyTabCtrl::Init()
{
	m_nCurTab = 0;

	m_cpPages[0]->Create(IDD_DLG_COMMAND, this);
	m_cpPages[1]->Create(IDD_DLG_CARD_INOUT, this);
	m_cpPages[2]->Create(IDD_DLG_CONFIG, this);
	m_cpPages[3]->Create(IDD_DLG_RETRANSFER, this);
	m_cpPages[4]->Create(IDD_DLG_PRINT, this);
	m_cpPages[5]->Create(IDD_DLG_PRINT2, this);
	m_cpPages[6]->Create(IDD_DLG_MAGNETIC, this);
	m_cpPages[7]->Create(IDD_DLG_SCARD, this);
	m_cpPages[8]->Create(IDD_DLG_UHF, this);
	m_cpPages[9]->Create(IDD_DLG_FM1208, this);
	m_cpPages[10]->Create(IDD_DLG_CONCAVE, this);
	m_cpPages[11]->Create(IDD_DLG_SECURITY, this);

	m_cpPages[0]->ShowWindow(SW_SHOW);
	m_cpPages[1]->ShowWindow(SW_HIDE);
	m_cpPages[2]->ShowWindow(SW_HIDE);
	m_cpPages[3]->ShowWindow(SW_HIDE);
	m_cpPages[4]->ShowWindow(SW_HIDE);
	m_cpPages[5]->ShowWindow(SW_HIDE);
	m_cpPages[6]->ShowWindow(SW_HIDE);
	m_cpPages[7]->ShowWindow(SW_HIDE);
	m_cpPages[8]->ShowWindow(SW_HIDE);
	m_cpPages[9]->ShowWindow(SW_HIDE);
	m_cpPages[10]->ShowWindow(SW_HIDE);
	m_cpPages[11]->ShowWindow(SW_HIDE);

	SetRectangle();
}

//void CMyTabCtrl::SetPrinterName(CString printerName)
void CMyTabCtrl::SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName)
{
	((CDlgCommand*)		m_cpPages[0])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgCardInout*)	m_cpPages[1])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgConfig*)		m_cpPages[2])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgRetransfer*)	m_cpPages[3])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgPrint*)		m_cpPages[4])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgPrint2*)		m_cpPages[5])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgMagnetic*)	m_cpPages[6])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgScard*)		m_cpPages[7])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgUhf*)			m_cpPages[8])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgFm1208*)		m_cpPages[9])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgConcave*)		m_cpPages[10])->SetPrinterName(pPrinterName, pDriverName);
	((CDlgSecurity*)	m_cpPages[11])->SetPrinterName(pPrinterName, pDriverName);
}

void CMyTabCtrl::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX=itemRect.left;
	nY=itemRect.bottom+1;
	nXc=tabRect.right-itemRect.left-1;
	nYc=tabRect.bottom-nY-1;

	m_cpPages[0]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
	for(int nCount=1; nCount < m_nTotalPages; nCount++){
		m_cpPages[nCount]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
	}
}