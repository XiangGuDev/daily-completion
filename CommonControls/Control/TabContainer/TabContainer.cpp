#include "stdafx.h"
#include "TabContainer.h"

enum { PADDING = 4, EDGE = 20};

COLORREF Darker(COLORREF crBase, float fFactor)
{
	ASSERT (fFactor < 1.0f && fFactor > 0.0f);

	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF Lighter(COLORREF crBase, float fFactor) 
{
	ASSERT (fFactor > 1.0f);

	fFactor = max(fFactor, 1.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}

CSize FormatText(CString& sText, CDC* pDC, int nWidth)
{
	CRect rect(0, 0, nWidth, 20);
	UINT uFlags = DT_CALCRECT | DT_SINGLELINE | DT_MODIFYSTRING | DT_END_ELLIPSIS;

	::DrawText(pDC->GetSafeHdc(), sText.GetBuffer(sText.GetLength() + 4), -1, rect, uFlags);
	sText.ReleaseBuffer();

	return pDC->GetTextExtent(sText);
}

CTabContainer::CTabContainer()
{
	m_crBack = -1;
	m_tabColor = RGB(145,155,165);
	m_tabTextColor = RGB(255,255,255);

	InitFont();
	_pParent = NULL;
	_nLastCurItem = -1;
	_strCloseSign = _T("  ");
	_bNeedCloseButton = false;
	_bTabItemChange = false;
}

CTabContainer::~CTabContainer()
{
	m_fontTab.DeleteObject();
}

void CTabContainer::InitFont()
{
	HFONT hDefFont = HFONT(GetStockObject(DEFAULT_GUI_FONT));
	CFont* pFont = CFont::FromHandle(hDefFont);
	LOGFONT lgfont;
	pFont->GetLogFont(&lgfont);
	
	m_fontTab.DeleteObject();
	m_fontTab.CreateFontIndirect(&lgfont);
}

int CTabContainer::SetCurSel(int nItem)
{
	int retItem = CTabCtrl::SetCurSel(nItem);
	MakeVisible(GetCurSel());

	// 设置当前页后，不属于点击修改
	_bTabItemChange = false;

	return retItem;
}

LONG CTabContainer::InsertItem(int nItem, LPCTSTR lpszItem, HWND hWnd)
{
	nItem = CTabCtrl::InsertItem(nItem, lpszItem);
	
	TCITEM tabCtrlItem = {0};
	tabCtrlItem.mask = TCIF_IMAGE | TCIF_PARAM;
	tabCtrlItem.lParam = LPARAM(hWnd);
	tabCtrlItem.iImage = nItem;
	SetItem(nItem, &tabCtrlItem);
	
	MakeVisible(GetCurSel());
	AdjustChildPos();

	return nItem;
}

LONG CTabContainer::InsertItem(int nItem, LPCTSTR lpszItem, HWND hWnd, int nImage)
{
	nItem = CTabCtrl::InsertItem(nItem, lpszItem, nImage);

	TCITEM tabCtrlItem = {0};
	tabCtrlItem.mask = TCIF_IMAGE | TCIF_PARAM;
	tabCtrlItem.lParam = LPARAM(hWnd);
	tabCtrlItem.iImage = nItem;
	SetItem(nItem, &tabCtrlItem);
	
	MakeVisible(GetCurSel());
	AdjustChildPos();

	return nItem;
}

void CTabContainer::DeleteItem(int nItem)
{
	// 隐藏将来删除的TAB
	TCITEM tabCtrlItem = {0};
	tabCtrlItem.mask = TCIF_IMAGE | TCIF_PARAM;
	tabCtrlItem.iImage = nItem;
	GetItem(nItem, &tabCtrlItem);

	HWND hWnd = HWND(tabCtrlItem.lParam);
	if ( ::IsWindow(hWnd) )
		::ShowWindow(hWnd, SW_HIDE);

	CTabCtrl::DeleteItem(nItem);
	
	// 显示新选择的TAB
	MakeVisible(GetCurSel());
}

void CTabContainer::DeleteAllItems()
{
	MakeVisible(-1);
	CTabCtrl::DeleteAllItems();
}

void CTabContainer::MakeVisible(int nItem)
{
	int nCount = GetItemCount();
	HWND hWnd = NULL;
	for( int i = 0; i < nCount; ++i )
	{
		TCITEM tabCtrlItem = {0};
		tabCtrlItem.mask = TCIF_IMAGE | TCIF_PARAM;
		tabCtrlItem.iImage = i;
		GetItem(i, &tabCtrlItem);

		if(i != nItem)
		{
			HWND hWnd = HWND(tabCtrlItem.lParam);
			if (::IsWindow(hWnd))
			{
				::ShowWindow(hWnd, SW_HIDE);
			}				

			// 上一次选中标签删除符号
			if (_bNeedCloseButton && i == _nLastCurItem)
			{
				// 获取标签文字
				TC_ITEM tci;
				CString sTemp;
				tci.mask = TCIF_TEXT;
				tci.pszText = sTemp.GetBuffer(100);
				tci.cchTextMax = 99;
				GetItem(i, &tci);
				sTemp.ReleaseBuffer();

				if (sTemp.Right(_strCloseSign.GetLength()) == _strCloseSign)
				{
					// 将关闭符号的绘制区域删除
					sTemp = sTemp.Left(sTemp.GetLength() - _strCloseSign.GetLength());

					tci.pszText = sTemp.GetBuffer(100);
					SetItem(i, &tci);
					sTemp.ReleaseBuffer();
				}
			}
		}
		else
		{
			hWnd = HWND(tabCtrlItem.lParam);

			// 选中同一标签不重复插入
			if (_bNeedCloseButton && i != _nLastCurItem)
			{
				// 获取标签文字
				TC_ITEM tci;
				CString sTemp;
				tci.mask = TCIF_TEXT;
				tci.pszText = sTemp.GetBuffer(100);
				tci.cchTextMax = 99;
				GetItem(i, &tci);
				sTemp.ReleaseBuffer();

				// 加入区域供关闭符号绘制
				sTemp += _strCloseSign;

				tci.pszText = sTemp.GetBuffer(100);
				SetItem(i, &tci);
				sTemp.ReleaseBuffer();

				// 确实切换了标签页面
				_bTabItemChange = true;
			}
			else
			{
				_bTabItemChange = false;
			}
		}			
	}

	_nLastCurItem = nItem;

	if (hWnd != NULL && ::IsWindow(hWnd))
	{
		::ShowWindow(hWnd, SW_SHOW);		
	}
}

void CTabContainer::AdjustChildPos()
{
	CRect rClient, rPage;
	GetClientRect(rClient);
	rPage = rClient;
	AdjustRect(FALSE, rPage);

	// 对竖向显示进行处理
	if(GetStyle() & TCS_VERTICAL)
		rClient.left = rPage.left - 2;
	else
		rClient.top = rPage.top - 2;
	
	int nCount = GetItemCount();
	for( int i = 0; i < nCount; ++i )
	{
		TCITEM tabCtrlItem = {0};
		tabCtrlItem.mask = TCIF_IMAGE | TCIF_PARAM;
		tabCtrlItem.iImage = i;
		GetItem(i, &tabCtrlItem);
		::MoveWindow(HWND(tabCtrlItem.lParam), rClient.left, rClient.top, rClient.Width(), rClient.Height(), TRUE);
	}
}

BEGIN_MESSAGE_MAP(CTabContainer, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, &CTabContainer::OnTcnSelChanging)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabContainer::OnTcnSelChange)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, &CTabContainer::OnNMClickTab)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CTabContainer::OnToolTipNotify)
END_MESSAGE_MAP()

int CTabContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 设置字体
	SetFont(&m_fontTab);

	return 0;
}

void CTabContainer::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	TC_ITEM     tci;
	CDC* pDC = CDC::FromHandle(lpdis->hDC);
	HIMAGELIST hilTabs = (HIMAGELIST)TabCtrl_GetImageList(GetSafeHwnd());

	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());
	CRect rItem(lpdis->rcItem);

	if (bSelected)
		rItem.bottom -= 1;
	else
		rItem.bottom += 2;

	COLORREF crFrom = GetTabColor(bSelected);

	pDC->FillSolidRect(rItem, crFrom);

	// text & icon
	rItem.left += PADDING;
	rItem.top += PADDING + (bSelected ? 1 : 0);

	pDC->SetBkMode(TRANSPARENT);

	CString sTemp;
	tci.mask        = TCIF_TEXT | TCIF_IMAGE;
	tci.pszText     = sTemp.GetBuffer(100);
	tci.cchTextMax  = 99;
	GetItem(lpdis->itemID, &tci);
	sTemp.ReleaseBuffer();

	// icon
	if (hilTabs)
	{
		ImageList_Draw(hilTabs, tci.iImage, *pDC, rItem.left, rItem.top-2, ILD_TRANSPARENT);
		rItem.left += 16 + PADDING;
	}

	// text
	rItem.right -= PADDING;

	HFONT hCurFont = m_fontTab;
	HFONT hOldFont = HFONT(pDC->SelectObject(hCurFont));

	FormatText(sTemp, pDC, rItem.Width());

	pDC->SetTextColor(GetTabTextColor(bSelected));
	pDC->DrawText(sTemp, rItem, DT_NOPREFIX | DT_CENTER);

	// 画出关闭符号
	if (_bNeedCloseButton && bSelected)
	{
		_rcCloseSign = rItem;
		_rcCloseSign.top += 2;
		_rcCloseSign.bottom -= 6;
		_rcCloseSign.right -= 1;
		_rcCloseSign.left = _rcCloseSign.right - _rcCloseSign.Height();

		CPen penCloseSign;
		penCloseSign.CreatePen(PS_SOLID, 2, GetTabTextColor(bSelected));
		CPen* pOldPen = pDC->SelectObject(&penCloseSign);

		pDC->MoveTo(_rcCloseSign.left, _rcCloseSign.top);
		pDC->LineTo(_rcCloseSign.right, _rcCloseSign.bottom);

		pDC->MoveTo(_rcCloseSign.right, _rcCloseSign.top);
		pDC->LineTo(_rcCloseSign.left, _rcCloseSign.bottom);

		pDC->SelectObject(pOldPen);
	}
	
	pDC->SelectObject(hOldFont);
}

