#include "stdafx.h"
#include "BwGridPanel.h"
#include "BwGanttControl.h"
#include "BwGanttChart.h"

#define IDC_PROGRESSPLAYER_NEW		1201
#define IDC_PROGRESSPLAYER_NEWCHILD 1202
#define IDC_PROGRESSPLAYER_BIND		1203
#define IDC_PROGRESSPLAYER_DELETE	1204
#define IDC_PROGRESSPLAYER_UP		1205
#define IDC_PROGRESSPLAYER_DOWN		1206
#define IDC_PROGRESSPLAYER_IMPORT	1208
#define IDC_PROGRESSPLAYER_EXPORT	1209

// 按钮区域矩形高度
#define BTN_AREA_HEIGHT 30

// 初始化按钮位置
#define INIT_BTN_RECT(rcBtn) rcBtn.top = 5; rcBtn.bottom = 25; \
rcBtn.left = rcBtn.right = 0;

#define COMPUTE_BTN_RECT(nWndWidth, nNum, nInterval, nWidth) \
nWidth = (nWndWidth - (nInterval * nNum + 1)) / nNum;

// 按钮偏移
#define BTN_OFFSET(rcBtn, nInterval, nWidth) rcBtn.left = rcBtn.right + nInterval;\
rcBtn.right = rcBtn.left + nWidth;

// Grid矩形
#define GetGridRect(rcGrid) GetClientRect(&rcGrid); \
rcGrid.top += _bShowToolBar ? BTN_AREA_HEIGHT : 0;

UINT BW_GRIDDLG_EXPORT = ::RegisterWindowMessage(_T("BW_GRIDDLG_EXPORT"));
UINT BW_GRIDDLG_IMPORT = ::RegisterWindowMessage(_T("BW_GRIDDLG_IMPORT"));
UINT BW_GRIDDLG_BEFOREDELETE = ::RegisterWindowMessage(_T("BW_GRIDDLG_BEFOREDELETE"));
UINT BW_GRIDDLG_DELETE = ::RegisterWindowMessage(_T("BW_GRIDDLG_DELETE"));
UINT BW_GRIDDLG_BIND = ::RegisterWindowMessage(_T("BW_GRIDDLG_BIND"));
UINT BW_GRIDDLG_NEW = ::RegisterWindowMessage(_T("BW_GRIDDLG_NEW"));
UINT BW_GRIDDLG_MOVE = ::RegisterWindowMessage(_T("BW_GRIDDLG_MOVE"));
UINT BW_GRIDDLG_SELCHANGE = ::RegisterWindowMessage(_T("BW_GRIDDLG_SELCHANGE"));
UINT BW_GRIDDLG_ENDEDIT = ::RegisterWindowMessage(_T("BW_GRIDDLG_ENDEDIT"));
UINT BW_GRIDDLG_ENDEDITDONE = ::RegisterWindowMessage(_T("BW_GRIDDLG_ENDEDITDONE"));

IMPLEMENT_DYNCREATE(CBwGridPanel, CWnd)

BEGIN_MESSAGE_MAP(CBwGridPanel, CWnd)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CREATE()

	ON_CONTROL_RANGE(BN_CLICKED, IDC_PROGRESSPLAYER_NEW, IDC_PROGRESSPLAYER_EXPORT, OnButtonClicked)
	ON_REGISTERED_MESSAGE(BCGM_GRID_SEL_CHANGED, OnClickGrid)
	ON_WM_PAINT()
END_MESSAGE_MAP()

#define ENDCLICKOP _pControl->UpdateScrollPos();\
_pControl->SetRedraw(TRUE);\
_pControl->RedrawWindow();\
_pChart->Invalidate();\
_pGrid->Invalidate();\
_pGrid->SendMessage(WM_PAINT);\
_pGrid->SendMessage(WM_SIZE);

CBwGridPanel::CBwGridPanel()
	: _pControl(NULL)
	, _pGrid(NULL)
	, _pChart(NULL)
	, _pRoot(NULL)
	, m_NotifyWnd(NULL)
	, _pImageList(NULL)
	, _bShowToolBar(true)
	, _nSelIndex(-1)
	, _oldSel(-1)
	, _oldPos(0)
{
	m_crTabContainerColor = RGB(255, 255, 255);
}
CBwGridPanel::~CBwGridPanel()
{

}

CString CBwGridPanel::s_ClassName = L"";

