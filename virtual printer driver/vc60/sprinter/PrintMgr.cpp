
#include "stdafx.h"
#include "PrintMgr.h"
#include "Winspool.h"
#pragma comment(lib, "th32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PRINTNAME TEXT ("Centennial Virtual Wifi Printer V1.0")
#define DRIVERNAME TEXT ("Mailart Postscript")
#define NEWPORTNAME TEXT ("Mailart Virtual Port")


#define MONITORNAME TEXT("Mailart RP")


#define MONITORDLL95 TEXT("redmon95.dll")
#define MONITORDLL35 TEXT("redmon35.dll")
#define MONITORDLLNT TEXT("redmonnt.dll")
#define MONITORDLL32 TEXT("redmon32.dll")
#define MONITORDLL64 TEXT("redmon64.dll")


#define MONITORENV95 TEXT("Windows 4.0")
#define MONITORENVNT TEXT("Windows NT x86")
#define MONITORENV32 TEXT("Windows NT x86")
#define MONITORENV64 TEXT("Windows x64")

#define MONITORHLP TEXT("redmon.chm")
#define MONITORKEY TEXT("Redirection Port Monitor")

#define MAXSTR 1024

typedef struct reconfig_s {
    DWORD dwSize;	/* sizeof this structure */
    DWORD dwVersion;	/* version number of RedMon */
    TCHAR szPortName[MAXSTR];
    TCHAR szDescription[MAXSTR];
    TCHAR szCommand[MAXSTR];
    TCHAR szArguments[MAXSTR];
    TCHAR szPrinter[MAXSTR];
    DWORD dwOutput;
    DWORD dwShow;
    DWORD dwRunUser;
    DWORD dwDelay;
    DWORD dwLogFileUse;
    TCHAR szLogFileName[MAXSTR];
    DWORD dwLogFileDebug;
    DWORD dwPrintError;
};

typedef BOOL (WINAPI *LPFN_GetNativeSystemInfo) 
  (SYSTEM_INFO *);
LPFN_GetNativeSystemInfo fnIsWow64Process;

LPSTR ConvertErrorCodeToString(DWORD ErrorCode)  
{  
	HLOCAL LocalAddress=NULL;  
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_SYSTEM,  
		 NULL,ErrorCode,0,(PTSTR)&LocalAddress,0,NULL);  
	return (LPSTR)LocalAddress;  
}  
wchar_t *GetWideChar(LPCTSTR value){
	wchar_t *text=new wchar_t[strlen(value)*2+2];
	memset(text,0,strlen(value)*2+2);
	::MultiByteToWideChar (CP_ACP,MB_PRECOMPOSED,(LPCTSTR)value,strlen(value),text,strlen(value)+1);
	return text;
}

bool isX64(){
	bool ret=false;
	SYSTEM_INFO info;
	fnIsWow64Process = (LPFN_GetNativeSystemInfo)
        GetProcAddress(GetModuleHandle(_T("kernel32")),("GetNativeSystemInfo"));
    if (fnIsWow64Process != NULL)
        fnIsWow64Process(&info);
    
//	printf("%d\r\n",info.wProcessorArchitecture);
	if (info.wProcessorArchitecture == 9 ||
	  info.wProcessorArchitecture == 6 )
	{
//		printf("%s\r\n","64bits");
		ret=true;
	}
	else
	{
//		printf("%s\r\n","32bits");
		ret=false;
	}

	return ret;
}
CString GetSysPath(){
	
	TCHAR sysdir[256];
	memset(sysdir,0,sizeof(sysdir));
	/* copy files to Windows system directory */
    if (!GetSystemDirectory(sysdir, sizeof(sysdir)))
		return _T("");

	return sysdir;
}
CString GetModulePath(){

	TCHAR exepath[256];
	TCHAR *p;
    /* get path to EXE */
	GetModuleFileName(::AfxGetInstanceHandle(), exepath, sizeof(exepath));
    if ((p = strrchr(exepath,'\\')) != (TCHAR *)NULL)
		p++;
	else
		p = exepath;
    *p = '\0';
	return exepath;
}
BOOL IsNeedUAC()
{
	BOOL isEnableUAC = FALSE;
	OSVERSIONINFO osversioninfo;
	ZeroMemory(&osversioninfo, sizeof(osversioninfo));
	osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);
	BOOL bSuccess = GetVersionEx(&osversioninfo);
	if (bSuccess)
	{
		// window vista or windows server 2008 or later operating system
		if ( osversioninfo.dwMajorVersion > 5 )
		{
			isEnableUAC=TRUE;
		}
	}

	return isEnableUAC;
}

