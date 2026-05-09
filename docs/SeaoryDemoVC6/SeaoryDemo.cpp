// SeaoryDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SeaoryDemo.h"
#include "SeaoryDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

WORD		g_wLangID = 0;
bool		g_bChangeLangRestart = false;
int			g_nOsVer = 0;

/////////////////////////////////////////////////////////////////////////////
// CSeaoryDemoApp

BEGIN_MESSAGE_MAP(CSeaoryDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CSeaoryDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeaoryDemoApp construction

CSeaoryDemoApp::CSeaoryDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSeaoryDemoApp object

CSeaoryDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSeaoryDemoApp initialization

BOOL CSeaoryDemoApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey(_T("Seaory"));

	LoadStdProfileSettings();  // Standard INI-Dateioptionen laden (einschlie羦ich MRU)

	CSeaoryDemoDlg	*cpDlg = 0;
	int			nResponse = 0;

	OSVERSIONINFOEX		osinfo = {0};
	BOOL				bSucc = TRUE;

	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bSucc = GetVersionEx((LPOSVERSIONINFO)&osinfo);
	g_nOsVer = osinfo.dwMajorVersion;

Label_ShowDialog:

	g_wLangID = GetProfileInt(_T("Settings"), _T("Language"), 0);

//#ifdef UNICODE
//+++++++++++++
typedef LANGID (WINAPI * pfnSetThreadUILanguage)(LANGID LangId);
pfnSetThreadUILanguage		fnSetThreadUILanguage = 0;
if ( g_nOsVer >= 6 )
	fnSetThreadUILanguage = (pfnSetThreadUILanguage)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "SetThreadUILanguage");
//+++++++++++++
//#endif

	if ( g_wLangID == 0 )
	{
		LANGID				LangId = 0;
		LangId    = GetUserDefaultUILanguage();

//#ifdef UNICODE
		if ( fnSetThreadUILanguage )
			fnSetThreadUILanguage(LangId);
		else
//#endif
			::SetThreadLocale(MAKELCID(LangId,SORT_DEFAULT));
	}
	else if ( g_wLangID == 1 )
	{
//#ifdef UNICODE
		if ( fnSetThreadUILanguage )
			fnSetThreadUILanguage(MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT));
		else
//#endif
			::SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT),SORT_DEFAULT));
	}
	else if ( g_wLangID == 2 )
	{
//#ifdef UNICODE
		if ( fnSetThreadUILanguage )
			fnSetThreadUILanguage(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
		else
//#endif
			::SetThreadLocale(MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_DEFAULT));
	}

	cpDlg = new CSeaoryDemoDlg();
	m_pMainWnd = 0;
	nResponse = cpDlg->DoModal();

	//CSeaoryDemoDlg dlg;
	//m_pMainWnd = &dlg;
	//int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
		if ( g_bChangeLangRestart )
		{
			delete cpDlg;
			goto Label_ShowDialog;
		}
	}

	delete cpDlg;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
