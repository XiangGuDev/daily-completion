#include "stdafx.h"
#include "BCGPPropEx.h"
#include "BCGPPropListPro.h"

using namespace PropEx;

#define visualManager	CBCGPVisualManager::GetInstance ()

#define TEXT_MARGIN		3
#define TEXT_VMARGIN	2
#define GRID_TEXT_MARGIN	globalUtils.ScaleByDPI(TEXT_MARGIN)
#define GRID_TEXT_VMARGIN	globalUtils.ScaleByDPI(TEXT_VMARGIN)

CBCGPPropEx::CBCGPPropEx(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList):CBCGPProp(strGroupName, dwData, bIsValueList)
{
	_bHighlighted = FALSE;
	_pDataSrc = nullptr;
}

CBCGPPropEx::CBCGPPropEx(const CString& strName, const _variant_t& varValue,
	LPCTSTR lpszDescr, DWORD_PTR dwData,
	LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate,
	LPCTSTR lpszValidChars):CBCGPProp(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	_bHighlighted = FALSE;
	_pDataSrc = nullptr;
}

CBCGPPropEx::CBCGPPropEx(const CString& strName, UINT nID, const _variant_t& varValue,
	LPCTSTR lpszDescr, DWORD_PTR dwData,
	LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate,
	LPCTSTR lpszValidChars):CBCGPProp(strName, nID, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
	_bHighlighted = FALSE;
	_pDataSrc = nullptr;
}

CBCGPPropEx::~CBCGPPropEx()
{
	_bDelete = true;
	if (_nameFont.GetSafeHandle() != NULL)
	{
		_nameFont.DeleteObject();
	}

	if (_valFont.GetSafeHandle() != NULL)
	{
		_valFont.DeleteObject();
	}

	if (_underline.GetSafeHandle() != NULL)
	{		
		_underline.DeleteObject();
	}
	_pDataSrc = nullptr;
}

void CBCGPPropEx::OnDrawName(CDC * pDC, CRect rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT_VALID(m_pWndList);

	COLORREF clrTextOld = (COLORREF)-1;

	HFONT hOldFont = NULL;

	if (_prop.bNameFont && _nameFont.GetSafeHandle() != NULL)
	{
		hOldFont = (HFONT)::SelectObject(pDC->GetSafeHdc(), _nameFont.GetSafeHandle());
	}
	
	if (IsSelected() && (!m_pWndList->IsVSDotNetLook() || !IsGroup() || m_bIsValueList))
	{
		CRect rectFill = rect;
		rectFill.top++;

		if (m_bDrawMenuButton)
		{
			rectFill.right += m_rectMenuButton.Width();
		}

		if (m_bHasState)
		{
			rectFill.right += m_rectState.Width();
		}

		auto pWndList = dynamic_cast<CBCGPPropListPro*>(m_pWndList);
		COLORREF clrText = visualManager->OnFillPropertyListSelectedItem(pDC, this, m_pWndList, rectFill, pWndList->IsFocused());
		clrTextOld = pDC->SetTextColor(clrText);
	}

	// 属性组名称颜色修改
	if (m_pWndList->IsVSDotNetLook() && IsGroup() && !m_bIsValueList)
	{
		if (_prop.nameClr != (COLORREF)-1)
		{
			clrTextOld = pDC->SetTextColor(_prop.nameClr);
		}
		else
		{
			clrTextOld = pDC->SetTextColor(visualManager->GetPropListGroupTextColor(m_pWndList));
		}
	}

	// 属性项名称颜色修改
	if (m_pWndList->IsVSDotNetLook() && !IsGroup() && !m_bIsValueList)
	{
		if (_prop.nameClr != (COLORREF)-1)
		{
			clrTextOld = pDC->SetTextColor(_prop.nameClr);
		}
	}

	auto parentEx = dynamic_cast<CBCGPPropEx *>(m_pParent);
	if (parentEx != NULL && parentEx->m_bIsValueList)
	{
		rect.left += m_pWndList->GetRowHeight(TRUE);
	}

	rect.DeflateRect(TEXT_MARGIN, 0);

	int nNameAlign = IsGroup() ? DT_LEFT : m_pWndList->GetNameAlign();

	UINT uiDTFlags = nNameAlign | DT_NOPREFIX | DT_END_ELLIPSIS;
	if (GetRowsNumber() == 1)
	{
		uiDTFlags |= (DT_SINGLELINE | DT_VCENTER);
	}
	else
	{
		uiDTFlags |= DT_WORDBREAK;
		rect.DeflateRect(0, TEXT_MARGIN / 2);
	}

	int nRectHeight = rect.Height();
	int nTextHeight = pDC->DrawText(m_strName, rect, uiDTFlags);

	m_bNameIsTrancated = GetRowsNumber() > 1 ? (nTextHeight > nRectHeight) : pDC->GetTextExtent(m_strName).cx > rect.Width();

	if (IsSelected() && m_pWndList->IsVSDotNetLook() && IsGroup() && !m_bIsValueList)
	{
		CRect rectFocus = rect;
		rectFocus.top = rectFocus.CenterPoint().y - nTextHeight / 2;
		rectFocus.bottom = rectFocus.top + nTextHeight;
		rectFocus.right =
			min(rect.right, rectFocus.left + pDC->GetTextExtent(m_strName).cx);
		rectFocus.InflateRect(2, 0);

		COLORREF clrShadow = m_pWndList->DrawControlBarColors() ?
			globalData.clrBarShadow : globalData.clrBtnShadow;

		pDC->Draw3dRect(rectFocus, clrShadow, clrShadow);
	}

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor(clrTextOld);
	}

	if (hOldFont != NULL)
	{
		::SelectObject(pDC->GetSafeHdc(), hOldFont);
	}
}

