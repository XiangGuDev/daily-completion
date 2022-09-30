
// daily-completion.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

// 程序单实例
#define SCANSETTINGS_CLASSNAME _T("ScanSettingsWindowClass")
#define APPMUTEX _T("Global\\ScanSettings")


#include "resource.h"		// 主符号


// CdailycompletionApp:
// 有关此类的实现，请参阅 daily-completion.cpp
//

class CdailycompletionApp : public CWinApp
{
public:
	CdailycompletionApp();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	BOOL RestrictOneInstance();
	DECLARE_MESSAGE_MAP()
private:
	HANDLE m_hMutex;
};

extern CdailycompletionApp theApp;
