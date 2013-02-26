#include "stdafx.h"
#include "WebPrintMgr.h"


WebPrintMgr::WebPrintMgr(void)
{
	m_balance=0;
	isremember=false;
	m_hwnd=false;
	ismoneyrunout=false;
}


WebPrintMgr::~WebPrintMgr(void)
{
}

bool WebPrintMgr::Print(LPCTSTR printname,LPCTSTR filepath,int copies,int type){
	//PR-LIB-FLOOR2
	//PR-ITD-P1
	ismoneyrunout=false;
	if(m_hwnd)
		::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Findding Printer...",NULL);


	switch(type){
		case WEBPRINT_CENTENNIAL:{
			CString cookie=m_cookie;
			HttpManager mgr;
			CString poststr=_T(""),ret=_T("");


			/*find print*/
			ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app?service=action/1/UserWebPrint/0/$ActionLink"),_T("http://pracad01.cencol.ca:9191/app?service=page/UserWebPrint"),"",cookie,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);
			poststr="service=direct%2F1%2FUserWebPrintSelectPrinter%2F%24Form&sp=S0&Form0=%24Hidden%2C%24Hidden%240%2C%24TextField%2C%24Submit%2C%24RadioGroup%2C%24Submit%240%2C%24Submit%241&%24Hidden=&%24Hidden%240=&%24TextField=%%PRINT%%&%24Submit=Find+Printer";
			poststr.Replace(_T("%%PRINT%%"),printname);
			cookie=m_cookie;
			ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app"),_T("http://pracad01.cencol.ca:9191/app?service=action/1/UserWebPrint/0/$ActionLink"),"",cookie,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);

			if(ret.Find(_T("Location/Department"))==-1){
				m_error=_T("Format error, Update please");
				return false;
			}

			ret=ret.Mid(ret.Find(_T("Location/Department"))+1);

			if(ret.Find(printname)==-1){
				m_error=_T("Can not find print on webprinter's list, please select another one and try again");
				return false;
			}

			/*choose print*/
			if(m_hwnd)
				::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Selecting Printer...",NULL);


			cookie=m_cookie;
			poststr=_T("service=direct%2F1%2FUserWebPrintSelectPrinter%2F%24Form&sp=S0&Form0=%24Hidden%2C%24Hidden%240%2C%24TextField%2C%24Submit%2C%24RadioGroup%2C%24Submit%240%2C%24Submit%241&%24Hidden=&%24Hidden%240=&%24TextField=%%PRINTNAME%%&%24RadioGroup=0&%24Submit%241=2.+%E6%89%93%E5%8D%B0%E9%80%89%E9%A1%B9%E5%8F%8A%E5%B8%90%E6%88%B7%E9%80%89%E6%8B%A9+%3E%3E");
			poststr.Replace(_T("%%PRINTNAME%%"),printname);
			ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app"),_T("http://pracad01.cencol.ca:9191/app?service=action/1/UserWebPrint/0/$ActionLink"),"",cookie,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);


			/*confirm copies*/
			if(m_hwnd)
				::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Setting Printer...",NULL);



			poststr=_T("service=direct%2F1%2FUserWebPrintOptionsAndAccountSelection%2F%24Form&sp=S0&Form0=copies%2C%24Submit%2C%24Submit%240&copies=%%NUMOFCOPY%%&%24Submit=3.+%E4%B8%8A%E4%BC%A0%E6%89%93%E5%8D%B0%E6%96%87%E4%BB%B6+%3E%3E");
			CString temp;
			temp.Format("%d",copies);
			poststr.Replace(_T("%%NUMOFCOPY%%"),temp);
			cookie=m_cookie;
			ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app"),_T("http://pracad01.cencol.ca:9191/app"),"",cookie,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);
			if(ret.Find(_T("uploadFormSubmitURL = '"))==-1){
				m_error=_T("Can not find Cmd, Update please");
				return false;
			}

			/*upload*/
			if(m_hwnd)
				::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Uploading Data...",NULL);


			CString posturl=ret.Mid(ret.Find(_T("uploadFormSubmitURL = '"))+strlen(_T("uploadFormSubmitURL = '")));
			posturl=posturl.Mid(0,posturl.Find("'"));
			CString boundary="----NetTransmitterStudio_Mailart_19810607";
			CString uploadfile="";

			CString item1="Content-Disposition: form-data; name=\"service\"\r\n\r\ndirect/1/UserWebPrintUpload/$Form\r\n";
			CString item2="Content-Disposition: form-data; name=\"sp\"\r\n\r\nS0\r\n";
			CString item3="Content-Disposition: form-data; name=\"Form0\"\r\n\r\n$Submit,$Submit$0\r\n";
			CString item4="Content-Disposition: form-data; name=\"file\"; filename=\"MailartVirtualWebPrint.pdf\"\r\nContent-Type: application/pdf\r\n\r\n";
			CString item5="Content-Disposition: form-data; name=\"$Submit$0\"\r\n\r\nUpload &amp; Complete &#187;\r\n";
			
			poststr="--"+boundary+"\r\n"+item1;
			poststr+="--"+boundary+"\r\n"+item2;
			poststr+="--"+boundary+"\r\n"+item3;
			poststr+="--"+boundary+"\r\n"+item4;
				
			CString poststrlast="\r\n";
			poststrlast+="--"+boundary+"\r\n"+item5+"--"+boundary+"--\r\n";
			DWORD dwPostSize=0;
			char *buf=NULL;

			CFile file;
			if(file.Open(filepath,CFile::modeReadWrite)){
				dwPostSize=file.GetLength()+poststr.GetLength()+poststrlast.GetLength();
				buf=new char[dwPostSize];
				memcpy(buf,poststr,poststr.GetLength());
				char *seek=buf+poststr.GetLength();
				file.Read(seek,file.GetLength());
				seek+=file.GetLength();
				memcpy(seek,poststrlast,poststrlast.GetLength());
				file.Close();
			}
			cookie=m_cookie;
			ret=mgr.GetData(1,_T("pracad01.cencol.ca"),posturl,_T("http://pracad01.cencol.ca:9191/app"),"multipart/form-data; boundary="+boundary,cookie,buf,dwPostSize,m_error);
	
			cookie=m_cookie;
			poststr="service=direct%2F1%2FUserWebPrintUpload%2F%24Form%240&sp=S1&Form1=";
			ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app"),_T("http://pracad01.cencol.ca:9191/app"),"",cookie,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);
			
			/*get status*/

			if(m_hwnd)
				::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)_T("Checking Status..."),NULL);

			if(ret.Find("var webPrintUID")==-1)
			{
				m_error=_T("Can not find Status Check URL, Update please");
				return false;
			}
			ret=ret.Mid(ret.Find("var webPrintUID")+1);
			ret=ret.Mid(ret.Find("'")+1);
			ret=ret.Mid(0,ret.Find("'"));
			CString jsoncheck=ret;

			/*
			{"status":{"code":"rendering","complete":false,"text":"Rendering","messages":[{"info":"Queued in position 1."},{"info":"Starting rendering process."},{"info":"Preparing job for rendering."},{"info":"Rendering job ..."}],"formatted":"<span class=\"info\">Rendering job ...</span>"},"documentName":"2323322.pdf","printer":"pracad01\\PR-LIB-FLOOR2"}

			{"status":{"code":"rendering","complete":false,"text":"Rendering","messages":[{"info":"Queued in position 1."},{"info":"Starting rendering process."},{"info":"Preparing job for rendering."},{"info":"Rendering job ..."},{"info":"Rendering successful.  Preparing job for processing."}],"formatted":"<span class=\"info\">Rendering successful.  Preparing job for processing.</span>"},"documentName":"2323322.pdf","printer":"pracad01\\PR-LIB-FLOOR2"}

			{"status":{"code":"queued","complete":true,"text":"Finished: Queued for printing","formatted":"Finished: Queued for printing"},"documentName":"2323322.pdf","printer":"pracad01\\PR-LIB-FLOOR2","pages":1,"cost":"$0.06"}


			*/

			int count=0;
			while(count<90){
				
				ret=mgr.GetData(0,_T("pracad01.cencol.ca"),_T("rpc/web-print/job-status/"+jsoncheck+".json"),_T("http://pracad01.cencol.ca:9191/app"),"",cookie,NULL,0,m_error);
				if(ret.GetLength ()==0){

					return false;
				}
				

				if(ret.Find (_T("\"formatted\":\""))!=-1){
					CString param=ret.Mid (ret.Find (_T("\"formatted\":\""))+strlen(_T("\"formatted\":\"")));
					
					bool isfinish=false;
					//"pages":1,"cost":"$0.06"
					if(param.Find (_T("\"pages\":"))!=-1&&param.Find (_T("\"cost\":"))!=-1){
						m_sheets=m_cost=0;
						CString text=param.Mid (param.Find (_T("\"pages\":")));
						sscanf(text,_T("\"pages\":%d,\"cost\":\"$%f\""),&m_sheets,&m_cost);
					
						isfinish=true;
						float eachpagescost=((float)m_cost/(float)m_sheets);
						if(eachpagescost>0.07){
							CString msg;
							msg.Format (_T("%s%.2f%s"),_T("The standard cost of each page is $0.06, you were charged $"),eachpagescost,_T(" each, it usually casue by an error of college's webprint system. Please contact helpdesk of College, check your account or return your money"));
							AfxMessageBox(msg);
						}
						else if(m_cost>m_balance){
							ismoneyrunout=true;
							m_error.Format (_T("This document needs $%.2f(%d pages), Your balance is $%.2f"),m_cost,m_sheets,m_balance);;
							return false;
						}
					}
					
					
					if(param.Find (_T(">"))!=-1){
						param=param.Mid (param.Find (_T(">"))+1);
						param=param.Mid (0,param.Find (_T("<")));
					}

					if(param.Find (_T("\""))!=-1)
						param=param.Mid (0,param.Find (_T("\"")));

					

					if(m_hwnd&&param.GetLength ())
						::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Checking Status...",(DWORD)(LPCTSTR)param);
				
					if(isfinish)//finish
						break;

				}

				Sleep(1000);
				TRACE("%s\r\n",ret);

				count++;
			}

			if(buf)
				delete []buf;
			break;
		}
	}

	m_hwnd=false;
	return true;
}
void WebPrintMgr::Init(){
	InitPConfig();
	InitAccount();
}
void WebPrintMgr::GetAccountInfo(CString &usr,CString &pwd){
	usr=m_usr;
	pwd=m_pwd;
}
bool WebPrintMgr::TopUp(CString num,int type){
	m_balance=0;
	switch(type){
		case WEBPRINT_CENTENNIAL:{

			
				if(m_hwnd)
					::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Starting Top-up...",NULL);

				HttpManager mgr;
				CString poststr="service=direct%2F1%2FHome%2F%24Form&sp=S0&Form0=%24Hidden%2CinputUsername%2CinputPassword%2C%24PropertySelection%2C%24Submit&%24Hidden=true&inputUsername=%%USR%%&inputPassword=%%PWD%%&%24PropertySelection=en&%24Submit=Log+in";//"Username=300679900&Password=etstudio";
				//CString ret=mgr.GetData(1,_T("e.centennialcollege.ca"),_T("d2l/lp/auth/login/login.d2l"),_T(""),_T(""),(PVOID)(LPCTSTR)poststr,poststr.GetLength(),error,cookie);
				//;jsessionid=za6tsp1p7p98
				poststr.Replace(_T("%%USR%%"),this->m_usr);
				poststr.Replace(_T("%%PWD%%"),m_pwd);

				m_cookie="";
				CString m_str=m_cookie;
				CString ret=mgr.GetData(0,_T("pracad01.cencol.ca"),_T("user"),_T("http://pracad01.cencol.ca:9191/"),"",m_cookie,NULL,0,m_error);
				m_str=m_cookie;
				ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app;")+m_cookie.Mid(0,m_cookie.GetLength()-1),_T("http://pracad01.cencol.ca:9191/user"),"",m_str,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);

				if(ret.GetLength ()==0){
					m_error=_T("Network error, Checking your connection try it again");
					return false;
				}
				if(ret.Find(_T("inputUsername"))!=-1||ret.Find(_T("Balance"))==-1){
					m_error=_T("Format error Update please");
					return false;
				}


				poststr="service=direct%2F1%2FUserTopUpCards%2F%24TopUpCardUse.%24Form&sp=S0&Form0=%24TextField%2C%24Submit&%24TextField=%%REMNO%%&%24Submit=Redeem+Card";
				//CString ret=mgr.GetData(1,_T("e.centennialcollege.ca"),_T("d2l/lp/auth/login/login.d2l"),_T(""),_T(""),(PVOID)(LPCTSTR)poststr,poststr.GetLength(),error,cookie);
				//;jsessionid=za6tsp1p7p98
				poststr.Replace(_T("%%REMNO%%"),num);

				m_str=m_cookie;
				ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app"),_T("http://pracad01.cencol.ca:9191/"),"",m_cookie,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);
				
				if(ret.GetLength ()==0){
					m_error=_T("Network error, Checking your connection try it again");
					return false;
				}//
				if(ret.Find(_T("The supplied card number is invalid"))!=-1){
					m_error=_T("Card number is invalid...");
					return false;
				}
				else if(ret.Find(_T("The value associated with this card has been successfully applied to your account."))==-1){
					m_error=_T("Top-up failed please try to do it on Website...");
					return false;
				}
				else if(m_hwnd)
					::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Top-up successful, print will be started soon...",NULL);
				else
					AfxMessageBox("Unknown error,if it failed you may need to top-up on website. Print will be started soon...",NULL);
			break;						 
		}
		default:
			break;
	}

	return true;
}
void WebPrintMgr::SetAccountInfo(CString &usr,CString &pwd,bool issave){
	m_usr=usr;
	m_pwd=pwd;
	if(issave){
		CFile file;
		if(file.Open(::GetModulePath()+"ac",CFile::modeCreate|CFile::modeReadWrite)){
			char buf[256];
			memset(buf,0,sizeof(buf));
			sprintf(buf,_T("%s*&%s\r\n"),usr,pwd);
			file.Write (buf,strlen(buf));
			file.Close();
		}
	}
}
void WebPrintMgr::InitPConfig(){
	CFile file;
	//Progress Campus$PR-LIB-FLOOR2,0,0,0,remark none$PR-LIB-FLOOR3,0,0,0,remark none
	if(file.Open(GetModulePath()+_T("pconfig"),CFile::modeReadWrite)){
		try{
			while(1){
				CString line=::ReadLineFromFile(file.m_hFile );

				if(line.GetLength ()>2){
					CString campusname=line.Mid(0,line.Find(_T("$")));
					CString block=campusname.Mid(campusname.Find(_T(","))+1);
					campusname=campusname.Mid (0,campusname.Find (_T(",")));
					line=line.Mid(line.Find(_T("$"))+1);
					line=line.Mid (0,line.GetLength ()-2);

					
					

					if(campusname.GetLength ()){
						PCAMPUSINFO pcampus=new CAMPUSINFO;
						pcampus->campusinfo=campusname;
						pcampus->p=new CPtrList;
						pCampus.AddTail(pcampus);

						
						while(block.GetLength ()){
							PLOCATION ploc=new LOCATION;
							ploc->p=new CPtrList;
							CString blockitem;
							if(block.Find (_T(","))==-1){
								blockitem=block;
								block=_T("");
							}
							else{
								blockitem=block.Mid(0,block.Find(_T(",")));
								block=block.Mid(block.Find(_T(","))+1);
							}

							ploc->building=blockitem;
							pcampus->p->AddTail (ploc);

						}

						while(line.GetLength ()){
							CString printitem;
							if(line.Find (_T("$"))==-1){
								printitem=line;
								line=_T("");
							}
							else{
								printitem=line.Mid(0,line.Find(_T("$")));
								line=line.Mid(line.Find(_T("$"))+1);
							}

							PPRINTERITEM p=new PRINTERITEM;
							p->printname=printitem.Mid(0,printitem.Find(_T(",")));
							printitem=printitem.Mid (printitem.Find(",")+1);
							p->level=atoi(printitem.Mid(0,printitem.Find(_T(","))));
							printitem=printitem.Mid (printitem.Find(",")+1);
							p->pointx=atoi(printitem.Mid(0,printitem.Find(_T(","))));
							printitem=printitem.Mid (printitem.Find(",")+1);
							p->pointy=atoi(printitem.Mid(0,printitem.Find(_T(","))));
							printitem=printitem.Mid (printitem.Find(",")+1);
							p->remark=printitem.Mid (0,printitem.Find(","));
							CString blackname=printitem.Mid (printitem.Find(",")+1);
							
							POSITION pos=pcampus->p->GetHeadPosition ();
							while(pos){
								PLOCATION ploc=(PLOCATION)pcampus->p->GetNext (pos);
								if(ploc->building .CompareNoCase (blackname)==0){
									ploc->p ->AddTail (p);
									break;
								}
							}
						}
					}
				}
			}
		}
		catch(...){
		}
		file.Close();
	}
}
void WebPrintMgr::InitAccount(){
	CFile file;

	if(file.Open(GetModulePath()+_T("ac"),CFile::modeReadWrite)){
		try{
			while(1){
				CString line=::ReadLineFromFile(file.m_hFile );
				if(line.GetLength ()>2){
					line=line.Mid (0,line.GetLength ()-2);
					m_usr=line.Mid (0,line.Find (_T("*&")));
					m_pwd=line.Mid (line.Find (_T("*&"))+2);
				}
			}
		}
		catch(...){
		}
		file.Close();
	}
}
bool WebPrintMgr::Login(int type){
	m_balance=0;
	switch(type){
		case WEBPRINT_CENTENNIAL:{

			if(m_hwnd)
				::SendMessage (m_hwnd,WM_PROGRESS,(DWORD)"Logging to Webprinter...",NULL);

			HttpManager mgr;
			CString poststr="service=direct%2F1%2FHome%2F%24Form&sp=S0&Form0=%24Hidden%2CinputUsername%2CinputPassword%2C%24PropertySelection%2C%24Submit&%24Hidden=true&inputUsername=%%USR%%&inputPassword=%%PWD%%&%24PropertySelection=en&%24Submit=Log+in";//"Username=300679900&Password=etstudio";
			//CString ret=mgr.GetData(1,_T("e.centennialcollege.ca"),_T("d2l/lp/auth/login/login.d2l"),_T(""),_T(""),(PVOID)(LPCTSTR)poststr,poststr.GetLength(),error,cookie);
			//;jsessionid=za6tsp1p7p98
			poststr.Replace(_T("%%USR%%"),this->m_usr);
			poststr.Replace(_T("%%PWD%%"),m_pwd);

			m_cookie="";
			CString m_str=m_cookie;
			CString ret=mgr.GetData(0,_T("pracad01.cencol.ca"),_T("user"),_T("http://pracad01.cencol.ca:9191/"),"",m_cookie,NULL,0,m_error);
			m_str=m_cookie;
			ret=mgr.GetData(1,_T("pracad01.cencol.ca"),_T("app;")+m_cookie.Mid(0,m_cookie.GetLength()-1),_T("http://http://pracad01.cencol.ca/:9191/user"),"",m_str,(PVOID)(LPCTSTR)poststr,poststr.GetLength(),m_error);

			if(ret.GetLength ()==0){
				m_error=_T("Network error, Checking your connection try it again");
				return false;
			}
			if(ret.Find(_T("inputUsername"))!=-1||ret.Find(_T("Balance"))==-1){
				m_error=_T("Format error Update please");
				return false;
			}

			ret=ret.Mid(ret.Find(_T("Balance"))+1);
			ret=ret.Mid(ret.Find(_T("$"))+1);
			ret=ret.Mid(0,ret.Find(_T(" ")));
			sscanf(ret,_T("%f"),&m_balance);

		//	int value=m_balance*100;
		//	m_balance=(float)value/(float)100;

			break;						 
		}
		default:
			break;
	}

	return true;
}