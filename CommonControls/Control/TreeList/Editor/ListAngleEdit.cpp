#include "StdAfx.h"
#include "ListAngleEdit.h"

IMPLEMENT_DYNAMIC(CListAngleEdit, CAngleEdit)

CListAngleEdit::CListAngleEdit()
{
	_bCancel = false;
}

CListAngleEdit::~CListAngleEdit()
{
}

BEGIN_MESSAGE_MAP(CListAngleEdit, CAngleEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

bool CListAngleEdit::Create(CWnd* pParent, CRect& rect, LPCTSTR sInitText)
{
	_bCancel = false;
	bool bSucceed = false;

	if (CAngleEdit::Create(WS_CHILD | ES_AUTOHSCROLL | ES_MULTILINE | ES_LEFT, rect, pParent, 0))
	{
		bSucceed = true;

		// 设置字符串角度，确保格式正确
		SetWindowText(sInitText);
	}

	return bSucceed;
}

void CListAngleEdit::OnKillFocus(CWnd* pNewWnd)
{
	CAngleEdit::OnKillFocus(pNewWnd);

	if (_bCancel)
		return;

	EndEdit();
}

void CListAngleEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{
		GetParent()->SetFocus();
	}
	if (nChar == VK_ESCAPE)
	{
		CancelEdit();
	}

	CAngleEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CListAngleEdit::CancelEdit()
{
	_bCancel = true;

	CWnd* pParent = GetParent();
	if (IsWindow(m_hWnd))
		DestroyWindow();

	if (NULL != pParent)
		pParent->SetFocus();
}

void CListAngleEdit::EndEdit()
{
	CString strText;
	static BOOL bAlreadyEnding = FALSE;

	if (bAlreadyEnding)
		return;

	bAlreadyEnding = TRUE;
	
	GetWindowText(strText);

	CWnd* pOwner = GetParent();
	if (pOwner->GetSafeHwnd())
		pOwner->SendMessage(LIST_CTRL_END, (WPARAM)LPCTSTR(strText), 0);

	if (IsWindow(m_hWnd))
		DestroyWindow();

	bAlreadyEnding = FALSE;
}