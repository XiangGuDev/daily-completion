#include "StdAfx.h"
#include "DefImage.h"

HICON CDefImage::LoadSysIcon()
{
	return LoadIcon(L"AppStarter.ico");
}

HICON CDefImage::LoadIcon(const CString& strFile)
{
	return (HICON)::LoadImage(NULL, CPathConfig::GetResourcePath() +strFile, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
}

HBITMAP CDefImage::LoadBitmap(const CString& strFile)
{
	return (HBITMAP)::LoadImage(NULL, CPathConfig::GetResourcePath() +strFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

