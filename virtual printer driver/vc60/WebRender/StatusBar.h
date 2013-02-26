#if !defined(AFX_STATUSBAR_H__851555F5_29B5_480D_A3F8_84AE7873F96E__INCLUDED_)
#define AFX_STATUSBAR_H__851555F5_29B5_480D_A3F8_84AE7873F96E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusBar.h : header file
//
#include "WebPrintMgr.h"
/////////////////////////////////////////////////////////////////////////////
// StatusBar dialog

class StatusBar : public CDialog
{
// Construction
public:
	StatusBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(StatusBar)
	enum { IDD = IDD_STATUS };
	CButton	m_topup;
	CButton	m_ok;
	CString	m_step;
	CString	m_stepparam;
	CString	m_balance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StatusBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(StatusBar)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	afx_msg LRESULT OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressError(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSBAR_H__851555F5_29B5_480D_A3F8_84AE7873F96E__INCLUDED_)
