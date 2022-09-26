//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0
//    @file           GdiTools.H
//    @author         KG
//    @date           2015/3/10 13:50
//    @brief          GDI工具集
//*****************************************************

#pragma once

//#include <tmschema.h>
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")

/**
*  @class    内存DC
*
*  @brief
*/
class CMemoryDC : public CDC
{
public:
	CMemoryDC(CDC* pDC, RECT& rcPaint) : _pOriginal(pDC), _hBmpOld(NULL)
	{
		if (NULL == _pOriginal)
			return;

		_rcPaint = rcPaint;
		CreateCompatibleDC(_pOriginal);

		_bmpTemp.CreateCompatibleBitmap(_pOriginal, _rcPaint.right - _rcPaint.left,
			_rcPaint.bottom - _rcPaint.top);

		_hBmpOld = (HBITMAP)SelectObject(_bmpTemp);

		SetViewportOrg(-_rcPaint.left, -_rcPaint.top);
	}

	~CMemoryDC()
	{
		if (NULL == _pOriginal)
			return;

		::BitBlt(_pOriginal->GetSafeHdc(), _rcPaint.left,
			_rcPaint.top, _rcPaint.right - _rcPaint.left, _rcPaint.bottom - _rcPaint.top, m_hDC, _rcPaint.left, _rcPaint.top, SRCCOPY);

		SelectObject(_hBmpOld);
	}

	void GradientVert(const CRect& rcRect, COLORREF rgbTop, COLORREF rgbBottom)
	{
		TRIVERTEX tri[2] = { 0 };

		tri[0].Alpha = 0;
		tri[0].Blue = GetBValue(rgbTop) << 8;
		tri[0].Green = GetGValue(rgbTop) << 8;
		tri[0].Red = GetRValue(rgbTop) << 8;
		tri[0].x = rcRect.left;
		tri[0].y = rcRect.top;
		tri[1].Alpha = 0;
		tri[1].Blue = GetBValue(rgbBottom) << 8;
		tri[1].Green = GetGValue(rgbBottom) << 8;
		tri[1].Red = GetRValue(rgbBottom) << 8;
		tri[1].x = rcRect.right;
		tri[1].y = rcRect.bottom;

		GRADIENT_RECT grdRect;
		grdRect.LowerRight = 0;
		grdRect.UpperLeft = 1;

		GradientFill(tri, 2, &grdRect, 1, GRADIENT_FILL_RECT_V);
	}

	void DotHLine(LONG x, LONG y, LONG w, COLORREF cr)
	{
		for (; w>0; w -= 2, x += 2)
			SetPixel(x, y, cr);
	}

	void DotVLine(LONG x, LONG y, LONG w, COLORREF cr)
	{
		for (; w>0; w -= 2, y += 2)
			SetPixel(x, y, cr);
	}

	// Hello<c #124515>my</c>World
	void DrawColorText(CString strText, RECT& rect, UINT nFormat)
	{
		RECT rcOld = rect;
		int nWidthCount = 0;

		do
		{
			int nFindStPos = strText.Find((LPCTSTR)L"<c #");
			int nFindEdPos = strText.Find((LPCTSTR)L"/c>");
			if (nFindStPos != -1 && nFindEdPos != -1 && nFindStPos == 0)
			{
				CString strTmpText = strText.Mid(nFindStPos + 11, nFindEdPos - 12);
				CString strTextCol = strText.Mid(nFindStPos + 4, 6);
				
				DWORD clrColor = _ttoi(strTextCol);
				COLORREF clOld = GetTextColor();
				SetTextColor(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));

				CRect rcCalc = rect;
				DrawText(strTmpText, &rcCalc, nFormat | DT_CALCRECT);

				if (!(nFormat & DT_CALCRECT))
					DrawText(strTmpText, &rect, nFormat);
				
				nWidthCount += rcCalc.Width();

				SetTextColor(clOld);

				strText = strText.Mid(nFindEdPos+3);
				rect.left += rcCalc.Width();
			}
			else if (nFindStPos != -1 && nFindEdPos != -1 && nFindStPos != 0)
			{
				CRect rcCalc = rect;
				DrawText(strText.Left(nFindStPos), &rcCalc, nFormat|DT_CALCRECT);

				if (!(nFormat & DT_CALCRECT))
					DrawText(strText.Left(nFindStPos), &rect, nFormat);
	
				nWidthCount += rcCalc.Width();
				
				strText = strText.Mid(nFindStPos);
				rect.left += rcCalc.Width();
			}
			else
			{
				if (!(nFormat & DT_CALCRECT))
				{
					DrawText(strText, &rect, nFormat);
				}
				else
				{
					CRect rcCalc = rect;
					DrawText(strText, &rcCalc, nFormat | DT_CALCRECT);
					nWidthCount += rcCalc.Width();
				}

				strText = L"";
			}
		} while (!strText.IsEmpty());

