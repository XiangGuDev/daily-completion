#include "stdafx.h"

#include <math.h>

#include "ThumbNailControl.h"

#define RGBTHUMBCTRLBKGD RGB( 250, 250, 250 )
#define RGBTHUMBCTRLBORDER RGB( 0, 0, 0 )

#define CTHUMBNAILCONTROL_CLASSNAME _T("CThumbNailControl")

CBrush  CThumbNailControl::m_bkBrush;
CThumbNailControl::CThumbNailControl()
{
	if (!RegisterWindowClass())
		return;

	m_arPtrData.clear();

	m_iCol = 0;
	m_iRow = 0;

	m_iStartX = 0;
	m_iStartY = 0;

	m_iThumbWidth = 0;
	m_iThumbHeight = 0;
}

CThumbNailControl::~CThumbNailControl()
{
	for (auto& e : m_arPtrData)
	{
		if (nullptr != e)
		{
			delete e;
			e = nullptr;
		}
	}
	m_arPtrData.clear();
}


BEGIN_MESSAGE_MAP(CThumbNailControl, CWnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()

END_MESSAGE_MAP()

void CThumbNailControl::InitializeVariables(CWnd* pWnd, int cX, int cY)
{
	m_pWnd = pWnd;
	m_arPtrData.clear();

	m_iThumbWidth = cX;
	m_iThumbHeight = cY;

	if (::IsWindow(m_hWnd))
	{
		CRect rect;
		GetClientRect(&rect);

		m_iCol = (int)floor(((float)rect.Width()) / (m_iThumbWidth + DEFAULT_SEPERATOR));
	}

}

void CThumbNailControl::Clear()
{
	for (auto& e : m_arPtrData)
	{
		if (nullptr != e)
		{
			//Delete(e);
			delete e;
			e = nullptr;
		}
	}
	m_arPtrData.clear();

	m_iStartY = 0;

	RecalButtonPos();
	RecalScrollBars();
}

int CThumbNailControl::GetSize()
{
	return (int)m_arPtrData.size();
}

void CThumbNailControl::RecalButtonPos()
{
	CRect rect;
	GetClientRect(&rect);

	int nX = 0;
	int nY = 0;
	m_iRow = 0;

	for (size_t n = 0; n < m_arPtrData.size(); ++n)
	{
		if (nX == 0)
			m_iRow++;

		CThumbnailButton * pBtn = (CThumbnailButton*)m_arPtrData[n];

		ASSERT(AfxIsValidAddress(pBtn, sizeof(CThumbnailButton), TRUE));
		ASSERT(::IsWindow(pBtn->m_hWnd));

		pBtn->SetWindowPos(NULL,
			m_iStartX + DEFAULT_SEPERATOR + nX,
			m_iStartY + DEFAULT_SEPERATOR + nY,
			0, 0, SWP_NOSIZE | SWP_NOZORDER);

		nX += m_iThumbWidth + DEFAULT_SEPERATOR;

		if ((rect.Width() - nX) < 0.8 * (m_iThumbWidth + DEFAULT_SEPERATOR))
		{
			nX = 0;
			nY += m_iThumbHeight + DEFAULT_SEPERATOR;
		}

	}

}


BOOL CThumbNailControl::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if (!(::GetClassInfo(hInst, CTHUMBNAILCONTROL_CLASSNAME, &wndcls)))
	{
		m_bkBrush.CreateSolidBrush(RGBTHUMBCTRLBKGD);
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)m_bkBrush.GetSafeHandle();
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = CTHUMBNAILCONTROL_CLASSNAME;
		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

void CThumbNailControl::Add(const CString&  sPath)
{
	ASSERT(::IsWindow(this->m_hWnd));

	// Initialize Variables Not done!
	ASSERT((m_iThumbWidth + m_iThumbHeight) > 0);

	Invalidate(FALSE);

	int iThumbImgWidth = (int)(1 * m_iThumbWidth); // 使用1，图片满绘制，0.75 空出的间隙，下同
	int iThumbImgHeight = (int)(1 * m_iThumbHeight);
	CThumbnailButton * pBtn = new CThumbnailButton(m_pWnd, sPath, iThumbImgWidth, iThumbImgHeight);
	pBtn->Create(_T("CThumbnailButton"),
		_T(""),
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, m_iThumbWidth, m_iThumbHeight), this, 0);

	if (pBtn->IsValid())
	{
		VERIFY(::IsWindow(pBtn->m_hWnd));

		m_arPtrData.emplace_back(pBtn);

		RecalButtonPos();
		RecalScrollBars();
	}
	else
	{
		pBtn->DestroyWindow();
		delete pBtn;
	}

}

