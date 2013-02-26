; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWebRenderDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WebRender.h"

ClassCount=6
Class1=CWebRenderApp
Class2=CWebRenderDlg
Class3=CAboutDlg

ResourceCount=8
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_WEBRENDER_DIALOG
Resource4=IDD_STATUS (English (U.S.))
Resource5=IDD_ABOUTBOX (English (U.S.))
Class4=StatusDlg
Resource6=IDD_STATUSDLG (English (U.S.))
Class5=StatusBar
Resource7=IDD_WEBRENDER_DIALOG (English (U.S.))
Class6=RedeemDlg
Resource8=IDD_WEBRENDER_DIALOG1 (English (U.S.))

[CLS:CWebRenderApp]
Type=0
HeaderFile=WebRender.h
ImplementationFile=WebRender.cpp
Filter=N

[CLS:CWebRenderDlg]
Type=0
HeaderFile=WebRenderDlg.h
ImplementationFile=WebRenderDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_STATICS

[CLS:CAboutDlg]
Type=0
HeaderFile=WebRenderDlg.h
ImplementationFile=WebRenderDlg.cpp
Filter=D
LastObject=IDOK
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_WEBRENDER_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CWebRenderDlg

[DLG:IDD_WEBRENDER_DIALOG (English (U.S.))]
Type=1
Class=CWebRenderDlg
ControlCount=22
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_USR,edit,1342242944
Control8=IDC_PWD,edit,1342242976
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_CAMPUS,combobox,1344340227
Control13=IDC_LOCATION,combobox,1344339971
Control14=IDC_SPIN1,msctls_updown32,1342177312
Control15=IDC_COPY,edit,1342242944
Control16=IDC_REMEMBERMARK,button,1342242819
Control17=IDC_STATIC,static,1342308352
Control18=IDC_PRINTER,combobox,1344340226
Control19=IDC_PIC,static,1342177294
Control20=IDC_TIPS,static,1342308352
Control21=IDABOUT,button,1342177281
Control22=IDC_STATICS,static,1342177283

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:StatusDlg]
Type=0
HeaderFile=StatusDlg.h
ImplementationFile=StatusDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_STEP
VirtualFilter=dWC

[DLG:IDD_STATUSDLG (English (U.S.))]
Type=1
Class=StatusDlg
ControlCount=1
Control1=IDC_STEP,static,1342308352

[DLG:IDD_STATUS (English (U.S.))]
Type=1
Class=StatusBar
ControlCount=6
Control1=IDC_STEP,static,1342308352
Control2=IDC_STEP2,static,1342308352
Control3=IDC_BUTTON1,button,1073807360
Control4=IDC_STEP3,static,1342308352
Control5=IDC_STATIC,static,1342177283
Control6=IDC_BUTTON2,button,1073807360

[CLS:StatusBar]
Type=0
HeaderFile=StatusBar.h
ImplementationFile=StatusBar.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=IDC_BUTTON1

[DLG:IDD_WEBRENDER_DIALOG1 (English (U.S.))]
Type=1
Class=RedeemDlg
ControlCount=6
Control1=ID_OKS,button,1342177281
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_USR,edit,1342242944
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:RedeemDlg]
Type=0
HeaderFile=RedeemDlg.h
ImplementationFile=RedeemDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

