#include "StdAfx.h"
#include "PathConfig.h"

namespace CommBase
{
	CString CPathConfig::GetPluginPath()
	{
		return GetAppStartPath() + L"Plugin\\";
	}

	CString CPathConfig::GetAppStartPath()
	{
		static CString strAppPath;

		if (strAppPath.IsEmpty())
		{
			TCHAR AppPath[MAX_PATH] = { 0 };
			::GetModuleFileName(gInstance, AppPath, MAX_PATH);

			// 去掉程序名
			TCHAR *pSlash = _tcsrchr(AppPath, _T('\\'));
			pSlash++;
			*pSlash = _T('\0');
			strAppPath = AppPath;
		}

		return strAppPath;
	}

	CString CPathConfig::GetWorkspacePath()
	{
		return GetAppStartPath() + L"Workspace\\";
	}

	CString CPathConfig::GetSystemPath()
	{
		return GetAppStartPath() + L"System\\";
	}

	CString CPathConfig::GetProjectsPath()
	{
		return GetAppStartPath() + L"用户工程\\";
	}

	CString CPathConfig::GetResourceLibPath()
	{
		return GetAppStartPath() + L"ResLibrary\\";
	}

	CString CPathConfig::GetResourcePath()
	{
		return GetAppStartPath() + L"Resource\\";
	}

	CString CPathConfig::GetMaterialPath()
	{
		return GetAppStartPath() + L"Material\\";
	}

	CString CPathConfig::GetDumpPath()
	{
		return GetAppStartPath() + L"Dump\\";
	}

}