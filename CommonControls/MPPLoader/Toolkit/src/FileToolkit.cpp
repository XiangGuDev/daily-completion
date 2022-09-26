#include "stdafx.h"
#include "..\include\FileToolkit.h"
#include "..\include\StringToolkit.h"

#include <Psapi.h>
#include <shlwapi.h>
#include <Shlobj.h>

#pragma comment(lib, "shlwapi")
#pragma comment(lib, "shell32")

using namespace BwProj::toolkit;

CString CFileToolkit::GetLocalAppDataPath()
{
	TCHAR m_lpszDefaultDir[MAX_PATH];
	TCHAR szDocument[MAX_PATH] = { 0 };
	memset(m_lpszDefaultDir, 0, _MAX_PATH);

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}

	return m_lpszDefaultDir;
}

CString CFileToolkit::GetLocalAppDataPath(LPCTSTR appName)
{
	CString path = GetLocalAppDataPath();
	path.Append(_T("\\"));
	path.Append(appName);

	if (_waccess(path, 0) == -1)
	{
		_wmkdir(path);
	}

	return path;
}

int CFileToolkit::DecDir (CString Filename, CAtlArray<CString> *arr)
{
	int i, Cnt;
	arr->RemoveAll ();
	Cnt = CStringToolkit::CountWord (Filename, _T('\\'));
	for (i = 1; i <= Cnt; i ++)
		arr->Add (CStringToolkit::ReadWord (Filename, i, _T('\\')));
	return (int) arr->GetCount ();
}

CString CFileToolkit::GetDrive (CString Path)
{
	// 路径长度小于2时认为是非法路径
	if(Path.GetLength() < 2 && Path.GetAt(0) != '\\')
		return _T("");

	CString LeftTwo, name = Path;

	LeftTwo = name.Left (2);
	if (LeftTwo == _T("\\\\"))	// 网络
		return _T("\\\\") + CStringToolkit::ReadWord (name, 1, _T('\\'));
	else if (LeftTwo.GetAt (1) == _T(':'))	// 磁盘
	{
		name.MakeUpper ();
		return name.Left (2);
	}
	else if (LeftTwo.GetAt (0) == _T('\\'))	// 根目录
		return GetCurrentDirectory ().Left (2);
	else	// 无盘符
		return _T("");
}

CString CFileToolkit::FormatDirectory (CString Directory)
{
	CAtlArray<CString> list;
	CString RetDir, SubDir;

	if (!Directory.GetLength ()) return _T("");

	DecDir (Directory, &list);
	if (!list.GetCount ()) return _T("");

	SubDir = list.GetAt (0);

	// 处理盘符
	if (Directory.Left (2) == _T("\\\\"))	// 网络
		RetDir = _T("\\\\") + SubDir;
	else if (Directory.GetAt (0) == _T('\\'))	// 根目录
		RetDir = GetCurrentDirectory ().Left (3) + SubDir;
	else if (Directory.GetLength () > 1 &&
		Directory.GetAt (1) == _T(':'))	// 盘符
		RetDir = SubDir.Left (2);
	else	// 当前目录
	{
		RetDir = GetCurrentDirectory ();
		if (SubDir == _T(".."))
		{
			RetDir = GetCurrentDirectory ();

			if (RetDir.GetAt (RetDir.GetLength () -2) == _T(':'))
				RetDir.Delete(RetDir.GetLength () -1); // SHl RetDir.SetAt (RetDir.GetLength () -1, 0x0000);
			else if (RetDir.GetAt (RetDir.GetLength () -2) != _T('\\'))
			{
				RetDir = RetDir.Left (RetDir.ReverseFind (_T('\\')));
				RetDir = RetDir.Left (RetDir.ReverseFind (_T('\\')));
			}
		}
		else if (SubDir == _T("."))
			RetDir = RetDir.Left (RetDir.ReverseFind (_T('\\')));
		else
			RetDir += SubDir;
	}
	for (int i = 1; i < (int) list.GetCount (); i ++)
	{
		RetDir += _T("\\");
		RetDir += list.GetAt (i);
	}
	RetDir += _T("\\");
	return RetDir;
}

