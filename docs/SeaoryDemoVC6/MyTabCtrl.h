#if !defined(AFX_MYTABCTRL_H__34C8E039_2EFE_4C57_BBAF_5476C007F465__INCLUDED_)
#define AFX_MYTABCTRL_H__34C8E039_2EFE_4C57_BBAF_5476C007F465__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyTabCtrl window

class CMyTabCtrl : public CTabCtrl
{
// Construction
public:
	CMyTabCtrl();

// Attributes
public:

	CDialog		*m_cpPages[12];
	int			m_nCurTab;
	int			m_nTotalPages;

// Operations
public:
	void Init();
	void SetRectangle();
	//void SetPrinterName(CString printerName);
	void SetPrinterName(TCHAR* pPrinterName, TCHAR* pDriverName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__34C8E039_2EFE_4C57_BBAF_5476C007F465__INCLUDED_)
