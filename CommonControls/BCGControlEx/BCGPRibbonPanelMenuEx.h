/*******************************************************************
* 文件名称：BCGPRibbonPanelMenuEx.h
* 作者姓名：徐立
* 文件描述：创建自定义菜单栏
* 需求版本：
* 创建时间：2019-11-1
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/
#pragma once

#include "bcgcbpro.h"
#include "BCGPPopupMenu.h"
#include "BCGPPopupMenuBar.h"
#include "BCGPRibbonPanel.h"
#include "BCGPScrollBar.h"
#include <BCGPRibbonPanelMenu.h>

class CBCGPRibbonButton;
class CBCGPRibbonPaletteButton;
class CBCGPRibbonCategory;

class CBCGPRibbonPanelMenuBarEx : public CBCGPRibbonPanelMenuBar
{
	// Construction
public:
	CBCGPRibbonPanelMenuBarEx(CBCGPRibbonPanel* pPanel);

	CBCGPRibbonPanelMenuBarEx(CBCGPRibbonBar* pRibbonBar,
		const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>&	arButtons, BOOL bIsFloatyMode = FALSE);

	CBCGPRibbonPanelMenuBarEx(CBCGPRibbonPaletteButton* pPaletteButton);

	CBCGPRibbonPanelMenuBarEx(CBCGPRibbonCategory* pCategory, CSize size);

	virtual ~CBCGPRibbonPanelMenuBarEx();

	void OnVScrollEx(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		OnVScroll(nSBCode, nPos, pScrollBar);
	}

protected:
	CBCGPRibbonPanelMenuBarEx();
	virtual void AdjustLocations();
};

////////////////////////////////////////////////////////////////////////////\
// CBCGPRibbonPanelMenu window

class CBCGPRibbonPanelMenuEx : public CBCGPRibbonPanelMenu
{
	friend class CBCGPRibbonPanelMenuBarEx;
	friend class CBCGPRibbonPanel;

	DECLARE_DYNAMIC(CBCGPRibbonPanelMenuEx)

	// Construction
public:
	CBCGPRibbonPanelMenuEx(CBCGPRibbonPanel* pPanel);

	CBCGPRibbonPanelMenuEx(
		CBCGPRibbonBar* pRibbonBar,
		const CArray<CBCGPBaseRibbonElement*, CBCGPBaseRibbonElement*>&	arButtons,
		BOOL bIsFloatyMode = FALSE);

	CBCGPRibbonPanelMenuEx(
		CBCGPRibbonPaletteButton* pPaletteButton);

	CBCGPRibbonPanelMenuEx(
		CBCGPRibbonCategory* pCategory, CSize size);


	// Attributes
public:
	void SetMenuMode(BOOL bIsMenuMode = TRUE)
	{
		m_wndRibbonBar.SetMenuMode(bIsMenuMode);
	}

	BOOL IsMenuMode() const
	{
		return m_wndRibbonBar.IsMenuMode();
	}

	void SetDefaultMenuLook(BOOL bIsDefaultMenuLook = TRUE)
	{
		m_wndRibbonBar.SetDefaultMenuLook(bIsDefaultMenuLook);
	}

	BOOL IsDefaultMenuLook() const
	{
		return m_wndRibbonBar.IsDefaultMenuLook();
	}

// 	CBCGPRibbonPanel* GetPanel() const
// 	{
// 		return m_wndRibbonBar.m_pPanel;
// 	}

// 	CBCGPRibbonCategory* GetCategory() const
// 	{
// 		return m_wndRibbonBar.m_pCategory;
// 	}
// 
// 	void SetPreferedSize(CSize size)
// 	{
// 		m_wndRibbonBar.SetPreferedSize(size);
// 	}

	CBCGPRibbonPanelMenuBar* GetParentRibbonMenuBar() const;

// 	void EnableCustomizeMenu(BOOL bEnable = TRUE)
// 	{
// 		m_wndRibbonBar.m_bCustomizeMenu = bEnable;
// 	}
// 
// 	BOOL IsCustomizeMenu() const
// 	{
// 		return m_wndRibbonBar.m_bCustomizeMenu;
// 	}

protected:
	CBCGPRibbonPanelMenuBarEx	m_wndRibbonBar;
	BOOL					m_bForceClose;
	BOOL					m_bInScroll;

	// Operations
public:
	virtual void SetActive(BOOL /*bIsActive*/) {}

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBCGPRibbonPanelMenu)
	//}}AFX_VIRTUAL

	virtual CBCGPPopupMenuBar* GetMenuBar()
	{
		return &m_wndRibbonBar;
	}

	virtual BOOL IsAlwaysClose() const;

	CBCGPBaseRibbonElement* FindByOrigin(CBCGPBaseRibbonElement* pOrigin) const
	{
		return m_wndRibbonBar.FindByOrigin(pOrigin);
	}

	virtual BOOL DefaultMouseClickOnClose() const
	{
		return m_wndRibbonBar.IsFloaty() && !m_wndRibbonBar.IsQATPopup();
	}

	virtual BOOL IsParentEditFocused();
	virtual BOOL IsDropListMode();

protected:
	virtual int GetBorderSize() const;

	// Implementation
public:
	virtual ~CBCGPRibbonPanelMenuEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBCGPRibbonPanelMenu)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////\
