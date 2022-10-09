#include "pch.h"
#include "AutoStartUtility.h"

void AutoStartUtility::SetAutoStart(bool bAuto)
{
	try
	{
		HKEY hKey;

		//找到系统的启动项
		CString lpRun = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

		//打开启动项
		long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpRun, 0, KEY_ALL_ACCESS, &hKey);
		if (lRet != ERROR_SUCCESS)
			MessageBox(::GetActiveWindow(), _T("打开启动项失败"), _T(""), MB_OK);

		//找到程序自身路径
		TCHAR pFileName[MAX_PATH] = {};
		GetModuleFileName(NULL, pFileName, MAX_PATH);

		//判断是否已经设置开机启动
		TCHAR PowerBoot[MAX_PATH] = {};
		DWORD nLongth = MAX_PATH;
		long result = RegGetValue(hKey, NULL, _T("PowerBoot"), RRF_RT_REG_SZ, 0, PowerBoot, &nLongth);
		if (result == ERROR_SUCCESS)        //自启状态
		{
			if (!bAuto)
			{
				//取消自动启动
				lRet = RegDeleteValue(hKey, _T("PowerBoot"));
				if (lRet == ERROR_SUCCESS)
					MessageBox(::GetActiveWindow(), _T("关闭自启成功"), _T(""), MB_OK);
				else
					MessageBox(::GetActiveWindow(), _T("关闭自启失败"), _T(""), MB_OK);
			}
		}
		else        //未自启状态
		{
			if (bAuto)
			{
				//设置自启
				lRet = RegSetValueEx(hKey, _T("PowerBoot"), 0, REG_SZ, (LPBYTE)pFileName, (lstrlen(pFileName) + 1) * sizeof(TCHAR));
				if (lRet == ERROR_SUCCESS)
					MessageBox(::GetActiveWindow(), _T("设置自启成功"), _T(""), MB_OK);
				else
					MessageBox(::GetActiveWindow(), _T("设置自启失败"), _T(""), MB_OK);
			}
		}

		//关闭注册表
		RegCloseKey(hKey);
	}
	catch (...) {
	}
}
