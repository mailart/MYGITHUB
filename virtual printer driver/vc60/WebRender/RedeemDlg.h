#if !defined(AFX_REDEEMDLG_H__72C6680B_47A1_4D66_B168_068407B193F8__INCLUDED_)
#define AFX_REDEEMDLG_H__72C6680B_47A1_4D66_B168_068407B193F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RedeemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RedeemDlg dialog

class RedeemDlg : public CDialog
{
// Construction
public:
	RedeemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RedeemDlg)
	enum { IDD = IDD_WEBRENDER_DIALOG1 };
	CString	m_num;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RedeemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RedeemDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnOk();
	afx_msg void OnOks();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REDEEMDLG_H__72C6680B_47A1_4D66_B168_068407B193F8__INCLUDED_)