void CThumbNailControl::Delete(CThumbnailButton * pBtn)
{
	ASSERT(::IsWindow(this->m_hWnd));
	// Initialize Variables Not done!
	ASSERT((m_iThumbWidth + m_iThumbHeight) > 0);

	Invalidate(FALSE);
	if (pBtn->IsValid())
	{
		VERIFY(::IsWindow(pBtn->m_hWnd));
		auto iter = m_arPtrData.begin();
		for (int i = m_arPtrData.size() - 1; i >= 0; i--)
		{
			if (pBtn == m_arPtrData[i])
			{
				if (m_arPtrData[i] != NULL)
					delete m_arPtrData[i];
				m_arPtrData.erase(iter + i);
				break;
			}

		}
		RecalButtonPos();
		RecalScrollBars();
	}

}

void CThumbNailControl::OnHScroll(UINT nSBCode, UINT /* nPos */, CScrollBar* /* pScrollBar */)
{
	int nScrollPos = GetScrollPos32(SB_HORZ);

	switch (nSBCode)
	{
	case SB_LEFT:
		break;

	case SB_ENDSCROLL:
		RedrawWindow();
		break;

	case SB_LINELEFT:
		SetScrollPos32(SB_HORZ, nScrollPos - 1);
		break;

	case SB_LINERIGHT:
		SetScrollPos32(SB_HORZ, nScrollPos + 1);
		break;

	case SB_PAGELEFT:
		SetScrollPos32(SB_HORZ, nScrollPos - 20);
		break;

	case SB_PAGERIGHT:
		SetScrollPos32(SB_HORZ, nScrollPos + 20);
		break;

	case SB_RIGHT:
		break;

	case SB_THUMBPOSITION:  // Go down...
	case SB_THUMBTRACK:
		SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
		break;

	default:
		break;
	}

	m_iStartX = -GetScrollPos32(SB_HORZ);
	RecalButtonPos();
}

void CThumbNailControl::RecalScrollBars()
{
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);

	int nTotalWidth = m_iThumbWidth * m_iCol + DEFAULT_SEPERATOR * (1 + m_iCol);
	int nTotalHeight = m_iThumbHeight * m_iRow + DEFAULT_SEPERATOR * (2 + m_iRow);

	int iWidDiff = nTotalWidth - rect.Width();
	int iHeiDiff = nTotalHeight - rect.Height();

	if (iWidDiff > DEFAULT_SEPERATOR && int(m_arPtrData.size()) >= m_iCol)
	{
		SCROLLINFO si;
		memset(&si, 0, sizeof(SCROLLINFO));

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = (int)(0.25 * (iWidDiff + 14));
		si.nMin = 0;
		si.nMax = (int)(1.25 * (iWidDiff + 14));

		SetScrollInfo(SB_HORZ, &si, TRUE);

		EnableScrollBarCtrl(SB_HORZ);
		EnableScrollBar(SB_HORZ);
	}

	if (iHeiDiff > DEFAULT_SEPERATOR)
	{
		SCROLLINFO si;
		memset(&si, 0, sizeof(SCROLLINFO));

		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = (int)(0.25 * (iHeiDiff + 14));
		si.nMin = 0;
		si.nMax = (int)(1.25 * (iHeiDiff + 14));

		SetScrollInfo(SB_VERT, &si, TRUE);

		EnableScrollBarCtrl(SB_VERT);
		EnableScrollBar(SB_VERT);
	}
	else
	{
		SCROLLINFO si;
		memset(&si, 0, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nPage = 0;
		si.nMin = 0;
		si.nMax = 0;

		SetScrollInfo(SB_VERT, &si, TRUE);
		ShowScrollBar(SB_VERT, FALSE);
	}
}

BOOL CThumbNailControl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_POS;
	si.nPos = nPos;
	return SetScrollInfo(nBar, &si, bRedraw);
}

int CThumbNailControl::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
			return si.nTrackPos;
	}
	else
	{
		if (GetScrollInfo(nBar, &si, SIF_POS))
			return si.nPos;
	}

	return 0;
}

void CThumbNailControl::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar */)
{
	int nScrollPos = GetScrollPos32(SB_VERT);
	switch (nSBCode)
	{
	case SB_BOTTOM:
		break;

	case SB_ENDSCROLL:
		RedrawWindow();
		break;

	case SB_LINEDOWN:
		SetScrollPos32(SB_VERT, nScrollPos + 1);
		break;

	case SB_LINEUP:
		SetScrollPos32(SB_VERT, nScrollPos - 1);
		break;

	case SB_PAGEDOWN:
		SetScrollPos32(SB_VERT, nScrollPos + 20);
		break;

	case SB_PAGEUP:
		SetScrollPos32(SB_VERT, nScrollPos - 20);
		break;

	case SB_THUMBPOSITION: // Go down..
	case SB_THUMBTRACK:
		SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
		break;

	case SB_TOP:
		break;

	default:
		break;
	}

	m_iStartY = -GetScrollPos32(SB_VERT);
	RecalButtonPos();
}

BOOL CThumbNailControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	int nScrollPos = GetScrollPos32(SB_VERT);
	int nStep = zDelta / WHEEL_DELTA;

	SetScrollPos32(SB_VERT, nScrollPos - nStep * 5);

	m_iStartY = -GetScrollPos32(SB_VERT);
	RecalButtonPos();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}