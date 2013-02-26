//
#pragma once

/*
InternetOpen
InternetConnect
HttpOpenRequest
HttpSendRequest
InternetReadFile
HttpAddRequestHeaders
InternetSetCookie
HttpQueryInfo
*/

class HttpManager{
	
	public:
	CString GetData(int type,CString host,CString url,CString reference,CString contenttype,CString &cookie,PVOID postsparam,DWORD dwpostsize,CString &error);
	/*
		type 
			0 http get
			1 http post
			2 https get
			3 https post
	*/
	bool Download(int type,CString host,CString url,CString reference,CString contenttype,CString &cookie,PVOID postsparam,DWORD dwpostsize,CString savepath,CString &error);



private:
	CString ProcessCookie(CString curcookie);

};