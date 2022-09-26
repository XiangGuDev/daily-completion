#include "stdafx.h"
#include "BCGPRibbonBarEx.h"
#include <BCGPRibbonInfo.h>
#include <BCGPRibbonInfoLoader.h>
#include <BCGPRibbonConstructor.h>
#include <BCGPRibbonCategory.h>
#include "BCGPRibbonConstructorEx.h"

static const UINT IdAutoCommand = 1;

controls::CBCGPRibbonBarEx::CBCGPRibbonBarEx()
{
	m_bAutoDestroyMainButton = TRUE;
}

controls::CBCGPRibbonBarEx::~CBCGPRibbonBarEx()
{
	
}

BOOL  controls::CBCGPRibbonBarEx::OnShowRibbonContextMenu(CWnd* pWnd, int x, int y, CBCGPBaseRibbonElement* pHit)
{
	return FALSE;
	DeactivateKeyboardFocus();

	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);

	if (m_bAutoCommandTimer)
	{
		KillTimer(IdAutoCommand);
		m_bAutoCommandTimer = FALSE;
	}

	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0 &&
		(GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0)
	{
		return FALSE;
	}

	if (g_pContextMenuManager == NULL)
	{
		return FALSE;
	}

	if (m_bDlgBarMode)
	{
		return FALSE;
	}

	if (m_bBackstageViewActive)
	{
		return FALSE;
	}

	if (pHit != NULL)
	{
		ASSERT_VALID(pHit);

		if (!pHit->IsHighlighted())
		{
			pHit->Redraw();
		}
	}

	CBCGPPopupMenu* pPopupMenu = DYNAMIC_DOWNCAST(CBCGPPopupMenu, pWnd->GetParent());

	CFrameWnd* pParentFrame = GetParentFrame();
	ASSERT_VALID(pParentFrame);

	const UINT idCustomizeQAT = (UINT)-102;
	const UINT idQATOnBottom = (UINT)-103;
	const UINT idQATOnTop = (UINT)-104;
	const UINT idAddToQAT = (UINT)-105;
	const UINT idRemoveFromQAT = (UINT)-106;
	const UINT idMinimize = (UINT)-107;
	const UINT idRestore = (UINT)-108;
	const UINT idCustomizeRibbon = (UINT)-109;

	CMenu menu;
	menu.CreatePopupMenu();

	BOOL bIsGalleryMenu = FALSE;
	CString strItem;
	if (!bIsGalleryMenu)
	{
		if (menu.GetMenuItemCount() > 0)
		{
			menu.AppendMenu(MF_SEPARATOR);
		}

		menu.AppendMenu(MF_STRING, idCustomizeRibbon, _T("自定义功能区"));
	}

	HWND hwndThis = pWnd->GetSafeHwnd();

	if (pPopupMenu != NULL)
	{
		g_pContextMenuManager->SetDontCloseActiveMenu();
	}

	OnBeforeShowContextMenu(menu);

	int nMenuResult = g_pContextMenuManager->TrackPopupMenu(
		menu, x, y, pWnd);

	if (pPopupMenu != NULL)
	{
		g_pContextMenuManager->SetDontCloseActiveMenu(FALSE);
	}

	if (!::IsWindow(hwndThis))
	{
		return FALSE;
	}

	if (pHit != NULL)
	{
		ASSERT_VALID(pHit);

		CBCGPBaseRibbonElement* pDroppedDown = pHit->GetDroppedDown();

		if (pDroppedDown != NULL)
		{
			ASSERT_VALID(pDroppedDown);

			pDroppedDown->ClosePopupMenu();
		}

		pHit->Redraw();
	}

	BOOL bRecalcLayout = FALSE;
	BOOL bUpdateAllChildren = FALSE;

	int nQATHeight = m_QAToolbar.GetRect().Height();

	switch (nMenuResult)
	{
	case idCustomizeQAT:
	case idCustomizeRibbon:
	{
		if (pHit == m_pHighlighted)
		{
			m_pHighlighted = NULL;
		}

		if (pHit == m_pPressed)
		{
			m_pPressed = NULL;
		}

		if (pPopupMenu != NULL)
		{
			pPopupMenu->SendMessage(WM_CLOSE);
		}

		WPARAM wp = (nMenuResult == idCustomizeQAT) ? 0 : 1;
		pParentFrame->SendMessage(BCGM_ON_RIBBON_CUSTOMIZE, wp, (LPARAM)this);
		return TRUE;
	}
	break;

	case idAddToQAT:
	{
		ASSERT_VALID(pHit);

		bRecalcLayout = pHit->OnAddToQAToolbar(m_QAToolbar);
	}
	break;

	case idRemoveFromQAT:
		ASSERT_VALID(pHit);

		if (pHit == m_pHighlighted)
		{
			m_pHighlighted = NULL;
		}

		if (pHit == m_pPressed)
		{
			m_pPressed = NULL;
		}
		bRecalcLayout = TRUE;
		break;

	case idQATOnBottom:
		SetQuickAccessToolbarOnTop(FALSE);
		bRecalcLayout = TRUE;
		bUpdateAllChildren = TRUE;
		break;

	case idQATOnTop:
		SetQuickAccessToolbarOnTop(TRUE);
		bRecalcLayout = TRUE;
		bUpdateAllChildren = TRUE;
		break;

	case idMinimize:
		if (m_pActiveCategory != NULL)
		{
			ASSERT_VALID(m_pActiveCategory);
			RedrawWindow();
		}
		break;

	case idRestore:
		if (m_pActiveCategory != NULL)
		{
			ASSERT_VALID(m_pActiveCategory);
			RedrawWindow();
		}
		break;

	default:
		if (m_bIsCustomizeMenu)
		{
			UINT uiCmd = -nMenuResult;

			if (uiCmd != 0)
			{
				CBCGPBaseRibbonElement* pElement = FindByID(uiCmd, FALSE);
				if (pElement != NULL)
				{
					ASSERT_VALID(pElement);
					bRecalcLayout = TRUE;
					break;
				}
			}
		}
		else if (nMenuResult != 0)
		{
			if (pParentFrame->GetSafeHwnd() != NULL)
			{
				if (pPopupMenu != NULL)
				{
					pPopupMenu->SendMessage(WM_CLOSE);
					pPopupMenu = NULL;
				}

				pParentFrame->PostMessage(WM_COMMAND, nMenuResult);
			}
		}

		return FALSE;
	}

	if (pPopupMenu != NULL)
	{
		pPopupMenu->SendMessage(WM_CLOSE);
	}

	if (bRecalcLayout)
	{
		m_bForceRedraw = TRUE;
		RecalcLayout();

		if (pParentFrame->GetSafeHwnd() != NULL)
		{
			pParentFrame->RecalcLayout();

			UINT nFlags = RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW;
			if (bUpdateAllChildren)
			{
				nFlags |= RDW_ALLCHILDREN;
			}
			else
			{
				if (nQATHeight != m_QAToolbar.GetRect().Height())
				{
					nFlags |= RDW_ALLCHILDREN;
				}
			}

			pParentFrame->RedrawWindow(NULL, NULL, nFlags);
		}
	}

	return TRUE;
}

