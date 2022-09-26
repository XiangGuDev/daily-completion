#include "stdafx.h"
#include "EditableTabCtrl.h"
#include "resource.h"
#define EDIT_ID 1024
#define TAB_PAGE_ID_START 1024

BEGIN_MESSAGE_MAP(CEditableTabCtrl, CWnd)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_END_EDIT, OnEndEdit)
END_MESSAGE_MAP()

CString CEditableTabCtrl::s_ClassName = L"";
CEditableTabCtrl::CEditableTabCtrl(DWORD dwStyle/* = TCS_ENABLEEDIT | TCS_SHOWADDBTN | TCS_SHOWCLOSEBTN*/)
{
	m_CurSel		  = -1;
	m_nTabHeight	  = 24;
	m_nMinTabWidth    = 48;
	m_nMaxTabWidth    = 100;
	m_nTabPoDu		  = 4;
	m_nButtonSpan	  = 8;
	m_nButtonWidth	  = 32;
	m_nTabToContainer = 2;
	
	m_crTabBkColor = RGB(194, 213, 232);
	m_crTabHightBkColor = RGB(255, 255, 255);
	m_crTabFrameColor = RGB(100, 100, 100);
	m_crContainerColor = RGB(255, 255, 255);
	m_crTabContainerColor = RGB(255, 255, 255);

	INIT_RESHANDLE();
	m_nIconSize = 16;
	m_nCloseBtSize = 16;
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_CONTROL_IMAGES);
	m_Images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
	m_Images.Add(&bitmap, RGB(0, 0, 0));
	m_nIconOffset = 4;

	CBitmap bitmap2;
	bitmap2.LoadBitmap(IDB_ADDBUTTON);
	m_AddButtonImgs.Create(32, 32, ILC_COLOR32 | ILC_MASK, 1, 1);
	m_AddButtonImgs.Add(&bitmap2, RGB(0,0,0));

	m_ButtonStatus = eDefStatus;
	m_nDisStyle = dwStyle;
	m_pInputEditor = NULL;

	m_NotifyWnd = NULL;
	m_TabFnt.Attach((HFONT)GetStockObject(DEFAULT_GUI_FONT));
}


CEditableTabCtrl::~CEditableTabCtrl()
{
	m_AddButtonImgs.DeleteImageList();
}

void CEditableTabCtrl::SetDisplayStyle(DWORD dwStyle)
{
	m_nDisStyle = dwStyle;
	if (FALSE == ::IsWindow(m_hWnd))
	{
		UpdateItemRect();
		Invalidate();
	}
}

void CEditableTabCtrl::SetNotifyWnd(HWND hWnd)
{
	m_NotifyWnd = hWnd;
}

DWORD CEditableTabCtrl::GetDisplayStyle()
{
	return m_nDisStyle;
}

void CEditableTabCtrl::SetCustomEdit(CInputEdit *pEdit)
{
	m_pInputEditor = pEdit;
}

void CEditableTabCtrl::CreateWnd(CWnd *pParent, LPRECT rt, UINT uId/* = 0*/)
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

int CEditableTabCtrl::InsertItem(LPCTSTR labelName, CWnd *pChildren, int nIndex /*= -1*/)
{
	// 插入tab数据
	TabInfo newInfo;
	newInfo.pChildren = pChildren;
	newInfo.tabName = labelName;
	newInfo.bSelected = false;
	newInfo.bCloseBtnHightLight = false;
	if (nIndex != -1)
		m_TabList.insert(m_TabList.begin() + nIndex, newInfo);
	else
		m_TabList.push_back(newInfo);
	if (pChildren)
	{
		// 调整子窗口大小，以适应tab控件
		pChildren->SetParent(this);
		CRect rt = GetContainerPos();
		pChildren->MoveWindow(rt);
		pChildren->ShowWindow(SW_HIDE);
	}
	
	// 更新tab大小
	UpdateItemRect();
	UpdateTipInfo();
	Invalidate();
	if (m_TabList.size() == 1)
		SelectItem(0);

	return nIndex == -1 ? m_TabList.size() - 1 : nIndex;
}