void CBCGPPropEx::OnDrawValue(CDC * pDC, CRect rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT_VALID(m_pWndList);

	if (Property::eHyperlink == _prop.type)
	{
		CString strText = (LPCTSTR)(_bstr_t)m_varValue;
		DrawHyperLink(pDC, rect, strText);
	}
	else if (Property::eButton == _prop.type)
	{
		rect.DeflateRect(2, 2);

		COLORREF clrOldText = (COLORREF)-1;
		COLORREF clrText = globalData.clrBarText;

		if (m_clrTextValue != (COLORREF)-1)
		{
			clrText = m_clrTextValue;
			clrOldText = pDC->SetTextColor(clrText);;
		}
		
		CBCGPVisualManager::GetInstance()->OnDrawPushButton(pDC, rect, NULL, clrText);
		
		CString strText = (LPCTSTR)(_bstr_t)m_varValue;

		if (_bHighlighted)
		{
			pDC->FillSolidRect(rect, globalData.clrBtnLight);
		}	

		pDC->DrawText(strText, rect, _prop.nValueAlign | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

		if (clrOldText != (COLORREF)-1)
		{
			pDC->SetTextColor(clrOldText);
		}		
	}
	else if (Property::eCheckBox == _prop.type)
	{
		CBCGPVisualManager::GetInstance()->OnDrawCheckBox(pDC, rect, _bHighlighted, _prop.bCheckState, !_prop.bReadOnly);
	}
	else
	{
		if ((IsGroup() && !m_bIsValueList) || !HasValueField() || IsToggleControl())
		{
			return;
		}

		CFont* pOldFont = NULL;
		if (IsModified() && m_pWndList->IsMarkModifiedProperties())
		{
			pOldFont = pDC->SelectObject(&m_pWndList->GetBoldFont());
		}
		else if (_prop.bValueFont && _valFont.GetSafeHandle() != NULL)
		{
			pOldFont = pDC->SelectObject(&_valFont);
		}

		CString strVal = FormatProperty();

		rect.DeflateRect(TEXT_MARGIN, 0);

		COLORREF clrOldText = (COLORREF)-1;
		if (m_clrTextValue != (COLORREF)-1)
		{
			clrOldText = pDC->SetTextColor(m_clrTextValue);
		}
	
		UINT uiDTFlags = _prop.nValueAlign | DT_NOPREFIX | DT_END_ELLIPSIS;
		if (GetRowsNumber() == 1)
		{
			uiDTFlags |= (DT_SINGLELINE | DT_VCENTER);
		}
		else
		{
			uiDTFlags |= DT_WORDBREAK;
			rect.DeflateRect(0, TEXT_MARGIN / 2 + m_pWndList->GetRowPadding());
		}

		int nRectHeight = rect.Height();
		int nTextHeight = pDC->DrawText(strVal, rect, uiDTFlags);

		if (clrOldText != (COLORREF)-1)
		{
			pDC->SetTextColor(clrOldText);
		}

		m_bValueIsTrancated = GetRowsNumber() > 1 ? nTextHeight > nRectHeight : pDC->GetTextExtent(strVal).cx > rect.Width();

		if (pOldFont != NULL)
		{
			pDC->SelectObject(pOldFont);
		}
	}
}

CWnd * CBCGPPropEx::CreateInPlaceEdit(CRect rectEdit, BOOL & bDefaultFormat)
{
	if (Property::eHyperlink == _prop.type || Property::eButton == _prop.type || Property::eCheckBox == _prop.type)
		return nullptr;

	if (_prop.type == Property::eNULL)
		return __super::CreateInPlaceEdit(rectEdit, bDefaultFormat);

	if (NoInplaceEdit())
	{
		return NULL;
	}

	switch (m_varValue.vt)
	{
	case VT_BSTR:
	case VT_R4:
	case VT_R8:
	case VT_UI1:
	case VT_I2:
	case VT_INT:
	case VT_UINT:
	case VT_I4:
	case VT_I8:
	case VT_UI8:
	case VT_UI2:
	case VT_UI4:
	case VT_BOOL:
		break;

	default:
		if (!m_bIsValueList)
		{
			return NULL;
		}
	}

	CEdit* pWndEdit = NULL;

	if (!m_strEditMask.IsEmpty() || !m_strEditTempl.IsEmpty() ||
		!m_strValidChars.IsEmpty())
	{
		CBCGPMaskEdit* pWndEditMask = new CBCGPMaskEdit;
		pWndEditMask->EnableSetMaskedCharsOnly(FALSE);
		pWndEditMask->EnableGetMaskedCharsOnly(FALSE);

		if (!m_strEditMask.IsEmpty() && !m_strEditTempl.IsEmpty())
		{
			pWndEditMask->EnableMask(m_strEditMask, m_strEditTempl, _T(' '));
		}

		if (!m_strValidChars.IsEmpty())
		{
			pWndEditMask->SetValidChars(m_strValidChars);
		}

		pWndEdit = pWndEditMask;
	}
	else
	{
		switch (_prop.type)
		{
		case Property::eNULL:
			break;
		case Property::eEdit:
			pWndEdit = new CEdit();
			break;
		case Property::eDigitEdit:
			pWndEdit = new CValidEdit();
			break;
		case Property::eScaleEdit:
			pWndEdit = new CScaleEdit();
			break;
		case Property::eAngleEdit:
			pWndEdit = new CAngleEdit();
			break;
		default:
			pWndEdit = new CEdit();
			break;
		}

		CValidEdit *pValidEdit = DYNAMIC_DOWNCAST(CValidEdit, pWndEdit);
		if (pValidEdit)
		{
			pValidEdit->SetDotNum(_prop.digit.nDotNum);
			pValidEdit->SetDigitNum(_prop.digit.nDigitNum);
			pValidEdit->SetEditFlag(_prop.digit.nFlag);
			pValidEdit->SetMaxMinValue(_prop.digit.dMaxVal, _prop.digit.dMinVal, true);
		}
	}

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN;

	if (GetRowsNumber() == 1)
	{
		dwStyle |= ES_AUTOHSCROLL;
	}
	else
	{
		dwStyle |= ES_MULTILINE | ES_AUTOVSCROLL;
	}

	if (!m_bEnabled || !m_bAllowEdit)
	{
		dwStyle |= ES_READONLY;
	}

	if (pWndEdit)
	{
		pWndEdit->Create(dwStyle, rectEdit, m_pWndList, BCGPROPLIST_ID_INPLACE);
	}
	
	bDefaultFormat = TRUE;
	return pWndEdit;
}

void CBCGPPropEx::SetPropertyCfg(const PROPERTYITEM & item)
{
	_prop = item;

	if (_nameFont.GetSafeHandle() != NULL)
	{
		_nameFont.DeleteObject();
	}

	_nameFont.CreateFontIndirect(&_prop.nameFont);

	if (_valFont.GetSafeHandle() != NULL)
	{
		_valFont.DeleteObject();
	}

	_valFont.CreateFontIndirect(&_prop.valueFont);


	if (NULL != _underline.GetSafeHandle())
	{
		_underline.DeleteObject();
	}

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	CFont* pFont = m_pWndList->GetFont();
	pFont->GetLogFont(&lf);

	LONG lfWeightSaved = lf.lfWeight;
	lf.lfWeight = lfWeightSaved;
	lf.lfUnderline = TRUE;

	_underline.CreateFontIndirect(&lf);
}

BOOL CBCGPPropEx::OnEndEdit()
{
	if (_bDelete)
		return false;
	BOOL bEnd = __super::OnEndEdit();
	
	if (_pDataSrc)
	{
		CAny old = _pDataSrc->GetItemValue();
		_variant_t val = GetValue();
		CAny newVal;

		// 保证CAny数据类型
		EAnyType anyType = old.GetType();

		switch (anyType)
		{
		case ANY_BOOL:
		{
			bool bVal = val.boolVal;
			newVal = bVal;
			break;
		}
		case ANY_INT:
		{
			int nVal = val.intVal;
			newVal = nVal;
			break;
		}
		case ANY_DOUBLE:
		{
			double dVal = val.dblVal;
			newVal = dVal;
			break;
		}
		case ANY_STRING:
		{
			CString strVal = (CString)val.bstrVal;
			newVal = strVal;
			break;
		}
		default:
			newVal = (CString)val;
			break;
		}

		_pDataSrc->SetItemValue(newVal, old);
	}

	return bEnd;
}

void CBCGPPropEx::SetItemData(IPropertyObjectData * pDataSrc)
{
	_pDataSrc = pDataSrc;
}

BOOL CBCGPPropEx::OnClickValue(UINT uiMsg, CPoint point)
{
	if (Property::eButton == _prop.type)
	{
		if (_pDataSrc)
		{
			_pDataSrc->ButtonPressDown(_prop);
		}
	}
	else if (Property::eHyperlink == _prop.type)
	{
		if (_pDataSrc)
		{
			_pDataSrc->HyperlinkPressDown(_prop);
		}
	}
	else if (Property::eCheckBox == _prop.type)
	{
		if (_pDataSrc)
		{
			_pDataSrc->ClickRValItem(_prop);
		}		
	}
	else
	{
		return __super::OnClickValue(uiMsg, point);
	}

	return TRUE;
}

bool CBCGPPropEx::IsDelete()
{
	return _bDelete;
}

void CBCGPPropEx::DrawHyperLink(CDC* pDC, CRect rect, const CString &strText)
{
	CFont* pOldFont = pDC->SelectObject(&_underline);
	ASSERT(pOldFont != NULL);

	COLORREF clrTextOld = (COLORREF)-1;
	COLORREF textClr = _bHighlighted ? globalData.clrHotLinkText : globalData.clrHotText;

	if (m_clrTextValue != (COLORREF)-1)
	{
		textClr = m_clrTextValue;
	}

	clrTextOld = pDC->SetTextColor(textClr);

	CRect rectText = rect;

	pDC->DrawText(strText, rectText, _prop.nValueAlign | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

	pDC->SelectObject(pOldFont);

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor(clrTextOld);
	}	
}

CString CBCGPPropEx::FormatProperty()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	_variant_t& var = m_varValue;

	CString strVal;

	if (m_bIsValueList)
	{
		for (POSITION pos = m_lstSubItems.GetHeadPosition(); pos != NULL;)
		{
			CBCGPProp* pProp = m_lstSubItems.GetNext(pos);
			ASSERT_VALID(pProp);

			strVal += pProp->FormatProperty();

			if (pos != NULL)
			{
				strVal += m_pWndList->GetListDelimiter();
				strVal += _T(' ');
			}
		}

		return strVal;
	}

	switch (var.vt)
	{
	case VT_BSTR:
		strVal = (LPCTSTR)(_bstr_t)var;
		break;

	case VT_I2:
		strVal.Format(m_strFormatShort, (short)var);
		break;

	case VT_I4:
	case VT_INT:
		strVal.Format(m_strFormatLong, (long)var);
		break;

	case VT_UI1:
		if ((BYTE)var != 0)
		{
			strVal.Format(m_strFormatChar, (TCHAR)(BYTE)var);
		}
		break;

	case VT_UI2:
		strVal.Format(m_strFormatUShort, var.uiVal);
		break;

	case VT_UINT:
	case VT_UI4:
		strVal.Format(m_strFormatULong, var.ulVal);
		break;

#if _MSC_VER >= 1500
	case VT_I8:
	{
		TCHAR szBuffer[32];
		_i64tot_s((LONGLONG)var, szBuffer, 32, 10);
		strVal = szBuffer;
	}
	break;

	case VT_UI8:
	{
		TCHAR szBuffer[32];
		_ui64tot_s((ULONGLONG)var, szBuffer, 32, 10);
		strVal = szBuffer;
	}
	break;
#endif

	case VT_R4:
		strVal.Format(m_strFormatFloat, (float)var);
		break;

	case VT_R8:
	{
		strVal.Format(m_strFormatDouble, (double)var);
		int nDotPos = strVal.Find(L".");

		if (nDotPos != -1)
		{
			strVal = strVal.Left(nDotPos + 1 + _prop.digit.nDotNum);
		}

		break;
	}
	case VT_BOOL:
	{
		bool bVal = (bool)var;

		CBCGPPropListPro *pPropPro = dynamic_cast<CBCGPPropListPro *>(m_pWndList);
		if (pPropPro)
		{
			strVal = bVal ? pPropPro->True() : pPropPro->False();
		}
		else
		{
			strVal = bVal ? _T("True") : _T("False");
		}

		if (IsSwitch() && !m_bFillAccData)
		{
			CString strSwitchLabel = bVal ? m_strSwitchOn : m_strSwitchOff;
			if (!strSwitchLabel.IsEmpty())
			{
				strVal = strSwitchLabel;
			}
		}
	}
	break;

	default:
		// Unsupported type
		strVal = _T("*** error ***");
	}

	return strVal;
}
