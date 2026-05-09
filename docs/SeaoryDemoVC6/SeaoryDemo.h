// SeaoryDemo.h : main header file for the SEAORYDEMO application
//

#if !defined(AFX_SEAORYDEMO_H__D168F4D2_740F_4BBB_8F20_B7EBDF4196E1__INCLUDED_)
#define AFX_SEAORYDEMO_H__D168F4D2_740F_4BBB_8F20_B7EBDF4196E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSeaoryDemoApp:
// See SeaoryDemo.cpp for the implementation of this class
//

class CSeaoryDemoApp : public CWinApp
{
public:
	CSeaoryDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeaoryDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSeaoryDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEAORYDEMO_H__D168F4D2_740F_4BBB_8F20_B7EBDF4196E1__INCLUDED_)
