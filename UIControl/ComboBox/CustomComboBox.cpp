#include "StdAfx.h"
#include "CustomComboBox.h"

namespace ControlUI
{
	BEGIN_MESSAGE_MAP(CCustomComboBox, CAutoWidthComboBox)
		ON_CONTROL_REFLECT(CBN_SELCHANGE, &CCustomComboBox::OnComboSelected)
		ON_WM_SIZE()
	END_MESSAGE_MAP()

	IMPLEMENT_DYNAMIC(CCustomComboBox, CAutoWidthComboBox)

		CCustomComboBox::CCustomComboBox()
		:_pParent(NULL)
	{

	}

	CCustomComboBox::~CCustomComboBox()
	{
		// Îö¹¹Ê±ÊÍ·Å×Ö·û´®
		std::vector<TCHAR*>::iterator iterEnd = _arrStrData.end();
		for (std::vector<TCHAR*>::iterator iter = _arrStrData.begin(); iter != iterEnd; ++iter)
		{
			delete[](*iter);
		}
	}

	int CCustomComboBox::SetItemData(int nIndex, const TCHAR* strData)
	{
		// ¿½±´×Ö·û´®,·ÀÖ¹×Ö·û´®ÔÚÍâ²¿Îö¹¹´íÎó
		size_t nLength = _tcslen(strData) + 1;
		TCHAR* str = new TCHAR[nLength];
		_tcscpy_s(str, nLength, strData);
		_arrStrData.push_back(str);

		return CComboBox::SetItemData(nIndex, (DWORD_PTR)str);
	}

	void CCustomComboBox::SetMsgParent(CWnd* pParent)
	{
		_pParent = pParent;
	}

	void CCustomComboBox::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
	{
		__super::MoveWindow(lpRect, bRepaint);
		ComboxAutoWidth(*this);
	}

	void CCustomComboBox::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /*= TRUE*/)
	{
		__super::MoveWindow(x, y, nWidth, nHeight, bRepaint);
		ComboxAutoWidth(*this);
	}

	void CCustomComboBox::OnComboSelected()
	{
		CWnd* pParent = GetParent();
		if (NULL != _pParent)
			pParent = _pParent;
		if (pParent == NULL)
			return;

		pParent->SendMessage(CBN_SELCHANGE, (WPARAM)m_hWnd, 0);
	}
}