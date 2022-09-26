#include "stdafx.h"
#include "ComboAnyOne.h"
#define EDIT_CTRL_ID 110
#define TIME_ID 2048
#include "resource.h"

// ================================== CComboPanel ==================================
CString CComboPanel::s_ClassName = L"";
BEGIN_MESSAGE_MAP(CComboPanel, CWnd)
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CComboPanel::CComboPanel(IPopupCtrl *subWnd) : m_pubWnd(subWnd)
{
}

CComboPanel::~CComboPanel()
{
}

void CComboPanel::PopupComboPanel()
{
	// 1. 容错
	if (!m_pubWnd)
		return;

	// 2. 显示窗口
	// 2.1 显示窗口前，通知下拉控件更新下拉列表的内容
	CString txt;
	GetParent()->GetWindowText(txt);
	m_pubWnd->OnPreShowCtrl(txt);
	
	// 2.2 显示窗口并激活
	ShowWindow(SW_SHOW);
	SetActiveWindow();
	ReleaseCapture();
	SetFocus();
}

void CComboPanel::EndEdit()
{
	// 1. 容错：如果已经隐藏了窗口，则不进入结束编辑时间，防止消息循环进入。
	if (FALSE == IsWindowVisible() && m_pubWnd)
		return;

	// 2. 隐藏窗口
	ShowWindow(SW_HIDE);

	// 更新父窗口内容即Combo编辑框的内容
	CString txt = m_pubWnd->GetCtrlTextForShow();
	SetWindowText(txt);
	auto parent = GetOwner();
	parent->SendMessage(WM_END_EDIT);
}

void CComboPanel::CreateWnd(CWnd *pParent)
{
	CRect rtWnd(0, 0, 100, 100);
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW));
	CreateEx(0, className, L"", WS_POPUP | WS_BORDER | WS_VISIBLE, rtWnd, pParent, 0);
	SetOwner(pParent);
}

CWnd* CComboPanel::GetSafeSubWnd()
{
	CWnd *pWnd = m_pubWnd->GetWnd(this);
	return (pWnd && ::IsWindow(pWnd->GetSafeHwnd())) ? NULL : pWnd;
}

void CComboPanel::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// 实时调整子窗口大小
	CWnd *pSubWnd = GetSafeSubWnd();
	if (!pSubWnd)
		return;
	if (::IsWindow(pSubWnd->GetSafeHwnd()))
	{
		CRect rtWnd;
		GetClientRect(rtWnd);		
		pSubWnd->MoveWindow(rtWnd);
	}
}

void CComboPanel::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if (nState == WA_INACTIVE)
	{
		EndEdit();
	}
}
// =================CComboAnyOne=================
CString CComboAnyOne::s_ClassName = L"";

BEGIN_MESSAGE_MAP(CComboAnyOne, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CComboAnyOne::CComboAnyOne(IPopupCtrl *subWnd):m_Panel(subWnd), m_pSubWnd(subWnd)
{
	m_pSubWnd->SetOwner(this);
	m_ComboStatus = PBS_NORMAL;

	INIT_RESHANDLE()

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_CONTROL_IMAGES);
	m_Images.Create(16, 16, ILC_COLOR32 | ILC_MASK, 1, 1);
	m_Images.Add(&bitmap, RGB(0, 0, 0));
	m_nDropDownListHeight = 128;
	m_Font.Attach((HFONT)GetStockObject(DEFAULT_GUI_FONT));
}

CComboAnyOne::~CComboAnyOne()
{
	delete m_pSubWnd;
}

