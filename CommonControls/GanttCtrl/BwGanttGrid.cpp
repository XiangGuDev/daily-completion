#include "stdafx.h"
#include "BwGanttGrid.h"
#include "BwGanttControl.h"

//--------------------------------------CBwGrowthPatternItem---------------------------------------------

IMPLEMENT_DYNCREATE(CBwGrowthPatternItem, CBCGPGridItem)

CBwGrowthPatternItem::CBwGrowthPatternItem()
{
	AllowEdit(false);

#ifndef _BCGSUITE_
	_clrTextOld = globalData.clrHotText;
#else
	_clrTextOld = globalData.clrHotLinkNormalText;
#endif
}

void CBwGrowthPatternItem::OnDrawValue(CDC* pDC, CRect rect)
{
	ASSERT_VALID(m_pGridRow);

	CBCGPGridCtrl* pWndList = GetOwnerList();
	ASSERT_VALID(pWndList);

	COLORREF clrText = OnFillBackground(pDC, rect);

	//-----------
	// Draw link:
	//-----------

	pDC->SetTextColor(_clrTextOld);

	CFont* pOldFont = pDC->SelectObject(&pWndList->GetUnderlineFont(TRUE));
	ASSERT(pOldFont != NULL);

	CString str = (LPCTSTR)(_bstr_t)m_varValue;

	rect.DeflateRect(globalUtils.ScaleByDPI(3), globalUtils.ScaleByDPI(2));
	DWORD dwFlags = GetFlags();
	int nTextAlign = GetAlign();

	UINT uiTextFlags = DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;

	if (nTextAlign & HDF_CENTER)
	{
		uiTextFlags |= DT_CENTER;
	}
	else if (nTextAlign & HDF_RIGHT)
	{
		uiTextFlags |= DT_RIGHT;
	}
	else // nTextAlign & HDF_LEFT
	{
		uiTextFlags |= DT_LEFT;
	}

	if (dwFlags & BCGP_GRID_ITEM_VTOP)
	{
		uiTextFlags |= DT_TOP;
	}
	else if (dwFlags & BCGP_GRID_ITEM_VBOTTOM)
	{
		uiTextFlags |= DT_BOTTOM;
	}
	else // dwFlags & BCGP_GRID_ITEM_VCENTER
	{
		uiTextFlags |= DT_VCENTER;
	}

	pDC->DrawText(str, rect, uiTextFlags);

	m_bValueIsTrancated = pDC->GetTextExtent(str).cx > rect.Width();

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(_clrTextOld);
}

void CBwGrowthPatternItem::FreezeItem(bool bFreeze)
{
	if (bFreeze)
	{
		// 置灰
		_clrTextOld = RGB(100, 100, 100);
	}
	else
	{
#ifndef _BCGSUITE_
		_clrTextOld = globalData.clrHotText;
#else
		_clrTextOld = globalData.clrHotLinkNormalText;
#endif
	}
}

// -----------------------------------------End-----------------------------------------------------

//--------------------------------------CNoEditItem---------------------------------------------

IMPLEMENT_DYNCREATE(CNoEditItem, CBCGPGridItem)

CNoEditItem::CNoEditItem()
{
	AllowEdit(false);
}

// -----------------------------------------End-----------------------------------------------------

//--------------------------------------CBwDateTimeItem------------------------------------------

IMPLEMENT_DYNCREATE(CBwDateTimeItem, CBCGPGridDateTimeItem)

CBwDateTimeItem::CBwDateTimeItem() 
{
	m_nFlags = CBCGPDateTimeCtrl::DTM_DATE;
}

CString CBwDateTimeItem::FormatItem()
{
	ASSERT_VALID(this);

	// 为空时就不要显示
	if (IsEmpty())
		return _T("");

	COleDateTime date = (DATE)m_varValue;

	SYSTEMTIME st;
	date.GetAsSystemTime(st);

	CString str;

	CString strDate;
	::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, strDate.GetBuffer(_MAX_PATH), _MAX_PATH);
	strDate.ReleaseBuffer();

	DWORD dwTimeFormatFlags = (m_nFlags & CBCGPDateTimeCtrl::DTM_SECONDS) == 0 ? TIME_NOSECONDS : 0;

	CString strTime;
	::GetTimeFormat(LOCALE_USER_DEFAULT, dwTimeFormatFlags, &st, NULL, strTime.GetBuffer(_MAX_PATH), _MAX_PATH);
	strTime.ReleaseBuffer();

	const UINT nFlags = m_nFlags & (CBCGPDateTimeCtrl::DTM_DATE | CBCGPDateTimeCtrl::DTM_TIME);
	if (nFlags == CBCGPDateTimeCtrl::DTM_DATE)
	{
		str = strDate;
	}
	else if (nFlags == CBCGPDateTimeCtrl::DTM_TIME)
	{
		str = strTime;
	}
	else
	{
		if (!strDate.IsEmpty())
		{
			str = strDate;
		}

		if (!strTime.IsEmpty())
		{
			if (!str.IsEmpty())
			{
				str += _T(" ");
			}

			str += strTime;
		}
	}

	return str;
}