void CBwGridPanel::OnButtonClicked(UINT uId)
{
	if (!_pControl || !_pChart || !_pGrid)
	{
		return;
	}

	_oldPos = _pGrid->GetVScrollPos(); // 滚动条上一次位置
	_pControl->SetRedraw(FALSE);
	
	if (_pGrid != nullptr)
		_pGrid->ClearSortColumn();			// 清除排序，防止数据内容出现混乱

	switch (uId)
	{
	case IDC_PROGRESSPLAYER_NEW:
		GetOwner()->SendMessage(BW_GRIDDLG_NEW, (WPARAM)CreateNewTask());
		break;
	case IDC_PROGRESSPLAYER_BIND:
		_pControl->SetRedraw(TRUE);
		Bind();
		GetOwner()->SendMessage(BW_GRIDDLG_BIND);
		ENDCLICKOP
		return;
	case IDC_PROGRESSPLAYER_DELETE:
		GetOwner()->SendMessage(BW_GRIDDLG_BEFOREDELETE);
		DeleteCurTask();
		GetOwner()->SendMessage(BW_GRIDDLG_DELETE);
		break;
	case IDC_PROGRESSPLAYER_UP:
		MoveUp();
		GetOwner()->SendMessage(BW_GRIDDLG_MOVE);
		break;
	case IDC_PROGRESSPLAYER_DOWN:
		MoveDown();
		GetOwner()->SendMessage(BW_GRIDDLG_MOVE);
		break;
	case IDC_PROGRESSPLAYER_IMPORT:
		_pControl->SetRedraw(TRUE);
		Import();
		GetOwner()->SendMessage(BW_GRIDDLG_IMPORT);
		break;
	case IDC_PROGRESSPLAYER_EXPORT:
		_pControl->SetRedraw(TRUE);
		Export();
		GetOwner()->SendMessage(BW_GRIDDLG_EXPORT);
		break;
	case IDC_PROGRESSPLAYER_NEWCHILD:
		GetOwner()->SendMessage(BW_GRIDDLG_NEW, (WPARAM)CreateNewChildTask());
		break;
	}

	ENDCLICKOP

	// 选择当前行
	SelectRow(_nSelIndex);
}

void CBwGridPanel::Create(CWnd * pParent, LPRECT rt, UINT uId)
{
	CRect rtWnd(0, 0, 1, 1);
	if (rt == NULL)
		rt = &rtWnd;
	if (s_ClassName == L"")
		s_ClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
	CWnd::Create(s_ClassName, L"tab", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, *rt, pParent, 0);
	EnableToolTips(TRUE);
	m_TipCtrl.Create(this, TTS_ALWAYSTIP);
	m_TipCtrl.Activate(TRUE);

	if (uId != 0)
		SetDlgCtrlID(uId);

	m_NotifyWnd = pParent->GetSafeHwnd();
}

void CBwGridPanel::SetControl(CBwGanttControl * pControl)
{
	_pControl = pControl;
	if (_pGrid != NULL)
	{
		_pGrid->SetControl(_pControl);
	}
	_pChart = GetChart();
}

CBwGanttControl * CBwGridPanel::GetControl()
{
	return _pControl;
}

CBwGanttGrid * CBwGridPanel::GetGrid()
{
	return _pGrid;
}

void CBwGridPanel::SetRoot(KNode * pRoot)
{
	_pRoot = pRoot;
	Load();
}

KNode * CBwGridPanel::GetRoot()
{
	return _pRoot;
}

bool CBwGridPanel::IsNameExist(const CString & strCurName, KNode *pNode)
{
	if (_pGrid == NULL || !pNode)
		return false;

	auto nodeList = pNode->GetNodeList();
	for (auto &node : nodeList)
	{
		CString strName = node->GetData(COL_NAME);
		if (strName == strCurName || IsNameExist(strCurName, node))
			return true;
	}
	return false;
}

std::set<int> CBwGridPanel::GetSelIndexes()
{
	std::set<int> arrSelIndexes;

	// 获取当前选择节点级别
	CList<CBCGPGridItem *, CBCGPGridItem *> arrSelGridItems;
	if (_pGrid == NULL)
		return arrSelIndexes;
	_pGrid->GetSelectedItems(arrSelGridItems);
	POSITION pos = arrSelGridItems.GetHeadPosition();

	while (pos != NULL)
	{
		CBCGPGridItem *pGridItem = arrSelGridItems.GetNext(pos);
		int nIndex = _pGrid->GetGridItemIndex(pGridItem);
		arrSelIndexes.insert(nIndex);
	}

	return arrSelIndexes;
}

void CBwGridPanel::SetImageList(CImageList *pImageList)
{
	if (pImageList == NULL)
		return;

	_pImageList = pImageList;
	_toolBar.SetImageList(_pImageList);
}

void CBwGridPanel::ShowToolbar(bool bShow)
{
	if (bShow)
	{
		_toolBar.ShowWindow(SW_SHOW);
	}
	else
	{
		_toolBar.ShowWindow(SW_HIDE);
	}
	_bShowToolBar = bShow;
}