CString CFileToolkit::FormatFilename (CString Filename, CString ExtendName)
{
	CString name = FormatDirectory (Filename);
	if (!name.GetLength ()) return _T("");

	// 取得格式化后的文件名;删除‘\’
	name = name.Left (name.GetLength () - 1);
	// 添加扩展名
	if (GetFileExtendName (name) == _T(""))
		name = name + _T(".") + ExtendName;

	return name;
}

CString CFileToolkit::GetFileLogicName (CString Filename)
{
	Filename.Replace(L"/", L"\\");

	int EndPos = CStringToolkit::FindLastChar (Filename, _T('\\'));
	if (EndPos >= 0)
		return Filename.Mid (EndPos + 1);
	else
		return Filename;
}

CString CFileToolkit::GetFileExtendName (CString Filename)
{
	int i = CStringToolkit::FindLastChar (Filename, _T('.'));
	int j = CStringToolkit::FindLastChar (Filename, _T('\\'));
	if (i < j)
		return _T("");
	else if (i >= 0)
		return Filename.Mid (i);
	else
		return _T("");
}

CString CFileToolkit::GetFileBaseName (CString Filename)
{
	Filename.Replace(L"/", L"\\");

	CString LogName = GetFileLogicName (Filename);
	int i = CStringToolkit::FindLastChar (LogName, _T('.'));
	if (i >= 0)
		return LogName.Left (i);
	else
		return LogName;
}

CString CFileToolkit::GetFileDirectory (CString Filename)
{
	int i = CStringToolkit::FindLastChar (Filename, _T('\\'));
	if (i >= 0)
		return Filename.Left (i + 1);
	else
		return _T("");
}

CString CFileToolkit::SetFileExtendName (CString Filename, CString ExtendName)
{
	// 文件名不允许为空
	CString baseName = GetFileBaseName(Filename);
	if (baseName.IsEmpty())
		return _T("");

	ExtendName.Trim(_T('.'));
	if (!ExtendName.IsEmpty())
		ExtendName = _T(".") + ExtendName;

	CString ExtName = GetFileExtendName (Filename);
	CString RetName;
	if (!ExtName.GetLength ())
		RetName = Filename + ExtendName;
	else
		RetName = Filename.Left (Filename.GetLength () -
		ExtName.GetLength ()) + ExtendName;

	return RetName;
}

CString CFileToolkit::SetFileLogicName (CString Filename, CString LogicName)
{
	return GetFileDirectory (Filename) + LogicName;
}

CString CFileToolkit::SetFileBaseName (CString Filename, CString BaseName)
{
	if (BaseName.IsEmpty())
		return _T("");

	CString Ext = GetFileExtendName (Filename);
	if (Ext.GetLength ())
		return GetFileDirectory (Filename) + BaseName + Ext;
	else
		return GetFileDirectory (Filename) + BaseName;
}

CString CFileToolkit::SetFileDirectory (CString Filename, CString Directory)
{
	if (Directory.GetLength () && Directory.Right (1) != _T("\\"))
		return Directory + _T("\\") + GetFileLogicName (Filename);
	else
		return Directory + GetFileLogicName (Filename);
}

//CString CFileToolkit::SelectDirectory (char *Title, HWND hWnd)
//{
//	BROWSEINFO bi;
//	char chDPName [MAX_PATH], chPath[MAX_PATH];
//	ITEMIDLIST *pidl;
//	bi.hwndOwner = hWnd;
//	bi.pidlRoot = 0;
//	bi.pszDisplayName = chDPName;
//	bi.lpszTitle = Title;
//
//	bi.ulFlags = BIF_RETURNONLYFSDIRS;
//	bi.lpfn = 0;
//	bi.lParam = 0;
//	bi.iImage = 0;
//	if (pidl = SHBrowseForFolder (&bi))
//		SHGetPathFromIDList (pidl, chPath);
//	else
//		chPath[0] = NULL;
//	return chPath;
//}

