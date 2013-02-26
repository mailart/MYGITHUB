// WebRenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WebRender.h"
#include "WebRenderDlg.h"
#include "StatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebRenderDlg dialog

CWebRenderDlg::CWebRenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebRenderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebRenderDlg)
	m_copy = 0;
	m_pwd = _T("");
	m_remembermark = FALSE;
	m_usr = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_copy = 1;
}

void CWebRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebRenderDlg)
	DDX_Control(pDX, IDC_STATICS, m_icon);
	DDX_Control(pDX, IDC_PIC, m_pic);
	DDX_Control(pDX, IDC_TIPS, m_top);
	DDX_Control(pDX, IDC_LOCATION, m_location);
	DDX_Control(pDX, IDC_PRINTER, m_print);
	DDX_Control(pDX, IDC_CAMPUS, m_campus);
	DDX_Text(pDX, IDC_COPY, m_copy);
	DDV_MinMaxInt(pDX, m_copy, 1, 9);
	DDX_Text(pDX, IDC_PWD, m_pwd);
	DDX_Check(pDX, IDC_REMEMBERMARK, m_remembermark);
	DDX_Text(pDX, IDC_USR, m_usr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWebRenderDlg, CDialog)
	//{{AFX_MSG_MAP(CWebRenderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_CAMPUS, OnSelchangeCampus)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_CBN_SELCHANGE(IDC_LOCATION, OnSelchangeLocation)
	ON_BN_CLICKED(IDABOUT, OnAbout)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebRenderDlg message handlers

BOOL CWebRenderDlg::OnInitDialog()
{	

	CString curtime=CTime::GetCurrentTime ().Format (_T("%Y%m"));
	if(curtime.Compare ("201303")>0)
	{
		AfxMessageBox("Demo version has expired, you need to update it");
		ExitProcess(0);
	}

	CDialog::OnInitDialog();
	this->CenterWindow ();
//	::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

		
//	bool ret=web.Login(WEBPRINT_CENTENNIAL);
//	web.Print("PR-LIB-FLOOR2","c:\\2323322.pdf",1,WEBPRINT_CENTENNIAL);

	POSITION pos=theApp.web.pCampus.GetHeadPosition ();
	while(pos){
		PCAMPUSINFO p=(PCAMPUSINFO)theApp.web.pCampus.GetNext (pos);
		
		this->m_campus.SetItemData (this->m_campus .AddString (p->campusinfo),(DWORD)p->p);
	}
	m_campus.SetCurSel (0);
	RefreshLocallist();
	RefreshAccount();
	LoadLastPrint();
	SetForegroundWindow();
BringWindowToTop();
::SetWindowPos(AfxGetMainWnd()->GetSafeHwnd(),HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	SetTimer(0,1000,NULL);
/*
	
	*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CWebRenderDlg::LoadLastPrint(){
	CFile file;
	if(file.Open (GetModulePath()+_T("sel"),CFile::modeReadWrite)){
		CString line="";
		try{
			line=ReadLineFromFile(file.m_hFile );
			if(line.GetLength ()>2)
				line=line.Mid (0,line.GetLength ()-2);

			this->m_campus .SetCurSel (atoi(line));
			line=line.Mid (line.Find (_T(","))+1);
			RefreshLocallist();
			this->m_location .SetCurSel (atoi(line));
			line=line.Mid (line.Find (_T(","))+1);
			
			this->m_print .SetWindowText (line);
		}
		catch(...){}
		file.Close ();
	}
}
void CWebRenderDlg::RefreshAccount(){
	theApp.web.GetAccountInfo(this->m_usr,m_pwd);
	if(m_usr.GetLength ())
		this->m_remembermark=true;
	else
		this->m_remembermark=false;
	UpdateData(FALSE);
}
void CWebRenderDlg::RefreshLocallist(){
	CPtrList *p=(CPtrList*)m_campus.GetItemData (m_campus.GetCurSel ());

	this->m_location .ResetContent();
	POSITION pitem=p->GetHeadPosition();
	while(pitem){
		PLOCATION ploc=(PLOCATION)p->GetNext (pitem);
		this->m_location.SetItemData (m_location.AddString(ploc->building),(DWORD)ploc->p);
	}
	m_location.SetCurSel (0);
	RefreshPrintlist();
}
void CWebRenderDlg::RefreshPrintlist(){
	CPtrList *p=(CPtrList*)m_location.GetItemData (m_location.GetCurSel ());

	this->m_print .ResetContent();
	POSITION pitem=p->GetHeadPosition();
	while(pitem){
		PPRINTERITEM pprint=(PPRINTERITEM)p->GetNext (pitem);
		this->m_print.SetItemData (m_print.AddString(pprint->printname),(DWORD)pprint);
	}
	m_print.SetCurSel (0);
}
void CWebRenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWebRenderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWebRenderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWebRenderDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_usr.GetLength ()==0){
		AfxMessageBox("Please input student ID");
		return;
	}
	if(m_pwd.GetLength ()==0){
		AfxMessageBox("Please input password");
		return;
	}


	theApp.web.SetAccountInfo (m_usr,m_pwd,m_remembermark);

	CString cursel,savestr;
	this->m_print .GetWindowText (cursel);
	savestr.Format ("%d,%d,%s\r\n",this->m_campus .GetCurSel (),this->m_location .GetCurSel (),cursel);
	CFile file;
	if(file.Open (GetModulePath()+_T("sel"),CFile::modeCreate|CFile::modeReadWrite)){
		file.Write (savestr,savestr.GetLength ());
		file.Close ();
	}
	


	ShowWindow(SW_HIDE);

	StatusBar dlg;
	dlg.DoModal ();
}

void CWebRenderDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	DeleteFile(theApp.filepath);
	CDialog::OnCancel();
}

void CWebRenderDlg::OnSelchangeCampus() 
{
	// TODO: Add your control notification handler code here
	RefreshLocallist();
}

void CWebRenderDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	this->m_copy =pNMUpDown->iPos+1;
	if(m_copy>9){
		m_copy=9;
		pNMUpDown->iPos=8;
	}
	UpdateData(FALSE);
	*pResult = 0;
}

void CWebRenderDlg::OnSelchangeLocation() 
{
	// TODO: Add your control notification handler code here
	RefreshPrintlist();
}

void CWebRenderDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal ();
}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CWebRenderDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
//::SetWindowPos(AfxGetMainWnd()->GetSafeHwnd(),HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	 
	if(m_top.IsWindowVisible ()){
		m_top.ShowWindow (SW_HIDE);
		m_icon.ShowWindow (SW_HIDE);
	//	m_pic.ShowWindow (SW_HIDE);
	}
	else{
		m_top.ShowWindow (SW_SHOW);
		m_icon.ShowWindow (SW_SHOW);
	//	m_pic.ShowWindow (SW_SHOW);
	}
	CDialog::OnTimer(nIDEvent);
}
