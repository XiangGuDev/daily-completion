#include "StdAfx.h"
#include "PathConfig.h"

namespace CommBase
{
	CString CPathConfig::GetProjectName()
	{
		CString strPrjName;
		if (strPrjName.IsEmpty())
		{
			TCHAR AppPath[MAX_PATH] = { 0 };
			::GetModuleFileName(NULL, AppPath, MAX_PATH);
			TCHAR *beg = _tcsrchr(AppPath, _T('\\'));
			beg++;
			TCHAR *end = _tcsrchr(AppPath, _T('.'));
			*end = _T('\0');
			strPrjName = beg;
		}
		return strPrjName;
		
	}

	CString CPathConfig::GetPersistentPath()
	{
		static CString strPersistentPath;
		if (strPersistentPath.IsEmpty())
		{
			wchar_t buffer[MAX_PATH];
			BOOL result = SHGetSpecialFolderPath(0, buffer, CSIDL_LOCAL_APPDATA, false);
			strPersistentPath = buffer;
			strPersistentPath += L"\\" + GetProjectName() + L"\\";
		}
		return strPersistentPath;
	}

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