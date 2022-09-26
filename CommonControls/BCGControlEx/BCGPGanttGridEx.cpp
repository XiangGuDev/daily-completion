#include "stdafx.h"
#include "BCGPGanttGridEx.h"
#include "BCGPGridItemEx.h"

#include "Imm.h"
//////////////////////////////////////////////////////////////////////
//       Implementation of the CBCGPGanttGridRowEx class
//////////////////////////////////////////////////////////////////////
#pragma comment(lib, "Imm32.Lib")

CBCGPGanttFirstItem::CBCGPGanttFirstItem()
	:CBCGPGridItem(_variant_t((LPCTSTR)_T("")), 0)
{
	//BCGP_GRID_ITEM_MULTILINE
	m_dwFlags = m_dwFlags & BCGP_GRID_ITEM_WORDWRAP;
	m_dwFlags = m_dwFlags & BCGP_GRID_ITEM_MULTILINE;
}

CBCGPGanttFirstItem::CBCGPGanttFirstItem(const _variant_t& varValue, DWORD_PTR dwData /*= 0*/, 
	LPCTSTR lpszEditMask /*= NULL*/, LPCTSTR lpszEditTemplate /*= NULL*/, LPCTSTR lpszValidChars /*= NULL*/)
	: CBCGPGridItem(varValue, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{

}

CBCGPGanttFirstItem::~CBCGPGanttFirstItem()
{

}

void CBCGPGanttFirstItem::SetRect(CRect rect)
{
	m_Rect = rect;
}

void CBCGPGanttFirstItem::SetLeftRect(CRect rect)
{
	_leftRect = rect;
}

CRect CBCGPGanttFirstItem::GetLeftRect()
{
	return _leftRect;
}

void CBCGPGanttFirstItem::OnDrawValue(CDC* pDC, CRect rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pGridRow);

	CBCGPGridCtrl* pWndList = m_pGridRow->GetOwnerList();
	CBCGPGanttGridEx* pWndListEx = DYNAMIC_DOWNCAST(CBCGPGanttGridEx, pWndList);

	ASSERT_VALID(pWndList);

	COLORREF clrTextOld = (COLORREF)-1;
	COLORREF clrText = OnFillBackground(pDC, rect);

	DrawExpandBox(pDC);

	CRect rectText = CRect(_leftRect.right, m_Rect.top, m_Rect.right, m_Rect.bottom);

	if (clrText != (COLORREF)-1)
	{
		clrTextOld = pDC->SetTextColor(clrText);
	}

	//-----------------------
	// Draw data color scale:
	//-----------------------
	if (!m_bInPlaceEdit)
	{
		OnDrawDataColorScale(pDC, rect);
	}

	//---------------
	// Draw data bar:
	//---------------
	if (!m_bInPlaceEdit)
	{
		OnDrawDataBar(pDC, rect);
	}

	//-----------
	// Draw icon:
	//-----------
	OnDrawIcon(pDC, rect);

	// -----------
	// Draw value:
	// -----------
	rect.DeflateRect(globalUtils.ScaleByDPI(TEXT_MARGIN), globalUtils.ScaleByDPI(MENU_IMAGE_MARGIN));

	// If merged, refer to main item
	CBCGPGridItem* pMergedItem = GetMergedMainItem();
	const CString& strText = (pMergedItem != NULL) ? pMergedItem->GetLabel() : GetLabel();
	const DWORD dwFlags = (pMergedItem != NULL) ? pMergedItem->GetFlags() : GetFlags();
	int nTextAlign = (pMergedItem != NULL) ? pMergedItem->GetAlign() : GetAlign();

	UINT uiTextFlags = DT_NOPREFIX | DT_END_ELLIPSIS;

	if (nTextAlign & HDF_CENTER)
	{
		uiTextFlags |= DT_CENTER;
	}
	else if (nTextAlign & HDF_RIGHT)
	{
		uiTextFlags |= DT_RIGHT;
	}
	else // nTextAlign & HDF_LEFT
	{
		uiTextFlags |= DT_LEFT;
	}

	if (dwFlags & BCGP_GRID_ITEM_VTOP)
	{
		uiTextFlags |= DT_TOP;
	}
	else if (dwFlags & BCGP_GRID_ITEM_VBOTTOM)
	{
		uiTextFlags |= DT_BOTTOM;
	}
	else // dwFlags & BCGP_GRID_ITEM_VCENTER
	{
		uiTextFlags |= DT_VCENTER;
	}

	if (!(dwFlags & BCGP_GRID_ITEM_MULTILINE))
	{
		uiTextFlags |= DT_SINGLELINE;
	}
	else
	{
		if (dwFlags & BCGP_GRID_ITEM_WORDWRAP)
		{
			uiTextFlags |= DT_WORDBREAK;
		}
	}

	if ((nTextAlign & HDF_RIGHT) == HDF_RIGHT && !m_rectButton.IsRectEmpty())
	{
		rect.right -= m_rectButton.Width();
	}
	auto dMarkedIndexes = pWndListEx->GetMarkedIndexes();
	CBCGPRAII<DWORD> raiiMarkedIndexes(dMarkedIndexes, m_nMarkedIndexes);

	CFont* pOldFont = NULL;

	if (m_bHighlighted && pWndList->IsHighlightedItemUnderlined(this))
	{
		pOldFont = m_pGridRow->IsBoldFont() ?
			pDC->SelectObject(&pWndList->GetBoldUnderlineFont(TRUE)) :
			pDC->SelectObject(&pWndList->GetUnderlineFont(TRUE));

		ASSERT(pOldFont != NULL);
	}

	const int xBadgeOffset = pWndList->GetNotificationBadgePadding();

	CRect rectBadge = rect;
	CSize sizeBadge = OnBeforeDrawTextLabel(pDC, xBadgeOffset, rectText, strText, FALSE);

	m_bValueIsTrancated = pWndList->OnDrawItemLabel(pDC, this, strText, rectText, uiTextFlags);

	int ncxCurrText = pWndListEx->GetcxCurrText();

	if (nTextAlign & HDF_CENTER)
	{
		m_nContentRight = rect.CenterPoint().x + ncxCurrText / 2;
	}
	else if (nTextAlign & HDF_RIGHT)
	{
		m_nContentRight = rect.right;
	}
	else // nTextAlign & HDF_LEFT
	{
		m_nContentRight = rect.left + ncxCurrText;
	}

	if (clrTextOld != (COLORREF)-1)
	{
		pDC->SetTextColor(clrTextOld);
	}

	OnDrawNotificationBadge(pDC, xBadgeOffset, rectBadge, sizeBadge, nTextAlign, m_nContentRight);

	if (pOldFont->GetSafeHandle() != NULL)
	{
		pDC->SelectObject(pOldFont);
	}
}

