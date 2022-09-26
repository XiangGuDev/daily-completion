#include "StdAfx.h"
#include "EditXPver.h"

namespace ControlUI
{
	IMPLEMENT_DYNAMIC(CEditXpVer, CEdit)
	BEGIN_MESSAGE_MAP(CEditXpVer, CEdit)
		//WM_CTLCOLOR的消息反射
		ON_WM_CTLCOLOR_REFLECT()
		ON_WM_KILLFOCUS()
		ON_WM_SETFOCUS()
	END_MESSAGE_MAP()

	void CEditXpVer::SetBannerText(const CString& str)
	{
		if (GetStyle() & ES_PASSWORD)
		{
			//若编辑框为密码框则获取密码字符并去除密码输入模式
			_chPass = GetPasswordChar();
			SetPasswordChar(0);
		}
		//获取提示文字并将默认文字设为提示文字
		_strBannerText = str;
		CEdit::SetWindowText(_strBannerText);
	}

	HBRUSH CEditXpVer::CtlColor(CDC* pDC, UINT nCtlColor)
	{
		//刷新前更改文本颜色
		pDC->SetTextColor(m_crText);

		//返回画刷,用来绘制整个控件背景
		return GetSysColorBrush(COLOR_WINDOW);
	}

	void CEditXpVer::OnKillFocus(CWnd* pNewWnd)
	{
		CEdit::OnKillFocus(pNewWnd);
		WhenKillFocus();
	}

	void CEditXpVer::OnSetFocus(CWnd* pOldWnd)
	{
		CEdit::OnSetFocus(pOldWnd);
		WhenOnFocus();
	}

	CString CEditXpVer::GetBannerText()
	{
		return _strBannerText;
	}

	CEditXpVer::CEditXpVer()
	{
		//初始化提示文字颜色以及提示文字内容
		m_crText = GetSysColor(COLOR_GRAYTEXT);
		_strBannerText = L"";
		_chPass = 0;
	}

	CEditXpVer::~CEditXpVer()
	{

	}

	void CEditXpVer::SetWindowText(LPCTSTR lpszString)
	{
		CEdit::SetWindowText(lpszString);
		bool bFocused = (::GetFocus() == GetSafeHwnd());
		bool bStrEmpty = (_tcslen(lpszString) == 0);
		if (!bStrEmpty)
		{
			m_crText = GetSysColor(COLOR_WINDOWTEXT);
			Invalidate(TRUE);
		}
		else
		{
			bFocused ? WhenOnFocus() : WhenKillFocus();
		}

	}

	void CEditXpVer::GetInputText(CString& strTemp)
	{
		GetWindowText(strTemp);
		if (strTemp == _strBannerText)
			strTemp = L"";
	}

	void CEditXpVer::WhenKillFocus()
	{
		CString strTemp;
		GetWindowText(strTemp);
		if (strTemp.IsEmpty())
		{
			//若编辑框文字为空则显示提示文字并去除密码框
			SetPasswordChar(0);
			CEdit::SetWindowText(_strBannerText);
			m_crText = GetSysColor(COLOR_GRAYTEXT);
			Invalidate(TRUE);
		}
	}

	void CEditXpVer::WhenOnFocus()
	{
		//获取编辑框文字
		CString strTemp;
		GetWindowText(strTemp);
		if (strTemp == _strBannerText)
		{
			//若编辑框文字为提示文字则在选中时去除文字并修改字体颜色为默认
			SetPasswordChar(_chPass);
			CEdit::SetWindowText(L"");
			m_crText = GetSysColor(COLOR_WINDOWTEXT);
			Invalidate(TRUE);
		}
	}
}