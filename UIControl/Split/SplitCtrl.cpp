#include "StdAfx.h"
#include "SplitCtrl.h"

namespace ControlUI
{

	// 控件类名
#define SPLITCTRL_CLASS L"CSplitCtrl"

	IMPLEMENT_DYNAMIC(CSplitCtrl, CWnd)

	BEGIN_MESSAGE_MAP(CSplitCtrl, CWnd)
		ON_MESSAGE(WM_CREATE, OnCreate)
		ON_MESSAGE(WM_PAINT, OnPaint)
		ON_MESSAGE(WM_PRINTCLIENT, OnPaint)
		ON_MESSAGE(WM_SETCURSOR, OnSetCursor)
		ON_MESSAGE(WM_MOUSEMOVE, OnMouseMove)
		ON_MESSAGE(WM_LBUTTONDOWN, OnLButtonDown)
		ON_MESSAGE(WM_LBUTTONUP, OnLButtonUp)
		ON_MESSAGE(WM_LBUTTONDBLCLK, OnLButtonDoubleClick)
		ON_MESSAGE(WM_CAPTURECHANGED, OnCaptureChanged)
		ON_MESSAGE(WM_SETFOCUS, OnSetFocus)
		ON_MESSAGE(WM_MOUSEACTIVATE, OnMouseActivate)
		ON_MESSAGE(WM_SETTINGCHANGE, OnSettingChange)
		ON_MESSAGE(WM_ERASEBKGND, OnEraseBackground)
		ON_MESSAGE(WM_SIZE, OnSize)
		ON_MESSAGE(WM_MOUSEWHEEL, OnMouseWheelEx)
		//CHAIN_MSG_MAP(_baseClass)
		//FORWARD_NOTIFICATIONS()
	END_MESSAGE_MAP()

	CSplitCtrl::CSplitCtrl()
	{
		m_xySplitterPos = -1;
		m_nDefActivePane = SPLIT_PANE_NONE;

		m_cxySplitBar = 0;
		m_cxyMin = 0;
		m_cxyBarEdge = 0;
		m_cxyDragOffset = 0;
		m_nProportionalPos = 0;

		m_bFullDrag = true;
		m_bUpdateProportionalPos = true;
		t_bVertical = false;
		_bFixed = false;

		m_dwExtendedStyle = SPLIT_PROPORTIONAL;
		m_nSinglePane = SPLIT_PANE_NONE;

		m_hCursor = NULL;
		m_hWndPane[SPLIT_PANE_LEFT] = NULL;
		m_hWndPane[SPLIT_PANE_RIGHT] = NULL;

		::SetRectEmpty(&m_rcSplitter);
	}