void CComboAnyOne::CreateWnd(CWnd *pParent, LPRECT rt, UINT uId /*= 0*/)
{
	CRect rtWnd(0, 0, 1, 1);
	if (rt == NULL)
		rt = &rtWnd;
	if (s_ClassName == L"")
		s_ClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
	CWnd::Create(s_ClassName, L"tab23", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, *rt, pParent, 0);
	m_Panel.CreateWnd(this);
	if (uId != 0)
		SetDlgCtrlID(uId);

	EnableToolTips(TRUE);
	m_TipCtrl.Create(this, TTS_ALWAYSTIP);
	m_TipCtrl.Activate(TRUE);

	m_NotifyWnd = pParent->GetSafeHwnd();

	MSG msg;
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		PreTranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

BOOL CComboAnyOne::PreTranslateMessage(MSG* pMsg)
{
	auto retVal = __super::PreTranslateMessage(pMsg);
	if (::IsWindow(m_TipCtrl.GetSafeHwnd()))
	{
		m_TipCtrl.RelayEvent(pMsg);
	}
	return retVal;
}


CString CComboAnyOne::GetSelectionText()
{
	CString showTxt = m_pSubWnd->GetCtrlTextForShow();
	showTxt = TranslateSelection(showTxt);
	return showTxt;
}

void CComboAnyOne::SetNotifyWnd(CWnd *pNewWnd)
{
	m_NotifyWnd = pNewWnd ? pNewWnd->GetSafeHwnd() : GetParent()->GetSafeHwnd();
	m_pSubWnd->SetNotifyWnd(CWnd::FromHandle(m_NotifyWnd));
}

CString CComboAnyOne::TranslateSelection(LPCTSTR szDefTxt)
{
	CString retTxt;
	retTxt = (szDefTxt == NULL || szDefTxt[0] == NULL) ? L"无" : szDefTxt;
	return retTxt;
}

void CComboAnyOne::UpdateWndSize()
{
	if (FALSE == ::IsWindow(m_hWnd))
		return;

	// 1. 计算按钮位置
	CRect rtWnd;
	GetClientRect(rtWnd);
	m_BtnRect = rtWnd;
	m_BtnRect.left = rtWnd.right - m_BtnSize;
	m_BtnRect.top = rtWnd.CenterPoint().y - m_BtnSize / 2;
	m_BtnRect.bottom = m_BtnRect.top + m_BtnSize;
	m_BtnRect.OffsetRect(-m_nMarin, 0);

	// 2. 计算下拉框大小
	if (!m_pSubWnd)
		return;
	CSize sz = m_pSubWnd->GetBestSize();
	ClientToScreen(rtWnd);
	m_DropListRect = rtWnd;
	m_DropListRect.top = rtWnd.bottom - 1;
	m_DropListRect.left += 1;
	m_DropListRect.right -= 1;

	if (sz.cx > m_DropListRect.Width())
		m_DropListRect.right = m_DropListRect.left + sz.cx;

	m_DropListRect.bottom = m_DropListRect.top + sz.cy;
}

void CComboAnyOne::DropDownSubCtrl()
{
	UpdateWndSize();
	m_Panel.MoveWindow(m_DropListRect);
	m_Panel.PopupComboPanel();
}

CRect CComboAnyOne::GetTxtRect()
{
	CRect rtWnd;
	GetClientRect(rtWnd);
	CRect rtTxt = rtWnd;
	rtTxt.left += 4;
	rtTxt.right = m_BtnRect.left;
	return rtTxt;
}

void CComboAnyOne::OnPaint()
{
	// 绘制背景
	CPaintDC dc(this);
	CRect rtWnd;
	GetClientRect(rtWnd);

	// 用主题绘制...
	HTHEME hTheme = OpenThemeData(GetSafeHwnd(), L"Button");
	if (!hTheme)
		return;

	// 绘制背景
	DrawThemeBackground(hTheme, dc.GetSafeHdc(), BP_PUSHBUTTON, m_ComboStatus, &rtWnd, NULL);

	// 绘制文本
	auto rtTxt = GetTxtRect();
	CString showTxt = GetSelectionText();
	UINT dwStyle = DT_SINGLELINE | DT_VCENTER | DT_LEFT ;
	dc.SelectObject(m_Font.GetSafeHandle());
	DrawThemeText(hTheme, dc.GetSafeHdc(), BP_PUSHBUTTON, m_ComboStatus, showTxt, showTxt.GetLength(), dwStyle, NULL, rtTxt);

	// 绘制按钮
	m_Images.Draw(&dc, m_nDropBtPos, m_BtnRect.TopLeft(), ILS_NORMAL);
	CloseThemeData(hTheme);
}

void CComboAnyOne::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	UpdateWndSize();
	Invalidate();
}

void CComboAnyOne::OnMouseMove(UINT nFlags, CPoint point)
{
	__super::OnMouseMove(nFlags, point);
	// 先检查当前combo是否处于非激活状态(PBS_NORMAL)
	if (m_ComboStatus == PBS_NORMAL)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		tme.dwHoverTime = 10;
		::_TrackMouseEvent(&tme);
	}
	m_ComboStatus = nFlags & MK_LBUTTON ? PBS_PRESSED : PBS_HOT;

	auto showText = GetSelectionText();
	auto rtTxt = GetTxtRect();
	m_TipCtrl.AddTool(this, showText, rtTxt, m_nTipId);
	m_TipCtrl.UpdateTipText(showText, this);

	Invalidate();
}

void CComboAnyOne::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ComboStatus = PBS_PRESSED;
	Invalidate();

	bool bPanelShowed = m_Panel.IsWindowVisible();
	__super::OnLButtonDown(nFlags, point);
	if (bPanelShowed)
		m_Panel.EndEdit();
	else
		DropDownSubCtrl();// 弹出子对话框
}

void CComboAnyOne::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);
	m_ComboStatus = PBS_HOT;
	Invalidate();
}

void CComboAnyOne::OnMouseLeave()
{
	m_ComboStatus = PBS_NORMAL;
	__super::OnMouseLeave();
	Invalidate();
}
