#include "StdAfx.h"
#include "ListDataTime.h"

IMPLEMENT_DYNAMIC(CListDataTime, CDateTimeCtrl)

BEGIN_MESSAGE_MAP(CListDataTime, CDateTimeCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CListDataTime::CListDataTime()
{
	_bCandel = false;
}

CListDataTime::~CListDataTime()
{
}

bool CListDataTime::Create(CWnd* pParent, CRect& rect, LPCTSTR sInitText, bool bAdd)
{
	_bCandel = false;

	if (!_container.Create(rect, pParent))
		return false;

	// 不显示边框;
	CRect rc(-1, -1, rect.Width() + 1, rect.Height()+1);
	DWORD dwEditStyle = WS_CHILD | WS_CLIPCHILDREN | DTS_RIGHTALIGN | DTS_SHORTDATEFORMAT;
	if (bAdd)
	{
		dwEditStyle = WS_CHILD | WS_CLIPCHILDREN | DTS_RIGHTALIGN | DTS_SHORTDATEFORMAT | DTS_UPDOWN;
	}
	if (!CDateTimeCtrl::Create(dwEditStyle, rc, &_container, 0xffff))
		return false;

	// 设置初始时间
	if (bAdd)
	{
		COleDateTime datatime;
		int nYear = _ttoi(sInitText);
		datatime.SetDate(nYear, 1, 1);
		SetTime(datatime);
	}
	else
	{
		COleDateTime dataTime;
		dataTime.ParseDateTime(sInitText);
		SetTime(dataTime);
	}

	return true;
}

void CListDataTime::OnKillFocus(CWnd* pNewWnd)
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);

	if (_bCandel)
		return;

	CString strClassName;
	LPTSTR pClassBuf = strClassName.GetBuffer(MAX_PATH);
	::GetClassName(pNewWnd->GetSafeHwnd(), pClassBuf, MAX_PATH);
	strClassName.ReleaseBuffer();

	// 修复在XP下面选择日期时失去焦点问题
	if (!strClassName.CompareNoCase(L"SysMonthCal32"))
		return;

	CWnd* pWndParent = NULL;
	if (GetParent() != NULL)
	{
		pWndParent = GetParent()->GetParent();
	}

	// 发送修改事件;
	if (pWndParent != NULL)
	{
		CString strText;
		GetWindowText(strText);
		pWndParent->SendMessage(LIST_CTRL_END, (LPARAM)((LPCTSTR)strText), 0);
	}

	if (IsWindow(m_hWnd))
		_container.DestroyWindow();
}

void CListDataTime::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

	CDateTimeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CListDataTime::CancelEdit()
{
	_bCandel = true;

	CWnd* pParent = _container.GetParent();
	if (NULL != pParent)
	{
		_container.DestroyWindow();
		pParent->SetFocus();
	}
}
