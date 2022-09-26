#include "StdAfx.h"
#include "FileTool.h"
#include "StringTool.h"
#include <Psapi.h>
#include <CommDlg.h>


#pragma warning(disable:4091)
#pragma warning(disable:4996)
#include <shlobj.h>
#include <shellapi.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "shell32.lib")

namespace CommBase
{

	size_t DecDir(CString Filename, StringArray *arr)
	{
		int i, Cnt;
		arr->clear();
		Cnt = CStringTool::CountWord(Filename, _T('\\'));
		for (i = 1; i <= Cnt; i++)
			arr->push_back(CStringTool::ReadWord(Filename, i, _T('\\')));
		return (int)arr->size();
	}

	CString CFileTool::GetDrive(CString Path)
	{
		// 路径长度小于2时认为是非法路径
		if (Path.GetLength() < 2 && Path.GetAt(0) != '\\')
			return _T("");

		CString LeftTwo, name = Path;

		LeftTwo = name.Left(2);
		if (LeftTwo == _T("\\\\"))	// 网络
			return _T("\\\\") + CStringTool::ReadWord(name, 1, _T('\\'));
		else if (LeftTwo.GetAt(1) == _T(':'))	// 磁盘
		{
			name.MakeUpper();
			return name.Left(2);
		}
		else if (LeftTwo.GetAt(0) == _T('\\'))	// 根目录
			return GetCurrentDirectory().Left(2);
		else	// 无盘符
			return _T("");
	}

	CString CFileTool::FormatDirectory(CString Directory)
	{
		StringArray list;
		CString RetDir, SubDir;

		if (!Directory.GetLength()) return _T("");

		DecDir(Directory, &list);
		if (!list.size()) return _T("");

		SubDir = list[0];

		// 处理盘符
		if (Directory.Left(2) == _T("\\\\"))	// 网络
			RetDir = _T("\\\\") + SubDir;
		else if (Directory.GetAt(0) == _T('\\'))	// 根目录
			RetDir = GetCurrentDirectory().Left(3) + SubDir;
		else if (Directory.GetLength() > 1 &&
			Directory.GetAt(1) == _T(':'))	// 盘符
			RetDir = SubDir.Left(2);
		else	// 当前目录
		{
			RetDir = GetCurrentDirectory();
			if (SubDir == _T(".."))
			{
				RetDir = GetCurrentDirectory();

				if (RetDir.GetAt(RetDir.GetLength() - 2) == _T(':'))
					RetDir.Delete(RetDir.GetLength() - 1); // SHl RetDir.SetAt (RetDir.GetLength () -1, 0x0000);
				else if (RetDir.GetAt(RetDir.GetLength() - 2) != _T('\\'))
				{
					RetDir = RetDir.Left(RetDir.ReverseFind(_T('\\')));
					RetDir = RetDir.Left(RetDir.ReverseFind(_T('\\')));
				}
			}
			else if (SubDir == _T("."))
				RetDir = RetDir.Left(RetDir.ReverseFind(_T('\\')));
			else
				RetDir += SubDir;
		}
		for (size_t i = 1; i < list.size(); i++)
		{
			RetDir += _T("\\");
			RetDir += list[i];
		}
		RetDir += _T("\\");
		return RetDir;
	}

	CString CFileTool::FormatFilename(CString Filename, CString ExtendName)
	{
		CString name = FormatDirectory(Filename);
		if (!name.GetLength()) return _T("");

		// 取得格式化后的文件名;删除‘\’
		name = name.Left(name.GetLength() - 1);
		// 添加扩展名
		if (GetFileExtendName(name) == _T(""))
			name = name + _T(".") + ExtendName;

		return name;
	}

	CString CFileTool::GetFileLogicName(CString Filename)
	{
		int EndPos = Filename.ReverseFind(_T('\\'));
		if (EndPos >= 0)
			return Filename.Mid(EndPos + 1);
		else
			return Filename;
	}

	CString CFileTool::GetFileExtendName(CString Filename)
	{
		int i = Filename.ReverseFind(_T('.'));
		int j = Filename.ReverseFind(_T('\\'));
		if (i < j)
			return _T("");
		else if (i >= 0)
			return Filename.Mid(i);
		else
			return _T("");
	}