		if (nFormat & DT_CALCRECT)
		{
			rect.left = rcOld.left;
			rect.right = rcOld.left + nWidthCount;
		}
		else
		{
			rect = rcOld;
		}
	}

private:
	CDC* _pOriginal;	///< 原绘图DC
	RECT _rcPaint;		///< 绘图区域
	CBitmap _bmpTemp;	///< 临时位图
	HBITMAP _hBmpOld;	///< 原DC旧位图
};

/**
*  @class    主题绘制类
*
*  @brief
*/
class CTheme
{
public:
	// Data members
	HTHEME m_hTheme;
	static int m_nIsThemingSupported;

	// Constructor
	CTheme(HTHEME hTheme = NULL) : m_hTheme(hTheme)
	{
		IsThemingSupported();
	}

	// Operators and helpers
	bool IsThemeNull() const
	{
		return (m_hTheme == NULL);
	}

	CTheme& operator =(HTHEME hTheme)
	{
		m_hTheme = hTheme;
		return *this;
	}

	operator HTHEME() const
	{
		return m_hTheme;
	}

	void Attach(HTHEME hTheme)
	{
		m_hTheme = hTheme;
	}

	HTHEME Detach()
	{
		HTHEME hTheme = m_hTheme;
		m_hTheme = NULL;
		return hTheme;
	}

	// Theme support helper
	static bool IsThemingSupported()
	{
		if (m_nIsThemingSupported == -1)
		{
			HMODULE hThemeDLL = ::LoadLibrary(_T("uxtheme.dll"));
			m_nIsThemingSupported = (hThemeDLL != NULL) ? 1 : 0;
			if (hThemeDLL != NULL)
				::FreeLibrary(hThemeDLL);
		}

		ATLASSERT(m_nIsThemingSupported != -1);
		return (m_nIsThemingSupported == 1);
	}

	// Operations and theme properties
	HTHEME OpenThemeData(HWND hWnd, LPCWSTR pszClassList)
	{
		if (!IsThemingSupported())
			return NULL;

		ATLASSERT(m_hTheme == NULL);
		m_hTheme = ::OpenThemeData(hWnd, pszClassList);
		return m_hTheme;
	}

	HRESULT CloseThemeData()
	{
		HRESULT hRet = S_FALSE;
		if (m_hTheme != NULL)
		{
			hRet = ::CloseThemeData(m_hTheme);
			if (SUCCEEDED(hRet))
				m_hTheme = NULL;
		}
		return hRet;
	}

	HRESULT DrawThemeBackground(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, LPCRECT pClipRect = NULL)
	{
		ATLASSERT(m_hTheme != NULL);
		return ::DrawThemeBackground(m_hTheme, hDC, nPartID, nStateID, pRect, pClipRect);
	}

