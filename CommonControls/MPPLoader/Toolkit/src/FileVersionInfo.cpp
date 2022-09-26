#include "stdafx.h"
#include "..\include\FileVersionInfo.h"

using namespace BwProj::toolkit;

CFileVersionInfo::CFileVersionInfo()
{
	Reset();
}

CFileVersionInfo::~CFileVersionInfo() {}

BOOL CFileVersionInfo::Create(HMODULE hModule)
{
	CString	strPath;

	GetModuleFileName(hModule, strPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	strPath.ReleaseBuffer();
	return Create(strPath);
}

BOOL CFileVersionInfo::Create(LPCTSTR lpszFileName)
{
	Reset();

	DWORD	dwHandle;
	DWORD	dwFileVersionInfoSize = GetFileVersionInfoSize((LPTSTR)lpszFileName, &dwHandle);
	if (!dwFileVersionInfoSize)
		return FALSE;

	LPVOID	lpData = (LPVOID)new BYTE[dwFileVersionInfoSize];
	if (!lpData)
		return FALSE;

	try
	{
		if (!GetFileVersionInfo((LPTSTR)lpszFileName, dwHandle, dwFileVersionInfoSize, lpData))
			throw FALSE;

		// catch default information，廖成慧加，把下面的 LPVOID	lpInfo　替换换．
		struct LANGANDCODEPAGE
		{
			WORD wLanguage;
			WORD wCodePage;
		} *lpInfo;

		//LPVOID	lpInfo;
		UINT		unInfoLen;
		if (VerQueryValue(lpData, _T("\\"), (LPVOID*)&lpInfo, &unInfoLen))
		{
			ATLASSERT(unInfoLen == sizeof(m_FileInfo));
			if (unInfoLen == sizeof(m_FileInfo))
				memcpy(&m_FileInfo, lpInfo, unInfoLen);
		}

		// find best matching language and codepage
		VerQueryValue(lpData, _T("\\VarFileInfo\\Translation"), (LPVOID*)&lpInfo, &unInfoLen);

		DWORD	dwLangCode = 0;
		if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, FALSE))
		{
			if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, TRUE))
			{
				if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), dwLangCode, TRUE))
				{
					if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), dwLangCode, TRUE))
						// use the first one we can get
						dwLangCode = *((DWORD*)lpInfo);
				}
			}
		}


		CString	strSubBlock;

		// 以下语句是廖成慧改的，dwLangCode&0x0000FFFF是地址运行而不是值运算，肯定是错的，现改成取值。
		//strSubBlock.Format(_T("\\StringFileInfo\\%04X%04X\\"), dwLangCode&0x0000FFFF, (dwLangCode&0xFFFF0000)>>16);
		strSubBlock.Format(_T("\\StringFileInfo\\%04X%04X\\"), lpInfo->wLanguage, lpInfo->wCodePage);

		// catch string table
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("CompanyName")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strCompanyName = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("FileDescription")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strFileDescription = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("FileVersion")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strFileVersion = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("InternalName")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strInternalName = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("LegalCopyright")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strLegalCopyright = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("OriginalFileName")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strOriginalFileName = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("ProductName")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strProductName = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("ProductVersion")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strProductVersion = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("Comments")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strComments = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("LegalTrademarks")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strLegalTrademarks = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("PrivateBuild")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strPrivateBuild = CString((LPCTSTR)lpInfo);
		if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock+_T("SpecialBuild")), (LPVOID*)&lpInfo, &unInfoLen))
			m_strSpecialBuild = CString((LPCTSTR)lpInfo);

		delete[] lpData;
	}
	catch (BOOL)
	{
		delete[] lpData;
		return FALSE;
	}

	return TRUE;
}

