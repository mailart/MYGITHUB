#if !defined(AFX_STATUSDLG_H__DB069592_EEC3_4CDD_9E53_AE0CDA7172F7__INCLUDED_)
#define AFX_STATUSDLG_H__DB069592_EEC3_4CDD_9E53_AE0CDA7172F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StatusDlg dialog

class StatusDlg : public CDialog
{
// Construction
public:
	StatusDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(StatusDlg)
	enum { IDD = IDD_STATUS };
	CString	m_step;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(StatusDlg)
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSDLG_H__DB069592_EEC3_4CDD_9E53_AE0CDA7172F7__INCLUDED_)
