#include "stdafx.h"
#include "BwGanttControl.h"
#include "BwGanttGrid.h"

#define IDD_BW_GRIDPANEL	14753

BEGIN_MESSAGE_MAP(CBwGanttControl, CBCGPGanttControl)
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_GANTT_STORAGE_CHANGED, OnNotifyStorageChanged)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

struct GridGroupEntry
{
	int             iIndent;
	CBCGPGridRow*   pParentRow;
};

enum
{
	eStateNormal,
	eStateProcessingGridNotification,
	eStateProcessingChartNotification
};

CBwGanttControl::CBwGanttControl()
	: _eTimeDimension(CBwGanttChart::eDate)
{
}
//*****************************************************************************
CBwGanttControl::~CBwGanttControl()
{
	
}

BOOL CBwGanttControl::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID)
{
	m_pParentWnd = pParentWnd;
	// m_pGrid = CreateGanttGrid();
	m_pChart = CreateGanttChart();
	BOOL bResult = CWnd::Create(globalData.RegisterWindowClass(_T("BwGanttControl")), _T(""), dwStyle, rect, pParentWnd, nID);

	AdjustChartHeadHeight();

	return bResult && m_pGrid != NULL && m_pChart != NULL;
}

CBwGanttGrid * CBwGanttControl::CreateGanttGrid()
{
	CBwGanttGrid* pNewGrid = NULL;

	if (m_pParentWnd->GetSafeHwnd() != NULL)
	{
		pNewGrid = (CBwGanttGrid*)m_pParentWnd->SendMessage(BCGM_GANTT_CONTROL_CREATE_GRID, 0, 0);
	}

	if (pNewGrid != NULL)
	{
		ASSERT_VALID(pNewGrid);
		return pNewGrid;
	}
	else
	{
		return new CBwGanttGrid;
	}
}

CBwGanttChart * CBwGanttControl::CreateGanttChart()
{
	CBwGanttChart* pNewChart = NULL;

	if (m_pParentWnd->GetSafeHwnd() != NULL)
	{
		pNewChart = (CBwGanttChart*)m_pParentWnd->SendMessage(BCGM_GANTT_CONTROL_CREATE_CHART, 0, 0);
	}

	if (pNewChart != NULL)
	{
		ASSERT_VALID(pNewChart);
		return pNewChart;
	}
	else
	{
		CBwGanttChart* pNewChart = new CBwGanttChart;
		pNewChart->SetControl(this);
		return pNewChart;
	}
}

int CBwGanttControl::CreateControls()
{
	INIT_RESHANDLE();

	if (m_wndSplitter.GetSafeHwnd() != NULL)
	{
		return -1;
	}

	CRect rcWnd;
	GetClientRect(&rcWnd);

	DWORD styleSplitter = WS_CHILD | WS_VISIBLE | WM_NOTIFY | WS_CLIPCHILDREN | WS_CLIPSIBLINGS/* | WS_BORDER*/;

	if (!m_wndSplitter.Create(rcWnd, this, 0x663322, true, styleSplitter))
	{
		ASSERT(FALSE);
		return -1;
	}

	m_wndSplitter.ModifyStyle(WS_HSCROLL | WS_VSCROLL, NULL);

	CRect rectDummy(0, 0, 1, 1);

	// 创建Grid视图
	CRect rcSplitter;
	m_wndSplitter.GetClientRect(&rcSplitter);
	_gridPanel.Create(&m_wndSplitter, rcSplitter, IDD_BW_GRIDPANEL);
	_gridPanel.SetControl(this);
	_gridPanel.ShowWindow(SW_SHOW);

	// 绑定甘特图控制器和Grid
	m_pGrid = _gridPanel.GetGrid();
	CBwGanttGrid *pGrid = dynamic_cast<CBwGanttGrid *>(m_pGrid);
	if (pGrid == NULL)
		return 0;

	// 设置Owner为控制器的父窗口，使可以向外发送消息
	CWnd *pParWnd = GetParent();
	if (pParWnd)
	{
		_gridPanel.SetOwner(pParWnd);
	}

	// 创建Chart视图
	if (!m_pChart->Create(WS_VISIBLE | WS_CHILD | WM_NOTIFY | WS_BORDER, rectDummy, &m_wndSplitter, 0x663324))
	{
		TRACE0("Failed to create chart control\n");
		ASSERT(FALSE);
		return -1;
	}

	CBwGanttChart *pChart = dynamic_cast<CBwGanttChart *>(m_pChart);
	if (pChart != NULL)
	{
		// 设置最小间隔
		pChart->SetTimeDelta(0.04);
		if (_eTimeDimension == CBwGanttChart::eValue)
		{
			pChart->SetTimeDelta(0.053); // 60

			// 设置滚动条初始位置
			pChart->SetHSrollPos(1 - COleDateTimeSpan(1, 0, 0, 0));
		}
		else if(_eTimeDimension == CBwGanttChart::eDate)
		{
			// 设置滚动条初始位置
			pChart->SetHSrollPos(COleDateTime::GetCurrentTime() - COleDateTimeSpan(1, 0, 0, 0));
		}
	}

	// 设置分隔条面板
	SetSplitterWidth(8);
	m_wndSplitter.SetSplitterPanes(_gridPanel.GetSafeHwnd(), m_pChart->GetSafeHwnd());
	SetSplitterPosPct(69);
	
	pGrid->SetFont(NULL);
	pGrid->SetRowMarker(FALSE);
	pGrid->SetSelectionBorder(FALSE);

	UpdateGridEx();

	return 0;
}

