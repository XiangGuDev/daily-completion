#include "stdafx.h"
#include "BCGPGanttControlEx.h"

IMPLEMENT_DYNCREATE(CBCGPGanttControlEx, CBCGPGanttControl)
BEGIN_MESSAGE_MAP(CBCGPGanttControlEx, CBCGPGanttControl)
	//{{AFX_MSG_MAP(CBCGPGanttControl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_MESSAGE(WM_GETFONT, OnGetFont)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_REGISTERED_MESSAGE(BCGM_GANTT_STORAGE_CHANGED, OnNotifyStorageChanged)
END_MESSAGE_MAP()

CBCGPGanttControlEx::CBCGPGanttControlEx()
{

}

CBCGPGanttControlEx::~CBCGPGanttControlEx()
{

}

BOOL CBCGPGanttControlEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_pParentWnd = pParentWnd;
	m_pGrid = dynamic_cast<CBCGPGanttGridEx*>(CreateGrid());
	m_pChart = dynamic_cast<CBCGPGanttChartEx*>(CreateChart());
	BOOL bResult = CWnd::Create(globalData.RegisterWindowClass(_T("CBCGPGanttControlEx")), _T(""), dwStyle, rect, pParentWnd, nID);

	return bResult && m_pGrid != NULL && m_pChart != NULL;
}

int CBCGPGanttControlEx::CreateControls()
{
	if (m_wndSplitter.GetSafeHwnd() != NULL)
	{
		return -1;
	}

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		ASSERT(FALSE);
		return -1;
	}

	m_wndSplitter.ModifyStyle(WS_HSCROLL | WS_VSCROLL, 0);

	CRect rectDummy(0, 0, 1, 1);

	if (!m_pGrid->Create(WS_VISIBLE | WS_CHILD, rectDummy, &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 0)))
	{
		TRACE0("Failed to create grid control\n");
		ASSERT(FALSE);
		return -1;
	}

	if (!m_pChart->Create(WS_VISIBLE | WS_CHILD, rectDummy, &m_wndSplitter, m_wndSplitter.IdFromRowCol(0, 1)))
	{
		TRACE0("Failed to create chart control\n");
		ASSERT(FALSE);
		return -1;
	}

	CRect rect;
	GetClientRect(rect);

	m_wndSplitter.SetColumnInfo(0, rect.Width() / 3, 30);
	m_wndSplitter.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOREDRAW);

	//设置CBCGPGanttGrid属性
	m_pGrid->SetFont(NULL);
	m_pGrid->SetRowMarker(FALSE);
	m_pGrid->SetSelectionBorder(TRUE);
	
	m_pGrid->EnableMarkSortedColumn(FALSE, TRUE);
	//m_pGrid->EnableHeader(TRUE); //操作表头

	m_pGrid->EnableRowHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_SELECT, 40);//设置序号列
	//m_pGrid->EnableRowHeader(TRUE);
	m_pGrid->EnableLineNumbers(); // 显示表格序号
	
	m_pGrid->SetClearInplaceEditOnEnter(FALSE); // 编辑时清空内容
	m_pGrid->EnableInvertSelOnCtrl(); // 按住Ctrl 进行反选
	//m_pGrid->SetScalingRange(0.1, 4.0); // 设置可缩放
	m_pGrid->EnableTreeButtons(TRUE); // 显示ExpandBox
	m_pGrid->EnableTreeLines(FALSE); // 显示

	// 自定义扩展风格 
	auto pGridEx = dynamic_cast<CBCGPGanttGridEx*>(m_pGrid);
	pGridEx->SetFontSizeScale(1.2f); // 设置字体大小
	pGridEx->SetBoldFontByHasChild(true); // 表头字体加粗
	pGridEx->SetDragDropRows(); // 设置拖拽

	//设置BCGPGanttChartEx
	m_pChart->SetHeaderHeights(22, 20);// 设置表头的行高
	m_pChart->SetDefaultItemHeight(globalUtils.ScaleByDPI(29));// 设置表格的行高 与 CBCGPGanttGrid的行高对应
	dynamic_cast<CBCGPGanttChartEx*>(m_pChart)->SetChartColor(RGB(116, 204, 212));//设置Gantt  Item的颜色
	UpdateGrid();

	return 0;
}

CBCGPGanttGrid* CBCGPGanttControlEx::CreateGrid()
{
	CBCGPGanttGridEx* pNewGrid = NULL;

	if (m_pParentWnd->GetSafeHwnd() != NULL)
	{
		pNewGrid = (CBCGPGanttGridEx*)m_pParentWnd->SendMessage(BCGM_GANTT_CONTROL_CREATE_GRID, 0, 0);
	}

	if (pNewGrid != NULL)
	{
		ASSERT_VALID(pNewGrid);
		return pNewGrid;
	}
	else
	{
		return new CBCGPGanttGridEx;
	}
}

CBCGPGanttChart* CBCGPGanttControlEx::CreateChart()
{
	CBCGPGanttChartEx* pNewChart = NULL;

	if (m_pParentWnd->GetSafeHwnd() != NULL)
	{
		pNewChart = (CBCGPGanttChartEx*)m_pParentWnd->SendMessage(BCGM_GANTT_CONTROL_CREATE_CHART, 0, 0);
	}

	if (pNewChart != NULL)
	{
		ASSERT_VALID(pNewChart);
		return pNewChart;
	}
	else
	{
		return new CBCGPGanttChartEx;
	}
}