void CBwGridPanel::CreateToolBar()
{
	CRect rect;
	GetWindowRect(rect);
	CRect rectTool;
	rectTool.top = 5;
	rectTool.left = 5;
	rectTool.right = rect.Width();
	rectTool.bottom = rectTool.top + 22;

	// 工具栏
	_toolBar.Create(WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, this, 0x53251);
	_toolBar.ModifyStyle(0, TBSTYLE_LIST);
	
	// 载入按钮
	UINT nBtnId[] = { IDC_PROGRESSPLAYER_NEW, IDC_PROGRESSPLAYER_NEWCHILD, IDC_PROGRESSPLAYER_DELETE,IDC_PROGRESSPLAYER_BIND, IDC_PROGRESSPLAYER_IMPORT
		, IDC_PROGRESSPLAYER_EXPORT,IDC_PROGRESSPLAYER_UP, IDC_PROGRESSPLAYER_DOWN};
	
	CStringArray strBtnName;
	strBtnName.Add(_T("新建"));
	strBtnName.Add(_T("新建子集"));
	strBtnName.Add(_T("删除"));
	strBtnName.Add(_T("绑定"));
	strBtnName.Add(_T("导入"));
	strBtnName.Add(_T("导出"));
	strBtnName.Add(_T("上移"));
	strBtnName.Add(_T("下移"));
	//strBtnName.Add(_T("保存"));

	int nNum = sizeof(nBtnId) / sizeof(nBtnId[0]);

	for (int i = 0; i < nNum; i++)
	{
		// 加入按钮
		shared_ptr<TBBUTTON> pBtn = make_shared<TBBUTTON>();
		_arrBtns.push_back(pBtn);
		pBtn->fsState = TBSTATE_ENABLED;
		pBtn->fsStyle = TBSTYLE_BUTTON;
		pBtn->idCommand = nBtnId[i];
		pBtn->iBitmap = i;
		_toolBar.InsertButton(i, pBtn.get());
		TBBUTTONINFO btnInfo;
		btnInfo.pszText = strBtnName[i].GetBuffer();
		btnInfo.fsState = TBSTATE_ENABLED;
		btnInfo.fsStyle = TBSTYLE_BUTTON;
		btnInfo.idCommand = nBtnId[i];
		btnInfo.cbSize = sizeof(TBBUTTONINFO);
		btnInfo.dwMask = TBIF_TEXT;
		btnInfo.cchText = strBtnName[i].GetLength();
		btnInfo.iImage = i;
		strBtnName[i].ReleaseBuffer();
		_toolBar.SetButtonInfo(nBtnId[i],&btnInfo);

	}

	_toolBar.SetOwner(this);
	_toolBar.MoveWindow(rectTool);
	_toolBar.BringWindowToTop();
}

CBwGanttChart * CBwGridPanel::GetChart()
{
	if (_pControl == NULL)
		return NULL;
	
	return dynamic_cast<CBwGanttChart *>(_pControl->GetChart());
}

CString CBwGridPanel::CreateTaskName()
{
	CString strPreFix = _T("名称"); // 前缀
	CString strNewName;

	if (_pGrid == NULL)
		return strNewName;

	int nRow = _pGrid->GetRowCount();
	int nNameIndex = _pControl->GetNameColumnIndex();
	int index = 1;
	for (int i = 0; i < nRow + 1; i++)
	{

		CString strCurName;
		strCurName.Format(L"%s%d", strPreFix, index);
		if (IsNameExist(strCurName, _pRoot))
		{
			index++;
		}
		else
		{
			strNewName.Format(strCurName);
			break;
		}
	}

	return strNewName;
}

int CBwGridPanel::LoadFromNode(KNode *pRoot, KNode *pFind, int *pFindIndex, int nStartRow, int nLevel)
{
	if (pRoot == _pRoot)
	{
		for (int i = _pChart->GetItemCount() - 1; i >= 0; i--)
		{
			_pChart->RemoveItem(_pChart->GetItemByIndex(i));
		}
	}

	if (nullptr == pRoot || nullptr == _pGrid || nullptr == _pChart || nullptr == _pControl)
		return nStartRow;

	int nNameIndex = _pGrid->GetColIndexByName(COL_NAME);
	int nPlanEndTimeIndex = _pGrid->GetColIndexByName(COL_PLAN_ENDTIME);
 	int nPlanStartTimeIndex = _pGrid->GetColIndexByName(COL_PLAN_STARTTIME);
	int nPlanDuringTimeIndex = _pGrid->GetColIndexByName(COL_PLAN_DURINGTIME);
	int nRealEndTimeIndex = _pGrid->GetColIndexByName(COL_REAL_ENDTIME);
	int nRealStartTimeIndex = _pGrid->GetColIndexByName(COL_REAL_STARTTIME);
	int nRealDuringTimeIndex = _pGrid->GetColIndexByName(COL_REAL_DURINGTIME);
	int nGrowthPatternIndex = _pGrid->GetColIndexByName(COL_GROWTH_PATTERN);

	// 从业务节点加载数据
	const KNodeList &nodeList = pRoot->GetNodeList();
	int nRow = nodeList.size();

	COleDateTime t = COleDateTime::GetCurrentTime();
	t.SetDate(t.GetYear(), t.GetMonth(), t.GetDay());

	CBwGanttItem* pItem;

	// 从KNode中读取信息
	for (int i = 0; i < nodeList.size(); i++)
	{
		KNode *pNode = *(std::next(nodeList.begin(), i));
		if (pNode == NULL)
			continue;

		if (pFind != NULL && pNode == pFind)
		{
			*pFindIndex = nStartRow;
		}

		CGanttNode progressNode(pNode);
		if (progressNode.IsNull())
			continue;

		pItem = _pControl->CreateGanttItemByKNode(pRoot, pNode);
		if (pItem == NULL)
			continue;

		_pChart->InsertItem(nStartRow, pItem);
		++nStartRow;

		// 加载子节点
		bool bGroup = pNode->GetNodeList().size() > 0;
		pItem->SetGroupItem((BOOL)bGroup);
		if (bGroup)
		{
			nStartRow = LoadFromNode(pNode,pFind, pFindIndex,  nStartRow, nLevel + 1);
		}
	}

	return nStartRow;
}

