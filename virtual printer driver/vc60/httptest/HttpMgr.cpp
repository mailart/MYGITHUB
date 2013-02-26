#include "stdafx.h"
#include "HttpMgr.h"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GetData(int type,CString host,CString url,CString reference,CString cookie,PVOID postsparam,DWORD dwpostsize,CString &error,CString &retcookie){
	HINTERNET m_hInternet=NULL;
	HINTERNET m_hRequest=NULL;
	HINTERNET m_hSession=NULL;
	CString retstr=_T("");
	DWORD ConnFlag=(type<2)?INTERNET_FLAG_RELOAD|INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE:INTERNET_FLAG_RELOAD|INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE
   |INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID;

	try{
		m_hInternet = InternetOpen(_T("Not have agent"), INTERNET_OPEN_TYPE_PRECONFIG , NULL, NULL, 0);
		if (!m_hInternet){
			error=(_T("Cannot open internet"));
			throw  int(9);
			return _T("");
		}
		m_hSession = InternetConnect(m_hInternet,host,(type<2)?80:443,_T(""),_T(""),INTERNET_SERVICE_HTTP,0,0);
		if (!m_hSession){  
			error=_T("Cannot connect to internet");
			throw  int(9);
			return _T("");
		}
		m_hRequest = HttpOpenRequest(m_hSession,(type==0||type==2)?_T("GET"):_T("POST"),url,NULL,reference,NULL,ConnFlag,0);
		if (!m_hRequest) {
			error=_T("Cannot perform http request");
			throw  int(9);
			return _T("");
		}  
	
	
		int result =  HttpSendRequest(m_hRequest,NULL,0,postsparam,dwpostsize);
		if(result)//??????
		{
			DWORD Count;
			char sz[1024];	
			int ret;
			do {
				ret = InternetReadFile(m_hRequest, sz, 1023, &Count);           
				sz[Count] = '\0';
				int x = strlen(sz);
				retstr += sz;
				memset(sz, 0, 1024);
  
			}while(ret && Count != 0);
			return retstr;
		}
		else
		{
			error=_T("Send Request Error!");
			throw  int(9);
			return _T("");
		}
	}
	catch(...) {
		if (m_hInternet) {
			InternetCloseHandle(m_hInternet);
			m_hInternet = NULL;
		}
 
		if (m_hSession) {
			InternetCloseHandle(m_hSession);
			m_hSession = NULL;
		}
		return _T("");
	}

	

	return _T("");
}
bool Download(int type,CString host,CString url,CString reference,CString cookie,PVOID postsparam,DWORD dwpostsize,CString savepath,CString &error,CString &retcookie){
	return true;
}
