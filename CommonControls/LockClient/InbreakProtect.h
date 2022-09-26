#pragma once

#include <Tlhelp32.h>

/************************************************************************/
/* 用于检查程序关键API或dll是否被拦截                                   */
/************************************************************************/

template<class T, DWORD ErrVal>
class CInbreakProtectT
{
public:
	/*
	*	检查关键API是否被拦截，如果没有拦截则返回传入的值，否则返回0
	*/
	T ApiDetection (T value)
	{
		ATLASSERT (value != (T) ErrVal);

		HMODULE hModule = NULL;
		T result = ErrVal;

		// 检查 Kernel32.dll
		CAtlArray<CString> api_list;
		api_list.Add (_T("DeviceIoControl"));
		api_list.Add (_T("CreateFileA"));
		api_list.Add (_T("GetSystemTime"));
		api_list.Add (_T("GetSystemTimeAsFileTime"));
		api_list.Add (_T("CreateToolhelp32Snapshot"));
		api_list.Add (_T("GetModuleFileNameW"));
		api_list.Add (_T("GetProcAddress"));
		api_list.Add (_T("LoadLibraryA"));
		api_list.Add (_T("LoadLibraryExW"));
		api_list.Add (_T("LoadLibraryW"));
		api_list.Add (_T("Module32FirstW"));
		api_list.Add (_T("Module32NextW"));

		hModule = LoadLibrary(_T("Kernel32.dll"));
		if (hModule == NULL)
			return result;

		if (_check_jmp_api_detection(hModule, api_list))
		{
			FreeLibrary(hModule);
			return result;
		}

		if (_check_twolib_api_detection(hModule, api_list))
		{
			FreeLibrary(hModule);
			return result;
		}

		FreeLibrary(hModule);

		// 检查 setupapi.dll
		api_list.RemoveAll ();
		api_list.Add (_T("SetupDiEnumDeviceInterfaces"));
		api_list.Add (_T("SetupDiEnumDeviceInfo"));
		api_list.Add (_T("SetupDiGetDeviceInterfaceDetailA"));
		api_list.Add (_T("SetupDiGetDeviceRegistryPropertyA"));

		hModule = LoadLibrary(_T("setupapi.dll"));
		if (hModule == NULL)
			return result;

		if (_check_jmp_api_detection(hModule, api_list))
		{
			FreeLibrary(hModule);
			return result;
		}

		if (_check_twolib_api_detection(hModule, api_list))
		{
			FreeLibrary(hModule);
			return result;
		}

		FreeLibrary(hModule);

		// 检查 USER32.DLL
		api_list.RemoveAll ();
		api_list.Add (_T("EnableWindow"));
		api_list.Add (_T("EnumChildWindows"));
		api_list.Add (_T("EnumWindows"));
		api_list.Add (_T("GetClassNameW"));

		hModule = LoadLibrary(_T("USER32.DLL"));
		if (hModule == NULL)
			return result;

		if (_check_jmp_api_detection(hModule, api_list))
		{
			FreeLibrary(hModule);
			return result;
		}

		if (_check_twolib_api_detection(hModule, api_list))
		{
			FreeLibrary(hModule);
			return result;
		}

		FreeLibrary(hModule);

		result = value;

		return result;
	}

	/*
	*	检查关键DLL是否被拦截,没有拦截返回传入的数据，失败返回0
	*/
	T DllDetection (T value)
	{
		ATLASSERT (value != (T) ErrVal);

		//1.1 查找当前目录下的文件
		TCHAR		szPath[MAX_PATH] = {0};
		TCHAR		szSearch[MAX_PATH] = {0};
		CString		strPath;

		DWORD	dwRet = 0;
		int		nPos = 0;

		T	result = ErrVal;

		// 获取程序所在的路径
		dwRet = GetModuleFileName(NULL, szPath, MAX_PATH);
		if (!dwRet)
		{
			return result;
		}

		strPath = szPath; strPath.MakeLower();
		nPos = strPath.ReverseFind('\\');
		_tcsncpy_s(szPath, MAX_PATH, strPath, nPos);
		_tcsncpy_s(szSearch, MAX_PATH, szPath, _tcslen (szPath));
		_tcscat_s(szSearch, MAX_PATH, _T("\\*.*"));

		// 搜索当前目录下的所有文件
		WIN32_FIND_DATA fd;
		HANDLE  hFile = NULL;

		hFile = FindFirstFile(szSearch, &fd);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return result;
		}

		while(FindNextFile(hFile, &fd))
		{
			CString strTemp = fd.cFileName;
			strTemp.MakeLower ();
			if (strTemp == _T("usp10.dll") || strTemp == _T("lpk.dll") || strTemp == _T("hid.dll"))
			{
				FindClose(hFile);
				return result;
			}
		}
		FindClose(hFile);

