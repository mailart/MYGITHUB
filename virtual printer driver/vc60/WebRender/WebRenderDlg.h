// WebRenderDlg.h : header file
//

#if !defined(AFX_WEBRENDERDLG_H__B76E5996_77BF_4323_A01E_6983C8B9B92B__INCLUDED_)
#define AFX_WEBRENDERDLG_H__B76E5996_77BF_4323_A01E_6983C8B9B92B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWebRenderDlg dialog

class CWebRenderDlg : public CDialog
{
// Construction
public:

	CWebRenderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWebRenderDlg)
	enum { IDD = IDD_WEBRENDER_DIALOG };
	CStatic	m_icon;
	CStatic	m_pic;
	CStatic	m_top;
	CComboBox	m_location;
	CComboBox	m_print;
	CComboBox	m_campus;
	int		m_copy;
	CString	m_pwd;
	BOOL	m_remembermark;
	CString	m_usr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebRenderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CWebRenderDlg::RefreshPrintlist();
	void CWebRenderDlg::RefreshAccount();
	void CWebRenderDlg::RefreshLocallist();
	void LoadLastPrint();;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWebRenderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCampus();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeLocation();
	afx_msg void OnAbout();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBRENDERDLG_H__B76E5996_77BF_4323_A01E_6983C8B9B92B__INCLUDED_)
