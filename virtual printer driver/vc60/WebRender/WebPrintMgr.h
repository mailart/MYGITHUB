#pragma once

#include "HttpManager.h"
#include "Globals.h"

#define WM_PROGRESS WM_USER+1000 
#define WM_PROGRESSERROR WM_USER+1001 

typedef struct{
	CString printname;
	int level;
	int pointx,pointy;
	CString remark;
}PRINTERITEM,*PPRINTERITEM;

typedef struct{
	CString building;
	CPtrList *p;
}LOCATION,*PLOCATION;

typedef struct{
	CString campusinfo;
	CPtrList *p;
}CAMPUSINFO,*PCAMPUSINFO;

#define WEBPRINT_CENTENNIAL	0
class WebPrintMgr
{
public:
	WebPrintMgr(void);
	~WebPrintMgr(void);

	void Init();
	void SetCallBack(HWND h){
		m_hwnd=h;
	}
	bool Login(int type);
	bool Print(LPCTSTR printname,LPCTSTR filepath,int copies,int type);
	void GetAccountInfo(CString &usr,CString &pwd);
	void SetAccountInfo(CString &usr,CString &pwd,bool issave);
	CString GetErrorStr(){
		return m_error;
	}
	CPtrList pCampus;
	float GetBalance(){
		return m_balance;
	}
	void GetRecipt(int &sheets,float &cost){
		sheets=m_sheets;
		cost=m_cost;
	}
	bool isOutofBalance(){
		return ismoneyrunout;
	}
	bool TopUp(CString num,int type);
private:
	void InitPConfig();
	void InitAccount();

	CString m_usr,m_pwd;
	HWND m_hwnd;
	bool ismoneyrunout;
	bool isremember;
	CString m_cookie,m_error;
	float m_balance,m_cost;
	int m_sheets;
};

