// rpt2lDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rpt2l.h"
#include "rpt2lDlg.h"
#include <fcntl.h> 
#include <io.h> 
#include "HttpManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

typedef int  (*GSDLL_CALLBACK)(int message, char *str, unsigned long count);
typedef int  (WINAPI *gsdll_revision)(char **product, char **copyright, long *gs_revision, long *gs_revisiondate);
typedef int  (WINAPI *gsdll_init)(GSDLL_CALLBACK callback, HWND hwnd, int argc, char *argv[]);
typedef int  (WINAPI *gsdll_execute_begin)(void);
typedef int  (WINAPI *gsdll_execute_cont)(const char *str, int len);
typedef int  (WINAPI *gsdll_execute_end)(void);
typedef int  (WINAPI *gsdll_exit)(void);
typedef int  (WINAPI *gsdll_lock_device)(unsigned char *device, int flag);

typedef int  (WINAPI *gsapi_new_instance)(PVOID callback, char **copyright);
typedef int  (WINAPI *gsapi_init_with_args)(PVOID callback,  int argc, char *argv[]);
typedef int  (WINAPI *gsapi_exit)(PVOID callback);
typedef int  (WINAPI *gsapi_delete_instance)(PVOID callback);



#define GSDLL_STDIN	 	1//	 	get count characters to str from stdin, return number of characters read
#define GSDLL_STDOUT	 	2//	 	put count characters from str to stdout, return number of characters written
#define GSDLL_DEVICE	 	3//	 	device str has been opened if count = 1, closed if count = 0
#define GSDLL_SYNC	 	4//	 	sync_output for device str
#define GSDLL_PAGE	 	5//	 	output_page for device str
#define GSDLL_SIZE	 	6//	 	resize for device str: LOWORD(count) is new xsize, HIWORD(count) is new ysize
#define GSDLL_POLL	 	7//	 	Called from gp_check_interrupt()

int gsdll_callback(int message, char *str, unsigned long count)
{
char *p;
    switch (message) {
        case GSDLL_STDIN:
            p = fgets(str, count, stdin);
            if (p)
                return strlen(str);
            else
                return 0;
        case GSDLL_STDOUT:
            if (str != (char *)NULL)
                fwrite(str, 1, count, stdout);
            return count;
        case GSDLL_DEVICE:
            fprintf(stdout,"Callback: DEVICE %p %s\n", str,
                count ? "open" : "close");
            break;
        case GSDLL_SYNC:
            fprintf(stdout,"Callback: SYNC %p\n", str);
            break;
        case GSDLL_PAGE:
            fprintf(stdout,"Callback: PAGE %p\n", str);
            break;
        case GSDLL_SIZE:
            fprintf(stdout,"Callback: SIZE %p width=%d height=%d\n", str,
                (int)(count & 0xffff), (int)((count>>16) & 0xffff) );
            break;
        case GSDLL_POLL:
            return 0; /* no error */
        default:
            fprintf(stdout,"Callback: Unknown message=%d\n",message);
            break;
    }
    return 0;
}

