#include "stdafx.h"
#include "NumEdit.h"

BEGIN_MESSAGE_MAP(CNumEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

CNumEdit::CNumEdit() :
	m_decimalNum(2),
	m_bIsPositiveNumber(false),
	m_bIsCanZero(true),
	m_bIsInteger(false),
	m_dMaxNum(DBL_MAX)
{
}


CNumEdit::~CNumEdit()
{
}

int CNumEdit::SetDecimalNum(unsigned int iNum)
{
	m_decimalNum = iNum;
	m_bIsInteger = false;
	return 0;
}

int CNumEdit::SetIsPositiveNumber(bool bIsOrNot)
{
	m_bIsPositiveNumber = bIsOrNot;

	return 0;
}

void CNumEdit::SetIsCanZero(bool bIsOrNot)
{
	m_bIsCanZero = bIsOrNot;
}

int CNumEdit::SetIsInteger(bool bIsOrNot)
{
	m_bIsInteger = bIsOrNot;

	return 0;
}

void CNumEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);

	str = TrimString(str);
	SetWindowText(str);

	if (!m_bIsCanZero && str == L"0" && m_bIsPositiveNumber)
	{
		SetWindowText(L"");
		SetCueBanner(L"请输入正数");
		// SetFocus(); // 暂时不需要获取焦点
	}
}

void CNumEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	return;
}

CString CNumEdit::TrimString(const CString& strText)
{
	CString strTmp = strText;
	size_t nCount = strTmp.GetLength(), nSt = 0, nEd = 0;
	for (size_t i = 0; i + 1 < nCount; ++i)
	{
		if (strTmp.GetAt(i) == L'.')
		{
			strTmp.Insert(0, L"0");
			break;
		}

		if (strTmp.GetAt(i) != L'0' || strTmp.GetAt(i + 1) == L'.')
		{
			break;
		}

		++nSt;
	}

	bool bEd = (strTmp.Find(L".") >= 0);
	nCount = strTmp.GetLength();
	nEd = nCount;
	for (size_t i = 0; i < nCount && bEd; ++i)
	{
		if (strTmp.GetAt(nCount - i - 1) == L'0')
		{
			--nEd;
		}
		else if (strTmp.GetAt(nCount - i - 1) == L'.')
		{
			--nEd;
			break;
		}
		else
		{
			break;
		}
	}

	return strTmp.Mid(nSt, nEd - nSt);
}

void CNumEdit::SetMaxNum(double dNum, const CString& strTitle, const CString& strText)
{
	m_dMaxNum = dNum;
	m_strTipTitle = strTitle;
	m_strTipText = strText;
}

void CNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	POINT cp = GetCaretPos(); // 获取光标相对于控件的位置
	int pos = CharFromPos(cp); // 获取光标位置

	CString strOrigin;
	GetWindowText(strOrigin);

	if (nChar == 0x08) // 允许Backspace
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (strOrigin == L"0" && nChar >= '0'&& nChar <= '9') // 禁止00...、01...等类似情况
	{
		SetWindowText(L"");
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == '.')
	{
		if (strOrigin == "" || pos < (strOrigin.GetLength() - 2) || m_bIsInteger) // 过滤小数点在最开始、光标在文本中时应该在最后两位才能有小数点
		{
		}
		else if (-1 == strOrigin.Find('.')) // 没有小数点
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if ((nChar >= '0'&&nChar <= '9'))
	{
		if (-1 != strOrigin.Find('.') && (strOrigin.GetLength() - strOrigin.Find('.') > (int)m_decimalNum) && (pos > strOrigin.Find('.'))) // 只允许小数点后两位
		{
		}
		//else if (pos == 0 && strOrigin != L"" && nChar == '0') // 禁止在已有数字前添0
		//{
		//}
		else
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if ('-' == nChar && pos == 0 && (-1 == strOrigin.Find('-')) && (!m_bIsPositiveNumber)) // 减号只能在最开始且文本中没有已存在减号
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}

	CString strRes;
	GetWindowText(strRes);
	double dNum = _wtof(strRes);
	if (dNum > m_dMaxNum)
	{
		ShowBalloonTip(m_strTipTitle, m_strTipText);
		SetWindowText(strOrigin);
		return;
	}
}
