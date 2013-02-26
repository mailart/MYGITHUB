// StatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "WebRender.h"
#include "StatusBar.h"
#include "WebRenderDlg.h"
#include "RedeemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DONE _T("WEBPRINTDONE")
/////////////////////////////////////////////////////////////////////////////
// StatusBar dialog


StatusBar::StatusBar(CWnd* pParent /*=NULL*/)
	: CDialog(StatusBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(StatusBar)
	m_step = _T("");
	m_stepparam = _T("");
	m_balance = _T("");
	//}}AFX_DATA_INIT
}


void StatusBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StatusBar)
	DDX_Control(pDX, IDC_BUTTON2, m_topup);
	DDX_Control(pDX, IDC_BUTTON1, m_ok);
	DDX_Text(pDX, IDC_STEP, m_step);
	DDX_Text(pDX, IDC_STEP2, m_stepparam);
	DDX_Text(pDX, IDC_STEP3, m_balance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StatusBar, CDialog)
	//{{AFX_MSG_MAP(StatusBar)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PROGRESS, OnProgress) 
	ON_MESSAGE(WM_PROGRESSERROR, OnProgressError) 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StatusBar message handlers

void StatusBar::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void StatusBar::OnOK() 
{

}

UINT __cdecl  StartProcss(PVOID pt){
	// ::SetWindowPos(AfxGetMainWnd()->GetSafeHwnd(),HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	Sleep(100);
	
	HWND phandle=(HWND)pt;
	theApp.web.SetCallBack (phandle);
	CWebRenderDlg *dlg=(CWebRenderDlg*)theApp.m_pMainWnd ;
	


	if(theApp.web.Login (WEBPRINT_CENTENNIAL)){
		CString printname;
		int n = dlg->m_print.GetLBTextLen (dlg->m_print .GetCurSel ());
		dlg->m_print.GetLBText( dlg->m_print .GetCurSel (), printname.GetBuffer(n) );
		if(theApp.web.Print(printname,theApp.filepath,dlg->m_copy,WEBPRINT_CENTENNIAL)==false)
		{
//			::ShowWindow(::GetDlgItem(phandle,IDC_BUTTON1),SW_SHOW);
			::SendMessage (phandle,WM_PROGRESSERROR,(DWORD)(LPCTSTR)theApp.web.GetErrorStr(),theApp.web .isOutofBalance ()?NULL:(DWORD)"");
			return 0;
		}
		printname.ReleaseBuffer();
	}
	else{
//		::ShowWindow(::GetDlgItem(phandle,IDC_BUTTON1),SW_SHOW);
		::SendMessage (phandle,WM_PROGRESSERROR,(DWORD)(LPCTSTR)theApp.web.GetErrorStr(),(DWORD)"");
		return 0;
	}
	
	::SendMessage (phandle,WM_PROGRESS,(DWORD)DONE,NULL);
//	::ShowWindow(::GetDlgItem(phandle,IDC_BUTTON1),SW_SHOW);
	DeleteFile(theApp.filepath );
	Sleep(4000);
	ExitProcess(0);
	
	return 0;
}
BOOL StatusBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->CenterWindow ();
	::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// TODO: Add extra initialization here
	AfxBeginThread(StartProcss,this->GetSafeHwnd ());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void StatusBar::OnButton1() 
{
	// TODO: Add your control notification handler code here
	DeleteFile(theApp.filepath );
	ExitProcess(0);
	CDialog::OnOK ();
}
LRESULT StatusBar::OnProgress(WPARAM wParam, LPARAM lParam) 
{ 
	
	CString str=(LPCTSTR)wParam;
	CString str2=lParam?(LPCTSTR)lParam:_T("");
	if(str.Find (DONE)==0){//finished
		int pages=0;;
		float cost=0;
		theApp.web.GetRecipt(pages,cost);
		m_balance.Format (_T("Balance: $%.2f (%.2f-%.2f)"),(theApp.web.GetBalance ()-cost),theApp.web.GetBalance (),cost);
	}
	else{
		if(theApp.web.GetBalance ()){
			m_balance.Format (_T("Balance: $%.2f"),theApp.web.GetBalance ());	
		}
		this->m_step =str;
		this->m_stepparam =str2;
	}
	UpdateData(FALSE);
	return 0; 
}
LRESULT StatusBar::OnProgressError(WPARAM wParam, LPARAM lParam) 
{ 
	CString str=(LPCTSTR)wParam;
	CString str2;
	str2=_T("Tips: make sure you have connected with college's network");
	if(theApp.web.isOutofBalance()){
		m_topup.ShowWindow (SW_SHOW);
		
		str2="You can top-up now or do it on website.";
	}
	m_ok.ShowWindow (SW_SHOW);
	
	m_balance="Error";
	this->m_step =str;
	this->m_stepparam =str2;

	

	UpdateData(FALSE);
	return 0; 
}

void StatusBar::OnButton2() 
{
	// TODO: Add your control notification handler code here
	RedeemDlg dlg;
	m_balance=_T("");

	

	if(dlg.DoModal ()==IDOK){
		this->m_topup .ShowWindow (SW_HIDE);
		this->m_ok .ShowWindow (SW_HIDE);
		if(theApp.web.TopUp (dlg.m_num,WEBPRINT_CENTENNIAL)){
			
			AfxBeginThread(StartProcss,this->GetSafeHwnd ());
			
		}
		UpdateData(FALSE);
		//SendMessage (WM_PROGRESSERROR,(DWORD)(LPCTSTR)theApp.web.GetErrorStr(),(DWORD)"");
	}
}