void CTabContainer::DrawItemBorder(LPDRAWITEMSTRUCT lpdis)
{
	BOOL bSelected = (lpdis->itemID == (UINT)GetCurSel());

	CRect rItem(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	COLORREF crTab = GetTabColor(bSelected);
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);

	if (bSelected)
	{
		rItem.bottom += bSelected ? -1 : 1;

		// edges
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.left + 1, rItem.bottom), crHighlight);
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.right, rItem.top + 1), crHighlight);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
	else // draw simple dividers
	{
		pDC->FillSolidRect(CRect(rItem.left, rItem.top, rItem.left, rItem.bottom), crShadow);
		pDC->FillSolidRect(CRect(rItem.right - 1, rItem.top, rItem.right, rItem.bottom), crShadow);
	}
}

void CTabContainer::DrawMainBorder(LPDRAWITEMSTRUCT lpdis)
{
	CRect rBorder(lpdis->rcItem);
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 

	COLORREF crTab = GetTabColor();
	COLORREF crHighlight = Lighter(crTab, 1.5f);
	COLORREF crShadow = Darker(crTab, 0.75f);

	pDC->Draw3dRect(rBorder, crHighlight, crShadow);
}

COLORREF CTabContainer::GetTabColor(BOOL bSelected)
{
	if (bSelected )
	{
		return m_tabColor;
	}
	else
	{
		return GetSysColor(COLOR_BTNFACE);
	}
}

COLORREF CTabContainer::GetTabTextColor(BOOL bSelected)
{
	if (bSelected)
	{
		return m_tabTextColor;
	}
	else
	{
		return Darker(m_tabTextColor, 0.1f);
	}
}

void CTabContainer::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();

	// 设置字体
	SetFont(&m_fontTab);

	ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

BOOL CTabContainer::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(&rcClient, GetSysColor(COLOR_3DFACE));

	return TRUE;
}

void CTabContainer::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// prepare dc
	CFont* font = GetFont();
	dc.SelectObject(font);

	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TAB;
	dis.CtlID = GetDlgCtrlID();
	dis.hwndItem = GetSafeHwnd();
	dis.hDC = dc.GetSafeHdc();
	dis.itemAction = ODA_DRAWENTIRE;

	// draw the rest of the border
	CRect rPage;
	GetClientRect(&dis.rcItem);
	rPage = dis.rcItem;
	AdjustRect(FALSE, rPage);
	dis.rcItem.top = rPage.top - 2;

	DrawMainBorder(&dis);

	// paint the tabs first and then the borders
	int nTab = GetItemCount();
	int nSel = GetCurSel();

	if (!nTab) // no pages added
		return;

	while (nTab--)
	{
		if (nTab != nSel)
		{
			dis.itemID = nTab;
			dis.itemState = 0;

			VERIFY(GetItemRect(nTab, &dis.rcItem));

			dis.rcItem.bottom -= 2;
			DrawItem(&dis);
			DrawItemBorder(&dis);
		}
	}

	// now selected tab
	dis.itemID = nSel;
	dis.itemState = ODS_SELECTED;

	VERIFY(GetItemRect(nSel, &dis.rcItem));

	dis.rcItem.bottom += 2;
	dis.rcItem.top -= 1;
	DrawItem(&dis);
	DrawItemBorder(&dis);
}

