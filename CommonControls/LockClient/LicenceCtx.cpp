#include "stdafx.h"
#include "LicenceCtx.h"
#include <CommonTools/PathConfig.h>

CString CLockTool::GetLockCfgFile()
{
	TCHAR AppPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, AppPath, MAX_PATH);

	TCHAR *pSlash = _tcsrchr(AppPath, _T('\\'));
	pSlash++;
	*pSlash = _T('\0');
	CString strModulePath = CPathConfig::GetModulePath();
	CString strCfg = strModulePath + L"..\\cfgs\\AppStarter.ini";
	return strCfg;
}