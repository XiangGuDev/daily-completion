#include "StdAfx.h"
#include "TitleTip.h"

CTitleTip::CTitleTip()
{
	_bShow = false;
	_pParent = NULL;
	_clrBack = GetSysColor(COLOR_INFOBK);
	_clrBorder = RGB(0, 0, 0);
	_pFont = NULL;
	_nTextMode = 0;
	_bClrText = false;
}

CTitleTip::~CTitleTip()
{
}

BEGIN_MESSAGE_MAP(CTitleTip, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

bool CTitleTip::Create(CWnd* pParentWnd, CFont* pFont)
{
	_pFont = pFont;
	_pParent = pParentWnd;

	return !!CWnd::CreateEx(WS_EX_NOACTIVATE , WC_STATIC, NULL,
		WS_POPUP, CRect(0, 0, 0, 0), pParentWnd, 0, NULL);
}

void CTitleTip::SetColor(COLORREF clrBack, COLORREF clrBorder)
{
	_clrBorder = clrBorder;
	_clrBack = clrBack;
}

void CTitleTip::SetTextMode(int nTextMode)
{
	_nTextMode = nTextMode;
}

void CTitleTip::SetTipText(const CString& strTipText)
{
	_strTipText = strTipText;
}

void CTitleTip::SetClrText(bool bClrText)
{
	_bClrText = bClrText;
}

void CTitleTip::SetRect(CRect rcMonitor, CRect rcWnd)
{
	_rcWnd = rcWnd;
	_rcMonitor = rcMonitor;
}

void CTitleTip::RelayEvent(PMSG pMsg)
{
	if (NULL != _pParent)
	{
		CPoint msPt;
		GetCursorPos(&msPt);
		_pParent->ScreenToClient(&msPt);

		if (_rcMonitor.PtInRect(msPt))
		{
			if (_bShow)
				return;

			CRect rcShow = _rcWnd;
			_pParent->ClientToScreen(&rcShow);
			SetWindowPos(NULL, rcShow.left, rcShow.top, rcShow.Width(), rcShow.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
			_bShow = true;
		}
		else
		{
			_bShow = false;
			ShowWindow(SW_HIDE);
			EnableWindow(TRUE);
		}
	}
}

void CTitleTip::Reset()
{
	_bShow = true;
	ShowWindow(SW_HIDE);
	_rcMonitor.SetRectEmpty();
	_rcWnd.SetRectEmpty();
}

void CTitleTip::OnPaint()
{
	CPaintDC dc(this);

	CMemoryDC memDc(NULL, CRect());
	memDc.Attach(dc.GetSafeHdc());

	CRect rc;
	GetClientRect(&rc);
	CBrush brush(_clrBack);
	dc.FillRect(&rc, &brush);

	CBrush border(_clrBorder);
	dc.FrameRect(&rc, &border);

	dc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = dc.SelectObject(_pFont);

	if (_bClrText)
		memDc.DrawColorText(_strTipText, rc, _nTextMode | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
	else
		memDc.DrawText(_strTipText, &rc, _nTextMode | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

	dc.SelectObject(pOldFont);
	memDc.Detach();
}

BOOL CTitleTip::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}