	HRESULT DrawThemeBackgroundEx(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, const DTBGOPTS* pOptions = NULL)
	{
		ATLASSERT(m_hTheme != NULL);
		return ::DrawThemeBackgroundEx(m_hTheme, hDC, nPartID, nStateID, pRect, pOptions);
	}

	HRESULT DrawThemeText(HDC hDC, int nPartID, int nStateID, LPCWSTR pszText, int nCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
	{
		ATLASSERT(m_hTheme != NULL);
		return ::DrawThemeText(m_hTheme, hDC, nPartID, nStateID, pszText, nCharCount, dwTextFlags, dwTextFlags2, pRect);
	}

	HRESULT GetThemeBackgroundContentRect(HDC hDC, int nPartID, int nStateID, LPCRECT pBoundingRect, LPRECT pContentRect) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeBackgroundContentRect(m_hTheme, hDC, nPartID, nStateID, pBoundingRect, pContentRect);
	}

	HRESULT GetThemeBackgroundExtent(HDC hDC, int nPartID, int nStateID, LPCRECT pContentRect, LPRECT pExtentRect) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeBackgroundExtent(m_hTheme, hDC, nPartID, nStateID, pContentRect, pExtentRect);
	}

	HRESULT GetThemePartSize(HDC hDC, int nPartID, int nStateID, LPRECT pRect, enum THEMESIZE eSize, LPSIZE pSize) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemePartSize(m_hTheme, hDC, nPartID, nStateID, pRect, eSize, pSize);
	}

	HRESULT GetThemeTextExtent(HDC hDC, int nPartID, int nStateID, LPCWSTR pszText, int nCharCount, DWORD dwTextFlags, LPCRECT  pBoundingRect, LPRECT pExtentRect) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeTextExtent(m_hTheme, hDC, nPartID, nStateID, pszText, nCharCount, dwTextFlags, pBoundingRect, pExtentRect);
	}

	HRESULT GetThemeTextMetrics(HDC hDC, int nPartID, int nStateID, PTEXTMETRICW pTextMetric) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeTextMetrics(m_hTheme, hDC, nPartID, nStateID, pTextMetric);
	}

	HRESULT GetThemeBackgroundRegion(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, HRGN* pRegion) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeBackgroundRegion(m_hTheme, hDC, nPartID, nStateID, pRect, pRegion);
	}

	HRESULT HitTestThemeBackground(HDC hDC, int nPartID, int nStateID, DWORD dwOptions, LPCRECT pRect, HRGN hrgn, POINT ptTest, WORD* pwHitTestCode) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::HitTestThemeBackground(m_hTheme, hDC, nPartID, nStateID, dwOptions, pRect, hrgn, ptTest, pwHitTestCode);
	}

	HRESULT DrawThemeEdge(HDC hDC, int nPartID, int nStateID, LPCRECT pDestRect, UINT uEdge, UINT uFlags, LPRECT pContentRect = NULL)
	{
		ATLASSERT(m_hTheme != NULL);
		return ::DrawThemeEdge(m_hTheme, hDC, nPartID, nStateID, pDestRect, uEdge, uFlags, pContentRect);
	}

	HRESULT DrawThemeIcon(HDC hDC, int nPartID, int nStateID, LPCRECT pRect, HIMAGELIST himl, int nImageIndex)
	{
		ATLASSERT(m_hTheme != NULL);
		return ::DrawThemeIcon(m_hTheme, hDC, nPartID, nStateID, pRect, himl, nImageIndex);
	}

	BOOL IsThemePartDefined(int nPartID, int nStateID) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::IsThemePartDefined(m_hTheme, nPartID, nStateID);
	}

	BOOL IsThemeBackgroundPartiallyTransparent(int nPartID, int nStateID) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::IsThemeBackgroundPartiallyTransparent(m_hTheme, nPartID, nStateID);
	}

	HRESULT GetThemeColor(int nPartID, int nStateID, int nPropID, COLORREF* pColor) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeColor(m_hTheme, nPartID, nStateID, nPropID, pColor);
	}

	HRESULT GetThemeMetric(HDC hDC, int nPartID, int nStateID, int nPropID, int* pnVal) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeMetric(m_hTheme, hDC, nPartID, nStateID, nPropID, pnVal);
	}

	HRESULT GetThemeString(int nPartID, int nStateID, int nPropID, LPWSTR pszBuff, int cchMaxBuffChars) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeString(m_hTheme, nPartID, nStateID, nPropID, pszBuff, cchMaxBuffChars);
	}

	HRESULT GetThemeBool(int nPartID, int nStateID, int nPropID, BOOL* pfVal) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeBool(m_hTheme, nPartID, nStateID, nPropID, pfVal);
	}

	HRESULT GetThemeInt(int nPartID, int nStateID, int nPropID, int* pnVal) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeInt(m_hTheme, nPartID, nStateID, nPropID, pnVal);
	}

	HRESULT GetThemeEnumValue(int nPartID, int nStateID, int nPropID, int* pnVal) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeEnumValue(m_hTheme, nPartID, nStateID, nPropID, pnVal);
	}

	HRESULT GetThemePosition(int nPartID, int nStateID, int nPropID, LPPOINT pPoint) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemePosition(m_hTheme, nPartID, nStateID, nPropID, pPoint);
	}

	HRESULT GetThemeFont(HDC hDC, int nPartID, int nStateID, int nPropID, LOGFONTW* pFont) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeFont(m_hTheme, hDC, nPartID, nStateID, nPropID, pFont);
	}

	HRESULT GetThemeRect(int nPartID, int nStateID, int nPropID, LPRECT pRect) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeRect(m_hTheme, nPartID, nStateID, nPropID, pRect);
	}

	HRESULT GetThemeMargins(HDC hDC, int nPartID, int nStateID, int nPropID, LPRECT pRect, PMARGINS pMargins) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeMargins(m_hTheme, hDC, nPartID, nStateID, nPropID, pRect, pMargins);
	}

	HRESULT GetThemeIntList(int nPartID, int nStateID, int nPropID, INTLIST* pIntList) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeIntList(m_hTheme, nPartID, nStateID, nPropID, pIntList);
	}

	HRESULT GetThemePropertyOrigin(int nPartID, int nStateID, int nPropID, enum PROPERTYORIGIN* pOrigin) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemePropertyOrigin(m_hTheme, nPartID, nStateID, nPropID, pOrigin);
	}

	HRESULT GetThemeFilename(int nPartID, int nStateID, int nPropID, LPWSTR pszThemeFileName, int cchMaxBuffChars) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeFilename(m_hTheme, nPartID, nStateID, nPropID, pszThemeFileName, cchMaxBuffChars);
	}

	COLORREF GetThemeSysColor(int nColorID) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeSysColor(m_hTheme, nColorID);
	}

	HBRUSH GetThemeSysColorBrush(int nColorID) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeSysColorBrush(m_hTheme, nColorID);
	}

	int GetThemeSysSize(int nSizeID) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeSysSize(m_hTheme, nSizeID);
	}

	BOOL GetThemeSysBool(int nBoolID) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeSysBool(m_hTheme, nBoolID);
	}

	HRESULT GetThemeSysFont(int nFontID, LOGFONTW* plf) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeSysFont(m_hTheme, nFontID, plf);
	}

	HRESULT GetThemeSysString(int nStringID, LPWSTR pszStringBuff, int cchMaxStringChars) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeSysString(m_hTheme, nStringID, pszStringBuff, cchMaxStringChars);
	}

	HRESULT GetThemeSysInt(int nIntID, int* pnValue) const
	{
		ATLASSERT(m_hTheme != NULL);
		return ::GetThemeSysInt(m_hTheme, nIntID, pnValue);
	}
};

__declspec(selectany) int CTheme::m_nIsThemingSupported = -1;