CBwSplitCtrl * CBwGanttControl::GetSplitter()
{
	return &m_wndSplitter;
}

void CBwGanttControl::UpdateGridEx(bool bRedraw)
{
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

	_gridPanel.SelectRow();

	m_pGrid->SetRedraw(TRUE);

	if(bRedraw)
		m_pGrid->RedrawWindow();

	ModifyAllRowEditState();			// 更新所有组的编辑状态
}

void CBwGanttControl::ModifyAllRowEditState()
{
	if (m_pGrid == nullptr || m_pGrid->GetSafeHwnd() == nullptr)
		return;

	int nRowCount = m_pGrid->GetRowCount();
	for (int i = 0; i < nRowCount; i++)
		ModifyRowEditState(m_pGrid->GetRow(i));
}

void CBwGanttControl::ModifyRowEditState(CBCGPGridRow * pRow)
{
	if (pRow == nullptr || pRow->GetItemCount() < 8)
		return;

	bool bAllowEdit = pRow->GetSubItemsCount() == 0;		// 是否是叶子节点,非叶子节点不允许编辑

	// 已设置不重复进行设置
	if (bAllowEdit == pRow->GetItem(1)->IsAllowEdit())
		return;

	pRow->GetItem(1)->AllowEdit(bAllowEdit);			// 计划持续时间
	pRow->GetItem(2)->AllowEdit(bAllowEdit);			// 计划开始时间
	pRow->GetItem(3)->AllowEdit(bAllowEdit);			//	计划结束时间
	pRow->GetItem(4)->AllowEdit(bAllowEdit);			//	实际持续时间
	pRow->GetItem(5)->AllowEdit(bAllowEdit);			//	实际开始时间
	pRow->GetItem(6)->AllowEdit(bAllowEdit);			//	实际结束时间
	pRow->GetItem(7)->AllowEdit(bAllowEdit);			//	前置任务
}

void CBwGanttControl::OnSize(UINT nType, int cx, int cy)
{
	CBCGPWnd::OnSize(nType, cx, cy);

	if (m_wndSplitter.GetSafeHwnd() != NULL)
	{
		CRect rcWnd;
		GetClientRect(&rcWnd);
		m_wndSplitter.MoveWindow(&rcWnd);
	}
}

void CBwGanttControl::SetCurTimeLineColor(COLORREF clr)
{
	CBwGanttChart *pChart = dynamic_cast<CBwGanttChart *>(m_pChart);
	if (pChart == NULL)
		return;
	pChart->SetCurTimeLineColor(clr);
}

void CBwGanttControl::SetCurTime(const COleDateTime &curTime)
{
	CBwGanttChart *pChart = dynamic_cast<CBwGanttChart *>(GetChart());
	if (pChart == NULL)
		return;

	pChart->SetCurTime(curTime);

	// 更新pChart绘制
	// pChart->PostMessage(WM_PAINT, 0, 0);
	pChart->Invalidate();
}

void CBwGanttControl::SetRoot(KNode * pRoot)
{
	_gridPanel.SetRoot(pRoot);
	UpdateScrollPos();
}

void CBwGanttControl::LoadFromNode()
{
	if (_gridPanel)
		_gridPanel.LoadFromNode(true);
}


void CBwGanttControl::EnableButtons(bool bEnable)
{
	if (_gridPanel)
	{
		_gridPanel.EnableButtons(bEnable);
	}
}

