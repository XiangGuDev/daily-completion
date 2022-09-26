// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "PathConfig/PathConfig.h"
#include "Tool\LogOutput.h"

#ifdef _X86_
extern "C" { int _afxForceUSRDLL; }
#else
extern "C" { int __afxForceUSRDLL; }
#endif

HINSTANCE gInstance = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		gInstance = hModule;
		// 初始化日志文件
		CString strSysConfig = CommBase::CPathConfig::GetSystemPath() + L"System.ini";
		UINT nLogType = GetPrivateProfileInt(L"SYSTEM", L"LogType", CommBase::eLogError | CommBase::eLogWarning, strSysConfig);
		CString strAppStartPath = CommBase::CPathConfig::GetAppStartPath();
		strAppStartPath += "AppLog\\";
		CommBase::CLog::InitLog(strAppStartPath, nLogType);

		CommBase::CLog::LogInfo(L"软件启动");

		CommBase::InitKObjectFactory();
	}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		CommBase::UninitKObjectFactory();

		CommBase::CLog::LogInfo(L"软件退出");

		// 释放日志文件
		CommBase::CLog::UninitLog();
		break;
	}

	return TRUE;
}