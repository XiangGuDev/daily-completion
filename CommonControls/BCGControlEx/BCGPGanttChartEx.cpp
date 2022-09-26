#include "stdafx.h"
#include "BCGPGanttChartEx.h"
#include "BCGPGanttViewEx.h"

IMPLEMENT_DYNAMIC(CBCGPGanttRendererEx, CBCGPGanttRenderer)

CBCGPGanttRendererEx::CBCGPGanttRendererEx()
	:CBCGPGanttRenderer()
{

}

CBCGPGanttRendererEx::~CBCGPGanttRendererEx()
{

}

void CBCGPGanttRendererEx::DrawBar(CBCGPGanttDrawContext& ctxDraw)
{
	if (ctxDraw.m_Item.IsMileStone())
	{
		DrawMileStoneItem(ctxDraw);
	}
	else if (ctxDraw.m_Item.IsGroupItem())
	{
		DrawGroupItem(ctxDraw);
	}
	else
	{
		DrawSimpleItem(ctxDraw);
		DrawItemName(ctxDraw);
	}
}

void CBCGPGanttRendererEx::DrawSelection(CBCGPGanttDrawContext& ctxDraw)
{
	//return __super::DrawSelection(ctxDraw);
	int k = ctxDraw.m_Item.IsMileStone() ? 4 : 3;
	CRect rectSel = ctxDraw.m_rectBar;
	rectSel.left = ctxDraw.m_rectClip.left;
	rectSel.right = ctxDraw.m_rectClip.right;
	k *= 2;
	rectSel.InflateRect(k, k, k, k);

	int d = min(10, rectSel.Height());
	CPoint pt(d, d);

	CRect rectOutput;
	rectOutput.IntersectRect(ctxDraw.m_rectClip, rectSel);
	if (rectOutput.IsRectEmpty())
	{
		return;
	}

	CBCGPAlphaDC dc(ctxDraw.m_DC, rectOutput, 0.37f);
	CBCGPPenSelector pen(dc, ctxDraw.m_clrBorder);
	CBCGPBrushSelector brush(dc, ctxDraw.m_clrFill);
	if (ctxDraw.m_Item.IsMileStone())
	{
		POINT ptCenter = rectSel.CenterPoint();
		d = rectSel.Height() / 2 - 1;
		POINT pts[4];

		pts[0] = ptCenter;
		pts[1] = ptCenter;
		pts[2] = ptCenter;
		pts[3] = ptCenter;

		pts[0].x -= d;
		pts[1].y -= d;
		pts[2].x += d;
		pts[3].y += d;
		dc.Polygon(pts, 4);
	}
	else
	{
		dc.RoundRect(&rectSel, pt);
	}
}

void CBCGPGanttRendererEx::DrawItemName(CBCGPGanttDrawContext& ctxDraw)
{
	int nLeftMargin = 10;
	CRect rect = ctxDraw.m_rectBar;
	auto textSize = ctxDraw.m_DC.GetTextExtent(ctxDraw.m_Item.GetName());
	CRect crText = CRect(rect.right + nLeftMargin,
		rect.top, rect.right + textSize.cx + nLeftMargin, rect.bottom);

	auto curFont = ctxDraw.m_DC.GetCurrentFont();
	LOGFONT curLfont;
	curFont->GetLogFont(&curLfont);
	_tcscpy_s(curLfont.lfFaceName, _T("Microsoft YaHei UI"));
	CFont font;
	font.CreateFontIndirect(&curLfont);

	//将字体选进DC中
	CFont* pOldFont = ctxDraw.m_DC.SelectObject(&font);
	ctxDraw.m_DC.DrawText(ctxDraw.m_Item.GetName(), crText, DT_LEFT | DT_SINGLELINE | DT_BOTTOM);
	ctxDraw.m_DC.SelectObject(pOldFont);
}

////////////////////////////////////////////////////////////////////////////\
// CBCGPGanttChart::UPDATE_INFO

