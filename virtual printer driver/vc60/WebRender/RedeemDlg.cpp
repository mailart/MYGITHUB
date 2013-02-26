// RedeemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRender.h"
#include "RedeemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RedeemDlg dialog


RedeemDlg::RedeemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(RedeemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RedeemDlg)
	m_num = _T("");
	//}}AFX_DATA_INIT
}


void RedeemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RedeemDlg)
	DDX_Text(pDX, IDC_USR, m_num);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(RedeemDlg, CDialog)
	//{{AFX_MSG_MAP(RedeemDlg)
	ON_BN_CLICKED(ID_OKS, OnOks)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RedeemDlg message handlers

void RedeemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void RedeemDlg::OnOK() 
{
	// TODO: Add extra validation here
}

void RedeemDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	
}

void RedeemDlg::OnOks() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialog::OnOK();
}