int CBwGridPanel::GetAllSubItemCount(CBCGPGridRow * pRow)
{
	int nCount = 0;
	if (pRow == NULL)
		return nCount;
	int nSubCount = pRow->GetSubItemsCount();
	for (size_t i = 0; i < nSubCount; i++)
	{
		CBCGPGridRow *pSubRow = pRow->GetSubItem(i);
		if (pSubRow == NULL)
			continue;
		nCount += GetAllSubItemCount(pSubRow) + 1;
	}
	
	return nCount;
}

void CBwGridPanel::GetDateRange(COleDateTime & startTime, COleDateTime & endTime, KNode * pParent)
{
	if (!pParent)
		return;
	
	const KNodeList &nodeList = pParent->GetNodeList();
	for (auto &pNode : nodeList)
	{
		if (pNode == NULL)
			continue;
		CGanttNode progressNode(pNode);
		if (progressNode.IsNull())
			continue;

		// 更新最早时间
		COleDateTime psTime = progressNode.GetPlanStartTime();
		if (startTime > psTime)
			startTime = psTime;

		if (_pControl->GetTimeDimension() == CBwGanttChart::eDate && pNode->HasData(COL_REAL_STARTTIME))
		{
			COleDateTime rsTime = progressNode.GetRealStartTime();
			if (startTime > rsTime)
				startTime = rsTime;
		}

		// 更新最晚时间
		COleDateTime peTime = progressNode.GetPlanEndTime();
		if (endTime < peTime)
			endTime = peTime;

		if (_pControl->GetTimeDimension() == CBwGanttChart::eDate&& pNode->HasData(COL_REAL_ENDTIME))
		{
			COleDateTime reTime = progressNode.GetRealEndTime();
			if (endTime < reTime)
				endTime = reTime;
		}
		GetDateRange(startTime, endTime, pNode);
	}
}

void CBwGridPanel::AjustVScroll()
{
	if (!_pGrid || !_pChart)
	{
		return;
	}

	
}

void CBwGridPanel::SelectRow(int nIndex)
{
	if (!_pGrid || nIndex < 0 || nIndex >= _pGrid->GetRowCount())
		return;
	
	// 防止触发全选，选择只选一项而不是一行
	CBCGPGridItemID cueItem(nIndex, 0);
	_pGrid->SetCurSel(cueItem);
	
	if (_oldSel < 0 && nIndex != _pGrid->GetRowCount() - 1)
	{
		return;
	}

	int nOffset = (nIndex - _oldSel) * 23;
	_pGrid->SetVScrollPos(_oldPos + nOffset);
	_oldSel = nIndex;
}

void CBwGridPanel::SelectRow()
{
	SelectRow(_oldSel);
}

void CBwGridPanel::LoadFromNode(bool bRedraw)
{
	if (_pControl == NULL)
		return;

	if (bRedraw)
	{
		_pControl->SetRedraw(FALSE);
	}

	LoadFromNode(_pRoot);
	UpdateDuringTime();

	_pControl->UpdateGridEx();

	if (bRedraw)
	{
		_pControl->SetRedraw(TRUE);
		_pControl->RedrawWindow();
	}


	if(_pGrid)
		_pGrid->Invalidate();
	if(_pChart)
		_pChart->Invalidate();
}

int CBwGridPanel::LoadAndFindIndex(KNode *pFind, bool bRedraw)
{
	int nIndex = -1;
	if (bRedraw)
	{
		_pControl->SetRedraw(FALSE);
	}

	LoadFromNode(_pRoot, pFind, &nIndex);
	UpdateDuringTime();
	_pControl->UpdateGridEx();

	if (bRedraw)
	{
		_pControl->SetRedraw(TRUE);
		_pControl->RedrawWindow();
		_pControl->GetChart()->Invalidate();
	}
	return nIndex;
}

CString CBwGridPanel::GetGrowthPatternString(int nGrowthPattern)
{
	switch (nGrowthPattern)
	{
	case GROWTH_BOTTOM2TOP:
		return STR_BOTTOM2TOP;
	case GROWTH_TOP2BOTTOM:
		return STR_TOP2BOTTOM;
	case GROWTH_LEFT2RIGHT:
		return STR_LEFT2RIGHT;
	case GROWTH_RIGHT2LEFT:
		return STR_RIGHT2LEFT;
	case GROWTH_MIDDLE2SIDES:
		return STR_MIDDLE2SIDES;
	default:
		return _T("");
	}
}

