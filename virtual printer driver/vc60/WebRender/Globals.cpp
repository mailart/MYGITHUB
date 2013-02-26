#include "stdafx.h"
#include "Global.h"


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
CString ReadLineFromFile(HFILE file)
{
	CFile read;
	read.m_hFile =file;
	bool isline=false;
	CString line;

	while(!isline)
	{
		Sleep(0);
		char tempbuf[2001];
		memset(tempbuf,0,2001);

		int readbytes=read.Read (tempbuf,2000);
		if(readbytes==2000)
		{
			char *point=tempbuf;
			for(int check=0;check<2000;check++)
			{
				Sleep(0);
				if(*point==10)//找到一行
				{
					isline=true;
					memset(&tempbuf[check+1],0,2000-(check+1));
					read.Seek (read.GetPosition ()-(2000-(check+1)),CFile::begin);

					line+=tempbuf;
		
					return line;
				}
				point++;
			}
			if(!isline)line+=tempbuf;
		}
		else//文件结束
		{
			char *point=tempbuf;
			for(int check=0;check<readbytes;check++)
			{
				Sleep(0);
				if(*point==10)//找到一行
				{
					isline=true;
					memset(&tempbuf[check+1],0,readbytes-(check+1));
					read.Seek (read.GetPosition ()-(readbytes-(check+1)),CFile::begin);

					line+=tempbuf;
					return line;
				}
				point++;
			}
			throw int(9);
		}
	
	}

	read.m_hFile =-1;

	return line;
}