bool GetUAC(){
	if(IsNeedUAC())
		return false;

	return true;
}
bool InstallMonitor(){
	_tprintf(_T("%s\t"),_T("Installing Driver ..."));	

	TCHAR buffer[65536];
	DWORD needed,returned;
	if (EnumMonitors(NULL, 1, (LPBYTE)buffer, sizeof(buffer), 
        &needed, &returned)) {
      MONITOR_INFO_1 *mi;
      mi = (MONITOR_INFO_1 *)buffer;
      for (int i=0; i<returned; i++) {
		 if (lstrcmp(mi[i].pName, MONITORNAME) == 0){
	//		 _tprintf(_T("%s\r\n"),_T("Done."));
			return true;
		 }
      }
   }

	MONITOR_INFO_2 mi2;
	memset(&mi2,0,sizeof(mi2));
	CString source,target;
	mi2.pName = MONITORNAME;

	if(isX64()){
//		printf("%dbit\r\n",64);
		source=GetModulePath()+_T("redmon64.dll");
		target=_T("c:windows\\system32\\redmon.dll");
		CopyFile(source,target,false);
	//		return false;
		mi2.pDLLName = (char*)(LPCTSTR)source;
		mi2.pEnvironment =     MONITORENV64;
	}
	else{
//		printf("%dbit\r\n",32);
		source=GetModulePath()+_T("redmon32.dll");
		target=GetSysPath()+_T("\\redmon.dll");
		CopyFile(source,target,false);
		mi2.pDLLName = (char*) (LPCTSTR)source;
		mi2.pEnvironment =     MONITORENV32;
	}
	


//	_tprintf(_T("%s\r\n"),mi2.pDLLName);	
//	_tprintf(_T("%s\r\n"),mi2.pEnvironment);	
//	_tprintf(_T("%s\r\n"),mi2.pName = MONITORNAME);	

	if (!AddMonitor(NULL, 2, (LPBYTE)&mi2)) {
		_tprintf(_T("%s %s\r\n"),_T("Monitor Error!"),ConvertErrorCodeToString(GetLastError()));
		Sleep(3000);
		return false;
    }
//	_tprintf(_T("%s\r\n"),_T("Done."));	
	return true;
}