int CBwGridPanel::GetGrowthPatternTag(const CString & strGrowthPattern)
{
	if (strGrowthPattern == STR_BOTTOM2TOP)
		return GROWTH_BOTTOM2TOP;
	else if (strGrowthPattern == STR_TOP2BOTTOM)
		return GROWTH_TOP2BOTTOM;
	else if (strGrowthPattern == STR_LEFT2RIGHT)
		return GROWTH_LEFT2RIGHT;
	else if (strGrowthPattern == STR_RIGHT2LEFT)
		return GROWTH_RIGHT2LEFT;
	else if (strGrowthPattern == STR_MIDDLE2SIDES)
		return GROWTH_MIDDLE2SIDES;
	else
		return GROWTH_BOTTOM2TOP;
}

void CBwGridPanel::UpdateDuringTime()
{
	if (_pGrid == NULL)
		return;
	int nNameIndex = _pGrid->GetColIndexByName(COL_NAME);
	int nPlanEndTimeIndex = _pGrid->GetColIndexByName(COL_PLAN_ENDTIME);
	int nPlanStartTimeIndex = _pGrid->GetColIndexByName(COL_PLAN_STARTTIME);
	int nPlanDuringTimeIndex = _pGrid->GetColIndexByName(COL_PLAN_DURINGTIME);
	int nRealEndTimeIndex = _pGrid->GetColIndexByName(COL_REAL_ENDTIME);
	int nRealStartTimeIndex = _pGrid->GetColIndexByName(COL_REAL_STARTTIME);
	int nRealDuringTimeIndex = _pGrid->GetColIndexByName(COL_REAL_DURINGTIME);
	int nGrowthPatternIndex = _pGrid->GetColIndexByName(COL_GROWTH_PATTERN);

	int nRow = _pGrid->GetRowCount();
	int nCol = _pGrid->GetColumnCount();
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			// 设置计划持续时间
			if (j == nPlanDuringTimeIndex)
			{
				int nPlanDuringTime = 0;
				COleDateTime planStartTime = _pGrid->GetGridData(i, nPlanStartTimeIndex);
				COleDateTime planEndTime = _pGrid->GetGridData(i, nPlanEndTimeIndex);
				nPlanDuringTime = planEndTime - planStartTime;
				nPlanDuringTime += 1;
				_pGrid->SetGridData(i, nPlanDuringTimeIndex, nPlanDuringTime);
			}
			// 设置实际持续时间
			else if (j == nRealDuringTimeIndex)
			{
				int nRealDuringTime = 0;
				COleDateTime RealStartTime = _pGrid->GetGridData(i, nRealStartTimeIndex);
				COleDateTime RealEndTime = _pGrid->GetGridData(i, nRealEndTimeIndex);
				nRealDuringTime = RealEndTime - RealStartTime;
				nRealDuringTime += 1;
				_pGrid->SetGridData(i, nRealDuringTimeIndex, nRealDuringTime);
			}
		}
	}
}

COleDateTime CBwGridPanel::CString2Datetime(CString strDate)
{
	COleVariant vtime(strDate);
	vtime.ChangeType(VT_DATE);
	return vtime;
}

CString CBwGridPanel::Datetime2CString(COleDateTime dateTime)
{
	COleVariant vtime(dateTime);
	vtime.ChangeType(VT_BSTR);
	return vtime;
}
CBwGridPanel::stTaskInfo CBwGridPanel::GetDefaultTaskInfo()
{
	CBwGridPanel::stTaskInfo taskInfo;
	// 填充默认数据
	COleDateTime t = COleDateTime::GetCurrentTime();
	t.SetDateTime(t.GetYear(), t.GetMonth(), t.GetDay(), 0, 0, 0);
	taskInfo._planStartTime = t;
	taskInfo._planEndTime = t;
	//taskInfo._planEndTime += 30;
	taskInfo._realStartTime = t;
	taskInfo._realEndTime = t;
	//taskInfo._realEndTime += 30;
	taskInfo._strName = CreateTaskName();

	return taskInfo;
}

void CBwGridPanel::GetDateRange(COleDateTime & startTime, COleDateTime & endTime)
{
	COleDateTime stTime;
	COleDateTime edTime;
	stTime.SetDate(9999, 9, 9);
	GetDateRange(stTime, edTime, _pRoot);
	startTime = stTime;
	endTime = edTime;
}

int CBwGridPanel::GetSelIndex()
{
	// 选中的下标（选中多行取最后一行）
	int nSelIndex = -1;
	if(_pGrid == NULL)
		return nSelIndex;
	// 获取当前选择节点级别
	CList<CBCGPGridItem *, CBCGPGridItem *> arrSelGridItems;
	_pGrid->GetSelectedItems(arrSelGridItems);
	POSITION pos = arrSelGridItems.GetHeadPosition();
	std::set<int> arrSelIndexes;

	while (pos != NULL)
	{
		CBCGPGridItem *pGridItem = arrSelGridItems.GetNext(pos);
		int nIndex = _pGrid->GetGridItemIndex(pGridItem);
		arrSelIndexes.insert(nIndex);
	}

	if (arrSelIndexes.size() == 0)
		return nSelIndex;

	std::set<int>::iterator it = arrSelIndexes.end();
	nSelIndex = *(--it);

	return nSelIndex;
}