void CBwGanttControl::GetDateRange(COleDateTime & startTime, COleDateTime & endTime)
{
	if (_gridPanel)
	{
		_gridPanel.GetDateRange(startTime, endTime);
	}
}

void CBwGanttControl::SetToolBarImageList(CImageList * pImageList)
{
	_gridPanel.SetImageList(pImageList);
}

void CBwGanttControl::ShowToolbar(bool bShow)
{
	_gridPanel.ShowToolbar(bShow);
}

void CBwGanttControl::SetSplitterWidth(int nWidth)
{
	m_wndSplitter.SetWidth(nWidth);
}

void CBwGanttControl::SetSplitterPosPct(int nPos)
{
	m_wndSplitter.SetSplitterPosPct(nPos);
}

void CBwGanttControl::SetSingleProgressBar(bool bSingle)
{
	CBwGanttChart *pChart = dynamic_cast<CBwGanttChart *>(GetChart());
	if (pChart == NULL)
		return;

	pChart->SetSingleProgressBar(bSingle);
}

void CBwGanttControl::SetTimeDimension(CBwGanttChart::ETimeDimension td)
{
	_eTimeDimension = td;
}

CBwGanttChart::ETimeDimension CBwGanttControl::GetTimeDimension()
{
	return _eTimeDimension;
}

void CBwGanttControl::UpdateScrollPos()
{
	CBwGanttChart *pChart = dynamic_cast<CBwGanttChart *>(GetChart());
	if (pChart == NULL)
		return;
	auto pRoot = _gridPanel.GetRoot();
	if (!pRoot)
		return;
	if (pRoot->GetNodeList().size() > 0)
	{
		// 设置滚动条初始位置
		COleDateTime stTime, edTime;
		GetDateRange(stTime, edTime);

		if (_eTimeDimension == CBwGanttChart::eDate)
		{
			pChart->SetHSrollPos(stTime - COleDateTimeSpan(1, 0, 0, 0));
		}
		else if (_eTimeDimension == CBwGanttChart::eValue)
		{
			if (stTime < 0)
				stTime = 0.0;
			pChart->SetHSrollPos(stTime - COleDateTimeSpan(0, 0, 0, 0));
		}
	}
	else
	{
		if (_eTimeDimension == CBwGanttChart::eValue)
		{
			pChart->SetHSrollPos(1 - COleDateTimeSpan(1, 0, 0, 0));
		}
	}
}

CBwGanttControl::BW_GANTT_INSERT_RESULT CBwGanttControl::InsertNode(KNode *pNode, bool bAddChild)
{
	CBwGanttControl::BW_GANTT_INSERT_RESULT result;
	CGanttNode prgNode = pNode;
	auto pChart = dynamic_cast<CBwGanttChart *>(GetChart());
	auto pGrid = dynamic_cast<CBwGanttGrid *>(GetGrid());
	if (prgNode.IsNull() || !pChart || !pGrid)
		return result;

	int nSel = pGrid->GetSelIndex();

	// 获取选择节点
	CBwGanttItem *pSelGanttItem = pChart->GetSelItem();
	CBCGPGridRow *pSelGridRow = pGrid->GetRow(nSel);

	int nInsertRow = pGrid->GetRowCount();
	int nParentIndex = 0;
	
	// 获取父节点
	CBwGanttItem *pParentGanttItem = pChart->GetSelItem();
	CBCGPGridRow *pParentGridRow = NULL;
	KNode *pParent = pParentGanttItem ? pParentGanttItem->GetNode() : NULL;
	if (!bAddChild && pParent)
	{
		pParent = pParent->GetParent();
	}
	pParent = pParent ? pParent : _gridPanel.GetRoot();

	auto ganttItemInfo = GetGanttItemByKNode(pParent);
	if (ganttItemInfo.IsExist())
	{
		pParentGanttItem = ganttItemInfo._pGanttItem;
		nParentIndex = ganttItemInfo._nIndex;
		pParentGridRow = pGrid->GetRow(nParentIndex);
	}
	if (!pParent)
		return result;

	// 通过KNode创建GanttItem
	CBwGanttItem *pItem = CreateGanttItemByKNode(pParent, pNode);
	if (pItem == NULL)
		return result;

	// 获取插入节点相对父节点的下标
	auto nodeList = pParent->GetNodeList();
	int nIndex = nodeList.size();
	if (pSelGanttItem)
	{
		int index = 0;
		for (auto &node : nodeList)
		{
			if (node == pSelGanttItem->GetNode())
			{
				nIndex = index + 1;
				break;
			}
			++index;
		}
	}

	// 获取插入行下标
	if (pSelGridRow)
	{
		if (bAddChild)
		{
			auto lastSubItem = pSelGridRow->GetSubItem(pSelGridRow->GetSubItemsCount() - 1);
			int nLastSubItemChilds = lastSubItem ? lastSubItem->GetSubItemsCount(TRUE) : 0;
			nInsertRow = nSel + pSelGridRow->GetSubItemsCount(TRUE) - nLastSubItemChilds + 1;
		}
		else
		{
			nInsertRow = nSel + pSelGridRow->GetSubItemsCount(TRUE) + 1;
		}
	}

	pChart->InsertItem(nInsertRow, pItem);
	pParent->InsertNode(pNode, nIndex);

	// 刷新Grid控件
	UpdateGridEx();

	result._bRes = true;
	result._nRow = nInsertRow;
	return result;
}