bool CFileToolkit::CreateDirectory (CString Directory)
{
	CAtlArray<CString> list;
	bool isOk;
	CString oldDir, subDir, fillDir = FormatDirectory (Directory);

	if (!fillDir.GetLength ()) return false;	// 无效的目录
	// 只有网络计算机名称的目录无法建立
	if (CStringToolkit::CountWord (fillDir, _T('\\')) == 1) return false;
	DecDir (fillDir, &list);

	// 如果是网络，则把计算名与第一个目录合起来
	if (fillDir.Left (2) == _T("\\\\"))
	{
		subDir = _T("\\\\") + list[0] + _T("\\") + list[1];
		list.RemoveAt (0);
		list[0] = subDir;
	}

	subDir = _T(""); 
	isOk = true;
	oldDir = GetCurrentDirectory ();
	// 测试目录树
	for (int i = 0; i < (int) list.GetCount (); i ++)
	{
		subDir = subDir + list[i] + _T("\\");
		if (SetCurrentDirectory (subDir) == FALSE)
			if (::CreateDirectory (subDir, NULL) == FALSE)
			{
				isOk = false;
				break;
			}
	}
	SetCurrentDirectory (oldDir);
	return isOk;
}

bool CFileToolkit::FileExist (CString Filename)
{
	WIN32_FIND_DATA fd;
	HANDLE fhd;
	fhd = FindFirstFile (Filename, &fd);
	if (fhd != INVALID_HANDLE_VALUE)
	{
		FindClose (fhd);
		return true;
	}
	else
		return false;
}

bool CFileToolkit::DirectoryExist (CString Directory)
{
	CString oldDir = GetCurrentDirectory ();
	BOOL isOk = SetCurrentDirectory (Directory);
	SetCurrentDirectory (oldDir);
	return isOk == TRUE;
}

CString CFileToolkit::GetCurrentDirectory ()
{
	TCHAR dir[MAX_PATH];
	::GetCurrentDirectory (MAX_PATH, dir);

	// 当前目录是根目录时，已包含‘\’
	if (_tcsclen(dir) == 3)
		return dir;

	_tcscat_s(dir, MAX_PATH, _T("\\"));
	return dir;
}

bool CFileToolkit::ReadDirectory (CString Directory, CSimpleArray<WIN32_FIND_DATA>& arr)
{
	CString strDir = FormatDirectory (Directory);
	strDir = GetFileDirectory (strDir);
	if (strDir.GetLength () == 0)
		return false;

	strDir += _T("*.*");

	arr.RemoveAll ();

	WIN32_FIND_DATA findData;

	HANDLE hHandle = ::FindFirstFile (strDir, &findData);
	if (hHandle == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if (_tcscmp (findData.cFileName, _T(".")) && _tcscmp (findData.cFileName, _T("..")))
			arr.Add (findData);

	}while (::FindNextFile (hHandle, &findData) != FALSE);

	::FindClose (hHandle);

	return true;
}

bool CFileToolkit::ReadDirectory (CString Directory, CSimpleArray<CString>& arr, bool Recursion)
{
	CString strDir = FormatDirectory (Directory);
	Directory = strDir;
	strDir = GetFileDirectory (strDir);
	if (strDir.GetLength () == 0)
		return false;

	strDir += _T("*.*");

	WIN32_FIND_DATA findData;

	HANDLE hHandle = ::FindFirstFile (strDir, &findData);
	if (hHandle == INVALID_HANDLE_VALUE)
		return false;

	do
	{
		if (_tcscmp (findData.cFileName, _T(".")) && _tcscmp (findData.cFileName, _T("..")))
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (Recursion)
					ReadDirectory (Directory + findData.cFileName, arr, Recursion);
			}
			else
			{
				arr.Add (Directory + findData.cFileName);
			}
		}
	}while (::FindNextFile (hHandle, &findData) != FALSE);

	::FindClose (hHandle);

	return true;
}