void CEditableTabCtrl::RemoveItem(int nIndex)
{
	TabNotify notifyHead;
	notifyHead.code = TCN_DELETEITEM;
	notifyHead.hwndFrom = GetSafeHwnd();
	notifyHead.idFrom = GetDlgCtrlID();
	notifyHead.tabInfo = m_TabList[nIndex];
	LRESULT retRes = ::SendMessage(m_NotifyWnd, WM_NOTIFY, notifyHead.idFrom, (LPARAM)&notifyHead);
	if (retRes == FALSE)
		return;

	CWnd *pWndWillRemoved = m_TabList[nIndex].pChildren;
	if (pWndWillRemoved && ::IsWindow(pWndWillRemoved->GetSafeHwnd()))
		pWndWillRemoved->ShowWindow(SW_HIDE);

	m_TabList.erase(m_TabList.begin() + nIndex);

	if (m_CurSel >= nIndex)	// 只有当前选中项在页签后面，才会调整当前选中索引，但不触发事件。
	{
		int nNewSelection = max(0, m_CurSel - 1);
		nNewSelection = min(nNewSelection, m_TabList.size() - 1);
		if (m_CurSel == nIndex)
		{
			SelectItem(nNewSelection);
		}
		else
		{
			m_CurSel = nNewSelection;
		}
	}
	if (m_CurSel > -1)
	{
		m_TabList[m_CurSel].pChildren->ShowWindow(SW_SHOW);
	}

	UpdateItemRect();
	UpdateTipInfo();
	Invalidate();

}

int CEditableTabCtrl::GetCurSel()
{
	return m_CurSel;
}

bool CEditableTabCtrl::GetItemRect(int nIndex, LPRECT retRect)
{
	if (nIndex >= (int)m_TabList.size() || nIndex < 0)
		return false;
	*retRect = m_TabList[nIndex].rectSize;
	return true;
}

void CEditableTabCtrl::SetItemText(int nIndex, LPCTSTR newTxt)
{
	if (nIndex >= (int)m_TabList.size() || nIndex < 0)
		return;
	m_TabList[nIndex].tabName = newTxt;
	UpdateTipInfo();
	Invalidate();
}

CString CEditableTabCtrl::GetItemText(int nIndex)
{
	if (nIndex >= (int)m_TabList.size() || nIndex < 0)
		return L"";

	return m_TabList[nIndex].tabName;
}

void CEditableTabCtrl::SelectItem(int nIndex)
{
	if (nIndex < 0 || nIndex >= (int)m_TabList.size())
		nIndex = 0;

	if (m_TabList.size() == 0)
	{
		m_CurSel = -1;
		UpdateItemRect();
		UpdateTipInfo();
		Invalidate();
		::SendMessage(m_NotifyWnd, WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), TCN_SELCHANGE), (LPARAM)m_hWnd);
		return;
	}
		
	nIndex = min(m_TabList.size() - 1, nIndex);
	if (m_CurSel != -1 && m_CurSel < (int)m_TabList.size())
	{
		m_TabList[m_CurSel].bSelected = false;
		if (m_TabList[m_CurSel].pChildren)
			m_TabList[m_CurSel].pChildren->ShowWindow(SW_HIDE);
	}
	m_CurSel = nIndex;
	m_TabList[m_CurSel].bSelected = true;
	if (m_TabList[m_CurSel].pChildren)
		m_TabList[m_CurSel].pChildren->ShowWindow(SW_SHOW);
	UpdateItemRect();
	UpdateTipInfo();
	Invalidate();
	::SendMessage(m_NotifyWnd, WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),TCN_SELCHANGE), (LPARAM)m_hWnd);
}

int CEditableTabCtrl::GetItemCount()
{
	return (int)m_TabList.size();
}

void CEditableTabCtrl::ClearAll()
{
	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		if (m_TabList[i].pChildren &&
			TRUE == ::IsWindow(m_TabList[i].pChildren->GetSafeHwnd()))
		{
			m_TabList[i].pChildren->ShowWindow(SW_HIDE);
		}
	}
	m_TabList.clear();
	UpdateItemRect();
	UpdateTipInfo();
	SelectItem(-1);
}