int CBwGridPanel::GetBtnAreaHeight()
{
	return _bShowToolBar ? BTN_AREA_HEIGHT : 0;
}

void CBwGridPanel::CreateGrid()
{
	// 创建Grid控件
	SAFE_DELETE(_pGrid);
	_pGrid = new CBwGanttGrid;

	CRect rcGrid;
	GetGridRect(rcGrid);
	if (!_pGrid->Create(WS_VISIBLE | WS_CHILD | WM_NOTIFY | WS_BORDER, rcGrid, this, 0x663323))
	{
		TRACE0("Failed to create grid control\n");
		ASSERT(FALSE);
		return;
	}

	// 设置选择和高亮
	_pGrid->SetDisableWholeSelHeaderHighlighting();
	//_pGrid->SetWholeRowSel();
	_pGrid->SetHighlightActiveItem();
	
	// 禁止拖拽多选
	_pGrid->EnableDragSelection(FALSE);
	_pGrid->EnableDragSelectionBorder(FALSE);
	
	// 设置为单选
	_pGrid->SetSingleSel(TRUE);

	// 隐藏滚动条
	_pGrid->ShowScrollBar(0, FALSE);
	_pGrid->ShowScrollBar(1, FALSE);

	// 列头对齐方式
	_pGrid->SetHeaderAlign(HDF_CENTER);

	// 启用首行数字显示
	_pGrid->EnableLineNumbers(); 

	// 允许下拉展开
	_pGrid->EnableTreeButtons(TRUE);

	// 禁用加粗字体
	_pGrid->SetBoldFontByHasChild(false);

	_pGrid->SetOwner(this);
}

// CBwGridPanel 诊断

#ifdef _DEBUG
void CBwGridPanel::AssertValid() const
{
	CWnd::AssertValid();
}

#ifndef _WIN32_WCE
void CBwGridPanel::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}

#endif
#endif //_DEBUG

void CBwGridPanel::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if (_bShowToolBar)
	{
		CRect rect;
		GetClientRect(&rect);
		CRect rectTool;
		rectTool.top = 5;
		rectTool.left = 5;
		rectTool.right = rect.Width();
		rectTool.bottom = rectTool.top + 22;
		_toolBar.MoveWindow(&rect);
	}

	CRect rcGrid;
	GetGridRect(rcGrid);
	if(_pGrid)
		_pGrid->MoveWindow(&rcGrid);
}


int CBwGridPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(NULL, WS_BORDER);
	ModifyStyleEx(WS_EX_CLIENTEDGE, NULL);

	// 创建按钮
	//CreateBtns();

	// 创建工具栏
	CreateToolBar();

	// 创建grid组件
	CreateGrid();

	return 0;
}

KNode * CBwGridPanel::CreateNewTask()
{
	KNode *pNewNode = NULL;
	if (!_pChart || !_pRoot || !_pControl)
		return pNewNode;

	// 当前选择的下标
	_oldSel = GetSelIndex();

	pNewNode = CreateNewTaskNode(GetDefaultTaskInfo());
	auto res = _pControl->InsertNode(pNewNode);
	if (res._bRes)
	{
		_nSelIndex = res._nRow;
	}

	return pNewNode;
}

BOOL CBwGridPanel::PreTranslateMessage(MSG * pMsg)
{
	auto retVal = __super::PreTranslateMessage(pMsg);
	if (::IsWindow(m_TipCtrl.GetSafeHwnd()))
	{
		m_TipCtrl.RelayEvent(pMsg);
	}
	return retVal;
}

KNode *CBwGridPanel::CreateNewTaskNode(const stTaskInfo & taskInfo)
{
	COleDateTime planStartTime;
	planStartTime = taskInfo._planStartTime;
	COleDateTime planEndTime;
	planEndTime = taskInfo._planEndTime;

	COleDateTime realStartTime;
	realStartTime = taskInfo._realStartTime;
	COleDateTime realEndTime;
	realEndTime = taskInfo._realEndTime;

	KNode *pNode = new KNode;
	pNode->SetTag(e4DProgressNode);
	CGanttNode progressNode(pNode);

	// 设置名称
	progressNode.SetName(taskInfo._strName);

	// 设置开始结束时间
	progressNode.SetPlanStartTime(planStartTime);
	progressNode.SetRealStartTime(realStartTime);
	progressNode.SetPlanEndTime(planEndTime);
	progressNode.SetRealEndTime(realEndTime);

	// 设置持续时间
	progressNode.SetPlanDuringTime(1);
	progressNode.SetRealDuringTime(1);

	// 设置进度条颜色
	progressNode.SetPlanFinishColor(RGB(72, 212, 20));

	int stOffset = realStartTime - planStartTime;
	int edOffset = realEndTime - planEndTime;
	
	if (stOffset == 0 && edOffset == 0)
	{
		// 实际进度与计划进度一致
		progressNode.SetRealFinishColor(RGB(206, 212, 20));
	}
	else
	{
		// 实际进度与计划进度不一致
		progressNode.SetRealFinishColor(RGB(206, 120, 120));
	}

	// 设置生长方式
	progressNode.SetGrowthPattern(GROWTH_BOTTOM2TOP);

	// 设置模型数量
	progressNode.SetModelNum(0);

	// 设置操作
	progressNode.SetOperation(OPERATION_RELEASEBIND);

	return pNode;
}

