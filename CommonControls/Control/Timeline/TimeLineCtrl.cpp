#include "stdafx.h"
#include "TimeLineCtrl.h"

IMPLEMENT_DYNAMIC(CTimeLineCtrl, CWnd)

BEGIN_MESSAGE_MAP(CTimeLineCtrl, CWnd)
	ON_WM_CREATE()
	//ON_WM_PAINT()
	//ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CTimeLineCtrl::CTimeLineCtrl(void)
{
	//_pTimeLineCtrlDataSource = NULL;
	_bDelDataSource = false;
}

CTimeLineCtrl::~CTimeLineCtrl(void)
{

}

bool CTimeLineCtrl::Create(const CRect& rect, CWnd* pParentWnd, UINT nID /*= NULL*/,
	DWORD dwStyle /*= WS_BORDER | WS_CHILD | WS_VISIBLE*/)
{
	bool bSuccess = false;

	// 注册控件类型
	if (RegisterTimeLineCtrl())
	{
		bSuccess = !!CWnd::Create(L"TimeLineCtrl", NULL, dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, pParentWnd, nID);
	}

	CRect curRect;
	GetClientRect(&curRect);
	_verSplit.Create(curRect, this, NULL, true);

	_treeLayerAreaCtrl.Create(curRect, &_verSplit);
	_treeLayerAreaCtrl.SetSingleSelect(false);
	_treeLayerAreaCtrl.SetRowWordWarp(false);
	_treeLayerAreaCtrl.SetAutoColumnWidth();

	_timeFrameListCtrl.Create(curRect, &_verSplit, this);
	_treeLayerAreaCtrl.SetCorrespondingTimeFrameCtrl(&_timeFrameListCtrl);

	_verSplit.SetSplitterPanes(_treeLayerAreaCtrl, _timeFrameListCtrl);
	_verSplit.SetSplitterPos((curRect.right - curRect.left) / 5);
	_verSplit.SetCorrespondingTimeFrameCtrl(&_timeFrameListCtrl);

	return bSuccess;
}

bool CTimeLineCtrl::LoadConfig(const CString& strCfg, const CString& strLayerCfgName /*= L""*/)
{
	_treeLayerAreaCtrl.LoadConfig(strCfg, strLayerCfgName);
	_timeFrameListCtrl.LoadConfig(strCfg);

	CXmlDocument doc;
	if (!doc.LoadFile(strCfg, DocFormatEnum::fmtXML))
	{
		return false;
	}
	CXmlElement *pRoot = doc.GetElementRoot();
	CXmlElement *pTimeFrameNode = pRoot->GetChildElementAt(L"时间帧区", FALSE);
	CXmlElement *pSizeNode = pTimeFrameNode->GetChildElementAt(L"Size", FALSE);
	int nHeadHeight = StrToInt(pSizeNode->GetAttrValue(L"HeadHeight"));
	_treeLayerAreaCtrl.SetHeadItemHeight(nHeadHeight);

	return false;
}

void CTimeLineCtrl::SetDataSource(ITimeLineCtrlDataSource *pDataSource, bool bDelDataSource)
{
	// 赋值新数据源
	_treeLayerAreaCtrl.SetDataSource(pDataSource, false);
	_treeLayerAreaCtrl.UpdateList(true);
	_listDataSource = _treeLayerAreaCtrl.GetListDataSource();
	_timeFrameListCtrl.SetItemCount(_listDataSource.size());
	_verSplit.UpdateScrollBar();
	_bDelDataSource = bDelDataSource;
}

void CTimeLineCtrl::UpdateCtrl(bool bDataChange /*= false*/)
{
	_treeLayerAreaCtrl.UpdateList(bDataChange);
	_listDataSource = _treeLayerAreaCtrl.GetListDataSource();
	_timeFrameListCtrl.SetItemCount(_listDataSource.size());

	if (bDataChange)
	{
		_verSplit.UpdateScrollBar();
	}
	
	_timeFrameListCtrl.UpdateCtrl();
}

void CTimeLineCtrl::SetNofityWnd(HWND hNotifyWnd)
{
	_treeLayerAreaCtrl.SetNofityWnd(hNotifyWnd);
	_timeFrameListCtrl.SetNofityWnd(hNotifyWnd);
}

void CTimeLineCtrl::SetTreeColumn(int nColumnIndex /*= 0*/)
{
	_treeLayerAreaCtrl.SetTreeColumn(nColumnIndex);
}

void CTimeLineCtrl::SetMouseResponseJudgment(IMouseResponseJudgment *pMouseResponseJudgement)
{
	_timeFrameListCtrl.SetMouseResponseJudgment(pMouseResponseJudgement);
}

void CTimeLineCtrl::SetTimeFrameLength(int nTimeFrameLength)
{
	_timeFrameListCtrl.SetTimeFrameLength(nTimeFrameLength);
}

int CTimeLineCtrl::GetTimeFrameLength(void)
{
	return _timeFrameListCtrl.GetTimeFrameLength();
}

void CTimeLineCtrl::GetDataSourceArray(DataSourceArray &datasourceArray)
{
	// 先清空数组;
	datasourceArray.clear();

	// 遍历当前选择的记录;
	for (ListDataSourceIter iter = _listDataSource.begin(); iter != _listDataSource.end(); ++iter)
	{
		datasourceArray.push_back(iter->second->GetDataSource());
	}
}

ListDataSource CTimeLineCtrl::GetListDataSource(void)
{
	return _listDataSource;
}

IListDataSource* CTimeLineCtrl::GetSelectItem()
{
	return _treeLayerAreaCtrl.GetSelectedItem();
}

void CTimeLineCtrl::SetLayerAreaFocusItem(int nIndex)
{
	_treeLayerAreaCtrl.SetFocusItem(nIndex);
}

int CTimeLineCtrl::GetLayerAreaSelectedIndex()
{
	return _treeLayerAreaCtrl.GetSelectedIndex();
}

bool CTimeLineCtrl::RegisterTimeLineCtrl(void)
{
	HINSTANCE hInstance = AfxGetInstanceHandle();

	WNDCLASS wndCls = { 0 };
	if (GetClassInfo(hInstance, L"TimeLineCtrl", &wndCls))
		return true;

	wndCls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndCls.lpfnWndProc = ::DefWindowProc;
	wndCls.cbClsExtra = 0;
	wndCls.cbWndExtra = 0;
	wndCls.hInstance = AfxGetInstanceHandle();
	wndCls.hIcon = NULL;
	wndCls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndCls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndCls.lpszMenuName = NULL;
	wndCls.lpszClassName = L"TimeLineCtrl";

	return !!AfxRegisterClass(&wndCls);
}

void CTimeLineCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);
	_verSplit.MoveWindow(rect);
	_verSplit.UpdateScrollBar();
}