CWnd* CBwDateTimeItem::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
	ASSERT_VALID(m_pGridRow);
	CBCGPGridCtrl* pWndList = GetOwnerList();
	ASSERT_VALID(pWndList);

	CBCGPDateTimeCtrl* pDateTime = new CBCGPDateTimeCtrl;
	ASSERT_VALID(pDateTime);

	pDateTime->SetAutoResize(FALSE);

	CRect rectSpin;
	AdjustInPlaceEditRect(rectEdit, rectSpin);

	pDateTime->Create(_T(""), WS_CHILD | BS_NOTIFY, rectEdit,
		pWndList, 1);
	pDateTime->SetFont(pWndList->GetFont());

	CString strFormat;
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, strFormat.GetBuffer(_MAX_PATH), _MAX_PATH);
	strFormat.ReleaseBuffer();

	if (strFormat.Find(_T("MMMM")) != -1)
	{
		pDateTime->m_monthFormat = 1;
	}
	else if (strFormat.Find(_T("MMM")) != -1)
	{
		pDateTime->m_monthFormat = 0;
	}
	else
	{
		pDateTime->m_monthFormat = 2;
	}

	SetState(*pDateTime);
	pDateTime->SetDate(IsEmpty() ? COleDateTime::GetCurrentTime() : GetDate());

	if (pWndList->IsVisualManagerStyle())
	{
		pDateTime->SendMessage(BCGM_ONSETCONTROLVMMODE, TRUE);
	}

	pDateTime->SetTextColor(pWndList->GetTextColor(), FALSE);
	pDateTime->SetBackgroundColor(pWndList->GetBkColor(), FALSE);

	pDateTime->ShowWindow(SW_SHOW);

	bDefaultFormat = FALSE;

	return pDateTime;
}

BOOL CBwDateTimeItem::OnUpdateValue()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndInPlace);
	ASSERT(::IsWindow(m_pWndInPlace->GetSafeHwnd()));
	ASSERT_VALID(m_pGridRow);

	CBCGPDateTimeCtrl* pDateTime = DYNAMIC_DOWNCAST(CBCGPDateTimeCtrl, m_pWndInPlace);
	ASSERT_VALID(pDateTime);

	// 为空默认选择为当前时间
	COleDateTime dateOld = GetDate();

	COleDateTime dateNew = pDateTime->GetDate();

	if (CanUpdateData())
	{
		m_varValue = _variant_t(dateNew, VT_DATE);
	}

	if (dateOld != dateNew)
	{
		SetItemChanged();
	}

	return TRUE;
}

// -----------------------------------------End-----------------------------------------------------

CBwGanttGrid::CBwGanttGrid() :
	_bCreated(FALSE)
	, _pRowCellMergeCall(nullptr)
	, _pRowCellFormatCall(nullptr)
	, _pMergeUserData(nullptr)
	, _pFormatUserData(nullptr)
	, _pControl(NULL)
{
}

CBwGanttGrid::~CBwGanttGrid()
{
}

const std::vector<CBwGanttGrid::ColumnFormat>& CBwGanttGrid::GetColumnFormats()
{
	return _ColumnFormats;
}

BOOL CBwGanttGrid::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID)
{
	//创建
	_bCreated = __super::Create(dwStyle, rect, pParentWnd, nID);
	if (!_bCreated)
		return _bCreated;
	ShowWindow(SW_SHOW);//// 必须设置为SW_SHOW否则控件不会显示
	SetDefaultStyle();

	return _bCreated;
}

void CBwGanttGrid::SetDefaultStyle()
{
	if (!_bCreated)
	{
		ASSERT(FALSE);
		return;
	}

	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);// 设置滚动条风格为BCG样式
	EnableMarkSortedColumn(FALSE);// 关闭排序
	EnableHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS);// 显示列头
	EnableRowHeader(TRUE);// 显示行头
	EnableInvertSelOnCtrl();// If TRUE, deselect item on single click
	SetEditFirstClick(FALSE);
	EnableDragHeaderItems(FALSE);

}

