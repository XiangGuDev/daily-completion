#include "stdafx.h"
#include "BwGanttChart.h"
#include "BCGPDrawManager.h"
#include "BwGanttControl.h"
#include "TrackMouse.h"

#define DATETIMERANGE_YEAR		1
#define DATETIMERANGE_QUARTER	2
#define DATETIMERANGE_MONTH		3
#define DATETIMERANGE_WEEK		4
#define DATETIMERANGE_DAY		5
#define DATETIMERANGE_12HOURS	6
#define DATETIMERANGE_6HOURS	7
#define DATETIMERANGE_4HOURS	8
#define DATETIMERANGE_2HOURS	9
#define DATETIMERANGE_1HOUR		10
#define DATETIMERANGE_30MINUTES 11
#define DATETIMERANGE_20MINUTES 12
#define DATETIMERANGE_15MINUTES 13
#define DATETIMERANGE_10MINUTES 14
#define DATETIMERANGE_5MINUTES  15
#define DATETIMERANGE_1MINUTE	16
#define DATETIMERANGE_30SECONDS 17
#define DATETIMERANGE_1SECOND	18


struct CSpanFormat
{
	double      m_tmMinSpan;
	DWORD       m_dwSpanType;
	LPCTSTR     m_pszDateFormat;
	DWORD       m_dwTimeFormat;
};

BW_GANTT_CHART_HEADER_CELL_INFO::BW_GANTT_CHART_HEADER_CELL_INFO()
	: rectCell(0, 0, 0, 0)
	, rectColumn(0, 0, 0, 0)
	, rectClip(0, 0, 0, 0)
	, pHeaderInfo(NULL)
{}

bool BW_GANTT_CHART_HEADER_CELL_INFO::operator == (const BW_GANTT_CHART_HEADER_CELL_INFO& rhs) const
{
	return (pHeaderInfo == rhs.pHeaderInfo && dtCellLeftMostTime == rhs.dtCellLeftMostTime);
}

bool BW_GANTT_CHART_HEADER_CELL_INFO::operator != (const BW_GANTT_CHART_HEADER_CELL_INFO& rhs) const
{
	return !(*this == rhs);
}

bool BW_GANTT_CHART_HEADER_CELL_INFO::Exists() const
{
	return pHeaderInfo != NULL;
}

void BW_GANTT_CHART_HEADER_CELL_INFO::Reset()
{
	*this = BW_GANTT_CHART_HEADER_CELL_INFO();
}

void DrawGanttHeaderCell(const CBCGPGanttChart*, CDC& dc, const BW_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL /*bHilite*/)
{
	dc.FillRect(cellInfo.rectCell, &globalData.brBarFace);
	dc.Draw3dRect(cellInfo.rectCell, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
}

COLORREF GetGanttHeaderTextColor(BOOL bHilite)
{
	return bHilite ? globalData.clrHotText : globalData.clrBarText;
}

void DrawGanttHeaderText(const CBCGPGanttChart* pGanttChart, CDC& dc, const BW_GANTT_CHART_HEADER_CELL_INFO& cellInfo, const CString& sCellText, BOOL bHilite) 
{
	CRect rcCell = cellInfo.rectCell;
	rcCell.DeflateRect(1, 2, 2, 2); // text padding

	CRect rcVisible;
	rcVisible.IntersectRect(&cellInfo.rectClip, rcCell);

	if (rcVisible.IsRectEmpty())
	{
		return;
	}

	CFont* pNewFont = NULL;
	if (pGanttChart != NULL)
	{
		pNewFont = pGanttChart->GetFont();
	}

	CFont* pOldFont = dc.SelectObject(pNewFont == NULL ? &globalData.fontRegular : pNewFont);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetGanttHeaderTextColor(bHilite));

	ASSERT(cellInfo.pHeaderInfo != NULL);

	DWORD dwFlags = DT_VCENTER | DT_NOPREFIX | cellInfo.pHeaderInfo->dwAlignment;
	dc.DrawText(sCellText, rcCell, dwFlags);

	dc.SelectObject(pOldFont);
}

static const CSpanFormat s_arrSpanFormats[] =
{
	COleDateTimeSpan(0, 0, 15, 0),  DATETIMERANGE_1MINUTE, _T(""),         TIMEFORMAT_HOURS_AND_MINUTES,
	COleDateTimeSpan(0, 0, 15, 0),  DATETIMERANGE_1MINUTE, _T(""),         TIMEFORMAT_MINUTES_ONLY,
	COleDateTimeSpan(0, 0, 30, 0),  DATETIMERANGE_1MINUTE, _T(""),         TIMEFORMAT_HOURS_AND_MINUTES,
	COleDateTimeSpan(0, 0, 30, 0),  DATETIMERANGE_1MINUTE, _T(""),         TIMEFORMAT_MINUTES_ONLY,
	COleDateTimeSpan(0, 1, 0, 0),   DATETIMERANGE_1HOUR,   _T(""),         TIMEFORMAT_HOURS_AND_MINUTES,
	COleDateTimeSpan(0, 1, 0, 0),   DATETIMERANGE_1HOUR,   _T(""),         TIMEFORMAT_HOURS_ONLY,
	COleDateTimeSpan(0, 2, 0, 0),   DATETIMERANGE_1HOUR,   _T(""),         TIMEFORMAT_HOURS_AND_MINUTES,
	COleDateTimeSpan(0, 2, 0, 0),   DATETIMERANGE_1HOUR,   _T(""),         TIMEFORMAT_HOURS_ONLY,
	COleDateTimeSpan(0, 4, 0, 0),   DATETIMERANGE_1HOUR,   _T(""),         TIMEFORMAT_HOURS_AND_MINUTES,
	COleDateTimeSpan(0, 4, 0, 0),   DATETIMERANGE_1HOUR,   _T(""),         TIMEFORMAT_HOURS_ONLY,
	COleDateTimeSpan(1, 0, 0, 0),   DATETIMERANGE_DAY,     _T("Long"),     TIMEFORMAT_NONE,    // long date representation (full date)
	COleDateTimeSpan(1, 0, 0, 0),   DATETIMERANGE_DAY,     _T("Short"),    TIMEFORMAT_NONE,    // short date representation (full date)
	COleDateTimeSpan(1, 0, 0, 0),   DATETIMERANGE_DAY,     _T("d MMM"),    TIMEFORMAT_NONE,    // day and month abbreviation
	COleDateTimeSpan(1, 0, 0, 0),   DATETIMERANGE_DAY,     _T("dd"),       TIMEFORMAT_NONE,    // day only
	COleDateTimeSpan(28, 0, 0, 0),  DATETIMERANGE_MONTH,   _T("MMMM yyyy"),TIMEFORMAT_NONE,    // month and year
	COleDateTimeSpan(28, 0, 0, 0),  DATETIMERANGE_MONTH,   _T("MMMM"),     TIMEFORMAT_NONE,    // month (full)
	COleDateTimeSpan(28, 0, 0, 0),  DATETIMERANGE_MONTH,   _T("MMM"),      TIMEFORMAT_NONE,    // month (abbr)
	COleDateTimeSpan(28, 0, 0, 0),  DATETIMERANGE_MONTH,   _T("MM"),       TIMEFORMAT_NONE,    // month (numeric)
	COleDateTimeSpan(365, 0, 0, 0), DATETIMERANGE_YEAR,    _T("yyyy"),     TIMEFORMAT_NONE,    // year only
};

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


