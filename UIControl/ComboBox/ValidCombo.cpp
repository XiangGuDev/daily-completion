#include "stdafx.h"
#include "ValidCombo.h"

namespace ControlUI
{
	IMPLEMENT_DYNAMIC(CValidCombo, CAutoWidthComboBox)
	BEGIN_MESSAGE_MAP(CValidCombo, CAutoWidthComboBox)
		ON_WM_CTLCOLOR()
		ON_WM_DESTROY()
	END_MESSAGE_MAP()

	CValidCombo::CValidCombo()
	{
		_nFlag = 0;
		_nDotNum = 0;
	}

	void CValidCombo::SetEditFlag(int nFlag)
	{
		_nFlag = nFlag;
	}

	void CValidCombo::SetDotNum(int nDotNum)
	{
		_nDotNum = nDotNum;
	}

	HBRUSH CValidCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
	{
		if (nCtlColor == CTLCOLOR_EDIT)
		{
			//[ASCII 160][ASCII 160][ASCII 160]Edit control
			if (m_edit.GetSafeHwnd() == NULL)
			{
				m_edit.SetEditFlag(_nFlag);
				m_edit.SetDotNum(_nDotNum);
				m_edit.SubclassWindow(pWnd->GetSafeHwnd());
			}
		}
		else if (nCtlColor == CTLCOLOR_LISTBOX)
		{
			//ListBox control
			if (m_listbox.GetSafeHwnd() == NULL)
				m_listbox.SubclassWindow(pWnd->GetSafeHwnd());
			else
				Edit_HideBalloonTip(m_edit.m_hWnd);
		}
		HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
		return hbr;
	}

	void CValidCombo::OnDestroy()
	{
		if (m_edit.GetSafeHwnd() != NULL)
			m_edit.UnsubclassWindow();
		if (m_listbox.GetSafeHwnd() != NULL)
			m_listbox.UnsubclassWindow();
		CComboBox::OnDestroy();
	}

}
