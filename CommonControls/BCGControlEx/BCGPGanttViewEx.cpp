#include "stdafx.h"
#include "BCGPGanttViewEx.h"
#include "BCGPGanttGridEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////\
// CBCGPGanttView

IMPLEMENT_DYNCREATE(CBCGPGanttViewEx, CView)

CBCGPGanttViewEx::CBCGPGanttViewEx()
{
}

CBCGPGanttViewEx::~CBCGPGanttViewEx()
{
}


BEGIN_MESSAGE_MAP(CBCGPGanttViewEx, CView)
	//{{AFX_MSG_MAP(CBCGPGanttView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONTROL_CREATE_CHART, OnNotifyCreateChart)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONTROL_CREATE_GRID, OnNotifyCreateGrid)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_CLICKCHART, OnNotifyChartClick)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_CLICKHEADER, OnNotifyChartClickHeader)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_CLICKITEM, OnNotifyChartClickItem)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_DBLCLICKCHART, OnNotifyChartDblClick)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_DBLCLICKHEADER, OnNotifyChartDblClickHeader)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_DBLCLICKITEM, OnNotifyChartDblClickItem)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_ITEM_MOVING, OnNotifyChartItemMoving)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_SCALE_CHANGING, OnNotifyChartScaleChanging)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CHART_SCALE_CHANGED, OnNotifyChartScaleChanged)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONTROL_READ_ITEM_DATA_FROM_GRID, OnNotifyReadDataFromGrid)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONTROL_WRITE_ITEM_DATA_TO_GRID, OnNotifyWriteDataToGrid)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_STORAGE_CHANGED, OnNotifyStorageChanged)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONNECTION_ADDED, OnNotifyStorageConnectionAdded)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_CONNECTION_REMOVED, OnNotifyStorageConnectionRemoved)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////\
// CBCGPGanttView drawing

void CBCGPGanttViewEx::OnDraw(CDC* /*pDC*/)
{
}

void CBCGPGanttViewEx::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_wndGanttControl.CreateControls();
}

////////////////////////////////////////////////////////////////////////////\
// CBCGPGanttView diagnostics

#ifdef _DEBUG
void CBCGPGanttViewEx::AssertValid() const
{
	CView::AssertValid();
}

void CBCGPGanttViewEx::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////////////\
// CBCGPGanttView message handlers

