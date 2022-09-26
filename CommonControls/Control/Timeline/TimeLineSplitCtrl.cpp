#include "stdafx.h"
#include "TimeLineSplitCtrl.h"

BEGIN_MESSAGE_MAP(CTimeLineSplitCtrl, CSplitCtrl)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

CTimeLineSplitCtrl::CTimeLineSplitCtrl(void)
{
	m_xySplitterPos = -1;
	m_nDefActivePane = SPLIT_PANE_NONE;

	m_cxySplitBar = 0;
	m_cxyMin = 0;
	m_cxyBarEdge = 0;
	m_cxyDragOffset = 0;
	m_nProportionalPos = 0;

	m_bFullDrag = true;
	m_bUpdateProportionalPos = true;
	t_bVertical = false;
	_bFixed = false;

	m_dwExtendedStyle = SPLIT_PROPORTIONAL;
	m_nSinglePane = SPLIT_PANE_NONE;

	m_hCursor = NULL;
	m_hWndPane[SPLIT_PANE_LEFT] = NULL;
	m_hWndPane[SPLIT_PANE_RIGHT] = NULL;

	::SetRectEmpty(&m_rcSplitter);

	_pTimeLineCtrl = nullptr;
}

void CTimeLineSplitCtrl::UpdateScrollBar(void)
{
	SCROLLINFO scrInfo;
	scrInfo.cbSize = sizeof(scrInfo);

	if (nullptr == _pTimeLineCtrl)
	{
		return;
	}

	int nSumRowHeight = _pTimeLineCtrl->GetCellHeight() * _pTimeLineCtrl->GetItemCount()
		+ _pTimeLineCtrl->GetHeadHeight() + scrInfo.cbSize;
	CRect rect;
	GetClientRect(&rect);

	scrInfo.fMask = SIF_PAGE | SIF_RANGE;
	scrInfo.nPage = rect.Height();
	scrInfo.nMin = 0;
	scrInfo.nMax = nSumRowHeight - 1;

	SetScrollInfo(SB_VERT, &scrInfo);
}

void CTimeLineSplitCtrl::SetCorrespondingTimeFrameCtrl(CTimeFrameListCtrl *pTimeFrameCtrl)
{
	_pTimeLineCtrl = pTimeFrameCtrl;
}

void CTimeLineSplitCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

	SCROLLINFO infoScroll;
	infoScroll.cbSize = sizeof(SCROLLINFO);
	infoScroll.fMask = SIF_ALL;

	if (!GetScrollInfo(SB_VERT, &infoScroll))
		return;

	int nNewPos = infoScroll.nPos;
	switch (nSBCode)
	{
	case SB_TOP:
		nNewPos = infoScroll.nMin;
		break;
	case SB_LINEUP:
		nNewPos = max(infoScroll.nPos - 5, infoScroll.nMin);
		break;
	case SB_PAGEUP:
		nNewPos = max(infoScroll.nPos - int(infoScroll.nPage), infoScroll.nMin);
		break;
	case SB_BOTTOM:
		nNewPos = infoScroll.nMax;
		break;
	case SB_LINEDOWN:
		nNewPos = min(infoScroll.nPos + 5, infoScroll.nMax);
		break;
	case SB_PAGEDOWN:
		nNewPos = min(int(infoScroll.nPos + infoScroll.nPage), infoScroll.nMax);
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		nNewPos = infoScroll.nTrackPos;
		break;
	}

	infoScroll.fMask = SIF_POS;
	infoScroll.nPos = nNewPos;

	SetScrollInfo(SB_VERT, &infoScroll);

	InvalidateRect(NULL);
}