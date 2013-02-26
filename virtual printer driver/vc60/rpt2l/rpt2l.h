// rpt2l.h : main header file for the RPT2L application
//

#if !defined(AFX_RPT2L_H__0AAB86A7_BC06_45FB_B9F4_A8BFC6E0542B__INCLUDED_)
#define AFX_RPT2L_H__0AAB86A7_BC06_45FB_B9F4_A8BFC6E0542B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRpt2lApp:
// See rpt2l.cpp for the implementation of this class
//

class CRpt2lApp : public CWinApp
{
public:
	CRpt2lApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRpt2lApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRpt2lApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RPT2L_H__0AAB86A7_BC06_45FB_B9F4_A8BFC6E0542B__INCLUDED_)