BEGIN_MESSAGE_MAP(CBwGanttChart, CBCGPGanttChart)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_REGISTERED_MESSAGE(BCGM_GANTT_STORAGE_CHANGED, OnStorageChanged)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipText)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//                          Helper functions
//////////////////////////////////////////////////////////////////////////

CFont* SelectFont(CDC* pDC, CFont* pFont)
{
	ASSERT_VALID(pDC);

	return pDC->SelectObject((pFont == NULL) ? &globalData.fontRegular : pFont);
}

UINT GetFontHeight(CFont* pFont, CDC* pDC = NULL)
{
	if (pFont == NULL)
	{
		return globalData.GetTextHeight();
	}

	TEXTMETRIC tm;

	if (pDC == NULL)
	{
		CWindowDC dc(NULL);
		CFont* pOldFont = dc.SelectObject(pFont);
		dc.GetTextMetrics(&tm);
		dc.SelectObject(pOldFont);
	}
	else
	{
		CFont* pOldFont = pDC->SelectObject(pFont);
		pDC->GetTextMetrics(&tm);
		pDC->SelectObject(pOldFont);
	}

	return tm.tmHeight + (tm.tmHeight < 15 ? 2 : 5);
	// see also  BCGPGLOBAL_DATA::UpdateTextMetrics() in BCGGlobals.cpp
}


// Extracts date by setting time to 0:00:00
static COleDateTime ExtractDate(const COleDateTime& date)
{
	return COleDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), 0, 0, 0);
}

static BOOL IsTimeNull(const COleDateTime& date)
{
	SYSTEMTIME st;
	date.GetAsSystemTime(st);
	return st.wHour == 0 && st.wMinute == 0 && st.wSecond == 0;
}

static DWORD DetectSpanType(const COleDateTimeSpan& tmSpan)
{
	int nDays = (int)tmSpan.GetTotalDays();
	int nMonths = nDays / 28;
	int nYears = nDays / 365;
	int nSeconds = (int)tmSpan.GetTotalSeconds();
	int nMinutes = nSeconds / 60;
	int nHours = nMinutes / 60;

	return DATETIMERANGE_DAY;

	if (nYears >= 1)
	{
		return DATETIMERANGE_YEAR;
	}
	if (nMonths == 3)
	{
		return DATETIMERANGE_QUARTER;
	}
	if (nMonths >= 1)
	{
		return DATETIMERANGE_MONTH;
	}
	if (nDays == 7)
	{
		return DATETIMERANGE_WEEK;
	}
	if (nDays >= 1)
	{
		return DATETIMERANGE_DAY;
	}

	if (nHours == 12)
	{
		return DATETIMERANGE_12HOURS;
	}
	if (nHours == 6)
	{
		return DATETIMERANGE_6HOURS;
	}
	if (nHours == 4)
	{
		return DATETIMERANGE_4HOURS;
	}
	if (nHours == 2)
	{
		return DATETIMERANGE_2HOURS;
	}
	if (nHours >= 1)
	{
		return DATETIMERANGE_1HOUR;
	}

	if (nMinutes == 30)
	{
		return DATETIMERANGE_30MINUTES;
	}
	if (nMinutes == 20)
	{
		return DATETIMERANGE_20MINUTES;
	}
	if (nMinutes == 15)
	{
		return DATETIMERANGE_15MINUTES;
	}
	if (nMinutes == 10)
	{
		return DATETIMERANGE_10MINUTES;
	}
	if (nMinutes == 5)
	{
		return DATETIMERANGE_5MINUTES;
	}
	if (nMinutes >= 1)
	{
		return DATETIMERANGE_1MINUTE;
	}

	if (nSeconds == 30)
	{
		return DATETIMERANGE_30SECONDS;
	}

	return DATETIMERANGE_1SECOND;
}

