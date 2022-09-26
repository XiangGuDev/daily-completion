#include "stdafx.h"
#include "BCGPGridItemEx.h"

IMPLEMENT_DYNAMIC(CBCGPSpinButtonCtrlEx, CBCGPSpinButtonCtrl)
BEGIN_MESSAGE_MAP(CBCGPSpinButtonCtrlEx, CBCGPSpinButtonCtrl)
	//{{AFX_MSG_MAP(CBCGPSpinButtonCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CBCGPSpinButtonCtrlEx::CBCGPSpinButtonCtrlEx()
	:CBCGPSpinButtonCtrl()
{

}

CBCGPSpinButtonCtrlEx::~CBCGPSpinButtonCtrlEx()
{

}

void CBCGPSpinButtonCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bVisualManagerStyle)
	{
		CRect rect;
		GetClientRect(rect);

		CRect rectUp = rect;
		CRect rectDown = rect;

		if ((GetStyle() & UDS_HORZ) == UDS_HORZ)
		{
			rectDown.right = rect.CenterPoint().x;
			rectUp.left = rectDown.right;
		}
		else
		{
			rectUp.bottom = rect.CenterPoint().y;
			rectDown.top = rectUp.bottom;
		}

		m_bIsButtonPressedUp = rectUp.PtInRect(point);
		m_bIsButtonPressedDown = rectDown.PtInRect(point);
	}

	CBCGPGridSpinEdit* pSpinEdit = DYNAMIC_DOWNCAST(CBCGPGridSpinEdit, GetBuddy());
	pSpinEdit->RefBySpinButtonDown(m_bIsButtonPressedUp);
}

IMPLEMENT_DYNAMIC(CBCGPGridSpinEdit, CBCGPEdit)

BEGIN_MESSAGE_MAP(CBCGPGridSpinEdit, CBCGPEdit)
ON_CONTROL_REFLECT_EX(EN_CHANGE, OnChange)
END_MESSAGE_MAP()

CBCGPGridSpinEdit::CBCGPGridSpinEdit()
	:CBCGPEdit()
{

}

CBCGPGridSpinEdit::~CBCGPGridSpinEdit()
{

}

IMPLEMENT_SERIAL(CBCGPGridSpinItem, CBCGPGridItem, VERSIONABLE_SCHEMA | 1)


void CBCGPGridSpinEdit::RefBySpinButtonDown(bool bIsUp)
{
	CString strText;
	GetWindowText(strText);

	int pos = 0;
	int nlength = strText.GetLength();
	for (int i = 0; i < nlength; i++)
	{
		if ((strText[i] >= '0') && (strText[i] <= '9'))
		{
			pos++;
		}
		else
			break;
	}

	CString strNum = strText.Left(pos);
	CString strUnit = strText.Right(nlength - pos);
	int nNum = _ttoi(strNum);
	nNum = bIsUp ? nNum + 1 : nNum - 1;
	if (nNum < 0)
		nNum = 0;

	strText.Format(_T("%d%s"), nNum, strUnit);
	SetWindowText(strText);

	SetSel(0, -1, FALSE);
}

BOOL CBCGPGridSpinEdit::OnChange()
{
	return __super::OnChange();
}

CBCGPGridSpinItem::CBCGPGridSpinItem()
	:CBCGPGridItem()
{
	EnableSpinControl(true, 0, 1000);
}

CBCGPGridSpinItem::CBCGPGridSpinItem(const _variant_t& varValue,
	DWORD_PTR dwData /*= 0*/, LPCTSTR lpszEditMask /*= NULL*/,
	LPCTSTR lpszEditTemplate /*= NULL*/, LPCTSTR lpszValidChars /*= NULL*/)
	: CBCGPGridItem(varValue, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	EnableSpinControl(true, 0, 1000);
}

CBCGPGridSpinItem::~CBCGPGridSpinItem()
{

}

CString CBCGPGridSpinItem::FormatItem()
{
	return m_varValue;
}

CEdit* CBCGPGridSpinItem::NewInPlaceEdit()
{
	return new CBCGPGridSpinEdit;
}

CSpinButtonCtrl* CBCGPGridSpinItem::CreateSpinControl(CRect rectSpin)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->GetOwnerList();
	ASSERT_VALID(pWndList);

	CBCGPSpinButtonCtrlEx* pWndSpin = new CBCGPSpinButtonCtrlEx;

	if (!pWndSpin->Create(
		WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS | UDS_SETBUDDYINT | UDS_NOTHOUSANDS,
		rectSpin, pWndList, 1))
	{
		return NULL;
	}

	pWndSpin->SetBuddy(m_pWndInPlace);
	pWndSpin->SetWindowPos(NULL, rectSpin.left-2, rectSpin.top, rectSpin.Width()+2, rectSpin.Height(), SWP_NOACTIVATE | SWP_NOZORDER);

	if (pWndList->IsVisualManagerStyle())
	{
		pWndSpin->m_bVisualManagerStyle = TRUE;
	}

	if (m_nMinValue != 0 || m_nMaxValue != 0)
	{
		pWndSpin->SetRange32(m_nMinValue, m_nMaxValue);
	}

	return pWndSpin;
}
//
//void CBCGPGridSpinItem::DoClickButton(CPoint point)
//{
//	ASSERT_VALID(this);
//
//	CString strPrevVal = FormatItem();
//
//	CBCGPGridCtrl* pGrid = GetOwnerList();
//	//CConditionalWaitCursor wait(pGrid != NULL ? pGrid->IsWaitCursorEnabled() : TRUE);
//	CBCGPGridItemID curGridItemId = pGrid->GetCurSelItemID();
//
//	OnClickButton(point);
//
//
	/*CBCGPGridItem* pItem = pGrid->GetRow(curGridItemId.m_nRow)->GetItem(curGridItemId.m_nColumn);
	if (strPrevVal != pItem->FormatItem())
	{
		SetItemChanged();*/
//	}
//}

void CBCGPGridSpinItem::EnableSpinControl(BOOL bEnable /*= TRUE*/, int nMin /*= 0*/, int nMax /*= 0*/)
{
	ASSERT_VALID(this);
	m_nMinValue = nMin;
	m_nMaxValue = nMax;
	m_dwFlags |= BCGP_GRID_ITEM_HAS_SPIN;
}
