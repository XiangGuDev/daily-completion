#include "stdafx.h"
#include "TreeListCtrl.h"

namespace ControlUI
{
	// 控件禁用背景色
#define DISABLE_BK_COLOR RGB(238,238,238)

	CTreeListCtrl::CTreeListCtrl()
	{
		_showHeader = true;
		_hasGrid = true;
		_hasLine = true;
		_hDragingCol = NULL;
		_bBtnDown = false;
		_bMouseOver = false;
		_bSingleSelect = true;
		_nHeaderHeight = LIST_HEAD_HEIGHT;
		_iconSize = CSize(16, 16);
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
		_pImageList = NULL;
		_bColumnSort = false;
		_bIsTrim = true;
		_bCheckSelChange = true;

		_bDelImageList = true;

		_bCollapseStyle = true;
	}

	CTreeListCtrl::~CTreeListCtrl()
	{
		_treeListFont.DeleteObject();
		_underlineFont.DeleteObject();

		DeleteAllColumn();

		if (_bDelImageList && _pImageList)
		{
			delete _pImageList;
		}

		if (NULL != _pConfig)
		{
			delete _pConfig;
			_pConfig = NULL;
		}

		if ((NULL != _pListDataSource) && _bDelDataSource)
		{
			delete _pListDataSource;
			_pListDataSource = NULL;
		}

		if (NULL != _hIconCollapse)
		{
			DestroyIcon(_hIconCollapse);
			_hIconCollapse = NULL;
		}
		if (NULL != _hIconExpand)
		{
			DestroyIcon(_hIconExpand);
			_hIconExpand = NULL;
		}
	}

	BEGIN_MESSAGE_MAP(CTreeListCtrl, CWnd)
		ON_WM_PAINT()
		ON_WM_ERASEBKGND()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_LBUTTONDBLCLK()
		ON_WM_RBUTTONDOWN()
		ON_WM_SETCURSOR()
		ON_WM_VSCROLL()
		ON_WM_HSCROLL()
		ON_WM_SIZE()
		ON_WM_KILLFOCUS()
		ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
		ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
		ON_MESSAGE(LIST_CTRL_END, OnEndEdit)
		ON_MESSAGE(LCN_HYPERLINK, OnHyperLink)
		ON_WM_MOUSEWHEEL()
		ON_WM_KEYDOWN()
		ON_WM_THEMECHANGED()
	END_MESSAGE_MAP()

	void CTreeListCtrl::InitControlData()
	{
		if (NULL == m_hWnd)
			return;

		_hNofigyWnd = GetParent()->GetSafeHwnd();

		// 加载皮肤数据
		if (_headerTheme.IsThemeNull())
		{
			_headerTheme.OpenThemeData(m_hWnd, L"HEADER");
		}
		
		RESOURCE_OVERRIDE;

		HINSTANCE curModule = AfxGetResourceHandle();

		if (_bCollapseStyle)
		{
			_hIconExpand = LoadIcon(curModule, MAKEINTRESOURCE(IDI_EXPAND));
			_hIconCollapse = LoadIcon(curModule, MAKEINTRESOURCE(IDI_COLLAPSE));
		}
		else
		{
			_hIconExpand = LoadIcon(curModule, MAKEINTRESOURCE(IDI_EXPAND_ADD));
			_hIconCollapse = LoadIcon(curModule, MAKEINTRESOURCE(IDI_COLLAPSE_MINUS));
		}

		if (_progressTheme.IsThemeNull())
		{
			_progressTheme.OpenThemeData(m_hWnd, L"PROGRESS");
		}

		// 初始化字体
		LOGFONT tempFont;
		HFONT hDefFont = HFONT(GetStockObject(DEFAULT_GUI_FONT));
		CFont* pFont = CFont::FromHandle(hDefFont);
		pFont->GetLogFont(&tempFont);

		_treeListFont.CreateFontIndirect(&tempFont);

		tempFont.lfUnderline = TRUE;
		_underlineFont.CreateFontIndirect(&tempFont);

		// 创建提示信息控件
		_cellToolTip.Create(this, &_treeListFont);
		_cellToolTip.SetColor(_hotTopColor, _hotBorderColor);
	}

