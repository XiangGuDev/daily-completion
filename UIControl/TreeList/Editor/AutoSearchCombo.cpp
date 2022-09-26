#include "StdAfx.h"
#include "AutoSearchCombo.h"

namespace ControlUI
{
	IMPLEMENT_DYNAMIC(CAutoSearchCombo, CComboBox)
	BEGIN_MESSAGE_MAP(CAutoSearchCombo, CComboBox)
		ON_WM_KILLFOCUS()
		ON_WM_KEYDOWN()
		ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditChange)
		ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
	END_MESSAGE_MAP()

	static LRESULT CALLBACK CoboboxEditProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		CAutoSearchCombo *pCob = reinterpret_cast<CAutoSearchCombo*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

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



	CAutoSearchCombo::CAutoSearchCombo()
	{
		_autoComplete = false;
		_bCandel = false;
		_bDropList = false;

		_crNormal = GetSysColor(COLOR_WINDOW);
		_crSelected = GetSysColor(COLOR_HIGHLIGHT);
		_crText = GetSysColor(COLOR_WINDOWTEXT);

		_pEditProc = NULL;
	}

	CAutoSearchCombo::~CAutoSearchCombo()
	{
	}

	bool CAutoSearchCombo::Create(CWnd* pParent, CRect& rect, int option, LPCTSTR sInitText)
	{
		_bCandel = false;
		_bDropList = !!(option & eDropListCobEdit);
		_strInitText = sInitText;

		if (!_container.Create(rect, pParent))
			return false;

		// 不显示边框;
		CRect rc(-1, -2, rect.Width() + 1, 226);
		DWORD dwEditStyle = WS_CHILD | CBS_AUTOHSCROLL | CBS_HASSTRINGS | WS_VSCROLL | CBS_OWNERDRAWVARIABLE
			| CBS_NOINTEGRALHEIGHT | (_bDropList ? CBS_DROPDOWNLIST : CBS_DROPDOWN);
		if (!CComboBox::Create(dwEditStyle, rc, &_container, 0xffff))
			return false;

		// 关联Edit消息处理
		CWnd* pWndEdit = GetWindow(GW_CHILD);
		if (!_bDropList && (NULL != pWndEdit))
		{
			::SetWindowLongPtr(pWndEdit->m_hWnd, GWLP_USERDATA, LONG_PTR(this));
			_pEditProc = (WNDPROC)::SetWindowLongPtr(pWndEdit->m_hWnd, GWLP_WNDPROC, LONG_PTR(CoboboxEditProcedure));
		}

		return true;
	}

	void CAutoSearchCombo::DrawItem(HDC hDC, const CRect& rcItem, int iItem, int iState)
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
			TCHAR iText[MAX_PATH];
			GetLBText(iItem, iText);

			dcContext.SetBkMode(TRANSPARENT);
			dcContext.DrawText(iText, lstrlen(iText), rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		}

		dcContext.Detach();
	}


	BOOL CAutoSearchCombo::PreTranslateMessage(MSG* pMsg)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			_autoComplete = true;
			int vk = (int)pMsg->wParam;

			if (vk == VK_DELETE || vk == VK_BACK)
				_autoComplete = false;
		}

		return CComboBox::PreTranslateMessage(pMsg);
	}

	void CAutoSearchCombo::MeasureItem(LPMEASUREITEMSTRUCT pMItem)
	{
		pMItem->itemHeight = 20;
	}

	void CAutoSearchCombo::DrawItem(LPDRAWITEMSTRUCT pDIStruct)
	{
		DrawItem(pDIStruct->hDC, pDIStruct->rcItem, pDIStruct->itemID, pDIStruct->itemState);
	}

	void CAutoSearchCombo::ProcessKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	void CAutoSearchCombo::SetSelAndFocus()
	{
		SetFocus();

		if (!_bDropList)
		{
			SetEditSel(0, -1);
		}
	}

	void CAutoSearchCombo::DoKillFocus(CWnd* pWnd)
	{
		// 如果是编缉模式的组合框，当获取焦点是自己的编缉器时不应销毁控件;
		if (!_bDropList && (pWnd == GetWindow(GW_CHILD)))
			return;

		if (_bCandel)
			return;

		CWnd* pWndParent = NULL;
		if (GetParent() != NULL)
		{
			pWndParent = GetParent()->GetParent();
		}

		// 发送修改事件;
		if (pWndParent != NULL)
		{
			CString strText, strData;
			int nSel = GetCurSel();

			if (nSel != CB_ERR && GetCount() > nSel)
			{
				GetLBText(nSel, strText);
				strData = (LPCTSTR)GetItemData(nSel);
			}

			GetWindowText(strText);
			pWndParent->SendMessage(LIST_CTRL_END, (WPARAM)((LPCTSTR)strText), (LPARAM)((LPCTSTR)strData));
		}

		if (IsWindow(m_hWnd))
			_container.DestroyWindow();
	}

	void CAutoSearchCombo::CancelEdit()
	{
		_bCandel = true;

		CWnd* pParent = _container.GetParent();
		if (NULL != pParent)
		{
			_container.DestroyWindow();
			pParent->SetFocus();
		}
	}

	int CAutoSearchCombo::ResetDropContent()
	{
		COMBOBOXINFO cbi;
		cbi.cbSize = sizeof(COMBOBOXINFO);
		GetComboBoxInfo(&cbi);

		CString strOldValue;
		CEdit *pEdit = (CEdit *)CWnd::FromHandle(cbi.hwndItem);
		pEdit->GetWindowText(strOldValue);

		CListBox *pList = (CListBox*)CWnd::FromHandle(cbi.hwndList);
		pList->ResetContent();

		for (size_t i = 0; i < _loadStringList.size(); ++i)
		{
			std::pair<CString, CString> item = _loadStringList[i];
			CString strItem = item.first;
			CString strData = item.second;

			if (strItem.Find(strOldValue) != -1)
			{
				int nIndex = pList->AddString(strItem);
				pList->SetItemData(nIndex, (DWORD_PTR)((LPCTSTR)strData));
			}
		}

		CAutoWidthComboBox::ComboxAutoResize(*this);


		if (pList->GetCount() > 0)
			return 0;
		else
			return -1;
	}

	bool CAutoSearchCombo::LoadXmlData(const CString& strXmlFile)
	{
		_loadStringList.clear();

		CXmlDocument xmlDoc;
		if (!xmlDoc.LoadFile(strXmlFile, fmtXML))
			return false;

		// 遍历添加下拉数据
		CXmlElements *pChilds = xmlDoc.GetElementRoot()->GetChildElements();
		size_t nChild = pChilds->GetCount();
		for (size_t i = 0; i < nChild; ++i)
		{
			CXmlElement* pChildElm = pChilds->GetAt(i);
			CString strName = pChildElm->GetAttrValue(L"Name");
			CString strMatCode = pChildElm->GetAttrValue(L"MaterialCode");

			_loadStringList.push_back(std::pair<CString, CString>(strName, strMatCode));
		}

		// 设置默认值
		SetWindowText(_strInitText);

		// 重置下拉内容
		ResetDropContent();

		return true;
	}

	void CAutoSearchCombo::LoadStringVector(CStringVector& listData)
	{
		_loadStringList.clear();

		// 添加下拉列表内容;
		CStringVectorIter endIter = listData.end();
		for (CStringVectorIter beginIter = listData.begin(); beginIter != endIter; ++beginIter)
		{
			const CString& strString = (*beginIter);
			_loadStringList.push_back(std::pair<CString, CString>(strString, L""));
		}

		// 设置默认值
		SetWindowText(_strInitText);

		// 重置下拉内容
		ResetDropContent();
	}

	void CAutoSearchCombo::OnKillFocus(CWnd* pNewWnd)
	{
		CComboBox::OnKillFocus(pNewWnd);

		DoKillFocus(pNewWnd);
	}

	void CAutoSearchCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		ProcessKeyDown(nChar, nRepCnt, nFlags);

		CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void CAutoSearchCombo::OnEditChange()
	{
		CString strOldValue;
		GetWindowText(strOldValue);


		int hilight = strOldValue.GetLength();
		int reset = ResetDropContent();

		if (!_autoComplete)
		{
			_autoComplete = true;
			return;
		}

		if (reset != -1)
		{
			ShowDropDown(TRUE);
			COMBOBOXINFO cb;
			cb.cbSize = sizeof(COMBOBOXINFO);
			GetComboBoxInfo(&cb);
			CListBox *pList = (CListBox*)CWnd::FromHandle(cb.hwndList);
			// PostMessage(CB_SETCURSEL, 0, 0);
			pList->SetCurSel(0);
		}
		else
			ShowDropDown(FALSE);

		SendMessage(CB_SETEDITSEL, 0, MAKELPARAM(hilight, -1));
	}

	void CAutoSearchCombo::OnDropDown()
	{
		::SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
}