void CBCGPGanttFirstItem::DrawExpandBox(CDC* pDC)
{
	int dx = GetOwnerList()->GetHierarchyOffset(GetParentRow());
	CRect rect = GetParentRow()->GetExpandBoxRect(dx);
	CPoint ptCenter = rect.CenterPoint();

	const int nMaxBoxSize = globalUtils.ScaleByDPI(11);
	int nBoxSize = bcgp_min(nMaxBoxSize, rect.Width());

	rect = CRect(ptCenter, CSize(1, 1));
	rect.InflateRect(nBoxSize / 2, nBoxSize / 2);

	COLORREF clrText = globalData.clrBtnText;

	CList<CBCGPGridRow*, CBCGPGridRow*> lst;
	GetParentRow()->GetSubItems(lst); 
	if (lst.GetSize() > 0)
	{
		CBCGPVisualManager::GetInstance()->OnDrawGridExpandingBox(pDC, rect, GetParentRow()->IsExpanded(), clrText);
	}
}

void CBCGPGanttFirstItem::AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin)
{
	__super::AdjustInPlaceEditRect(rectEdit, rectSpin);
	rectEdit.top -= 4;;
}

// 对应SetItemRTC(i, RUNTIME_CLASS(CBCGPGridItemEx))失败
IMPLEMENT_SERIAL(CBCGPGanttFirstItem, CBCGPGridItem, VERSIONABLE_SCHEMA | 1)
//IMPLEMENT_DYNAMIC(CBCGPGridItemEx, CBCGPGridItem)

//////////////////////////////////////////////////////////////////////
//       Implementation of the CBCGPGanttGridRowEx class
//////////////////////////////////////////////////////////////////////

void CBCGPGanttGridRowEx::DrawFirstItemValue(CDC* pDC, CRect rect, BOOL bGroupUnderline)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	CBCGPGanttGridEx* m_pWndListEx = DYNAMIC_DOWNCAST(CBCGPGanttGridEx, m_pWndList);
	HFONT hFontOld = /*m_pWndList->SetBoldFont(pDC);*/ m_bBold ? m_pWndList->SetBoldFont(pDC) : NULL;
	CBCGPGridItem* pItem = m_arrRowItems[0];

	// 自定义绘制
	CBCGPGanttFirstItem* pItemEx = DYNAMIC_DOWNCAST(CBCGPGanttFirstItem, pItem);
	if (pItemEx == nullptr)
		return;

	// 重新设置控件大小
	CRect rectValue = pItemEx->GetRect();
	CRect newRect = CRect(rect.left, rectValue.top, rectValue.right, rectValue.bottom);
	pItemEx->SetRect(newRect);
	pItemEx->SetLeftRect(rect);

	// 绘制背景文字等
	pItemEx->OnDrawValue(pDC, newRect);

	// 绘制边框
	if (m_pWndListEx->m_bGridItemBorders)
	{

		pItem->OnDrawBorders(pDC, newRect);
	}

	if (!pItem->GetButtonRect().IsRectEmpty())
	{
		pItem->OnDrawButton(pDC, pItem->GetButtonRect());
	}

	pDC->SelectClipRgn(&m_pWndListEx->m_rgnClipRow);
	m_pWndListEx->m_rgnClipItem.DeleteObject();


	if (hFontOld != NULL)
	{
		pDC->SelectObject(hFontOld);
	}
}

void CBCGPGanttGridRowEx::OnExpand(BOOL bExpand)
{
	CBCGPGridCtrl* pGridCtrl = GetOwnerList(); // parent grid

	CBCGPGanttGridEx* pGanttGrid = DYNAMIC_DOWNCAST(CBCGPGanttGridEx, pGridCtrl);
	if (pGanttGrid != NULL)
	{
		pGanttGrid->OnExpandGroup(DYNAMIC_DOWNCAST(CBCGPGridRow, this), bExpand);
	}
}

void CBCGPGanttGridRowEx::SwapSubItems(CBCGPGridRow* pRow1, CBCGPGridRow* pRow2)
{
	ASSERT_VALID(pRow1);
	ASSERT_VALID(pRow2);

	POSITION pos1 = m_lstSubItems.Find(pRow1);
	POSITION pos2 = m_lstSubItems.Find(pRow2);

	if (pos1 == NULL || pos2 == NULL || pos1 == pos2)
	{
		return;
	}

	int id1 = pRow1->GetRowId();
	int id2 = pRow2->GetRowId();
	CBCGPGanttGridRowEx* pGanttRow1 = (CBCGPGanttGridRowEx*)pRow1;
	CBCGPGanttGridRowEx* pGanttRow2 = (CBCGPGanttGridRowEx*)pRow2;
	pGanttRow1->m_nIdRow = id2;
	pGanttRow2->m_nIdRow = id1;

	m_lstSubItems.SetAt(pos1, pRow2);
	m_lstSubItems.SetAt(pos2, pRow1);

	pRow1->Redraw();
	pRow2->Redraw();
}

bool CBCGPGanttGridRowEx::HasChild()
{
	return (m_lstSubItems.GetCount() > 0);
}

IMPLEMENT_DYNAMIC(CBCGPGanttGridRowEx, CBCGPGridRow)


//////////////////////////////////////////////////////////////////////
//       Implementation of the CBCGPGanttGrid class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CBCGPGanttGridEx, CBCGPGanttGrid)

BEGIN_MESSAGE_MAP(CBCGPGanttGridEx, CBCGPGanttGrid)
	//{{AFX_MSG_MAP(CBCGPGanttGrid)
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_REGISTERED_MESSAGE(BCGM_GRID_ITEM_ENDINPLACEEDIT, OnEndLabelEdit)
END_MESSAGE_MAP()

CBCGPGanttGridEx::CBCGPGanttGridEx():
	_bBoldFontByHasChild(true)
{
	m_nIMECharSize = 0;
	m_nIMECharLeft = 0;
}

CBCGPGanttGridEx::~CBCGPGanttGridEx()
{

}

CBCGPGridRow* CBCGPGanttGridEx::CreateRow()
{
	return new CBCGPGanttGridRowEx;
}

CBCGPGridRow* CBCGPGanttGridEx::CreateRow(CString strName)
{
	return new CBCGPGanttGridRowEx(strName);
}

CBCGPGridRow* CBCGPGanttGridEx::CreateRow(int nColumns)
{
	ASSERT_VALID(this);

	CBCGPGridRow* pRow = CreateRow();
	ASSERT_VALID(pRow);

	SetupRow(pRow, nColumns);

	return pRow;
}

void CBCGPGanttGridEx::InsertRowAfter(CBCGPGridRow* pParentRow, int nSubPos, CBCGPGridRow* pSubItem)
{
	if (pParentRow == NULL) // top-level item
	{
		CBCGPGridCtrl::InsertRowAfter(nSubPos, pSubItem);
	}
	else
	{
		CBCGPGanttGridRowEx* pGridParentRow = DYNAMIC_DOWNCAST(CBCGPGanttGridRowEx, pParentRow);
		pGridParentRow->AllowSubItems(TRUE);
		pGridParentRow->AddSubItem(pSubItem);

		if (nSubPos < pGridParentRow->GetSubItemsCount(FALSE))
		{
			pGridParentRow->SwapSubItems(pGridParentRow->GetSubItem(nSubPos), pSubItem);
		}
	}
}