void CBwGanttGrid::SetColumnFormats(const std::vector<CBwGanttGrid::ColumnFormat>& columnFormats,
	int nHeaderLineCount, const std::vector<ColumnMergeFormat>& ColumnMergeFormat)
{
	if (!_bCreated)
	{
		ASSERT(FALSE);
		return;
	}
	_ColumnFormats = columnFormats;
	_ColumnMergeFormat = ColumnMergeFormat;

	m_ColumnsEx.SetHeaderLineCount(nHeaderLineCount);

	CreateColumns();
}

void CBwGanttGrid::SetRowCellFormatCallBack(RowCellFormatCall callfun, VOID * pUserData)
{
	_pRowCellFormatCall = callfun;
	_pFormatUserData = pUserData;
}

void CBwGanttGrid::SetRowCellMergeCallBack(RowCellMergeCall callfun, VOID * pUserData)
{
	_pRowCellMergeCall = callfun;
	_pMergeUserData = pUserData;
}


int CBwGanttGrid::GetGridItemIndex(CBCGPGridRow * pItem)
{
	if (pItem == NULL)
		return -1;

	int nRow = GetRowCount();
	for (int i = 0; i < nRow; i++)
	{
		if (GetRow(i) == pItem)
			return i;
	}
	return -1;
}

int CBwGanttGrid::GetGridItemIndex(CBCGPGridItem * pItem)
{
	return GetGridItemIndex(pItem ? pItem->GetParentRow() : NULL);
}

void CBwGanttGrid::SetGridData(int nRow, int nCol, const _variant_t & varValue)
{
	if (nRow < 0 || nCol < 0)
		return;
	if (nRow >= GetRowCount()  || nCol >= GetColumnCount())
		return;
	CBCGPGridRow *pRow = GetRow(nRow);
	if (pRow == NULL)
		return;
	CBCGPGridItem *pItem = pRow->GetItem(nCol);
	if (pItem == NULL)
		return;
	pItem->SetValue(varValue);
}

_variant_t CBwGanttGrid::GetGridData(int nRow, int nCol)
{
	if (nRow < 0 || nCol < 0)
		return 0;
	if (nRow >= GetRowCount() || nCol >= GetColumnCount())
		return 0;
	CBCGPGridRow *pRow = GetRow(nRow);
	if (pRow == NULL)
		return 0;
	CBCGPGridItem *pItem = pRow->GetItem(nCol);
	if (pItem == NULL)
		return 0;
	return pItem->GetValue();
}

