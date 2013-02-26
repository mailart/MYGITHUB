// rpt2lDlg.h : header file
//

#if !defined(AFX_RPT2LDLG_H__D4BFC2A9_4A50_4888_97C3_C96035E8D857__INCLUDED_)
#define AFX_RPT2LDLG_H__D4BFC2A9_4A50_4888_97C3_C96035E8D857__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRpt2lDlg dialog

class CRpt2lDlg : public CDialog
{
// Construction
public:
	CRpt2lDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRpt2lDlg)
	enum { IDD = IDD_RPT2L_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRpt2lDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRpt2lDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RPT2LDLG_H__D4BFC2A9_4A50_4888_97C3_C96035E8D857__INCLUDED_)