CBCGPRibbonMainPanelEx* controls::CBCGPRibbonBarEx::AddMainCategoryEx(LPCTSTR lpszName, UINT uiSmallImagesResID, UINT uiLargeImagesResID, CSize sizeSmallImage /*= CSize(16, 16)*/, CSize sizeLargeImage /*= CSize(32, 32)*/, CRuntimeClass* pRTI /*= NULL*/)
{	
	ASSERT_VALID(this);

	CBCGPRibbonBar::AddMainCategory(lpszName, uiSmallImagesResID, uiLargeImagesResID, sizeSmallImage, sizeLargeImage, pRTI);

	if (m_pMainCategory == NULL)
	{
		ASSERT_VALID(FALSE);
	}
	m_pMainCategory->RemovePanel(0);
	//CBCGPRibbonMainPanelEx* pPanel = new CBCGPRibbonMainPanelEx();
	auto pPanel = m_pMainCategory->AddPanel(lpszName, 0, RUNTIME_CLASS(CBCGPRibbonMainPanelEx));
	//DYNAMIC_DOWNCAST(CBCGPRibbonMainPanelEx)
	auto pPanel2 = dynamic_cast<CBCGPRibbonMainPanelEx*>(pPanel);
	return pPanel2;
}

void controls::CBCGPRibbonBarEx::SetmAutoDestroy(BOOL bAutoDestory)
{
	m_bAutoDestroyMainButton = bAutoDestory;
}

void controls::CBCGPRibbonBarEx::RecalcLayout()
{
	__super::RecalcLayout();

	auto pAccessToolbar = GetQuickAccessToolbar();
	int iAccessToolbarBtnCnt = pAccessToolbar->GetCount();
	if (iAccessToolbarBtnCnt <= 0)
		return;

	auto pBtn = pAccessToolbar->GetButton(iAccessToolbarBtnCnt - 1);
	if (pBtn != NULL)
	{
		pBtn->SetRect(CRect(0, 0, 0, 0));
		pAccessToolbar->Redraw();
	}
}