int CBwGanttGrid::GetColIndexByName(const CString & strName)
{
	int nIndex = -1;
	CBCGPGridColumnsInfo &colInfo = GetColumnsInfo();
	int nColCount = colInfo.GetColumnCount();
	for (int i = 0; i < nColCount; i++)
	{
		if (strName == colInfo.GetColumnName(i))
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}

void CBwGanttGrid::CreateColumns()
{
	if (!_bCreated)
	{
		ASSERT(FALSE);
		return;
	}
	//清空所有行和列
	RemoveAll();
	DeleteAllColumns();

	//先设置合并信息 
	for (int i = 0; i < _ColumnMergeFormat.size(); ++i)
	{
		auto& mergeFormat = _ColumnMergeFormat[i];

		if (mergeFormat._nLeft > mergeFormat._nRight || mergeFormat._nTop > mergeFormat._nBottom)
			continue;//过滤不合法数据

		CArray<int, int> arrMergeColumns, arrMergeLines;
		for (int nCol = mergeFormat._nLeft;nCol <= mergeFormat._nRight;++nCol)
		{
			arrMergeColumns.Add(nCol);
		}
		for (int nCol = mergeFormat._nTop;nCol <= mergeFormat._nBottom;++nCol)
		{
			arrMergeLines.Add(nCol);
		}
		m_ColumnsEx.AddHeaderItem(&arrMergeColumns, &arrMergeLines, -1, mergeFormat._strName, HDF_CENTER);
	}

	//添加列
	for (int i = 0; i < _ColumnFormats.size(); ++i)
	{
		InsertColumn(i, _ColumnFormats[i]._strName, globalUtils.ScaleByDPI(_ColumnFormats[i]._width));
		m_ColumnsEx.SetHeaderAlign(i, HDF_CENTER);
		m_ColumnsEx.SetColumnAlign(i, HDF_CENTER);
		m_ColumnsEx.SetHeaderMultiLine(i, TRUE);
		SetItemRTC(i, _ColumnFormats[i]._runtime);
	}
	AdjustLayout();
}

CRect CBwGanttGrid::OnGetHeaderRect(CDC* pDC, const CRect& rectDraw)
{
	CRect rect = __super::OnGetHeaderRect(pDC, rectDraw);
	rect.bottom = rect.top + rect.Height();// 设置列头高度（Header Height）

	return rect;
}

void CBwGanttGrid::OnDrawHeader(CDC* pDC)
{
	m_ColumnsEx.PrepareDrawHeader();

	__super::OnDrawHeader(pDC);
}

void CBwGanttGrid::OnPrintHeader(CDC* pDC, CPrintInfo* pInfo)
{
	m_ColumnsEx.PreparePrintHeader();

	__super::OnPrintHeader(pDC, pInfo);
}

void CBwGanttGrid::OnPosSizeChanged()
{
	__super::OnPosSizeChanged();

	m_ColumnsEx.ReposHeaderItems();
}

void CBwGanttGrid::OnUpdateVScrollPos(int nVOffset, int nPrevVOffset)
{
	__super::OnUpdateVScrollPos(nVOffset, nPrevVOffset);
}

int CBwGanttGrid::GetSelIndex()
{
	CList <CBCGPGridItem*, CBCGPGridItem*> selItems;
	GetSelectedItems(selItems);

	POSITION pos = selItems.GetHeadPosition();
	if (!pos)
		return -1;

	CBCGPGridItem *pGridItem = selItems.GetNext(pos);
	return GetGridItemIndex(pGridItem);
}

void CBwGanttGrid::GetSelectedIndexes(std::set<int> &arrSelIndexes)
{
	CList <CBCGPGridItem*, CBCGPGridItem*> selItems;
	GetSelectedItems(selItems);

	POSITION pos = selItems.GetHeadPosition();
	while (pos != NULL)
	{
		CBCGPGridItem *pGridItem = selItems.GetNext(pos);
		if (pGridItem == NULL)
			continue;
		int nIndex = GetGridItemIndex(pGridItem);
		if (nIndex > -1)
			arrSelIndexes.insert(nIndex);
	}
}

void CBwGanttGrid::SetControl(CBwGanttControl * pControl)
{
	_pControl = pControl;
}

void CBwGanttGrid::SetHeaderAlign(int nAlign)
{
	int nColCount = GetColumnCount();
	for (int i = 0; i < nColCount; ++i)
	{
		__super::SetHeaderAlign(i, nAlign);
	}
}

CBCGPGridRow * CBwGanttGrid::GetRow(int nPos)
{
	if (nPos < 0 || nPos > GetRowCount())
		return NULL;
	return __super::GetRow(nPos);
}

CBCGPGanttControl* CBwGanttGrid::GetGanttControlNotify() const
{
	return _pControl;
}

void CBwGanttGrid::OnItemChanged(CBCGPGridItem* pGridItem, int nRow, int nColumn)
{
	CBCGPGridCtrl::OnItemChanged(pGridItem, nRow, nColumn);

	CBCGPGanttControl* pBCGPNotify = GetGanttControlNotify();
	CBwGanttControl *pNotify = dynamic_cast<CBwGanttControl *>(pBCGPNotify);
	if (pNotify != NULL)
	{
		ASSERT_VALID(pNotify);

		pNotify->OnGridItemChanged(pGridItem, nRow, nColumn);
	}
}

void CBwGanttGrid::DoRebuildTerminalItems()
{
	if (m_bUseQuickSort)
		m_bUseQuickSort = false;			// 防止进入sort方法重绘
	__super::DoRebuildTerminalItems();
}

void CBwGanttGrid::OnHeaderColumnClick(int nColumn)
{
	switch (nColumn)
	{
	case 2:
	case 3:
	case 5:
	case 6:
	{
		bool bAsc = true;
		auto pIter = _mapColClick.find(nColumn);
		if (pIter != _mapColClick.end() && (pIter->second % 2 == 1))
		{
			bAsc = false;
			_mapColClick[nColumn] = 0;
		}
		else
		{
			_mapColClick[nColumn] = 1;
		}
		_nCurSelColIndex = nColumn;
		Sort(nColumn, bAsc, TRUE);		// 为显示图标
		::PostMessage(GetParent()->GetSafeHwnd(), WM_USER_SORTCOLUM, (WPARAM)bAsc, (LPARAM)nColumn);
		break;
	}
	default:
	{
		ClearSortColumn();
		break;
	}
	}
}

void CBwGanttGrid::ClearSortColumn()
{
	switch (_nCurSelColIndex)
	{
	case 2:
	case 3:
	case 5:
	case 6:
	{
		RemoveSortColumn(_nCurSelColIndex);
		_nCurSelColIndex = -1;
		break;
	}
	default: break;
	}
}