struct CBCGPGanttChartEx::UPDATE_INFO
{
	int                 nHScrollDelta;   // Differs from 0 if horizontal scrolling occurred
	int                 nVScrollDelta;   // Differs from 0 if horizontal scrolling occurred

	BOOL                bItemsLayoutChanged; // If TRUE, scroll deltas are ignored.

	BOOL                bLayoutChanged; // Occurs when control has been resized, header positions changed etc.

	BOOL                bAppearanceChanged; // Fonts, Colors, Grid lines, Visual style etc.

	BOOL                bConnectionsChanged;

	UPDATE_INFO()
		: nHScrollDelta(0)
		, nVScrollDelta(0)
		, bItemsLayoutChanged(TRUE)
		, bLayoutChanged(TRUE)
		, bAppearanceChanged(TRUE)
		, bConnectionsChanged(TRUE)
	{
	}

	bool NeedsUpdate() const
	{
		return  nHScrollDelta != 0 ||
			nVScrollDelta != 0 ||
			bItemsLayoutChanged ||
			bLayoutChanged ||
			bAppearanceChanged ||
			bConnectionsChanged;
	}

	void Overwrite(const UPDATE_INFO& info)
	{
		nHScrollDelta += info.nHScrollDelta;
		nVScrollDelta += info.nVScrollDelta;
		bItemsLayoutChanged = bItemsLayoutChanged || info.bItemsLayoutChanged;
		bLayoutChanged = bLayoutChanged || info.bLayoutChanged;
		bAppearanceChanged = bAppearanceChanged || info.bAppearanceChanged;
		bConnectionsChanged = bConnectionsChanged || info.bConnectionsChanged;
	}

	void Reset()
	{
		nHScrollDelta = 0;
		nVScrollDelta = 0;
		bItemsLayoutChanged = FALSE;
		bLayoutChanged = FALSE;
		bAppearanceChanged = FALSE;
		bConnectionsChanged = FALSE;
	}
};

////////////////////////////////////////////////////////////////////////////\
// CBCGPGanttChart

IMPLEMENT_DYNCREATE(CBCGPGanttChartEx, CBCGPGanttChart)
BEGIN_MESSAGE_MAP(CBCGPGanttChartEx, CBCGPGanttChart)
	//{{AFX_MSG_MAP(CBCGPGanttChart)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_SETTINGCHANGE()
	ON_WM_CANCELMODE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipText)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_REGISTERED_MESSAGE(BCGM_UPDATETOOLTIPS, OnUpdateToolTips)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_STORAGE_CHANGED, OnStorageChanged)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONNECTION_ADDED, OnStorageConnectionAdded)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONNECTION_REMOVED, OnStorageConnectionRemoved)
	ON_REGISTERED_MESSAGE(BCGM_ONGETGESTURECONFIG, OnGetGestureConfig)
END_MESSAGE_MAP()

CWnd* CBCGPGanttChartEx::GetNotificationWindow() const
{
	CBCGPGanttControl* pControl = GetGanttControl();

	if (pControl != NULL)
	{
		return pControl;
	}

	return GetParent();
}

CBCGPGanttRenderer* CBCGPGanttChartEx::QueryRenderer()
{
	return new CBCGPGanttRendererEx();
}

static BOOL IsTimeNull(const COleDateTime& date)
{
	SYSTEMTIME st;
	date.GetAsSystemTime(st);
	return st.wHour == 0 && st.wMinute == 0 && st.wSecond == 0;
}

