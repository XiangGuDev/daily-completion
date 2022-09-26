
#include "stdafx.h"
#include "trackmouse.h"
#include "BCGPRibbonPanelMenu.h"
#include "BCGPRibbonPanel.h"
#include "BCGPRibbonCategory.h"
#include "BCGPRibbonBar.h"
#include "BCGPVisualManager.h"
#include "BCGPTooltipManager.h"
#include "BCGPToolTipCtrl.h"
#include "BCGPRibbonPaletteButton.h"
#include "BCGPRibbonFloaty.h"
#include "BCGPRibbonEdit.h"
#include "BCGPRibbonPanelMenuEx.h"
#include "BCGPRibbonConstructorEx.h"


static const int uiPopupTimerEvent = 1;
static const int uiRemovePopupTimerEvent = 2;
static const UINT IdAutoCommand = 3;
static const int nScrollBarID = 1;
static const int nScrollBarHorzID = 2;

////////////////////////////////////////////////////////////////////////////\
// CBCGPRibbonPanelMenuEx

IMPLEMENT_DYNAMIC(CBCGPRibbonPanelMenuEx, CBCGPRibbonPanelMenu)

CBCGPRibbonPanelMenuEx::CBCGPRibbonPanelMenuEx(CBCGPRibbonPanel* pPanel) :
	m_wndRibbonBar(pPanel)
{
	m_bForceClose = FALSE;
	m_bInScroll = FALSE;
}

CBCGPRibbonPanelMenuEx::CBCGPRibbonPanelMenuEx(
	CBCGPRibbonBar* pRibbonBar,
	const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>&	arButtons, BOOL bIsFloatyMode) :
	m_wndRibbonBar(pRibbonBar, arButtons, bIsFloatyMode)
{
	m_bForceClose = FALSE;
	m_bInScroll = FALSE;
}

CBCGPRibbonPanelMenuEx::CBCGPRibbonPanelMenuEx(
	CBCGPRibbonPaletteButton* pPaletteButton) :
	m_wndRibbonBar(pPaletteButton)
{
	
}

CBCGPRibbonPanelMenuEx::CBCGPRibbonPanelMenuEx(
	CBCGPRibbonCategory* pCategory, CSize size) :
	m_wndRibbonBar(pCategory, size)
{
	m_bForceClose = FALSE;
	m_bInScroll = FALSE;
}

CBCGPRibbonPanelMenuEx::~CBCGPRibbonPanelMenuEx()
{
}

BEGIN_MESSAGE_MAP(CBCGPRibbonPanelMenuEx, CBCGPRibbonPanelMenu)
	//{{AFX_MSG_MAP(CBCGPRibbonPanelMenuEx)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBCGPRibbonPanelMenuBar* CBCGPRibbonPanelMenuEx::GetParentRibbonMenuBar() const
{
	ASSERT_VALID(this);

	CBCGPPopupMenu* pParentMenu = GetParentPopupMenu();
	if (pParentMenu == NULL)
	{
		return NULL;
	}

	ASSERT_VALID(pParentMenu);

	return DYNAMIC_DOWNCAST(CBCGPRibbonPanelMenuBar,
		pParentMenu->GetMenuBar());
}
//****************************************************************************************
void CBCGPRibbonPanelMenuEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	ASSERT_VALID(this);

	if (!m_wndRibbonBar.OnKey(nChar))
	{
		CBCGPPopupMenu::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}
//**************************************************************************
BOOL CBCGPRibbonPanelMenuEx::IsAlwaysClose() const
{
	return m_bForceClose || ((CBCGPRibbonPanelMenuEx*) this)->m_wndRibbonBar.IsMainPanel();
}
//**************************************************************************
int CBCGPRibbonPanelMenuEx::GetBorderSize() const
{
	return IsMenuMode() ?
		CBCGPPopupMenu::GetBorderSize() :
		CBCGPVisualManager::GetInstance()->GetRibbonPopupBorderSize(this);
}
//**************************************************************************
BOOL CBCGPRibbonPanelMenuEx::IsParentEditFocused()
{
	CBCGPPopupMenu* pParentMenu = GetParentPopupMenu();
	if (pParentMenu == NULL)
	{
		return CBCGPPopupMenu::IsParentEditFocused();
	}

	return pParentMenu->IsParentEditFocused();
}
//**************************************************************************
BOOL CBCGPRibbonPanelMenuEx::IsDropListMode()
{
	CBCGPPopupMenu* pParentMenu = GetParentPopupMenu();
	if (pParentMenu == NULL)
	{
		return CBCGPPopupMenu::IsDropListMode();
	}

	return pParentMenu->IsDropListMode();
}

////////////////////////////////////////////////////////////////////////////\
// CBCGPRibbonPanelMenuEx message handlers

void CBCGPRibbonPanelMenuEx::OnDestroy()
{
	if (m_bEscClose && m_wndRibbonBar.GetCategory() != NULL &&
		BCGCBProGetTopLevelFrame(&m_wndRibbonBar) != NULL)
	{
		BCGCBProGetTopLevelFrame(&m_wndRibbonBar)->SetFocus();
	}

	CBCGPPopupMenu::OnDestroy();

}

CBCGPRibbonPanelMenuBarEx::CBCGPRibbonPanelMenuBarEx(CBCGPRibbonPanel* pPanel) : CBCGPRibbonPanelMenuBar(pPanel)
{

}

CBCGPRibbonPanelMenuBarEx::CBCGPRibbonPanelMenuBarEx(CBCGPRibbonBar* pRibbonBar, const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>& arButtons, BOOL bIsFloatyMode /*= FALSE*/)
	: CBCGPRibbonPanelMenuBar(pRibbonBar, arButtons, bIsFloatyMode)
{

}

CBCGPRibbonPanelMenuBarEx::CBCGPRibbonPanelMenuBarEx(CBCGPRibbonPaletteButton* pPaletteButton)
	: CBCGPRibbonPanelMenuBar(pPaletteButton)
{

}

CBCGPRibbonPanelMenuBarEx::CBCGPRibbonPanelMenuBarEx(CBCGPRibbonCategory* pCategory, CSize size)
	: CBCGPRibbonPanelMenuBar(pCategory, size)
{

}

CBCGPRibbonPanelMenuBarEx::CBCGPRibbonPanelMenuBarEx()
{

}

void CBCGPRibbonPanelMenuBarEx::AdjustLocations()
{
	__super::AdjustLocations();

}

CBCGPRibbonPanelMenuBarEx::~CBCGPRibbonPanelMenuBarEx()
{

}
