#include "StdAfx.h"
#include "ColorStatic.h"

BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CColorStatic::CColorStatic() : m_colorBg(RGB(255,255,255))
{
}


CColorStatic::~CColorStatic()
{
}

void CColorStatic::SetBgColor(COLORREF cr)
{
	m_colorBg = cr;
}

BOOL CColorStatic::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CColorStatic::OnPaint()
{
	RECT rt;
	CPaintDC dc(this);
	CBrush brush(m_colorBg);

	GetClientRect(&rt);
	dc.FillRect(&rt, &brush);

}
