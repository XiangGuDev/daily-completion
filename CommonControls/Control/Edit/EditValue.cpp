#include "stdafx.h"
#include "EditValue.h"

BEGIN_MESSAGE_MAP(CEditValue, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

CEditValue::CEditValue()
{
}


CEditValue::~CEditValue()
{
}

CString CEditValue::GetOldValue()
{
	return m_strOldValue;
}

void CEditValue::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString str;
	GetWindowText(str);

	if (str != L"")
	{
		m_strOldValue = str;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