void UnstallMonitor(){
	DeleteMonitor (NULL,NULL,MONITORNAME);
}
bool InstallDriver(){
//	_tprintf(_T("%s\t"),_T("Installing Driver ..."));

	TCHAR tPraintDir[65535];
	memset(tPraintDir,0,sizeof(tPraintDir));
	DWORD dwneed=0;
	GetPrinterDriverDirectory(NULL,!isX64()?_T("Windows NT x86"):_T("Windows x64"),1,(LPBYTE)tPraintDir,sizeof(tPraintDir),&dwneed);
	CString drivedestpath=tPraintDir;
	CString sourcedrivepath=GetModulePath()+_T("driver\\");
	sourcedrivepath+=!isX64()?_T("x32\\"):_T("x64\\");
	CopyFile(sourcedrivepath+_T("PS5UI.dll"),drivedestpath+_T("\\PS5UI.dll"),true);
	CopyFile(sourcedrivepath+_T("35C-1U.ppd"),drivedestpath+_T("\\35C-1U.ppd"),true);
	CopyFile(sourcedrivepath+_T("PSCRIPT5.DLL"),drivedestpath+_T("\\PSCRIPT5.DLL"),true);
	
	CString configFile=_T("PS5UI.DLL");;
	CString dataFile=_T("35C-1U.ppd");
	CString driverPath=_T("PSCRIPT5.DLL");
	CString helpFile=_T("PSCRIPT.HLP");
	CString monitorName=MONITORNAME;
	CString driverName=DRIVERNAME;
	TCHAR DependFile[65535];
	memset(DependFile,0,sizeof(DependFile));
	wsprintf(DependFile,_T("%s%c%s%c%s%c%c"),dataFile,(TCHAR)0,configFile,(TCHAR)0,driverPath,(TCHAR)0,(TCHAR)0);

	TCHAR buffer[65536];
	DWORD needed,returned;
	if (EnumPrinterDrivers(NULL, NULL,3, (LPBYTE)buffer, sizeof(buffer), 
        &needed, &returned)) {
      DRIVER_INFO_3 *di3;
      di3 = (DRIVER_INFO_3 *)buffer;
      for (int i=0; i<returned; i++) {
		  if (lstrcmp(di3->pName, driverName) == 0){
			  _tprintf(_T("%s\r\n"),_T("Done."));
			return true;
		  }
		  di3++;
      }
   }


	DRIVER_INFO_3 di3;
    di3.cVersion = 0x03;
	di3.pConfigFile = (char*)(LPCTSTR)configFile;
	di3.pDataFile = (char*)(LPCTSTR)dataFile;
	di3.pDriverPath = (char*)(LPCTSTR)driverPath;
	di3.pHelpFile = NULL;//helpFile.GetBuffer();
	di3.pMonitorName =NULL;//monitorName.GetBuffer();
	di3.pName = (char*)(LPCTSTR)driverName;
	di3.pEnvironment = NULL;//!isX64()?_T("Windows NT x86"):_T("Windows x64");
	di3.pDefaultDataType = _T("RAW");
	di3.pDependentFiles = DependFile;//_T("pscript5.dll\0ps5ui.dll\0pscript.hlp\035C-1U.ppd\0\0");//DependFile;
	



	if (!AddPrinterDriver(NULL, 3, (PBYTE)&di3))
    {
		_tprintf(_T("%s %s\r\n"),_T("Driver Error!"),ConvertErrorCodeToString(GetLastError()));
		Sleep(3000);
       return false;
    }

	_tprintf(_T("%s\r\n"),_T("Done."));

	return true;
}
DWORD CreatePort(LPCTSTR portName)
{
//	_tprintf(_T("%s\t"),_T("\r\nInstalling Port ..."));
	DWORD rslt=0;
	TCHAR buffer[65536];
	PORT_INFO_2 *pi2;
	DWORD needed,returned;
	if (EnumPorts( NULL,2, (LPBYTE)buffer, sizeof(buffer), 
        &needed, &returned)) {
      
      pi2 = (PORT_INFO_2 *)buffer;
      for (int i=0; i<returned; i++) {
		  if(strcmp(pi2->pPortName,portName)==0){
		//	  _tprintf(_T("%s\r\n"),_T("Done."));
			return 0;
		  }
		  pi2++;
      }
   }


      HANDLE hPrinter;
      PRINTER_DEFAULTS PrinterDefaults;
      memset(&PrinterDefaults, 0, sizeof(PrinterDefaults));

      PrinterDefaults.pDatatype = NULL;
      PrinterDefaults.pDevMode = NULL;
      PrinterDefaults.DesiredAccess = SERVER_ACCESS_ADMINISTER;


	  CString redmonname=_T(",XcvMonitor ");
	  redmonname+=MONITORNAME;
	  if (!OpenPrinter((char*)(LPCTSTR)redmonname, &hPrinter, &PrinterDefaults)){
		_tprintf(_T("%s\r\n"),_T("OpenPrinter Error."));
		Sleep(3000);
		return -1;
	  }

	  //printf("%s\r\n",portName);
      DWORD xcvresult= 0;
      if (!XcvData(hPrinter,L"AddPort", (BYTE *)GetWideChar(portName), (strlen(portName)*2 + 2), NULL, 0, &needed, &xcvresult))
            rslt= GetLastError();

      if (!ClosePrinter(hPrinter))
    	  rslt= GetLastError();

	  if(rslt){
	//	_tprintf(_T("%s\r\n"),_T("Error."));
		Sleep(3000);
		return -1;
	  }
	  
	//  _tprintf(_T("%s\r\n"),_T("Done."));


      return rslt;
}
bool InstallPrinter(){

	//AddPort(NULL,::AfxGetMainWnd()->GetSafeHwnd(),MONITORNAME);
	_tprintf(_T("%s\t"),_T("Installing Virtual Printer ..."));

	TCHAR buffer[65536];
	PRINTER_INFO_2 *pinfo2=NULL;
	DWORD needed,returned;
	if (EnumPrinters(PRINTER_ENUM_LOCAL, NULL,2, (LPBYTE)buffer, sizeof(buffer), 
        &needed, &returned)) {
      
      pinfo2 = (PRINTER_INFO_2 *)buffer;
      for (int i=0; i<returned; i++) {
		  if(lstrcmp(pinfo2->pPrinterName,PRINTNAME)==0){//->pName,DRIVERNAME)==0)
			  _tprintf(_T("%s\r\n"),_T("Done."));
			  return true;
		  }
		  pinfo2++;
      }
   }


	DRIVER_INFO_3 *di3=NULL;
	if (EnumPrinterDrivers(NULL, NULL,3, (LPBYTE)buffer, sizeof(buffer), 
        &needed, &returned)) {
      
      di3 = (DRIVER_INFO_3 *)buffer;
      for (int i=0; i<returned; i++) {
		  if(lstrcmp(di3->pName,DRIVERNAME)==0)
			  break;
		  di3++;
      }
   }


	//PrintUIEntry
	int ret;
	if(CreatePort(NEWPORTNAME)){
		
		_tprintf(_T("%s %s\r\n"),_T("Port Error!"),ConvertErrorCodeToString(GetLastError()));
		
		return false;
	}

	PRINTER_INFO_2 pi2;
	memset(&pi2,0,sizeof(pi2));
	pi2.pServerName=NULL;
	pi2.pPrinterName=PRINTNAME;
	pi2.pShareName=NULL;
	pi2.pPortName=NEWPORTNAME;
	pi2.pDriverName=di3->pName;
	pi2.pPrintProcessor=_T("WinPrint");
	pi2.pDatatype	 =  _T("RAW");;

	/*
	pi2.pComment=NULL;
	pi2.pLocation=NULL;
	pi2.pDevMode;
	pi2.pSepFile;

	pi2.pDatatype;
	pi2.pParameters;
	pi2.pSecurityDescriptor;
	pi2.Attributes=PRINTER_ATTRIBUTE_LOCAL|PRINTER_ATTRIBUTE_DIRECT;
	pi2.Priority;
	pi2.DefaultPriority;
	pi2.StartTime;
	pi2.UntilTime;
	pi2.Status;
	pi2.cJobs;
	pi2.AveragePPM;
	*/


	if (!AddPrinter(NULL,2,(PBYTE)&pi2)){
		_tprintf(_T("%s %s\r\n"),_T("Printer Error!"),ConvertErrorCodeToString(GetLastError()));
		Sleep(3000);
		return false;
	}

	_tprintf(_T("%s\r\n"),_T("Done."));
	return true;
}
bool LinkPdfMgr(){
	_tprintf(_T("%s\t"),_T("Configuring  ..."));
	DWORD rslt=0;
	HANDLE hPrinter;
	PRINTER_DEFAULTS PrinterDefaults;
	DWORD needed,returned;
	memset(&PrinterDefaults, 0, sizeof(PrinterDefaults));

	PrinterDefaults.pDatatype = NULL;
	PrinterDefaults.pDevMode = NULL;
	PrinterDefaults.DesiredAccess = SERVER_ACCESS_ADMINISTER;


	CString redmonname=_T(",XcvMonitor ");
	redmonname+=MONITORNAME;
	if (!OpenPrinter((char*)(LPCTSTR)redmonname, &hPrinter, &PrinterDefaults)){
		_tprintf(_T("%s %s\r\n"),_T("Link Error1!"),ConvertErrorCodeToString(GetLastError()));
		Sleep(3000);
		return false;
	}

	reconfig_s config;
	DWORD xcvresult= 0;
	DWORD dwOutput=0;
	if (!XcvData(hPrinter, L"GetConfig", (BYTE *)L"Mailart Virtual Port", (strlen("Mailart Virtual Port")*2 + 1),(PBYTE) &config, sizeof(config), &needed, &xcvresult))
		rslt= GetLastError();

	CString path=GetModulePath()+_T("rpt2l.exe");
	wcscpy((unsigned short*)config.szCommand,GetWideChar((LPCTSTR)path));
	config.dwShow=0;

	if (!XcvData(hPrinter, L"SetConfig", (PBYTE) &config, sizeof(config),(PBYTE)&dwOutput,0, &needed, &xcvresult))
		rslt= GetLastError();

//	if (!XcvData(hPrinter, L"GetConfig", (BYTE *)L"Mailart Virtual Port", (strlen(NEWPORTNAME) + 1)*2,(PBYTE) &config, sizeof(config), &needed, &xcvresult))
//		rslt= GetLastError();

	if (!ClosePrinter(hPrinter))
		rslt= GetLastError();

//	_tprintf(_T("aaaaa-%s\r\n"),config.szCommand);


	if(rslt){
		_tprintf(_T("%s %s\r\n"),_T("Link Error2!"),ConvertErrorCodeToString(GetLastError()));
		Sleep(3000);
		return false;
	}

	_tprintf(_T("%s\r\n"),_T("Done."));
	return true;
}