BOOL CBCGPGanttGridEx::OnDrawItem(CDC* pDC, CBCGPGridRow* pItem)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT_VALID(pItem);

	if (!pItem->GetRect().IsRectEmpty())
	{
		if (pItem->GetRect().top >= m_rectClip.bottom)
		{
			return FALSE;
		}

		if (pItem->GetRect().bottom >= m_rectClip.top)
		{
			int dx = GetHierarchyOffset(pItem);

			BOOL bIsAutoGroup = pItem->IsGroup() && (pItem->GetFlags() & BCGP_GRID_ITEM_AUTOGROUP) != 0;

			COLORREF clrTextOld = (COLORREF)-1;

			BOOL bNoScrollCol0 = (pItem->IsGroup() && m_bFreezeGroups) || (m_nHorzScrollOffset > 0 && GetColumnsInfo().GetFrozenColumnCount() > 0);
			BOOL bNoScrollGroups = pItem->IsGroup() && m_bFreezeGroups && m_nHorzScrollOffset > 0;

			int nColumn0Clip = 0;
			if (GetColumnsInfo().GetColumnCount(TRUE) > 0)
			{
				int nCol0Idx = GetColumnsInfo().OrderToIndex(0);
				if (nCol0Idx != -1)
				{
					nColumn0Clip = pItem->GetRect().left + (bNoScrollCol0 ? m_nHorzScrollOffset : 0) +
						GetColumnsInfo().GetColumnWidth(nCol0Idx) + GetHierarchyOffset() + GetExtraHierarchyOffset();
				}
			}

			// ----------------
			// Draw left margin
			// ----------------
			CRect rectLeft = pItem->GetIndentRect(dx);
			CRect rectTreeLines = rectLeft;

			if (IsCheckBoxesEnabled() && pItem->HasCheckBox())
			{
				CRect rectCheckBox = pItem->GetCheckBoxRect(dx);
				if (rectCheckBox.right > rectLeft.right)
				{
					rectLeft.right = rectCheckBox.right;
				}
			}

			CRgn rgnClipLeft;
			CRect rectLeftClip = rectLeft;
			rectLeftClip.left = bcgp_max(rectLeftClip.left, m_rectList.left);
			rectLeftClip.bottom = bcgp_min(rectLeftClip.bottom + 1, m_rectList.bottom);

			if (nColumn0Clip > 0 && (!m_bHighlightGroups || !pItem->IsGroup()))
			{
				rectLeftClip.right = bcgp_min(rectLeftClip.right, nColumn0Clip);
			}

			if (rectLeftClip.left < rectLeftClip.right)
			{
				if (GetLeftItemBorderOffset() > rectLeft.right)
				{
					rectLeftClip.top = bcgp_max(rectLeftClip.top - 1, m_rectList.top);
				}

				rgnClipLeft.CreateRectRgnIndirect(&rectLeftClip);
				pDC->SelectClipRgn(&rgnClipLeft);
				
				OnFillLeftOffset(pDC, rectLeft, pItem,
					m_bGridLines && !bIsAutoGroup);

				if (m_bTreeLines)
				{
					OnDrawTreeLines(pDC, pItem, rectTreeLines);
				}

				pDC->SelectClipRgn(&m_rgnClip);
			}

			if (!pItem->IsEnabled())
			{
				clrTextOld = pDC->SetTextColor(globalData.clrGrayedText);
			}
			
			// ---------------
			// draw expandbox:
			// ---------------
			if (pItem->HasExpandButton())
			{
				CRect rectExpand = pItem->GetExpandBoxRect(dx);
				CRgn rgnClipExpand;
				CRect rectExpandClip = rectExpand;
				rectExpandClip.bottom = bcgp_min(rectExpandClip.bottom, m_rectList.bottom);

// 				if (nColumn0Clip > 0 && (!bIsAutoGroup))
// 				{
// 					rectExpandClip.right = bcgp_min(rectLeftClip.right, nColumn0Clip);
// 				}

				if (!rectExpandClip.IsRectEmpty())
				{
					rgnClipExpand.CreateRectRgnIndirect(&rectExpandClip);
					pDC->SelectClipRgn(&rgnClipExpand);
					pItem->OnDrawExpandBox(pDC, rectExpand);

					pDC->SelectClipRgn(&m_rgnClip);
				}
			}
			

			// ----------------
			// Draw row marker:
			// ----------------
			BOOL bActiveItem = (GetCurSel() == pItem);
			if (m_bRowMarker && !IsRowMarkerOnRowHeader() && bActiveItem &&
				!pItem->IsGroup() && GetExtraHierarchyOffset() > 0 && !IsCheckBoxesEnabled())
			{
				CRect rectRowMarker = rectLeft;
				rectRowMarker.left = bcgp_max(
					rectRowMarker.right - GetExtraHierarchyOffset(),
					rectRowMarker.left);

				CRgn rgnClipMarker;
				CRect rectMarkerClip = rectRowMarker;
				rectMarkerClip.left = bcgp_min(rectMarkerClip.left, m_rectList.left);
				rectMarkerClip.bottom = bcgp_min(rectMarkerClip.bottom, m_rectList.bottom);

				rgnClipMarker.CreateRectRgnIndirect(&rectMarkerClip);
				pDC->SelectClipRgn(&rgnClipMarker);

				pItem->OnDrawRowMarker(pDC, rectRowMarker);

				pDC->SelectClipRgn(&m_rgnClip);
			}

			// ----------
			// draw name:
			// ----------
			CRect rectName = pItem->GetNameRect(dx);

			if (rectName.right > rectName.left)
			{
				CRgn rgnClipName;
				CRect rectNameClip = rectName;
				rectNameClip.left = bcgp_max(m_rectRowHeader.right, rectNameClip.left);
				rectNameClip.bottom = bcgp_min(rectNameClip.bottom, m_rectList.bottom);

				rgnClipName.CreateRectRgnIndirect(&rectNameClip);
				pDC->SelectClipRgn(&rgnClipName);

				CFont* pOldFont = NULL;
				if (pItem->IsGroup())
				{
					pOldFont = pDC->SelectObject(&GetBoldFont(TRUE));
				}

				pItem->OnDrawName(pDC, rectName);

				if (pOldFont->GetSafeHandle() != NULL)
				{
					pDC->SelectObject(pOldFont);
				}

				pDC->SelectClipRgn(&m_rgnClip);
			}

			// ---------------
			// draw check box:
			// ---------------
			if (IsCheckBoxesEnabled() && pItem->HasCheckBox())
			{
				CRect rectCheckBox = pItem->GetCheckBoxRect(dx);

				CRgn rgnClipCheckBox;
				CRect rectCheckBoxClip = rectCheckBox;

				if (nColumn0Clip > 0 && (!bIsAutoGroup))
				{
					rectCheckBoxClip.right = bcgp_min(rectLeftClip.right, nColumn0Clip);
				}

				if (!rectCheckBoxClip.IsRectEmpty())
				{
					rgnClipCheckBox.CreateRectRgnIndirect(&rectCheckBoxClip);
					pDC->SelectClipRgn(&rgnClipCheckBox);

					OnDrawCheckBox(pDC, rectCheckBox, pItem->GetCheck(), FALSE, FALSE, pItem->IsEnabled());

					pDC->SelectClipRgn(&m_rgnClip);
				}
			}

			// ------------
			// draw values:
			// ------------
			if (!bIsAutoGroup)
			{
				CRect rectItems = pItem->GetRect();

				CRect rectValClip = rectItems;
				rectValClip.left = bcgp_max(m_rectRowHeader.right, rectValClip.left);
				rectValClip.bottom = bcgp_min(rectValClip.bottom, m_rectList.bottom);
				rectValClip.bottom++;

				if (GetLeftItemBorderOffset() > rectLeft.right)
				{
					rectValClip.top = bcgp_max(rectValClip.top - 1, m_rectList.top);
				}

				m_rgnClipRow.CreateRectRgnIndirect(&rectValClip);
				pDC->SelectClipRgn(&m_rgnClipRow);

				CBCGPGanttGridRowEx* pGanttGridRowEx = DYNAMIC_DOWNCAST(CBCGPGanttGridRowEx, pItem);
				bool bBold = GetBoldFontByHasChild() && pGanttGridRowEx->HasChild();
				pGanttGridRowEx->SetBoldFont(bBold);
				pGanttGridRowEx->DrawFirstItemValue(pDC, rectLeft, FALSE);
				pItem->OnDrawItems(pDC, rectItems);

				if (m_bGridLines && !m_bGridItemBorders)
				{
					// Draw default horizontal grid lines
					pDC->MoveTo(m_rectList.left + GetExtraHierarchyOffset() + dx - m_nHorzScrollOffset, pItem->GetRect().bottom);
					pDC->LineTo(m_rectList.right, pItem->GetRect().bottom);

					if (GetLeftItemBorderOffset() > rectLeft.right)
					{
						// repeat line of the previous row
						pDC->MoveTo(m_rectList.left + GetExtraHierarchyOffset() + dx - m_nHorzScrollOffset, pItem->GetRect().top);
						pDC->LineTo(m_rectList.right, pItem->GetRect().top);
					}
				}

				pDC->SelectClipRgn(NULL);
				m_rgnClipRow.DeleteObject();
			}
			else if (m_bGridLines && !m_bHighlightGroups)
			{
				pDC->SelectClipRgn(NULL);

				int nLeftBorder = bcgp_max(m_rectRowHeader.right, m_rectList.left + GetExtraHierarchyOffset() + dx - m_nHorzScrollOffset);

				pDC->MoveTo(nLeftBorder, pItem->GetRect().bottom);
				pDC->LineTo(rectName.right, pItem->GetRect().bottom);

				if (GetLeftItemBorderOffset() > rectLeft.right)
				{
					// repeat line of the previous row
					pDC->MoveTo(m_rectList.left + GetExtraHierarchyOffset() + dx - m_nHorzScrollOffset, pItem->GetRect().top);
					pDC->LineTo(rectName.right, pItem->GetRect().top);
				}
			}

			m_nLeftItemBorderOffset = (!true || bIsAutoGroup) ? 0 : rectLeft.right;

			pDC->SelectClipRgn(&m_rgnClip);

			if (m_bDrawFocusRect &&
				IsFocused() && IsWholeRowSel() && GetCurSel() == pItem)
			{
				CRect rect = pItem->GetRect();
				if (bNoScrollGroups)
				{
					rect.OffsetRect(m_nHorzScrollOffset, 0);
				}
				rect.top++;
				rect.DeflateRect(dx, 0, 0, 0);
				pDC->DrawFocusRect(rect);
			}

			if (clrTextOld != (COLORREF)-1)
			{
				pDC->SetTextColor(clrTextOld);
			}
		}
	}

	return TRUE;
}