	CString CFileTool::GetFileBaseName(CString Filename)
	{
		CString LogName = GetFileLogicName(Filename);
		int i = LogName.ReverseFind(_T('.'));

		if (i >= 0)
		{
			LogName = LogName.Left(i);
		}

		return LogName.Trim();
	}

	CString CFileTool::GetFileDirectory(CString Filename)
	{
		int i = Filename.ReverseFind(_T('\\'));
		if (i >= 0)
			return Filename.Left(i);
		else
			return _T("");
	}

	CString CFileTool::SetFileExtendName(CString Filename, CString ExtendName)
	{
		// 文件名不允许为空
		CString baseName = GetFileBaseName(Filename);
		if (baseName.IsEmpty())
			return _T("");

		ExtendName.Trim(_T('.'));
		if (!ExtendName.IsEmpty())
			ExtendName = _T(".") + ExtendName;

		CString ExtName = GetFileExtendName(Filename);
		CString RetName;
		if (!ExtName.GetLength())
			RetName = Filename + ExtendName;
		else
			RetName = Filename.Left(Filename.GetLength() -
				ExtName.GetLength()) + ExtendName;

		return RetName;
	}

	CString CFileTool::SetFileLogicName(CString Filename, CString LogicName)
	{
		return GetFileDirectory(Filename) + LogicName;
	}

	CString CFileTool::SetFileBaseName(CString Filename, CString BaseName)
	{
		if (BaseName.IsEmpty())
			return _T("");

		CString Ext = GetFileExtendName(Filename);
		if (Ext.GetLength())
			return GetFileDirectory(Filename) +L"\\"+ BaseName + Ext;
		else
			return GetFileDirectory(Filename) + L"\\" + BaseName;
	}

	CString CFileTool::SetFileDirectory(CString Filename, CString Directory)
	{
		if (Directory.GetLength() && Directory.Right(1) != _T("\\"))
			return Directory + _T("\\") + GetFileLogicName(Filename);
		else
			return Directory + GetFileLogicName(Filename);
	}

	bool CFileTool::CreateDirectory(CString Directory)
	{
		return (ERROR_SUCCESS == SHCreateDirectoryEx(NULL, Directory, NULL));
	}

	bool CFileTool::FileExist(CString Filename)
	{
		return !!PathFileExists(Filename);
	}

	bool CFileTool::DirectoryExist(CString Directory)
	{
		return !!PathIsDirectory(Directory);
	}

	CString CFileTool::GetCurrentDirectory()
	{
		TCHAR dir[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, dir);

		// 当前目录是根目录时，已包含‘\’
		if (_tcsclen(dir) == 3)
			return dir;

		_tcscat(dir, _T("\\"));
		return dir;
	}