		//1.3程序加载模块的检查
		//如果程序路径下的加载模块和系统路径下的加载模块名称相同，则认为出现了DLL劫持
		TCHAR szSysPath[MAX_PATH] = {0};
		BOOL bOk = FALSE;
		BOOL bSameModule = FALSE;
		DWORD sysFileCnt = 0;
		DWORD curDirFileCnt = 0;
		CString SysPath;

		CString *pCurDirFile[MAX_PATH];
		CString *pSysDirFile[MAX_PATH];	 

		GetSystemDirectory(szSysPath, MAX_PATH);
		SysPath = szSysPath; SysPath.MakeLower();
		DWORD dwCurid = GetCurrentProcessId();

		//枚举程序中的所有模块
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwCurid);
		if (hSnapShot == INVALID_HANDLE_VALUE)
			return result;

		MODULEENTRY32 moduleEntry = { 0 };
		moduleEntry.dwSize = sizeof (MODULEENTRY32);

		if (!Module32First(hSnapShot, &moduleEntry))
		{
			CloseHandle (hSnapShot);
			return result;
		}

		do
		{
			CString ExePath = moduleEntry.szExePath;
			ExePath.MakeLower();

			if (!_tcsncmp(ExePath, szPath, _tcslen(szPath)))
			{
				//当前目录下的文件
				pCurDirFile[curDirFileCnt] = new CString();
				*pCurDirFile[curDirFileCnt] = CString(moduleEntry.szModule).MakeLower();
				curDirFileCnt += 1;
			}
			else if (!_tcsncmp(ExePath, SysPath, SysPath.GetLength()))
			{
				//系统目录下的文件
				pSysDirFile[sysFileCnt] = new CString();
				*pSysDirFile[sysFileCnt] = CString(moduleEntry.szModule).MakeLower();
				sysFileCnt += 1;
			}
		}while (Module32Next (hSnapShot, &moduleEntry));

		// 看是否有相同的文件
		for (size_t i = 0; i< curDirFileCnt; i++)
		{
			for (size_t j = 0; j< sysFileCnt; j++)
			{
				if (*pCurDirFile[i] == *pSysDirFile[j])
				{
					bSameModule = TRUE;
					break;
				}
			}
			if (bSameModule)
			{
				break;
			}
		}

		//回收内存
		for (size_t i = 0; i< curDirFileCnt; i++)
		{
			delete pCurDirFile[i];
		}
		for (size_t i = 0; i< sysFileCnt; i++)
		{
			delete pSysDirFile[i];		 
		}

		// 如果没有发现相同模块，则返回正确结果
		if (!bSameModule)
			result = value;

		return result;
	}

	/** 测试程序是否被调试，没有调试返回输入值，有调试返回0 */
	T DebugDetection (T value)
	{
		ATLASSERT (value != (T) ErrVal);

		T result = ErrVal;

#ifdef _DEBUG

		result = value;

#else
		// 通过中断函数来判断是否有没有调试器
		typedef void (FAR WINAPI *DEBUG_BREAK_FUNC)();
		HMODULE hModule = LoadLibrary(_T("Kernel32.dll"));
		__try 
		{
			DEBUG_BREAK_FUNC break_func = (DEBUG_BREAK_FUNC) GetProcAddress(hModule, "DebugBreak");
			break_func();
		}
		__except(GetExceptionCode() == EXCEPTION_BREAKPOINT ?
					EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) 
		{
			// 如果没有关联调试器，则再查找是否有调试软件
			if (!_find_ollydbg_ida ())
			{
				result = value;
			}
		}

		FreeLibrary(hModule);
#endif

		return result;
	}

