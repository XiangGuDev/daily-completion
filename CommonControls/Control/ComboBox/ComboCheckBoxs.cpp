#include "stdafx.h"
#include "ComboCheckBoxs.h"

#define RANDOM_ID (rand()%1024+1000)

BEGIN_MESSAGE_MAP(CCheckBoxList, CTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClickTree)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDBClickTree)
END_MESSAGE_MAP()

CCheckBoxList::CCheckBoxList()
{
	srand(0);
}

CCheckBoxList::~CCheckBoxList()
{

}

int CCheckBoxList::AddItem(LPCTSTR szItem, DWORD_PTR extraData)
{
	auto newItem = InsertItem(szItem, NULL);
	SetItemData(newItem, extraData);
	m_arrTreeItems.push_back(newItem);
	return (int)m_arrTreeItems.size() - 1;
}

HTREEITEM CCheckBoxList::GetItemAt(int nIndex)
{
	return m_arrTreeItems.size() > nIndex && nIndex >= 0 ? m_arrTreeItems[nIndex] : NULL;
}

void CCheckBoxList::DeleteItem(int nIndex)
{
	auto item = GetItemAt(nIndex);
	if (item)
	{
		CTreeCtrl::DeleteItem(item);
		m_arrTreeItems.erase(m_arrTreeItems.begin() + nIndex);
		auto res = std::find(m_arrSelItems.begin(), m_arrSelItems.end(), item);
		if (res != m_arrSelItems.end())
			m_arrSelItems.erase(res);
	}
}

void CCheckBoxList::DeleteAllItem()
{
	m_arrTreeItems.clear();
	m_arrSelItems.clear();
	CTreeCtrl::DeleteAllItems();
}

BOOL CCheckBoxList::SetCheck(HTREEITEM hItem, BOOL fCheck /*= TRUE*/)
{
	BOOL bRetVal = CTreeCtrl::SetCheck(hItem, fCheck);
	SendNotify(TCN_CHECKSTATUS_CHANGE, hItem);
	if (fCheck)
	{
		auto res = std::find(m_arrSelItems.begin(), m_arrSelItems.end(), hItem);
		if (res == m_arrSelItems.end())
			m_arrSelItems.push_back(hItem);
	}
	else
	{
		auto res = std::find(m_arrSelItems.begin(), m_arrSelItems.end(), hItem);
		if (res != m_arrSelItems.end())
			m_arrSelItems.erase(res);
	}

	CWnd::Invalidate();
	MSG msg;
	while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		PreTranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return bRetVal;
}

int CCheckBoxList::GetIndex(HTREEITEM hItem)
{
	auto res = std::find(m_arrTreeItems.begin(), m_arrTreeItems.end(), hItem);
	return res == m_arrTreeItems.end() ? -1 : res - m_arrTreeItems.begin();
}

void CCheckBoxList::SendNotify(UINT code, HTREEITEM hItem)
{
	ComboCheckBoxNotify notifyMsg;
	notifyMsg.triggleItemIndex = GetIndex(hItem);
	notifyMsg.hwndFrom = m_pOwner->GetSafeHwnd();
	notifyMsg.idFrom = m_pOwner->GetDlgCtrlID();
	notifyMsg.code = code;
	if (m_pNotifyWnd)
		::SendMessage(m_pNotifyWnd->GetSafeHwnd(), WM_NOTIFY, notifyMsg.idFrom, (LPARAM)&notifyMsg);
}

void CCheckBoxList::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnLButtonDown(nFlags, point);
	HTREEITEM item = HitTest(point);
	if (item != NULL)
		SelectItem(item);

	UpdateOwner();
}

void CCheckBoxList::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pHdr = (NM_TREEVIEW*)pNMHDR;
	CPoint point;
	UINT uFlag;
	GetCursorPos(&point);
	ScreenToClient(&point);
	HTREEITEM hItem = HitTest(point, &uFlag);
	if ((hItem) && (TVHT_ONITEMSTATEICON & uFlag) && m_pNotifyWnd)
	{
		BOOL checkStatus = GetCheck(hItem);
		SetCheck(hItem, !checkStatus);
		*pResult = 1;
	}
	else
	{
		*pResult = 0;
	}
}

void CCheckBoxList::OnNMDBClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 1;
}