int CBCGPGanttGridEx::GetExtraHierarchyOffset() const
{
	return globalUtils.ScaleByDPI(21);
}

int CBCGPGanttGridEx::GetHierarchyLevelOffset() const
{
	return globalUtils.ScaleByDPI(13);
}

int CBCGPGanttGridEx::GetTreeColumn()
{
	return m_nTreeColumn;
}

void CBCGPGanttGridEx::SetTreeColumn(int nColumn)
{
	m_nTreeColumn = nColumn;
}

CBCGPGridRow * CBCGPGanttGridEx::GetLastSelRow()
{
	return m_pLastSelRow;
}

void CBCGPGanttGridEx::SetFontSize(int nSize)
{
	LOGFONT   logfont;
	CFont   *pfont1 = GetFont();
	pfont1->GetLogFont(&logfont);
	logfont.lfHeight = nSize;
	logfont.lfWidth = nSize;
	CFont   font1;
	font1.CreateFontIndirect(&logfont);
	SetFont(&font1);
	font1.Detach();
}

void CBCGPGanttGridEx::SetFontSizeScale(float fScale)
{
	LOGFONT   logfont;
	CFont   *pfont1 = GetFont();
	pfont1->GetLogFont(&logfont);
	_tcscpy_s(logfont.lfFaceName, _T("Microsoft YaHei UI"));
	logfont.lfHeight = logfont.lfHeight * fScale;   //这里可以修改字体的高比例
	logfont.lfWidth = logfont.lfWidth * fScale;   //这里可以修改字体的宽比例
	CFont   font1;
	font1.CreateFontIndirect(&logfont);
	SetFont(&font1);
	font1.Detach();
}

void CBCGPGanttGridEx::SetBoldFontByHasChild(bool bBold /*= true*/)
{
	_bBoldFontByHasChild = bBold;
}

bool CBCGPGanttGridEx::GetBoldFontByHasChild()
{
	return _bBoldFontByHasChild;
}

void CBCGPGanttGridEx::SetDragDropRows()
{
	EnableDragSelectionBorder(TRUE);
	SetHighlightActiveItem(FALSE);
	SetEditFirstClick(TRUE);

	HideDragInsertMarker();
	HideDragFrame();
	Invalidate();

	SetCurSel(GetCurSelItemID());
	m_pSerializeManager->SetClipboardFormatType(CBCGPGridSerializeManager::CF_Rows);
}

void CBCGPGanttGridEx::OnRowHeaderDblClick(CBCGPGridRow* pRow, CPoint point, CRect rect)
{
	ASSERT_VALID(pRow);
	if (pRow->IsGroup())
	{
		CBCGPGridRow::ClickArea clickArea;
		CBCGPGridItemID id;
		CBCGPGridItem* pHitItem = NULL;
		HitTest(point, id, pHitItem, &clickArea);

		if (clickArea != CBCGPGridRow::ClickExpandBox && clickArea != CBCGPGridRow::ClickCheckBox)
		{
			ExpandRow(pRow, !pRow->IsExpanded());
			return;
		}
	}
	
	pRow->OnDblClick(point);
}

