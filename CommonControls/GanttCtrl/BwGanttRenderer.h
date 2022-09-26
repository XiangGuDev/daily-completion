//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           BwGanttRenderer.h
//    @author         yf
//    @data           2020/12/16  
//    @brief          ¸ÊÌØÍ¼Grid¿Ø¼þ
//
//*****************************************************

#include "BCGPGanttRenderer.h"
#include "BwGanttItem.h"

class CBwGanttItem;

class CBwGanttDrawContext
{
public:
	CBwGanttDrawContext(const CBwGanttItem& item, CDC& dc);

	CDC&					m_DC;
	CRect					m_rectClip;
	CRect					m_rectBar;
	COLORREF				m_clrFill;
	COLORREF				m_clrFill2;

	const CBwGanttItem&   m_Item;
	CRect					m_rectRealBar;
	COLORREF				m_clrRealFill;
	COLORREF				m_clrRealFill2;
	COLORREF				m_clrBorder;
};

class  CBwGanttRenderer : public CBCGPGanttRenderer
{

public:
	virtual void DrawBar(CBwGanttDrawContext& ctxDraw);
	virtual void DrawSelection(CBwGanttDrawContext& ctxDraw);

protected:
	// Draws an item. Used by DrawBar only.
	virtual void DrawSimpleItem(CBwGanttDrawContext& ctxDraw);

	// Draws a group item. Used by DrawBar only.
	virtual void DrawGroupItem(CBwGanttDrawContext& ctxDraw);

	// Draws a milestone item. Used by DrawBar only.
	virtual void DrawMileStoneItem(CBwGanttDrawContext& ctxDraw);
};

