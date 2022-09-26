#include "StdAfx.h"
#include "AutoWidthComboBox.h"


void CAutoWidthComboBox::ComboxAutoWidth(CComboBox &combox)
{
	if (NULL == combox.m_hWnd)
		return;

	CFont* font = combox.GetFont();
	CDC* dc = combox.GetDC();
	CFont* oldFont = dc->SelectObject(font);

	int nTextHeight = 0;
	int nDroppedWidth = 0;
	int nCount = combox.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString strDropContent;
		combox.GetLBText(i, strDropContent);
		CSize size = dc->GetTextExtent(strDropContent, strDropContent.GetLength());
		if (size.cx > nDroppedWidth)
			nDroppedWidth = size.cx;

		nTextHeight += size.cy;
	}

	dc->SelectObject(oldFont);

	CRect rect;
	combox.GetDroppedControlRect(rect);
	if (nTextHeight <= rect.Height())
	{
		nDroppedWidth += ::GetSystemMetrics(SM_CXVSCROLL) + 2 * ::GetSystemMetrics(SM_CXEDGE);
	}
	combox.SetDroppedWidth(nDroppedWidth + 10);
	combox.SetEditSel(-1, 0);
	combox.ReleaseDC(dc);
}
void CAutoWidthComboBox::ComboxAutoResize(CComboBox &combox)
{
	COMBOBOXINFO cbi;
	cbi.cbSize = sizeof(COMBOBOXINFO);
	combox.GetComboBoxInfo(&cbi);
	// 获得组合框内的列表控件
	CListBox *pList = (CListBox*)CWnd::FromHandle(cbi.hwndList);
	
	if (pList == NULL)	// 中途出错，返回
		return;

	// 获得列表控件当前使用的字体
	CFont* font = pList->GetFont();
	CDC* dc = pList->GetDC();
	CFont* oldFont = dc->SelectObject(font);
	int height = 0, width = 0;
	int nCount = pList->GetCount();

	for (int i = 0; i < nCount; i++)
	{
		CString strItem;
		pList->GetText(i, strItem);
		CSize size = dc->GetTextExtent(strItem, strItem.GetLength());
		width = max(width, size.cx);
		height += size.cy;
	}

	dc->SelectObject(oldFont);
	pList->ReleaseDC(dc);
	
	CRect rect;
	pList->GetWindowRect(rect);

	if (height >= rect.Height())		// 滚动条处理
		width += ::GetSystemMetrics(SM_CXVSCROLL) + 2 * ::GetSystemMetrics(SM_CXEDGE);

	CRect rc, r;
	combox.GetDroppedControlRect(r);	// 下拉区域
	pList->GetWindowRect(rc);
	rc.right = rc.left + max(width + 10, r.Width());
	rc.bottom = rc.top + max(min(height, 400), 50); 
	combox.SetDroppedWidth(rc.Width());
	pList->MoveWindow(rc);
}

int CAutoWidthComboBox::AddString(LPCTSTR lpszString, bool bAutoWidth)
{
	int nRetIndex = __super::AddString(lpszString);
	if (bAutoWidth)
		ComboxAutoWidth(*this);

	return nRetIndex;
}

int CAutoWidthComboBox::InsertString(int nIndex, LPCTSTR lpszString)
{
	int nRetIndex = __super::InsertString(nIndex, lpszString);
	ComboxAutoWidth(*this);
	return nRetIndex;
}

int CAutoWidthComboBox::DeleteString(UINT nIndex)
{
	int nRetIndex = __super::DeleteString(nIndex);
	ComboxAutoWidth(*this);
	return nRetIndex;
}

void CAutoWidthComboBox::ResetContent()
{
	__super::ResetContent();
	ComboxAutoWidth(*this);
}

void CAutoWidthComboBox::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
	__super::MoveWindow(lpRect, bRepaint);
	ComboxAutoWidth(*this);
}

void CAutoWidthComboBox::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
{
	__super::MoveWindow(x, y, nWidth, nHeight, bRepaint);
	ComboxAutoWidth(*this);
}
