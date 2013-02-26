// StatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRender.h"
#include "StatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StatusDlg dialog


StatusDlg::StatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(StatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(StatusDlg)
	m_step = _T("");
	//}}AFX_DATA_INIT
}


void StatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StatusDlg)
	DDX_Text(pDX, IDC_STEP, m_step);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StatusDlg, CDialog)
	//{{AFX_MSG_MAP(StatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StatusDlg message handlers

void StatusDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void StatusDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