CWnd* CCheckBoxList::GetWnd(CWnd *pOwner)
{
	if (m_hWnd == NULL)
	{
		CRect rtWnd;
		pOwner->GetClientRect(rtWnd);
		UINT uId = pOwner->GetDlgCtrlID() + RANDOM_ID;
		Create(WS_VISIBLE | WS_CHILD | TVS_CHECKBOXES | TVS_FULLROWSELECT, rtWnd, pOwner, uId);
	}
	return this;
}

CString CCheckBoxList::GetCtrlTextForShow()
{
	CString retTxt;
	for (auto item : m_arrSelItems)
	{
		retTxt += GetItemText(item);
		retTxt += L";";
	}
	retTxt.TrimRight(L';');
	return retTxt;
}

void CCheckBoxList::OnPreShowCtrl(LPCTSTR curComboTxt)
{
// 	for (auto item : m_arrSelItems)
// 		CTreeCtrl::SetCheck(item, TRUE);
	Invalidate(TRUE);
}

CSize CCheckBoxList::GetBestSize()
{
	CSize retSize(0, 100);
	if (::IsWindow(GetSafeHwnd()))
	{
		int nSingleHeight = GetItemHeight();
		auto root = GetRootItem();
		int nTotalItem = 0;
		retSize.cy = max(nSingleHeight, nSingleHeight * m_arrTreeItems.size());
	}
	return retSize;
}

CComboCheckBoxs::CComboCheckBoxs() :CComboAnyOne(new CCheckBoxList())
{
}


CComboCheckBoxs::~CComboCheckBoxs()
{
}

int CComboCheckBoxs::AddItem(LPCTSTR szItem, DWORD_PTR extraData)
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return -1;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	return pList->AddItem(szItem, extraData);
}

void CComboCheckBoxs::DeleteAllItem()
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	pList->DeleteAllItem();
}

void CComboCheckBoxs::SetItemSelectStatus(int nIndex, BOOL bSelected)
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	auto hItem = pList->GetItemAt(nIndex);
	pList->SetCheck(hItem, bSelected);
}

BOOL CComboCheckBoxs::GetItemStatus(int nIndex)
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return FALSE;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	auto hItem = pList->GetItemAt(nIndex);
	return hItem == NULL ? FALSE : pList->GetCheck(hItem);
}

void CComboCheckBoxs::GetSelectionIndexs(std::vector<int> &indexBuffer)
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	auto root = pList->GetRootItem();
	int nIndex = 0;
	for (auto child = root; child != NULL; child = pList->GetNextSiblingItem(child), ++nIndex)
	{
		auto status = pList->GetCheck(child);
		if (status)
		{
			indexBuffer.push_back(nIndex);
		}
	}
}

void CComboCheckBoxs::GetSelectionExtraDatas(std::vector<DWORD_PTR> &extraBuffer)
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	auto root = pList->GetRootItem();
	int nIndex = 0;
	for (auto child = root; child != NULL; child = pList->GetNextSiblingItem(child), ++nIndex)
	{
		auto status = pList->GetCheck(child);
		if (status)
		{
			auto data = pList->GetItemData(child);
			extraBuffer.push_back(data);
		}
	}
}

void CComboCheckBoxs::GetSeletionItemTexts(std::vector<CString> &selItemTexts)
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	auto root = pList->GetRootItem();
	int nIndex = 0;
	for (auto child = root; child != NULL; child = pList->GetNextSiblingItem(child), ++nIndex)
	{
		auto status = pList->GetCheck(child);
		if (status)
		{
			CString strItemText = pList->GetItemText(child);
			selItemTexts.push_back(strItemText);
		}
	}
}

void CComboCheckBoxs::SetSelectionItemText(CString strSelItemTexts)
{
	CWnd *pWnd = m_pSubWnd->GetWnd(&m_Panel);
	if (pWnd == NULL || !::IsWindow(pWnd->GetSafeHwnd()))
	{
		return;
	}
	CCheckBoxList *pList = (CCheckBoxList*)m_pSubWnd;
	auto root = pList->GetRootItem();
	int nIndex = 0;
	for (auto child = root; child != NULL; child = pList->GetNextSiblingItem(child), ++nIndex)
	{
		CString strChildText = pList->GetItemText(child);
		if (strChildText == strSelItemTexts)
		{
			pList->SetCheck(child, TRUE);
			break;
		}
	}
}
