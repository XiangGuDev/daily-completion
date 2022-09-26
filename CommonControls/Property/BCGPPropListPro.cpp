#include "stdafx.h"
#include "BCGPPropListPro.h"
#include "BCGPPropEx.h"
#define STRETCH_DELTA	2

using namespace PropEx;
volatile bool CBCGPPropListPro::_bDelete = false;

IMPLEMENT_DYNAMIC(CBCGPPropListPro, CBCGPPropList)

BEGIN_MESSAGE_MAP(CBCGPPropListPro, CBCGPPropList)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CBCGPPropListPro::CBCGPPropListPro():CBCGPPropList()
{
	_bDelete = false;
}

CBCGPPropListPro::~CBCGPPropListPro()
{
	_bDelete = true;
}

void CBCGPPropListPro::RemoveAllProperty()
{
	m_lstProps.RemoveAll();
	m_lstTerminalProps.RemoveAll();
}

BOOL CBCGPPropListPro::IsFocused()
{
	return m_bFocused;
}

void CBCGPPropListPro::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (_bDelete)
		return;

	CBCGPProp::ClickArea clickArea;
	CBCGPProp* pHit = HitTest(point, &clickArea);
	CBCGPPropEx *pHitEx = dynamic_cast<CBCGPPropEx *>(pHit);
	
	if (pHitEx == nullptr)
		return;

	if (clickArea == CBCGPProp::ClickValue && pHitEx)
	{
		if (pHitEx == nullptr || pHitEx->m_strName.IsEmpty())
			return;
		CBCGPWnd::OnLButtonDown(nFlags, point);
		SetFocus();

		CRect rectClient;
		GetClientRect(rectClient);

		if (Property::eButton == pHitEx->_prop.type || Property::eHyperlink == pHitEx->_prop.type)
		{
			if (!pHitEx->_prop.bReadOnly)
			{
				pHitEx->_bHighlighted = TRUE;
				pHitEx->OnClickValue(WM_LBUTTONDOWN, point);
				//pHitEx->Redraw();
			}
		}
		else if (Property::eCheckBox == pHitEx->_prop.type)
		{
			if (!pHitEx->_prop.bReadOnly)
			{
				CRect hitRt = pHitEx->GetRect();
				hitRt.left = m_rectList.left + m_nLeftColumnWidth;
				CPoint centerPt = hitRt.CenterPoint();
				CRect checkRt(centerPt.x - 7, centerPt.y - 7, centerPt.x + 7, centerPt.y + 7);

				if (checkRt.PtInRect(point))
				{
					pHitEx->_bHighlighted = TRUE;
					pHitEx->OnClickValue(WM_LBUTTONDOWN, point);
					//pHitEx->Redraw();
				}
			}
		}
		else if (Property::eNULL == pHitEx->_prop.type)
		{
			return;
		}
		else
		{
			if (pHitEx == nullptr || pHitEx->m_strName.IsEmpty())
				return;
			__super::OnLButtonDown(nFlags, point);
		}
	}
	else
	{
		__super::OnLButtonDown(nFlags, point);
	}
}

void CBCGPPropListPro::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (_bDelete)
		return;

	CBCGPProp::ClickArea clickArea;
	CBCGPProp* pHit = HitTest(point, &clickArea);
	CBCGPPropEx *pHitEx = dynamic_cast<CBCGPPropEx *>(pHit);

	if (pHitEx)
	{
		pHitEx->_bHighlighted = FALSE;
		pHitEx->Redraw();
	}
	
	__super::OnLButtonUp(nFlags, point);
}