// 删除文件或文件夹到回收站
bool CFileToolkit::DeleteToRecycle (CString strFile, FILEOP_FLAGS fFlag)
{
	if (strFile.IsEmpty())
		return false;

	// 清除目录最后一个斜杠符号
	if (strFile.GetAt (strFile.GetLength () -1) == _T('\\'))
		strFile.Delete (strFile.GetLength () -1, 1);

	strFile += _T('\0');
	SHFILEOPSTRUCT shFOInfo;
	memset (&shFOInfo, 0, sizeof (SHFILEOPSTRUCT));
	shFOInfo.hwnd = ::GetActiveWindow ();
	shFOInfo.wFunc = FO_DELETE;
	shFOInfo.pFrom = strFile;
	shFOInfo.fFlags = fFlag;
	shFOInfo.fAnyOperationsAborted = FALSE;

	int nRet = ::SHFileOperation (&shFOInfo);

	if(nRet == 0)
	{
		if(shFOInfo.fAnyOperationsAborted == TRUE)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return false;
}


// 根据文件扩展名取得对应的系统图标
bool CFileToolkit::GetIconByExtName (CString strExt, HICON& hIcon, int& nIndex, DWORD attrib)
{
	hIcon = NULL;

	SHFILEINFO  sfi;
	memset (&sfi, 0, sizeof (sfi));

	if (SHGetFileInfo(strExt, attrib, &sfi,  
		sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))  
	{
		nIndex = sfi.iIcon;
		hIcon = sfi.hIcon;  
	}  

	return hIcon != NULL;  
}


// 检查文件名是否为标准文件名（即不包含无效字符的文件名）(不能包含路径)
bool CFileToolkit::CheckFileNameValidate (CString strFile, TCHAR* pChar)
{
	// 文件名中不能存在的无效字符
	TCHAR InvalidChar[9+31] = {_T('\\'),_T('/'),_T(':'),_T('*'),_T('?'),_T('"'),_T('<'),_T('>'),_T('|')};
	for (int i = 1; i < 32; ++i)
	{
		InvalidChar[i+8] = i;
	}

	// 检查是否包含无效字符
	int nCount = sizeof (InvalidChar) / sizeof (TCHAR);
	for (int i = 0; i < nCount; ++i)
	{
		if (strFile.Find (InvalidChar[i]) != -1)
		{
			if (pChar != NULL)
				*pChar = InvalidChar[i];
			return false;
		}
	}

	// 文件名不能使用的设备名
	TCHAR InvalidStr[22][5] = {
		_T("CON"), _T("PRN"), _T("AUX"),
		_T("NUL"), _T("COM1"), _T("COM2"),
		_T("COM3"), _T("COM4"), _T("COM5"),
		_T("COM6"), _T("COM7"), _T("COM8"),
		_T("COM9"), _T("LPT1"), _T("LPT2"),
		_T("LPT3"), _T("LPT4"), _T("LPT5"),
		_T("LPT6"), _T("LPT7"), _T("LPT8"),
		_T("LPT9"),
	};
	CString baseName = GetFileBaseName(strFile);
	for (int i = 0; i < 22; ++i)
	{
		if (baseName == InvalidStr[i])
		{
			return false;
		}
	}

	return true;
}

typedef struct _IO_STATUS_BLOCK {
	LONG Status;
	LONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _FILE_NAME_INFORMATION {
	ULONG FileNameLength;
	WCHAR FileName[MAX_PATH];
} FILE_NAME_INFORMATION;

__declspec(dllimport) LONG __stdcall ZwQueryInformationFile (
	IN HANDLE FileHandle,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG FileInformationLength,
	IN ULONG FileInformationClass
	);

typedef LONG (__stdcall * PFN_ZwQueryInformationFile) (
	IN HANDLE FileHandle,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG FileInformationLength,
	IN ULONG FileInformationClass
	);

BOOL CFileToolkit::GetVolumeNameByHandle(HANDLE hFile, LPWSTR szVolumeName, UINT cchMax)
{
	BOOL bResult = FALSE;
	WCHAR szBuf[500] = { 0 };
	WCHAR * pIter = szBuf;
	int i = 0;
	BY_HANDLE_FILE_INFORMATION stFileInfo = { 0 };

	do 
	{
		if(FALSE == GetFileInformationByHandle(hFile, &stFileInfo)) {
			break;
		}

		if(0 == GetLogicalDriveStringsW(_countof(szBuf), szBuf)) {
			break;
		}

		for(; pIter; pIter+=4)
		{
			DWORD dwVolumeSerialNumber = 0;

			if(GetVolumeInformationW(pIter, NULL, 0, &dwVolumeSerialNumber, 
				NULL, NULL, NULL, 0))
			{
				if(dwVolumeSerialNumber == stFileInfo.dwVolumeSerialNumber)
				{
					lstrcpynW(szVolumeName, pIter, cchMax);
					bResult = TRUE;
					break;
				}
			}
		}

	} while (FALSE);

	return bResult;
}

typedef struct _OBJECT_NAME_INFORMATION {
    WORD Length;
    WORD MaximumLength;
    LPWSTR Buffer;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

typedef long(__stdcall * PNtQueryObject)(HANDLE ObjectHandle,ULONG ObjectInformationClass,PVOID ObjectInformation,ULONG ObjectInformationLength,PULONG ReturnLength);


/**根据文件句柄取得文件路径 */
BOOL CFileToolkit::GetFilePathFromHandle(HANDLE hFile, LPWSTR lpszPath, UINT cchMax)
{
	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	// LY 2013-11-08 解决由于GHOST进行分区对拷后分区序列号相同而导致的获取路径错误BUG
	BOOL bResult = FALSE;
	TCHAR pszFilename[MAX_PATH + 1] = {0};

	// 使用文件映射方式获取文件名
	do
	{
		BY_HANDLE_FILE_INFORMATION File_Info;
		GetFileInformationByHandle(hFile, &File_Info);

		// 文件大小为0时无法创建文件内存映射
		if (File_Info.nFileSizeHigh == 0 && File_Info.nFileSizeLow == 0)
			break;

		HANDLE hFileMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 1, NULL);
		if(hFileMap == NULL) 
			break;

		void * pMem = ::MapViewOfFile(hFileMap, FILE_MAP_READ, 0 , 0 , 1 );
		if (pMem == NULL)
		{
			::CloseHandle(hFileMap);
			break;
		}

		::GetMappedFileName(GetCurrentProcess(), pMem, pszFilename, MAX_PATH);

		::UnmapViewOfFile(pMem);
		::CloseHandle(hFileMap);
	}while(false);

	// 通过文件内存映射方式失败时使用 Native API 方式获取
	if (_tcslen(pszFilename) == 0)
	{
		do 
		{
			PNtQueryObject NtQueryObject(reinterpret_cast<PNtQueryObject>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"),"NtQueryObject")));

			if (NtQueryObject == NULL)
				break;

			OBJECT_NAME_INFORMATION name,*pname;
			ULONG len = 0;
			long value = NtQueryObject(hFile,1,&name,sizeof name,&len);
			if (len == 0)
				break;

			BYTE * buffer = new BYTE[len];
			ZeroMemory(buffer, len);
			pname=reinterpret_cast<POBJECT_NAME_INFORMATION>(buffer);
			value = NtQueryObject(hFile,1,pname,len,&len);
			if (pname->Buffer)
			{
				_tcscpy_s(pszFilename, MAX_PATH, pname->Buffer);
			}
			delete []buffer;
		} while (FALSE);
	}

	if (_tcslen(pszFilename) == 0)
		return FALSE;

	// 将 “\Device\HarddiskVolume3\xxx\abc.abc”形式的路径生成标准路径
	TCHAR szTemp[MAX_PATH] = { 0 };
	if (::GetLogicalDriveStrings(MAX_PATH - 1 , szTemp)) 
	{
		TCHAR szName[MAX_PATH];
		TCHAR szDrive[3] = _T(" :");
		bool bFound = false;
		TCHAR * p = szTemp;
		do 
		{
			*szDrive = * p;
			if (::QueryDosDevice(szDrive, szName, MAX_PATH) > 0)
			{
				UINT uNameLen = _tcslen(szName);
				if (uNameLen < MAX_PATH) 
				{
					bFound = ::_tcsncmp(pszFilename, szName, uNameLen) == 0;
					if (bFound) 
					{
						bResult = TRUE;

						_tcscpy_s(lpszPath, MAX_PATH, szDrive);
						PathAppend(lpszPath, pszFilename + uNameLen);
					}
				}
			}
			while(*p++);

		}while(!bFound && *p); 
	}

	return bResult;
}