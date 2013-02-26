#include "stdafx.h"
#include "HttpManager.h"

#include <shlwapi.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "shlwapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

wchar_t *GetWideChar(LPCTSTR value){
	wchar_t *text=new wchar_t[strlen(value)*2+2];
	memset(text,0,strlen(value)*2+2);
	::MultiByteToWideChar (CP_ACP,MB_PRECOMPOSED,(LPCTSTR)value,strlen(value),text,strlen(value)+1);
	return text;
}


CString HttpManager::ProcessCookie(CString curcookie){
	CString retcook="";
	curcookie.Replace(",",";");
	CString lastval="";
	while(curcookie.GetLength()>0){
		CString half="";
		if(curcookie.Find(";")!=-1){
			half=curcookie.Mid(0, curcookie.Find(";")+1);
			curcookie=curcookie.Mid(curcookie.Find(";")+1);
		}
		else{
			half=curcookie;
			curcookie="";
		}
		half.Replace(" ","");
		if(half.Find("=")!=-1){//???
				 
			CString vname=half.Mid(0, half.Find("="));
			CString vvalue=half.Mid(half.Find("=")+1);
			if(vname.CompareNoCase("Comment")!=0&&
					vname.CompareNoCase("Domain")!=0&&
					vname.CompareNoCase("Max-Age")!=0&&
					vname.CompareNoCase("Path")!=0&&
					vname.CompareNoCase("expires")!=0&&
					vname.CompareNoCase("Versions")!=0&&
					vvalue.GetLength()>1
					)
			{
				retcook+=half+" ";
				lastval=half;
			}
				
		}
		else if(half.Find("HttpOnly")!=-1){
//		 retcook=retcook.substring(0, retcook.indexOf(lastval));
		}
			 
	}
		 
	return retcook;
}
CString HttpManager::GetData(int type,CString host,CString url,CString reference,CString contenttype,CString &cookie,PVOID postsparam,DWORD dwpostsize,CString &error){
	HINTERNET m_hInternet=NULL;
	HINTERNET m_hRequest=NULL;
	HINTERNET m_hSession=NULL;
	CString retstr=_T("");
	DWORD ConnFlag=(type<2)?0x10|INTERNET_FLAG_RELOAD|INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE:INTERNET_FLAG_RELOAD|INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_CACHE_WRITE
   |INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|0x10;

	try{
		m_hInternet = InternetOpen(_T("Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)"), INTERNET_OPEN_TYPE_PRECONFIG , NULL, NULL, 0);
		if (!m_hInternet){
			error=(_T("Cannot open internet"));
			throw  int(9);
			return _T("");
		}//
		m_hSession = InternetConnect(m_hInternet,host,(type<2)?9191:443,_T(""),_T(""),INTERNET_SERVICE_HTTP,0,0);
		if (!m_hSession){  
			error=_T("Cannot connect to internet");
			throw  int(9);
			return _T("");
		}//

		int iTimeOut=3000;
		InternetSetOption(m_hSession,INTERNET_OPTION_CONNECT_TIMEOUT, &iTimeOut, sizeof(int));


		m_hRequest = HttpOpenRequest(m_hSession,(type==0||type==2)?_T("GET"):_T("POST"),url,NULL,reference,NULL,ConnFlag,0);
		if (!m_hRequest) {
			error=_T("Cannot perform http request");
			throw  int(9);
			return _T("");
		}  


		struct   
		{   
			char* Accept;  
			char* Language;   
			char* Encoding;   
			char* ContentType;   
		}Headers = {"Accept: text/html, application/xhtml+xml, */*\r\n",
			"Accept-Language:zh-cn\r\n",
                "Accept-Encoding:gzip,deflate\r\n",   
                "Content-Type:application/x-www-form-urlencoded\r\n"};
    

		if(cookie.GetLength())
			HttpAddRequestHeaders(m_hRequest,
			"Cookie: "+cookie+"\r\n", 
                                -1,
                                HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);   

		HttpAddRequestHeaders(m_hRequest,
			Headers.Accept, 
                                -1,
                                HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);   

        HttpAddRequestHeaders(m_hRequest,
                                Headers.Language, 
                                -1,
                                HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);   
    
        HttpAddRequestHeaders(m_hRequest,
                                Headers.Encoding, 
                                -1,
                                HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);   
   

		if(contenttype.GetLength()==0)
			HttpAddRequestHeaders(m_hRequest,
                                Headers.ContentType,
                                -1,
                                HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);   
   
		else
			HttpAddRequestHeaders(m_hRequest,
                                "Content-Type:"+contenttype+"\r\n",
                                -1,
                                HTTP_ADDREQ_FLAG_ADD|HTTP_ADDREQ_FLAG_REPLACE);   
		

	
		
	
		
		
		//Accept: text/html, application/xhtml+xml, */*
		//Accept-Language: zh-CN
//Accept-Encoding: gzip, deflate
//Content-Type: application/x-www-form-urlencoded

		int result =  HttpSendRequest(m_hRequest,NULL,0,postsparam,dwpostsize);
		if(result)//??????
		{

			char szBuff[65535];  
			DWORD dwReadSize = 65535;  
			HttpQueryInfo(m_hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szBuff, &dwReadSize, NULL);  
			szBuff[dwReadSize] = '/0'; 

			char *p=StrStrI(szBuff,"Set-Cookie:");
			if(p!=NULL){
				CString cookieraw=strstr(p,":")+1;
				cookieraw.Replace("\r\n","%^&*");
				cookieraw.Replace("\n","%^&*");
				if(cookieraw.Find("%^&*")!=-1){
					cookieraw=cookieraw.Mid(0,cookieraw.Find("%^&*"));
				}
				cookie=ProcessCookie(cookieraw);

			}
			


			

			DWORD Count=0;
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
			error=_T("Send Request Error! Checking network connection");
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

	

		if (m_hInternet) {
			InternetCloseHandle(m_hInternet);
			m_hInternet = NULL;
		}
 
		if (m_hSession) {
			InternetCloseHandle(m_hSession);
			m_hSession = NULL;
		}
		if(m_hRequest){
			InternetCloseHandle(m_hRequest);
			m_hRequest = NULL;
		}

	return _T("");
}
bool HttpManager::Download(int type,CString host,CString url,CString reference,CString contenttype,CString &cookie,PVOID postsparam,DWORD dwpostsize,CString savepath,CString &error){
	return true;
}