KNode *CBwGridPanel::CreateNewChildTask()
{
	KNode *pNewNode = NULL;
	if (!_pChart || !_pRoot || !_pControl)
		return pNewNode;

	if (!CanCreateNewChildTask())			// 判断是否继续添加子集
		return pNewNode;

	// 当前选择的下标
	_oldSel = GetSelIndex();

	pNewNode = CreateNewTaskNode(GetDefaultTaskInfo());
	auto res = _pControl->InsertNode(pNewNode, true);
	if (res._bRes)
	{
		_nSelIndex = res._nRow;
	}

	return pNewNode;
}

bool CBwGridPanel::CanCreateNewChildTask()
{
	// 新增子级直接获取当前选中行数据
	CBwGanttItem *pParentGanttItem = _pChart->GetSelItem();
	if (pParentGanttItem == nullptr)
		return false;

	// 获取选中行对应的KNode节点
	CGanttNode  pPrjNode = pParentGanttItem->GetNode();
	if (pPrjNode.IsNull())
		return false;

	// 判断是否需要弹出提示
	int nModelNum = pPrjNode.GetModelNum();
	if (nModelNum < 1 || pParentGanttItem->IsGroupItem())
		return true;

	// 判断是否需要弹出提示
	auto bRes = ::MessageBox(GetSafeHwnd(), L"若继续创建子集任务项，当前任务将解绑？", L"提示", MB_OKCANCEL | MB_ICONINFORMATION);
	bool bReBind = (IDOK == bRes);
	if (bReBind)
	{
		auto &dataMap = ((KNode*)pPrjNode)->GetDataMap();
		for (int i = 1; i <= nModelNum; ++i)
		{
			CString strKey = _T("modelstruct") + CStringToolkitEx::IntToStr(i);
			pPrjNode.RemoveData(strKey);
		}
		pPrjNode.SetModelNum(0);
	}
	return bReBind;
}

void CBwGridPanel::Bind()
{
}

void CBwGridPanel::DeleteCurTask()
{
	if (_pGrid == NULL || _pChart == NULL)
		return;

	// 获取选择的下标数组
	std::set<int> arrSelIndexes = GetSelIndexes();
	_oldSel = _nSelIndex = GetSelIndex();

	for (std::set<int>::reverse_iterator it = arrSelIndexes.rbegin(); it != arrSelIndexes.rend(); it++)
	{
		int nRemoveIndex = *it;

		CBwGanttItem *pGanttItem = dynamic_cast<CBwGanttItem *>(_pChart->GetItemByIndex(nRemoveIndex));
		if (pGanttItem == NULL)
			continue;

		// 更新业务节点
		KNode *pNode = pGanttItem->GetNode();
		if (pNode == NULL)
			continue;
		KNode *pRoot = pNode->GetParent();
		if (pRoot == NULL)
			continue;
		pRoot->RemoveNode(pNode);

		_pChart->RemoveItem(pGanttItem);
	}
	_oldSel -= 1;
	LoadFromNode(false);

	// 设置选择删除后选择项
	_nSelIndex -= 1;
	int nRowCount = _pGrid->GetRowCount();
	if (_nSelIndex < 0 && nRowCount > 0)
	{
		_nSelIndex = 0;
	}

}

void CBwGridPanel::MoveUp()
{
	MoveCurTask(true);
}

void CBwGridPanel::MoveDown()
{
	MoveCurTask(false);
}