static CString FormatDate(COleDateTime datetime, const CString& sFormat, LCID locale = LOCALE_USER_DEFAULT, DWORD dwFlags = 0)
{
	TCHAR buf[256] = { 0 };

	SYSTEMTIME st;
	datetime.GetAsSystemTime(st);

	LPCTSTR pszFormat = sFormat;
	if (sFormat.CompareNoCase(_T("Long")) == 0)
	{
		pszFormat = NULL;
		dwFlags |= DATE_LONGDATE;
	}
	else if (sFormat.CompareNoCase(_T("Short")) == 0)
	{
		pszFormat = NULL;
		dwFlags |= DATE_SHORTDATE;
	}

	::GetDateFormat(locale, dwFlags, &st, pszFormat, buf, 256);
	return CString(buf);
}

static CString FormatTime(COleDateTime datetime, DWORD dwTimeFormat, LCID locale = LOCALE_USER_DEFAULT, DWORD dwFlags = 0)
{
	TCHAR buf[256] = { 0 };

	SYSTEMTIME st;
	datetime.GetAsSystemTime(st);

	switch (dwTimeFormat)
	{
	case TIMEFORMAT_NONE:
		return CString();
	case TIMEFORMAT_MINUTES_ONLY:
		::GetTimeFormat(locale, dwFlags, &st, _T("mm"), buf, 256);
		return CString(buf);
		break;
	case TIMEFORMAT_HOURS_AND_MINUTES:
		dwFlags |= TIME_NOSECONDS;
		break;
	case TIMEFORMAT_HOURS_ONLY:
		dwFlags |= TIME_NOMINUTESORSECONDS;
		break;
	default:
		TRACE0("Invalid flags\n");
		ASSERT(0);
		return CString();
	}

	::GetTimeFormat(locale, dwFlags, &st, NULL, buf, 256);
	return CString(buf);
}

BOOL CBCGPGanttChartEx::QueryToolTipText(const CBCGPGanttItem* pItem, CString& strText, CString& strDescription) const
{
	if (pItem == NULL)
	{
		return FALSE;
	}

	int nItemIndex = IndexOfItem(pItem);
	auto pSplitterWndEx = GetOwner();
	CBCGPGanttControl* m_pWnd = DYNAMIC_DOWNCAST(CBCGPGanttControl, pSplitterWndEx->GetOwner());
	m_pWnd->GetGrid()->GetRow(nItemIndex);

	CString strFormat;

	if (!pItem->GetName().IsEmpty())
	{
		strFormat = _T(" (%d%%)");
	}
	else
	{
		strFormat = _T("%d%%");
	}

	strText.Format(strFormat, (int)(pItem->GetProgress() * 100.0f + 0.5f));
	strText = pItem->GetName() + strText;

	COleDateTime tmStart = pItem->GetStartTime();
	COleDateTime tmFinish = pItem->GetFinishTime();

	CString strStart, strFinish;
	if (IsTimeNull(tmStart) && IsTimeNull(tmFinish))
	{
		strStart = FormatDate(tmStart, _T("Short"));
		strFinish = FormatDate(tmFinish, _T("Short"));
	}
	else
	{
		strStart = FormatDate(tmStart, _T("Short")) + _T(" ") + FormatTime(tmStart, TIMEFORMAT_HOURS_AND_MINUTES);
		strFinish = FormatDate(tmFinish, _T("Short")) + _T(" ") + FormatTime(tmFinish, TIMEFORMAT_HOURS_AND_MINUTES);
	}

	if (pItem->IsMileStone())
	{
		strText = strStart;
	}
	else
	{
		strDescription.Format(
			_T("%s - %s"),
			(LPCTSTR)strStart,
			(LPCTSTR)strFinish);
	}

	return TRUE;
}