	bool CSplitCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, bool bVertical, DWORD dwStyle)
	{
		bool bSuccess = false;
		// 水平还是垂直滚动条;
		t_bVertical = bVertical;

		if (m_hCursor == NULL)
		{
			m_hCursor = ::LoadCursor(NULL, t_bVertical ? IDC_SIZEWE : IDC_SIZENS);
		}

		// 注册控件类型
		if (RegisterTreeList())
		{
			bSuccess = !!CWnd::Create(SPLITCTRL_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
		}

		return bSuccess;
	}

	void CSplitCtrl::SetSplitterRect(LPRECT lpRect /*= NULL*/, bool bUpdate /*= true*/)
	{
		if (lpRect == NULL)
		{
			this->GetClientRect(&m_rcSplitter);
		}
		else
		{
			m_rcSplitter = *lpRect;
		}

		if (IsProportional())
		{
			UpdateProportionalPos();
		}
		else if (IsRightAligned())
		{
			UpdateRightAlignPos();
		}

		if (bUpdate)
			UpdateSplitterLayout();
	}

	void CSplitCtrl::GetSplitterRect(LPRECT lpRect) const
	{
		ATLASSERT(lpRect != NULL);
		*lpRect = m_rcSplitter;
	}

	bool CSplitCtrl::SetSplitterPos(int xyPos /*= -1*/, bool bUpdate /*= true*/)
	{
		if (xyPos == -1)   // -1 == middle
		{
			if (t_bVertical)
				xyPos = (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) / 2;
			else
				xyPos = (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge) / 2;
		}

		// Adjust if out of valid range
		int cxyMax = 0;
		if (t_bVertical)
			cxyMax = m_rcSplitter.right - m_rcSplitter.left;
		else
			cxyMax = m_rcSplitter.bottom - m_rcSplitter.top;

		if (xyPos < m_cxyMin + m_cxyBarEdge)
			xyPos = m_cxyMin;
		else if (xyPos > (cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin))
			xyPos = cxyMax - m_cxySplitBar - m_cxyBarEdge - m_cxyMin;

		// Set new position and update if requested
		bool bRet = (m_xySplitterPos != xyPos);
		m_xySplitterPos = xyPos;

		if (m_bUpdateProportionalPos)
		{
			if (IsProportional())
			{
				StoreProportionalPos();
			}
			else if (IsRightAligned())
			{
				StoreRightAlignPos();
			}
		}
		else
		{
			m_bUpdateProportionalPos = true;
		}

		if (bUpdate && bRet)
		{
			UpdateSplitterLayout();
		}

		return bRet;
	}

	void CSplitCtrl::SetSplitterPosPct(int nPct, bool bUpdate /*= true*/)
	{
		ATLASSERT(nPct >= 0 && nPct <= 100);

		m_nProportionalPos = ::MulDiv(nPct, m_nPropMax, 100);
		UpdateProportionalPos();

		if (bUpdate)
			UpdateSplitterLayout();
	}

	int CSplitCtrl::GetSplitterPos() const
	{
		return m_xySplitterPos;
	}

	bool CSplitCtrl::SetSinglePaneMode(int nPane /*= SPLIT_PANE_NONE*/)
	{
		ATLASSERT(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT || nPane == SPLIT_PANE_NONE);
		if (!(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT || nPane == SPLIT_PANE_NONE))
			return false;

		if (nPane != SPLIT_PANE_NONE)
		{
			if (!::IsWindowVisible(m_hWndPane[nPane]))
				::ShowWindow(m_hWndPane[nPane], SW_SHOW);

			int nOtherPane = (nPane == SPLIT_PANE_LEFT) ? SPLIT_PANE_RIGHT : SPLIT_PANE_LEFT;
			::ShowWindow(m_hWndPane[nOtherPane], SW_HIDE);
			if (m_nDefActivePane != nPane)
				m_nDefActivePane = nPane;
		}
		else if (m_nSinglePane != SPLIT_PANE_NONE)
		{
			int nOtherPane = (m_nSinglePane == SPLIT_PANE_LEFT) ? SPLIT_PANE_RIGHT : SPLIT_PANE_LEFT;
			::ShowWindow(m_hWndPane[nOtherPane], SW_SHOW);
		}

		m_nSinglePane = nPane;
		UpdateSplitterLayout();

		return true;
	}

	int CSplitCtrl::GetSinglePaneMode() const
	{
		return m_nSinglePane;
	}

	DWORD CSplitCtrl::GetSplitterExtendedStyle() const
	{
		return m_dwExtendedStyle;
	}

	DWORD CSplitCtrl::SetSplitterExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask /*= 0*/)
	{
		DWORD dwPrevStyle = m_dwExtendedStyle;
		if (dwMask == 0)
			m_dwExtendedStyle = dwExtendedStyle;
		else
			m_dwExtendedStyle = (m_dwExtendedStyle & ~dwMask) | (dwExtendedStyle & dwMask);
		return dwPrevStyle;
	}

	void CSplitCtrl::SetSplitterPanes(HWND hWndLeftTop, HWND hWndRightBottom, bool bUpdate)
	{
		m_hWndPane[SPLIT_PANE_LEFT] = hWndLeftTop;
		m_hWndPane[SPLIT_PANE_RIGHT] = hWndRightBottom;
		ATLASSERT(m_hWndPane[SPLIT_PANE_LEFT] == NULL || m_hWndPane[SPLIT_PANE_RIGHT] == NULL || m_hWndPane[SPLIT_PANE_LEFT] != m_hWndPane[SPLIT_PANE_RIGHT]);
		if (bUpdate)
		{
			UpdateSplitterLayout();
		}
	}

	bool CSplitCtrl::SetSplitterPane(int nPane, HWND hWnd, bool bUpdate /*= true*/)
	{
		ATLASSERT(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT);

		if (nPane != SPLIT_PANE_LEFT && nPane != SPLIT_PANE_RIGHT)
			return false;
		m_hWndPane[nPane] = hWnd;
		ATLASSERT(m_hWndPane[SPLIT_PANE_LEFT] == NULL || m_hWndPane[SPLIT_PANE_RIGHT] == NULL || m_hWndPane[SPLIT_PANE_LEFT] != m_hWndPane[SPLIT_PANE_RIGHT]);
		if (bUpdate)
		{
			UpdateSplitterLayout();
		}

		return true;
	}

	HWND CSplitCtrl::GetSplitterPane(int nPane) const
	{
		ATLASSERT(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT);

		if (nPane != SPLIT_PANE_LEFT && nPane != SPLIT_PANE_RIGHT)
			return NULL;

		return m_hWndPane[nPane];
	}

	bool CSplitCtrl::SetActivePane(int nPane)
	{
		ATLASSERT(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT);

		if (nPane != SPLIT_PANE_LEFT && nPane != SPLIT_PANE_RIGHT)
			return false;

		if (m_nSinglePane != SPLIT_PANE_NONE && nPane != m_nSinglePane)
			return false;

		::SetFocus(m_hWndPane[nPane]);
		m_nDefActivePane = nPane;

		return true;
	}

	int CSplitCtrl::GetActivePane() const
	{
		int nRet = SPLIT_PANE_NONE;
		HWND hWndFocus = ::GetFocus();
		if (hWndFocus != NULL)
		{
			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if (hWndFocus == m_hWndPane[nPane] || ::IsChild(m_hWndPane[nPane], hWndFocus))
				{
					nRet = nPane;
					break;
				}
			}
		}
		return nRet;
	}

	bool CSplitCtrl::ActivateNextPane(bool bNext /*= true*/)
	{
		int nPane = m_nSinglePane;
		if (nPane == SPLIT_PANE_NONE)
		{
			switch (GetActivePane())
			{
			case SPLIT_PANE_LEFT:
				nPane = SPLIT_PANE_RIGHT;
				break;
			case SPLIT_PANE_RIGHT:
				nPane = SPLIT_PANE_LEFT;
				break;
			default:
				nPane = bNext ? SPLIT_PANE_LEFT : SPLIT_PANE_RIGHT;
				break;
			}
		}
		return SetActivePane(nPane);
	}

	bool CSplitCtrl::SetDefaultActivePane(int nPane)
	{
		ATLASSERT(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT);

		if (nPane != SPLIT_PANE_LEFT && nPane != SPLIT_PANE_RIGHT)
			return false;
		m_nDefActivePane = nPane;
		return true;
	}

	bool CSplitCtrl::SetDefaultActivePane(HWND hWnd)
	{
		for (int nPane = 0; nPane < m_nPanesCount; nPane++)
		{
			if (hWnd == m_hWndPane[nPane])
			{
				m_nDefActivePane = nPane;
				return true;
			}
		}
		return false;   // not found
	}

	int CSplitCtrl::GetDefaultActivePane() const
	{
		return m_nDefActivePane;
	}

	void CSplitCtrl::DrawSplitter(CDC &dc)
	{
		ATLASSERT(dc.m_hDC != NULL);
		if (m_nSinglePane == SPLIT_PANE_NONE && m_xySplitterPos == -1)
			return;

		if (m_nSinglePane == SPLIT_PANE_NONE)
		{
			this->DrawSplitterBar(dc);

			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if (m_hWndPane[nPane] == NULL)
				{
					this->DrawSplitterPane(dc, nPane);
				}
			}
		}
		else
		{
			if (m_hWndPane[m_nSinglePane] == NULL)
			{
				this->DrawSplitterPane(dc, m_nSinglePane);
			}
		}
	}

	void CSplitCtrl::DrawSplitterBar(CDC &dc)
	{
		RECT rect;
		if (GetSplitterBarRect(&rect))
		{
			CBrush brush;
			brush.CreateSysColorBrush(COLOR_3DFACE);

			dc.FillRect(&rect, &brush);

			// draw 3D edge if needed
			if ((this->GetExStyle() & WS_EX_CLIENTEDGE) != 0)
			{
				dc.DrawEdge(&rect, EDGE_RAISED, t_bVertical ? (BF_LEFT | BF_RIGHT) : (BF_TOP | BF_BOTTOM));
			}
		}
	}

	void CSplitCtrl::DrawSplitterPane(CDC &dc, int nPane)
	{
		RECT rect;
		if (GetSplitterPaneRect(nPane, &rect))
		{
			if ((this->GetExStyle() & WS_EX_CLIENTEDGE) == 0)
			{
				dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
			}

			CBrush brush;
			brush.CreateSysColorBrush(COLOR_APPWORKSPACE);

			dc.FillRect(&rect, &brush);
		}
	}

	LRESULT CSplitCtrl::OnCreate(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		GetSystemSettings(false);

		return 1;
	}

	LRESULT CSplitCtrl::OnPaint(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// try setting position if not set
		if (m_nSinglePane == SPLIT_PANE_NONE && m_xySplitterPos == -1)
		{
			this->SetSplitterPos();
		}

		// do painting
		CPaintDC dc(this);
		DrawSplitter(dc);
		return 0;
	}

	LRESULT CSplitCtrl::OnSetCursor(WPARAM wParam, LPARAM lParam)
	{
		if ((HWND)wParam == this->m_hWnd && LOWORD(lParam) == HTCLIENT)
		{
			DWORD dwPos = ::GetMessagePos();
			POINT ptPos = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
			this->ScreenToClient(&ptPos);
			if (IsOverSplitterBar(ptPos.x, ptPos.y))
				return 1;
		}

		return 0;
	}

	LRESULT CSplitCtrl::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		if (_bFixed)
			return 0;

		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if ((wParam & MK_LBUTTON) && ::GetCapture() == this->m_hWnd)
		{
			int xyNewSplitPos = 0;
			if (t_bVertical)
				xyNewSplitPos = xPos - m_rcSplitter.left - m_cxyDragOffset;
			else
				xyNewSplitPos = yPos - m_rcSplitter.top - m_cxyDragOffset;

			if (xyNewSplitPos == -1)   // avoid -1, that means middle
				xyNewSplitPos = -2;

			if (m_xySplitterPos != xyNewSplitPos)
			{
				if (m_bFullDrag)
				{
					if (this->SetSplitterPos(xyNewSplitPos, true))
						this->UpdateWindow();
				}
				else
				{
					DrawGhostBar();
					this->SetSplitterPos(xyNewSplitPos, false);
					DrawGhostBar();
				}
			}
		}
		else		// not dragging, just set cursor
		{
			if (IsOverSplitterBar(xPos, yPos))
				::SetCursor(m_hCursor);
		}

		return 0;
	}

	LRESULT CSplitCtrl::OnLButtonDown(WPARAM /*wParam*/, LPARAM lParam)
	{
		if (_bFixed)
			return 1;

		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (IsOverSplitterBar(xPos, yPos))
		{
			this->SetCapture();
			::SetCursor(m_hCursor);
			if (!m_bFullDrag)
				DrawGhostBar();
			if (t_bVertical)
				m_cxyDragOffset = xPos - m_rcSplitter.left - m_xySplitterPos;
			else
				m_cxyDragOffset = yPos - m_rcSplitter.top - m_xySplitterPos;
		}

		return 1;
	}

	LRESULT CSplitCtrl::OnLButtonUp(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		::ReleaseCapture();

		return 1;
	}

	LRESULT CSplitCtrl::OnLButtonDoubleClick(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		this->SetSplitterPos();   // middle
		return 0;
	}

	LRESULT CSplitCtrl::OnCaptureChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		if (!m_bFullDrag)
		{
			DrawGhostBar();
			UpdateSplitterLayout();

			this->UpdateWindow();
		}
		return 0;
	}

	LRESULT CSplitCtrl::OnSetFocus(WPARAM /*wParam*/, LPARAM)
	{
		if (m_nSinglePane == SPLIT_PANE_NONE)
		{
			if (m_nDefActivePane == SPLIT_PANE_LEFT || m_nDefActivePane == SPLIT_PANE_RIGHT)
				::SetFocus(m_hWndPane[m_nDefActivePane]);
		}
		else
		{
			::SetFocus(m_hWndPane[m_nSinglePane]);
		}

		return 1;
	}

	LRESULT CSplitCtrl::OnMouseActivate(WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRet = DefWindowProc(33, wParam, lParam);
		if (lRet == MA_ACTIVATE || lRet == MA_ACTIVATEANDEAT)
		{
			DWORD dwPos = ::GetMessagePos();
			POINT pt = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
			this->ScreenToClient(&pt);
			RECT rcPane;
			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if (GetSplitterPaneRect(nPane, &rcPane) && ::PtInRect(&rcPane, pt))
				{
					m_nDefActivePane = nPane;
					break;
				}
			}
		}
		return lRet;
	}

	LRESULT CSplitCtrl::OnSettingChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		GetSystemSettings(true);
		return 0;
	}

	LRESULT CSplitCtrl::OnEraseBackground(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// handled, no background painting needed
		return 1;
	}

	LRESULT CSplitCtrl::OnSize(WPARAM wParam, LPARAM /*lParam*/)
	{
		if (wParam != SIZE_MINIMIZED)
			SetSplitterRect();

		return 1;
	}

	void CSplitCtrl::UpdateSplitterLayout()
	{
		if (m_nSinglePane == SPLIT_PANE_NONE && m_xySplitterPos == -1)
			return;

		RECT rect = { 0, 0, 0, 0 };
		if (m_nSinglePane == SPLIT_PANE_NONE)
		{
			if (GetSplitterBarRect(&rect))
				this->InvalidateRect(&rect);

			for (int nPane = 0; nPane < m_nPanesCount; nPane++)
			{
				if (GetSplitterPaneRect(nPane, &rect))
				{
					if (m_hWndPane[nPane] != NULL)
						::SetWindowPos(m_hWndPane[nPane], NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
					else
						this->InvalidateRect(&rect);
				}
			}
		}
		else
		{
			if (GetSplitterPaneRect(m_nSinglePane, &rect))
			{
				if (m_hWndPane[m_nSinglePane] != NULL)
					::SetWindowPos(m_hWndPane[m_nSinglePane], NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
				else
					this->InvalidateRect(&rect);
			}
		}
	}

	bool CSplitCtrl::GetSplitterBarRect(LPRECT lpRect) const
	{
		ATLASSERT(lpRect != NULL);
		if (m_nSinglePane != SPLIT_PANE_NONE || m_xySplitterPos == -1)
			return false;

		if (t_bVertical)
		{
			lpRect->left = m_rcSplitter.left + m_xySplitterPos;
			lpRect->top = m_rcSplitter.top;
			lpRect->right = m_rcSplitter.left + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
			lpRect->bottom = m_rcSplitter.bottom;
		}
		else
		{
			lpRect->left = m_rcSplitter.left;
			lpRect->top = m_rcSplitter.top + m_xySplitterPos;
			lpRect->right = m_rcSplitter.right;
			lpRect->bottom = m_rcSplitter.top + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
		}

		return true;
	}

	bool CSplitCtrl::GetSplitterPaneRect(int nPane, LPRECT lpRect) const
	{
		ATLASSERT(nPane == SPLIT_PANE_LEFT || nPane == SPLIT_PANE_RIGHT);
		ATLASSERT(lpRect != NULL);
		bool bRet = true;
		if (m_nSinglePane != SPLIT_PANE_NONE)
		{
			if (nPane == m_nSinglePane)
				*lpRect = m_rcSplitter;
			else
				bRet = false;
		}
		else if (nPane == SPLIT_PANE_LEFT)
		{
			if (t_bVertical)
			{
				lpRect->left = m_rcSplitter.left;
				lpRect->top = m_rcSplitter.top;
				lpRect->right = m_rcSplitter.left + m_xySplitterPos;
				lpRect->bottom = m_rcSplitter.bottom;
			}
			else
			{
				lpRect->left = m_rcSplitter.left;
				lpRect->top = m_rcSplitter.top;
				lpRect->right = m_rcSplitter.right;
				lpRect->bottom = m_rcSplitter.top + m_xySplitterPos;
			}
		}
		else if (nPane == SPLIT_PANE_RIGHT)
		{
			if (t_bVertical)
			{
				lpRect->left = m_rcSplitter.left + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
				lpRect->top = m_rcSplitter.top;
				lpRect->right = m_rcSplitter.right;
				lpRect->bottom = m_rcSplitter.bottom;
			}
			else
			{
				lpRect->left = m_rcSplitter.left;
				lpRect->top = m_rcSplitter.top + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge;
				lpRect->right = m_rcSplitter.right;
				lpRect->bottom = m_rcSplitter.bottom;
			}
		}
		else
		{
			bRet = false;
		}
		return bRet;
	}

	bool CSplitCtrl::IsOverSplitterRect(int x, int y) const
	{
		// -1 == don't check
		return ((x == -1 || (x >= m_rcSplitter.left && x <= m_rcSplitter.right)) &&
			(y == -1 || (y >= m_rcSplitter.top && y <= m_rcSplitter.bottom)));
	}

	bool CSplitCtrl::IsOverSplitterBar(int x, int y) const
	{
		if (m_nSinglePane != SPLIT_PANE_NONE)
			return false;
		if (m_xySplitterPos == -1 || !IsOverSplitterRect(x, y))
			return false;
		int xy = t_bVertical ? x : y;
		int xyOff = t_bVertical ? m_rcSplitter.left : m_rcSplitter.top;
		return ((xy >= (xyOff + m_xySplitterPos)) && (xy < xyOff + m_xySplitterPos + m_cxySplitBar + m_cxyBarEdge));
	}

	void CSplitCtrl::DrawGhostBar()
	{
		RECT rect = { 0, 0, 0, 0 };
		if (GetSplitterBarRect(&rect))
		{
			// invert the brush pattern (looks just like frame window sizing)
			CWindowDC dc(this);
			CBrush* brush = CDC::GetHalftoneBrush();
			if (brush != NULL)
			{
				CBrush *brushOld = dc.SelectObject(brush);
				dc.PatBlt(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
				dc.SelectObject(brushOld);
			}
		}
	}

	void CSplitCtrl::GetSystemSettings(bool bUpdate)
	{
		m_cxySplitBar = ::GetSystemMetrics(t_bVertical ? SM_CXSIZEFRAME : SM_CYSIZEFRAME);

		if ((this->GetExStyle() & WS_EX_CLIENTEDGE))
		{
			m_cxyBarEdge = 2 * ::GetSystemMetrics(t_bVertical ? SM_CXEDGE : SM_CYEDGE);
			m_cxyMin = 0;
		}
		else
		{
			m_cxyBarEdge = 0;
			m_cxyMin = 2 * ::GetSystemMetrics(t_bVertical ? SM_CXEDGE : SM_CYEDGE);
		}

#ifndef _WIN32_WCE
		::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &m_bFullDrag, 0);
#endif // !_WIN32_WCE

		if (bUpdate)
			UpdateSplitterLayout();
	}

	bool CSplitCtrl::IsProportional() const
	{
		return ((m_dwExtendedStyle & SPLIT_PROPORTIONAL) != 0);
	}

	void CSplitCtrl::StoreProportionalPos()
	{
		int cxyTotal = t_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
			m_nProportionalPos = ::MulDiv(m_xySplitterPos, m_nPropMax, cxyTotal);
		else
			m_nProportionalPos = 0;
	}

	void CSplitCtrl::UpdateProportionalPos()
	{
		int cxyTotal = t_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
		{
			int xyNewPos = ::MulDiv(m_nProportionalPos, cxyTotal, m_nPropMax);
			m_bUpdateProportionalPos = false;

			this->SetSplitterPos(xyNewPos, false);
		}
	}

	bool CSplitCtrl::IsRightAligned() const
	{
		return ((m_dwExtendedStyle & SPLIT_RIGHTALIGNED) != 0);
	}

	void CSplitCtrl::StoreRightAlignPos()
	{
		int cxyTotal = t_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
			m_nProportionalPos = cxyTotal - m_xySplitterPos;
		else
			m_nProportionalPos = 0;
	}

	void CSplitCtrl::UpdateRightAlignPos()
	{
		int cxyTotal = t_bVertical ? (m_rcSplitter.right - m_rcSplitter.left - m_cxySplitBar - m_cxyBarEdge) : (m_rcSplitter.bottom - m_rcSplitter.top - m_cxySplitBar - m_cxyBarEdge);
		if (cxyTotal > 0)
		{
			m_bUpdateProportionalPos = false;

			this->SetSplitterPos(cxyTotal - m_nProportionalPos, false);
		}
	}

	bool CSplitCtrl::IsInteractive() const
	{
		return ((m_dwExtendedStyle & SPLIT_NONINTERACTIVE) == 0);
	}

	bool CSplitCtrl::RegisterTreeList()
	{
		HINSTANCE hInstance = AfxGetInstanceHandle();

		WNDCLASS wndCls = { 0 };
		if (GetClassInfo(hInstance, SPLITCTRL_CLASS, &wndCls))
		{
			wndCls.hCursor = m_hCursor;
			return true;
		}

		wndCls.style = CS_HREDRAW | CS_VREDRAW;
		wndCls.lpfnWndProc = ::DefWindowProc;
		wndCls.cbClsExtra = 0;
		wndCls.cbWndExtra = 0;
		wndCls.hInstance = AfxGetInstanceHandle();
		wndCls.hIcon = NULL;
		wndCls.hCursor = m_hCursor; //LoadCursor(NULL, IDC_ARROW);
		wndCls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndCls.lpszMenuName = NULL;
		wndCls.lpszClassName = SPLITCTRL_CLASS;

		return !!AfxRegisterClass(&wndCls);
	}

	void CSplitCtrl::SetFixed(bool bFixed)
	{
		_bFixed = bFixed;
	}

	LRESULT CSplitCtrl::OnMouseWheelEx(WPARAM wParam, LPARAM lParam)
	{
		//Win7系统下，控件无法获取到消息，分隔条增加当前控件所在位置的消息转发
		OSVERSIONINFOEX	ver;
		DWORDLONG dwlConditionMask = 0;
		VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
		VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
		ZeroMemory(&ver, sizeof(ver));
		ver.dwMajorVersion = 6;
		ver.dwMinorVersion = 1;
		if (TRUE == VerifyVersionInfo(&ver, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask))
		{
			POINT pos;
			GetCursorPos(&pos);
			CWnd *hwnd = WindowFromPoint(pos);
			::SendMessage(hwnd->GetSafeHwnd(), WM_MOUSEWHEEL, wParam, lParam);
		}

		return TRUE;
	}
}