WORD CFileVersionInfo::GetFileVersion(int nIndex) const
{
	if (nIndex == 0)
		return (WORD)(m_FileInfo.dwFileVersionLS & 0x0000FFFF);
	else if (nIndex == 1)
		return (WORD)((m_FileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);
	else if (nIndex == 2)
		return (WORD)(m_FileInfo.dwFileVersionMS & 0x0000FFFF);
	else if (nIndex == 3)
		return (WORD)((m_FileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);
	else
		return 0;
}


WORD CFileVersionInfo::GetProductVersion(int nIndex) const
{
	if (nIndex == 0)
		return (WORD)(m_FileInfo.dwProductVersionLS & 0x0000FFFF);
	else if (nIndex == 1)
		return (WORD)((m_FileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);
	else if (nIndex == 2)
		return (WORD)(m_FileInfo.dwProductVersionMS & 0x0000FFFF);
	else if (nIndex == 3)
		return (WORD)((m_FileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);
	else
		return 0;
}


DWORD CFileVersionInfo::GetFileFlagsMask() const
{
	return m_FileInfo.dwFileFlagsMask;
}


DWORD CFileVersionInfo::GetFileFlags() const
{
	return m_FileInfo.dwFileFlags;
}


DWORD CFileVersionInfo::GetFileOs() const
{
	return m_FileInfo.dwFileOS;
}


DWORD CFileVersionInfo::GetFileType() const
{
	return m_FileInfo.dwFileType;
}


DWORD CFileVersionInfo::GetFileSubtype() const
{
	return m_FileInfo.dwFileSubtype;
}


CTime CFileVersionInfo::GetFileDate() const
{
	FILETIME	ft;
	ft.dwLowDateTime = m_FileInfo.dwFileDateLS;
	ft.dwHighDateTime = m_FileInfo.dwFileDateMS;
	return CTime(ft);
}


CString CFileVersionInfo::GetCompanyName() const
{
	return m_strCompanyName;
}


CString CFileVersionInfo::GetFileDescription() const
{
	return m_strFileDescription;
}


CString CFileVersionInfo::GetFileVersion() const
{
	return m_strFileVersion;
}


CString CFileVersionInfo::GetInternalName() const
{
	return m_strInternalName;
}


CString CFileVersionInfo::GetLegalCopyright() const
{
	return m_strLegalCopyright;
}


CString CFileVersionInfo::GetOriginalFileName() const
{
	return m_strOriginalFileName;
}


CString CFileVersionInfo::GetProductName() const
{
	return m_strProductName;
}


CString CFileVersionInfo::GetProductVersion() const
{
	return m_strProductVersion;
}


CString CFileVersionInfo::GetComments() const
{
	return m_strComments;
}


CString CFileVersionInfo::GetLegalTrademarks() const
{
	return m_strLegalTrademarks;
}


CString CFileVersionInfo::GetPrivateBuild() const
{
	return m_strPrivateBuild;
}


CString CFileVersionInfo::GetSpecialBuild() const
{
	return m_strSpecialBuild;
}

void CFileVersionInfo::Reset()
{
	ZeroMemory(&m_FileInfo, sizeof(m_FileInfo));
	m_strCompanyName.Empty();
	m_strFileDescription.Empty();
	m_strFileVersion.Empty();
	m_strInternalName.Empty();
	m_strLegalCopyright.Empty();
	m_strOriginalFileName.Empty();
	m_strProductName.Empty();
	m_strProductVersion.Empty();
	m_strComments.Empty();
	m_strLegalTrademarks.Empty();
	m_strPrivateBuild.Empty();
	m_strSpecialBuild.Empty();
}

BOOL CFileVersionInfo::GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, BOOL bPrimaryEnough/*= FALSE*/)
{
	LPWORD lpwData = NULL;
	for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
	{
		if (*lpwData == wLangId)
		{
			dwId = *((DWORD*)lpwData);
			return TRUE;
		}
	}

	if (!bPrimaryEnough)
		return FALSE;

	for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData)+unBlockSize; lpwData+=2)
	{
		if (((*lpwData)&0x00FF) == (wLangId&0x00FF))
		{
			dwId = *((DWORD*)lpwData);
			return TRUE;
		}
	}

	return FALSE;
}
