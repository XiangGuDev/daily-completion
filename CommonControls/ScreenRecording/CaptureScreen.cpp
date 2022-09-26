#include "stdafx.h"  // #include "stdafx.h" // #include "pch.h" // 预编译头
#include "CaptureScreen.h"
#include "ScreenRecorder.h"

CCaptureScreen::CCaptureScreen(CScreenRecorder *pSR)
{
	// 全屏设置（缺省）
	m_hSavedCursor = NULL;
	hScreenDC = NULL;
	hMemDC = NULL;
	hbm = NULL;
	_bFullScreen = true;
	_pSR = pSR;
	FetchCursorHandle();
	Init();
}

CCaptureScreen::CCaptureScreen(CScreenRecorder *pSR, int width, int height)
{
	// 区域设置（有参）
	m_hSavedCursor = NULL;
	hScreenDC = NULL;
	hMemDC = NULL;
	hbm = NULL;

	m_width = width;
	m_height = height;
	_bFullScreen = false;
	_pSR = pSR;
	FetchCursorHandle();
	Init();
}
//
// 释放资源
//
CCaptureScreen::~CCaptureScreen(void)
{
	DeleteObject(hbm);
	if (hScreenDC){

		::ReleaseDC(NULL, hScreenDC);
	}
	if (hMemDC) {

		DeleteDC(hMemDC);
	}
	if (hbm)
	{
		DeleteObject(hbm);
	}
	// 获取屏幕dc
	auto pWnd = _pSR ? _pSR->GetCurWnd() : NULL;
	auto hWnd = pWnd ? pWnd->GetSafeHwnd() : NULL;

	::ReleaseDC(hWnd ? hWnd : GetDesktopWindow(), hMemDC);
}

//
// 初始化
//
int CCaptureScreen::Init()
{
	// 获取屏幕dc
	auto pWnd = _pSR ? _pSR->GetCurWnd() : NULL;
	auto hWnd = pWnd ? pWnd->GetSafeHwnd() : NULL;

	hScreenDC = ::GetDC(hWnd ? hWnd : GetDesktopWindow());
	if (hScreenDC == NULL) return 0;

	int m_nMaxxScreen = GetDeviceCaps(hScreenDC, HORZRES);
	int m_nMaxyScreen = GetDeviceCaps(hScreenDC, VERTRES);

	hMemDC = ::CreateCompatibleDC(hScreenDC);
	if (hMemDC == NULL) return 0;

	if (_bFullScreen)
	{
		m_width = m_nMaxxScreen;
		m_height = m_nMaxyScreen;
	}
	// 补位为4的倍数
	if (m_width % 4 != 0 || m_height % 4 != 0)
	{
		m_width = m_width - m_width % 4;
		m_height = m_height - m_height % 4;
	}

	hbm = CreateCompatibleBitmap(hScreenDC, m_width, m_height);
	SelectObject(hMemDC, hbm);

	wLineLen = ((m_width * 24 + 31) & 0xffffffe0) / 8;
	wColSize = sizeof(RGBQUAD)* ((24 <= 8) ? 1 << 24 : 0);
	dwSize = (DWORD)(UINT)wLineLen * (DWORD)(UINT)m_height;

	//位图头信息结构体
	pbi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbi.bmiHeader.biWidth = m_width;
	pbi.bmiHeader.biHeight = m_height;
	pbi.bmiHeader.biPlanes = 1;
	pbi.bmiHeader.biBitCount = 24;
	pbi.bmiHeader.biCompression = BI_RGB;
	pbi.bmiHeader.biSizeImage = dwSize;

	_hcur = LoadCursor(NULL, IDC_ARROW);

	return 1;
}

void CCaptureScreen::GetSize(int & nWidth, int & nHeight)
{
	nWidth = m_width;
	nHeight = m_height;
}

//抓取屏幕数据
BYTE* CCaptureScreen::CaptureImage()
{

	VOID*  alpbi = CaptureScreenFrame(0, 0, m_width, m_height);
	return (BYTE*)(alpbi);
}

BYTE* CCaptureScreen::CaptureImage(int left, int top, int width, int height)
{

	VOID*  alpbi = CaptureScreenFrame(left, top, width, height);
	return (BYTE*)(alpbi);
}
void CCaptureScreen::ShowMouse(bool bShow)
{
	_bShowMouse = bShow;
}

void CCaptureScreen::AddText(PRGBTRIPLE hdib, int left, int top, int width, int height)
{
	if (_pSR == NULL)
		return;

	CFont font;
	CSize fontSize;

	// 文字大小
	fontSize.cx = 20;
	fontSize.cy = 40;

	// 文字位置
	CSize fontPos;
	fontPos = _pSR->GetTextPos();

	if (fontPos.cx == 0 && fontPos.cy == 0)
	{
		// 设置默认字幕位置
		fontPos = CSize(left + width / 2, top + height * 7 / 8);
	}

	font.CreateFont(fontSize.cy, fontSize.cx, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, OUT_DEFAULT_PRECIS, PROOF_QUALITY, FF_SWISS, L"宋体");
	SelectObject(hMemDC, font);

	// 通过DC句柄获取DC对象指针
	CDC *pDC = CDC::FromHandle(hMemDC);
	if (pDC == NULL)
		return;

	// 设置文字背景透明
	pDC->SetBkMode(TRANSPARENT);

	// 设置文字颜色
	pDC->SetTextColor(RGB(100, 175, 100));

	// 设置文字居中
	pDC->SetTextAlign(TA_CENTER);

	// 添加文字到指定位置
	pDC->TextOut(fontPos.cx, fontPos.cy, _pSR->GetText());



	GetDIBits(hMemDC, hbm, 0, m_height, hdib, (LPBITMAPINFO)&pbi, DIB_RGB_COLORS);
}

void* CCaptureScreen::CaptureScreenFrame(int left, int top, int width, int height)
{

	if (hbm == NULL || hMemDC == NULL || hScreenDC == NULL) return NULL;

	BitBlt(hMemDC, 0, 0, width, height, hScreenDC, left, top, SRCCOPY);
	
	if(_bShowMouse)
		DrawMouse(left, top);

	//动态分配的内存
	PRGBTRIPLE hdib = (PRGBTRIPLE)malloc(pbi.bmiHeader.biSizeImage);//24位图像大小
	GetDIBits(hMemDC, hbm, 0, m_height, hdib, (LPBITMAPINFO)&pbi, DIB_RGB_COLORS);

	// 添加文字
	AddText(hdib, left, top, width, height);

	return hdib;
}

// 绘制鼠标
void CCaptureScreen::DrawMouse(int left, int top)
{
	/*-------------------------捕获鼠标-------------------------------*/
	{
		POINT xPoint;
		GetCursorPos(&xPoint);
		
		xPoint.x -= left;
		xPoint.y -= top;

		BOOL ret;
		ret = GetIconInfo(_hcur, &_iconinfo);
		if (ret) {
			xPoint.x -= _iconinfo.xHotspot;
			xPoint.y -= _iconinfo.yHotspot;

			if (_iconinfo.hbmMask) DeleteObject(_iconinfo.hbmMask);
			if (_iconinfo.hbmColor) DeleteObject(_iconinfo.hbmColor);
		}
		/*画鼠标*/
		::DrawIcon(hMemDC, xPoint.x, xPoint.y, _hcur);
	}
}

//
// 获取窗体鼠标光标
//
HCURSOR CCaptureScreen::FetchCursorHandle()
{
	if (m_hSavedCursor == NULL)
	{
		m_hSavedCursor = GetCursor();
	}
	return m_hSavedCursor;
}