#include "stdafx.h"
#include "BCGPComboBoxEx.h"

controls::CBCGPComboBoxEx::CBCGPComboBoxEx()
	:_nIndex(-1)
	,_nType(0)
{

}

controls::CBCGPComboBoxEx::~CBCGPComboBoxEx()
{

}

void controls::CBCGPComboBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	ASSERT_VALID(pDC);

	CRect rc = lpDIS->rcItem;
	int nIndexDC = pDC->SaveDC();
	BOOL bSelected = (lpDIS->itemState & ODS_SELECTED) == ODS_SELECTED;
	_nIndex = lpDIS->itemID;
	if (m_bDefaultPrintClient && GetDroppedState())
	{
		bSelected = TRUE;
	}

	BOOL bInGlass = m_bOnGlass;
	if ((lpDIS->itemState & ODS_COMBOBOXEDIT) == 0)
	{
		m_bOnGlass = FALSE;
	}

	COLORREF clrText = globalData.clrHotText;
	if (_nType == 1)
	{
		if (_nIndex != 0 && _nIndex != 4)
		{
			clrText = OnFillLbItem(pDC, (int)lpDIS->itemID, rc, FALSE, bSelected);
		}
	}
	else if(_nType == 2)
	{
		if (_nIndex != 0 )
		{
			clrText = OnFillLbItem(pDC, (int)lpDIS->itemID, rc, FALSE, bSelected);
		}
	}
	
	 
	if (clrText != (COLORREF)-1)
	{
		pDC->SetTextColor(clrText);
	}

	m_bOnGlass = bInGlass;

	pDC->SetBkMode(TRANSPARENT);

	int id = (int)lpDIS->itemID;
	if (id >= 0)
	{
		if (!m_bDefaultPrintClient)
		{
			int nIndent = GetItemIndent(id);
			if (GetExStyle() & WS_EX_RIGHT)
			{
				rc.right -= nIndent;
			}
			else
			{
				rc.left += nIndent;
			}
		}

		int nPadding = globalUtils.ScaleByDPI(2);

		if ((m_hImageList != NULL || m_ImageList.GetCount() > 0) && m_sizeImage != CSize(0, 0))
		{
			CRect rectImage = rc;
			rectImage.top += max(0, (rc.Height() - m_sizeImage.cy) / 2);
			rectImage.bottom = rectImage.top + m_sizeImage.cy;

			if (GetExStyle() & WS_EX_RIGHT)
			{
				rectImage.right -= nPadding;
				rectImage.left = rectImage.right - m_sizeImage.cx;
			}
			else
			{
				rectImage.left += nPadding;
				rectImage.right = rectImage.left + m_sizeImage.cx;
			}

			int nIcon = GetItemImage(id);
			if (nIcon >= 0)
			{
				if (m_hImageList != NULL)
				{
					CImageList* pImageList = CImageList::FromHandle(m_hImageList);
					pImageList->Draw(pDC, nIcon, rectImage.TopLeft(), ILD_TRANSPARENT);
				}
				else if (m_ImageList.GetCount() > 0)
				{
					m_ImageList.DrawEx(pDC, rectImage, nIcon);
				}
			}

			if (GetExStyle() & WS_EX_RIGHT)
			{
				rc.right -= m_sizeImage.cx + nPadding;
			}
			else
			{
				rc.left += m_sizeImage.cx + nPadding;
			}
		}

		CString strText;
		GetLBText(id, strText);

		rc.DeflateRect(nPadding, 0);

		UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX;

		if (GetExStyle() & WS_EX_RIGHT)
		{
			nFormat |= DT_RIGHT;
		}
		else
		{
			nFormat |= DT_LEFT;
		}

		if (m_bOnGlass)
		{
			CBCGPVisualManager::GetInstance()->DrawTextOnGlass(pDC, strText, rc, nFormat, 0, clrText);
		}
		else
		{
			pDC->DrawText(strText, rc, nFormat);
		}
	}

	pDC->RestoreDC(nIndexDC);
}

LRESULT controls::CBCGPComboBoxEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_COMMAND)
	{
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			if (_nType == 1)
			{
				if (_nIndex == 0 || _nIndex == 4)
				{
					return 0;
				}
			}

			if (_nType == 2)
			{
				if (_nIndex == 0)
				{
					return 0;
				}
			}

		}
	}
	LRESULT lRes = CComboBox::WindowProc(message, wParam, lParam);
	if (message == CB_SETCURSEL && m_bVisualManagerStyle)

	{
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}

	return lRes;
}

void controls::CBCGPComboBoxEx::SetEnable(int nRowIndex)
{
	_nEnableIndex += (2 ^ nRowIndex);
}

void controls::CBCGPComboBoxEx::SetEnableType(int nType)
{
	_nType = nType;
}