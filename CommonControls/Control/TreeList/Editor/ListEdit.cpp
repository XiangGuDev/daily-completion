#include "StdAfx.h"
#include "ListEdit.h"

IMPLEMENT_DYNAMIC(CListEdit, CValidEdit)

CListEdit::CListEdit()
{
	_bCancel = false;
}

CListEdit::~CListEdit()
{
}

BEGIN_MESSAGE_MAP(CListEdit, CValidEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

bool CListEdit::Create(CWnd* pParent, CRect& rect, LPCTSTR sInitText, bool bWordWrap)
{
	bool bSucceed = false;
	_bCancel = false;

	UINT nStyle = WS_CHILD | WS_CLIPCHILDREN | ES_MULTILINE | ES_AUTOHSCROLL | ES_LEFT;
	if (bWordWrap)
	{
		nStyle |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL;
		nStyle &= ~(ES_AUTOHSCROLL | WS_HSCROLL);
	}

	if (CValidEdit::Create(nStyle, rect, pParent, 0))
	{
		bSucceed = true;

		// 设置显示默认文字;
		SetWindowText(sInitText);

		// 设置全选;
		SetSel(0, -1);
	}

	return bSucceed;
}

void CListEdit::OnKillFocus(CWnd* pNewWnd)
{
	CValidEdit::OnKillFocus(pNewWnd);

	if (_bCancel)
		return;

	EndEdit();
}

void CListEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB || (nChar == VK_RETURN 
		&& !(GetStyle()&ES_WANTRETURN)))
	{
		GetParent()->SetFocus();
	}
	if (nChar == VK_ESCAPE)
	{
		CancelEdit();
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CListEdit::CancelEdit()
{
	_bCancel = TRUE;

	CWnd* pParent = GetParent();
	if (IsWindow(m_hWnd))
		DestroyWindow();

	if (NULL != pParent)
		pParent->SetFocus();
}

void CListEdit::EndEdit()
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