CRect CBCGPGanttGridEx::OnGetSelectionRect()
{
	m_rectClipSel.SetRectEmpty();

	if (!IsSelectionBorderEnabled() ||
		m_lstSel.GetCount() != 1 || m_lstSel.GetTail() == NULL)
	{
		return CRect(0, 0, 0, 0);
	}

	BOOL bFreeze = (GetColumnsInfo().IsFreezeColumnsEnabled() && m_nHorzScrollOffset > 0);

	const CBCGPGridRange rangeTrackSel = *(m_lstSel.GetTail());

	CBCGPGridRow* pTopRow = (rangeTrackSel.m_nTop != -1) ? GetRow(rangeTrackSel.m_nTop) : NULL;
	CBCGPGridItem* pLeftItem = (pTopRow != NULL) ? pTopRow->GetItem(rangeTrackSel.m_nLeft) : NULL;
	CBCGPGridRow* pBottomRow = (rangeTrackSel.m_nBottom != -1) ? GetRow(rangeTrackSel.m_nBottom) : NULL;
	CBCGPGridItem* pRightItem = (pBottomRow != NULL) ? pBottomRow->GetItem(rangeTrackSel.m_nRight) : NULL;

	//-------------------------------
	// Calculate selection rectangle:
	//-------------------------------
	const int nLeftOffset = 0;

	CRect rectTopLeft(0, 0, 0, 0);
	if (pTopRow != NULL)
	{
		ASSERT_VALID(pTopRow);

		if (pLeftItem != NULL)
		{
			auto pLeftItemEx = dynamic_cast<CBCGPGanttFirstItem*>(pLeftItem);
			if (pLeftItemEx != nullptr)
			{
				rectTopLeft = pLeftItem->GetRect();
				auto nLeftPos = pLeftItemEx->GetLeftRect().left;
				rectTopLeft.right -= (rectTopLeft.left - nLeftPos);
				rectTopLeft.left = nLeftPos;
			}
			else
				rectTopLeft = pLeftItem->GetRect();
		}else
			rectTopLeft = pTopRow->GetRect();

		if ((IsFilterBarEnabled() || IsFilterEnabled()) && m_idActive.IsColumn())
		{
			GetColumnsInfo().GetColumnRect(m_idActive.m_nColumn, rectTopLeft);
			rectTopLeft.top = m_rectList.top;
			rectTopLeft.bottom = m_rectList.bottom;
		}

		if (pLeftItem == NULL)
		{
			rectTopLeft.right = bcgp_max(rectTopLeft.right - 1, rectTopLeft.left);
			rectTopLeft.left = bcgp_min(rectTopLeft.right, rectTopLeft.left + nLeftOffset);

			if (bFreeze)
			{
				rectTopLeft.left = bcgp_min(rectTopLeft.right, m_rectList.left + nLeftOffset);
			}
		}
		rectTopLeft.NormalizeRect();

		// Extend if merged:
		if (pLeftItem != NULL && pLeftItem->GetMergedCells() != NULL)
		{
			CRect rectMerged = pLeftItem->GetMergedRect();
			if (!rectMerged.IsRectEmpty())
			{
				rectMerged.NormalizeRect();
				rectTopLeft.UnionRect(rectTopLeft, rectMerged);
			}
		}
	}

	CRect rectBottomRight(0, 0, 0, 0);
	if (pBottomRow != NULL)
	{
		ASSERT_VALID(pBottomRow);

		rectBottomRight = (pRightItem != NULL) ? pRightItem->GetRect() : pBottomRow->GetRect();

		if ((IsFilterBarEnabled() || IsFilterEnabled()) && m_idLastSel.IsColumn())
		{
			GetColumnsInfo().GetColumnRect(m_idLastSel.m_nColumn, rectBottomRight);
			rectBottomRight.top = m_rectList.top;
			rectBottomRight.bottom = m_rectList.bottom;
		}

		if (pRightItem == NULL)
		{
			rectBottomRight.left = bcgp_min(rectBottomRight.right, rectBottomRight.left + nLeftOffset);

			if (bFreeze)
			{
				rectBottomRight.left = bcgp_min(rectBottomRight.right, m_rectList.left + nLeftOffset);
			}
		}
		rectBottomRight.NormalizeRect();

		// Extend if merged:
		if (pRightItem != NULL && pRightItem->GetMergedCells() != NULL)
		{
			CRect rectMerged = pRightItem->GetMergedRect();
			if (!rectMerged.IsRectEmpty())
			{
				rectMerged.NormalizeRect();
				rectBottomRight.UnionRect(rectBottomRight, rectMerged);
			}
		}
	}

	CRect rectTrackSel(0, 0, 0, 0);
	rectTrackSel.UnionRect(rectTopLeft, rectBottomRight);

	if (!rectTrackSel.IsRectEmpty())
	{
		rectTrackSel.InflateRect(2, 1, 2, 2);
	}

	// Frozen columns support:
	m_rectClipSel = rectTrackSel;
	if (bFreeze && pLeftItem != NULL && pRightItem != NULL)
	{
		const int nFreezeOffset = m_rectList.left + GetColumnsInfo().GetFreezeOffset();
		BOOL bA = (GetColumnsInfo().IsColumnFrozen(pLeftItem->GetColumnId()));
		BOOL bB = (GetColumnsInfo().IsColumnFrozen(pRightItem->GetColumnId()));

		if (!bA && !bB) // Clip Selection
		{
			m_rectClipSel.left = bcgp_min(nFreezeOffset, m_rectClipSel.right);
		}
		else if (bA && bB)
		{
		}
		else
		{
			rectTrackSel = CRect(0, 0, 0, 0);
			rectTrackSel.UnionRect(rectTopLeft, rectBottomRight);

			if (bA && !bB)
			{
				// scrolled column must not change left border of selection
				if (rectTrackSel.left < rectTopLeft.left)
				{
					rectTrackSel.left = rectTopLeft.left;
				}
			}
			else if (!bA && bB)
			{
				// scrolled column must not change left border of selection
				if (rectTrackSel.left < rectBottomRight.left)
				{
					rectTrackSel.left = rectBottomRight.left;
				}
			}

			if (!rectTrackSel.IsRectEmpty())
			{
				rectTrackSel.InflateRect(2, 1, 2, 2);
			}

			// scrolled column must not change right border of selection
			if (rectTrackSel.right < nFreezeOffset)
			{
				rectTrackSel.right = nFreezeOffset;
			}

			m_rectClipSel = rectTrackSel;
		}
	}
	else if (IsRowHeaderEnabled())
	{
		m_rectClipSel.left = bcgp_min(m_rectList.left, m_rectClipSel.right);
	}

	// Filterbar support:
	if (IsFilterBarEnabled() && !m_rectFilterBar.IsRectEmpty() && m_rectFilterBar.bottom > m_rectClipSel.top)
	{
		m_rectClipSel.top = bcgp_min(m_rectFilterBar.bottom, m_rectClipSel.bottom);
	}

	return rectTrackSel;
}

void CBCGPGanttGridEx::OnDrawSelectionBorder(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	if (m_bIsPrinting || IsGrouping())
	{
		return;
	}

	if (!m_rectTrackSel.IsRectEmpty() && !m_rectClipSel.IsRectEmpty())
	{
		CRect rectClip(0, 0, 0, 0);
		if (!rectClip.IntersectRect(m_rectClipSel, m_rectList))
		{
			return;
		}

		CRgn rgnClipSel;
		rgnClipSel.CreateRectRgnIndirect(&rectClip);
		pDC->SelectClipRgn(&rgnClipSel);

		// paint clipped
		CRect rectTmp(m_rectTrackSel);
		COLORREF colorGreen = RGB(49, 117, 47);//设置选中框的颜色
		pDC->Draw3dRect(rectTmp, colorGreen, colorGreen);
		rectTmp.DeflateRect(1, 1);
		pDC->Draw3dRect(rectTmp, colorGreen, colorGreen);

		pDC->SelectClipRgn(NULL);
	}
}

