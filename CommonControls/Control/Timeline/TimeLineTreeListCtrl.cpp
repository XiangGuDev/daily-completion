#include "stdafx.h"
#include "TimeLineTreeListCtrl.h"

BEGIN_MESSAGE_MAP(CTimeLineTreeListCtrl, CTreeListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CTimeLineTreeListCtrl::CTimeLineTreeListCtrl(void)
{
	_showHeader = true;
	_hasGrid = true;
	_hasLine = true;
	_hDragingCol = NULL;
	_bBtnDown = false;
	_bMouseOver = false;
	_bSingleSelect = true;
	_nHeaderHeight = LIST_HEAD_HEIGHT;
	_iconSize = CSize(12, 12);
	_bkColor = GetSysColor(COLOR_WINDOW);
	_pListDataSource = NULL;
	_hHoverCol = NULL;
	_hPressCol = NULL;
	_hHoverRow = NULL;
	_nHoverRowIndex = -1;
	_lockTipUpdate = false;
	_nEditRowIndex = -1;
	_nEditColIndex = -1;
	_treeColumn = 0;
	_selTopColor = RGB(220, 235, 252);
	_selBottomColor = RGB(193, 219, 252);
	_selBorderColor = RGB(170, 200, 245);
	_hotTopColor = RGB(250, 251, 252);
	_hotBottomColor = RGB(233, 243, 253);
	_hotBorderColor = RGB(184, 214, 251);
	_gridColor = RGB(173, 193, 218);
	_bNoChild = true;
	_bDelDataSource = false;
	_pConfig = new CTreeListConfig(this);
	_pControl = NULL;
	_rcLastRect = CRect(0, 0, 0, 0);
	_bAutoColumnWidth = false;
	_nLastSelectItem = -1;
	_bUpDownFlag = false;
	_bLinkOn = false;
	_bColumnWordWarp = false;
	_bRowWordWarp = false;
	_nRowHeight = LIST_ROW_HEIGHT;

	_hIconCollapse = NULL;
	_hIconExpand = NULL;
	_hNofigyWnd = NULL;
	_bColumnSort = false;
	_bIsTrim = true;

	_pTimeLineCtrl = nullptr;
}

ListDataSource CTimeLineTreeListCtrl::GetListDataSource(void)
{
	return _listDataSource;
}

void CTimeLineTreeListCtrl::UpdateScrollBar(int scrollMode)
{
	CRect rcHeader = CalcHeaderRect();

	int nSumColumnWidth = 0;

	// 统计列头宽度
	size_t nCount = _columnVector.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		nSumColumnWidth += _columnVector[i]->GetWidth();
	}

	SCROLLINFO scrInfo;
	scrInfo.cbSize = sizeof(scrInfo);

	scrInfo.fMask = SIF_PAGE | SIF_RANGE;
	scrInfo.nPage = rcHeader.Width();
	scrInfo.nMin = 0;
	scrInfo.nMax = nSumColumnWidth - 1;

	SetScrollInfo(SB_HORZ, &scrInfo);
}