CBwGanttControl::BW_GANTT_DELETE_RESULT CBwGanttControl::DeleteNode(KNode * pNode)
{
	CBwGanttControl::BW_GANTT_DELETE_RESULT result;
	auto pChart = GetChart();
	if (!pNode || !pChart)
		return result;
	return result;
}

CBwGanttChart::CBwGanttItemInfo CBwGanttControl::GetGanttItemByKNode(KNode * pNode)
{
	CBwGanttChart::CBwGanttItemInfo ganttItemInfo;

	auto pChart = GetChart();
	if (!pNode || !pChart)
		return ganttItemInfo;
	int nCnt = pChart->GetItemCount();
	for (int i = 0; i < nCnt; ++i)
	{
		auto pGanttItem = dynamic_cast<CBwGanttItem *>(pChart->GetItemByIndex(i));
		if (pGanttItem == NULL)
			continue;
		if (pGanttItem->GetNode() == pNode)
		{
			ganttItemInfo._nIndex = i;
			ganttItemInfo._pGanttItem = pGanttItem;
			return ganttItemInfo;
		}
	}
	return ganttItemInfo;
}

CBwGanttItem * CBwGanttControl::CreateGanttItemByKNode(KNode *pParent, KNode * pNode)
{
	CGanttNode prgNode = pNode;
	auto pParentGanttItem = GetGanttItemByKNode(pParent)._pGanttItem;
	if (prgNode.IsNull())
		return NULL;

	auto pItem = new CBwGanttItem;

	// 绑定KNode节点
	pItem->SetNode(pNode);

	CString sName = prgNode.GetName();
	pItem->SetName(sName);

	// 设置级别
	pItem->SetHierarchyLevel(pParentGanttItem ? pParentGanttItem->GetHierarchyLevel() + 1 : 0);

	// 设置开始结束时间
	COleDateTime startTime = prgNode.GetPlanStartTime();
	COleDateTime endTime = prgNode.GetPlanEndTime();
	COleDateTime realStartTime = prgNode.GetRealStartTime();
	COleDateTime realEndTime = prgNode.GetRealEndTime();

	pItem->SetInterval(startTime, endTime);
	pItem->SetRealInterval(realStartTime, realEndTime);

	// 设置进度为100
	pItem->SetProgress(100);
	pItem->SetRealProgress(100);

	// 设置进度条颜色
	prgNode.SetPlanFinishColor(RGB(72, 212, 20));

	int stOffset = realStartTime - startTime;
	int edOffset = realEndTime - endTime;

	if (stOffset == 0 && edOffset == 0)
	{
		// 实际进度与计划进度一致
		prgNode.SetRealFinishColor(RGB(206, 212, 20));
	}
	else
	{
		// 实际进度与计划进度不一致
		prgNode.SetRealFinishColor(RGB(206, 120, 120));
	}
	pItem->SetCompleteColor(prgNode.GetPlanFinishColor());
	COLORREF colPlan = pItem->GetCompleteColor();
	pItem->SetRealCompleteColor(prgNode.GetRealFinishColor());
	COLORREF colReal = pItem->GetRealCompleteColor();

	// 设置生长方式
	CString strGrowthPattern = prgNode.GetGrowthPatternString();
	pItem->SetGrowthPattern(strGrowthPattern);

	// 设置组节点
	if (pParentGanttItem)
		pParentGanttItem->SetGroupItem(TRUE);

	return pItem;
}