BOOL CBCGPGanttGridEx::DragItems(CPoint point)
{
	if (m_bDragDrop)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	point = m_ptClickOnce;

	//-----------------
	// Set m_idDragFrom
	//-----------------
	CRect rectRowHeader(0, 0, 0, 0);
	CBCGPGridRow* pRow = HitTestRowHeader(point, rectRowHeader);

	if (pRow != NULL)
	{
		m_bDragRowHeader = TRUE;
		m_idDragFrom = CBCGPGridItemID(pRow->GetRowId(), 0);
	}
	else
	{
		CBCGPGridItem* pHitItem = NULL;
		HitTest(point, m_idDragFrom, pHitItem, NULL, TRUE);
	}

	if (m_idDragFrom.IsNull())
	{
		return FALSE;
	}

	//-------
	// Notify
	//-------
	if (OnBeginDrag(point))
	{
		return TRUE;	// Custom Drag-and-Drop
	}

	if (m_pSerializeManager == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pSerializeManager);

	//----------------------
	// Start OLE drag'n'drop
	//----------------------
	COleDataSource* pSrcItem = new COleDataSource();
	m_bDragDrop = TRUE;

	if (!m_bDragDrop)
	{
		if (pSrcItem != NULL)
		{
			pSrcItem->InternalRelease();
			pSrcItem = NULL;
		}

		return FALSE;
	}

	DROPEFFECT dropEffect = pSrcItem->DoDragDrop(DROPEFFECT_COPY | DROPEFFECT_MOVE/*, NULL, &m_DropSource*/);
	dropEffect = DROPEFFECT_MOVE;

	//-----------------------
	// Finish OLE drag'n'drop
	//-----------------------
	CPoint ptEnd;
	GetCursorPos(&ptEnd);
	ScreenToClient(&ptEnd);

	BOOL bUpdate = FALSE;

	if (dropEffect != DROPEFFECT_NONE)
	{
		if (dropEffect == DROPEFFECT_MOVE)
		{
			// m_DropEffect is DROPEFFECT_NONE when content is already dropped to this grid
			// m_idDropTo is null when content is dropped to another application
			if (m_DropEffect != DROPEFFECT_NONE)
			{
				m_pSerializeManager->ClearPreviousSelection(FALSE);
				bUpdate = TRUE;
			}
		}
	}

	StopDragItems();
	m_pSerializeManager->CleanUp();

	if (bUpdate)
	{
		RedrawWindow();
	}

	pSrcItem->InternalRelease();
	return TRUE;
}

BOOL CBCGPGanttGridEx::DoDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);

	if (IsReadOnly() || m_pSerializeManager == NULL)
	{
		return FALSE;
	}

	ASSERT_VALID(m_pSerializeManager);

// 	if (!pDataObject->IsDataAvailable(m_pSerializeManager->GetClipboardFormat()) || IsReadOnly())
// 	{
// 		return FALSE;
// 	}

	CBCGPGridItemID idOldDropTo = m_idDropTo;

	//--------
	// HitTest
	//--------
	CBCGPGridRow* pHitRow = NULL;
	if (m_bDragRowHeader)
	{
		CRect rectRowHeader(0, 0, 0, 0);
		pHitRow = HitTestRowHeader(point, rectRowHeader);
		if (pHitRow != NULL)
		{
			m_idDropTo = CBCGPGridItemID(pHitRow->GetRowId(), 0);
		}
	}
	if (pHitRow == NULL)
	{
		CBCGPGridItem* pHitItem = NULL;
		pHitRow = HitTest(point, m_idDropTo, pHitItem, NULL, TRUE);
	}

	if (m_bDragRowHeader || IsWholeRowSel())
	{
		m_idDropTo.m_nColumn = 0;
	}

	if (pHitRow == NULL)
	{
		return FALSE;
	}

	DropArea dropArea = HitTestDropArea(point, m_idDropTo, pHitRow);

	DROPEFFECT dropEffect = (dwKeyState & MK_CONTROL) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;

	if (m_idDropTo == idOldDropTo && dropArea == m_DropArea) // no changes - return saved drop effect
	{
		dropEffect = m_DropEffect;
		return TRUE;
	}

	//---------------
	// Get drop range 涉及数据直接拖动的，注释掉，只需要拖动的行及动态显示信息即可
	//---------------
// 	CFile* pFile = pDataObject->GetFileData(m_pSerializeManager->GetClipboardFormat());
// 	if (pFile == NULL)
// 	{
// 		return FALSE;
// 	}

	// m_bDragDrop is FALSE when content is dragged from another application
	if (!m_bDragDrop)
	{
		m_idDragFrom.SetNull();
	}

// 	BOOL bCanDrop =
// 		m_pSerializeManager->SerializeFrom(*pFile) &&
// 		m_pSerializeManager->CanDrop(m_idDropTo, m_idDragFrom, (dropEffect == DROPEFFECT_MOVE) && m_bDragDrop, dropArea == DropAfter);

	//delete pFile;

// 	if (!bCanDrop)
// 	{
// 		return FALSE;
// 	}

	m_DropEffect = dropEffect;
	m_DropArea = dropArea;

	return TRUE;
}

BOOL CBCGPGanttGridEx::DoDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	ASSERT_VALID(this);

	if (m_pSerializeManager == NULL)
	{
		return FALSE;
	}

	if (m_bClickDrag)
	{
		CWnd* pOwner = GetOwner();
		if (pOwner == NULL)
		{
			return FALSE;
		}

		int nRowFrom = m_idDragFrom.m_nRow;
		int nRowTo = m_idDropTo.m_nRow;
		if (nRowFrom <= nRowTo && m_DropArea == DropBefore)
			nRowTo--;
		else if (nRowFrom > nRowTo && m_DropArea != DropBefore)
			nRowTo++;
		
		if(nRowTo != nRowFrom)
			pOwner->SendMessage(BCGM_GRID_ITEM_DRAG, nRowFrom, nRowTo);
	}

	ASSERT_VALID(m_pSerializeManager);

	if (IsReadOnly() || !pDataObject->IsDataAvailable(m_pSerializeManager->GetClipboardFormat()))
	{
		return FALSE;
	}

	if (dropEffect != DROPEFFECT_MOVE && dropEffect != DROPEFFECT_COPY)
	{
		return FALSE;
	}

	ASSERT(dropEffect == m_DropEffect);

	//--------------------------
	// Update previous selection
	//--------------------------
	InvalidateRect(OnGetSelectionRect());
	for (int i = 0; i < GetSelectionCount(); i++)
	{
		CBCGPGridRange range;
		if (GetSelection(i, range))
		{
			CRect rect = GetRect(range);
			InvalidateRect(rect);
		}
	}

	//-----
	// Drop
	//-----
	// m_bDragDrop is FALSE when content is dragged from another application
	if (!m_bDragDrop)
	{
		m_idDragFrom.SetNull();
	}

// 	BOOL bResult =
// 		m_pSerializeManager->Drop(m_idDropTo, m_idDragFrom,
// 		(dropEffect == DROPEFFECT_MOVE) && m_bDragDrop, m_DropArea == DropAfter);

	if (!m_bDragDrop)
	{
		m_pSerializeManager->CleanUp();
	}

	return TRUE;
}