static void GetDateTimeRange(const COleDateTime& date, DWORD dwRangeType, COleDateTime& tmStart, COleDateTime& tmEnd)
{
	SYSTEMTIME st;
	date.GetAsSystemTime(st);

	// 时间间隔设置为周
	tmStart = date - COleDateTimeSpan(st.wDayOfWeek, 0, 0, 0);
	tmStart = ExtractDate(tmStart);
	tmEnd = tmStart + COleDateTimeSpan(7, 0, 0, 0);
	return;

	switch (dwRangeType)
	{
	case DATETIMERANGE_YEAR:
		tmStart.SetDateTime(st.wYear, 1, 1, 0, 0, 0);
		tmEnd.SetDateTime(st.wYear + 1, 1, 1, 0, 0, 0);
		return;
	case DATETIMERANGE_QUARTER:
		tmStart.SetDateTime(st.wYear, (st.wMonth / 3) * 3, 1, 0, 0, 0);
		if (st.wMonth < 10)
		{
			tmEnd.SetDateTime(st.wYear, (st.wMonth / 3) * 3 + 3, 1, 0, 0, 0);
		}
		else
		{
			tmEnd.SetDateTime(st.wYear + 1, 1, 1, 0, 0, 0);
		}
		return;
	case DATETIMERANGE_MONTH:
		tmStart.SetDateTime(st.wYear, st.wMonth, 1, 0, 0, 0);
		if (st.wMonth < 12)
		{
			tmEnd.SetDateTime(st.wYear, st.wMonth + 1, 1, 0, 0, 0);
		}
		else
		{
			tmEnd.SetDateTime(st.wYear + 1, 1, 1, 0, 0, 0);
		}
		return;
	case DATETIMERANGE_WEEK:
		tmStart = date - COleDateTimeSpan(st.wDayOfWeek, 0, 0, 0);
		tmStart = ExtractDate(tmStart);
		tmEnd = tmStart + COleDateTimeSpan(7, 0, 0, 0);
		return;
	case DATETIMERANGE_DAY:
		tmStart = ExtractDate(date);
		tmEnd = tmStart + COleDateTimeSpan(1, 0, 0, 0);
		return;
	case DATETIMERANGE_4HOURS:
		tmStart.SetDateTime(st.wYear, st.wMonth, st.wDay, (st.wHour / 4) * 4, 0, 0);
		tmEnd = tmStart + COleDateTimeSpan(0, 4, 0, 0);
		return;
	case DATETIMERANGE_1HOUR:
		tmStart.SetDateTime(st.wYear, st.wMonth, st.wDay, st.wHour, 0, 0);
		tmEnd = tmStart + COleDateTimeSpan(0, 1, 0, 0);
		return;
	case DATETIMERANGE_15MINUTES:
		tmStart.SetDateTime(st.wYear, st.wMonth, st.wDay, st.wHour, (st.wMinute / 15) * 15, 0);
		tmEnd = tmStart + COleDateTimeSpan(0, 0, 15, 0);
		return;
	case DATETIMERANGE_5MINUTES:
		tmStart.SetDateTime(st.wYear, st.wMonth, st.wDay, st.wHour, (st.wMinute / 5) * 5, 0);
		tmEnd = tmStart + COleDateTimeSpan(0, 0, 5, 0);
		return;
	case DATETIMERANGE_1MINUTE:
		tmStart.SetDateTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, 0);
		tmEnd = tmStart + COleDateTimeSpan(0, 0, 1, 0);
		return;
	case DATETIMERANGE_1SECOND:
		tmStart.SetDateTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		tmEnd = tmStart + COleDateTimeSpan(0, 0, 0, 1);
		return;
	default:
		TRACE0("GetDateTimeRange() : Invalid range type argument\n");
		ASSERT(FALSE);
		break;
	}
}

CBwGanttChart::CBwGanttChart()
	: m_pRenderer(NULL)
	, _pControl(NULL)
	, _bSingleProgressBar(FALSE)
{
	_clrCurTime = RGB(255, 100, 100);
	_arrWeekDays = { _T("一"), _T("二"), _T("三"), _T("四"), _T("五"), _T("六"), _T("日") };
}

