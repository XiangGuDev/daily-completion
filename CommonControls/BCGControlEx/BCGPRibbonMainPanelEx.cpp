#include "stdafx.h"
#include "BCGPRibbonMainPanelEx.h"
#include "BCGPRibbonPanelMenuEx.h"
#include "Include\BCGPRibbonBarEx.h"

using namespace controls;

#define LABEL_MARGIN_X				8
#define LABEL_MARGIN_Y				4
#define BACKSTAGE_BUTTON_MARGIN_X	6
#define PINNED_FILE_ID				(UINT)-2

CBCGPRibbonMainButtonEx::CBCGPRibbonMainButtonEx()
{

}

CBCGPRibbonMainButtonEx::~CBCGPRibbonMainButtonEx()
{

}

void CBCGPRibbonMainButtonEx::OnLButtonDown(CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pRibbonBar);

	if (m_pRibbonBar->GetMainCategory() == NULL && m_pRibbonBar->GetBackstageCategory() == NULL)
	{
		CBCGPRibbonButton::OnLButtonDown(point);
		return;
	}

	CBCGPBaseRibbonElement::OnLButtonDown(point);

	if (!ShowMainMenuEx())
	{
		CBCGPRibbonButton::OnLButtonDown(point);
	}
}

BOOL CBCGPRibbonMainButtonEx::ShowMainMenuEx()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pRibbonBar);
	CBCGPRibbonBarEx* pRibbonBarEx = dynamic_cast<CBCGPRibbonBarEx*>(m_pRibbonBar);

	if (IsDisabled())
	{
		return FALSE;
	}

	if (m_pRibbonBar->IsBackstageViewActive())
	{
		return FALSE;
	}

	CBCGPRibbonCategory* pCategory = NULL;

	if (m_pRibbonBar->GetBackstageCategory() != NULL && m_pRibbonBar->GetMainCategory() != NULL)
	{
		pCategory = pRibbonBarEx->GetBackstageMode() ? m_pRibbonBar->GetBackstageCategory() : m_pRibbonBar->GetMainCategory();
	}
	else
	{
		pCategory = m_pRibbonBar->GetBackstageCategory() != NULL ? m_pRibbonBar->GetBackstageCategory() : m_pRibbonBar->GetMainCategory();
	}

	if (pCategory == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (pCategory->GetPanelCount() == 0)
	{
		return FALSE;
	}

	if (pCategory == m_pRibbonBar->GetBackstageCategory() && pRibbonBarEx->GetPrintPreviewMode())
	{
		return FALSE;
	}

	CBCGPBaseRibbonElement::OnShowPopupMenu();

	const BOOL bIsRTL = (m_pRibbonBar->GetExStyle() & WS_EX_LAYOUTRTL);

	CRect rectBtn = m_rect;
	m_pRibbonBar->ClientToScreen(&rectBtn);

	pCategory->SetPadding(CSize(5,6));//文件记录显示十条
	//CBCGPRibbonMainPanel* pPanel = DYNAMIC_DOWNCAST(CBCGPRibbonMainPanel, pCategory->GetPanel(0));
	CBCGPRibbonMainPanelEx* pPanel = DYNAMIC_DOWNCAST(CBCGPRibbonMainPanelEx, pCategory->GetPanel(0));
	ASSERT_VALID(pPanel);
	auto name = typeid (pPanel).name();
	if (pPanel->IsBackstageView())
	{
		pPanel->SetTopMargin(6);
		//pPanel->m_nTopMargin = 6;
	}
	else if (!m_pRibbonBar->IsScenicLook())
	{
		pPanel->SetTopMargin(rectBtn.Height() / 2 - 2);
		//pPanel->m_nTopMargin = rectBtn.Height() / 2 - 2;
	}
	else
	{
		pPanel->SetTopMargin(m_pRibbonBar->GetTabsHeight() - 1);
		//pPanel->m_nTopMargin = m_pRibbonBar->GetTabsHeight() - 1;
	}

	pPanel->SetXMargin(CBCGPVisualManager::GetInstance()->GetRibbonPanelMargin(pCategory));
	//pPanel->m_nXMargin = CBCGPVisualManager::GetInstance()->GetRibbonPanelMargin(pCategory);

	pPanel->SetMainButton(this);
	//pPanel->m_pMainButton = this;

	CClientDC dc(m_pRibbonBar);

	CFont* pOldFont = dc.SelectObject(pPanel->IsBackstageView() ? m_pRibbonBar->GetBackstageViewItemFont() : m_pRibbonBar->GetFont());
	ASSERT(pOldFont != NULL);

	pPanel->RecalcWidthsEx(&dc, 32767);

	dc.SelectObject(pOldFont);

	if (pPanel->IsKindOf(RUNTIME_CLASS(CBCGPRibbonBackstageViewPanel)))
	{
		if (!pRibbonBarEx->OnBeforeShowBackstageViewEx())
		{
			return FALSE;
		}

		CBCGPRibbonBackstageView* pView = new CBCGPRibbonBackstageView(m_pRibbonBar, pPanel);
		pView->Create();

		m_bIsDroppedDown = TRUE;
	}
	else
	{
		if (!pRibbonBarEx->OnBeforeShowMainPanelEx())
		{
			return FALSE;
		}
		CBCGPRibbonPanelMenu* pMenu = new CBCGPRibbonPanelMenu(pPanel);
		pMenu->SetParentRibbonElement(this);

		const int y = m_pRibbonBar->IsScenicLook() ? rectBtn.bottom - m_pRibbonBar->GetTabsHeight() - 1 : rectBtn.CenterPoint().y;

		pMenu->Create(m_pRibbonBar, bIsRTL ? rectBtn.right : rectBtn.left, y, (HMENU)NULL);

		SetDroppedDown(pMenu);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonSearchBox

class CBCGPRibbonSearchBox : public CBCGPRibbonEdit
{
	DECLARE_DYNCREATE(CBCGPRibbonSearchBox)

	// Construction:
public:
	CBCGPRibbonSearchBox() :
		CBCGPRibbonEdit(0, 0)
	{
		m_bDontScaleInHighDPI = TRUE;
	}

protected:
	virtual BOOL OnEditChange()
	{
		CBCGPRibbonMainPanel* pMainPanel = DYNAMIC_DOWNCAST(CBCGPRibbonMainPanel, GetParentPanel());
		if (pMainPanel != NULL)
		{
			pMainPanel->OnSearch(m_strEdit);
		}
		return FALSE;
	}

	virtual BOOL OnProcessKey(UINT nChar)
	{
		CBCGPRibbonMainPanel* pMainPanel = DYNAMIC_DOWNCAST(CBCGPRibbonMainPanel, GetParentPanel());
		if (pMainPanel != NULL && pMainPanel->GetHighlighted() != this)
		{
			CBCGPBaseRibbonElement* pHighlighted = pMainPanel->GetHighlighted();
			if (pHighlighted != NULL && pHighlighted->IsDroppedDown())
			{
				return FALSE;
			}

			return pMainPanel->OnSearchNavigate(nChar);
		}

		return FALSE;
	}
};

IMPLEMENT_DYNCREATE(CBCGPRibbonSearchBox, CBCGPRibbonEdit)

//////////////////////////////////////////////////////////////////////////////
// CBCGPRecentFileLabel

class CBCGPRecentFileLabel : public CBCGPRibbonLabel
{
	DECLARE_DYNCREATE(CBCGPRecentFileLabel)

	CBCGPRecentFileLabel()
	{
	}

	CBCGPRecentFileLabel(LPCTSTR lpszText) : CBCGPRibbonLabel(lpszText)
	{
	}

protected:

	virtual CSize GetRegularSize(CDC* pDC)
	{
		CSize size = CBCGPRibbonButton::GetRegularSize(pDC);
		size.cy = globalData.GetTextHeight() + 2 * LABEL_MARGIN_Y - 1;
		return size;
	}

	virtual void OnDraw(CDC* pDC)
	{
		ASSERT_VALID(this);
		ASSERT_VALID(pDC);

		if (m_rect.IsRectEmpty())
		{
			return;
		}

		CRect rectText = m_rect;
		rectText.DeflateRect(LABEL_MARGIN_X, 0);

		CBCGPRibbonBar* pTopLevelRibbon = GetTopLevelRibbonBar();

		CFont* pOldFont = pDC->SelectObject((pTopLevelRibbon->GetSafeHwnd() != NULL) ? pTopLevelRibbon->GetBoldFont() : &globalData.fontBold);
		ASSERT_VALID(pOldFont);

		DoDrawText(pDC, m_strText, rectText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

		pDC->SelectObject(pOldFont);

		CRect rectSeparator = m_rect;
		rectSeparator.top = rectSeparator.bottom - 2;

		if (m_pParentMenu != NULL)
		{
			CBCGPVisualManager::GetInstance()->OnDrawSeparator(pDC, m_pParentMenu, rectSeparator, FALSE);
		}
	}
};

IMPLEMENT_DYNCREATE(CBCGPRecentFileLabel, CBCGPRibbonLabel)

//////////////////////////////////////////////////////////////////////////////
// CBCGPRecentFileButton

class CBCGPRecentFileButton : public CBCGPRibbonButton
{
	friend class CBCGPRibbonRecentFilesList;

	DECLARE_DYNCREATE(CBCGPRecentFileButton)

public:
	CBCGPRecentFileButton()
	{
		m_bHasPin = FALSE;
		m_bIsPinned = FALSE;
		m_bPinIsDark = TRUE;
	}

protected:
	virtual BOOL CanBeAddedToQAT() const
	{
		return FALSE;
	}

	virtual CSize GetRegularSize(CDC* pDC)
	{
		CSize size = CBCGPRibbonButton::GetRegularSize(pDC);

		if (IsBackstageViewMode())
		{
			size.cy = 100;
		}
		else
		{
			size.cy = globalData.GetTextHeight() + 2 * LABEL_MARGIN_Y - 1;
		}
		return size;
	}

	virtual int DoDrawText(CDC* pDC, const CString& strText, CRect rectText, UINT uiDTFlags,
		COLORREF clrText = (COLORREF)-1)
	{
		rectText.DeflateRect(LABEL_MARGIN_X - 1, 0);
		rectText.right -= m_rectMenu.Width();

		return CBCGPRibbonButton::DoDrawText(pDC, strText, rectText, uiDTFlags, clrText);
	}

	virtual BOOL HasPin() const
	{
		return m_bHasPin;
	}

	virtual void CopyFrom(const CBCGPBaseRibbonElement& s)
	{
		ASSERT_VALID(this);

		CBCGPRibbonButton::CopyFrom(s);

		CBCGPRecentFileButton& src = (CBCGPRecentFileButton&)s;

		m_bHasPin = src.m_bHasPin;
		m_bIsPinned = src.m_bIsPinned;
	}

	virtual void OnDraw(CDC* pDC)
	{
		CRect rectSaved = m_rect;
		m_rect.DeflateRect(1, 1);

		CBCGPRibbonButton::OnDraw(pDC);

		m_rect = rectSaved;
	}

	virtual int GetDropDownImageWidth() const
	{
		return CBCGPVisualManager::GetInstance()->GetPinSize(TRUE).cx + 8;
	}

	virtual void OnDrawMenuArrow(CDC* pDC, const CRect& rectMenuArrow)
	{
		CSize sizePin = CBCGPVisualManager::GetInstance()->GetPinSize(m_bIsPinned);
		CRect rectPinImage(
			CPoint(
				rectMenuArrow.CenterPoint().x - sizePin.cx / 2,
				rectMenuArrow.CenterPoint().y - sizePin.cy / 2),
			sizePin);

		CBCGPVisualManager::GetInstance()->OnDrawPin(pDC, rectPinImage, m_bIsPinned, m_bPinIsDark,
			IsHighlighted(), IsPressed(), IsDisabled());
	}

	virtual void OnLButtonDown(CPoint point)
	{
		if (m_bHasPin && m_rectMenu.PtInRect(point))
		{
			return;
		}

		CBCGPRibbonButton::OnLButtonDown(point);
	}

	virtual void OnLButtonUp(CPoint point)
	{
		if (m_bHasPin && m_rectMenu.PtInRect(point))
		{
			m_bIsPinned = !m_bIsPinned;
			Redraw();

			if (GetWorkspace() != NULL)
			{
				GetWorkspace()->PinPath(TRUE, GetToolTip(), m_bIsPinned);
			}

			return;
		}

		CBCGPRibbonButton::OnLButtonUp(point);
	}

	virtual CString GetToolTipText() const
	{
		return CBCGPRibbonButton::GetToolTipText();
	}

	virtual BOOL IsDroppedDown() const
	{
		return FALSE;
	}

	virtual COLORREF OnFillBackground(CDC* pDC)
	{
		if (!m_bHasPin)
		{
			return CBCGPRibbonButton::OnFillBackground(pDC);
		}

		return CBCGPVisualManager::GetInstance()->OnFillRibbonPinnedButton(pDC, this, m_bPinIsDark);
	}

	virtual void OnDrawBorder(CDC* pDC)
	{
		if (!m_bHasPin)
		{
			CBCGPRibbonButton::OnDrawBorder(pDC);
			return;
		}

		CBCGPVisualManager::GetInstance()->OnDrawRibbonPinnedButtonBorder(pDC, this);
	}

	virtual BOOL NotifyCommand(BOOL bWithDelay = FALSE)
	{
		if (m_nID != PINNED_FILE_ID || AfxGetApp() == NULL)
		{
			return CBCGPRibbonButton::NotifyCommand(bWithDelay);
		}

		CString strFilePath = GetToolTip();

		if (bWithDelay)
		{
			CBCGPRibbonBar* pTopLevelRibbon = GetTopLevelRibbonBar();
			if (pTopLevelRibbon != NULL)
			{
				ASSERT_VALID(pTopLevelRibbon);
				pTopLevelRibbon->DoOpenPinnedFile(strFilePath);

				return TRUE;
			}
		}

		AfxGetApp()->OpenDocumentFile(strFilePath);
		return TRUE;
	}

	virtual BOOL QueryElements(const CStringArray& /*arWords*/, CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& /*arButtons*/, int /*nMaxResults*/, BOOL /*bDescription*/, BOOL /*bAll*/) { return FALSE; }

public:
	BOOL	m_bHasPin;
	BOOL	m_bIsPinned;
	BOOL	m_bPinIsDark;
};

IMPLEMENT_DYNCREATE(CBCGPRecentFileButton, CBCGPRibbonButton)

//////////////////////////////////////////////////////////////////////////////
// CBCGPRibbonMainPanelEx

CBCGPRibbonMainPanelEx::CBCGPRibbonMainPanelEx()
{

}

CBCGPRibbonMainPanelEx::~CBCGPRibbonMainPanelEx()
{

}

void CBCGPRibbonMainPanelEx::Repos(CDC* pDC, const CRect& rect)
{
	ASSERT_VALID(pDC);

	m_nTotalHeight = 0;

	CSize size = rect.Size();
	size.cx -= m_nXMargin;
	size.cy -= m_nYMargin;

	int nTopMargin = m_nTopMargin;

	if (IsBackstageView())
	{
		nTopMargin -= m_nScrollOffset;
	}

	int y = m_bSearchMode ? m_rectMenuElements.top : nTopMargin;
	int i = 0;

	const int nMenuElements = GetMenuElements();

	if (!m_bReposAfterSearch)
	{
		m_rectMenuElements = rect;
		m_rectMenuElements.OffsetRect(-m_nScrollOffsetHorz, 0);

		if (!IsBackstageView())
		{
			m_rectMenuElements.DeflateRect(m_nXMargin, m_nYMargin);
			m_rectMenuElements.top += m_nTopMargin;
		}
	}

	int nImageWidth = 0;

	if (m_pParent != NULL)
	{
		ASSERT_VALID(m_pParent);
		nImageWidth = m_pParent->GetImageSize(TRUE).cx;
	}

	if (m_bSearchMode)
	{
		for (i = 0; i < nMenuElements; i++)
		{
			CBCGPBaseRibbonElement* pElem = m_arElements[i];
			ASSERT_VALID(pElem);

			pElem->SetRect(CRect(0, 0, 0, 0));
		}
	}

	//----------------------------------------
	// Repos menu elements (on the left side):
	//----------------------------------------
	int nColumnWidth = m_nSearchBoxWidth;
	int iMenuStart = m_bSearchMode ? (int)m_arElements.GetSize() - m_nSearchResults : 0;
	int iMenuFinish = m_bSearchMode ? (int)m_arElements.GetSize() : nMenuElements;

	for (i = iMenuStart; i < iMenuFinish; i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements[i];
		ASSERT_VALID(pElem);

		if (pElem->IsBackstageViewMode() && pElem->HasSubitems())
		{
			//	pElem->m_nExtraMaginX = BACKSTAGE_BUTTON_MARGIN_X;
		}

		pElem->OnCalcTextSize(pDC);
		pElem->SetTextAlwaysOnRight();

		CSize sizeElem = pElem->GetSize(pDC);

		if (sizeElem == CSize(0, 0))
		{
			pElem->SetRect(CRect(0, 0, 0, 0));
			continue;
		}

		if (pElem->IsBackstageViewMode() && !pElem->HasSubitems())
		{
			sizeElem.cx += 2 * BACKSTAGE_BUTTON_MARGIN_X;
		}

		CRect rectElem = CRect
		(CPoint(rect.left - m_nScrollOffsetHorz + m_nXMargin, rect.top + y + m_nYMargin),
			sizeElem);

		if (m_bSearchMode && rectElem.bottom > m_rectMenuElements.bottom)
		{
			rectElem = CRect(0, 0, 0, 0);
		}
		else if (pElem->IsBackstageViewMode() && !pElem->HasSubitems())
		{
			rectElem.DeflateRect(BACKSTAGE_BUTTON_MARGIN_X, 0);
		}

		pElem->SetRect(rectElem);

		nColumnWidth = max(nColumnWidth, sizeElem.cx);
		y += sizeElem.cy;

		if (IsBackstageView())
		{
			y += 2;
		}

	}

	nColumnWidth += 2 * m_nXMargin;

	if (!m_bReposAfterSearch)
	{
		m_rectMenuElements.right = m_rectMenuElements.left + nColumnWidth;

		if (IsBackstageView())
		{
			m_rectMenuElements.right += 2 * m_nXMargin;
		}
		else
		{
			m_rectMenuElements.bottom = y + m_nYMargin;
			m_rectMenuElements.InflateRect(1, 1);
		}

		m_nFullWidth = nColumnWidth + 2 * m_nXMargin;
	}

	//----------------------------------------------
	// All menu elements should have the same width:
	//----------------------------------------------
	for (i = iMenuStart; i < iMenuFinish; i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements[i];
		ASSERT_VALID(pElem);

		CRect rectElem = pElem->GetRect();

		if (!rectElem.IsRectEmpty())
		{
			if (m_bSearchMode)
			{
				rectElem.right = m_rectMenuElements.right;
			}
			else
			{
				rectElem.right = rectElem.left + nColumnWidth;

				if (pElem->IsBackstageViewMode() && !pElem->HasSubitems())
				{
					rectElem.right -= 2 * BACKSTAGE_BUTTON_MARGIN_X;
				}
			}

			if (nImageWidth > 0 &&
				pElem->IsKindOf(RUNTIME_CLASS(CBCGPRibbonSeparator)))
			{
				rectElem.left += nImageWidth + LABEL_MARGIN_Y;
			}

			pElem->SetRect(rectElem);
		}
	}

	//----------------------
	// Put element on right:
	//----------------------
	if (m_pElemOnRight != NULL && !m_bSearchMode)
	{
		CBCGPRibbonRecentFilesListEx* pRecentList =
			DYNAMIC_DOWNCAST(CBCGPRibbonRecentFilesListEx, m_pElemOnRight);
		auto name = typeid (pRecentList).name();
		if (pRecentList != NULL)
		{
			ASSERT_VALID(pRecentList);

			if (pRecentList->GetCount() == 0)
			{
				pRecentList->FillList();
			}
		}

		m_pElemOnRight->SetInitialMode();
		m_pElemOnRight->OnCalcTextSize(pDC);

		CSize sizeRecentList = m_pElemOnRight->GetSize(pDC);

		int nDefaultWidth = globalUtils.ScaleByDPI(m_nRightPaneWidth);

		sizeRecentList.cx = max(sizeRecentList.cx, nDefaultWidth);

		if (m_rectMenuElements.Height() < sizeRecentList.cy && !IsBackstageView())
		{
			m_rectMenuElements.bottom = m_rectMenuElements.top + sizeRecentList.cy;
		}

		CRect rectRecentList = CRect
		(m_rectMenuElements.right, m_rectMenuElements.top,
			m_rectMenuElements.right + sizeRecentList.cx, m_rectMenuElements.bottom);

		if (pRecentList == NULL)
		{
			rectRecentList.DeflateRect(0, 1);
		}

		m_pElemOnRight->SetRect(rectRecentList);

		if (!m_bReposAfterSearch)
		{
			m_nFullWidth += sizeRecentList.cx;
		}
	}

	//---------------------------------
	// Put "bottom" elements on bottom:
	//---------------------------------
	if (m_nBottomElementsNum > 0 && !m_bSearchMode)
	{
		const int nXMargin = max(m_nXMargin, 2);

		int x = rect.left + m_nFullWidth - nXMargin;
		int nRowHeight = 0;

		y = m_rectMenuElements.bottom + m_nYMargin;

		int xLeft = rect.left + nXMargin;

		if (m_pSearchBox != NULL)
		{
			ASSERT_VALID(m_pSearchBox);

			m_pSearchBox->OnCalcTextSize(pDC);

			int xMargin = m_pParent == NULL ? 0 : max(0, 4 - CBCGPVisualManager::GetInstance()->GetRibbonPanelMargin(m_pParent));

			m_pSearchBox->SetWidth(m_rectMenuElements.Width() - xMargin);

			CSize sizeElem = m_pSearchBox->GetSize(pDC);
			sizeElem.cx = m_rectMenuElements.Width() - xMargin;

			CRect rectElem = CRect(CPoint(m_rectMenuElements.left + xMargin, y), sizeElem);
			m_pSearchBox->SetRect(rectElem);

			nRowHeight = max(nRowHeight, sizeElem.cy);
			xLeft = m_rectMenuElements.left + sizeElem.cx;
		}

		for (int nCount = 0; nCount < m_nBottomElementsNum; nCount++)
		{
			int nIndex = (int)m_arElements.GetSize() - nCount - 1 - m_nSearchResults;

			CBCGPBaseRibbonElement* pElem = m_arElements[nIndex];
			ASSERT_VALID(pElem);

			if (pElem == m_pSearchBox)
			{
				// Already positioned
				continue;
			}

			pElem->OnCalcTextSize(pDC);

			CSize sizeElem = pElem->GetSize(pDC);

			if (sizeElem == CSize(0, 0))
			{
				pElem->SetRect(CRect(0, 0, 0, 0));
				continue;
			}

			sizeElem.cx += LABEL_MARGIN_Y - 1;

			if (x - sizeElem.cx < xLeft)
			{
				x = rect.left + m_nFullWidth - nXMargin;
				y += nRowHeight;
				nRowHeight = 0;
				xLeft = rect.left + nXMargin;
			}

			CRect rectElem = CRect(CPoint(x - sizeElem.cx, y), sizeElem);
			pElem->SetRect(rectElem);

			nRowHeight = max(nRowHeight, sizeElem.cy);
			x = rectElem.left - LABEL_MARGIN_Y;
		}

		y += nRowHeight;
	}
	else if (m_nBottomElementsNum == 0)
	{
		y += globalData.GetTextHeight();
	}

	for (i = 0; i < m_arElements.GetSize(); i++)
	{
		CBCGPBaseRibbonElement* pElem = m_arElements[i];
		ASSERT_VALID(pElem);

		pElem->OnAfterChangeRect(pDC);
	}

	m_nTotalHeight = y + m_nScrollOffset;

	if (!m_bReposAfterSearch)
	{
		m_rect = rect;

		if (IsBackstageView())
		{
			m_nFullWidth = m_rect.Width();
			m_rectMenuElements.bottom = m_rect.bottom + 3;
		}
		else
		{
			m_rect.bottom = m_rect.top + y + m_nYMargin;
			m_rect.right = m_rect.left + m_nFullWidth + m_nXMargin;
		}
	}
}

void CBCGPRibbonMainPanelEx::AddRecentFilesList(LPCTSTR lpszLabel, int nWidth /*= 300*/, BOOL bShowPins /*= FALSE*/)
{
	ASSERT_VALID(this);
	ASSERT(lpszLabel != NULL);

	if (IsBackstageView())
	{
		ASSERT(FALSE);
		return;
	}

	AddToRight(new CBCGPRibbonRecentFilesListEx(lpszLabel, bShowPins), nWidth);
}

IMPLEMENT_DYNCREATE(CBCGPRibbonMainPanelEx, CBCGPRibbonMainPanel)

//////////////////////////////////////////////////////////////////////////////
//  CBCGPRibbonRecentFilesListEx
CBCGPRibbonRecentFilesListEx::CBCGPRibbonRecentFilesListEx(LPCTSTR lpszLabel /*= NULL*/, BOOL bShowPins /*= FALSE*/)
	:CBCGPRibbonRecentFilesList(lpszLabel, bShowPins)
{

}

CBCGPRibbonRecentFilesListEx::~CBCGPRibbonRecentFilesListEx()
{

}

static CString PrepareDisplayName(int nIndex, const CString & strName)
{
	CString strOut;
	strOut = strName;
	if (nIndex == 9)
	{
		strOut.Format(_T("10. %s"), strName);
	}
	else if (nIndex < 9)
	{
		strOut.Format(_T("%d.  %s"), nIndex + 1, strName);
	}
	else
	{
		strOut = strName;
	}

	return strOut;
}

void CBCGPRibbonRecentFilesListEx::FillList()
{
	ASSERT_VALID(this);

	RemoveAll();

	//-----------
	// Add label:
	//-----------
	AddButton(new CBCGPRecentFileLabel(m_strText));

	int iNumOfFiles = 0;	// Actual added to menu

	TCHAR szCurDir[_MAX_PATH + 1];
	::GetCurrentDirectory(_MAX_PATH, szCurDir);

	int nCurDir = lstrlen(szCurDir);
	ASSERT(nCurDir >= 0);

	szCurDir[nCurDir] = _T('\\');
	szCurDir[++nCurDir] = _T('\0');

	//---------------
	// Add MRU items:
	//---------------
	for (int i = 0; i < CBCGPWinApp::_GetRecentFilesCount(); i++)
	{
		CString strName;
		if (CBCGPWinApp::_GetRecentFileDisplayName(strName, i, szCurDir, nCurDir))
		{
			CString strPath = CBCGPWinApp::_GetRecentFilePath(i);
			strName = BwProj::toolkit::CFileToolkit::GetFileLogicName(strPath);
			BOOL bAlreadyExist = FALSE;

			for (int j = 0; j < m_arButtons.GetSize(); j++)
			{
				if (strPath.CompareNoCase(m_arButtons[j]->GetToolTip()) == 0)
				{
					bAlreadyExist = TRUE;
					break;
				}
			}

			if (!bAlreadyExist)
			{
				CBCGPRecentFileButton* pFile = new CBCGPRecentFileButton;

				pFile->SetText(PrepareDisplayName(iNumOfFiles, strName));
				pFile->SetID(ID_FILE_MRU_FILE1 + i);
				pFile->SetToolTipText(strPath);
				pFile->m_bHasPin = m_bShowPins;

				AddButton(pFile);

				iNumOfFiles++;
			}
		}
	}
}
IMPLEMENT_DYNCREATE(CBCGPRibbonRecentFilesListEx, CBCGPRibbonRecentFilesList)