void CBwGridPanel::MoveCurTask(bool bUp)
{
	if (_pChart == NULL)
		return;

	// 当前选择的下标
	_oldSel = GetSelIndex();
	if (_oldSel < 0)
		return;
	CBwGanttItem *pCurItem = dynamic_cast<CBwGanttItem *>(_pChart->GetItemByIndex(_oldSel));
	if (pCurItem == NULL)
		return;

	KNode *pNode = pCurItem->GetNode();
	if (pNode == NULL)
		return;
	KNode *pParent = pNode->GetParent();
	if (pParent == NULL)
		return;
	
	// 查找选中业务节点下标
	const KNodeList &nodeList = pParent->GetNodeList();
	int nChildCount = nodeList.size();
	int nCurIndex = -1;
	for (size_t i = 0; i < nChildCount; i++)
	{
		KNode *pChild = *(std::next(nodeList.begin(), i));
		if (pChild == pNode)
			nCurIndex = i;
	}

	if (nCurIndex < 0)
		return;

	// 上下移边界
	if ((bUp && nCurIndex == 0)
		|| (!bUp && nCurIndex == nChildCount - 1))
	{
		// 处理子节点上移到父节点外（当前不需要）

		//if (pParent == _pRoot)
		//	return;
		//KNode *pGrandParent = pParent->GetParent();
		//if (pGrandParent != NULL)
		//{
		//	// 查找父节点下标
		//	const KNodeList &parNodeList = pGrandParent->GetNodeList();
		//	int nParChildCount = parNodeList.size();
		//	int nParIndex = -1;
		//	for (size_t i = 0; i < nParChildCount; i++)
		//	{
		//		KNode *pChild = *(std::next(parNodeList.begin(), i));
		//		if (pChild == pParent)
		//			nParIndex = i;
		//	}
		//	if (nParIndex < 0)
		//		return;
		//	
		//	pParent->RemoveNode(pNode, false);
		//	pGrandParent->InsertNode(pNode, bUp ? nParIndex : nParIndex + 1);
		//}
	} 
	else
	{
		pNode->SwapNode(*(std::next(nodeList.begin(), bUp ? nCurIndex - 1 : nCurIndex + 1)));
	}

	// 重新载入业务节点
	_nSelIndex = LoadAndFindIndex(pNode, false);
}

void CBwGridPanel::SetCurTime(const COleDateTime & curTime)
{
}

void CBwGridPanel::Export()
{
}

void CBwGridPanel::Import()
{
	_oldSel = -1;
	LoadFromNode(false);
}


void CBwGridPanel::Load()
{
	_oldSel = -1;
	LoadFromNode(false);
}

void CBwGridPanel::EnableButtons(bool bEnable)
{
	_toolBar.EnableWindow(bEnable);

	CRect rect;
	_toolBar.GetClientRect(&rect);
	_toolBar.Invalidate();
	_toolBar.InvalidateRect(&rect);
	_toolBar.UpdateWindow();
	_toolBar.UpdateData();

	//for (auto &btn : _arrBtns)
	//{
	//	btn->EnableWindow(bEnable);
	//}
}

void CBwGridPanel::OnPaint()
{
	// 使用双缓存绘制
	CPaintDC dc(this);
	CMemDC dc2(dc, this);

	// 填充tab页签区域的底色
	CBrush fillBrush;

	CRect rcWnd;
	GetClientRect(&rcWnd);

	fillBrush.CreateSolidBrush(m_crTabContainerColor);
	dc2.GetDC().FillRect(&rcWnd, &fillBrush);
}

bool CBwGridPanel::OnEndEditGrid(CBCGPGridItem * pItem, KNode *pNode, const CBwGanttGrid::ColumnFormat &colFormat)
{
	CBwGanttGrid::stBwGridEditInfo editInfo(pItem, pNode, colFormat);
	GetOwner()->SendMessage(BW_GRIDDLG_ENDEDIT, (WPARAM)&editInfo, NULL);
	if (!editInfo._result)
		return false;
	
	return true;
}

void CBwGridPanel::OnEndEditGridDone(CBCGPGridItem * pItem, KNode *pNode, const CBwGanttGrid::ColumnFormat &colFormat)
{
	CBwGanttGrid::stBwGridEditInfo editInfo(pItem, pNode, colFormat);
	GetOwner()->SendMessage(BW_GRIDDLG_ENDEDITDONE, (WPARAM)&editInfo, NULL);

	int nIndex = _pGrid ? _pGrid->GetGridItemIndex(pItem) : -1;
	if (nIndex > -1)
	{
		CBwGanttItem *pCurItem = dynamic_cast<CBwGanttItem *>(_pChart->GetItemByIndex(nIndex));
		if (pCurItem != NULL && _pControl && _pChart)
		{
			_pControl->SetRedraw(FALSE);

			int planStartTime = pCurItem->GetStartTime();
			int realStartTime = pCurItem->GetRealStartTime();
			int planEndTime = pCurItem->GetFinishTime();
			int realEndTime = pCurItem->GetRealFinishTime();

			int stOffset = realStartTime - planStartTime;
			int edOffset = realEndTime - planEndTime;
			if (stOffset == 0 && edOffset == 0)
			{
				// 实际进度与计划进度一致
				pCurItem->SetPrimaryColor(RGB(206, 212, 20));
				pCurItem->SetRealCompleteColor(RGB(206, 212, 20));
			}
			else
			{
				// 实际进度与计划进度不一致
				pCurItem->SetRealPrimaryColor(RGB(206, 120, 120));
				pCurItem->SetRealCompleteColor(RGB(206, 120, 120));
			}

			_pControl->SetRedraw(TRUE);
			_pControl->RedrawWindow();
			_pChart->Invalidate();
		}
	}
}


LRESULT CBwGridPanel::OnClickGrid(WPARAM wParam, LPARAM lParam)
{
	GetOwner()->SendMessage(BW_GRIDDLG_SELCHANGE, wParam, lParam);
	return 0;
}