CBwGanttChart::~CBwGanttChart()
{
	if (m_pRenderer != NULL)
	{
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

CBwGanttRenderer * CBwGanttChart::GetRenderer()
{
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CBwGanttRenderer();
	}
	return m_pRenderer;
}

void CBwGanttChart::PerformAdjustLayout()
{
	if (GetSafeHwnd() == NULL || m_bInAdjustLayout || m_nControlState == ecsDestroying)
	{
		return;
	}

	stUpdateInfo *pUpdateInfo = (stUpdateInfo *)m_pUpdateInfo;
	if (pUpdateInfo == NULL)
		return;

	m_bInAdjustLayout = TRUE;
	{
		UpdateScrollBars(FALSE);

		HiliteHeaderCell(NULL);

		CRect rcClient;
		GetClientRect(&rcClient);

		UINT nToolTipId = 1;

		BOOL bHorzBarVisible = (m_wndScrollHorz.GetSafeHwnd() != NULL) && (m_wndScrollHorz.IsWindowVisible());
		BOOL bVertBarVisible = (m_wndScrollVert.GetSafeHwnd() != NULL) && (m_wndScrollVert.IsWindowVisible());
		if (bHorzBarVisible)
		{
			rcClient.bottom -= ::GetSystemMetrics(SM_CXHSCROLL);
		}
		if (bVertBarVisible)
		{
			rcClient.right -= ::GetSystemMetrics(SM_CXVSCROLL);
		}

		// Calculate headers and chart areas

		m_rectChart = rcClient;
		m_rectLargeHeader.SetRectEmpty();
		m_rectSmallHeader.SetRectEmpty();

		if (m_hdrLargeHeader.bVisible)
		{
			m_rectLargeHeader = m_rectChart;
			if (m_hdrLargeHeader.bAboveChart)
			{
				m_rectLargeHeader.top = m_rectChart.top;
				m_rectLargeHeader.bottom = m_rectLargeHeader.top + m_hdrLargeHeader.nHeight;
				m_rectChart.top = m_rectLargeHeader.bottom;
			}
			else
			{
				m_rectLargeHeader.bottom = m_rectChart.bottom;
				m_rectLargeHeader.top = m_rectLargeHeader.bottom - m_hdrLargeHeader.nHeight;
				m_rectChart.bottom = m_rectLargeHeader.top;
			}
			if (m_pToolTip != NULL && m_bShowTooltips)
			{
				m_pToolTip->AddTool(this, LPSTR_TEXTCALLBACK, &m_rectLargeHeader, nToolTipId++);
			}
		}

		if (m_hdrSmallHeader.bVisible)
		{
			m_rectSmallHeader = m_rectChart;
			if (m_hdrSmallHeader.bAboveChart)
			{
				m_rectSmallHeader.top = m_rectChart.top;
				m_rectSmallHeader.bottom = m_rectSmallHeader.top + m_hdrSmallHeader.nHeight;
				m_rectChart.top = m_rectSmallHeader.bottom;
			}
			else
			{
				m_rectSmallHeader.bottom = m_rectChart.bottom;
				m_rectSmallHeader.top = m_rectSmallHeader.bottom - m_hdrSmallHeader.nHeight;
				m_rectChart.bottom = m_rectSmallHeader.top;
			}
			if (m_pToolTip != NULL && m_bShowTooltips)
			{
				m_pToolTip->AddTool(this, LPSTR_TEXTCALLBACK, &m_rectSmallHeader, nToolTipId++);
			}
		}

		

		if (pUpdateInfo->bLayoutChanged)
		{
			CBwGanttControl* pGanttNotify = GetGanttControl();
			if (pGanttNotify != NULL)
			{
				pGanttNotify->DoVerticalResize(this);
			}
		}

		UpdateItemsLayout();

		int n = (int)m_arrItemRects.GetSize();
		m_nFirstVisibleRow = -1;
		m_nLastVisibleRow = -1;
		for (int i = 0; i < n; ++i)
		{
			const stRowLayout& r = m_arrItemRects[i];
			if (r.nRowTopY <= m_rectChart.bottom && r.nRowBottomY >= m_rectChart.top)
			{
				if (m_nFirstVisibleRow < 0)
				{
					m_nFirstVisibleRow = i;
				}
			}
			else if (r.nRowTopY > m_rectChart.bottom)
			{
				m_nLastVisibleRow = i - 1;
			}
		}

		if (m_nLastVisibleRow < 0)
		{
			m_nLastVisibleRow = n - 1;
		}

		// Dynamic item tooltip (for whole chart area)
		if (m_pToolTip != NULL && m_bShowTooltips)
		{
			m_pToolTip->AddTool(this, LPSTR_TEXTCALLBACK, m_rectChart, nToolTipId++);
		}

		UpdateScrollBars(TRUE); // Update scrollbar values

	} // end of Adjust layout code block


	pUpdateInfo->Reset();
	m_bInAdjustLayout = FALSE;
}

void CBwGanttChart::SetCurTime(const COleDateTime &curTime)
{
	_curTime = curTime;
}

void CBwGanttChart::SetCurTimeLineColor(COLORREF clr)
{
	_clrCurTime = clr;
}

void CBwGanttChart::SetControl(CBwGanttControl * pControl)
{
	_pControl = pControl;
}

void CBwGanttChart::SetSingleProgressBar(bool bSingle)
{
	_bSingleProgressBar = bSingle;
}

void CBwGanttChart::SetHSrollPos(const COleDateTime& time)
{
	SetLeftmostDateTime(time);
}

CBwGanttItem * CBwGanttChart::GetSelItem()
{
	int nSel = GetSelIndex();
	if (nSel < 0)
		return NULL;
	return dynamic_cast<CBwGanttItem *>(GetItemByIndex(nSel));
}

int CBwGanttChart::GetSelIndex()
{
	if (!_pControl)
		return -1;

	auto pGrid = dynamic_cast<CBwGanttGrid *>(_pControl->GetGrid());
	if (!pGrid)
		return -1;
	
	return pGrid->GetSelIndex();
}

void CBwGanttChart::OnPaint()
{
	CPaintDC dc(this);
	OnDraw(&dc);
}

void CBwGanttChart::OnMouseMove(UINT nFlags, CPoint point)
{
	return;
}

LRESULT CBwGanttChart::OnStorageChanged(WPARAM wParam, LPARAM lParam)
{
	__super::OnStorageChanged(wParam, lParam);
	
	// 更新水平滚动条
	RecalculateItemsTimeRange();

	return 0L;
}
CBwGanttControl* CBwGanttChart::GetGanttControl()
{
	CWnd* pParent = GetParent();
	if (pParent == NULL)
	{
		return NULL;
	}
	CWnd* pParent2 = pParent->GetParent();
	if (pParent2 == NULL)
	{
		pParent2 = pParent;
	}
	CBwGanttControl* pGanttCtrl = DYNAMIC_DOWNCAST(CBwGanttControl, pParent2);
	if (pGanttCtrl == NULL)
	{
		return NULL;
	}
	return static_cast<CBwGanttControl*>(pGanttCtrl);
}

void CBwGanttChart::DoPaint(CDC & dc)
{
	CRect rcClip;
	dc.GetClipBox(&rcClip);

	CRect rcChart = GetVisibleChartArea();
	rcChart.IntersectRect(&rcChart, &rcClip);
	DrawChartBackground(dc, rcChart);

	// Calculate rows that are really need update

	int nFirstVisibleRow = -1; // actual first visible row (depends on clipping rectangle)
	int nLastVisibleRow = -1; // actual last visible row (depends on clipping rectangle)
	int nItemRects = (int)m_arrItemRects.GetSize();
	int i;
	for (i = m_nFirstVisibleRow; m_nFirstVisibleRow >= 0 && i <= m_nLastVisibleRow && i < nItemRects; ++i)
	{
		int nRowTop = m_arrItemRects[i].nRowTopY;
		int nRowBottom = m_arrItemRects[i].nRowBottomY;
		if (nRowTop <= rcChart.bottom && nRowBottom >= rcChart.top)
		{
			if (nFirstVisibleRow < 0)
			{
				nFirstVisibleRow = i;
			}
		}
		else if (nRowTop > rcChart.bottom)
		{
			nLastVisibleRow = i - 1;
			break;
		}
	}

	if (nLastVisibleRow < 0)
	{
		nLastVisibleRow = m_nLastVisibleRow;
	}
	if (nFirstVisibleRow < 0)
	{
		nFirstVisibleRow = m_nFirstVisibleRow;
	}

	// Drawing visible items background

	for (i = nFirstVisibleRow; (i <= nLastVisibleRow) && (i >= 0) && (i < nItemRects); ++i)
	{
		CRect rectRow(m_rectChart.left, m_arrItemRects[i].nRowTopY, m_rectChart.right, m_arrItemRects[i].nRowBottomY);
		DrawItemBackground(dc, rectRow, m_arrItemRects[i].pItem);
	}

	if (m_bShowEmptyRows)
	{
		CRect rectRow = rcChart;

		if (m_nFirstVisibleRow < 0 || m_nLastVisibleRow < 0 || m_nLastVisibleRow < m_nFirstVisibleRow) // no visible items
		{
			rectRow.bottom = rectRow.top;
		}
		else
		{
			rectRow.top = m_arrItemRects[m_nLastVisibleRow].nRowTopY;
			rectRow.bottom = m_arrItemRects[m_nLastVisibleRow].nRowBottomY;
		}
		while (rectRow.bottom < rcChart.bottom)
		{
			rectRow.top = rectRow.bottom;
			rectRow.bottom = rectRow.top + GetDefaultItemHeight();
			DrawItemBackground(dc, rectRow, NULL);
		}
	}

	// Drawing vertical grid lines

	BCGP_GANTT_CHART_COLORS colors = GetActualColors();

	int yBottom = rcChart.bottom;

	if ((double)m_dtPixelTimeDelta > 0.0 && (double)m_dtGridLinesSmallSpan / (double)m_dtPixelTimeDelta > 3.0)
	{
		CBCGPPenSelector pen(dc, colors.clrGridLine1, 1);

		COleDateTime t = m_dtLeftMostChartTime;
		while (IsTimeVisible(t))
		{
			COleDateTime tmStart, tmEnd;
			GetHeaderCellTimeRange(t, m_dtGridLinesSmallSpan, tmStart, tmEnd);

			int x = TimeToClient(tmStart) - 1;
			dc.MoveTo(x, rcChart.top);
			dc.LineTo(x, yBottom);

			t = tmEnd;
		}
	}

	if ((double)m_dtPixelTimeDelta > 0.0 && (double)m_dtGridLinesLargeSpan / (double)m_dtPixelTimeDelta > 3.0)
	{
		CBCGPPenSelector pen(dc, colors.clrGridLine0, 1, PS_DOT);

		COleDateTime t = m_dtLeftMostChartTime;
		while (IsTimeVisible(t))
		{
			COleDateTime tmStart, tmEnd;
			GetHeaderCellTimeRange(t, m_dtGridLinesLargeSpan, tmStart, tmEnd);

			int x = TimeToClient(tmStart) - 1;
			dc.MoveTo(x, rcChart.top);
			dc.LineTo(x, yBottom);

			t = tmEnd;
		}
	}

	// Drawing headers

	DrawHeader(dc, m_hdrLargeHeader, m_rectLargeHeader);
	DrawHeader(dc, m_hdrSmallHeader, m_rectSmallHeader);

	CRgn rgnChart;
	rgnChart.CreateRectRgnIndirect(rcChart);

	// Drawing item bars
	CBwGanttRenderer* pRenderer = GetRenderer();
	COLORREF clrSelection = GetActualColors().clrSelection;
	COLORREF clrSelectionBorder = GetActualColors().clrSelectionBorder;

	BOOL bDraggingItems = m_nControlState == ecsMoveItems || m_nControlState == ecsResizeItems;

	for (i = nFirstVisibleRow; (i <= nLastVisibleRow) && (i >= 0) && (i < nItemRects); ++i)
	{
		const CBwGanttItem* pItem = dynamic_cast<CBwGanttItem *>(m_arrItemRects[i].pItem);
		if (pItem == NULL)
			return;
		BOOL  bSelected = pItem->IsSelected();

		dc.SelectClipRgn(&rgnChart);
		CBwGanttDrawContext ctxDraw(*pItem, dc);
		SetupItemDrawContext(ctxDraw);
		ctxDraw.m_rectClip = rcChart;
		ctxDraw.m_rectBar = m_arrItemRects[i].rectBar;
		ctxDraw.m_rectRealBar = m_arrItemRects[i].rectRealBar;

		pRenderer->DrawBar(ctxDraw);

		if (bSelected)
		{
			ctxDraw.m_clrFill = clrSelection;
			ctxDraw.m_clrBorder = clrSelectionBorder;
			pRenderer->DrawSelection(ctxDraw);
		}
	}

	// 绘制当前所在位置的竖直线
	if ((double)m_dtPixelTimeDelta > 0.0 && (double)m_dtGridLinesSmallSpan / (double)m_dtPixelTimeDelta > 3.0)
	{
		CBCGPPenSelector pen(dc, _clrCurTime, 1);

		if (IsTimeVisible(_curTime))
		{
			int x = TimeToClient(_curTime) - 1;
			dc.MoveTo(x, rcChart.top);
			dc.LineTo(x, yBottom);

		}
	}

	dc.SelectClipRgn(NULL);
}

void CBwGanttChart::DrawHeader(CDC& dc, const BCGP_GANTT_CHART_HEADER& header, const CRect& rcPaint) const
{
	CRect rcChart = GetVisibleChartArea();
	COleDateTime t = m_dtLeftMostChartTime;

	while (IsTimeVisible(t))
	{
		COleDateTime tmStart, tmEnd;
		GetHeaderCellTimeRange(t, header.dtCellTimeDelta, tmStart, tmEnd);
		BW_GANTT_CHART_HEADER_CELL_INFO ci;
		ci.pHeaderInfo = &header;
		ci.rectCell = rcPaint;
		ci.rectCell.left = TimeToClient(tmStart);
		ci.rectCell.right = TimeToClient(tmEnd);
		ci.rectColumn = ci.rectCell;
		ci.rectColumn.top = rcChart.top;
		ci.rectColumn.bottom = rcChart.bottom;
		ci.dtCellLeftMostTime = tmStart;
		ci.dtCellTimeDelta = (tmEnd - tmStart);
		ci.rectClip = rcPaint;

		if (header.bVisible && header.nHeight > 0)
		{
			CString sText;
			if (_pControl->GetTimeDimension() == CBwGanttChart::eDate)
			{
				sText = FormatDate(t, header.sDateFormat) + FormatTime(t, header.dwTimeFormat);
			}
			else if(!header.sDateFormat.IsEmpty())
			{
				sText.Format(_T("%d"), (int)t);
			}
			DrawHeaderCell(dc, ci, FALSE);
			DrawHeaderText(dc, ci, sText, FALSE);
		}

		t = tmEnd;
	}
}

void CBwGanttChart::DrawHeaderCell(CDC& dc, const BW_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL bHilite) const
{
	DrawGanttHeaderCell(this, dc, cellInfo, bHilite);
}

void CBwGanttChart::DrawHeaderText(CDC& dc, const BW_GANTT_CHART_HEADER_CELL_INFO& cellInfo, const CString& sCellText, BOOL bHilite) const
{
	DrawGanttHeaderText(this, dc, cellInfo, sCellText, bHilite);
}

void CBwGanttChart::SetupItemDrawContext(CBwGanttDrawContext & ctx) const
{
	ctx.m_clrFill = ctx.m_Item.GetPrimaryColor();
	ctx.m_clrFill2 = ctx.m_Item.GetCompleteColor();
	ctx.m_clrRealFill = ctx.m_Item.GetRealPrimaryColor();
	ctx.m_clrRealFill2 = ctx.m_Item.GetRealCompleteColor();

	BCGP_GANTT_CHART_COLORS colors = GetActualColors();

	if (ctx.m_clrFill == CLR_DEFAULT)
	{
		ctx.m_clrFill = ctx.m_Item.IsGroupItem() ? colors.clrGroupBarFill : colors.clrBarFill;
	}
	if (ctx.m_clrFill2 == CLR_DEFAULT)
	{
		ctx.m_clrFill2 = colors.clrBarComplete;
	}

	ctx.m_clrBorder = CBCGPDrawManager::MixColors(ctx.m_clrFill, RGB(48, 48, 48), 0.75);
}

void CBwGanttChart::UpdateItemsLayout()
{
	stUpdateInfo *pUpdateInfo = (stUpdateInfo *)m_pUpdateInfo;
	if (pUpdateInfo == NULL)
		return;
	if (pUpdateInfo->bItemsLayoutChanged)
	{
		// Update all items layout

		m_arrItemRects.RemoveAll();

		int y = GetFirstItemOffset();

		POSITION pos = m_pItems->GetHeadPosition();

		while (pos != NULL)
		{
			CBCGPGanttItem* pCurItem = m_pItems->GetNext(pos);
			CBwGanttItem *pCurrentItem = dynamic_cast<CBwGanttItem *>(pCurItem);
			if (!pCurrentItem)
				continue;
			KNode *pNode = pCurrentItem->GetNode();
			if (!pNode)
				continue;
			int h = GetItemHeight(pCurrentItem);
			int nBarHeight = GetItemBarHeight(pCurrentItem); // virtual call
			nBarHeight = min(nBarHeight, h - 4);
			if (h > 0)
			{
				stRowLayout r;
				r.pItem = pCurrentItem;

				r.nRowTopY = y;
				r.nRowBottomY = y + h;

				// 单行进度
				if (_bSingleProgressBar)
				{
					// 进度显示
					r.rectBar.left = TimeToClient(pCurrentItem->GetStartTime());
					COleDateTime finishTime = pCurrentItem->GetFinishTime();
					r.rectBar.right = TimeToClient(finishTime);
					r.rectBar.top = y + (h - nBarHeight) / 2;
					r.rectBar.bottom = r.rectBar.top + nBarHeight;
				}
				// 多行进度
				else
				{
					// 计划进度显示
					r.rectBar.left = TimeToClient(pCurrentItem->GetStartTime());
					COleDateTime finishTime = pCurrentItem->GetFinishTime();
					r.rectBar.right = TimeToClient(finishTime);
					r.rectBar.top = y + (h - nBarHeight) / 2;
					r.rectBar.bottom = r.rectBar.top + nBarHeight / 2;

					// 实际进度显示
					if (pNode->HasData(COL_REAL_STARTTIME) && pNode->HasData(COL_REAL_ENDTIME))
					{
						r.rectRealBar.left = TimeToClient(pCurrentItem->GetRealStartTime());
						COleDateTime realFinishTime = pCurrentItem->GetRealFinishTime();
						r.rectRealBar.right = TimeToClient(realFinishTime);
						r.rectRealBar.top = y + (h - nBarHeight) / 2;
						r.rectRealBar.bottom = r.rectRealBar.top + nBarHeight;
						r.rectRealBar.top += nBarHeight / 2;
					}
				}

				m_arrItemRects.Add(r);
			}
			y += h;
		}

		CArray<CBCGPGanttConnection*, CBCGPGanttConnection*> arrLinks;
		m_pItems->GetAllConnections(arrLinks);
		UpdateConnections(arrLinks);

		pUpdateInfo->bConnectionsChanged = FALSE;
		pUpdateInfo->bItemsLayoutChanged = FALSE;
	}
	else if (pUpdateInfo->nHScrollDelta != 0 || pUpdateInfo->nVScrollDelta != 0)
	{
		// Just scroll items

		for (int n = (int)m_arrItemRects.GetSize(); n > 0; --n)
		{
			m_arrItemRects[n - 1].nRowTopY -= pUpdateInfo->nVScrollDelta;
			m_arrItemRects[n - 1].nRowBottomY -= pUpdateInfo->nVScrollDelta;
			m_arrItemRects[n - 1].rectBar.OffsetRect(-pUpdateInfo->nHScrollDelta, -pUpdateInfo->nVScrollDelta);
			m_arrItemRects[n - 1].rectRealBar.OffsetRect(-pUpdateInfo->nHScrollDelta, -pUpdateInfo->nVScrollDelta);
		}

		pUpdateInfo->nHScrollDelta = 0;
		pUpdateInfo->nVScrollDelta = 0;
	}

}

void CBwGanttChart::RecalculateItemsTimeRange()
{
	__super::RecalculateItemsTimeRange();
	m_dtItemsMin = COleDateTime::GetCurrentTime();
	m_dtItemsMax = m_dtItemsMin;

	_pControl->GetDateRange(m_dtItemsMin, m_dtItemsMax);

	// 值刻度直接从0开始，不显示0前面的值
	if (_pControl->GetTimeDimension() == CBwGanttChart::eValue)
	{
		m_dtItemsMin += 1;
	}

	m_dtHScrollMin = m_dtItemsMin;
	m_dtHScrollMax = m_dtItemsMax;
}

void CBwGanttChart::UpdateHeaders()
{
	if (_pControl == NULL)
		return;
	// m_dtPixelTimeDelta has changed. Recalculate headers.

	ASSERT((double)m_dtPixelTimeDelta > 0.0);
	if ((double)m_dtPixelTimeDelta <= 0.0)
	{
		return;
	}

	// Detecting headers font metrics
	TEXTMETRIC tmHeaderFont;
	{
		CWindowDC dc(NULL);
		CFont* pOldFont = SelectFont(&dc, m_pFont);

		dc.GetTextMetrics(&tmHeaderFont);
		dc.SelectObject(pOldFont);
	}

	UINT nAverageCharWidth = (tmHeaderFont.tmAveCharWidth * 2 + tmHeaderFont.tmMaxCharWidth) / 3;

	// Expect array is ordered.

	const COleDateTime tmTest(1980, 11, 30, 23, 59, 59);

	// header 1 

	int i1, n = sizeof(s_arrSpanFormats) / sizeof(*s_arrSpanFormats);

	for (i1 = 0; i1 < n; ++i1)
	{
		CString sFormat = FormatDate(tmTest, s_arrSpanFormats[i1].m_pszDateFormat) + FormatTime(tmTest, s_arrSpanFormats[i1].m_dwTimeFormat);
		if (!sFormat.IsEmpty())
		{
			int nFmtMaxWidth = (nAverageCharWidth * sFormat.GetLength()) + tmHeaderFont.tmOverhang; // max width of text in pixels for this span
			int nSpanMinWidth = (int)(s_arrSpanFormats[i1].m_tmMinSpan / (double)m_dtPixelTimeDelta); // min width of this span (for current zoom)

			m_hdrSmallHeader.dtCellTimeDelta = 1; //s_arrSpanFormats[i1].m_tmMinSpan;
			m_hdrSmallHeader.sDateFormat = _T("dd");//s_arrSpanFormats[i1].m_pszDateFormat;
			m_hdrSmallHeader.dwTimeFormat = 0; //s_arrSpanFormats[i1].m_dwTimeFormat;
			if (nSpanMinWidth >= nFmtMaxWidth + 3)
			{
				break;
			}
		}
	}

	// header 0 (Large)

	m_hdrLargeHeader.dtCellTimeDelta =  s_arrSpanFormats[i1].m_tmMinSpan;
	m_hdrLargeHeader.sDateFormat = s_arrSpanFormats[i1].m_pszDateFormat;
	m_hdrLargeHeader.dwTimeFormat = s_arrSpanFormats[i1].m_dwTimeFormat;
	int i0;
	for (i0 = i1; i0 < n; ++i0)
	{
		if (s_arrSpanFormats[i0].m_dwSpanType == s_arrSpanFormats[i1].m_dwSpanType)
		{
			continue;
		}
		CString sFormat = _pControl->GetTimeDimension() == CBwGanttChart::eDate ? FormatDate(tmTest, s_arrSpanFormats[i0].m_pszDateFormat) + FormatTime(tmTest, s_arrSpanFormats[i0].m_dwTimeFormat) : _T("value");
		if (!sFormat.IsEmpty())
		{
			int nFmtMaxWidth = (tmHeaderFont.tmMaxCharWidth * sFormat.GetLength()) + tmHeaderFont.tmOverhang; // max width of text in pixels for this span
			int nSpanMinWidth = (int)(s_arrSpanFormats[i0].m_tmMinSpan / (double)m_dtPixelTimeDelta); // min width of this span (for current zoom)
			m_hdrLargeHeader.dtCellTimeDelta = s_arrSpanFormats[i0].m_tmMinSpan;
			m_hdrLargeHeader.sDateFormat = _pControl->GetTimeDimension() == CBwGanttChart::eDate ? s_arrSpanFormats[i0].m_pszDateFormat : _T("");
			m_hdrLargeHeader.dwTimeFormat = s_arrSpanFormats[i0].m_dwTimeFormat;

			if (nSpanMinWidth >= nFmtMaxWidth + 3) // 3 is an indent for cell borders
			{
				break;
			}
		}
	}

	// Set grid lines span

	m_dtGridLinesSmallSpan = m_hdrSmallHeader.dtCellTimeDelta;
	COleDateTimeSpan tmDay(1, 0, 0, 0), tmWeek(7, 0, 0, 0);

	int nDaySpanWidth = (int)((double)tmDay / (double)m_dtPixelTimeDelta); // in pixels

	if (true)
	{
		m_dtGridLinesLargeSpan.SetDateTimeSpan(7, 0, 0, 0); //week
	}
	else if (nDaySpanWidth > 240)
	{
		m_dtGridLinesLargeSpan.SetDateTimeSpan(0, 1, 0, 0); //hour
	}
	else if (nDaySpanWidth > 8)
	{
		m_dtGridLinesLargeSpan.SetDateTimeSpan(1, 0, 0, 0); //1 day
	}
	else if (nDaySpanWidth >= 4)
	{
		m_dtGridLinesLargeSpan.SetDateTimeSpan(7, 0, 0, 0); //week
	}
	else
	{
		m_dtGridLinesLargeSpan.SetDateTimeSpan(28, 0, 0, 0); //month
		m_dtGridLinesSmallSpan.SetDateTimeSpan(365, 0, 0, 0); //year
	}

	if (m_dtGridLinesLargeSpan == m_dtGridLinesSmallSpan)
	{
		m_dtGridLinesLargeSpan = m_hdrLargeHeader.dtCellTimeDelta;
	}

	if (m_dtGridLinesLargeSpan < m_dtGridLinesSmallSpan)
	{
		COleDateTimeSpan t = m_dtGridLinesSmallSpan;
		m_dtGridLinesSmallSpan = m_dtGridLinesLargeSpan;
		m_dtGridLinesLargeSpan = t;
	}

	InvalidateHeaders();
}

void CBwGanttChart::DrawItemBackground(CDC& dc, const CRect& rcItem, const CBCGPGanttItem* pItem) const
{
	CRect r = rcItem;
	r.bottom--;

	CBCGPPenSelector pen(dc, GetActualColors().clrGridLine1, 1);
	dc.MoveTo(r.left, r.bottom);
	dc.LineTo(r.right, r.bottom);

	CRect rcClip;
	dc.GetClipBox(&rcClip);
	rcClip.IntersectRect(&rcClip, &r);
	if (rcClip.IsRectEmpty())
	{
		return;
	}

	BOOL bShowDaysOff = TRUE;
	COleDateTimeSpan tmDay(1, 0, 0, 0);   // Check if scale is large enough so days-off can be shown
	double dPixelsPerDay = (double)tmDay / (double)m_dtPixelTimeDelta;
	if (dPixelsPerDay < 4.0) // one day must be at least 4 pixels wide to show days-off
	{
		bShowDaysOff = FALSE;
	}

	COleDateTime t = ClientToTime(rcClip.left);
	COleDateTime tEnd = ClientToTime(rcClip.right);
	DWORD dwRangeType = DATETIMERANGE_DAY;

	COleDateTime tmRangeStart = t;
	BOOL bPrevIsWT = IsWorkingTime(pItem, t) || !bShowDaysOff;

	CBCGPVisualManager* pVisualManager = CBCGPVisualManager::GetInstance();
	ASSERT_VALID(pVisualManager);

	BCGP_GANTT_CHART_COLORS colors = GetActualColors();
	for (;;)
	{
		COleDateTime tmStart;
		COleDateTime tmEnd;
		GetDateTimeRange(t, dwRangeType, tmStart, tmEnd);

		BOOL bIsWT = IsWorkingTime(pItem, tmStart) || !bShowDaysOff;

		if (bIsWT != bPrevIsWT || tmStart >= tEnd)
		{
			CRect rcFill = rcClip;
			rcFill.left = TimeToClient(tmRangeStart);
			rcFill.right = TimeToClient(tmStart); // start of current range

			pVisualManager->DrawGanttItemBackgroundCell(this, dc, rcItem, rcFill, colors, !bPrevIsWT);

			tmRangeStart = tmStart;
			bPrevIsWT = bIsWT;
		}

		if (tmStart > tEnd)
		{
			break;
		}

		t = tmEnd;
	}
}

BOOL CBwGanttChart::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 禁用鼠标滚轮事件
	return FALSE;
}

