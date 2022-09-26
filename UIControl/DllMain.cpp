// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

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
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
		{
			CommBase::CLog::LogInfo(L"MFC模块初始化失败");
		}
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