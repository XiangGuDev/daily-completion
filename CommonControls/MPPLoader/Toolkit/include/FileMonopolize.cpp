#include "stdafx.h"
 #include "..\Include\FileMonopolize.h"
#include "FileToolkit.h"
// #include "..\Include\FileToolkit.h"

using namespace BwProj::toolkit;
CFileMonopolize::CFileMonopolize() : _handle(INVALID_HANDLE_VALUE)
{
}


CFileMonopolize::~CFileMonopolize()
{
	Release();
}

bool CFileMonopolize::Reset(CString strFilenames)
{
	Release();
	
	HANDLE hFile = ::CreateFile(strFilenames, FILE_READ_DATA, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		Release();
		return false;
	}

	_handle = hFile;
	return true;
}

void CFileMonopolize::Release(void)
{
	if (_handle != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
}

bool CFileMonopolize::IsReadable(LPCTSTR szFileName)
{
	if (szFileName == NULL)
		return false;

	HANDLE hFile = ::CreateFile(szFileName, FILE_READ_DATA, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		::CloseHandle(hFile);
	}

	return true;
}

bool CFileMonopolize::IsWritable(LPCTSTR szFileName)
{
	if (szFileName == NULL)
		return false;

	if(!BwProj::toolkit::CFileToolkit::FileExist(szFileName))
		return true;

	HANDLE hFile = ::CreateFile(szFileName, FILE_READ_DATA | FILE_WRITE_DATA, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	else
	{
		::CloseHandle(hFile);
	}

	return true;
}

CFileMonopolizeHelper::CFileMonopolizeHelper(LPCTSTR szFileName, bool bAnyFile/* = false*/) : _handle(INVALID_HANDLE_VALUE)
{
	if (szFileName != NULL)
	{
		CString strFileExtend = BwProj::toolkit::CFileToolkit::GetFileExtendName(szFileName);
		if (bAnyFile || strFileExtend.MakeUpper() == _T(".PDF"))
		{
			_handle = ::CreateFile(szFileName, FILE_READ_DATA, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
	}
}

CFileMonopolizeHelper::~CFileMonopolizeHelper()
{
	if (_handle != INVALID_HANDLE_VALUE)
		CloseHandle(_handle);
}