BOOL CBwGanttControl::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	else if (WM_USER_SORTCOLUM == pMsg->message)
	{
		::PostMessage(GetParent()->GetSafeHwnd(), WM_USER_SORTCOLUM, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

LRESULT CBwGanttControl::OnNotifyStorageChanged(WPARAM, LPARAM lParam)
{
	BCGP_GANTT_STORAGE_UPDATE_INFO* pUpdate = (BCGP_GANTT_STORAGE_UPDATE_INFO*)lParam;
	if (pUpdate != NULL)
	{
		DoItemsChange(GetChart(), *pUpdate);
	}
	return 0L;
}

void CBwGanttControl::DoItemsChange(CWnd * pSender, const BCGP_GANTT_STORAGE_UPDATE_INFO & updateInfo)
{
	if (!StartProcessNotification(pSender))
	{
		return;
	}

	if (IsProcessingChartNotification()) // Chart -> Grid only supported now
	{
		// for single item updates only
		CBCGPGanttItem* pItem = updateInfo.pItem;
		CBCGPGridRow* pRow = NULL;
		int nRow = -1;

		if (updateInfo.iFirstItemIndex == updateInfo.iLastItemIndex)
		{
			nRow = updateInfo.iFirstItemIndex;
			if (nRow >= 0)
			{
				pRow = m_pGrid->GetRow(nRow);
			}
		}

		switch (updateInfo.uiAction)
		{
		case BCGP_GANTT_STORAGE_INSERT_ITEM:
			if (pItem != NULL) // inserting single item only supported
			{
				ASSERT_VALID(pItem);

				CBCGPGanttItemStorageBase* pStorage = m_pChart->GetStorage();
				ASSERT_VALID(pStorage);

				CBCGPGanttItem* pParentItem = pStorage->GetParentGroupItem(pItem);
				int nParentRow = RowFromItem(pParentItem);

				pRow = m_pGrid->CreateRow(m_pGrid->GetColumnCount());

				if (nParentRow >= 0)
				{
					CBCGPGridRow* pParentRow = m_pGrid->GetRow(nParentRow);
					ASSERT_VALID(pParentRow);
					m_pGrid->InsertRowAfter(pParentRow, nRow - nParentRow - 1, pRow);
				}
				else
				{
					m_pGrid->InsertRowAfter(NULL, nRow - 1, pRow);
				}

				UpdateGridItem(pItem, pRow);
			}
			break;

		case BCGP_GANTT_STORAGE_UPDATE_ITEM:
		{
			bool bNeedUpdate = (updateInfo.dwFlags & ~BCGP_GANTT_ITEM_PROP_SELECTED) != BCGP_GANTT_ITEM_PROP_NONE;

			if (bNeedUpdate)
			{
				if (pItem != NULL)
				{
					if (pRow != NULL)
					{
						UpdateGridItem(pItem, pRow);
					}
				}
				else // multiple items
				{
					UpdateGridEx();
				}
			}
		}
		break;

		case BCGP_GANTT_STORAGE_REMOVED_ITEM:
		{
			int nRemovedCount = 0;
			if (pItem == NULL) // multiple items
			{
				int i = updateInfo.iLastItemIndex;
				while (i >= updateInfo.iFirstItemIndex)
				{
					nRemovedCount = m_pGrid->RemoveRow(i, FALSE);
					nRemovedCount = max(1, nRemovedCount);
					i -= nRemovedCount;
				}

				m_pGrid->AdjustLayout();
			}
			else // single item
			{
				nRemovedCount = m_pGrid->RemoveRow(nRow, TRUE);
				ASSERT(nRemovedCount <= 1); // If it is not, the grid row had subitems that were also removed.
			}
		}
		break;
		}
	}

	FinishProcessNotification();
}

void CBwGanttControl::UpdateGridItem(CBCGPGanttItem * pItem, CBCGPGridRow * pGridRow)
{
	auto pGrid = _gridPanel.GetGrid();
	auto pGanttItem = dynamic_cast<CBwGanttItem *>(pItem);
	KNode *pNode = (pGanttItem ? pGanttItem->GetNode() : NULL);
	if (!pGrid || !pGanttItem || !pNode)
		return;
	ASSERT_VALID(pGridRow);

	int nColumns = pGrid->GetColumnCount();

	for (int i = 0; i < nColumns; ++i)
	{
		CBwGanttGrid::EDataType dataType = pGrid->GetColumnFormats()[i]._editInPlaceFlags;
		CString strField =  pGrid->GetColumnFormats()[i]._dataFieldName;
		CBCGPGridItem* pGridItem = pGridRow->GetItem(i);
		if (pGridItem == NULL)
			continue;

		if (!pNode->HasData(strField))
		{
			pGridItem->EmptyValue();
			continue;
		}

		CGanttNode progressNode(pNode);
		if (progressNode.IsNull())
			return;

		// 更新数据
		switch (dataType)
		{
		case CBwGanttGrid::eString:
			if (strField == COL_OPERATION)
			{
				auto patternItem = dynamic_cast<CBwGrowthPatternItem *>(pGridItem);
				if (!progressNode.IsNull() && patternItem != NULL )
				{
					int nModelNum = progressNode.GetModelNum();
					patternItem->FreezeItem(nModelNum == 0);
					pGridItem->SetValue(variant_t(_T("解绑")));
				}
				break;
			}
			else if (strField == COL_GROWTH_PATTERN)
			{
				CString strGrowthPattern = progressNode.GetGrowthPatternString();
				pGridItem->SetValue(pGanttItem->IsGroupItem() ? _T("") : (LPCTSTR)strGrowthPattern);
				break;
			}
			pGridItem->SetValue((LPCTSTR)pNode->GetData(strField).GetStringValue());
			break;
		case CBwGanttGrid::eDate:
			pGridItem->SetValue(_variant_t((COleDateTime)(double)pNode->GetData(strField), VT_DATE));
			break;
		case CBwGanttGrid::eInt:
			pGridItem->SetValue((long)pNode->GetData(strField));
			break;
		case CBwGanttGrid::eDouble:
			pGridItem->SetValue((double)pNode->GetData(strField));
			break;
		case CBwGanttGrid::eBool:
			pGridItem->SetValue(((bool)pNode->GetData(strField)) ? _T("是") : _T("否"));
			break;
		default:
			break;
		}
		
	}

	CWnd* pParent = GetParent();
	if (pParent->GetSafeHwnd() != NULL)
	{
		BCGP_GANTT_CONTROL_ROW rowInfo;
		rowInfo.pGridRow = pGridRow;
		rowInfo.pItem = pItem;
		pParent->SendMessage(BCGM_GANTT_CONTROL_WRITE_ITEM_DATA_TO_GRID, 0, (LPARAM)&rowInfo);
	}

}

void CBwGanttControl::UpdateGridItem(KNode * pNode)
{
	auto pGanttItemInfo = GetGanttItemByKNode(pNode);
	auto pGrid = GetGrid();
	if (!pGanttItemInfo.IsExist() || !pGrid)
		return;
	auto pGridRow = pGrid->GetRow(pGanttItemInfo._nIndex);
	UpdateGridItem(pGanttItemInfo._pGanttItem, pGridRow);
}

void CBwGanttControl::OnGridItemChanged(CBCGPGridItem * pGridItem, int nRow, int nColumn)
{
	if (nRow < 0 || nColumn < 0 || pGridItem == NULL)
	{
		return;
	}

	auto pGrid = _gridPanel.GetGrid();
	if (!pGrid)
		return;

	// 获取列下表
	int nPlanDuringTimeIndex = pGrid->GetColIndexByName(COL_PLAN_DURINGTIME);
	int nRealDuringTimeIndex = pGrid->GetColIndexByName(COL_REAL_DURINGTIME);
	int nRealStartTimeIndex = pGrid->GetColIndexByName(COL_REAL_STARTTIME);
	int nRealFinishTimeIndex = pGrid->GetColIndexByName(COL_REAL_ENDTIME);

	CBCGPGanttItem *pGanttItem = ItemFromRow(nRow);
	CBwGanttItem *pItem = dynamic_cast<CBwGanttItem *>(pGanttItem);
	KNode *pNode = (pItem ? pItem->GetNode() : NULL);
	if (pItem == NULL || pNode == NULL)
	{
		return;
	}

	ASSERT_VALID(pItem);

	_variant_t v = pGridItem->GetValue();
	
	CBwGanttGrid::ColumnFormat colFormat = pGrid->GetColumnFormats()[pGridItem->GetColumnId()];
	CString strField = colFormat._dataFieldName;

	if(!_gridPanel.OnEndEditGrid(pGridItem, pNode, colFormat))
		return;

	bool bIsDate = v.vt == VT_DATE;

	// 特殊属性处理
	if (strField == COL_NAME)
	{
		CString sName = (LPCTSTR)(_bstr_t)pGridItem->GetValue();
		CGanttNode prgNode = pNode;
		prgNode.SetName(sName);
		pItem->SetName(sName);
	}
	else if (strField == COL_PLAN_STARTTIME && bIsDate)
	{
		COleDateTime dt = COleDateTime(v);
		CGanttNode prgNode = pNode;
		if (!prgNode.IsNull())
		{
			prgNode.SetPlanStartTime(v.date);
			pItem->SetInterval(prgNode.GetPlanStartTime(), prgNode.GetPlanEndTime());
		}
	}
	else if (strField == COL_PLAN_ENDTIME && bIsDate)
	{
		COleDateTime dt = COleDateTime(v);
		CGanttNode prgNode = pNode;
		if (!prgNode.IsNull())
		{
			prgNode.SetPlanEndTime(v.date);
			pItem->SetInterval(prgNode.GetPlanStartTime(), prgNode.GetPlanEndTime());
		}
	}
	else if (strField == COL_REAL_STARTTIME && bIsDate)
	{
		COleDateTime dt = COleDateTime(v);
		CGanttNode prgNode = pNode;
		if (!prgNode.IsNull())
		{
			prgNode.SetRealStartTime(v.date);
			pItem->SetRealInterval(prgNode.GetRealStartTime(), prgNode.GetRealEndTime());
		}
	}
	else if (strField == COL_REAL_ENDTIME && bIsDate)
	{
		COleDateTime dt = COleDateTime(v);
		CGanttNode prgNode = pNode;
		if (!prgNode.IsNull())
		{
			prgNode.SetRealEndTime(v.date);
			pItem->SetRealInterval(prgNode.GetRealStartTime(), prgNode.GetRealEndTime());
		}
	}
	else if (strField == COL_PLAN_DURINGTIME)
	{
		CGanttNode prgNode = pNode;
		if (!prgNode.IsNull())
		{
			if (v.vt == VT_BSTR)
			{
				long val = ((long)CStringToolkitEx::StrToInt(v.bstrVal));
				pGridItem->ChangeType(VT_I4);
				pGridItem->SetValue(val);
			}
			prgNode.SetPlanDuringTime(v.intVal);
			pItem->SetInterval(prgNode.GetPlanStartTime(), prgNode.GetPlanEndTime());
		}
	}
	else if (strField == COL_REAL_DURINGTIME)
	{
		CGanttNode prgNode = pNode;
		if (!prgNode.IsNull())
		{
			if (v.vt == VT_BSTR)
			{
				long val = ((long)CStringToolkitEx::StrToInt(v.bstrVal));
				pGridItem->ChangeType(VT_I4);
				pGridItem->SetValue(val);
			}
			prgNode.SetRealDuringTime(v.intVal);
			pItem->SetRealInterval(prgNode.GetRealStartTime(), prgNode.GetRealEndTime());
		}
	}
	// 一般属性处理
	else if (!strField.IsEmpty())
	{
		CBwGanttGrid::EDataType dataType = pGrid->GetColumnFormats()[pGridItem->GetColumnId()]._editInPlaceFlags;
		switch (dataType)
		{
		case CBwGanttGrid::eString:
			pNode->SetData(strField, v.bstrVal);
			break;
		case CBwGanttGrid::eDate:
			pNode->SetData(strField, v.date);
			break;
		case CBwGanttGrid::eInt:
			pNode->SetData(strField, v.intVal);
			break;
		case CBwGanttGrid::eDouble:
			pNode->SetData(strField, v.dblVal);
			break;
		case CBwGanttGrid::eBool:
			pNode->SetData(strField, v.boolVal);
			break;
		default:
			break;
		}
	}

	UpdateGridItem(pGanttItem, pGridItem->GetParentRow());
	CWnd* pParent = GetParent();
	if (pParent->GetSafeHwnd() != NULL)
	{
		BCGP_GANTT_CONTROL_ROW rowInfo;
		rowInfo.pGridRow = pGridItem->GetParentRow();
		rowInfo.pItem = pItem;
		pParent->SendMessage(BCGM_GANTT_CONTROL_READ_ITEM_DATA_FROM_GRID, 0, (LPARAM)&rowInfo);
	}

	// 更新进度条颜色
	int planStartTime = pItem->GetStartTime();
	int realStartTime = pItem->GetRealStartTime();
	int planEndTime = pItem->GetFinishTime();
	int realEndTime = pItem->GetRealFinishTime();

	int stOffset = realStartTime - planStartTime;
	int edOffset = realEndTime - planEndTime;
	if (stOffset == 0 && edOffset == 0)
	{
		// 实际进度与计划进度一致
		pItem->SetPrimaryColor(RGB(206, 212, 20));
		pItem->SetRealCompleteColor(RGB(206, 212, 20));
	}
	else
	{
		// 实际进度与计划进度不一致
		pItem->SetRealPrimaryColor(RGB(206, 120, 120));
		pItem->SetRealCompleteColor(RGB(206, 120, 120));
	}

	_gridPanel.OnEndEditGridDone(pGridItem, pNode, colFormat);
}

void CBwGanttControl::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int i = 1;
	CBCGPGanttControl::OnClose();
}

void CBwGanttControl::DoVerticalResize(CWnd* pSender)
{
	AdjustChartHeadHeight();
	if (!StartProcessNotification(pSender))
	{
		return;
	}

	int nFooter = 0;

	if (IsProcessingChartNotification()) // Chart -> Grid
	{
		CRect rectClient;
		m_pChart->GetClientRect(&rectClient);
		CRect rectChart = m_pChart->GetVisibleChartArea();

		m_pGrid->SetVerticalSizes(rectChart.top - rectClient.top - _gridPanel.GetBtnAreaHeight(), m_pChart->GetDefaultItemHeight());
	}
	if (IsProcessingGridNotification()) // Grid -> Chart
	{
		UINT nHeader, nRows;
		m_pGrid->GetVerticalSizes(&nHeader, &nRows);
		nHeader += _gridPanel.GetBtnAreaHeight();
		m_pChart->SetDefaultItemHeight(nRows);

		UINT nChartHeader0_hgt, nChartHeader1_hgt;
		m_pChart->GetHeaderHeights(&nChartHeader0_hgt, &nChartHeader1_hgt);

		if (nHeader > 0 && nFooter > 0) // both header and footer
		{
			m_pChart->SetHeaderHeights(nHeader, nFooter);
			m_pChart->SetHeaderVisible(0, TRUE);
			m_pChart->SetHeaderVisible(1, TRUE);
			m_pChart->SetHeaderAboveChart(0, TRUE);
			m_pChart->SetHeaderAboveChart(1, FALSE);
		}
		else if (nHeader > 0 || nFooter > 0) // either header or footer
		{
			BOOL bH0Visible = m_pChart->GetHeaderVisible(0);
			BOOL bH1Visible = m_pChart->GetHeaderVisible(1);
			int h = (nHeader > 0) ? nHeader : nFooter;

			if (bH0Visible && bH1Visible)
			{
				m_pChart->SetHeaderHeights(h / 2, h - (h / 2));
				m_pChart->SetHeaderAboveChart(0, (nHeader > 0));
				m_pChart->SetHeaderAboveChart(1, (nHeader > 0));
			}
			else if (bH0Visible)
			{
				m_pChart->SetHeaderHeights(h, nChartHeader1_hgt);
				m_pChart->SetHeaderAboveChart(0, (nHeader > 0));
			}
			else if (bH1Visible)
			{
				m_pChart->SetHeaderHeights(nChartHeader0_hgt, h);
				m_pChart->SetHeaderAboveChart(1, (nHeader > 0));
			}
			else
			{
				m_pChart->SetHeaderHeights(h, nChartHeader1_hgt);
				m_pChart->SetHeaderAboveChart(0, (nHeader > 0));
			}
		}
		else // nor header neither footer
		{
			m_pChart->SetHeaderVisible(0, FALSE);
			m_pChart->SetHeaderVisible(1, FALSE);
		}
	}

	FinishProcessNotification();
}

void CBwGanttControl::AdjustChartHeadHeight()
{
	if (m_pChart == NULL)
		return;

	// 调整Chart列头
	UINT nColHeight = _gridPanel.GetBtnAreaHeight();
	UINT nSubColHeight = nColHeight + 20;

	// 如果为数值列头则隐藏主列头
	if (_eTimeDimension == CBwGanttChart::eValue)
	{
		nColHeight = 0;
		m_pChart->SetHeaderVisible(0, FALSE);
	}

	m_pChart->SetHeaderHeights(nColHeight, nSubColHeight);
	AdjustLayout();
}

void CBwGanttControl::OnDestroy()
{
	CBCGPGanttControl::OnDestroy();
	_gridPanel.DestroyWindow();
}