void CBCGPGanttControlEx::OnGridItemChanged(CBCGPGridItem* pGridItem, int nRow, int nColumn)
{
	if (nRow < 0 || nColumn < 0 || pGridItem == NULL)
	{
		return;
	}
	CBCGPGanttItem* pItem = ItemFromRow(nRow);

	if (pItem != NULL)
	{
		ASSERT_VALID(pItem);

		_variant_t v = pGridItem->GetValue();
		bool bIsDate = v.vt == VT_DATE;

// 		if (nColumn == m_nItemNameColumn)
// 		{
// 			CString sName = (LPCTSTR)(_bstr_t)pGridItem->GetValue();
// 			pItem->SetName(sName);
// 		}
// 		else if (nColumn == m_nItemStartDateColumn && bIsDate)
// 		{
// 			COleDateTime dt = COleDateTime((double)v);
// 			pItem->SetInterval(dt, pItem->GetFinishTime());
// 		}
// 		else if (nColumn == m_nItemFinishDateColumn && bIsDate)
// 		{
// 			COleDateTime dt = COleDateTime((double)v);
// 			pItem->SetInterval(pItem->GetStartTime(), dt);
// 		}
// 		else if (nColumn == m_nItemProgressColumn)
// 		{
// 			double dProgress = (double)v;
// 			pItem->SetProgress(dProgress);
//		}

		CWnd* pParent = GetParent();
		if (pParent->GetSafeHwnd() != NULL)
		{
// 			BCGP_GANTT_CONTROL_ROW rowInfo;
// 			rowInfo.pGridRow = pGridItem->GetParentRow();
// 			rowInfo.pItem = pItem;
			pParent->SendMessage(BCGM_GRID_ITEM_CHANDE, nColumn, (LPARAM)pGridItem);
		}
	}
}

struct GridGroupEntry
{
	int             iIndent;
	CBCGPGridRow*   pParentRow;
};

void CBCGPGanttControlEx::UpdateGridEx()
{
	//记录所有row的折叠状态
	int nRowCount = m_pChart->GetItemCount();
	std::map<int,BOOL> mapRowIsExpanded;
	CBCGPGanttItemStorageBase* pStorageBase = m_pChart->GetStorage();
	
	for (int i = 0; i < nRowCount; i++)
	{
		CBCGPGanttItem* pChartItem = m_pChart->GetItemByIndex(i);
		if(nullptr == pChartItem)
			continue;
		if(pChartItem->IsGroupItem())
		{
			//如果pChartItem下一个item不可见，则说明pChartItem为折叠状态
		
			if (m_pChart->GetItemCount() <= (i + 1))
				break;
			CBCGPGanttItem* pNextItem = m_pChart->GetItemByIndex(i + 1);
			if (nullptr == pNextItem)
				continue;
			if(pNextItem->GetHierarchyLevel() <= pChartItem->GetHierarchyLevel())
				continue;

			BOOL bVisible = pNextItem->IsVisible();
			int nIndexOf = pStorageBase->IndexOf(pChartItem);
			mapRowIsExpanded[nIndexOf] = bVisible;
		}
	}
	
	m_pGrid->SetRedraw(FALSE);
	m_pGrid->RemoveAll();
	
	CBCGPGanttItemStorageBase* pStorage = m_pChart->GetStorage();
	
	if (pStorage == NULL)
	{
		return;
	}
	

	int nColumns = m_pGrid->GetColumnCount();
	POSITION pos = pStorage->GetHeadPosition();

	CList <GridGroupEntry, GridGroupEntry&> stack;
	int iLevel = 0;
	CBCGPGridRow* pParentRow = NULL;
	CBCGPGridRow* pPrevRow = NULL;
	CBCGPGanttItem* pPrevItem = NULL;

	while (pos != NULL)
	{
		CBCGPGanttItem* pItem = pStorage->GetNext(pos);
		ASSERT_VALID(pItem);

		BOOL bBeginGroup = FALSE;

		int iItemLevel = pItem->GetHierarchyLevel();

		if (iItemLevel > iLevel && pPrevItem != NULL && pPrevItem->IsGroupItem()) // push
		{
			GridGroupEntry entry;
			entry.iIndent = iLevel;
			entry.pParentRow = pParentRow;
			stack.AddTail(entry);

			iLevel = iItemLevel;
			bBeginGroup = TRUE;
		}
		else if (iItemLevel < iLevel) // pop
		{
			while (iItemLevel < iLevel && !stack.IsEmpty())
			{
				GridGroupEntry entry = stack.GetTail();
				if (iItemLevel <= entry.iIndent)
				{
					stack.RemoveTail();
					iLevel = entry.iIndent;
					pParentRow = entry.pParentRow;
				}
			}
		}

		pPrevItem = pItem;

		CBCGPGridRow* pRow = m_pGrid->CreateRow(nColumns);
		ASSERT_VALID(pRow);

		UpdateGridItem(pItem, pRow);

		if (bBeginGroup)
		{
			pParentRow = pPrevRow;
		}

		if (pParentRow == NULL)
		{
			m_pGrid->AddRow(pRow);
		}
		else
		{
			pParentRow->AllowSubItems(TRUE);
			pParentRow->AddSubItem(pRow);
		}

		pPrevRow = pRow;
	}
	
	std::map<int, BOOL>::iterator it = mapRowIsExpanded.begin();
	for(; it != mapRowIsExpanded.end();++it)
	{
		const int nItemId = it->first;
		CBCGPGridRow* pRow = m_pGrid->GetRow(nItemId);
		if(nullptr == pRow)
			continue;
		if(pRow->IsGroup())
		{
			DoExpandGroup(m_pGrid, pRow, it->second);
			pRow->Expand(it->second);
			
		}
	}
	
	m_pGrid->SetRedraw(TRUE);
}
