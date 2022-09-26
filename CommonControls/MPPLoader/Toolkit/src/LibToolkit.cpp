#include "stdafx.h"
#include "..\include\libtoolkit.h"
#include "..\include\EnumProc.h"
#include "shellapi.h"
#include "psapi.h"

using namespace BwProj::toolkit;

/** 生成 GUID　*/
CString CLibToolkit::CreateGuid (void)
{
	GUID id;
	WCHAR idstr[MAXBYTE];
	CoCreateGuid (&id);
	StringFromGUID2 (id, idstr, MAXBYTE);

	return CString (idstr);
}

/** 取得模块路径（c:\a.exe）*/
CString CLibToolkit::GetAppModuleFilename (HINSTANCE hInst)
{
	TCHAR m_pstr[MAX_PATH];

	// 2012-01-30 LY 处理在DLL中调用该函数传入NULL参数时获取的不是应用程序路径问题
	// Get entire module filepath
	//#if (_ATL_VER >= 0x0700)
	//	if( hInst == NULL ) hInst = ATL::_AtlBaseModule.GetResource2Instance();
	//#else
	//	if( hInst == NULL ) hInst = _Module.GetModuleInstance();
	//#endif // _ATL_VER
	//

	if (hInst == NULL) {
		// 用于获取本进程路径
		::GetModuleFileName(hInst, m_pstr, MAX_PATH);
	} else {
		// 用于获取外进程路径
		::GetModuleFileNameEx(hInst, 0, m_pstr, MAX_PATH);
	}
		

	return m_pstr;
}

/** 取得模块路径（c:\）　*/
CString CLibToolkit::GetAppModulePath (HINSTANCE hInst)
{
	CString path = GetAppModuleFilename(hInst);

	if (path.GetLength() > 0) {
		path = path.Mid(0, path.ReverseFind('\\')+1);
	}

	return path;
}

/** 取得临时路径（c:\）　*/
CString CLibToolkit::GetTempPath (void)
{
	TCHAR m_pstr[MAX_PATH];

	DWORD dwLength = ::GetTempPath(MAX_PATH, m_pstr);
	m_pstr[dwLength] = _T('\0');
	return m_pstr;
}

/** 取得API出错信息　*/
CString CLibToolkit::GetWin32LastError ()
{
	CString str;
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf, 0, NULL 
	);
	str = (LPCTSTR)lpMsgBuf;
	LocalFree( lpMsgBuf );

	return str;
}

/** 取得系统图标 */
HICON CLibToolkit::GetSystemIcon(SYSTEM_ICON_INDEX index)
{
	TCHAR path[MAX_PATH] = {0};
	GetSystemDirectory(path, MAX_PATH);
	CString systemPath = CString(path);
	CString shell32Path = systemPath + CString("\\shell32.dll");
	HICON icon = ExtractIcon((HINSTANCE)::GetCurrentProcess(), shell32Path.GetBuffer(),index);
	return icon;
}

// 获取进程路径列表
// 获取进程路径列表
int CLibToolkit::LoadProcessList(CAtlArray<CString>& list)
{
	CProcessIterator piFind;

	list.RemoveAll();
	DWORD pid = piFind.First();
	while (pid) {
		CProcessModuleIterator miFind(pid);
		if (miFind.GetProcessHandle() == NULL) {
			pid = piFind.Next();
			continue;
		}

		CString path = GetAppModuleFilename((HINSTANCE)miFind.GetProcessHandle());
		if (path.GetLength() == 0) {
			pid = piFind.Next();
			continue;
		}

		list.Add(path);
		pid = piFind.Next();
	}

	return list.GetCount();
}