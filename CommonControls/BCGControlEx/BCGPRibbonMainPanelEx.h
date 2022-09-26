/*******************************************************************
* 文件名称：BCGPRibbonMainPanelEx.h
* 作者姓名：徐立
* 文件描述：创建自定义菜单栏
* 需求版本：
* 创建时间：2019-11-1
* Copyright (C) 江西博微新技术有限公司 All rights reserved
*******************************************************************/

#pragma once
#include "BCGPRibbonBarEx.h"

/////////////////////////////
// CBCGPRibbonMainButtonEx  主菜单按钮扩展类
class CONTROLSAPI_EXPORT CBCGPRibbonMainButtonEx : public CBCGPRibbonMainButton
{
public:
	CBCGPRibbonMainButtonEx();
	~CBCGPRibbonMainButtonEx();

protected:
	virtual void OnLButtonDown(CPoint point);
	BOOL ShowMainMenuEx();
};



class CONTROLSAPI_EXPORT CBCGPRibbonMainPanelEx : public CBCGPRibbonMainPanel
{
	friend class CBCGPRibbonMainButton;
	friend class CBCGPRibbonCollector;
	friend class CBCGPRibbonSearchBox;
	friend class CBCGPBaseRibbonElement;

	DECLARE_DYNCREATE(CBCGPRibbonMainPanelEx)
	// Construction
public:
	CBCGPRibbonMainPanelEx();
	virtual ~CBCGPRibbonMainPanelEx();

	virtual void Repos(CDC* pDC, const CRect& rect);

	void AddRecentFilesList(LPCTSTR lpszLabel, int nWidth = 300, BOOL bShowPins = FALSE);

	void SetTopMargin(int nMargin)
	{
		m_nTopMargin = nMargin;
	}

	void SetXMargin(int nMargin)
	{
		m_nXMargin = nMargin;
	}

	void SetMainButton(CBCGPRibbonMainButton* pBtn)
	{
		m_pMainButton = pBtn;
	}

	void RecalcWidthsEx(CDC* pDC, int nHeight)
	{
		RecalcWidths(pDC, nHeight);
	}

	void SetSizeIsLocked(BOOL bSizeIsLocked)
	{
		m_bSizeIsLocked = bSizeIsLocked;
	}

	void SetScrollOffset(int nScrollOffset)
	{
		m_nScrollOffset = nScrollOffset;
	}

	void OnAfterChangeRectEx(CDC* pDC)
	{
		OnAfterChangeRect(pDC);
	}
};

class CBCGPRibbonRecentFilesListEx : public CBCGPRibbonRecentFilesList
{
	DECLARE_DYNCREATE(CBCGPRibbonRecentFilesListEx)
	// Construction
public:
	CBCGPRibbonRecentFilesListEx(LPCTSTR lpszLabel = NULL, BOOL bShowPins = FALSE);
	virtual ~CBCGPRibbonRecentFilesListEx();

	virtual void FillList();
};
