#include "stdafx.h"
#include "BCGPPropList.h"
#include "BCGPSplitterWndEx.h"
#include "afxglobals.h"

using namespace controls;
enum HitTestValue
{
	noHit = 0,
	vSplitterBox = 1,
	hSplitterBox = 2,
	bothSplitterBox = 3,        // just for keyboard
	vSplitterBar1 = 101,
	vSplitterBar15 = 115,
	hSplitterBar1 = 201,
	hSplitterBar15 = 215,
	splitterIntersection1 = 301,
	splitterIntersection225 = 525
};

BEGIN_MESSAGE_MAP(CBCGPSplitterWndEx, CBCGPSplitterWnd)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_DBLCLICK, OnGridDBLClick)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_ENDINPLACEEDIT, OnEndLabelEdit)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_BEGININPLACEEDIT, OnBeginLabelEdit)
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_CHANGED, OnGridItemChange)
	ON_REGISTERED_MESSAGE(BCGM_GRID_SEL_CHANGED, OnGridSelectChange)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()	

BOOL controls::CBCGPSplitterWndEx::PreTranslateMessage(MSG* pMsg)
{
	BOOL bResult =  __super::PreTranslateMessage(pMsg);

	switch (pMsg->message)
	{
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_NOTIFY:
	{
		GetParent()->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	break;
	default:
		break;
	}
	
	return bResult;
}

void controls::CBCGPSplitterWndEx::StartTracking(int ht)
{
	ASSERT_VALID(this);
	if (ht == noHit)
		return;

	// GetHitRect will restrict 'm_rectLimit' as appropriate
	GetInsideRect(m_rectLimit);

	if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		// split two directions (two tracking rectangles)
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;

		GetHitRect(row + vSplitterBar1, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(col + hSplitterBar1, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;
	}
	else if (ht == bothSplitterBox)
	{
		// hit on splitter boxes (for keyboard)
		GetHitRect(vSplitterBox, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(hSplitterBox, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;

		// center it
		m_rectTracker.OffsetRect(0, m_rectLimit.Height() / 2);
		m_rectTracker2.OffsetRect(m_rectLimit.Width() / 2, 0);
	}
	else
	{
		// only hit one bar
		GetHitRect(ht, m_rectTracker);
	}

	// allow active view to preserve focus before taking it away
	// 		CView* pView = (CView*)GetActivePane();
	// 		if (pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CView)))
	// 		{
	// 			ASSERT_VALID(pView);
	// 			CFrameWnd* pFrameWnd = EnsureParentFrame();
	// 			pView->OnActivateFrame(WA_INACTIVE, pFrameWnd);
	// 		}

	// steal focus and capture
	SetCapture();
	SetFocus();

	// make sure no updates are pending
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);

	// set tracking state and appropriate cursor
	m_bTracking = TRUE;
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_htTrack = ht;
	SetSplitCursor(ht);
}

void controls::CBCGPSplitterWndEx::StopTracking(BOOL bAccept)
{
	ASSERT_VALID(this);

	if (!m_bTracking)
		return;

	ReleaseCapture();

	// erase tracker rectangle
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_bTracking = m_bTracking2 = FALSE;

	// save old active view
	//	CWnd* pOldActiveView = GetActivePane();

	// m_rectTracker is set to the new splitter position (without border)
	// (so, adjust relative to where the border will be)
	m_rectTracker.OffsetRect(-AFX_CX_BORDER, -AFX_CY_BORDER);
	m_rectTracker2.OffsetRect(-AFX_CX_BORDER, -AFX_CY_BORDER);

	if (bAccept)
	{
		if (m_htTrack == vSplitterBox)
		{
			SplitRow(m_rectTracker.top);
		}
		else if (m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			// set row height
			TrackRowSize(m_rectTracker.top, m_htTrack - vSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack == hSplitterBox)
		{
			SplitColumn(m_rectTracker.left);
		}
		else if (m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			// set column width
			TrackColumnSize(m_rectTracker.left, m_htTrack - hSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225)
		{
			// set row height and column width
			int row = (m_htTrack - splitterIntersection1) / 15;
			int col = (m_htTrack - splitterIntersection1) % 15;

			TrackRowSize(m_rectTracker.top, row);
			TrackColumnSize(m_rectTracker2.left, col);
			RecalcLayout();
		}
		else if (m_htTrack == bothSplitterBox)
		{
			// rectTracker is vSplitter (splits rows)
			// rectTracker2 is hSplitter (splits cols)
			SplitRow(m_rectTracker.top);
			SplitColumn(m_rectTracker2.left);
		}
	}

	// 		if (pOldActiveView == GetActivePane())
	// 		{
	// 			if (pOldActiveView != NULL)
	// 			{
	// 				SetActivePane(-1, -1, pOldActiveView); // re-activate
	// 				pOldActiveView->SetFocus(); // make sure focus is restored
	// 			}
	// 		}
}

BOOL controls::CBCGPSplitterWndEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (CWnd::OnNotify(wParam, lParam, pResult))
	{
		return TRUE;
	}
		

	GetParent()->SendMessage(WM_NOTIFY, wParam, lParam);

	return TRUE;
}

BOOL controls::CBCGPSplitterWndEx::OnCommand(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(WM_COMMAND, wParam, lParam);

	return TRUE;
}

LRESULT controls::CBCGPSplitterWndEx::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(BCGM_PROPERTY_CHANGED, wParam, lParam);
	return 0;
}

LRESULT controls::CBCGPSplitterWndEx::OnGridDBLClick(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(BCGM_GRID_ITEM_DBLCLICK, wParam, lParam);
	return 0;
}

LRESULT controls::CBCGPSplitterWndEx::OnEndLabelEdit(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(BCGM_GRID_ITEM_ENDINPLACEEDIT, wParam, lParam);
	return 0;
}

LRESULT controls::CBCGPSplitterWndEx::OnBeginLabelEdit(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(BCGM_GRID_ITEM_BEGININPLACEEDIT, wParam, lParam);
	return 0;
}

LRESULT controls::CBCGPSplitterWndEx::OnGridItemChange(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(BCGM_GRID_ITEM_CHANGED, wParam, lParam);
	return 0;
}

LRESULT controls::CBCGPSplitterWndEx::OnGridSelectChange(WPARAM wParam, LPARAM lParam)
{
	GetParent()->SendMessage(BCGM_GRID_SEL_CHANGED, wParam, lParam);
	return 0;
}