BOOL CBwGanttChart::HeaderCellFromPoint(CPoint pt, BW_GANTT_CHART_HEADER_CELL_INFO& cellInfo) const
{
	if (m_rectLargeHeader.PtInRect(pt))
	{
		cellInfo.pHeaderInfo = &m_hdrLargeHeader;
		cellInfo.rectCell = m_rectLargeHeader;
	}
	else if (m_rectSmallHeader.PtInRect(pt))
	{
		cellInfo.pHeaderInfo = &m_hdrSmallHeader;
		cellInfo.rectCell = m_rectSmallHeader;
	}
	else
	{
		return FALSE;
	}

	COleDateTime t = m_dtLeftMostChartTime;
	while (IsTimeVisible(t))
	{
		COleDateTime tmStart, tmFinish;
		GetHeaderCellTimeRange(t, cellInfo.pHeaderInfo->dtCellTimeDelta, tmStart, tmFinish);
		int xLeft = TimeToClient(tmStart);
		int xRight = TimeToClient(tmFinish);
		if (pt.x >= xLeft && pt.x < xRight)
		{
			CRect rcChart = GetVisibleChartArea();

			cellInfo.rectCell.left = xLeft;
			cellInfo.rectCell.right = xRight;
			cellInfo.rectColumn = cellInfo.rectCell;
			cellInfo.rectColumn.top = rcChart.top;
			cellInfo.rectColumn.bottom = rcChart.bottom;
			cellInfo.dtCellLeftMostTime = tmStart;
			cellInfo.dtCellTimeDelta = (tmFinish - tmStart);
			cellInfo.rectClip.SetRect(0, 0, 0, 0);
			return TRUE;
		}

		t = tmFinish;
	}

	return TRUE;
}