void CTimeLineTreeListCtrl::DrawRow(CMemoryDC& dcMemory, const CRect& rcHeader, const CRect& rcList)
{
	// 没有列头
	if (0 == _columnVector.size())
		return;

	// 获取列头叶子节点
	ColumnDataVector leafColumnVector;
	GetColumnAllLeafStruct(leafColumnVector);

	// 行递增高度
	int nIncreaseHeight = 0;
	int nVertPos = GetOwner()->GetScrollPos(SB_VERT);

	HGDIOBJ hOldFont = dcMemory.SelectObject(_treeListFont);

	// 记录行
	size_t nRow = 0;

	// 遍历绘制行
	ListDataSourceIter endIter = _listDataSource.end();
	for (ListDataSourceIter iter = _listDataSource.begin();
		iter != endIter; ++iter, ++nRow)
	{
		HITEMDATA hItemData = iter->second;
		HITEMDATA hParentData = hItemData->hParentData;

		// 获取单元格数据
		IListDataSource* pDataSource = hItemData->pDataSource;
		if (pDataSource == NULL)
			return;

		// 所在节点上层已收缩,则不展现
		if (NULL != hParentData && hParentData->IsCollapse())
		{
			hItemData->rowRect.SetRectEmpty();
			continue;
		}

		// 计算行区域
		CRect rcRow;
		rcRow.left = _columnVector[0]->colRect.left;
		rcRow.top = nIncreaseHeight - nVertPos + rcHeader.bottom;
		rcRow.bottom = rcRow.top + _nRowHeight;
		rcRow.right = _columnVector[_columnVector.size() - 1]->colRect.right;

		// 获取每个单元实格信息
		CellConfigArray cellCfgArray;

		int nMaxRowHeight = _nRowHeight;
		size_t nCount = leafColumnVector.size();
		cellCfgArray.resize(nCount);

		// 遍历列头提前获取单元格文本(顺便统计计算行高）
		for (size_t i = 0; i < nCount; ++i)
		{
			HCOLUMNDATA hColumnData = leafColumnVector[i];

			// 获取单元格文本
			CELLCONFIG& cellCfg = cellCfgArray[i];
			hItemData->pDataSource->GetCellData(i, nRow, hColumnData,
				&cellCfg.cellInfo, &cellCfg.editConfig, _pConfig);

			cellCfg.cellText = hItemData->pDataSource->GetCellText(i, nRow, &cellCfg.editConfig, _pConfig);

			// 不计算行高情况
			if (!_bRowWordWarp || cellCfg.cellText.IsEmpty())
				continue;

			CRect rcNeedRect = hColumnData->colRect;
			CRect rcCell(rcNeedRect.left, rcRow.top, rcNeedRect.right, rcRow.bottom);
			CalcCellTextOutRect(&dcMemory, cellCfg.cellText, rcCell, (i == _treeColumn) && (!_bNoChild || hItemData->hParentData != NULL),
				hItemData, &cellCfg.editConfig, hColumnData, rcNeedRect);

			cellCfg.cellInfo.wpHeight = rcNeedRect.Height();
			if (rcNeedRect.Height() <= rcCell.Height())
				continue;

			int nNeedHeight = cellCfg.cellInfo.wpHeight + 8;
			if (nNeedHeight > nMaxRowHeight)
			{
				nMaxRowHeight = nNeedHeight;
			}
		}

		// 赋值最高行高
		if (_bRowWordWarp)
		{
			rcRow.bottom = rcRow.top + nMaxRowHeight;
		}

		// 更新行区域
		hItemData->rowRect = rcRow;
		hItemData->expandRect.SetRectEmpty(); // Fix:b w-136479

											  // 递增行高
		nIncreaseHeight += rcRow.Height();

		// 不在显示区域范围内则不绘制(加快性能)
		if ((rcRow.top < rcList.top && rcRow.bottom < rcList.top)
			|| (rcRow.top > rcList.bottom && rcRow.bottom > rcList.bottom))
		{
			continue;
		}

		// 绘制行背景
		DrawRowBackground(dcMemory, hItemData, rcRow, leafColumnVector, !nRow);

		// 初始化单元格数据
		for (size_t i = 0; i < nCount; ++i)
		{
			HCOLUMNDATA hColumnData = leafColumnVector[i];

			// 计算单元格区域
			CRect rcCell = rcRow;
			rcCell.left = hColumnData->colRect.left;
			rcCell.right = hColumnData->colRect.right;

			// 列头不在显示区域范围内则不绘制(加快性能)
			if ((hColumnData->colRect.left < rcHeader.left && hColumnData->colRect.right < rcHeader.left)
				|| (hColumnData->colRect.left > rcHeader.right && hColumnData->colRect.right > rcHeader.right))
			{
				continue;
			}

			int nSave = dcMemory.SaveDC();

			// 设置剪切区域，防止绘制到别的单元格当中
			CRgn rgnClip;
			rgnClip.CreateRectRgn(rcCell.left, rcCell.top, rcCell.right, rcCell.bottom);
			dcMemory.SelectClipRgn(&rgnClip, RGN_AND);

			// 绘制单元格
			CELLCONFIG& cellCfg = cellCfgArray[i];
			DrawCell(dcMemory, rcCell, (i == _treeColumn && (!_bNoChild || hItemData->hParentData != NULL)),
				(i == 0), hItemData, &cellCfg.cellInfo, &cellCfg.editConfig, hColumnData, cellCfg.cellText);

			dcMemory.RestoreDC(nSave);
		}
	}

	dcMemory.SelectObject(hOldFont);
}

void CTimeLineTreeListCtrl::SetCorrespondingTimeFrameCtrl(CTimeFrameListCtrl *pTimeFrameCtrl)
{
	_pTimeLineCtrl = pTimeFrameCtrl;
}

void CTimeLineTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 隐藏提示
	_cellToolTip.ShowWindow(SW_HIDE);

	SetFocus();

	// 记录列拖动起始位置
	_dragStartPt = point;

	// 捕获鼠标
	SetCapture();
	_bBtnDown = true;

	// 更新鼠标测试
	UpdateHitTest(true);

	// 设置选择项
	if (_hHoverRow != NULL)
	{
		// 释放鼠标捕获(避免点击时弹出对话框，鼠标卡主)
		ReleaseCapture();

		// 设置单选择项
		if (SetItemSelectFlag(_hHoverRow))
		{
			NotifyParent(LCN_SELECTED);
		}

		// 处理单击控件事件;
		EditItemByPoint(point, false);
	}

	InvalidateRect(NULL);
	CTimeLineSplitCtrl* pTimeLineSplitCtrl = (CTimeLineSplitCtrl*)GetOwner();
	if (nullptr != pTimeLineSplitCtrl)
	{
		pTimeLineSplitCtrl->UpdateScrollBar();
	}
	if (nullptr != _pTimeLineCtrl)
	{
		_pTimeLineCtrl->UpdateCtrl();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

LRESULT CTimeLineTreeListCtrl::NotifyParent(UINT nMessage, int nCol, int nRow, HEDITCONFIG hEditCfg,
	const CString& strCellText, CString* pStrNewText, int nClickType)
{
	CWnd* pTimeLineWnd = nullptr;
	if (_pTimeLineCtrl != nullptr)
	{
		pTimeLineWnd = _pTimeLineCtrl->GetTimeLineCtrl();
		if (pTimeLineWnd == nullptr || pTimeLineWnd->GetSafeHwnd() == nullptr)
			pTimeLineWnd = nullptr;
	}

	CTreeListNotify notify;
	notify.hdrNotify.hwndFrom = pTimeLineWnd == nullptr ? m_hWnd : pTimeLineWnd->GetSafeHwnd();
	notify.hdrNotify.idFrom = pTimeLineWnd == nullptr ? GetDlgCtrlID() : pTimeLineWnd->GetDlgCtrlID() ;
	notify.hdrNotify.code = nMessage;
	notify.row = nRow;
	notify.col = nCol;
	notify.strOldText = strCellText;
	notify.pStrNewText = pStrNewText;
	notify.hEditConfig = hEditCfg;
	notify.nClickType = nClickType;

	// 发送NOTIFY消息;
	return ::SendMessage(_hNofigyWnd, WM_NOTIFY, 0, (LPARAM)&notify);
}