int CBCGPGanttViewEx::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	if (!m_wndGanttControl.Create(WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, 1))
	{
		TRACE0("CBCGPGanttControl::OnCreate: cannot create gantt control\n");
		return -1;
	}

	return 0;
}
//*******************************************************************************
BOOL CBCGPGanttViewEx::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}
//*******************************************************************************
void CBCGPGanttViewEx::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_wndGanttControl.GetSafeHwnd() != NULL)
	{
		m_wndGanttControl.SetWindowPos(this, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER |
			SWP_NOOWNERZORDER | SWP_NOACTIVATE);
	}
}
//*******************************************************************************
void CBCGPGanttViewEx::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	if (m_wndGanttControl.GetSafeHwnd() != NULL)
	{
		m_wndGanttControl.SetFocus();
	}
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartClick(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	POINTSTOPOINT(point, lParam);

	return (LRESULT)OnChartClick((UINT)wParam, point);
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartClickItem(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)OnChartClickItem((UINT)wParam, (CBCGPGanttItem*)lParam);
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartClickHeader(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	POINTSTOPOINT(point, lParam);

	return (LRESULT)OnChartClickHeader((UINT)wParam, point);
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartDblClick(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	POINTSTOPOINT(point, lParam);

	return (LRESULT)OnChartDoubleClick((UINT)wParam, point);
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartDblClickItem(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)OnChartDoubleClickItem((UINT)wParam, (CBCGPGanttItem*)lParam);
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartDblClickHeader(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	POINTSTOPOINT(point, lParam);

	return (LRESULT)OnChartDoubleClickHeader((UINT)wParam, point);
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartItemMoving(WPARAM, LPARAM lParam)
{
	return OnChartItemMoving((BCGP_GANTT_ITEM_DRAGDROP*)lParam) ? 0L : 1L;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartItemsChanged(WPARAM wParam, LPARAM lParam)
{
	OnChartItemChanged((CBCGPGanttItem*)lParam, (DWORD)wParam);
	return 0L;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartScaleChanging(WPARAM wParam, LPARAM)
{
	return OnChartScaleChanging(wParam == 1) ? 1L : 0L;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyChartScaleChanged(WPARAM, LPARAM)
{
	OnChartScaleChanged();
	return 0L;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyStorageChanged(WPARAM, LPARAM lParam)
{
	BCGP_GANTT_STORAGE_UPDATE_INFO* pUpdate = (BCGP_GANTT_STORAGE_UPDATE_INFO*)lParam;
	if (pUpdate != NULL)
	{
		OnStorageChanged(*pUpdate);
	}
	return 0L;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyStorageConnectionAdded(WPARAM, LPARAM lParam)
{
	CBCGPGanttConnection* pConnection = (CBCGPGanttConnection*)lParam;
	if (pConnection != NULL)
	{
		OnStorageConnectionAdded(*pConnection);
	}
	return 0L;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyStorageConnectionRemoved(WPARAM, LPARAM lParam)
{
	CBCGPGanttConnection* pConnection = (CBCGPGanttConnection*)lParam;
	if (pConnection != NULL)
	{
		OnStorageConnectionRemoved(*pConnection);
	}
	return 0L;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyReadDataFromGrid(WPARAM, LPARAM lParam)
{
	BCGP_GANTT_CONTROL_ROW* pRowInfo = (BCGP_GANTT_CONTROL_ROW*)lParam;
	if (pRowInfo != NULL)
	{
		return (LRESULT)OnGridRowReadData(*pRowInfo);
	}
	return 0;
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyWriteDataToGrid(WPARAM, LPARAM lParam)
{
	BCGP_GANTT_CONTROL_ROW* pRowInfo = (BCGP_GANTT_CONTROL_ROW*)lParam;
	if (pRowInfo != NULL)
	{
		return (LRESULT)OnGridRowWriteData(*pRowInfo);
	}
	return 0;
}
//*******************************************************************************
BOOL CBCGPGanttViewEx::OnPreparePrinting(CPrintInfo* pInfo)
{
	return FALSE;
// 	CBCGPGanttGridEx* pWndGridCtrl = GetGrid();
// 	if (pWndGridCtrl != NULL)
// 	{
// 		ASSERT_VALID(pWndGridCtrl);
// 
// 		int nFirstItem = 0;										// By default print all grid items
// 		int nLastItem = max(pWndGridCtrl->GetTotalItems() - 1, 0);	// from first row to the last
// 		pWndGridCtrl->OnPreparePrintPages(pInfo, nFirstItem, nLastItem);
// 	}
// 
// 	return DoPreparePrinting(pInfo);
}
//*******************************************************************************
void CBCGPGanttViewEx::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	return;
// 	CBCGPGanttGridEx* pWndGridCtrl = GetGrid();
// 	if (pWndGridCtrl != NULL)
// 	{
// 		ASSERT_VALID(pWndGridCtrl);
// 
// 		ASSERT_VALID(pDC);
// 		ASSERT(pInfo != NULL);
// 
// 		int nFirstItem = 0;										// By default print all grid items
// 		int nLastItem = max(pWndGridCtrl->GetTotalItems() - 1, 0);	// from first row to the last
// 		pWndGridCtrl->OnPreparePrintPages(pInfo, nFirstItem, nLastItem);
// 
// 		ASSERT(pInfo == pWndGridCtrl->m_PrintParams.m_pPrintInfo);
// 		ASSERT(pInfo->m_lpUserData != NULL);
// 
// 		pWndGridCtrl->m_bIsPrinting = TRUE;
// 		pWndGridCtrl->m_pPrintDC = pDC;
// 
// 		pWndGridCtrl->OnBeginPrinting(pDC, pInfo);
// 
// 		// The printable area has not been initialized. Initialize it.
// 		pInfo->m_rectDraw.SetRect(0, 0,
// 			pDC->GetDeviceCaps(HORZRES),
// 			pDC->GetDeviceCaps(VERTRES));
// 
// 		// Page margins:
// 		CRect rectMargins = pWndGridCtrl->OnGetPageMargins(pDC, pInfo);
// 		pInfo->m_rectDraw.DeflateRect(&rectMargins);
// 
// 		// Specify pages count:
// 		int nPagesCount = pWndGridCtrl->OnCalcPrintPages(pDC, pInfo);
// 		pInfo->SetMaxPage(nPagesCount);
// 	}
}
//*******************************************************************************
void CBCGPGanttViewEx::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	return;
// 	CBCGPGanttGridEx* pWndGridCtrl = GetGrid();
// 	if (pWndGridCtrl != NULL)
// 	{
// 		ASSERT_VALID(pWndGridCtrl);
// 		pWndGridCtrl->OnEndPrinting(pDC, pInfo);
// 
// 		pWndGridCtrl->m_bIsPrinting = FALSE;
// 		pWndGridCtrl->m_pPrintDC = NULL;
// 
// 		pWndGridCtrl->AdjustLayout();
// 	}
}
//*******************************************************************************
void CBCGPGanttViewEx::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGanttGrid* pWndGridCtrl = GetGrid();
	if (pWndGridCtrl != NULL)
	{
		ASSERT_VALID(pDC);
		ASSERT(pInfo != NULL);
		ASSERT_VALID(pWndGridCtrl);

		// don't do anything if not fully initialized
// 		if (!pWndGridCtrl->m_bIsPrinting || pWndGridCtrl->m_pPrintDC == NULL)
// 		{
// 			return;
// 		}

		// Page margins:
		CRect rectMargins = pWndGridCtrl->OnGetPageMargins(pDC, pInfo);
		pInfo->m_rectDraw.DeflateRect(&rectMargins);

		pWndGridCtrl->OnPrint(pDC, pInfo);
	}
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyCreateChart(WPARAM, LPARAM)
{
	return (LRESULT)OnCreateChart();
}
//*******************************************************************************
LRESULT CBCGPGanttViewEx::OnNotifyCreateGrid(WPARAM, LPARAM)
{
	return (LRESULT)OnCreateGrid();
}
//*******************************************************************************
CBCGPGanttChartEx* CBCGPGanttViewEx::OnCreateChart()
{
	return NULL; // use default chart implementation
}
//*******************************************************************************
CBCGPGanttGridEx* CBCGPGanttViewEx::OnCreateGrid()
{
	return NULL; // use default grid implementation
}
//**************************************************************************
LRESULT CBCGPGanttViewEx::OnChangeVisualManager(WPARAM wp, LPARAM lp)
{
	if (m_wndGanttControl.GetSafeHwnd() != NULL)
	{
		m_wndGanttControl.SendMessage(BCGM_CHANGEVISUALMANAGER, wp, lp);
	}

	return 0;
}