void CTabContainer::OnSize(UINT nType, int cx, int cy)
{
	CTabCtrl::OnSize(nType, cx, cy);

	if ( GetSafeHwnd() )
	{
		AdjustChildPos();
	}
}

void CTabContainer::OnTcnSelChanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	int curSel = GetCurSel();

	CWnd* pParent = GetParent();
	if (NULL != _pParent)
		pParent = _pParent;
	if (pParent == NULL)
		return;

	*pResult = pParent->SendMessage(TAB_SELECTING, (WPARAM)m_hWnd, curSel);
}

void CTabContainer::OnTcnSelChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int curSel = GetCurSel(); 
	MakeVisible(curSel);

	CWnd* pParent = GetParent();
	if (NULL != _pParent)
		pParent = _pParent;
	if (pParent == NULL)
		return;
	pParent->SendMessage(TAB_SELECT, (WPARAM)m_hWnd, curSel);
	*pResult = 0;
}

void CTabContainer::SetMsgParent( CWnd* pParent )
{
	_pParent = pParent;
}

void CTabContainer::SetTabText(int nItem, LPCTSTR lpStrText)
{
	TCHAR buffer[256] = {0};
	lstrcpyn(buffer, lpStrText, 256);

	TCITEM tcItem = {0};
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = buffer;
	SetItem(nItem, &tcItem);
}

CString CTabContainer::GetTabText(int nItem)
{
	TCITEM tcItem = {0};
	tcItem.mask = TCIF_TEXT;
	CString strText;
	tcItem.pszText = strText.GetBuffer(256);
	tcItem.cchTextMax = 256;
	GetItem(nItem, &tcItem);
	strText.ReleaseBuffer(256);

	return strText;
}

void CTabContainer::OnNMClickTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!_bNeedCloseButton || _bTabItemChange)
	{
		_bTabItemChange = false;
		return;
	}

	CPoint ptMouse;
	
	// 获取当前鼠标位置
	GetCursorPos(&ptMouse);
	this->ScreenToClient(&ptMouse);

	// 将区域略微扩展
	CRect rcMouse = _rcCloseSign;
	rcMouse.left -= 1;
	rcMouse.top -= 1;
	rcMouse.right += 2;
	rcMouse.bottom += 2;

	// 鼠标在关闭符号的区域内
	if (rcMouse.PtInRect(ptMouse))
	{
		//DeleteItem(GetCurSel());TAB_ITEM_CLOSE
		
		// 发送关闭符号被点击的事件
		CTabContainerNotify notify;
		notify.hdrNotify.hwndFrom = m_hWnd;
		notify.hdrNotify.idFrom = GetDlgCtrlID();
		notify.hdrNotify.code = TAB_ITEM_CLOSE;
		notify.nSelItem = GetCurSel();

		HWND hNotifyWhd = NULL;

		// 获取有效的消息传递窗口
		if (_pParent == NULL)
		{
			hNotifyWhd = GetParent()->m_hWnd;
		}
		else
		{
			hNotifyWhd = _pParent->m_hWnd;
		}

		::SendMessage(hNotifyWhd, WM_NOTIFY, 0, (LPARAM)&notify);
	}
}

void CTabContainer::SetItemCloseButton(bool bNeedCloseButton /*= true*/)
{
	// 设置显示关闭按钮标识
	_bNeedCloseButton = bNeedCloseButton;

	// 清空关闭按钮坐标
	_rcCloseSign.SetRectEmpty();
}

BOOL CTabContainer::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	// 发送移动显示tooltips的事件
	CTabContainerNotify notify;
	notify.hdrNotify.hwndFrom = m_hWnd;
	notify.hdrNotify.idFrom = pNMHDR->idFrom;
	notify.hdrNotify.code = TTN_NEEDTEXT;
	notify.nSelItem = GetCurSel();

	HWND hNotifyWhd = NULL;

	// 获取有效的消息传递窗口
	if (_pParent == NULL)
	{
		hNotifyWhd = GetParent()->m_hWnd;
	}
	else
	{
		hNotifyWhd = _pParent->m_hWnd;
	}
	*pResult = ::SendMessage(hNotifyWhd, TAB_TOOLTIPS, (WPARAM)pNMHDR, (LPARAM)&notify);
	return TRUE;
}