DROPEFFECT CBCGPGanttGridEx::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (!DoDragOver(pDataObject, dwKeyState, point))
	{
		m_DropEffect = DROPEFFECT_NONE;
	}

	if (m_DropEffect != DROPEFFECT_COPY && m_DropEffect != DROPEFFECT_MOVE)
	{
		//--------------------
		// Hide drag indicator
		//--------------------
		HideDragFrame();
		HideDragInsertMarker();
		RedrawWindow();
	}
	else
	{
		//--------------------
		// Draw drag indicator
		//--------------------
		ShowDragFrame();
		ShowDragInsertMarker();
		RedrawWindow();
	}

	return m_DropEffect;
}

//void CBCGPGanttGridEx::OnEndInplaceEdit(CBCGPGridItem * pItem)
//{
//
//}


void CBCGPGanttGridEx::OnEndInplaceEdit(CBCGPGridItem * pItem)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pItem);

	CWnd* pOwner = GetOwner();
	if (pOwner == NULL)
	{
		return;
	}

	CBCGPGridItemID id = pItem->GetGridItemID();

	memset(&m_CurrentItemInfo, 0, sizeof(BCGPGRID_ITEM_INFO));

	m_CurrentItemInfo.pItem = pItem;
	m_CurrentItemInfo.nRow = id.m_nRow;
	m_CurrentItemInfo.nCol = id.m_nColumn;
	m_CurrentItemInfo.dwResultCode = (DWORD_PTR)m_dwEndEditResult;

	ClearBeginEditReason();
	ClearEndEditReason();

	//pOwner->PostMessage(BCGM_GRID_ITEM_ENDINPLACEEDIT, GetDlgCtrlID(), LPARAM(&m_CurrentItemInfo));
	//使用SendMessage发送消息  使其响应消息后在返回
	this->SendMessage(BCGM_GRID_ITEM_ENDINPLACEEDIT, GetDlgCtrlID(), LPARAM(&m_CurrentItemInfo));
}

BOOL CBCGPGanttGridEx::EditItem(CBCGPGridRow * pItem, LPPOINT lptClick)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pItem);

	ClearEndEditReason();

	if (!EndEditItem())
	{
		return FALSE;
	}

	//if (pItem->IsGroup() && !pItem->HasValueField())
	if (pItem->IsGroup() && !TRUE)
	{
		return FALSE;
	}

	//ASSERT(pItem == GetCurSel());
	if (pItem != GetCurSel())
	{
		return FALSE;
	}

	if (pItem->OnEdit(lptClick))
	{
		pItem->Redraw();
		//SetCurSel (m_idActive, SM_SET_ACTIVE_ITEM);
		SetCapture();
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CBCGPGanttGridEx::CanEndInplaceEditOnChar(UINT nChar, UINT nRepCnt, UINT nFlags) const
{
	switch (nChar)
	{
	case VK_TAB:	// Enable these keys
		return TRUE; // To close in-place edit
		break;
	case VK_LEFT:
	case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
	case VK_HOME:
	case VK_END:
		break;
	}

	return CBCGPGridCtrl::CanEndInplaceEditOnChar(nChar, nRepCnt, nFlags);
}

LRESULT CBCGPGanttGridEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_IME_ENDCOMPOSITION)//当输入法结束输入时(shift、space)时发送此消息
	{
		HIMC hIMC = ImmGetContext(this->m_hWnd);
		m_nIMECharLeft = m_nIMECharSize = ImmGetCompositionString(hIMC, GCS_RESULTSTR, 0, 0);
		ImmReleaseContext(this->m_hWnd, hIMC);
	}
	LRESULT lRes = CBCGPGanttGrid::WindowProc(message, wParam, lParam);

	return lRes;
}

void CBCGPGanttGridEx::UpdateInplaceWnd()
{
	CBCGPGridRow* pSel = GetCurSel();
	if (nullptr == pSel || nullptr == GetCurSelItem(pSel))
		return ;

	CBCGPGridItem* pSelItem = GetCurSelItem(pSel);
	CString strFormatItem = pSelItem->FormatItem();
	CWnd* hwnd = pSelItem->GetInPlaceWnd();
	if (pSelItem->IsKindOf(RUNTIME_CLASS(CBCGPGridPopupDlgItem)) && nullptr != hwnd)
	{
		CString strText;
		hwnd->GetWindowText(strText);
		CRect buttonRect = pSelItem->GetButtonRect();
		CPoint ptCursor;
		::GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		if (strText != strFormatItem && buttonRect.PtInRect(ptCursor))
		{
			hwnd->SetWindowTextW(strFormatItem);
		}
	}
}

BOOL CBCGPGanttGridEx::PreTranslateMessage(MSG* pMsg)
{
	//SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		UpdateInplaceWnd();
	}
	return CBCGPGanttGrid::PreTranslateMessage(pMsg);
}

void CBCGPGanttGridEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//__super::OnLButtonDblClk(nFlags, point);
	CBCGPWnd::OnLButtonDblClk(nFlags, point);

	// if header double click:
	if (m_rectHeader.PtInRect(point))
	{
		if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
		{
			// do nothing
		}
		else
		{
			int nClickColumn = GetColumnsInfo().HitTestColumn(point, TRUE, m_nTrackStretchDelta);
			if (nClickColumn >= 0)
			{
				OnHeaderDividerDblClick(nClickColumn);
			}
		}

		return;
	}

	//-------------------------
	// Row header double click:
	//-------------------------
	CRect rectRowHeader(0, 0, 0, 0);
	CBCGPGridRow* pHitHeaderRow = HitTestRowHeader(point, rectRowHeader);
	if (pHitHeaderRow != NULL)
	{
		OnRowHeaderDblClick(pHitHeaderRow, point, rectRowHeader);
		return;
	}

	CBCGPGridRow* pSel = GetCurSel();

	if (pSel == NULL || !pSel->GetRect().PtInRect(point))
	{
		return;
	}

	ASSERT_VALID(pSel);

	if (pSel != NULL && pSel->IsEnabled())
	{
		CBCGPGridRow::ClickArea clickArea;
		CBCGPGridItemID id;
		CBCGPGridItem* pHitItem = NULL;
		HitTest(point, id, pHitItem, &clickArea);
		if (clickArea == CBCGPGridRow::ClickExpandBox ||
			clickArea == CBCGPGridRow::ClickCheckBox)
		{
			return;
		}

		pSel->OnDblClick(point);
	}
}

void CBCGPGanttGridEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	CBCGPWnd::OnRButtonDown(nFlags, point);

	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
	{
		return;
	}

	SetFocus();

	// -------------
	// Track header:
	// -------------
	if (OnTrackHeader() && point.y >= m_rectList.top)
	{
		CPoint ptHeader = point;
		ptHeader.y = m_rectHeader.top;

		if (GetColumnsInfo().HitTestColumn(ptHeader, TRUE, m_nTrackStretchDelta) != -1)
		{
			return;
		}
	}

	//--------------
	// Header click:
	//--------------
	int nColumnHit = GetColumnsInfo().HitTestColumn(point, FALSE, m_nTrackStretchDelta);
	if (nColumnHit >= 0)
	{
		OnHeaderColumnRClick(nColumnHit);
		return;
	}

	CBCGPGridRow::ClickArea clickArea;
	CBCGPGridItemID id;
	CBCGPGridItem* pHitItem = NULL;
	CBCGPGridRow* pHitRow = HitTest(point, id, pHitItem, &clickArea);
	
	//------------------
	// Row header click:
	//------------------
	if(pHitRow == nullptr)
	{
		CRect rectRowHeader(0, 0, 0, 0);
		pHitRow = HitTestRowHeader(point, rectRowHeader);
	}
	
	if (DYNAMIC_DOWNCAST(CBCGPGridCaptionRow, pHitRow) != NULL)
	{
		return;
	}

	BOOL bSaveSelection = (id.IsNull() && !m_bClearSelOnEmptySpace) ||
		pHitRow != NULL && (pHitItem != NULL ? pHitItem->IsSelected() : pHitRow->IsSelected());

	BOOL bSelChanged = id != m_idLastSel;

	const BOOL bShift = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0 && !m_bIgnoreShiftBtn;
	const BOOL bCtrl = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0 && !m_bIgnoreCtrlBtn;

	if (!bSaveSelection)
	{
		DWORD dwSelMode = SM_NONE;
		if (!id.IsNull())
		{
			dwSelMode = SM_FIRST_CLICK |
				(m_bSingleSel ? SM_SINGE_SEL_GROUP :
				(bCtrl ? SM_ADD_SEL_GROUP :
					(bShift ? SM_CONTINUE_SEL_GROUP : SM_SINGE_SEL_GROUP)));

			if (bCtrl && m_bInvertSelOnCtrl)
			{
				dwSelMode |= SM_INVERT_SEL;
			}
		}

		m_pSetSelItem = m_bVirtualMode ? NULL : pHitItem;

		CBCGPRAII<UINT> raiiRecentAction(m_nRecentSelectAction, TVC_BYMOUSE);
		SetCurSel(id, dwSelMode);

		m_pSetSelItem = NULL;
	}

	if (id.IsNull() || pHitRow == NULL)
	{
		return;
	}

	ASSERT_VALID(pHitRow);
	EnsureVisible(pHitRow);

	CBCGPGridRow* pCurSel = GetCurSel();
	CBCGPGridItem* pItem = GetCurSelItem(pCurSel);
	if (id != m_idActive || pCurSel == NULL || pItem == NULL)
	{
		// The hitten item is not active - do not translate a click to the grid item.
		// Translate a click for single item only.
		return;
	}

	ASSERT_VALID(pCurSel); // pCurSel - hitten row
	ASSERT_VALID(pItem);	// pItem - hitten active item

	switch (clickArea)
	{
	case CBCGPGridRow::ClickExpandBox:
		break;

	case CBCGPGridRow::ClickName:
		pCurSel->OnRClickName(point);
		break;

	case CBCGPGridRow::ClickValue:
		pCurSel->OnRClickValue(point, bSelChanged);

		if (pCurSel->IsEnabled() && !bSelChanged && !bSaveSelection)
		{
			SetBeginEditReason(BeginEdit_MouseClick, bShift, bCtrl);
			if (EditItem(pCurSel, &point) && pCurSel->IsInPlaceEditing())
			{
				CRect buttonRect = pItem->GetButtonRect();
				if (buttonRect.PtInRect(point))
				{
					return;
				}

				pItem->OnClickValue(WM_RBUTTONDOWN, point);
			}
		}
		break;

	default:
		break;
	}
}

void CBCGPGanttGridEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBCGPWnd::OnChar(nChar, nRepCnt, nFlags);
	CString strText;
	bool bFlag = true;//若为中文输入将置为false;
	if (m_nIMECharSize != 0)
	{
		bFlag = false;
		m_nIMECharLeft -= 2;
		if(m_nIMECharLeft != 0)
		{
			return;
		}
		if(m_nIMECharLeft == 0)
		{
			HIMC  hIMC = ImmGetContext(this->m_hWnd);
			WCHAR pBuf[1000] = { 0 };
			ImmGetCompositionString(hIMC, GCS_RESULTSTR, pBuf, 1000);
			strText = pBuf;
			ImmReleaseContext(this->m_hWnd, hIMC);
			//DoInputString(pBuf);
			m_nIMECharSize = 0;
		}
	}

 	if (m_nDraggedColumn >= 0 || m_bTracking || m_bSelecting)
 	{
 		return;
 	}
 
 	if (!CanBeginInplaceEditOnChar(nChar, nRepCnt, nFlags))
 	{
 		return;
 	}
 
 	CBCGPGridRow* pSel = GetCurSel();
 	if (pSel == NULL || !pSel->IsEnabled())
 	{
 		return;
 	}
 
 	ASSERT_VALID(pSel);
 
 	if (nChar == VK_SPACE && pSel->HasCheckBox())
 	{
 		// If active item has a check box, let it toogle.
 		CBCGPGridItem* pSelItem = GetCurSelItem(pSel);
 		if (pSelItem == NULL || !pSelItem->IsKindOf(RUNTIME_CLASS(CBCGPGridCheckItem)))
 		{
 			return;
 		}
 	}
	
	CBCGPGridItem* pSelItem = GetCurSelItem(pSel);
	if(nullptr == pSelItem || !pSelItem->IsEnabled())
		return;
	ASSERT_VALID(pSelItem);
	if(pSelItem->IsKindOf(RUNTIME_CLASS(CBCGPGridDateTimeItem)) ||
		pSelItem->IsKindOf(RUNTIME_CLASS(CBCGPGridSpinItem)))
	{
		return;
	}
	if(pSelItem->IsReadOnly())
	{
		return;
	}
 	if (::GetAsyncKeyState(VK_CONTROL) & 0x8000)
 	{
 		return;
 	}
 
 	EnsureVisible(pSel, TRUE);
 
 	SetBeginEditReason(BeginEdit_Char);
 	if (!EditItem(pSel))
 	{
 		return;
 	}
	
 	DoInplaceEditSetSel(OnInplaceEditSetSel(GetCurSelItem(pSel), BeginEdit_Char));

	if(bFlag)
	{
		pSel->PushChar(nChar);
	}
	else
	{
		CWnd* hwnd = pSelItem->GetInPlaceWnd();
		hwnd->SetWindowText(strText);
		hwnd->SendMessage(WM_KEYDOWN, VK_END, 0);//将鼠标移至文本框尾部
	}
}

LRESULT CBCGPGanttGridEx::OnEndLabelEdit(WPARAM, LPARAM lp)
{
	BCGPGRID_ITEM_INFO* pii = (BCGPGRID_ITEM_INFO*)lp;
	ASSERT(pii != NULL);
	ASSERT_VALID(pii->pItem);

	// Handle ENTER after in-place edit is closed
	if ((pii->dwResultCode & EndEdit_Return) != 0)
	{
		//OnInplaceEditEnter(pii->pItem);
		//return 0;
	}

	// Handle other keys
	UINT nDirection = OnGridKeybordNavigation(pii->pItem, (UINT)pii->dwResultCode);
	if (nDirection != NoMove)
	{
		GoToNextItem(nDirection);
		//CBCGPGridItemID itemId = pii->pItem->GetGridItemID();
		//itemId.m_nColumn += 1;
		//SetCurSel(itemId);
	}
	return 0;
}
