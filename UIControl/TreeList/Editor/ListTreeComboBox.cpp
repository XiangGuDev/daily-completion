#include "StdAfx.h"
#include "ListTreeComboBox.h"

namespace ControlUI
{
	static LRESULT CALLBACK TreeComboEditProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		CListTreeComboBox *pCob = reinterpret_cast<CListTreeComboBox*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (NULL != pCob)
		{
			if (message == WM_KEYDOWN)
			{
				pCob->ProcessKeyDown((UINT)wParam, 0, 0);
			}
			else if (message == WM_KILLFOCUS)
			{
				pCob->DoKillFocus(CWnd::FromHandle((HWND)wParam));
			}

			return pCob->_pEditProc(hWnd, message, wParam, lParam);
		}
		else
		{
			return ::DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	IMPLEMENT_DYNAMIC(CListTreeComboBox, CTreeComboBox)

	BEGIN_MESSAGE_MAP(CListTreeComboBox, CTreeComboBox)
		ON_WM_KILLFOCUS()
		ON_WM_KEYDOWN()
		ON_CONTROL_REFLECT(CBN_EDITCHANGE, &CListTreeComboBox::OnEditChange)
	END_MESSAGE_MAP()

	CListTreeComboBox::CListTreeComboBox()
	{
		_bCandel = false;

		_crNormal = GetSysColor(COLOR_WINDOW);
		_crSelected = GetSysColor(COLOR_HIGHLIGHT);
		_crText = GetSysColor(COLOR_WINDOWTEXT);

		_bDropList = false;
		_pEditProc = NULL;

		_bIsSelect = true;
	}

	CListTreeComboBox::~CListTreeComboBox()
	{
	}

	bool CListTreeComboBox::CreateCtrl(CWnd* pParent, CRect& rect, LPCTSTR lpszInit, LPCTSTR lpszData, bool bDropList)
	{
		_bCandel = false;
		_bDropList = bDropList;

		if (!_container.Create(rect, pParent))
			return false;

		// 不显示边框;
		CRect rc(-1, -2, rect.Width() + 1, 226);
		DWORD dwEditStyle = WS_CHILD | CBS_AUTOHSCROLL | CBS_HASSTRINGS | WS_VSCROLL | CBS_OWNERDRAWVARIABLE
			| CBS_NOINTEGRALHEIGHT | (_bDropList ? CBS_DROPDOWNLIST : CBS_DROPDOWN);

		if (!CTreeComboBox::Create(dwEditStyle, rc, &_container, 0xffff))
			return false;

		_strInit = lpszInit;
		_strData = lpszData;

		// 关联Edit消息处理
		CWnd* pWndEdit = GetWindow(GW_CHILD);
		if (!_bDropList && (NULL != pWndEdit))
		{
			::SetWindowLongPtr(pWndEdit->m_hWnd, GWLP_USERDATA, LONG_PTR(this));
			_pEditProc = (WNDPROC)::SetWindowLongPtr(pWndEdit->m_hWnd, GWLP_WNDPROC, LONG_PTR(TreeComboEditProcedure));
		}

		return true;
	}

	bool CListTreeComboBox::LoadXmlData(const CString& xmlFile, bool bNoLimitOrSelAll)
	{
		if (!CTreeComboBox::LoadXmlData(xmlFile, bNoLimitOrSelAll))
			return false;

		AutoSelectItem();
		return true;
	}

	void CListTreeComboBox::LoadStringVector(CStringVector& listData, bool bNoLimitOrSelAll)
	{
		CTreeComboBox::LoadStringVector(listData, bNoLimitOrSelAll);
		AutoSelectItem();
	}

	void CListTreeComboBox::OnKillFocus(CWnd* pNewWnd)
	{
		CTreeComboBox::OnKillFocus(pNewWnd);
		DoKillFocus(pNewWnd);
	}

	void CListTreeComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		ProcessKeyDown(nChar, nRepCnt, nFlags);
		CTreeComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void CListTreeComboBox::AutoSelectItem()
	{
		if (_bCheckMode || !_bDropList)
		{
			int nIdx = AddString(_strInit);
			SetCurSel(nIdx);
			CString* pStrData = new CString(_strData);
			SetItemData(nIdx, DWORD_PTR(pStrData));
		}
		else
		{
			// 如果默认值在下拉框范围内则直接设置当前选择索引;
			int nIndex = CB_ERR, subIndex = CB_ERR;

			do
			{
				subIndex = FindStringExact(nIndex, _strInit);
				if (subIndex <= nIndex)
					break;

				CString strData;
				CString *pData = (CString*)GetItemData(subIndex);
				if (pData)
					strData = *pData;

				// 判断附加数据是否符合
				if (strData == _strData)
					break;

				nIndex = subIndex;
			} while (true);

			if (subIndex <= nIndex)
				nIndex = FindStringExact(-1, _strInit);
			else
				nIndex = subIndex;

			if (nIndex != CB_ERR)
				SetCurSel(nIndex);
		}
	}
	void CListTreeComboBox::MeasureItem(LPMEASUREITEMSTRUCT pMItem)
	{
		pMItem->itemHeight = 20;
	}

	void CListTreeComboBox::DrawItem(LPDRAWITEMSTRUCT pDIStruct)
	{
		DrawItem(pDIStruct->hDC, pDIStruct->rcItem, pDIStruct->itemID, pDIStruct->itemState);
	}

	void CListTreeComboBox::DrawItem(HDC hDC, const CRect& rcItem, int iItem, int iState)
	{
		CRect rcText = rcItem;
		CDC dcContext;
		dcContext.Attach(hDC);

		// 选择项颜色
		if (iState & ODS_SELECTED && !(iState & ODS_COMBOBOXEDIT))
		{
			dcContext.SetTextColor((0x00FFFFFF & ~(_crText)));
			dcContext.SetBkColor(_crSelected);
			dcContext.FillSolidRect(&rcText, _crSelected);
		}
		else
		{
			dcContext.SetTextColor(_crText);
			dcContext.SetBkColor(_crNormal);
			dcContext.FillSolidRect(&rcText, _crNormal);	// 清除背景
		}

		// 选中背景与焦点线
		if (iState & ODS_FOCUS && !(iState & ODS_COMBOBOXEDIT))
		{
			dcContext.DrawFocusRect(&rcText);
		}

		// 文字向右移两个像素;
		rcText.left += 2;
		if (iItem != -1)
		{
			CString iText;
			GetLBText(iItem, iText);

			dcContext.SetBkMode(TRANSPARENT);
			dcContext.DrawText(iText, iText.GetLength(), rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		}

		dcContext.Detach();
	}

	void CListTreeComboBox::CancelEdit()
	{
		_bCandel = true;

		CWnd* pParent = _container.GetParent();
		if (NULL != pParent)
		{
			_container.DestroyWindow();
			pParent->SetFocus();
		}
	}

	void CListTreeComboBox::DoKillFocus(CWnd* pWnd)
	{
		if (pWnd == NULL || pWnd == this)
			return;

		// 如果是编缉模式的组合框，当获取焦点是自己的编缉器时不应销毁控件;
		if (!_bDropList && ((pWnd == GetWindow(GW_CHILD)) || (pWnd == this)))
			return;

		if (_bCandel)
			return;

		if (_dropTree.GetSafeHwnd() == pWnd->GetSafeHwnd())
			return;

		CWnd* pWndParent = NULL;
		if (GetParent() != NULL)
		{
			pWndParent = GetParent()->GetParent();
		}

		// 发送修改事件;
		if (pWndParent != NULL)
		{
			CString strText = GetControlText();
			CString strData = GetCurrentSelData();
			if (!_bCheckMode)
			{
				if (!_bIsSelect)
				{
					strData = L"";
				}
				SetSelectData(strText, strData);
			}

			_bIsSelect = true;

			pWndParent->SendMessage(LIST_CTRL_END, (WPARAM)((LPCTSTR)strText), (LPARAM)((LPCTSTR)strData));
		}

		if (IsWindow(m_hWnd))
			_container.DestroyWindow();
	}

	void CListTreeComboBox::ProcessKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		CWnd* pWndParent = NULL;
		if (GetParent() != NULL)
		{
			pWndParent = GetParent()->GetParent();
		}

		if (nChar == VK_ESCAPE)
		{
			_bCandel = true;
			if (IsWindow(m_hWnd))
				_container.DestroyWindow();

			if (pWndParent != NULL)
				pWndParent->SetFocus();
		}
		else if (nChar == VK_RETURN || nChar == VK_EXECUTE)
		{
			if (pWndParent != NULL)
				pWndParent->SetFocus();
		}
	}

	void CListTreeComboBox::OnEditChange()
	{
		_bIsSelect = false;
	}
}