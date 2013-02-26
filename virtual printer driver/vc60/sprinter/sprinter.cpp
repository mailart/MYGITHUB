// sprinter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sprinter.h"
#include "Printmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
	//	cout << (LPCTSTR)strHello << endl;
	}

	
	
	CString first=GetCommandLine();
	if(GetUAC()||first.Find (_T("UACSTART"))!=-1){
		cout << (LPCTSTR)"Virtual Webprint 1.0\r\ngujg2010@gmail.com\r\n"<< endl;
		if(InstallMonitor())
			if(InstallDriver())
				if(InstallPrinter())
					 LinkPdfMgr();

		cout << (LPCTSTR)"Installing Finished\r\n"<< endl;
	}
	else{
	//	::ShellExecute (NULL,NULL,GetModulePath()+"UAC.exe","",NULL,SW_HIDE);
		TCHAR szPath[MAX_PATH];
    
        // Launch itself as administrator.
		::GetModuleFileName (NULL,szPath,MAX_PATH);
	//	CString path=(GetModulePath()+_T("sprinter.exe"));
        SHELLEXECUTEINFO sei = { sizeof(sei) };
        sei.lpVerb = _T("runas");
		sei.lpFile = szPath;
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;
		sei.lpParameters=_T("UACSTART");

        if (!ShellExecuteEx(&sei))
        {
            DWORD dwError = GetLastError();
            if (dwError == ERROR_CANCELLED)
            {
				_tprintf(_T("%s\t"),_T("User refused to allow privileges elevation."));	
				return false;
				// The user refused to allow privileges elevation.
                // Do nothing ...
            }
        }
        else
        {
            return true;
        }
    


		return 0;
	}

	 Sleep(5000);

	return nRetCode;
}