BOOL CBwGanttChart::OnToolTipText(UINT uiToolID, NMHDR* pNMH, LRESULT* pResult)
{
	static CString strTipText;
	CString strDescription;

	if (m_pToolTip->GetSafeHwnd() == NULL || pNMH->hwndFrom != m_pToolTip->GetSafeHwnd())
	{
		return FALSE;
	}

	if (CBCGPPopupMenu::GetActiveMenu() != NULL)
	{
		return FALSE;
	}

	LPNMTTDISPINFO pTTDispInfo = (LPNMTTDISPINFO)pNMH;
	ASSERT((pTTDispInfo->uFlags & TTF_IDISHWND) == 0);

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	BW_GANTT_CHART_HEADER_CELL_INFO cellInfo;
	if (HeaderCellFromPoint(point, cellInfo))
	{
		COleDateTime dateTime = cellInfo.dtCellLeftMostTime;
		if (_pControl->GetTimeDimension() == CBwGanttChart::eValue)
		{
			strTipText.Format(_T("%d秒"), (int)dateTime);
			pTTDispInfo->lpszText = const_cast<LPTSTR> ((LPCTSTR)strTipText);
			return TRUE;
		}
		else
		{
			__super::OnToolTipText(uiToolID, pNMH, pResult);
			return TRUE;
		}
	}

	return FALSE;
}