	bool CFileTool::ReadDirectory(CString Directory, StringArray& arr, bool Recursion, CString fileExt)
	{
		CString strNewDir = FormatDirectory(Directory);
		CString strDir = GetFileDirectory(strNewDir);
		if (strDir.GetLength() == 0)
			return false;

		strDir += _T("\\");
		bool bAllFileExt = (fileExt == L"*.*");

		WIN32_FIND_DATA findData;

		HANDLE hHandle = ::FindFirstFile(strDir + _T("*.*"), &findData);
		if (hHandle == INVALID_HANDLE_VALUE)
			return false;

		do
		{
			if (_tcscmp(findData.cFileName, _T(".")) && _tcscmp(findData.cFileName, _T("..")))
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (Recursion)
						ReadDirectory(strDir + findData.cFileName, arr, Recursion, fileExt);
				}
				else
				{
					if (bAllFileExt)
					{
						arr.push_back(strDir + findData.cFileName);
					}
					else
					{
						CString strExtName = _T("*") + CFileTool::GetFileExtendName(findData.cFileName);
						if (strExtName.CompareNoCase(fileExt) == 0)
							arr.push_back(strDir + findData.cFileName);
					}
				}
			}
		} while (::FindNextFile(hHandle, &findData) != FALSE);

		::FindClose(hHandle);

		return true;
	}

	bool CFileTool::ReadDirectoryDir(CString Directory, StringArray& arr, bool Recursion /*= false*/)
	{
		CString strNewDir = FormatDirectory(Directory);
		CString strDir = GetFileDirectory(strNewDir);
		if (strDir.GetLength() == 0)
			return false;

		strDir += _T("\\");

		WIN32_FIND_DATA findData;

		HANDLE hHandle = ::FindFirstFile(strDir + _T("*.*"), &findData);
		if (hHandle == INVALID_HANDLE_VALUE)
			return false;

		do
		{
			if (_tcscmp(findData.cFileName, _T(".")) && _tcscmp(findData.cFileName, _T("..")))
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					arr.push_back(strDir + findData.cFileName);

					if (Recursion)
						ReadDirectoryDir(strDir + findData.cFileName, arr, Recursion);
				}
			}
		} while (::FindNextFile(hHandle, &findData) != FALSE);

		::FindClose(hHandle);

		return true;
	}

	// 删除文件或文件夹
	bool CFileTool::DeleteDirectory(CString strDir)
	{
		if (strDir.IsEmpty())
			return false;

		//加入多文件结束符号
		strDir += _T('\0');

		SHFILEOPSTRUCT shFOInfo;
		memset(&shFOInfo, 0, sizeof(SHFILEOPSTRUCT));
		shFOInfo.hwnd = NULL;
		shFOInfo.wFunc = FO_DELETE;
		shFOInfo.pFrom = strDir;
		shFOInfo.fFlags = FOF_NO_UI;
		shFOInfo.fAnyOperationsAborted = FALSE;

		return (0 == ::SHFileOperation(&shFOInfo));
	}


	// 根据文件扩展名取得对应的系统图标
	bool CFileTool::GetIconByExtName(CString strExt, HICON& hIcon, int& nIndex, DWORD attrib)
	{
		hIcon = NULL;

		SHFILEINFO  sfi;
		memset(&sfi, 0, sizeof(sfi));

		if (SHGetFileInfo(strExt, attrib, &sfi,
			sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
		{
			nIndex = sfi.iIcon;
			hIcon = sfi.hIcon;
		}

		return hIcon != NULL;
	}


	// 检查文件名是否为标准文件名（即不包含无效字符的文件名）(不能包含路径)
	bool CFileTool::CheckFileNameValidate(CString strFile, TCHAR* pChar)
	{
		// 文件名中不能存在的无效字符
		TCHAR InvalidChar[9 + 31] = { _T('\\'), _T('/'), _T(':'), _T('*'), _T('?'), _T('"'), _T('<'), _T('>'), _T('|') };
		for (int i = 1; i < 32; ++i)
		{
			InvalidChar[i + 8] = i;
		}

		// 检查是否包含无效字符
		int nCount = sizeof(InvalidChar) / sizeof(TCHAR);
		for (int i = 0; i < nCount; ++i)
		{
			if (strFile.Find(InvalidChar[i]) != -1)
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

	CString CFileTool::GetValidateFileName(const CString strFile)
	{
		CString strRet = strFile;
		strRet.Trim();
		
		// 文件不支持的半角符号转换为全角
		strRet.Replace(_T("\\"), _T("＼"));
		strRet.Replace(_T("/"), _T("／"));
		strRet.Replace(_T(":"), _T("："));
		strRet.Replace(_T("*"), _T("＊"));
		strRet.Replace(_T("?"), _T("？"));
		strRet.Replace(_T("\""), _T("＂"));
		strRet.Replace(_T("<"), _T("＜"));
		strRet.Replace(_T(">"), _T("＞"));
		strRet.Replace(_T("|"), _T("｜"));
		strRet.Replace(_T("ϕ"), _T("φ"));

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

		for (int i = 0; i < 22; ++i)
		{
			if (strRet == InvalidStr[i])
			{
				return strRet + L"s";
			}
		}
		
		return strRet;
	}

	typedef struct _OBJECT_NAME_INFORMATION
	{
		WORD Length;
		WORD MaximumLength;
		LPWSTR Buffer;
	} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

	typedef long(__stdcall * PNtQueryObject)(HANDLE ObjectHandle, ULONG ObjectInformationClass, PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength);


	/**根据文件句柄取得文件路径 */
	BOOL CFileTool::GetFilePathFromHandle(HANDLE hFile, LPWSTR lpszPath, UINT cchMax)
	{
		if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}
		// LY 2013-11-08 解决由于GHOST进行分区对拷后分区序列号相同而导致的获取路径错误BUG
		BOOL bResult = FALSE;
		TCHAR pszFilename[MAX_PATH + 1] = { 0 };

		// 使用文件映射方式获取文件名
		do
		{
			BY_HANDLE_FILE_INFORMATION File_Info;
			GetFileInformationByHandle(hFile, &File_Info);

			// 文件大小为0时无法创建文件内存映射
			if (File_Info.nFileSizeHigh == 0 && File_Info.nFileSizeLow == 0)
				break;

			HANDLE hFileMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 1, NULL);
			if (hFileMap == NULL)
				break;

			void * pMem = ::MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 1);
			if (pMem == NULL)
			{
				::CloseHandle(hFileMap);
				break;
			}

			::GetMappedFileName(GetCurrentProcess(), pMem, pszFilename, MAX_PATH);

			::UnmapViewOfFile(pMem);
			::CloseHandle(hFileMap);
		} while (false);

		// 通过文件内存映射方式失败时使用 Native API 方式获取
		if (_tcslen(pszFilename) == 0)
		{
			do
			{
				PNtQueryObject NtQueryObject(reinterpret_cast<PNtQueryObject>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryObject")));

				if (NtQueryObject == NULL)
					break;

				OBJECT_NAME_INFORMATION name, *pname;
				ULONG len;
				NtQueryObject(hFile, 1, &name, sizeof name, &len);

				BYTE * buffer = new BYTE[len];
				pname = reinterpret_cast<POBJECT_NAME_INFORMATION>(buffer);
				NtQueryObject(hFile, 1, pname, len, &len);
				_tcscpy_s(pszFilename, MAX_PATH, pname->Buffer);
				delete[]buffer;
			} while (FALSE);
		}

		if (_tcslen(pszFilename) == 0)
			return FALSE;

		// 将 “\Device\HarddiskVolume3\xxx\abc.abc”形式的路径生成标准路径
		TCHAR szTemp[MAX_PATH] = { 0 };
		if (::GetLogicalDriveStrings(MAX_PATH - 1, szTemp))
		{
			TCHAR szName[MAX_PATH];
			TCHAR szDrive[3] = _T(" :");
			bool bFound = false;
			TCHAR * p = szTemp;
			do
			{
				*szDrive = *p;
				if (::QueryDosDevice(szDrive, szName, MAX_PATH) > 0)
				{
					UINT uNameLen = UINT(_tcslen(szName));
					if (uNameLen < MAX_PATH)
					{
						bFound = ::_tcsncmp(pszFilename, szName, uNameLen) == 0;
						if (bFound)
						{
							bResult = TRUE;

							_tcscpy(lpszPath, szDrive);
							PathAppend(lpszPath, pszFilename + uNameLen);
						}
					}
				}
				while (*p++);

			} while (!bFound && *p);
		}

		return bResult;
	}

	BOOL CFileTool::CopyDirectory(const CString& srcDir, const CString& dstDir)
	{
		TCHAR szFrom[MAX_PATH] = { L'\0' };
		TCHAR szTo[MAX_PATH] = { L'\0' };

		::memcpy(szFrom, (LPCTSTR)srcDir, sizeof(TCHAR) * srcDir.GetLength());
		::memcpy(szTo, (LPCTSTR)dstDir, sizeof(TCHAR) * dstDir.GetLength());

		SHFILEOPSTRUCT shFOInfo;
		memset(&shFOInfo, 0, sizeof(SHFILEOPSTRUCT));
		shFOInfo.hwnd = NULL;
		shFOInfo.wFunc = FO_COPY;
		shFOInfo.pFrom = szFrom;
		shFOInfo.pTo = szTo;
		shFOInfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
		shFOInfo.fAnyOperationsAborted = FALSE;

		int nRet = ::SHFileOperation(&shFOInfo);

		return nRet == 0;
	}

	CString CFileTool::GetRelatePath(const CString& strSrcPath, const CString& strRelateDir)
	{
		// 计算图纸相对位置
		WCHAR szBuffer1[MAX_PATH] = { 0 };
		::PathRelativePathTo(szBuffer1, strRelateDir,
			FILE_ATTRIBUTE_DIRECTORY, strSrcPath, FILE_ATTRIBUTE_NORMAL);
		CString strRelatePath = szBuffer1;

		return strRelatePath;
	}

	/** 取得临时路径（c:\）　*/
	CString CFileTool::GetTempPath(void)
	{
		TCHAR m_pstr[MAX_PATH];

		DWORD dwLength = ::GetTempPath(MAX_PATH, m_pstr);
		m_pstr[dwLength] = _T('\0');
		return m_pstr;
	}

	BOOL CALLBACK FindTreeViewCallback(HWND hwnd, LPARAM lParam)
	{
		TCHAR szClassName[MAX_PATH];
		szClassName[0] = 0;
		::GetClassName(hwnd, szClassName, sizeof(szClassName));
		szClassName[MAX_PATH - 1] = 0;
		if (lstrcmpi(szClassName, L"SysTreeView32") == 0)
		{
			HWND* phWnd = (HWND*)lParam;
			if (phWnd)
				*phWnd = hwnd;
			return FALSE;
		}
		return TRUE;
	}

	int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		static BOOL bProcessEnsureVisible = FALSE;

		TCHAR szPath[MAX_PATH];
		LPCITEMIDLIST pidl;

		switch (uMsg)
		{
		case BFFM_INITIALIZED:
		{
			bProcessEnsureVisible = FALSE;

			OSVERSIONINFO osi;
			osi.dwOSVersionInfoSize = sizeof(osi);
			if (GetVersionEx(&osi) &&
				osi.dwMajorVersion >= 6 && osi.dwMinorVersion >= 1)
			{
				bProcessEnsureVisible = TRUE;
			}
			::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
		}
		break;
		case BFFM_SELCHANGED:
		{
			pidl = LPCITEMIDLIST(lParam);
			SHGetPathFromIDList(pidl, szPath);
			if (szPath[0] == '\0')
			{
				SendMessage(hwnd, BFFM_ENABLEOK, lParam, 0);
			}
			TCHAR szRoot[MAX_PATH + 1];
			lstrcpyn(szRoot, szPath, MAX_PATH);
			::PathStripToRoot(szRoot);

			// 如果不是磁盘或者不是文件夹，不让OK
			if (GetDriveType(szRoot) != DRIVE_FIXED || ((GetFileAttributes(szPath) & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY))
			{
				SendMessage(hwnd, BFFM_ENABLEOK, lParam, 0);
			}

			if (!bProcessEnsureVisible)
			{
				break;
			}

			bProcessEnsureVisible = FALSE;
			HWND hWndTree = NULL;
			HTREEITEM hItm = NULL;
			EnumChildWindows(hwnd, FindTreeViewCallback, (LPARAM)&hWndTree);
			if (!hWndTree || !(hItm = TreeView_GetSelection(hWndTree)))
			{
				break;
			}
			TreeView_EnsureVisible(hWndTree, hItm);
		}
		break;

		case BFFM_VALIDATEFAILED:
			return 1;

		}
		return 0;
	}

	bool CFileTool::OpenBrowseDirDlg(const CString& strTitle, CString& strPathChoosed, HWND hwndOwner /*= NULL*/, const CString& strDirBegin /*= _T("")*/)
	{
		BROWSEINFO bi;
		TCHAR path[MAX_PATH];
		ITEMIDLIST __unaligned  *pidl = NULL;

		bi.hwndOwner = hwndOwner;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = path;
		bi.lpszTitle = strTitle;
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
		bi.lpfn = strDirBegin.IsEmpty() ? NULL : BrowseCallbackProc;
		bi.lParam = (LPARAM)(LPCTSTR)strDirBegin;
		bi.iImage = 0;

		pidl = SHBrowseForFolder(&bi);
		if (!pidl || !SHGetPathFromIDList(pidl, path))
		{
			return false;
		}

		strPathChoosed = path;
		CoTaskMemFree(pidl);
		return true;
	}

	bool CFileTool::EncryptFile(const CString& srcFile, const CString& dstFile, int nHeadLen, int nKey)
	{
		// 打开源文件
		CAtlFile fileSrc;
		if (S_OK != fileSrc.Create(srcFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))
			return false;

		ULONGLONG nFileSize;
		fileSrc.GetSize(nFileSize);
		if (nFileSize <= nHeadLen)
			return false;

		// 读取源数据
		BYTE* pDataBuffer = new BYTE[(UINT)nFileSize];
		fileSrc.Read(pDataBuffer, (UINT)nFileSize);
		fileSrc.Close();

		// 创建目标文件
		CAtlFile fileDst;
		if (S_OK != fileDst.Create(dstFile, GENERIC_ALL, FILE_SHARE_WRITE, CREATE_NEW))
		{
			delete[] pDataBuffer;
			return false;
		}

		// 加密文件头
		BYTE* lpKey = (BYTE*)&nKey;
		for (ULONGLONG i = 0; i < nHeadLen; ++i)
		{
			pDataBuffer[i] ^= lpKey[i % 4];
		}
		// 加密文件主体
		nKey = (nKey & 0xB3641259);
		for (ULONGLONG i = 0; i < nFileSize; ++i)
		{
			pDataBuffer[i] ^= lpKey[i % 4];
		}
		fileDst.Write(pDataBuffer, (UINT)nFileSize);

		// 写加密识别标记
		CHAR tagChar[] = "PWSJFILE";
		fileDst.Write(&tagChar, UINT(strlen(tagChar)));

		delete[] pDataBuffer;
		fileDst.Close();

		return true;
	}

	bool CFileTool::DeEncryptFile(const CString& srcFile, const CString& dstFile, int nHeadLen, int nKey)
	{
		// 打开源文件
		CAtlFile fileSrc;
		if (S_OK != fileSrc.Create(srcFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))
			return false;

		ULONGLONG nFileSize;
		fileSrc.GetSize(nFileSize);
		if (nFileSize <= nHeadLen)
			return false;

		CHAR tagChar[] = "PWSJFILE";
		nFileSize -= strlen(tagChar);

		// 读取源数据
		BYTE* pDataBuffer = new BYTE[(UINT)nFileSize];
		fileSrc.Read(pDataBuffer, (UINT)nFileSize);
		fileSrc.Close();

		// 创建目标文件
		CAtlFile fileDst;
		if (S_OK != fileDst.Create(dstFile, GENERIC_ALL, FILE_SHARE_WRITE, CREATE_NEW))
		{
			delete[] pDataBuffer;
			return false;
		}

		// 解密文件头
		BYTE* lpKey = (BYTE*)&nKey;
		for (ULONGLONG i = 0; i < nHeadLen; ++i)
		{
			pDataBuffer[i] ^= lpKey[i % 4];
		}
		// 解密文件主体
		nKey = (nKey & 0xB3641259);
		for (ULONGLONG i = 0; i < nFileSize; ++i)
		{
			pDataBuffer[i] ^= lpKey[i % 4];
		}
		fileDst.Write(pDataBuffer, (UINT)nFileSize);

		delete[] pDataBuffer;
		fileDst.Close();

		return true;
	}

	bool CFileTool::IsFileEncrypt(const CString& strFile)
	{
		CAtlFile testFile;
		if (S_OK != testFile.Create(strFile, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING))
			return false;

		CHAR tagChar[] = "PWSJFILE";
		INT nTagLen = (INT)strlen(tagChar);

		ULONGLONG nFileSize;
		testFile.GetSize(nFileSize);
		if (nFileSize <= nTagLen)
			return false;

		CHAR* pReadTagChar = new CHAR[nTagLen];
		testFile.Seek(-nTagLen, FILE_END);
		testFile.Read(pReadTagChar, UINT(nTagLen));

		// 比较是否跟原标记相同
		bool bSame = !memcmp(tagChar, pReadTagChar, nTagLen);
		delete[] pReadTagChar;

		return bSame;
	}
	StringArray CFileTool::ImportFileWnd(const LPCTSTR& szFilters, const LPCTSTR& strDefExt)
	{

		OPENFILENAME ofn = { 0 };
		TCHAR szBuffer[MAX_PATH] = { 0 };
		TCHAR szPath[MAX_PATH];
		TCHAR* p;
		int nLen = 0;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetActiveWindow();

		TCHAR filterBuf[MAX_PATH];
		ZeroMemory(filterBuf, MAX_PATH);
		TCHAR* pBuf = filterBuf;
		StringArray filters;
		CStringTool::SplitString(szFilters, filters, L"|");
		for (size_t i = 0; i < filters.size(); ++i)
		{
			if (filters[i].IsEmpty())
				break;
			int nLen = filters[i].GetLength();
			lstrcpy(pBuf, filters[i].GetString());
			pBuf += (nLen + 1);
		}
		ofn.lpstrFilter = filterBuf;//要选择的文件后缀 
		ofn.lpstrFile = szBuffer;
		ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
		ofn.lpstrDefExt = strDefExt;
		ofn.lpstrTitle = _T("打开");
		ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_EXTENSIONDIFFERENT;

		StringArray strArr;
		if (GetOpenFileName(&ofn))
		{
			//把第一个文件名前的复制到szPath,即:
			//如果只选了一个文件,就复制到最后一个'/'
			//如果选了多个文件,就复制到第一个NULL字符
			lstrcpyn(szPath, szBuffer, ofn.nFileOffset);
			//当只选了一个文件时,下面这个NULL字符是必需的.
			//这里不区别对待选了一个和多个文件的情况
			if (lstrlen(szBuffer) > ofn.nFileOffset)
			{
				strArr.push_back(szBuffer);
			}
			else
			{
				nLen = lstrlen(szPath);

				if (szPath[nLen - 1] != '\\')   //如果选了多个文件,则必须加上'//'
				{
					lstrcat(szPath, TEXT("\\"));
				}

				p = szBuffer + ofn.nFileOffset; //把指针移到第一个文件


				while (*p)
				{
					TCHAR szFileName[MAX_PATH];
					ZeroMemory(szFileName, sizeof(szFileName));
					lstrcat(szFileName, szPath);  //给文件名加上路径  
					lstrcat(szFileName, p);    //加上文件名  
					strArr.push_back(szFileName);
					p += lstrlen(p) + 1;     //移至下一个文件
				}
			}
		}
		return strArr;
	}
	CString CFileTool::ExportFileWnd(const CString& strName, const CString& szFilters, const LPCTSTR& strDefExt)
	{
		TCHAR szBuffer[MAX_PATH] = { '\0' };
		_tcscpy(szBuffer, strName);
		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = GetActiveWindow();

		TCHAR filterBuf[MAX_PATH];
		ZeroMemory(filterBuf, MAX_PATH);
		TCHAR* pBuf = filterBuf;
		StringArray filters;
		CStringTool::SplitString(szFilters, filters, L"|");
		for (size_t i = 0; i < filters.size(); ++i)
		{	
			if (filters[i].IsEmpty())
				break;
			int nLen = filters[i].GetLength();
			lstrcpy(pBuf, filters[i].GetString());
			pBuf += (nLen + 1);
		}
		ofn.lpstrFilter = filterBuf;//要选择的文件后缀 
		ofn.lpstrFile = szBuffer;//存放文件的缓冲区 
		ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
		ofn.nFilterIndex = 0;
		ofn.lpstrDefExt = strDefExt;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_OVERWRITEPROMPT; //标志如果是多选要加上OFN_ALLOWMULTISELECT
		if (!GetSaveFileName(&ofn))
			return L"";

		return szBuffer;
	}
}