void CEditableTabCtrl::UpdateItemRect()
{
	if (m_TabList.size() == 0)
		return;

	// 1. 按等分方式排布
	CRect wndRect = GetTabContainerPos(false);
	int nTabTotalWidth = wndRect.Width() - 2 * m_nButtonSpan - m_nButtonWidth - m_nTabPoDu;
	int nAverageWidth = nTabTotalWidth / m_TabList.size();

	// 2. 如果等分后的宽度小于最小宽度，那么需要根据最小的宽度排布
	nAverageWidth = max(m_nMinTabWidth, nAverageWidth);
	nAverageWidth = min(m_nMaxTabWidth, nAverageWidth);

	// 3. 保证被选中项可见：通过计算从列表开始到到选中项的距离，推算左移多少像素
	int nEnsureVisibableSize = nAverageWidth * (m_CurSel+1) + 2 * m_nTabPoDu;
	int nCurOffset = max(0, nEnsureVisibableSize - nTabTotalWidth);
	
	CRect rtStart;
	rtStart.left = -nCurOffset + m_nTabPoDu;
	rtStart.right = rtStart.left + nAverageWidth;
	rtStart.top = 0;
	rtStart.bottom = m_nTabHeight;
	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		if (i != 0)
			rtStart.OffsetRect(nAverageWidth, 0);
		m_TabList[i].rectSize = rtStart;

		// 计算图标位置
		CPoint iconPos = rtStart.TopLeft();
		iconPos.y = m_nTabHeight / 2 - m_nIconSize / 2;
		iconPos.x += m_nIconOffset;

		m_TabList[i].iconPos.left = iconPos.x ;
		m_TabList[i].iconPos.right = iconPos.x + m_nIconSize;
		m_TabList[i].iconPos.top = iconPos.y;
		m_TabList[i].iconPos.bottom = iconPos.y + m_nIconSize;

		// 计算关闭按钮位置
		CPoint ptCloseBtnLeftTop = rtStart.BottomRight();
		ptCloseBtnLeftTop.y = m_nTabHeight / 2 - m_nCloseBtSize / 2;
		ptCloseBtnLeftTop.x -= m_nCloseBtSize;
		m_TabList[i].closeBtPos.left = ptCloseBtnLeftTop.x;
		m_TabList[i].closeBtPos.top = ptCloseBtnLeftTop.y;
		m_TabList[i].closeBtPos.bottom = ptCloseBtnLeftTop.y + m_nCloseBtSize;
		m_TabList[i].closeBtPos.right = ptCloseBtnLeftTop.x + m_nCloseBtSize;

		// 计算文本位置
		CRect txtRect = rtStart;
		txtRect.left += (m_nIconSize + m_nIconOffset);
		txtRect.top += 1;
		txtRect.bottom -= 1;
		txtRect.right -= m_nCloseBtSize;
		m_TabList[i].txtPos = txtRect;
	}
}

void CEditableTabCtrl::UpdateTipInfo()
{
	if (::IsWindow(m_TipCtrl.GetSafeHwnd()) == FALSE)
		return;

	for (int i = 0; i < m_TipCtrl.GetToolCount(); ++i)
	{
		m_TipCtrl.DelTool(this, i + TAB_PAGE_ID_START);
	}
	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		m_TipCtrl.AddTool(this, m_TabList[i].tabName, m_TabList[i].rectSize, i+ TAB_PAGE_ID_START);
	}
}

CRect CEditableTabCtrl::GetAddButtonPos()
{
	CRect rtButton;
	GetClientRect(rtButton);
	int nPosLeft = rtButton.right - m_nButtonSpan * 2 - m_nButtonWidth;
	if (m_TabList.size())
		nPosLeft = min(m_TabList.rbegin()->rectSize.right + m_nButtonSpan, nPosLeft);
	else
		nPosLeft = m_nButtonSpan;
	rtButton.left = nPosLeft;
	rtButton.top = (m_nTabHeight - m_nButtonWidth) / 2;
	rtButton.right = rtButton.left + m_nButtonWidth;
	rtButton.bottom = rtButton.top + m_nButtonWidth;
	return rtButton;
}

CRect CEditableTabCtrl::GetContainerPos()
{
	CRect rtContainer;
	GetClientRect(rtContainer);
	rtContainer.top += (m_nTabHeight + m_nTabToContainer);
	return rtContainer;
}

CRect CEditableTabCtrl::GetTabContainerPos(bool bIncludeBtn)
{
	CRect wndRect;
	GetClientRect(wndRect);
	int nTabTotalWidth = wndRect.Width();
	if (!bIncludeBtn && (m_nDisStyle & TCS_SHOWADDBTN))
		nTabTotalWidth = nTabTotalWidth -2 * m_nButtonSpan - m_nButtonWidth;
	CRect retRect(0, 0, nTabTotalWidth, m_nTabHeight);
	return retRect;
}

