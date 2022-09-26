#include "StdAfx.h"
#include "StaticPic.h"
#include <GdiPlus.h>

using namespace Gdiplus;

// 释放COM组件的宏
#ifdef SAFE_RELEASE
#undef SAFE_RELEASE
#endif
#define SAFE_RELEASE(x) do{\
							if((x) != NULL)\
							{\
								while((x)->Release() != 0);\
								(x) = NULL;\
							}\
						}while(0)


StaticPic::StaticPic(void) : CStatic(),
m_pStream(NULL),
m_pStreamClosePic(NULL),
m_bIsPicLoaded(FALSE),
m_bIsClosePicLoaded(false),
m_gdiplusToken(0)
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

StaticPic::~StaticPic(void)
{
	FreeData();
	GdiplusShutdown(m_gdiplusToken);
}

IStream* StaticPic::FileStream(const CString& strPicFilePath)
{
	IStream* pStream = nullptr;
	// 分配流
	DWORD dwResult = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
	if (dwResult != S_OK)
	{
		SetLastError(dwResult);
		return nullptr;
	}

	// 打开指定文件
	CFile cFile;
	CFileException cFileException;
	if (!cFile.Open(strPicFilePath, CStdioFile::modeRead | CStdioFile::typeBinary, &cFileException))
	{
		SetLastError(cFileException.m_lOsError);
		SAFE_RELEASE(pStream);
		return FALSE;
	}

	// 加载文件到流
	BYTE pBuffer[1024] = { 0 };
	while (UINT dwRead = cFile.Read(pBuffer, 1024))
	{
		dwResult = pStream->Write(pBuffer, dwRead, NULL);
		if (dwResult != S_OK)
		{
			SetLastError(dwResult);
			SAFE_RELEASE(pStream);
			cFile.Close();
			return FALSE;
		}
	}

	cFile.Close();
	return pStream;
}

BOOL StaticPic::LoadPicFiles(const CString& strMainPicFilePath, const CString& strClosePicFilePath)
{
	SetLastError(ERROR_SUCCESS);
	FreeData();

	m_pStream = FileStream(strMainPicFilePath);
	if (nullptr == m_pStream)
	{
		return FALSE;
	}

	m_strMainPicFilePath = strMainPicFilePath;

	m_bIsPicLoaded = TRUE;
	if (strClosePicFilePath != L"")
	{
		m_pStreamClosePic = FileStream(strClosePicFilePath);
		if (nullptr == m_pStreamClosePic)
		{
			return FALSE;
		}
		m_bIsClosePicLoaded = true;
	}

	Invalidate();
	RedrawWindow();

	return TRUE;
}

void StaticPic::FreeData()
{
	m_bIsPicLoaded = FALSE;
	SAFE_RELEASE(m_pStream);
	SAFE_RELEASE(m_pStreamClosePic);
}

CString StaticPic::GetMainPicPath()
{
	return m_strMainPicFilePath;
}

void StaticPic::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_OWNERDRAW);
}

void StaticPic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (m_bIsPicLoaded)
	{
		RECT rc;
		this->GetClientRect(&rc);

		Graphics graphics(lpDrawItemStruct->hDC);
		Gdiplus::Image image(m_pStream);

		int imageWidth = (INT)image.GetWidth();
		int imageHeight = (INT)image.GetHeight();
		int rcWidth = (INT)(rc.right - rc.left);
		int rcHeight = (INT)(rc.bottom - rc.top);

		if (imageWidth <= rcWidth && imageHeight <= rcHeight)
		{
			graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, imageWidth, imageHeight);
		}
		else
		{
			graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, rcWidth, rcHeight);
		}

		if (m_bIsClosePicLoaded)
		{
			RECT rcClosePic = { rc.right - 16, 0, rc.right, 16 };
			Gdiplus::Image imageClose(m_pStreamClosePic);
			graphics.DrawImage(&imageClose, (INT)rcClosePic.left, (INT)rcClosePic.top, (INT)(rcClosePic.right - rcClosePic.left), (INT)(rcClosePic.bottom - rcClosePic.top));
		}
	}
}

BOOL StaticPic::OnEraseBkgnd(CDC *pDC)
{
	if (m_bIsPicLoaded)
	{
		RECT rc;
		this->GetClientRect(&rc);

		Graphics graphics(pDC->GetSafeHdc());
		LARGE_INTEGER liSeekPos;
		liSeekPos.QuadPart = 0;
		m_pStream->Seek(liSeekPos, STREAM_SEEK_SET, NULL);
		Gdiplus::Image image(m_pStream);
		graphics.DrawImage(&image, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left), (INT)(rc.bottom - rc.top));
		return TRUE;
	}
	else
	{
		return CStatic::OnEraseBkgnd(pDC);
	}
}