BOOL WINAPI GhostPsToPdf(char *PsFile, char *PdfFile )
{
    HMODULE hModule = NULL;
    TCHAR exepath[256];
	TCHAR *p;
    /* get path to EXE */
	GetModuleFileName(::AfxGetInstanceHandle(), exepath, sizeof(exepath));
    if ((p = strrchr(exepath,'\\')) != (TCHAR *)NULL)
		p++;
	else
		p = exepath;
    *p = '\0';

	CString DllPath=exepath;
	DllPath+=_T("gsdll32.dll");
	hModule = LoadLibrary(DllPath);
	if (!hModule)
		return FALSE;
 
 
   // *** Declare all the dynamic typedef function pointers
   gsdll_init lp_gsapi_instance ;
   lp_gsapi_instance = (gsdll_init) GetProcAddress(hModule,("gsdll_init"));

   gsdll_execute_begin lp_gsapi_execute_begin ;
   lp_gsapi_execute_begin = (gsdll_execute_begin) GetProcAddress(hModule,("gsdll_execute_begin"));

   gsdll_execute_cont lp_gsapi_execute_cont ;
   lp_gsapi_execute_cont = (gsdll_execute_cont) GetProcAddress(hModule,("gsdll_execute_cont"));

   gsdll_execute_end lp_gsapi_execute_end ;
   lp_gsapi_execute_end = (gsdll_execute_end) GetProcAddress(hModule,("gsdll_execute_end"));

   gsdll_exit lp_gsapi_exit ;
   lp_gsapi_exit = (gsdll_exit) GetProcAddress(hModule,("gsdll_exit"));

   gsdll_lock_device lp_gsapi_device ;
   lp_gsapi_device = (gsdll_lock_device) GetProcAddress(hModule,("gsdll_lock_device"));


 
   char ArgOutputFile[MAX_PATH + 16] = "-sOutputFile=";
   strcat(ArgOutputFile,PdfFile);
 
      // *** Set up the parameters to the engine 
    const char * gsargv[12];
    int gsargc;
    gsargv[0] = "PsToPdf";    /* actual value doesn't matter */
    gsargv[1] = "-dNOPAUSE";  // no prompts
    gsargv[2] = "-dBATCH";    // exit after processing
    gsargv[3] = "-dSAFER";    // Safe mode
    gsargv[4] = "-r720x720";
    gsargv[5] = "-sPAPERSIZE=a4";
    gsargv[6] = "-sDEVICE=pdfwrite"; 
    gsargv[7] = ArgOutputFile;
    gsargv[8] = "-c";
    gsargv[9] = ".setpdfwrite";
    gsargv[10] = "-f";
    gsargv[11] = PsFile;
    gsargc=12;
 
      // *** Simulate pointer to struct with plain char *
      char *Inst = NULL;
      int code = 0;
 
	  code = (lp_gsapi_instance)((GSDLL_CALLBACK)gsdll_callback,::AfxGetMainWnd()->GetSafeHwnd(),gsargc,(char**)gsargv);

	  
      if (code == 0)
      {
		  (lp_gsapi_execute_begin)();

		  char *command = "1 2 add == flush\n";
			code = (lp_gsapi_execute_cont)(command, strlen(command));
			command = "qu";
			code = (lp_gsapi_execute_cont)(command, strlen(command));
			command = "it\n";
			code = (lp_gsapi_execute_cont)(command, strlen(command));

		  (lp_gsapi_execute_end)();

            // Do the conversion
           
           code = (lp_gsapi_exit) ();
      }
 
      if (hModule)
            FreeLibrary(hModule);
 
      if (code == 0)
            return TRUE;
 
      return FALSE;
}
BOOL WINAPI GhostPsToPdfs(char *PsFile, char *PdfFile )
{
    HMODULE hModule = NULL;
    TCHAR exepath[256];
	TCHAR *p;
    /* get path to EXE */
	GetModuleFileName(::AfxGetInstanceHandle(), exepath, sizeof(exepath));
    if ((p = strrchr(exepath,'\\')) != (TCHAR *)NULL)
		p++;
	else
		p = exepath;
    *p = '\0';

	CString DllPath=exepath;
	DllPath+=_T("gsdll32.dll");
	hModule = LoadLibrary(DllPath);
	if (!hModule)
		return FALSE;
 
 
   // *** Declare all the dynamic typedef function pointers

	typedef int  (WINAPI *gsapi_new_instance)(PVOID callback, char **copyright);
typedef int  (WINAPI *gsapi_init_with_args)(PVOID callback,  int argc, char *argv[]);
typedef int  (WINAPI *gsapi_exit)(PVOID callback);
typedef int  (WINAPI *gsapi_delete_instance)(PVOID callback);


   gsapi_new_instance lp_gsapi_instance ;
   lp_gsapi_instance = (gsapi_new_instance) GetProcAddress(hModule,("gsapi_new_instance"));

   gsapi_init_with_args lp_gsapi_init_with_args ;
   lp_gsapi_init_with_args = (gsapi_init_with_args) GetProcAddress(hModule,("gsapi_init_with_args"));

   gsapi_exit lp_gsapi_exit ;
   lp_gsapi_exit = (gsapi_exit) GetProcAddress(hModule,("gsapi_exit"));

   gsapi_delete_instance lp_gsapi_delete_instance ;
   lp_gsapi_delete_instance = (gsapi_delete_instance) GetProcAddress(hModule,("gsapi_delete_instance"));


 
   void *minst;
   char ArgOutputFile[MAX_PATH + 16] = "-sOutputFile=";
   strcat(ArgOutputFile,PdfFile);
 
   int code, code1;
    const char * gsargv[10];
    int gsargc;
      // *** Set up the parameters to the engine 
    gsargv[0] = "ps2pdf";	/* actual value doesn't matter */
    gsargv[1] = "-dNOPAUSE";
    gsargv[2] = "-dBATCH";
    gsargv[3] = "-dSAFER";
    gsargv[4] = "-sDEVICE=pdfwrite";
    gsargv[5] = ArgOutputFile;
    gsargv[6] = "-c";
    gsargv[7] = ".setpdfwrite";
    gsargv[8] = "-f";
    gsargv[9] = PsFile;
    gsargc=10;

    code = (lp_gsapi_instance)(&minst, NULL);
    if (code < 0)
	return 1;
    code = (lp_gsapi_init_with_args)(minst, gsargc, (char**)gsargv);
    code1 = (lp_gsapi_exit)(minst);
    if (code == 0)
		code = code1;

    (lp_gsapi_delete_instance)(minst);

    if ((code == 0) )
		return 0;


}