void CEditableTabCtrl::GetTabCorner(const TabInfo& tabInfo, CPoint& leftTop, CPoint& rightTop, CPoint &leftBottom, CPoint &rightBottom)
{
	leftTop = tabInfo.rectSize.TopLeft();
	leftTop.x += m_nTabPoDu;

	rightTop = tabInfo.rectSize.TopLeft();
	rightTop.x += (tabInfo.rectSize.Width() - m_nTabPoDu);

	leftBottom = tabInfo.rectSize.TopLeft();
	leftBottom.y = tabInfo.rectSize.bottom;
	leftBottom.x -= m_nTabPoDu;
	
	rightBottom = tabInfo.rectSize.BottomRight();
	rightBottom.x += m_nTabPoDu;
}

void CEditableTabCtrl::DrawTabItemFrame(CDC *pDC, const TabInfo& info)
{
	CPoint leftTop, rightTop, leftBottom, rightBottom;
	GetTabCorner(info, leftTop, rightTop, leftBottom, rightBottom);
	
	// 绘制轮廓线
	CPen framePen;
	framePen.CreatePen(PS_SOLID, 1, m_crTabFrameColor);
	pDC->SelectObject(&framePen);
	pDC->MoveTo(leftBottom);
	pDC->LineTo(leftTop);
	pDC->LineTo(rightTop);
	pDC->LineTo(rightBottom);

	// 如果是选中项，还需要将底部轮廓线绘制到控件2端
	if (info.bSelected)
	{
		CRect rtWnd;
		GetClientRect(rtWnd);
		CPoint startPt = rtWnd.TopLeft();
		startPt.y = m_nTabHeight;
		pDC->MoveTo(startPt);
		pDC->LineTo(leftBottom);
		pDC->MoveTo(rightBottom);
		CPoint endPnt = rtWnd.BottomRight();
		endPnt.y = m_nTabHeight;
		pDC->LineTo(endPnt);
	}
	
	// 绘制填充区
	CBrush bkBrush;
	COLORREF bkColor = info.bSelected ? m_crTabHightBkColor : m_crTabBkColor;
	bkBrush.CreateSolidBrush(bkColor);
	pDC->SelectObject(&bkBrush);
	leftTop.x += 1; leftTop.y += 1;
	rightTop.x -= 1; rightTop.y += 1;
	leftBottom.x += 1; leftBottom.y += 1;
	rightBottom.x -= 1; rightBottom.y += 1;
	pDC->BeginPath();
	pDC->MoveTo(leftTop);
	pDC->LineTo(rightTop);
	pDC->LineTo(rightBottom);
	pDC->LineTo(leftBottom);
	pDC->LineTo(leftTop);
	pDC->EndPath();
	pDC->FillPath();
}

void CEditableTabCtrl::DrawTabItemText(CDC *pDC, const TabInfo& info)
{
	CPoint iconPos = info.iconPos.TopLeft();
	m_Images.Draw(pDC, 2, iconPos, ILS_NORMAL);
	pDC->SelectObject(&m_TabFnt);
	COLORREF bkColor = info.bSelected ? m_crTabHightBkColor : m_crTabBkColor;
	pDC->SetBkColor(bkColor);
	CRect txtRect = info.txtPos;
	pDC->DrawText(info.tabName, &txtRect, DT_CENTER | DT_VCENTER |DT_SINGLELINE | DT_END_ELLIPSIS);
}

void CEditableTabCtrl::DrawTabCloseButton(CDC *pDC, const TabInfo &info)
{
	int nIconIndex = info.bCloseBtnHightLight ? 1 : 0;
	CPoint ptIconLeftTop = info.closeBtPos.TopLeft();
	m_Images.Draw(pDC, nIconIndex, ptIconLeftTop, ILS_NORMAL);
}

void CEditableTabCtrl::DrawTabItem(CDC * pDC, const TabInfo & info)
{
	DrawTabItemFrame(pDC, info);
	DrawTabItemText(pDC, info);
	if (m_nDisStyle & TCS_SHOWCLOSEBTN)
		DrawTabCloseButton(pDC, info);
}

void CEditableTabCtrl::DrawAddButton(CDC * pDC, ButtonStatus btInfo)
{
	CRect rt = GetAddButtonPos();
	CPoint pt = rt.TopLeft();
	m_AddButtonImgs.Draw(pDC, btInfo, pt, ILS_NORMAL);
}

