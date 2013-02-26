// WebRender.h : main header file for the WEBRENDER application
//

#if !defined(AFX_WEBRENDER_H__C0275914_12F4_4891_AEB1_185221311BE7__INCLUDED_)
#define AFX_WEBRENDER_H__C0275914_12F4_4891_AEB1_185221311BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWebRenderApp:
// See WebRender.cpp for the implementation of this class
//

class CWebRenderApp : public CWinApp
{
public:
	CWebRenderApp();

	WebPrintMgr web;
	CString filepath;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebRenderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWebRenderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
extern CWebRenderApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBRENDER_H__C0275914_12F4_4891_AEB1_185221311BE7__INCLUDED_)