bool IslastestVer(CString path,CString str){
	CFile file;
	if(file.Open (path,CFile::modeReadWrite)){
		char *buf=new char[file.GetLength ()+1];
		memset(buf,0,file.GetLength ()+1);
		CString record;
		file.Read (buf,file.GetLength ());
		record=buf;
		delete buf;
		CString newrecord=record+","+str;
		file.SeekToBegin ();
		file.Write (newrecord,newrecord.GetLength ());
		file.Close ();
		if(record.Find (str)!=-1)
			return true;
	}
	else
		return false;
	return false;
}

 UINT __cdecl  StartProcss(PVOID pt){
	 ::SetWindowPos(AfxGetMainWnd()->GetSafeHwnd(),HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	 Sleep(100);
	TCHAR exepath[256];
	TCHAR *p;
    /* get path to EXE */
	GetModuleFileName(::AfxGetInstanceHandle(), exepath, sizeof(exepath));
    if ((p = strrchr(exepath,'\\')) != (TCHAR *)NULL)
		p++;
	else
		p = exepath;
    *p = '\0';

	CString DllPath,pdfpath,webrenderpath,updatepath,updaterecord;
	DllPath.Format(_T("%s%d.tmp"),exepath,GetTickCount());
	webrenderpath.Format(_T("%srpt2l2.exe"),exepath);
	pdfpath.Format(_T("%soutput\\%d.pdf"),exepath,GetTickCount());
	updatepath.Format(_T("%soutput\\update.exe"),exepath);
	updaterecord.Format(_T("%s\\vp"),exepath);
	DeleteFile(updatepath);

	FILE *f;
	f=fopen((LPCTSTR)DllPath,(TCHAR *)_T("wb"));

	_setmode(_fileno(stdin), O_BINARY);
	int len=65535;
	char buf[65535];
	while ( (len = fread(buf, 1, sizeof(buf), stdin)) != 0) {
		fwrite(buf, 1, len, f);
	}
	fclose(f);
		
	GhostPsToPdfs((char*)(LPCTSTR)DllPath,(char*)(LPCTSTR)pdfpath);

	DeleteFile(DllPath);

	

	
	AfxGetApp()->m_pMainWnd ->ShowWindow (SW_HIDE);
	
	/**update**/
	//vpu1123:vm.exe
	HttpManager httpmgr;
	CString cookie,error;
	CString ret=httpmgr.GetData(0,"www.elearntech.info","vpu","","",cookie,NULL,0,error);
	if(ret.GetLength ()&&ret.Find (":")!=-1&&ret.Find ("vpu")==0){
		CString ver,url;
		ver=ret.Mid (0,ret.Find (":"));
		url=ret.Mid (ret.Find (":")+1);
	//	save.Format(_T("%soutput\\%s"),exepath,url);
		if(!IslastestVer(updaterecord,ver)){
			if(httpmgr.Download(0,"www.elearntech.info",url,"","",cookie,NULL,0,updatepath,error))
			{
				if(url.Find ("pconfig")!=-1){
					CString pconfigpath;
					pconfigpath.Format(_T("%s\\pconfig"),exepath);
					DeleteFile(pconfigpath);
					::rename (updatepath,pconfigpath);
					
				}
				else
					::ShellExecute (NULL,NULL,updatepath, "",NULL,SW_HIDE);
			}
		}
	}	

	::ShellExecute (NULL,NULL,webrenderpath, "\""+pdfpath+"\"",NULL,SW_SHOW);
	ExitProcess(0);
	return 0;
}

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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRpt2lDlg dialog

CRpt2lDlg::CRpt2lDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRpt2lDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRpt2lDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRpt2lDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRpt2lDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRpt2lDlg, CDialog)
	//{{AFX_MSG_MAP(CRpt2lDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRpt2lDlg message handlers

BOOL CRpt2lDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	AfxBeginThread(StartProcss,NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRpt2lDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRpt2lDlg::OnPaint() 
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
HCURSOR CRpt2lDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRpt2lDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CRpt2lDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}