int CEditableTabCtrl::HitTest(CPoint pt)
{
	CRect rtTotalArea = GetTabContainerPos(false);
	if (!rtTotalArea.PtInRect(pt))
		return -1;

	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		if (TRUE == m_TabList[i].rectSize.PtInRect(pt))
			return i;
	}
	return -1;
}

int CEditableTabCtrl::HitCloseBtTest(CPoint pt)
{
	if (!(m_nDisStyle & TCS_SHOWCLOSEBTN))
		return -1;

	CRect rt = GetTabContainerPos(false);
	if (FALSE == rt.PtInRect(pt))
		return -1;

	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		if (TRUE == m_TabList[i].closeBtPos.PtInRect(pt))
			return i;
	}
	return -1;
}

void CEditableTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	__super::OnLButtonDblClk(nFlags, point);

	// 检测是否点击到了tab上
	if ((m_nDisStyle & TCS_ENABLEEDIT) == 0 || -1 == HitTest(point))
		return;

	// 获得item位置
	int nIndex = GetCurSel();
	CRect rt;
	GetItemRect(nIndex, rt);
	
	// 将编辑框移动到tab上方并显示出来
	auto pInputEditor = GetInputEditor();
	ClientToScreen(rt);
	pInputEditor->MoveWindow(rt);
	pInputEditor->ShowWindow(SW_SHOW);

	// 设置默认的文本内容
	CString defTxt = GetItemText(nIndex);
	pInputEditor->ShowDefText(defTxt);
}

void CEditableTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	static MSG lstLBtDownMsg;
	MSG* pCurMsg = AfxGetCurrentMessage();

	if (lstLBtDownMsg.message == WM_LBUTTONDOWN && 
		(pCurMsg->time - lstLBtDownMsg.time) < 300 &&
		pCurMsg->lParam == lstLBtDownMsg.lParam)
	{
		OnLButtonDblClk(nFlags, point);
		lstLBtDownMsg = *pCurMsg;
		return;
	}

	__super::OnLButtonDown(nFlags, point);
	int nIndex = HitCloseBtTest(point);
	if (nIndex != -1)
	{
		RemoveItem(nIndex);
		return;
	}

	lstLBtDownMsg = *pCurMsg;
	nIndex = HitTest(point);
	if (nIndex != -1 && nIndex != m_CurSel)
	{
		SelectItem(nIndex);
		
		return;
	}
	
	if (CheckMouseInAddButtonArea(point))
	{
		m_ButtonStatus = ePushDownSatus;
		Invalidate();
	}
}

void CEditableTabCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);
	// 更新关闭按钮事件
	UpdateCloseButtonStatus(point);

	// 更新添加按钮状态
	if (CheckMouseInAddButtonArea(point))
	{
		if (m_ButtonStatus == ePushDownSatus)
		{
			auto parent = GetParent();
			UINT ctrId = GetDlgCtrlID();
			::SendMessage(m_NotifyWnd, WM_COMMAND, MAKEWPARAM(ctrId, BN_CLICKED), (LPARAM)GetSafeHwnd());
		}
		m_ButtonStatus = eHightLightStatus;
		
	}
	else
	{
		m_ButtonStatus = eDefStatus;
	}
	Invalidate();
	
}

void CEditableTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	// 更新关闭按钮事件
	UpdateCloseButtonStatus(point);

	// 检查鼠标是否移动到了页签上
	int nIndex = HitTest(point);
	if (nIndex != -1)
	{
		CString strTip = GetItemText(nIndex);
		m_TipCtrl.UpdateTipText(strTip, this);
	}

	// 检查是否移到了添加按钮上
	auto oldBtStatus = m_ButtonStatus;
	if (CheckMouseInAddButtonArea(point))
	{
		m_ButtonStatus = nFlags & MK_LBUTTON ? ePushDownSatus : eHightLightStatus;
	}
	else
	{
		m_ButtonStatus = eDefStatus;
	}

	if (oldBtStatus != m_ButtonStatus)
		Invalidate();
}

LRESULT CEditableTabCtrl::OnEndEdit(WPARAM wParam, LPARAM lParam)
{
	CString retTxt;
	auto pInputEditor = GetInputEditor();
	pInputEditor->GetWindowText(retTxt);
	int nIndex = GetCurSel();
	SetItemText(nIndex, retTxt);

	TabNotify notifyHead;
	notifyHead.code = TCN_ENDEDITITEM;
	notifyHead.hwndFrom = GetSafeHwnd();
	notifyHead.idFrom = GetDlgCtrlID();
	notifyHead.tabInfo = m_TabList[nIndex];
	::SendMessage(m_NotifyWnd, WM_NOTIFY, notifyHead.idFrom, (LPARAM)&notifyHead);
	return 0;
}