private:
	// 查找所有顶层窗口
	static BOOL CALLBACK EnumTopWindowProc(HWND hwndChild, LPARAM lParam)
	{
		HWND* findDebugWindow = (HWND*) lParam;
		EnumChildWindows(hwndChild, EnumChildProc, lParam);
		return *findDebugWindow == NULL;
	}

	// 查找子窗口
	static BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam)
	{
		HWND* findDebugWindow = (HWND*) lParam;
		TCHAR* find_names[] =
		{
			_T("lCPUASM"),	_T("lCPUREG"),	_T("lCPUINFO"),	_T("lCPUSTACK"),
			_T("IDAView"),	_T("hexview_t"),	_T("structview_t")
		};

		// 取得窗口类名称并判断类型是否是调试器
		TCHAR className[MAX_PATH] = { 0 };
		if (GetClassName (hwndChild, className, MAX_PATH))
		{
			for (int i = 0; i < 7; i ++)
			{
				// 找到名称相同的类则退出
				if (!_tcscmp (className, find_names[i]))
				{
					*findDebugWindow = hwndChild;
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	// 冻结所有窗口
	static BOOL CALLBACK EnableTopWindowProc(HWND hwndChild, LPARAM lParam)
	{
		EnumChildWindows(hwndChild, EnableChildProc, lParam);
		EnableWindow(hwndChild, !!lParam);
		return TRUE;
	}
	// 冻结所有窗口
	static BOOL CALLBACK EnableChildProc(HWND hwndChild, LPARAM lParam)
	{
		EnableWindow(hwndChild, !!lParam);
		return TRUE;
	}

	/**查找 OllyDbg 程序，成功返回1，失败返回0 */
	BOOL _find_ollydbg_ida (void)
	{
		// 以下是OllyDbg、IDA程序的窗口级别
		//OllyDbg
		//	MDIClient
		//	  lCPU
		//	    lCPUASM
		//	    lCPUREG
		//	    lCPUINFO
		//	    lCPUSTACK
		//IDA
		//  TaqDockingSite
		//    TaqDockingControl
		//	    TMDIForm \ IDAView
		//	    THexForm \ hexview_t
		//	    TStrucForm \ structview_t

		// 原理：查找顶层窗口及窗口，找到名为lCPUASM、lCPUREG、lCPUINFO、lCPUSTACK、
		//			IDAView、hexview_t、structview_t表示有调试器

		HWND findDebugWindow = NULL;
		EnumWindows(EnumTopWindowProc, (LPARAM) &findDebugWindow);

		// 如果发现有调试程序，则立即冻结所有窗口
		if (findDebugWindow)
		{
			EnumWindows (EnableTopWindowProc, FALSE);
		}

		return findDebugWindow != NULL;
	}

	/** 采用比较跳转命令的方法来判断是否有 API 劫持 */
	BOOL _check_jmp_api_detection (HMODULE hModule, CAtlArray<CString>& api_list)
	{
		FARPROC lpProc = NULL;
		BYTE    jmpOpCode1 = 0xE9;
		BYTE    jmpOpCode2 = 0xEB;
		BYTE	firstProcCode = 0;

		for (size_t i = 0; i < api_list.GetCount (); i ++)
		{
			// 如果函数开前为jmp则表示函数被挂钩
			CW2A apiname = api_list[i];
			lpProc = GetProcAddress(hModule, apiname);
			firstProcCode = *((BYTE*)lpProc);
			if (lpProc != NULL && (firstProcCode == jmpOpCode1 || firstProcCode == jmpOpCode2))
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	/** 采用加载第二份dll文件的方法来判断是否有api劫持 */
	BOOL _check_twolib_api_detection (HMODULE hModule, CAtlArray<CString>& api_list)
	{
		// 获取模块路径，如果获取不到则认为被劫持
		TCHAR src_path[MAX_PATH] = { 0 };
		::GetModuleFileName (hModule, src_path, MAX_PATH);

		// 生成临时文件
		TCHAR lpPathBuffer[MAX_PATH] = { 0 };
		if (GetTempPath (MAX_PATH, lpPathBuffer) == 0)
			return TRUE;

		TCHAR temp_sign[MAX_PATH] = { 0 };
		_stprintf_s(temp_sign, MAX_PATH, _T("%d_"), ::GetTickCount());
		_tcscat_s (lpPathBuffer, MAX_PATH, temp_sign);
		TCHAR *nameptr = _tcsrchr (src_path, _T('\\'));
		_tcscat_s (lpPathBuffer, MAX_PATH, ++ nameptr);
		if (!CopyFile (src_path, lpPathBuffer, TRUE))
			return TRUE;

		// 加载临时文件，如果失败则表示劫持
		HMODULE tempModule = LoadLibraryEx (lpPathBuffer, NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (tempModule == NULL)
		{
			DeleteFile (lpPathBuffer);
			return TRUE;
		}

		// 比较两个模块中函数的头5个字节，看是否有不相同的，有不相同则表示有拦劫
		FARPROC lpProc1 = NULL;
		FARPROC lpProc2 = NULL;
		long errcnt = 0;
		for (size_t i = 0; i < api_list.GetCount (); i ++)
		{
			// 如果函数开前为jmp则表示函数被挂钩
			CW2A apiname = api_list[i];
			lpProc1 = GetProcAddress (hModule, apiname);
			lpProc2 = GetProcAddress (tempModule, apiname);

			BYTE* codebuff1 = (BYTE *) lpProc1;
			BYTE* codebuff2 = (BYTE *) lpProc2;

			if (memcmp (codebuff1, codebuff2, 5))
				errcnt ++;
		}

		// 如果 errcnt > 0 表示有拦截
		FreeLibrary (tempModule);
		DeleteFile (lpPathBuffer);

		return errcnt > 0;
	}
};

typedef CInbreakProtectT<DWORD,0> CInbreakProtect;