	void CTreeListCtrl::DeleteAllItem()
	{
		// 清除行数据
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin();
			iter != endIter; ++iter)
		{
			delete iter->second;
		}
		_listDataSource.clear();
	}

	bool CTreeListCtrl::RegisterTreeList()
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASS wndCls = { 0 };
		if (GetClassInfo(hInstance, L"TreeListCtrl", &wndCls))
			return true;

		wndCls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndCls.lpfnWndProc = ::DefWindowProc;
		wndCls.cbClsExtra = 0;
		wndCls.cbWndExtra = 0;
		wndCls.hInstance = GetModuleHandle(NULL);
		wndCls.hIcon = NULL;
		wndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndCls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndCls.lpszMenuName = NULL;
		wndCls.lpszClassName = L"TreeListCtrl";

		return !!AfxRegisterClass(&wndCls);
	}

	void CTreeListCtrl::UpdateScrollBar(int scrollMode)
	{
		if (scrollMode == SB_HORZ || scrollMode == SB_BOTH)
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

		if (scrollMode == SB_VERT || scrollMode == SB_BOTH)
		{
			CRect rcListRow = CalcListRect();

			int nSumRowHeight = 0;

			// 统计行总高度
			ListDataSourceIter endIter = _listDataSource.end();
			for (ListDataSourceIter iter = _listDataSource.begin();
				iter != endIter; ++iter)
			{
				HITEMDATA hParentData = iter->second->hParentData;
				if (NULL != hParentData && hParentData->IsCollapse())
					continue;

				nSumRowHeight += iter->second->rowRect.Height();
			}

			SCROLLINFO scrInfo;
			scrInfo.cbSize = sizeof(scrInfo);

			scrInfo.fMask = SIF_PAGE | SIF_RANGE;
			scrInfo.nPage = rcListRow.Height();
			scrInfo.nMin = 0;
			scrInfo.nMax = nSumRowHeight;

			SetScrollInfo(SB_VERT, &scrInfo);
		}
	}

	bool CTreeListCtrl::CollectItemListData(IListDataSource* pDataSource,
		ListDataSource& dataSouceList, MapDataSource& dataSourceMap,
		HITEMDATA hParentData, int nTreeLevel, HITEMDATA& hLastChildItem)
	{
		if (NULL == pDataSource)
			return false;

		bool bHasChild = false;
		HITEMDATA hItemData = NULL; // 临时行数据也是最后一行数据
		MapDataSourceIter dataSourceEndIter = dataSourceMap.end();

		// 递归获取行数据
		size_t nRow = pDataSource->GetCount(_pConfig);
		for (size_t i = 0; i < nRow; ++i)
		{
			bool bShowItem = true;
			ITEMDATA itemData;	// 临时数据,用于设置是否展开等信息

			IListDataSource* pChildDataSource = pDataSource->GetItem(i, bShowItem, &itemData, _pConfig);

			if (NULL == pChildDataSource)
				continue;

			// 如果不显示则只递归查询其下子节点
			if (!bShowItem)
			{
				// 遍历子节点
				bool bTempHasChild = CollectItemListData(pChildDataSource,
					dataSouceList, dataSourceMap, hParentData, nTreeLevel, hLastChildItem);

				if (!bHasChild)
					bHasChild = bTempHasChild;

				// Fix bw-135813
				if (bTempHasChild)
					hItemData = hLastChildItem;

				// 清除不显示的数据源
				delete pChildDataSource;
				continue;
			}

			// 已存在子节点
			bHasChild = true;
			size_t nKey = pChildDataSource->GetKey();

			// 查找是否已存此节点数据
			MapDataSourceIter itemDataSourceIter = dataSourceMap.find(nKey);

			// 是否存在缓存中
			if (itemDataSourceIter != dataSourceEndIter)
			{
				dataSouceList.push_back(*itemDataSourceIter);

				hItemData = itemDataSourceIter->second;
				hItemData->hParentData = hParentData;
				hItemData->lastChild = false;

				// 清除原数据源指针
				hItemData->DelDataSource();
				hItemData->pDataSource = pChildDataSource;

				// 已查找到，则移除
				dataSourceMap.erase(itemDataSourceIter);
			}
			else // 数据源第一次同步或有新增节点
			{
				hItemData = new ITEMDATA();
				*hItemData = itemData;	 // 初始化赋值(用于反馈是否展开等信息)

				hItemData->hParentData = hParentData;
				hItemData->pDataSource = pChildDataSource;

				dataSouceList.push_back(ListDataSource::value_type(nKey, hItemData));
			}

			// 赋值节点层次
			hItemData->treeLevel = nTreeLevel;

			// 遍历子节点
			HITEMDATA hChildItem = NULL;
			hItemData->hasChild = CollectItemListData(pChildDataSource,
				dataSouceList, dataSourceMap, hItemData, nTreeLevel + 1, hChildItem);

			// 标记最后节点
			if (NULL != hChildItem)
				hChildItem->lastChild = true;
		}

		// 当过滤节点时，会出现为空情况
		if (NULL != hItemData)
			hLastChildItem = hItemData;

		// 修复bw-134327
		if (bHasChild && nTreeLevel != 0)
			_bNoChild = false;

		return bHasChild;
	}

	void CTreeListCtrl::CollectItemListData()
	{
		// 建立原数据行的状态
		MapDataSource itemDataSourceMap;

		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin();
			iter != endIter; ++iter)
		{
			itemDataSourceMap.insert(*iter);
		}
		_listDataSource.clear();

		// 修复bw-134327
		_bNoChild = true;

		// 递归获取数据
		HITEMDATA hLastChildItem = NULL;
		CollectItemListData(_pListDataSource, _listDataSource, itemDataSourceMap, NULL, 0, hLastChildItem);

		// 标记最后节点
		if (hLastChildItem != NULL)
		{
			hLastChildItem->lastChild = true;
		}

		// 把剩余删除的数据源节点对应的表格数据删除(数据源有删除节点)
		MapDataSourceIter dataSourceEndIter = itemDataSourceMap.end();
		for (MapDataSourceIter iter = itemDataSourceMap.begin();
			iter != dataSourceEndIter; ++iter)
		{
			delete iter->second;
		}
		itemDataSourceMap.clear();
	}

	void CTreeListCtrl::CancelEdit()
	{
		if (_pControl && ::IsWindow((_pControl->GetSafeHwnd())))
		{
			IEditBase* pEditBase = dynamic_cast<IEditBase*>(_pControl);
			if (NULL != pEditBase)
				pEditBase->CancelEdit();
		}

		// 如果正在编辑则取消
		DeleteControl();
	}

	void CTreeListCtrl::UpdateList(bool bDataChange, bool bCancelSel)
	{
		// 如果正在编辑则取消
		CancelEdit();

		// 强制更新数据
		if (bDataChange)
		{
			// 收集行数据
			CollectItemListData();

			if (bCancelSel)
			{
				for (ListDataSourceIter iter = _listDataSource.begin(); iter != _listDataSource.end(); ++iter)
				{
					iter->second->seleced = false;
				}
			}

			// 清空上选中一行记录
			_nLastSelectItem = -1;
		}

		UpdateHitTest(false);

		// 多行显示文本时滚动条计算错误
		SendMessage(WM_PAINT);

		UpdateScrollBar(SB_BOTH);
		InvalidateRect(NULL);

		// 清空提示框
		_cellToolTip.Reset();
	}

	bool CTreeListCtrl::UpdateHitTest(bool bUpdateSel)
	{
		// 获取鼠标位置
		CPoint mPt;
		GetCursorPos(&mPt);
		ScreenToClient(&mPt);

		// 客户区域
		CRect rcClient;
		GetClientRect(&rcClient);

		// 是否需要更新界面(加快性能)
		bool bNeedUpdate = false;

		// 计算列头位置与状态(列头在移动的时候不需要更新列头状态)
		if (!(_bBtnDown && _hDragingCol != NULL) && _showHeader)
		{
			HCOLUMNDATA hOldHoverCol = _hHoverCol;
			HCOLUMNDATA hOldPressCol = _hPressCol;

			// 重置拖动列与悬停列
			_hHoverCol = _hDragingCol = _hPressCol = NULL;

			// 遍历测试列头(先转换成线性结构)
			ColumnDataVector columnVector;
			GetColumnLinearityStruct(columnVector);

			size_t nCount = columnVector.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				HCOLUMNDATA hColumnData = columnVector[i];

				// 测试是否在拖拽区域
				CRect rcDragArea = hColumnData->colRect;
				rcDragArea.left = rcDragArea.right - 4;
				rcDragArea.right = rcDragArea.right + 4;

				// 是否在托动区域中(鼠标按下时不检测)
				if (!_bBtnDown && rcDragArea.PtInRect(mPt))
				{
					_hDragingCol = hColumnData;
				}

				// 解决移动滚动条上的测试问题
				CRect rcTest = hColumnData->colRect;
				rcTest.left = rcTest.left < rcClient.left ? rcClient.left : rcTest.left;
				rcTest.right = rcTest.right > rcClient.right ? rcClient.right : rcTest.right;

				// 测试是否在列头中
				if (rcTest.PtInRect(mPt))
				{
					// 托动的时候鼠标按下无效果
					if (_bBtnDown && (_hDragingCol == NULL))
						_hPressCol = hColumnData;

					// 设置新的悬停列
					_hHoverCol = hColumnData;
				}
			}

			// 状态变化需要刷新
			if (!bNeedUpdate)
			{
				bNeedUpdate = (_hHoverCol != hOldHoverCol) || (_hPressCol != hOldPressCol);
			}
		}
		else if (_showHeader)
		{
			// 列头在移动
			bNeedUpdate = true;
		}

		HITEMDATA hOldHoverRow = _hHoverRow;

		// 重置悬停行
		_hHoverRow = NULL;
		_nHoverRowIndex = -1;

		int nRow = 0;
		// 检测行悬停状态
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin();
			iter != endIter; ++iter, ++nRow)
		{
			HITEMDATA hItemData = iter->second;

			// 当按下左键时，检查鼠标位置是否在收缩图标区域内，如果在此区域则刷新树列表展开收缩的状态。
			if (_bBtnDown && bUpdateSel)
			{
				// 解决移动到滚动条上的测试问题
				CRect rcTest = hItemData->expandRect;

				// 测试鼠标是否在行上
				if (rcTest.PtInRect(mPt))
				{
					hItemData->expand = !hItemData->expand;
					// 此处不要break，否则界面选中行焦点会丢失。;
					//break;
				}
			}

			// 解决移动到滚动条上的测试问题
			CRect rcTest = hItemData->rowRect;
			rcTest.left = rcTest.left < rcClient.left ? rcClient.left : rcTest.left;
			rcTest.right = rcTest.right > rcClient.right ? rcClient.right : rcTest.right;

			// 鼠标在列头上,不选中行
			CRect rcHeader = CalcHeaderRect();

			// 测试鼠标是否在行上
			if (!(_showHeader && rcHeader.PtInRect(mPt)) && rcTest.PtInRect(mPt))
			{
				_nHoverRowIndex = nRow;
				_hHoverRow = hItemData;
				break;
			}
		}

		if (!bNeedUpdate)
		{
			bNeedUpdate = (hOldHoverRow != _hHoverRow);
		}

		return bNeedUpdate;
	}

	void CTreeListCtrl::UpdateColumnSortState()
	{
		// 不使用列头排序事件，或悬停列为空
		if (!_bColumnSort || NULL == _hHoverCol)
			return;

		// 获取鼠标位置
		CPoint mPt;
		GetCursorPos(&mPt);
		ScreenToClient(&mPt);

		// 遍历测试列头
		ColumnDataVector leafColumnVector;
		GetColumnLinearityStruct(leafColumnVector);

		HCOLUMNDATA hSelColumn, hUnsetColumn;
		hSelColumn = hUnsetColumn = NULL;

		size_t nSortColIndex = 0;
		size_t nCount = leafColumnVector.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			HCOLUMNDATA hColumnData = leafColumnVector[i];

			if (hColumnData->sortType != TLC_NO_SORT)
				hUnsetColumn = hColumnData;

			CRect rcDragArea = hColumnData->colRect;
			rcDragArea.left += 4;  // 排除拖拽区域
			rcDragArea.right -= 4; // 排除拖拽区域

			// 测试是否在列头中点击
			if (rcDragArea.PtInRect(mPt))
			{
				nSortColIndex = i;
				hSelColumn = hColumnData;
			}
		}

		if (hSelColumn != NULL)
		{
			CColumnSortNotify sortNotify;
			sortNotify.hdrNotify.hwndFrom = m_hWnd;
			sortNotify.hdrNotify.idFrom = GetDlgCtrlID();
			sortNotify.hdrNotify.code = LCH_COLUMN_BEGINSORT;
			sortNotify.hColData = hSelColumn;
			sortNotify.nColIndex = nSortColIndex;

			// 发送开始排序消息
			if (LCNR_OK != ::SendMessage(_hNofigyWnd, WM_NOTIFY, 0, (LPARAM)&sortNotify))
				return;

			if (hUnsetColumn != NULL && hUnsetColumn != hSelColumn)
				hUnsetColumn->sortType = TLC_NO_SORT;

			if (hSelColumn->sortType == TLC_ASC_SORT)
			{
				hSelColumn->sortType = TLC_DSC_SORT;
			}
			else
			{
				hSelColumn->sortType = TLC_ASC_SORT;
			}

			sortNotify.hdrNotify.code = LCH_COLUMN_ENDSORT;

			// 发送结束排序消息
			::SendMessage(_hNofigyWnd, WM_NOTIFY, 0, (LPARAM)&sortNotify);
		}
	}

	void CTreeListCtrl::DoDragColumn()
	{
		// 获取鼠标位置
		CPoint mPt;
		GetCursorPos(&mPt);
		ScreenToClient(&mPt);

		// 正在移动列头
		if (_bBtnDown && _hDragingCol != NULL && _showHeader)
		{
			int diff = mPt.x - _dragStartPt.x;

			// 列已固定宽度,不移动
			if (_hDragingCol->fixed)
				return;

			// 获取允许的最小值
			SHORT nAllowMinWidth = 0;
			size_t nChildSize = _hDragingCol->childColumn.size();
			if (nChildSize > 1)
			{
				for (size_t i = 0; i < nChildSize - 1; ++i)
					nAllowMinWidth += _hDragingCol->childColumn[i]->GetWidth();
			}

			// 限制最小宽度
			int nSetValue = _hDragingCol->GetWidth() + diff;
			if (nSetValue < nAllowMinWidth)
			{
				nSetValue = nAllowMinWidth;
			}

			if (_hDragingCol->GetWidth() != nSetValue)
			{
				_hDragingCol->SetWidth(nSetValue);
				_dragStartPt = mPt;
				UpdateScrollBar(SB_BOTH);
			}
		}
	}

	bool CTreeListCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
	{
		bool bSuccess = false;

		// 注册控件类型
		if (RegisterTreeList())
		{
			bSuccess = !!CWnd::Create(L"TreeListCtrl", NULL, dwStyle | WS_CLIPCHILDREN, rect, pParentWnd, nID);
		}

		// 初始化控件必要数据
		if (bSuccess)
		{
			InitControlData();
		}

		return bSuccess;
	}

	void CTreeListCtrl::SetDataSource(IListDataSource* pDataSource, bool bDelDataSource)
	{
		// 清除原有数据源
		if ((NULL != _pListDataSource) && _bDelDataSource)
		{
			delete _pListDataSource;
		}

		// 赋值新数据源
		_pListDataSource = pDataSource;
		_bDelDataSource = bDelDataSource;
	}

	bool CTreeListCtrl::LoadConfig(const CString& strCfg, const CString& strCfgName)
	{
		if (_pConfig != NULL)
			delete _pConfig;

		_pConfig = new CTreeListConfig(this);
		return _pConfig->LoadConfig(strCfg, strCfgName);
	}

	void CTreeListCtrl::SetImageList(CImageList* pImageList, bool bAutoDel)
	{
		if (_bDelImageList && _pImageList)
		{
			delete _pImageList;
			_pImageList = NULL;
		}

		_bDelImageList = bAutoDel;
		_pImageList = pImageList;
	}

	void CTreeListCtrl::SetNofityWnd(HWND hNotifyWnd)
	{
		_hNofigyWnd = hNotifyWnd;
	}

	void CTreeListCtrl::SetSingleSelect(bool bSingleSelect)
	{
		_bSingleSelect = bSingleSelect;
	}

	void CTreeListCtrl::SetFocusItem(size_t nSelectIndex)
	{
		if (!_listDataSource.empty() && nSelectIndex >= _listDataSource.size())
			nSelectIndex = _listDataSource.size() - 1;

		int nCount = -1;
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin(); iter != endIter; ++iter)
		{
			if ((++nCount) == nSelectIndex)
			{
				iter->second->seleced = true;
				_nLastSelectItem = (int)nSelectIndex;
			}
			else
			{
				iter->second->seleced = false;
			}
		}

		// 确保显示
		MakeRowVisible(int(nSelectIndex));

		NotifyParent(LCN_SELECTED);
	}

	void CTreeListCtrl::SetFocusItemByDataSource(IListDataSource* pDataSource)
	{
		if (pDataSource)
		{
			SetFocusItemByKey(pDataSource->GetKey());
		}
		else
		{
			SetFocusItem();
		}
	}

	HCOLUMNDATA CTreeListCtrl::InsertColumn(size_t nCol, HCOLUMNDATA hParentCol)
	{
		ColumnDataVector& columnVector = hParentCol
			? hParentCol->childColumn : _columnVector;

		// 检查索引合法性
		if (nCol < 0 || nCol > columnVector.size())
			return NULL;

		// 清除行数据
		DeleteAllItem();

		// 新建列
		HCOLUMNDATA hColData = new COLUMNDATA();
		hColData->hParent = hParentCol;
		columnVector.insert(columnVector.begin() + nCol, hColData);

		return hColData;
	}

	void CTreeListCtrl::DeleteColumn(size_t nCol, HCOLUMNDATA hParentCol)
	{
		ColumnDataVector& columnVector = hParentCol
			? hParentCol->childColumn : _columnVector;

		// 检查索引合法性
		if (nCol < 0 || nCol > columnVector.size())
			return;

		// 清除行数据
		DeleteAllItem();

		// 删除列
		delete columnVector[nCol];
		columnVector.erase(columnVector.begin() + nCol);
	}

	void CTreeListCtrl::DeleteAllColumn()
	{
		// 清除行数据
		DeleteAllItem();

		size_t sz = _columnVector.size();
		for (size_t nCol = 0; nCol < sz; ++nCol)
		{
			DeleteAllColumn(_columnVector[nCol]);
		}
		_columnVector.clear();
	}

	double CTreeListCtrl::GetFontScale()
	{
		HDC screen = ::GetDC(NULL);

		double hPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSX);
		double vPixelsPerInch = GetDeviceCaps(screen, LOGPIXELSY);
		::ReleaseDC(NULL, screen);

		double dpi = (hPixelsPerInch + vPixelsPerInch) * 0.5;
		dpi = dpi / 96;
		if (dpi == 1)
			return dpi;
		else
			return dpi * 0.935;
	}

	void CTreeListCtrl::DeleteAllColumn(HCOLUMNDATA hParentCol)
	{
		size_t sz = hParentCol->childColumn.size();
		for (size_t nCol = 0; nCol < sz; ++nCol)
		{
			DeleteAllColumn(hParentCol->childColumn[nCol]);
		}
		delete hParentCol;
	}

	IListDataSource* CTreeListCtrl::GetSelectedItem()
	{
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin(); iter != endIter; ++iter)
		{
			if (iter->second->seleced)
			{
				return iter->second->pDataSource;
			}
		}

		return NULL;
	}

	void CTreeListCtrl::GetSelectedItems(DataSourceArray &datasourceArray)
	{
		// 先清空数组;
		datasourceArray.clear();

		// 遍历当前选择的记录;
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin(); iter != endIter; ++iter)
		{
			if (iter->second->seleced)
			{
				datasourceArray.push_back(iter->second->pDataSource);
			}
		}
	}

	void CTreeListCtrl::GetDataSourceArray(DataSourceArray &datasourceArray)
	{
		// 先清空数组;
		datasourceArray.clear();

		// 遍历当前选择的记录;
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin(); iter != endIter; ++iter)
		{
			datasourceArray.push_back(iter->second->pDataSource);
		}
	}

	int CTreeListCtrl::GetSelectedIndex()
	{
		int nSelIndex = -1;

		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin();
			iter != endIter; ++iter, ++nSelIndex)
		{
			if (iter->second->seleced)
			{
				return ++nSelIndex;
			}
		}

		return -1;
	}

	size_t CTreeListCtrl::GetTotalRowCount()
	{
		return _listDataSource.size();
	}


	HCOLUMNDATA CTreeListCtrl::GetColumn(size_t nCol, HCOLUMNDATA hColumnData)
	{
		ColumnDataVector& columnVector = hColumnData
			? hColumnData->childColumn : _columnVector;

		// 检查索引合法性
		if (nCol < 0 || nCol > columnVector.size())
			return NULL;

		return columnVector[nCol];
	}

	size_t CTreeListCtrl::GetColumnCount(HCOLUMNDATA hColumnData)
	{
		if (NULL == hColumnData)
		{
			return _columnVector.size();
		}
		else
		{
			return hColumnData->childColumn.size();
		}
	}

	/***********************消息响应***********************/

	void CTreeListCtrl::OnPaint()
	{
		CPaintDC dc(this);

		// 绘图区域
		CRect rcClient;
		GetClientRect(&rcClient);

		// 内存绘图DC
		CMemoryDC dcMemory(&dc, rcClient);
		COLORREF bkColor = _bkColor;
		if (!IsWindowEnabled())
			bkColor = DISABLE_BK_COLOR;

		dcMemory.FillSolidRect(rcClient, bkColor);

		// 绘制列头
		CRect rcHeader = CalcHeaderRect();
		DrawHeader(dcMemory, rcHeader);

		// 排除列头绘制区域
		dcMemory.ExcludeClipRect(rcHeader);

		// 绘制列表行
		CRect rcListRow = CalcListRect();
		DrawRow(dcMemory, rcHeader, rcListRow);

		// 行坐标已更新
		_lockTipUpdate = false;
	}

	BOOL CTreeListCtrl::OnEraseBkgnd(CDC* pDC)
	{
		return true;
	}

	void CTreeListCtrl::OnMouseMove(UINT nFlags, CPoint point)
	{
		CWnd::OnMouseMove(nFlags, point);

		// 强制设置焦点(防止其他控件抢焦点)
		/*if (::GetFocus() != m_hWnd)
			SetFocus();*/

			// 捕获鼠标离开
		if (!_bMouseOver)
		{
			_bMouseOver = true;

			TRACKMOUSEEVENT trkMouse;
			trkMouse.cbSize = sizeof(TRACKMOUSEEVENT);
			trkMouse.dwFlags = TME_LEAVE;
			trkMouse.hwndTrack = m_hWnd;
			::TrackMouseEvent(&trkMouse);
		}

		// 执行移动列头
		DoDragColumn();

		// 更新鼠标测试
		if (UpdateHitTest(false))
		{
			InvalidateRect(NULL);
		}

		// 处理单元格提示信息
		ProcessCellTooltip();
	}

	BOOL CTreeListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		CWnd::OnMouseWheel(nFlags, zDelta, pt);

		// 滚动条移动后，行坐标为错误只到OnPaint才更新正确
		_lockTipUpdate = true;

		// 重置提示信息
		_cellToolTip.Reset();

		// 如果正在编辑则取消
		CancelEdit();

		SCROLLINFO infoScroll;
		infoScroll.cbSize = sizeof(SCROLLINFO);
		infoScroll.fMask = SIF_ALL;

		if (!GetScrollInfo(SB_VERT, &infoScroll))
			return TRUE;

		int nNewPos = infoScroll.nPos - (zDelta / 5);

		infoScroll.fMask = SIF_POS;
		infoScroll.nPos = nNewPos;
		SetScrollInfo(SB_VERT, &infoScroll);

		InvalidateRect(NULL);

		return TRUE;
	}

	void CTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
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

		CWnd::OnLButtonDown(nFlags, point);
	}

	void CTreeListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// 释放捕获鼠标
		ReleaseCapture();
		_bBtnDown = false;

		bool bDragIng = (NULL != _hDragingCol);

		// 更新鼠标测试
		UpdateHitTest(false);

		if (!bDragIng)
			UpdateColumnSortState();

		// 多行显示文本时滚动条计算错误
		SendMessage(WM_PAINT);

		UpdateScrollBar(SB_VERT);
		InvalidateRect(NULL);

		// 设置选择项;
		if (_hHoverRow != NULL)
		{
			CRect rect;
			int nCol = -1, nRow = 0;
			HCOLUMNDATA hColumnData = NULL;
			IListDataSource* pDataSource = NULL;
			GetFocusRect(nCol, nRow, hColumnData, rect, pDataSource, &point);
			NotifyParent(LCN_LEFTCLICK, nCol, nRow, &hColumnData->editConfig);
		}

		CWnd::OnLButtonUp(nFlags, point);
	}

	void CTreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		CWnd::OnLButtonDblClk(nFlags, point);

		// 隐藏提示
		_cellToolTip.ShowWindow(SW_HIDE);

		// 更新选中行;
		UpdateHitTest(false);
		if (_hHoverRow != NULL)
		{
			// 设置单选择项
			if (SetItemSelectFlag(_hHoverRow))
			{
				NotifyParent(LCN_SELECTED);
			}
		}
		InvalidateRect(NULL);

		// 设置选择项;
		if (_hHoverRow != NULL)
		{
			CRect rect;
			int nCol = -1, nRow = 0;
			HCOLUMNDATA hColumnData = NULL;
			IListDataSource* pDataSource = NULL;
			GetFocusRect(nCol, nRow, hColumnData, rect, pDataSource, &point);

			// 发送双击事件;
			LRESULT pResult = NotifyParent(LCN_DBLCLICK, nCol, nRow, &hColumnData->editConfig);

			//若返回默认值0，则执行编辑
			if(!pResult)
				EditItemByPoint(point, true);
		}
	}

	void CTreeListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
	{
		CWnd::OnRButtonDown(nFlags, point);

		// 隐藏提示
		_cellToolTip.ShowWindow(SW_HIDE);

		InvalidateRect(NULL);
		SetFocus();

		// 非多选的情况下，发送事件

		// 更新选中行;
		UpdateHitTest(false);

		if (_hHoverRow != NULL)
		{
			// 设置单选择项
			if (!_hHoverRow->seleced && SetItemSelectFlag(_hHoverRow))
			{
				NotifyParent(LCN_SELECTED, -1, -1, 0, L"", 0, 1);
			}
		}
		InvalidateRect(NULL);

		// 设置选择项;
		if (_hHoverRow != NULL)
		{
			// 设置选择项;
			ShowRightMenu();

			CRect rect;
			int nCol = -1, nRow = 0;
			HCOLUMNDATA hColumnData = NULL;
			IListDataSource* pDataSource = NULL;
			GetFocusRect(nCol, nRow, hColumnData, rect, pDataSource, &point);

			// 发送消息;
			NotifyParent(LCN_RIGHTCLICK, nCol, nRow, &hColumnData->editConfig);
		}
	}

	LRESULT CTreeListCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
	{
		_bMouseOver = false;

		CPoint curPt;
		GetCursorPos(&curPt);
		ScreenToClient(&curPt);

		// 更新鼠标测试,必须判断
		if (UpdateHitTest(false))
		{
			InvalidateRect(NULL);
		}

		return TRUE;
	}

	LRESULT CTreeListCtrl::OnMouseHover(WPARAM wParam, LPARAM lParam)
	{
		SetFocus();
		return TRUE;
	}

	LRESULT CTreeListCtrl::OnEndEdit(WPARAM wParam, LPARAM lParam)
	{
		if (!IsWindowVisible())
			return TRUE;

		if (_listTreeCombo)
			_listTreeCombo.ResetItemData();

		IListDataSource* pSelectDataSource = GetSelectedItem();
		ATLASSERT(pSelectDataSource != NULL);
		if (pSelectDataSource != NULL)
		{
			ColumnDataVector leafColumnVector;
			GetColumnAllLeafStruct(leafColumnVector);

			if ((size_t)_nEditColIndex >= leafColumnVector.size())
				return FALSE;

			HCOLUMNDATA hColumnData = leafColumnVector[_nEditColIndex];

			// 获取修改后的值与附加数据;
			CString strVal = LPCTSTR(wParam);
			CString strData = LPCTSTR(lParam);
			if (_bIsTrim)
			{
				strVal.Trim();
			}


			// 获取单元格数据;
			CELLINFO cellInfo;
			EDITCONFIG editConfig;
			editConfig.isEditMode = true;
			pSelectDataSource->GetCellData(_nEditColIndex, _nEditRowIndex, hColumnData, &cellInfo, &editConfig, _pConfig);

			CString strOldText = pSelectDataSource->GetCellText(_nEditColIndex, _nEditRowIndex, &editConfig, _pConfig);
			CString strOldData = _pConfig->GetTempData();

			// 新值旧没有修改
			if (strOldText == strVal && strOldData == strData)
			{
				NotifyParent(LCN_CANCELEDIT, (int)_nEditColIndex, (int)_nEditRowIndex, &editConfig, strOldText, &strVal);
				return FALSE;
			}

			if (!OnEndEdit((int)_nEditColIndex, (int)_nEditRowIndex, &editConfig, strOldText, strVal))
				return FALSE;

			// 发送消息通知界面状态更新;
			if (LCNR_OK != NotifyParent(LCN_ENDEDIT, (int)_nEditColIndex, (int)_nEditRowIndex, &editConfig, strOldText, &strVal))
				return FALSE;

			// 传递临时数据
			_pConfig->SetTempData(strData);

			pSelectDataSource->SetCellText(_nEditColIndex, _nEditRowIndex, &editConfig, strVal, _pConfig);

			// 完成编辑
			NotifyParent(LCN_ENDEDITDONE, (int)_nEditColIndex, (int)_nEditRowIndex, &editConfig, strOldText, &strVal);

			InvalidateRect(NULL);
		}

		return TRUE;
	}

	void CTreeListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		CWnd::OnVScroll(nSBCode, nPos, pScrollBar);

		// 如果正在编辑则取消
		CancelEdit();

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

	void CTreeListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

		// 如果正在编辑则取消
		CancelEdit();

		SCROLLINFO infoScroll;
		infoScroll.cbSize = sizeof(SCROLLINFO);
		infoScroll.fMask = SIF_ALL;

		if (!GetScrollInfo(SB_HORZ, &infoScroll))
			return;

		int nNewPos = infoScroll.nPos;
		switch (nSBCode)
		{
		case SB_LEFT:
			nNewPos = infoScroll.nMin;
			break;
		case SB_LINELEFT:
			nNewPos = max(infoScroll.nPos - 5, infoScroll.nMin);
			break;
		case SB_PAGELEFT:
			nNewPos = max(infoScroll.nPos - int(infoScroll.nPage), infoScroll.nMin);
			break;
		case SB_RIGHT:
			nNewPos = infoScroll.nMax;
			break;
		case SB_LINERIGHT:
			nNewPos = min(infoScroll.nPos + 5, infoScroll.nMax);
			break;
		case SB_PAGERIGHT:
			nNewPos = min(int(infoScroll.nPos + infoScroll.nPage), infoScroll.nMax);
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			nNewPos = infoScroll.nTrackPos;
			break;
		}

		infoScroll.fMask = SIF_POS;
		infoScroll.nPos = nNewPos;
		SetScrollInfo(SB_HORZ, &infoScroll);

		InvalidateRect(NULL);
	}

	void CTreeListCtrl::OnSize(UINT nType, int cx, int cy)
	{
		CWnd::OnSize(nType, cx, cy);

		if (NULL != m_hWnd)
		{
			CancelEdit();

			AutoColumnWidth();

			UpdateScrollBar(SB_BOTH);
		}
	}

	/***********************列头绘制***********************/

	CRect CTreeListCtrl::CalcHeaderRect()
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CRect rcHeader = rcClient;

		if (_showHeader)
		{
			int nMaxLevel = 0;
			GetMaxColumnLevel(nMaxLevel);

			rcHeader.bottom = rcHeader.top + _nHeaderHeight*nMaxLevel;
		}
		else
		{
			rcHeader.bottom = rcHeader.top;
		}

		return rcHeader;
	}

	void CTreeListCtrl::DrawHeader(CMemoryDC& dcMemory, const CRect& rcHeader)
	{
		// 列递增宽度
		int nIncreaseWidth = 0;

		// 剩余列头区域
		CRect rcReaminColumn;
		rcReaminColumn.SetRectEmpty();
		int nHorzPos = GetScrollPos(SB_HORZ);

		HGDIOBJ hOldFont = dcMemory.SelectObject(_treeListFont);

		// 获取列头线性结构
		ColumnDataVector columnVector;
		GetColumnLinearityStruct(columnVector);

		// 遍历绘制列
		size_t nCount = columnVector.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			HCOLUMNDATA hColumnData = columnVector[i];

			// 计算列绘制区域
			CRect rcColumn = rcHeader;
			bool bNoParent = (NULL == hColumnData->hParent);

			// 计算父列头区域
			if (bNoParent)
			{
				rcColumn.left = nIncreaseWidth - nHorzPos + rcHeader.left;
				rcColumn.right = rcColumn.left + hColumnData->GetWidth();
				rcColumn.bottom = rcColumn.top + _nHeaderHeight;
			}
			else // 计算子列头区域
			{
				CRect& rcParentCol = hColumnData->hParent->colRect;
				ColumnDataVector& childColumns = hColumnData->hParent->childColumn;

				rcColumn.top = rcParentCol.bottom;
				rcColumn.left = rcParentCol.left;

				// 计算偏起始值
				size_t nChildCol = childColumns.size();
				for (size_t j = 0; j < nChildCol; ++j)
				{
					if (childColumns[j] == hColumnData)
						break;

					rcColumn.left += childColumns[j]->GetWidth();
				}

				rcColumn.bottom = rcColumn.top + _nHeaderHeight;
				rcColumn.right = rcColumn.left + hColumnData->GetWidth();
			}

			// 没有子列头的列下边与表头对齐
			if (0 == hColumnData->childColumn.size())
				rcColumn.bottom = rcHeader.bottom;

			// 更新列头区域
			hColumnData->colRect = rcColumn;

			// 列递增(只计算第一层列头)
			if (bNoParent)
			{
				nIncreaseWidth += hColumnData->GetWidth();
			}

			// 不在显示区域范围内则不绘制(加快性能)
			if (!_showHeader || (rcColumn.left < rcHeader.left && rcColumn.right < rcHeader.left)
				|| (rcColumn.left > rcHeader.right && rcColumn.right > rcHeader.right))
			{
				continue;
			}

			// 临时存储最后列
			if (NULL == hColumnData->hParent)
			{
				rcReaminColumn = rcColumn;
			}

			// 绘制列头列
			DrawColumn(dcMemory, rcColumn, hColumnData);
		}

		// 是否有剩余列头列需要绘制
		if (_showHeader && (rcReaminColumn.right < rcHeader.right))
		{
			rcReaminColumn.left = rcReaminColumn.right;
			rcReaminColumn.right = rcHeader.right;
			rcReaminColumn.bottom = rcHeader.bottom;

			// 绘制剩余列头列
			DrawColumn(dcMemory, rcReaminColumn, NULL);
		}

		dcMemory.SelectObject(hOldFont);
	}

	void CTreeListCtrl::DrawColumn(CMemoryDC& dcMemory, const CRect& rcColumn, HCOLUMNDATA hColumnData)
	{
		// 列头项边距
		int headerItemMargin = 6;

		// 判断列显示外观
		int nState = HIS_NORMAL;

		if (NULL == hColumnData)
		{
			nState = HIS_NORMAL;
		}
		else if (hColumnData == _hPressCol)
		{
			nState = HIS_PRESSED;
		}
		else if (hColumnData == _hHoverCol)
		{
			nState = HIS_HOT;
		}

		// 绘制带皮肤列头
		if (!_headerTheme.IsThemeNull())
		{
			_headerTheme.DrawThemeBackgroundEx(dcMemory, HP_HEADERITEM, nState, rcColumn, NULL);

			CPen pLinePen;
			pLinePen.CreatePen(PS_SOLID, 1, RGB(229, 229, 229));
			HGDIOBJ hOldPen = dcMemory.SelectObject(pLinePen.GetSafeHandle());
			dcMemory.MoveTo(rcColumn.TopLeft());
			dcMemory.LineTo(CPoint(rcColumn.right, rcColumn.top));
			dcMemory.SelectObject(hOldPen);
		}
		else
		{
			dcMemory.FillSolidRect(rcColumn, GetSysColor(COLOR_BTNFACE));
			dcMemory.Draw3dRect(rcColumn, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DSHADOW));
		}

		// 多余列头是不需要绘制文字与图标
		if (NULL == hColumnData)
			return;

		// 是否绘制了图标
		bool bDrawIcon = false;

		// 计算图标区域
		CRect rcIcon = rcColumn;
		rcIcon.DeflateRect(headerItemMargin, 0, headerItemMargin, 0);
		rcIcon.top = rcIcon.CenterPoint().y - _iconSize.cy / 2;

		// 绘制图标
		if (rcIcon.IsRectEmpty())
		{
			bDrawIcon = false;
		}
		else if (_pImageList && (hColumnData->image != -1))
		{
			bDrawIcon = true;
			_pImageList->DrawEx(&dcMemory, hColumnData->image, rcIcon.TopLeft(), _iconSize,
				CLR_DEFAULT, CLR_DEFAULT, ILD_IMAGE);
		}

		// 计算文字置
		CRect rcText = rcColumn;
		if (bDrawIcon)
		{
			rcText.left = (rcIcon.left + _iconSize.cx + headerItemMargin / 2);
		}
		else
		{
			rcText.DeflateRect(headerItemMargin, 0, headerItemMargin, 0);
		}

		// 绘制文字
		if (NULL != hColumnData && !rcText.IsRectEmpty())
		{
			dcMemory.SetBkMode(TRANSPARENT);
			dcMemory.SetTextColor(hColumnData->textColor);

			bool bHasDrawText = false;

			// 自动换行
			if (_bColumnWordWarp)
			{
				UINT nFormat = hColumnData->format | DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX;

				CRect rcCalc = rcText;
				int nHeight = dcMemory.DrawText(hColumnData->title, rcCalc, nFormat | DT_CALCRECT);

				if (rcCalc.Height() <= rcText.Height())
				{
					bHasDrawText = true;
					rcText.OffsetRect(0, (rcText.Height() - nHeight) / 2);
					dcMemory.DrawText(hColumnData->title, rcText, nFormat);
				}
			}

			if (!bHasDrawText)
			{
				dcMemory.DrawText(hColumnData->title, rcText,
					hColumnData->format | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS | DT_NOPREFIX);
			}
		}

		// 绘制排序图标(在列头上方中间绘制)
		if (hColumnData != NULL && hColumnData->sortType != TLC_NO_SORT
			&& hColumnData->IsLeafCol())
		{
			const int sortIcoHeight = 5; //排序图标5像素高

			CPen sortPen(PS_SOLID, 1, hColumnData->textColor);
			HGDIOBJ hOldPen = dcMemory.SelectObject(&sortPen);

			CPoint topCt(rcColumn.left + rcColumn.Width() / 2, rcColumn.top + 1);

			// 升序
			if (hColumnData->sortType == TLC_ASC_SORT)
			{
				dcMemory.MoveTo(topCt);
				dcMemory.LineTo(topCt.x - sortIcoHeight + 1, topCt.y + sortIcoHeight - 1);
				dcMemory.MoveTo(topCt);
				dcMemory.LineTo(topCt.x + sortIcoHeight - 1, topCt.y + sortIcoHeight - 1);
			}
			else if (hColumnData->sortType == TLC_DSC_SORT) // 降序
			{
				dcMemory.MoveTo(topCt.x, topCt.y + sortIcoHeight - 1);
				dcMemory.LineTo(topCt.x - sortIcoHeight + 1, topCt.y);
				dcMemory.MoveTo(topCt.x, topCt.y + sortIcoHeight - 1);
				dcMemory.LineTo(topCt.x + sortIcoHeight - 1, topCt.y);
			}
		}
	}

	/***********************行绘制***********************/

	CRect CTreeListCtrl::CalcListRect()
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CRect rcHeader = CalcHeaderRect();

		CRect rcListRow = rcClient;
		rcListRow.top = rcListRow.top + rcHeader.Height();

		return rcListRow;
	}

	void CTreeListCtrl::DrawRow(CMemoryDC& dcMemory, const CRect& rcHeader, const CRect& rcList)
	{
		// 没有列头
		if (0 == _columnVector.size())
			return;

		// 获取列头叶子节点
		ColumnDataVector leafColumnVector;
		GetColumnAllLeafStruct(leafColumnVector);

		// 行递增高度
		int nIncreaseHeight = 0;
		int nVertPos = GetScrollPos(SB_VERT);

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
			hItemData->expandRect.SetRectEmpty(); // Fix:bw-136479

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

	void CTreeListCtrl::DrawRowBackground(CMemoryDC& dcMemory, HITEMDATA hItemData,
		const CRect& rcRow, ColumnDataVector& leafColumnVector, bool bFirstRow)
	{
		// 绘制选中时背景
		if (hItemData->seleced)
		{
			CRect backgroundArea = rcRow;

			// 排除掉网格线一个像素
			if (_hasGrid)
				backgroundArea.top += 1;

			if (!_headerTheme.IsThemeNull())
			{
				// 没有网格线时绘制边框,免得背景看起来没边框
				if (!_hasGrid)
				{
					CBrush borderBrush;
					borderBrush.CreateSolidBrush(_selBorderColor);
					dcMemory.FrameRect(backgroundArea, &borderBrush);
				}
			}
		}
		else if (_hHoverRow == hItemData)  // 悬浮行背景
		{
			CRect backgroundArea = rcRow;

			// 排除掉网格线一个像素
			if (_hasGrid)
				backgroundArea.top += 1;

			// 没有网格线时绘制边框
			if (!_hasGrid)
			{
				CBrush borderBrush;
				borderBrush.CreateSolidBrush(_hotBorderColor);
				dcMemory.FrameRect(backgroundArea, &borderBrush);
			}
		}

		// 绘制网格
		if (_hasGrid)
		{
			CPen solidPen;
			solidPen.CreatePen(PS_SOLID, 1, _gridColor);

			HGDIOBJ hOldPen = dcMemory.SelectObject(solidPen);

			// 绘制水平网格
			if (bFirstRow)
			{
				dcMemory.MoveTo(rcRow.left, rcRow.top);
				dcMemory.LineTo(rcRow.right, rcRow.top);
			}

			dcMemory.MoveTo(rcRow.left, rcRow.bottom);
			dcMemory.LineTo(rcRow.right, rcRow.bottom);

			bool bHasBorderStyle = !!(GetStyle() & WS_BORDER);

			CRect rcClient;
			GetClientRect(&rcClient);

			// 绘制垂直网格
			size_t nCount = leafColumnVector.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				HCOLUMNDATA hColumnData = leafColumnVector[i];
				const CRect& rcCell = hColumnData->colRect;

				// 防止与边框重叠
				if (i == 0 && !bHasBorderStyle)
				{
					dcMemory.MoveTo(rcCell.left, rcRow.top);
					dcMemory.LineTo(rcCell.left, rcRow.bottom);
				}

				// 防止与边框重叠
				if ((i == nCount - 1) && bHasBorderStyle
					&& rcCell.right == rcClient.right)
					continue;

				dcMemory.MoveTo(rcCell.right - 1, rcRow.top);
				dcMemory.LineTo(rcCell.right - 1, rcRow.bottom);
			}

			dcMemory.SelectObject(hOldPen);
		}
	}

	void CTreeListCtrl::DrawCell(CMemoryDC& dcMemory, const CRect& rcCell, bool bTreeNode, bool bFirstCell,
		HITEMDATA hItemData, HCELLINFO hCellInfo, HEDITCONFIG hEditConfig, HCOLUMNDATA hColumnData, const CString& strCellText)
	{
		/*
		* 注意修改此方法时，查看是否需要同步修改CalcCellTextOutRect方法
		*/
		int leftMargin = 4;	// 单元格左侧边距
		int rightMargin = 4;   // 单元格右侧边距

		CRect backgroundArea = rcCell;

		// 有网格时调整背景区域
		if (_hasGrid)
		{
			backgroundArea.top += 1;
			backgroundArea.right -= 1;

			if (bFirstCell)
				backgroundArea.left += 1;
		}

		// 绘制单元格背景
		if (!IsWindowEnabled())
			dcMemory.FillSolidRect(backgroundArea, DISABLE_BK_COLOR);
		else
		{
			short topr = GetRValue(hCellInfo->backColor);
			short topg = GetGValue(hCellInfo->backColor);
			short topb = GetBValue(hCellInfo->backColor);
			short bottomR = GetRValue(hCellInfo->backColor);
			short bottomG = GetGValue(hCellInfo->backColor);
			short bottomB = GetBValue(hCellInfo->backColor);
			if (hItemData->seleced)
			{
				topr = topr * GetRValue(_selTopColor) / 255;
				topg = topg * GetGValue(_selTopColor) / 255;
				topb = topb * GetBValue(_selTopColor) / 255;
				bottomR = bottomR * GetRValue(_selBottomColor) / 255;
				bottomG = bottomG * GetGValue(_selBottomColor) / 255;
				bottomB = bottomB * GetBValue(_selBottomColor) / 255;
			}
			else if (_hHoverRow == hItemData)
			{
				topr = topr * GetRValue(_hotTopColor) / 255;
				topg = topg * GetGValue(_hotTopColor) / 255;
				topb = topb * GetBValue(_hotTopColor) / 255;
				bottomR = bottomR * GetRValue(_hotBottomColor) / 255;
				bottomG = bottomG * GetGValue(_hotBottomColor) / 255;
				bottomB = bottomB * GetBValue(_hotBottomColor) / 255;
			}
			dcMemory.GradientVert(backgroundArea, RGB(topr, topg, topb), RGB(bottomR, bottomG, bottomB));
		}
			

		// 当前是树节点， 如果不是计算文本区域
		if (bTreeNode)
		{
			int dotLineSpace = _hasLine ? 12 : 9; // 树结构线间距
			int dashLineSpace = 15; // 树结构破折线长

			// 绘制树结构线
			if (_hasLine)
			{
				COLORREF dotColor = GetSysColor(COLOR_ACTIVEBORDER);

				// 绘制起始位置
				int startPos = dotLineSpace;
				for (int nLevel = 0; nLevel < hItemData->treeLevel; ++nLevel)
				{
					bool bIsLastLevel = (nLevel == hItemData->treeLevel - 1);

					// 绘制叶子节点的最右侧一根竖线
					if (hItemData->lastChild && bIsLastLevel)
					{
						dcMemory.DotVLine(rcCell.left + startPos, rcCell.top + 1, rcCell.Height() / 2, dotColor);
					}
					// 如果当前绘制的层级是最后节点，则不绘制
					else if (!hItemData->IsLevelLastChild(hItemData->treeLevel - nLevel - 1))
					{
						dcMemory.DotVLine(rcCell.left + startPos, rcCell.top, rcCell.Height(), dotColor);
					}

					startPos += dotLineSpace;
				}

				// 绘制横向破折线
				if (hItemData->treeLevel != 0)
				{
					CPoint cPt = rcCell.CenterPoint();
					dcMemory.DotHLine(rcCell.left + dotLineSpace*hItemData->treeLevel + 2, cPt.y, dashLineSpace, dotColor);
				}
			}

			// 有子节点则需要绘制+-号
			if (hItemData->hasChild)
			{
				int startPos = (hItemData->treeLevel + 1)*dotLineSpace;

				// 这里+1是根据实际效果微调得到的数值
				CRect rcGlyph(CPoint(rcCell.left + startPos + 1, rcCell.CenterPoint().y + 1), CSize(0, 0));
				rcGlyph.InflateRect(dotLineSpace, dotLineSpace);

				CPoint ptCenter = rcGlyph.CenterPoint();
				DrawIconEx(dcMemory, ptCenter.x - 8, ptCenter.y - 8,
					hItemData->expand ? _hIconCollapse : _hIconExpand, 16, 16, 0, NULL, DI_NORMAL);

				// 记录收缩图标区域;
				hItemData->expandRect = rcGlyph;
				hItemData->expandRect.DeflateRect(1, 1, 1, 1);
			}

			// 计算树型结点的起始边距
			leftMargin = hItemData->treeLevel*dotLineSpace + dashLineSpace + rightMargin + 2;
		}

		// 是否绘制了图标
		bool bDrawIcon = false;

		// 计算图标区域
		CRect rcIcon = rcCell;
		rcIcon.DeflateRect(leftMargin, 0, rightMargin, 0);
		rcIcon.top = rcIcon.CenterPoint().y - _iconSize.cy / 2;

		// 绘制图标
		if (rcIcon.IsRectEmpty())
		{
			bDrawIcon = false;
		}
		else if (_pImageList)
		{
			if (hEditConfig->image != -1)
			{
				bDrawIcon = true;

				if (hEditConfig->bImageCenter)
					rcIcon.left = rcIcon.CenterPoint().x - _iconSize.cx / 2;		//居中

				_pImageList->DrawEx(&dcMemory, hEditConfig->image, rcIcon.TopLeft(), _iconSize,
					CLR_DEFAULT, CLR_DEFAULT, ILD_IMAGE | ILD_TRANSPARENT);
			}
		}

		// 绘制复选框;
		int nOption = hEditConfig->option;
		if ((nOption & eCheckBoxEdit) || (nOption & eCheckBox3StateEdit))
		{
			CRect rcCheckBox;
			rcCheckBox.left = ((backgroundArea.left + backgroundArea.right) / 2) - (_iconSize.cx / 2) - 1;
			rcCheckBox.right = min(rcCheckBox.left + _iconSize.cx, rcCell.right);
			rcCheckBox.top = ((rcCell.top + rcCell.bottom) / 2) - (_iconSize.cy / 2);
			rcCheckBox.bottom = min(rcCheckBox.top + _iconSize.cy, rcCell.bottom);

			int nCheckValue = _ttoi(strCellText);

			if (nOption & eCheckBoxEdit)
			{
				// 普通复选框;
				dcMemory.DrawFrameControl(rcCheckBox, DFC_BUTTON,
					nCheckValue ? DFCS_CHECKED | DFCS_FLAT : DFCS_BUTTONCHECK | DFCS_FLAT);
			}
			else if (nOption & eCheckBox3StateEdit)
			{
				// 三态复选框;
				UINT BtnStyle = DFCS_FLAT;
				if (nCheckValue < 0)
					BtnStyle |= DFCS_BUTTONCHECK;
				else if (nCheckValue > 0)
					BtnStyle |= DFCS_CHECKED;
				else
					BtnStyle |= DFCS_BUTTONPUSH;

				dcMemory.DrawFrameControl(rcCheckBox, DFC_BUTTON, BtnStyle);
			}

			// 绘制边框，用于遮盖 DrawFrameControl 画出来的边框;
			COLORREF crFrame = hCellInfo->readonly ? GetSysColor(COLOR_3DSHADOW) : GetSysColor(COLOR_HIGHLIGHT);
			for (int i = 0; i < 2; i++)
			{
				dcMemory.Draw3dRect(rcCheckBox.left + i, rcCheckBox.top + i,
					rcCheckBox.right - rcCheckBox.left - i * 2, rcCheckBox.bottom - rcCheckBox.top - i * 2,
					crFrame, crFrame);
			}

			return;
		}
		else if (nOption & eProgressEdit)
		{
			// 获取单元格区域及文字
			CRect rcPrg = rcCell;

			// 转化为进度条内容及长度
			int nProgess = _ttoi(strCellText);
			if (nProgess > 100)
			{
				nProgess = 100;
			}

			if (!_progressTheme.IsThemeNull())
			{
				// 绘制进度条外框
				rcPrg.DeflateRect(1, 2, 2, 1);
				_progressTheme.DrawThemeBackground(dcMemory, PP_BAR, 0, rcPrg);

				// 绘制进度条填充
				rcPrg.right -= rcPrg.Width() * (100 - nProgess) / 100;
				_progressTheme.DrawThemeBackground(dcMemory, PP_CHUNK, 0, rcPrg);
			}
			else
			{
				rcPrg.DeflateRect(1, 2, 2, 1);
				CRect rcFrame = rcPrg;

				// 绘制进度条填充
				rcPrg.right -= rcPrg.Width() * (100 - nProgess) / 100;
				dcMemory.FillSolidRect(rcPrg, _selBottomColor);

				// 绘制进度条外框
				dcMemory.FrameRect(rcFrame, &CBrush(_selBorderColor));
			}

			// 显示文字
			if (nOption & ePrgShowText)
			{
				CString strProg;
				strProg.Format(_T("%d%%"), nProgess);
				rcPrg = rcCell;

				// 文字居中，背景透明
				dcMemory.SetBkMode(TRANSPARENT);
				dcMemory.DrawText(strProg, rcPrg, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS | DT_NOPREFIX);
			}
			return;
		}

		if (nOption & eHyperLinkEdit)
		{
			dcMemory.SelectObject(_underlineFont);
			dcMemory.SetTextColor(RGB(0, 102, 204));
		}
		else
		{
			dcMemory.SetTextColor(hCellInfo->textColor);
		}

		// 计算文字置
		CRect rcText = rcCell;
		if (bDrawIcon)
		{
			rcText.left = (rcIcon.left + _iconSize.cx + rightMargin / 2);
		}
		else
		{
			rcText.DeflateRect(leftMargin, 0, rightMargin, 0);
		}

		if (hEditConfig->bImageCenter)
			return;

		// 绘制文字
		if (!rcText.IsRectEmpty() && !strCellText.IsEmpty())
		{
			dcMemory.SetBkMode(TRANSPARENT);

			if (_bRowWordWarp)
			{
				rcText.OffsetRect(0, (rcText.Height() - hCellInfo->wpHeight) / 2);
				dcMemory.DrawText(strCellText, rcText, hColumnData->format | DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX);
			}
			else
			{
				if (hEditConfig->option & eDiffColText)
					dcMemory.DrawColorText(strCellText, rcText,
						hColumnData->format | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS | DT_NOPREFIX);
				else
					dcMemory.DrawText(strCellText, rcText,
						hColumnData->format | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS | DT_NOPREFIX);
			}
		}
	}

	BOOL CTreeListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
		if (_hDragingCol != NULL)
		{
			// 设置移动列头光标
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			return TRUE;
		}
		else if (_hHoverRow != NULL)  // 设置超链接手掌光标
		{
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);

			CRect rect = _hHoverRow->rowRect;

			// 获取列头叶子节点
			ColumnDataVector leafColumnVector;
			GetColumnAllLeafStruct(leafColumnVector);

			size_t nCount = leafColumnVector.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				HCOLUMNDATA hColumnData = leafColumnVector[i];

				// 测试是否在区域;
				rect.left = hColumnData->colRect.left;
				rect.right = hColumnData->colRect.right;

				// 鼠标单击时位置是否在控件区域内;
				if (rect.PtInRect(point))
				{
					CELLCONFIG cellCfg;
					_hHoverRow->pDataSource->GetCellData(i, _nHoverRowIndex, hColumnData, &cellCfg.cellInfo, &cellCfg.editConfig, _pConfig);
					if (cellCfg.editConfig.option & eHyperLinkEdit && !cellCfg.cellInfo.readonly)
					{
						SetCursor(LoadCursor(NULL, IDC_HAND));
						return TRUE;
					}
				}
			}
		}

		return CWnd::OnSetCursor(pWnd, nHitTest, message);
	}

	bool CTreeListCtrl::SetItemSelectFlag(HITEMDATA hItemSelect)
	{
		bool bChange = !_bCheckSelChange;
		short nCtrlState = 0;
		short nShiftState = 0;

		// 多选时检测Ctrl和Shift键按下状态
		if (!_bSingleSelect)
		{
			nCtrlState = GetKeyState(VK_CONTROL);
			nShiftState = GetKeyState(VK_SHIFT);
		}

		ListDataSourceIter endIter = _listDataSource.end();

		if (nShiftState >> 8)
		{
			// 按下Shift点击
			int nSelectRow = -1;
			ListDataSourceIter iter = _listDataSource.begin();
			for (int nRow = 0; iter != endIter; ++iter, ++nRow)
			{
				// 找到选中行的行号
				if (iter->second == hItemSelect)
				{
					nSelectRow = nRow;
				}
			}

			iter = _listDataSource.begin();
			for (int nRow = 0; iter != endIter; ++iter, ++nRow)
			{
				// 当前选择状态不满足SHIFT功能
				if (nSelectRow < 0 || _nLastSelectItem < 0)
				{
					// 只选中点击的行
					if (iter->second == hItemSelect)
					{
						if (!hItemSelect->seleced)
						{
							bChange = true;
							hItemSelect->seleced = true;
						}

						_nLastSelectItem = nRow;
						break;
					}

					continue;
				}

				if ((nRow <= nSelectRow && nRow >= _nLastSelectItem)
					|| (nRow <= _nLastSelectItem && nRow >= nSelectRow))
				{
					// 在点击行和上一选中行之间的行全被选中
					if (!iter->second->seleced)
					{
						iter->second->seleced = true;
						bChange = true;
					}
				}
				else
				{
					// 不满足条件的行不选中
					if (iter->second->seleced)
					{
						iter->second->seleced = false;
						bChange = true;
					}
				}
			}
		}
		else if (nCtrlState >> 8)
		{
			// 按下Ctrl点击
			ListDataSourceIter iter = _listDataSource.begin();
			for (int nRow = 0; iter != endIter; ++iter, ++nRow)
			{
				// 只要点击了都选中
				if (iter->second == hItemSelect)
				{
					bChange = true;

					// CTRL可以选中也可以取消
					hItemSelect->seleced = (!hItemSelect->seleced);

					if (hItemSelect->seleced)
					{
						// 最后一次“选中”的行更新为上一选中行
						_nLastSelectItem = nRow;
					}
				}
			}
		}
		else
		{
			// 单纯点击状态
			ListDataSourceIter iter = _listDataSource.begin();
			for (int nRow = 0; iter != endIter; ++iter, ++nRow)
			{
				// 选中点击行，去点其他行的选中状态
				if (iter->second == hItemSelect)
				{
					if (!iter->second->seleced)
						bChange = true;

					hItemSelect->seleced = true;
					_nLastSelectItem = nRow;
				}
				else
				{
					if (iter->second->seleced)
						bChange = true;
					iter->second->seleced = false;
				}
			}
		}

		return bChange;
	}

	LRESULT CTreeListCtrl::NotifyParent(UINT nMessage, int nCol, int nRow, HEDITCONFIG hEditCfg,
		const CString& strCellText, CString* pStrNewText, int nClickType)
	{
		CTreeListNotify notify;
		notify.hdrNotify.hwndFrom = m_hWnd;
		notify.hdrNotify.idFrom = GetDlgCtrlID();
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

	void CTreeListCtrl::DeleteControl()
	{
		if (_pControl)
		{
			if (IsWindow(_pControl->GetSafeHwnd()))
			{
				HWND hChild = _pControl->m_hWnd;

				// 支持子窗口多层嵌套,最多3层
				for (int i = 0; i < 3; ++i)
				{
					HWND hParent = ::GetParent(hChild);
					if (hParent == m_hWnd)
					{
						::DestroyWindow(hChild);
						break;
					}
					hChild = hParent;
				}
			}

			_pControl = NULL;
		}
	}

	LRESULT CTreeListCtrl::OnHyperLink(WPARAM wParam, LPARAM lParam)
	{
		int nCol = int(wParam);
		int nRow = int(lParam);

		size_t nSelRow = GetSelectedIndex();
		if (nRow != nSelRow)
			return TRUE;

		// 进入到此消息时可能数据源进行了修改
		IListDataSource* pSelectDataSource = GetSelectedItem();
		if (pSelectDataSource == NULL)
			return TRUE;

		ColumnDataVector leafColumnVector;
		GetColumnAllLeafStruct(leafColumnVector);

		HCOLUMNDATA hColumnData = leafColumnVector[nCol];

		// 获取单元格数据
		CELLINFO cellInfo;
		EDITCONFIG editConfig;
		editConfig.isEditMode = true;
		pSelectDataSource->GetCellData(nCol, nRow, hColumnData, &cellInfo, &editConfig, _pConfig);

		if (cellInfo.readonly)
			return TRUE;

		CString strEventHandle = editConfig.eventHandle;
		if (!strEventHandle.IsEmpty())
		{
			/*IEventHandle *pHandle = (IEventHandle*)GET_OSGIOBJECT((LPCTSTR)strEventHandle);
			_bLinkOn = true;
			pHandle->ExcuteHandle(this, nCol, nRow, pSelectDataSource, &editConfig, _pConfig);
			_bLinkOn = false;
			delete pHandle;*/

			return TRUE;
		}

		_bLinkOn = true;
		// 超链接，则发送消息由用户自己处理;
		NotifyParent(LCN_HYPERLINK, (int)nCol, (int)nRow, &editConfig);
		_bLinkOn = false;

		return TRUE;
	}

	void CTreeListCtrl::EditFocusItem(int nCol)
	{
		CRect rect;
		int nRow = 0;
		HCOLUMNDATA hColumnData = NULL;
		IListDataSource* pDataSource = NULL;

		if (!GetFocusRect(nCol, nRow, hColumnData, rect, pDataSource, NULL))
			return;

		EditItemByParam(pDataSource, nRow, nCol, rect, hColumnData, true);
	}

	void CTreeListCtrl::EditItemByPoint(CPoint ptMs, bool bIsDbClk)
	{
		if (_hHoverRow == NULL)
			return;

		CRect rect;
		int nCol = -1, nRow = 0;
		HCOLUMNDATA hColumnData = NULL;
		IListDataSource* pDataSource = NULL;

		if (!GetFocusRect(nCol, nRow, hColumnData, rect, pDataSource, &ptMs))
			return;

		EditItemByParam(pDataSource, nRow, nCol, rect, hColumnData, bIsDbClk);
	}

	void CTreeListCtrl::EditItemByParam(IListDataSource* pDataSource,
		int nRow, int nCol, CRect rect, HCOLUMNDATA hColumnData, bool bIsDbClk)
	{
		// 获取单元格数据
		CELLINFO cellInfo;
		EDITCONFIG editConfig;
		editConfig.isEditMode = true;
		pDataSource->GetCellData(nCol, nRow, hColumnData, &cellInfo, &editConfig, _pConfig);

		if (cellInfo.readonly)
			return;

		_nEditRowIndex = nRow;
		_nEditColIndex = nCol;

		// 修改区域大小请到各自创建函数中修改
		rect.top += 2;
		rect.right -= 1;
		rect.bottom -= 1;

		int nOption = editConfig.option;

		CString strNewText;
		CString strOldText = pDataSource->GetCellText(nCol, nRow, &editConfig, _pConfig);
		CString strOldData = _pConfig->GetTempData();

		if (!OnBeginEdit((int)nCol, (int)nRow, &editConfig, strOldText))
			return;

		if (NotifyParent(LCN_BEGINEDIT, (int)nCol, (int)nRow, &editConfig, strOldText, NULL) != LCNR_OK)
			return;

		if (!bIsDbClk)
		{
			if ((nOption & eCheckBoxEdit) || (nOption & eCheckBox3StateEdit))
			{
				if ((nOption & eCheckBoxEdit))
				{
					// 普通复选框;
					strNewText = _ttoi(strOldText) > 0 ? _T("0") : _T("1");
				}
				else
				{
					// 三态复选框;
					int nCheckImage = _ttoi(strOldText);
					if (nCheckImage < 0)
						strNewText = _T("0");
					else if (nCheckImage > 0)
						strNewText = _T("-1");
					else
						strNewText = _T("1");
				}

				// 没有修改
				if (strNewText == strOldText)
					return;

				if (!OnEndEdit((int)nCol, (int)nRow, &editConfig, strOldText, strNewText))
					return;

				if (LCNR_OK != NotifyParent(LCN_ENDEDIT, (int)nCol, (int)nRow, &editConfig, strOldText, &strNewText))
					return;

				pDataSource->SetCellText((int)nCol, (int)nRow, &editConfig, strNewText, _pConfig);

				// 完成编辑
				NotifyParent(LCN_ENDEDITDONE, (int)_nEditColIndex, (int)_nEditRowIndex, &editConfig, strOldText, &strNewText);
			}
			else if (nOption & eHyperLinkEdit)
			{
				// 超链接，则发送消息由用户自己处理;
				// 发送消息给自己，让点击消息延迟处理优先让其它编辑控件结束编辑
				PostMessage(LCN_HYPERLINK, nCol, nRow);
			}
		}
		// 创建编辑控件
		else if (bIsDbClk)
		{
			CreateEditorControl(nOption, editConfig, rect, strOldText, strOldData);
		}
	}

	void CTreeListCtrl::CreateEditorControl(int nOption,
		EDITCONFIG editConfig,
		CRect rect,
		const CString& strDefaultValue,
		const CString& strDefaultData)
	{
		if ((nOption & eNormalEdit) || (nOption & eValidEdit) || (nOption & eFileNameEdit))
		{
			DeleteControl();
			if (!_listEdit.Create(this, rect, strDefaultValue, _bRowWordWarp))
				return;

			_pControl = &_listEdit;
			_listEdit.SetFont(&_treeListFont);
			_listEdit.SetEditFlag(editConfig.option);
			_listEdit.SetDotNum(editConfig.dotNum);

			// 调整光是显示区域，必须启用ES_MULTILINE风格
			CRect rcRect;
			_listEdit.GetRect(&rcRect);
			rcRect.DeflateRect(4, 5, 0, -5);
			_listEdit.SetRect(rcRect);

			_listEdit.ShowWindow(SW_SHOW);
			_listEdit.SetFocus();
		}
		else if (nOption & eDropListCobEdit || nOption & eDropDownCobEdit)
		{
			DeleteControl();
			bool bDropList = !!(editConfig.option & eDropListCobEdit);

			// 创建控件;
			if (!_listCombo.Create(this, rect, bDropList, strDefaultValue))
				return;

			// 加载下拉数据
			_listCombo.SetFont(&_treeListFont);
			CString strXmlFile = CPathConfig::GetSystemPath() + editConfig.comboDataFile;
			if (!_listCombo.LoadXmlData(strXmlFile))
				_listCombo.LoadStringVector(editConfig.comboList);

			_pControl = &_listCombo;

			// 当没有下拉数据时不显示太长的下拉框
			if (_listCombo.GetCount() == 0)
			{
				CRect rc;
				_listCombo.GetWindowRect(&rc);
				rc.bottom = rc.top + 50;
				_listCombo.GetParent()->ScreenToClient(&rc);
				_listCombo.MoveWindow(rc);
			}

			// 调整编辑框位置
			CWnd* pWndEdit = _listCombo.GetWindow(GW_CHILD);
			if (!bDropList && (NULL != pWndEdit))
			{
				CRect rc;
				pWndEdit->GetWindowRect(&rc);
				rc.DeflateRect(2, 4, 0, -2);
				_listCombo.ScreenToClient(&rc);
				pWndEdit->MoveWindow(rc);
			}
			_listCombo.ShowWindow(SW_SHOW);
			_listCombo.SetSelAndFocus();
		}
		else if (nOption & eDateTimeEdit)
		{
			DeleteControl();

			bool bSuccess = false;
			if (eDateYear & editConfig.option)
				bSuccess = _listDataTime.Create(this, rect, strDefaultValue, true);
			else
				bSuccess = _listDataTime.Create(this, rect, strDefaultValue);

			if (!bSuccess)
				return;

			_pControl = &_listDataTime;
			_listDataTime.SetFont(&_treeListFont);

			if (eDateSlashYMD & editConfig.option)
				_listDataTime.SetFormat(L"yyyy/MM/dd");
			else if (eDateChineseYMD & editConfig.option)
				_listDataTime.SetFormat(L"yyyy年MM月dd日");
			else if (eDateDashYMD & editConfig.option)
				_listDataTime.SetFormat(L"yyyy-MM-dd");
			else if (eDateYear & editConfig.option)
			{
				_listDataTime.SetFormat(L"yyyy");
			}

			_listDataTime.ShowWindow(SW_SHOW);
			_listDataTime.SetFocus();
		}
		else if (nOption & eAngleEdit)
		{
			DeleteControl();
			if (!_listAngleEdit.Create(this, rect, strDefaultValue))
				return;

			_pControl = &_listAngleEdit;
			_listAngleEdit.SetFont(&_treeListFont);

			// 调整光是显示区域，必须启用ES_MULTILINE风格
			CRect rcRect;
			_listAngleEdit.GetRect(&rcRect);
			rcRect.DeflateRect(4, 5, -4, -5);
			_listAngleEdit.SetRect(rcRect);

			_listAngleEdit.ShowWindow(SW_SHOW);
			_listAngleEdit.SetEditFocus();
		}
		else if (nOption & eTreeComboEdit || (nOption & eTreeComboManualEdit))
		{
			DeleteControl();

			_listTreeCombo.SetCheckMode(false);
			_listTreeCombo.SetSplitChar(editConfig.splitChar);

			// 自适应宽度或随空间宽度
			if (nOption&eTreeAutoWidth)
				_listTreeCombo.SetDroppedWidth(0);
			else
				_listTreeCombo.SetDroppedWidth(-1);

			if (nOption&eTreeSelNotByText)
				_listTreeCombo.SetSelectByText(false);
			else
				_listTreeCombo.SetSelectByText(true);

			// 创建控件;
			bool bDropList = !!(editConfig.option & eTreeComboEdit);
			if (!_listTreeCombo.CreateCtrl(this, rect, strDefaultValue, strDefaultData, bDropList))
				return;

			// 加载下拉数据
			CString strXmlFile = CPathConfig::GetSystemPath();
			strXmlFile += editConfig.comboDataFile;

			// 优先加载文件
			if (editConfig.comboDataFile.IsEmpty())
			{
				_listTreeCombo.LoadStringVector(editConfig.comboList);
			}
			else if (CFileTool::FileExist((LPCTSTR)strXmlFile))
			{
				_listTreeCombo.LoadXmlData(strXmlFile, !!(nOption&eTreeHasNoLimit));
			}
			else // 加载XML字符串
			{
				_listTreeCombo.LoadXmlData(editConfig.comboDataFile, !!(nOption&eTreeHasNoLimit));
			}

			_pControl = &_listTreeCombo;
			_listTreeCombo.SetFont(&_treeListFont);
			_listTreeCombo.SetSelectData(strDefaultValue, strDefaultData);
			_listTreeCombo.ShowWindow(SW_SHOW);
			_listTreeCombo.SetFocus();
		}
		else if (nOption & eTreeCheckComboEdit || (nOption & eTreeCheckComboManualEdit))
		{
			DeleteControl();

			_listTreeCombo.SetCheckMode(true);
			_listTreeCombo.SetSplitChar(editConfig.splitChar);

			// 自适应宽度
			_listTreeCombo.SetDroppedWidth(0);

			if (nOption&eTreeSelNotByText)
				_listTreeCombo.SetSelectByText(false);
			else
				_listTreeCombo.SetSelectByText(true);

			// 创建控件;
			bool bDropList = !!(editConfig.option & eTreeCheckComboEdit);
			if (!_listTreeCombo.CreateCtrl(this, rect, strDefaultValue, strDefaultData, bDropList))
				return;

			// 加载下拉数据
			CString strXmlFile = CPathConfig::GetSystemPath() + editConfig.comboDataFile;

			// 优先加载文件
			if (editConfig.comboDataFile.IsEmpty())
			{
				_listTreeCombo.LoadStringVector(editConfig.comboList, !!(nOption & eTreeHasSelAll));
			}
			else if (CFileTool::FileExist((LPCTSTR)strXmlFile))
			{
				_listTreeCombo.LoadXmlData(strXmlFile, !!(nOption & eTreeHasSelAll));
			}
			else // 加载XML字符串
			{
				_listTreeCombo.LoadXmlData(editConfig.comboDataFile, !!(nOption & eTreeHasSelAll));
			}

			_pControl = &_listTreeCombo;
			_listTreeCombo.SetFont(&_treeListFont);
			_listTreeCombo.ShowWindow(SW_SHOW);
			_listTreeCombo.SetFocus();
		}
		else if (nOption & eAutoSearchCombo)
		{
			DeleteControl();

			// 创建控件;
			if (!_autoCombo.Create(this, rect, false, strDefaultValue))
				return;

			// 加载下拉数据
			_autoCombo.SetFont(&_treeListFont);

			CString strXmlFile = CPathConfig::GetSystemPath();
			strXmlFile += editConfig.comboDataFile;
			if (!_autoCombo.LoadXmlData(strXmlFile))
				_autoCombo.LoadStringVector(editConfig.comboList);

			_pControl = &_autoCombo;

			// 当没有下拉数据时不显示太长的下拉框
			if (_autoCombo.GetCount() == 0)
			{
				CRect rc;
				_autoCombo.GetWindowRect(&rc);
				rc.bottom = rc.top + 50;
				_autoCombo.GetParent()->ScreenToClient(&rc);
				_autoCombo.MoveWindow(rc);
			}

			// 调整编辑框位置
			CWnd* pWndEdit = _autoCombo.GetWindow(GW_CHILD);

			if (pWndEdit)
			{
				CRect rc;
				pWndEdit->GetWindowRect(&rc);
				rc.DeflateRect(2, 4, 0, -2);
				_autoCombo.ScreenToClient(&rc);
				pWndEdit->MoveWindow(rc);
			}

			_autoCombo.ShowWindow(SW_SHOW);
			_autoCombo.SetSelAndFocus();
		}
	}

	bool CTreeListCtrl::GetFocusRect(int &nCol, int &nRow, HCOLUMNDATA& hColData,
		CRect &rect, IListDataSource* &pDataSource, CPoint* ptMs)
	{
		// 确定选中行
		nRow = -1;
		HITEMDATA hItemData = NULL;
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin();
			iter != endIter; ++iter)
		{
			++nRow;
			if (iter->second->seleced)
			{
				hItemData = iter->second;
				break;
			}
		}

		if (NULL == hItemData)
			return false;

		rect = hItemData->rowRect;
		pDataSource = hItemData->GetDataSource();

		// 获取列头叶子节点
		ColumnDataVector leafColumnVector;
		GetColumnAllLeafStruct(leafColumnVector);

		size_t nCount = leafColumnVector.size();
		if (-1 != nCol)
		{
			if (nCol < 0 || nCol >= int(nCount))
				return false;

			HCOLUMNDATA hColumnData = leafColumnVector[nCol];

			// 测试是否在区域;
			rect.left = hColumnData->colRect.left;
			rect.right = hColumnData->colRect.right;
			hColData = hColumnData;

			return true;
		}

		if (NULL == ptMs)
			return false;

		// 遍历列头
		for (size_t i = 0; i < nCount; ++i)
		{
			HCOLUMNDATA hColumnData = leafColumnVector[i];

			// 测试是否在区域;
			rect.left = hColumnData->colRect.left;
			rect.right = hColumnData->colRect.right;

			// 鼠标单击时位置是否在控件区域内;
			if (rect.PtInRect(*ptMs))
			{
				nCol = (int)i;
				hColData = hColumnData;
				return true;
			}
		}

		return false;
	}

	bool CTreeListCtrl::OnBeginEdit(int nCol, int nRow,
		HEDITCONFIG hEditCfg, const CString& strOldCellText)
	{
		return true;
	}

	bool CTreeListCtrl::OnEndEdit(int nCol, int nRow,
		HEDITCONFIG hEditCfg, const CString& strOldCellText, CString& strNewCellText)
	{
		return true;
	}

	bool CTreeListCtrl::ShowRightMenu()
	{
		return true;
	}

	void CTreeListCtrl::SetFocusItemByKey(size_t nKey)
	{
		size_t nRow = 0;
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin(); iter != endIter; ++iter)
		{
			if (iter->second->pDataSource->GetKey() == nKey)
			{
				SetFocusItem(nRow);
				break;
			}
			nRow++;
		}
	}

	void CTreeListCtrl::ShowHeader(bool bShow)
	{
		_showHeader = bShow;
	}

	void CTreeListCtrl::SetHasLine(bool hasLine)
	{
		_hasLine = hasLine;
	}

	void CTreeListCtrl::SetHasGrid(bool bShow)
	{
		_hasGrid = bShow;
	}

	int CTreeListCtrl::GetCtrlHeight()
	{
		int nHeight = 0;

		if (_showHeader)
		{
			nHeight += CalcHeaderRect().Height();
		}

		// 统计行总高度
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin();
			iter != endIter; ++iter)
		{
			HITEMDATA hParentData = iter->second->hParentData;
			if (NULL != hParentData && hParentData->IsCollapse())
				continue;

			CRect& rcRow = iter->second->rowRect;

			// 当没有绘制初始化时还是使用默认高度
			if (_bRowWordWarp && rcRow.Height() != 0)
				nHeight += rcRow.Height();
			else
				nHeight += _nRowHeight;
		}

		return nHeight + 1;  ///< 加1防止滚动条出现
	}

	void CTreeListCtrl::SetHeadItemHeight(int nHeight)
	{
		if (nHeight <= 0)
			return;

		_nHeaderHeight = nHeight;
	}

	void CTreeListCtrl::SetTreeColumn(int nColumnIndex /*= 0*/)
	{
		_treeColumn = nColumnIndex;
	}

	void CTreeListCtrl::EnableColumnSort(bool bEnable)
	{
		_bColumnSort = bEnable;
	}

	void CTreeListCtrl::CalcCellTextOutRect(CDC* pDc, const CString& strText,
		CRect& rcCell, bool bTreeNode, HITEMDATA hItemData, HEDITCONFIG hEditConfig,
		HCOLUMNDATA hColumnData, CRect& rcTextOut)
	{
		CMemoryDC* pDcTest = (CMemoryDC*)pDc;

		int leftMargin = 4;	// 单元格左侧边距
		int rightMargin = 4;   // 单元格右侧边距
		int dotLineSpace = _hasLine ? 12 : 9; // 树结构线间距; // 树结构线间距
		int dashLineSpace = 15; // 树结构破折线长

		// 当前是树节点， // 计算树型结点的起始边距
		if (bTreeNode)
		{
			leftMargin = hItemData->treeLevel*dotLineSpace + dashLineSpace + rightMargin + 2;
		}

		// 是否绘制了图标
		bool bDrawIcon = false;

		// 计算图标区域
		CRect rcIcon = rcCell;
		rcIcon.DeflateRect(leftMargin, 0, rightMargin, 0);
		rcIcon.top = rcIcon.CenterPoint().y - _iconSize.cy / 2;

		// 绘制图标
		if (rcIcon.IsRectEmpty())
		{
			bDrawIcon = false;
		}
		else if (_pImageList)
		{
			if (hEditConfig->image != -1)
				bDrawIcon = true;
		}

		// 计算文字置
		if (bDrawIcon)
		{
			rcCell.left = (rcIcon.left + _iconSize.cx + rightMargin / 2);
		}
		else
		{
			rcCell.DeflateRect(leftMargin, 0, rightMargin, 0);
		}

		rcTextOut = rcCell;

		if (!_bRowWordWarp)
		{
			if (hEditConfig->option & eDiffColText)
				pDcTest->DrawColorText(strText, rcTextOut, hColumnData->format | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT | DT_NOPREFIX);
			else
				pDcTest->DrawText(strText, rcTextOut, hColumnData->format | DT_SINGLELINE | DT_VCENTER | DT_CALCRECT | DT_NOPREFIX);

			rcTextOut.bottom = rcCell.bottom + 1;
		}
		else
		{
			pDcTest->DrawText(strText, rcTextOut, hColumnData->format | DT_EDITCONTROL | DT_WORDBREAK | DT_NOPREFIX | DT_CALCRECT);
		}
	}

	void CTreeListCtrl::ProcessCellTooltip()
	{
		if (_lockTipUpdate || (NULL == _hHoverRow)
			|| (-1 == _nHoverRowIndex) || _bLinkOn || _bRowWordWarp)
		{
			return;
		}

		// 获取鼠标位置
		CPoint mPt;
		GetCursorPos(&mPt);
		ScreenToClient(&mPt);

		CRect rcWnd;
		GetWindowRect(&rcWnd);
		ScreenToClient(&rcWnd);

		// 检查鼠标是否在窗口区域内，才处理提示信息
		if (!rcWnd.PtInRect(mPt))
			return;

		int nHoverColIndex = -1;
		CRect rcCell = _hHoverRow->rowRect;

		HCOLUMNDATA hColumnData = NULL;

		// 获取列头叶子节点
		ColumnDataVector leafColumnVector;
		GetColumnAllLeafStruct(leafColumnVector);

		// 遍历列头
		size_t nCount = leafColumnVector.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			hColumnData = leafColumnVector[i];

			// 测试是否在区域;
			rcCell.left = hColumnData->colRect.left;
			rcCell.right = hColumnData->colRect.right;

			// 鼠标单击时位置是否在控件区域内;
			if (rcCell.PtInRect(mPt))
			{
				nHoverColIndex = (int)i;
				break;
			}
		}

		// 没找到列头
		if (-1 == nHoverColIndex)
			return;

		CELLINFO cellInfo;
		EDITCONFIG editConfig;
		_hHoverRow->GetDataSource()->GetCellData(nHoverColIndex, _nHoverRowIndex,
			hColumnData, &cellInfo, &editConfig, _pConfig);

		// 不显示提示信息，则不需要取文本信息
		if (!editConfig.isShowTips)
			return;

		// 获取单元格文文本
		CString strCellText = _hHoverRow->GetDataSource()->GetCellText(nHoverColIndex,
			_nHoverRowIndex, &editConfig, _pConfig);

		// 先设置显示区域为零，解决第一次显示tooltip后修改成短名称后再次显示错误问题
		_cellToolTip.SetRect(CRect(0, 0, 0, 0), CRect(0, 0, 0, 0));

		// 文本为空且不是在编辑状态
		if (strCellText.IsEmpty() || (_pControl != NULL && _pControl->GetSafeHwnd() != NULL && _pControl->IsWindowVisible()))
			return;

		CClientDC dc(this);
		CFont* pOldFont = dc.SelectObject(&_treeListFont);

		// 判断是否需要显现提示信息
		CRect rcTipRect;
		CalcCellTextOutRect(&dc, strCellText, rcCell, (nHoverColIndex == _treeColumn && (!_bNoChild || _hHoverRow->hParentData != NULL)),
			_hHoverRow, &editConfig, hColumnData, rcTipRect);
		dc.SelectObject(pOldFont);

		if (!editConfig.bImageCenter)
		{
			if (rcTipRect.Width() <= rcCell.Width())
				return;
		}
		else
		{
			//图片居中显示
			double dWidth = rcTipRect.Width();
			rcTipRect.left = rcCell.right;
			rcTipRect.right = rcTipRect.left + dWidth;
		}

		_cellToolTip.SetClrText(!!(editConfig.option&eDiffColText));
		_cellToolTip.SetTextMode(hColumnData->format);
		_cellToolTip.SetTipText(strCellText);
		_cellToolTip.SetRect(rcCell, rcTipRect);
	}

	void CTreeListCtrl::AutoColumnWidth()
	{
		CRect rcNowClient;
		GetClientRect(&rcNowClient);

		if (!_bAutoColumnWidth)
		{
			return;
		}

		size_t nCount = _columnVector.size();
		int nSumColumnWidth = 0;

		// 获取列累计宽度
		for (size_t i = 0; i < nCount; ++i)
		{
			nSumColumnWidth += _columnVector[i]->GetWidth();
		}

		std::vector<int> vecWidthArr;

		int nMinWidth = rcNowClient.Width();

		// 按比例增加列宽
		for (size_t i = 0; i < nCount; ++i)
		{
			double dScale = (double)_columnVector[i]->GetWidth() / nSumColumnWidth;

			int nNewWidth = (int)(dScale * rcNowClient.Width());

			// 找到最小列宽
			if (nNewWidth < nMinWidth)
			{
				nMinWidth = nNewWidth;
			}

			// 存好更新后的列宽
			vecWidthArr.push_back(nNewWidth);
		}

		// 根据新列宽中最小的，进行补足
		int nDiff = nMinWidth - 35;
		if (nDiff < 0)
		{
			nDiff = 35 - nMinWidth;
		}
		else
		{
			nDiff = 0;
		}

		for (size_t i = 0; i < nCount; ++i)
		{
			_columnVector[i]->SetWidth(vecWidthArr.at(i) + nDiff);
		}
	}

	void CTreeListCtrl::SetAutoColumnWidth(bool bIsAutoColumnWidth /*= true*/)
	{
		_bAutoColumnWidth = bIsAutoColumnWidth;
	}

	bool CTreeListCtrl::SetExpandBySelectItem(bool bExpand /*= true*/)
	{
		bool bFlag = false;

		// 遍历当前选择的记录;
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin(); iter != endIter; ++iter)
		{
			// 选中则将状态修改
			if (iter->second->seleced)
			{
				iter->second->expand = bExpand;
				bFlag = true;
			}
		}

		return bFlag;
	}

	bool CTreeListCtrl::SetExpandItemByKey(size_t nKey, bool bExpand /*= true*/, bool bParent /*= false*/)
	{
		bool bFlag = false;

		// 遍历数据源来找到对应KEY
		ListDataSourceIter endIter = _listDataSource.end();
		for (ListDataSourceIter iter = _listDataSource.begin(); iter != endIter; ++iter)
		{
			if (iter->first == nKey)
			{
				iter->second->expand = bExpand;
				bFlag = true;

				// 递归父节点并设置展开状态
				HITEMDATA ParentInfo = iter->second->hParentData;
				while (bParent && ParentInfo != NULL)
				{
					ParentInfo->expand = bExpand;
					ParentInfo = ParentInfo->hParentData;
				}

				break;
			}
		}

		return bFlag;
	}

	BOOL CTreeListCtrl::PreTranslateMessage(MSG* pMsg)
	{
		// 上下按键标志位置否
		_bUpDownFlag = false;

		// 检测键盘按下的事件消息
		if (pMsg->message == WM_KEYDOWN &&
			(_pControl == NULL || _pControl->GetSafeHwnd() == NULL || !_pControl->IsWindowVisible()))
		{
			// 有子控件显示时，上下按键不响应
			switch (pMsg->wParam)
			{
			case VK_UP:
				// 上下键标志位
				_bUpDownFlag = true;

				// 设置选中
				SetSelRowByKey(false);

				// 刷新
				Invalidate(TRUE);
				break;
			case VK_DOWN:
				_bUpDownFlag = true;
				SetSelRowByKey(true);
				Invalidate(TRUE);
				break;
			default:
				break;
			}
		}

		// 提示信息
		if (_cellToolTip.GetSafeHwnd())
		{
			_cellToolTip.RelayEvent(pMsg);
		}

		return CWnd::PreTranslateMessage(pMsg);
	}

	void CTreeListCtrl::SetSelRowByKey(bool bNextRow)
	{
		if (_listDataSource.empty())
		{
			return;
		}

		// 遍历绘制行
		int nSelIndex = GetSelectedIndex();
		ListDataSourceIter iter = _listDataSource.begin();
		ListDataSourceIter enditer = _listDataSource.end();
		for (int i = 0; i < nSelIndex && nSelIndex > 0; ++i)
		{
			iter++;
		}

		int nNextRow = nSelIndex;
		size_t nCount = _listDataSource.size();

		while (true)
		{
			if (bNextRow)
			{
				nNextRow = int((nNextRow + 1) % nCount);
				if (++iter == enditer)
				{
					iter = _listDataSource.begin();
				}
			}
			else
			{
				nNextRow = (nNextRow < 0) ? 0 : nNextRow;
				nNextRow = int((nNextRow - 1 + nCount) % nCount);
				if (iter == _listDataSource.begin())
				{
					iter = enditer;
				}
				--iter;
			}

			HITEMDATA hItemData = iter->second;
			HITEMDATA hParentData = hItemData->hParentData;

			// 所在节点上层已收缩,则不展现
			if (NULL == hParentData || !hParentData->IsCollapse())
			{
				break;
			}
		}

		SetFocusItem(nNextRow);
	}

	bool CTreeListCtrl::SetFocusItemByCString(const CString& strSel)
	{
		if (_listDataSource.empty())
		{
			return false;
		}

		ColumnDataVector leafColumnVector;
		GetColumnAllLeafStruct(leafColumnVector);

		// 遍历行
		ListDataSourceIter iter = _listDataSource.begin();
		ListDataSourceIter enditer = _listDataSource.end();
		for (int i = 0; iter != enditer; ++iter, ++i)
		{
			HITEMDATA hItemData = iter->second;
			HCOLUMNDATA hColumnData = leafColumnVector[0];
			CELLINFO cellInfo;
			EDITCONFIG editConfig;
			hItemData->pDataSource->GetCellData(i, 0, hColumnData, &cellInfo, &editConfig, _pConfig);
			CString strText = hItemData->pDataSource->GetCellText(i, 0, &editConfig, _pConfig);
			if (strSel == strText)
			{
				SetFocusItem(i);
				return true;
			}
		}

		return false;
	}

	void CTreeListCtrl::OnKillFocus(CWnd* pNewWnd)
	{
		// 丢失焦点时，判断是否按下上下键
		if (_bUpDownFlag)
		{
			// 将焦点设置回来
			SetFocus();
			_bUpDownFlag = false;
		}
		else if (NULL != _cellToolTip)
		{
			// 清空提示框
			_cellToolTip.Reset();
		}
	}

	LRESULT CTreeListCtrl::OnThemeChanged()
	{
		// 主题更改后重新加载皮肤数据
		_headerTheme.CloseThemeData();
		_headerTheme.OpenThemeData(m_hWnd, L"HEADER");

		_progressTheme.CloseThemeData();
		_progressTheme.OpenThemeData(m_hWnd, L"PROGRESS");

		return 0;
	}

	void CTreeListCtrl::SetColumnWordWarp(bool bWorldWarp)
	{
		_bColumnWordWarp = bWorldWarp;
	}

	void CTreeListCtrl::SetRowWordWarp(bool bWorldWarp)
	{
		_bRowWordWarp = bWorldWarp;
	}

	void CTreeListCtrl::MakeRowVisible(int nDataRowIndex)
	{
		if (nDataRowIndex < 0 || nDataRowIndex >= int(_listDataSource.size()))
			return;

		SCROLLINFO infoScroll;
		infoScroll.cbSize = sizeof(SCROLLINFO);
		infoScroll.fMask = SIF_ALL;
		if (GetScrollInfo(SB_VERT, &infoScroll))
		{
			infoScroll.fMask = SIF_POS;

			HITEMDATA hItemData = NULL;
			ListDataSourceIter endIter = _listDataSource.end();
			for (ListDataSourceIter iter = _listDataSource.begin();
				iter != endIter; ++iter)
			{
				if (nDataRowIndex == 0)
				{
					hItemData = iter->second;
					break;
				}

				nDataRowIndex--;
			}

			if (hItemData == NULL)
				return;

			CRect rcHead = CalcHeaderRect();
			CRect rcItem = hItemData->rowRect;
			rcItem.top = rcItem.top - rcHead.Height() + infoScroll.nPos;
			rcItem.bottom = rcItem.bottom - rcHead.Height() + infoScroll.nPos;

			// 当前选中的不在显示范围内
			if (!(rcItem.top >= LONG(infoScroll.nPos) &&
				rcItem.bottom <= LONG(infoScroll.nPos + infoScroll.nPage)))
			{
				// 向下定位
				if (rcItem.top > infoScroll.nPos)
				{
					infoScroll.nPos = min(LONG(rcItem.bottom - infoScroll.nPage), LONG(infoScroll.nMax - infoScroll.nPage + 1));
				}
				else // 向上定位
				{
					infoScroll.nPos = min(rcItem.top, LONG(infoScroll.nMax - infoScroll.nPage + 1));
				}
			}

			SetScrollInfo(SB_VERT, &infoScroll);
		}
	}

	void CTreeListCtrl::SelectFirstLeafNode(bool bFocusOnTop)
	{
		ListDataSource::const_iterator iterRowEnd = _listDataSource.end();
		for (ListDataSource::const_iterator iterRowData = _listDataSource.begin();
			iterRowData != iterRowEnd; ++iterRowData)
		{
			if (!iterRowData->second->HasChildNode())
			{
				SetFocusItemByDataSource(iterRowData->second->GetDataSource());
				// 设置默认选中第一个叶子节点
				break;
			}
		}

		if (bFocusOnTop)
		{
			SendMessage(WM_VSCROLL, SB_TOP, NULL);
		}
	}

	void CTreeListCtrl::GetMaxColumnLevel(int& nMaxLevel, HCOLUMNDATA hColumnData, int nCurLevel)
	{
		// 记录最大层级
		if (nCurLevel > nMaxLevel)
			nMaxLevel = nCurLevel;

		ColumnDataVector& columnVector = hColumnData
			? hColumnData->childColumn : _columnVector;

		size_t sz = columnVector.size();
		for (size_t nCol = 0; nCol < sz; ++nCol)
		{
			GetMaxColumnLevel(nMaxLevel, columnVector[nCol], nCurLevel + 1);
		}
	}

	void CTreeListCtrl::GetColumnAllLeafStruct(ColumnDataVector& leafColumnVector,
		HCOLUMNDATA hColumnData)
	{
		// 叶子节点
		if (hColumnData && 0 == hColumnData->childColumn.size())
			leafColumnVector.push_back(hColumnData);

		ColumnDataVector& columnVector = hColumnData
			? hColumnData->childColumn : _columnVector;

		size_t sz = columnVector.size();
		for (size_t nCol = 0; nCol < sz; ++nCol)
		{
			GetColumnAllLeafStruct(leafColumnVector, columnVector[nCol]);
		}
	}

	void CTreeListCtrl::GetColumnLinearityStruct(ColumnDataVector& outColumnVector,
		HCOLUMNDATA hColumnData)
	{
		if (NULL != hColumnData)
		{
			outColumnVector.push_back(hColumnData);
		}

		ColumnDataVector& columnVector = hColumnData
			? hColumnData->childColumn : _columnVector;

		size_t sz = columnVector.size();
		for (size_t nCol = 0; nCol < sz; ++nCol)
		{
			GetColumnLinearityStruct(outColumnVector, columnVector[nCol]);
		}
	}

	void CTreeListCtrl::SetTrim(bool bTrim)
	{
		_bIsTrim = bTrim;
	}

	void CTreeListCtrl::SetSelectChangeCheck(bool bCheck)
	{
		_bCheckSelChange = bCheck;
	}

	void CTreeListCtrl::SetCollapseStyle(bool bStyle /*= false*/)
	{
		_bCollapseStyle = bStyle;
	}

}