BOOL CEditableTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CEditableTabCtrl::OnPaint()
{
	// 使用双缓存绘制
	CPaintDC dc(this);
	CMemDC dc2(dc, this);

	// 填充tab页签区域的底色
	CBrush fillBrush;
	fillBrush.CreateSolidBrush(m_crTabContainerColor);
	dc2.GetDC().FillRect(GetTabContainerPos(true), &fillBrush);
	if (m_TabList.size() != 0)
	{
		// 绘制每个页签
		CRect rtTabRange = GetTabContainerPos(false);
		CRgn rgn;
		rgn.CreateRectRgn(rtTabRange.left, rtTabRange.top, rtTabRange.right, rtTabRange.bottom);
		dc2.GetDC().SelectClipRgn(&rgn);
		// 绘制tab页签
		for (int i = 0; i < (int)m_TabList.size(); ++i)
		{
			if (i != m_CurSel)
				DrawTabItem(&dc2.GetDC(), m_TabList[i]);
		}
		if (m_CurSel != -1)
			DrawTabItem(&dc2.GetDC(), m_TabList[m_CurSel]);
		dc2.GetDC().SelectClipRgn(NULL);
	}
	

	// 绘制添加按钮
	if (m_nDisStyle & TCS_SHOWADDBTN)
	{
		CRgn rgnBt;
		CRect rtBtRect = GetAddButtonPos();
		rgnBt.CreateRectRgn(rtBtRect.left, rtBtRect.top, rtBtRect.right, rtBtRect.bottom);
		dc2.GetDC().SelectClipRgn(&rgnBt);
		DrawAddButton(&dc2.GetDC(), m_ButtonStatus);
		dc2.GetDC().SelectClipRgn(NULL);
	}

	// 绘制子窗口容器区域
	CRect rtContainer = GetContainerPos();
	rtContainer.top -= m_nTabToContainer;
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(m_crContainerColor);
	dc2.GetDC().FillRect(rtContainer, &whiteBrush);
}

void CEditableTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	UpdateItemRect();
	UpdateTipInfo();

	// 调整子窗口大小
	CRect rtWnd = GetContainerPos();
	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		if (m_TabList[i].pChildren)
			m_TabList[i].pChildren->MoveWindow(rtWnd);
	}
	Invalidate();
}

void CEditableTabCtrl::UpdateCloseButtonStatus(CPoint msPt)
{
	// 检查是否移到了关闭按钮上
	bool bNeedUpdate = false;
	int nLastHitBtn = -1;
	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		if (m_TabList[i].bCloseBtnHightLight)
		{
			nLastHitBtn = i;
			break;
		}
	}

	int nIndex = HitCloseBtTest(msPt);
	for (int i = 0; i < (int)m_TabList.size(); ++i)
	{
		m_TabList[i].bCloseBtnHightLight = (nIndex == i);
	}
	if (nLastHitBtn != nIndex)
		Invalidate();
}

bool CEditableTabCtrl::CheckMouseInAddButtonArea(CPoint msPt)
{
	CRect rtAddPt = GetAddButtonPos();
	return (m_nDisStyle & TCS_SHOWADDBTN) && (rtAddPt.PtInRect(msPt) == TRUE);
}

BOOL CEditableTabCtrl::PreTranslateMessage(MSG* pMsg)
{
	auto retVal = __super::PreTranslateMessage(pMsg);
	if (::IsWindow(m_TipCtrl.GetSafeHwnd()))
	{
		m_TipCtrl.RelayEvent(pMsg);
	}
	return retVal;
}

CInputEdit* CEditableTabCtrl::GetInputEditor()
{
	// 先使用自定义的控件
	if (m_pInputEditor && IsWindow(m_pInputEditor->GetSafeHwnd()))
	{
		return m_pInputEditor;
	}

	// 如果没有自定义控件，则使用默认控件
	if (FALSE == ::IsWindow(m_DefInputEditor.GetSafeHwnd()))
	{
		m_DefInputEditor.CreateWnd(this);
		m_DefInputEditor.SetEditorFont(&m_TabFnt);
		m_DefInputEditor.ShowWindow(SW_HIDE);
	}
	
	return &m_DefInputEditor;
}