void CBCGPGanttChartEx::MoveSelectedItems(COleDateTimeSpan dtOffset)
{
	BOOL bAltKeyPressed = ::GetAsyncKeyState(VK_MENU) & 0x8000;
	for (int i = m_pItems->GetCount() - 1; i >= 0; --i)
	{
		CBCGPGanttItem* pItem = m_pItems->GetItem(i);
		if (pItem->IsSelected())
		{
// 			COleDateTime dtStart = pItem->GetStartTime();
// 			COleDateTime dtFinish = pItem->GetFinishTime();
			BCGP_GANTT_ITEM_DRAGDROP dragdrop;
			dragdrop.pItem = pItem;
			CalculateMovePosition(dragdrop, dtOffset, !bAltKeyPressed);

			BOOL bCanMove = TRUE;
			CWnd* pWnd = GetNotificationWindow();
			if (pWnd != NULL)
			{
				pWnd->SendMessage(BCGM_GANTT_CHART_ITEM_MOVING, (WPARAM)0, (LPARAM)&dragdrop);
			}
// 			if (bCanMove)
// 			{
// 				dragdrop.pItem->SetInterval(dragdrop.dtNewStartTime, dragdrop.dtNewFinishTime);
// 			}
		}
	}

	InvalidateChart();
}

void CBCGPGanttChartEx::ResizeSelectedItems(COleDateTimeSpan dtOffset)
{
	BOOL bAltKeyPressed = ::GetAsyncKeyState(VK_MENU) & 0x8000;
	for (int i = m_pItems->GetCount() - 1; i >= 0; --i)
	{
		CBCGPGanttItem* pItem = m_pItems->GetItem(i);
		if (pItem->IsSelected())
		{
// 			COleDateTime dtStart = pItem->GetStartTime();
// 			COleDateTime dtFinish = pItem->GetFinishTime();
			BCGP_GANTT_ITEM_DRAGDROP dragdrop;
			dragdrop.pItem = pItem;
			CalculateItemsResize(dragdrop, dtOffset, !bAltKeyPressed);
			//dragdrop.pItem->SetInterval(dragdrop.dtNewStartTime, dragdrop.dtNewFinishTime);BCGM_GANTT_CHART_ITEM_RESIZE
			CWnd* pWnd = GetNotificationWindow();
			if (pWnd == nullptr)
				return;

			CWnd* parent = pWnd->GetParent();
			if (parent != NULL)
			{
				parent->SendMessage(BCGM_GANTT_CHART_ITEM_RESIZED, (WPARAM)0, (LPARAM)&dragdrop);
			}
		}
	}

	InvalidateChart();
}

CBCGPGanttChartEx::CBCGPGanttChartEx()
	:CBCGPGanttChart(), _bShowCustomTooltip(false)
{
	
}


CBCGPGanttChartEx::~CBCGPGanttChartEx()
{
}

void CBCGPGanttChartEx::CustomTooltip(bool bCustom)
{
	_bShowCustomTooltip = bCustom;
}

void CBCGPGanttChartEx::SetToolTipTitle(const CBCGPGanttItem* pItem, CString strTitle)
{
	_strTitle = strTitle;
}

void CBCGPGanttChartEx::SetToolTipContex(const CBCGPGanttItem* pItem, CString strContext)
{
	_strContext = strContext;
}

void CBCGPGanttChartEx::SetChartColor(COLORREF cf)
{
	m_UserColors.clrBarFill = cf;
	m_UserColors.clrConnectorLines = cf;
}

BOOL CBCGPGanttChartEx::OnToolTipText(UINT id, NMHDR* pNMH, LRESULT* pResult)
{
	if (_bShowCustomTooltip)
	{
		LPNMTTDISPINFO pTTDispInfo = (LPNMTTDISPINFO)pNMH;
		ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

		CBCGPToolTipCtrl* pToolTip = DYNAMIC_DOWNCAST(CBCGPToolTipCtrl, m_pToolTip);

		if (pToolTip != NULL)
		{
			ASSERT_VALID(pToolTip);
			pToolTip->SetDescription(_strContext);
		}

		pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR)_strTitle);
		return TRUE;
	}
	else
	{
		return __super::OnToolTipText(id, pNMH, pResult);
	}
}

BOOL CBCGPGanttChartEx::DoChartClick(UINT nFlags, CPoint point)
{
	return TRUE;
}