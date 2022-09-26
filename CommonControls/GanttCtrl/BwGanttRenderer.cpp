#include "stdafx.h"
#include "BCGPDrawManager.h"
#include "BCGPVisualManager.h"
#include "BwGanttChart.h" // 里面有BwGanttRenderer.h（防止重复包含头文件）

static void FillBar(const CBCGPGanttItem* pItem, CDC& dc, const CRect& rcFill, COLORREF color, double dGlowLine = 0.6f)
{
	CBCGPVisualManager::GetInstance()->FillGanttBar(pItem, dc, rcFill, color, dGlowLine);
}

void CBwGanttRenderer::DrawSimpleItem(CBwGanttDrawContext & ctxDraw)
{
	COLORREF clrPrimary = ctxDraw.m_clrFill;
	COLORREF clrComplete = ctxDraw.m_clrFill2;
	COLORREF clrRealPrimary = ctxDraw.m_clrRealFill;
	COLORREF clrRealComplete = ctxDraw.m_clrRealFill2;

	COLORREF clrBorder = ctxDraw.m_clrBorder;

	CBCGPPenSelector penBorder(ctxDraw.m_DC, clrBorder);

	CRect rectBar = ctxDraw.m_rectBar;
	rectBar.DeflateRect(1, 1, 1, 1);
	CRect rectRealBar = ctxDraw.m_rectRealBar;
	rectRealBar.DeflateRect(1, 1, 1, 1);

	int progressWidth = (int)(rectBar.Width() * ctxDraw.m_Item.GetProgress());
	int progressRealWidth = (int)(rectRealBar.Width() * ctxDraw.m_Item.GetRealProgress());

	bool bFillProgress = (progressWidth > 1);
	bool bFillRealProgress = (progressRealWidth > 1);
	bool bFillBar = true;
	bool bFillRealBar = true;

	if (progressWidth <= 1 && ctxDraw.m_Item.GetProgress() > 0.5f)
	{
		// if there's a little space and the progress is high, fill the whole bar as complete
		bFillProgress = true;
		bFillBar = false;
	}

	if (progressRealWidth <= 1 && ctxDraw.m_Item.GetRealProgress() > 0.5f)
	{
		// if there's a little space and the progress is high, fill the whole bar as complete
		bFillRealProgress = true;
		bFillRealBar = false;
	}

	if (progressWidth >= rectBar.Width() - 1)
	{
		bFillProgress = true;
		bFillBar = false;
	}

	if (progressRealWidth >= rectRealBar.Width() - 1)
	{
		bFillRealProgress = true;
		bFillRealBar = false;
	}

	if (bFillProgress && bFillBar)
	{
		CRect rectComplete = rectBar;
		rectComplete.right = rectComplete.left + progressWidth;
		FillBar(&ctxDraw.m_Item, ctxDraw.m_DC, rectComplete, clrComplete);
		ctxDraw.m_DC.MoveTo(rectComplete.right, rectBar.top);
		ctxDraw.m_DC.LineTo(rectComplete.right, rectBar.bottom);
		rectBar.left = rectComplete.right + 1;
		FillBar(&ctxDraw.m_Item, ctxDraw.m_DC, rectBar, clrPrimary);
	}
	else // either bFillProgress, or bFillBar is true
	{
		// Fill whole bar
		FillBar(&ctxDraw.m_Item, ctxDraw.m_DC, rectBar, bFillProgress ? clrComplete : clrPrimary);
	}

	if (bFillRealProgress && bFillRealBar)
	{
		CRect rectRealComplete = rectRealBar;
		rectRealComplete.right = rectRealComplete.left + progressRealWidth;
		FillBar(&ctxDraw.m_Item, ctxDraw.m_DC, rectRealComplete, clrRealComplete);
		ctxDraw.m_DC.MoveTo(rectRealComplete.right, rectRealBar.top);
		ctxDraw.m_DC.LineTo(rectRealComplete.right, rectRealBar.bottom);
		rectRealBar.left = rectRealComplete.right + 1;
		FillBar(&ctxDraw.m_Item, ctxDraw.m_DC, rectRealBar, clrRealPrimary);
	}
	else // either bFillProgress, or bFillBar is true
	{
		// Fill whole bar
		FillBar(&ctxDraw.m_Item, ctxDraw.m_DC, rectRealBar, bFillRealProgress ? clrRealComplete : clrRealPrimary);
	}

	// 画外边框
	CPoint pt(6, 6);
	CBrush* pOldBrush = (CBrush*)ctxDraw.m_DC.SelectStockObject(NULL_BRUSH);
	ctxDraw.m_DC.RoundRect(ctxDraw.m_rectBar, pt);
	ctxDraw.m_DC.SelectObject(pOldBrush);

	CBrush* pOldRealBrush = (CBrush*)ctxDraw.m_DC.SelectStockObject(NULL_BRUSH);
	ctxDraw.m_DC.RoundRect(ctxDraw.m_rectRealBar, pt);
	ctxDraw.m_DC.SelectObject(pOldRealBrush);
}

void CBwGanttRenderer::DrawGroupItem(CBwGanttDrawContext & ctxDraw)
{
	ctxDraw.m_DC.BeginPath();
	DrawGroupPath(ctxDraw.m_DC, ctxDraw.m_rectBar);
	ctxDraw.m_DC.CloseFigure();
	ctxDraw.m_DC.EndPath();

	CRgn rgn;
	rgn.CreateRectRgn(0, 0, 0, 0);
	HDC hdc = ctxDraw.m_DC.GetSafeHdc();
	::GetClipRgn(hdc, (HRGN)rgn.GetSafeHandle());
	::SelectClipPath(hdc, RGN_AND);
	COLORREF clrFill = ctxDraw.m_clrFill;
	FillBar(&ctxDraw.m_Item, ctxDraw.m_DC, ctxDraw.m_rectBar, clrFill, 0.36f);
	::SelectClipRgn(hdc, (HRGN)rgn.GetSafeHandle());

	CBCGPPenSelector pen(ctxDraw.m_DC, ctxDraw.m_clrBorder);
	DrawGroupPath(ctxDraw.m_DC, ctxDraw.m_rectBar);
}

void CBwGanttRenderer::DrawMileStoneItem(CBwGanttDrawContext & ctxDraw)
{
	CBCGPPenSelector pen(ctxDraw.m_DC, ctxDraw.m_clrBorder);
	CBCGPBrushSelector brush(ctxDraw.m_DC, ctxDraw.m_Item.IsCompleted() ? ctxDraw.m_clrFill2 : ctxDraw.m_clrFill);
	DrawMileStonePolygon(ctxDraw.m_DC, ctxDraw.m_rectBar);
}

void CBwGanttRenderer::DrawBar(CBwGanttDrawContext & ctxDraw)
{
	DrawSimpleItem(ctxDraw);
}

void CBwGanttRenderer::DrawSelection(CBwGanttDrawContext & ctxDraw)
{
	int k = ctxDraw.m_Item.IsMileStone() ? 4 : 3;
	CRect rectSel = ctxDraw.m_rectBar;
	rectSel.InflateRect(k, k, k, k);

	int d = min(10, rectSel.Height());
	CPoint pt(d, d);

	CRect rectOutput;
	rectOutput.IntersectRect(ctxDraw.m_rectClip, rectSel);
	if (rectOutput.IsRectEmpty())
	{
		return;
	}

	CBCGPAlphaDC dc(ctxDraw.m_DC, rectOutput, 0.37f);
	CBCGPPenSelector pen(dc, ctxDraw.m_clrBorder);
	CBCGPBrushSelector brush(dc, ctxDraw.m_clrFill);
	if (ctxDraw.m_Item.IsMileStone())
	{
		POINT ptCenter = rectSel.CenterPoint();
		int d = rectSel.Height() / 2 - 1;
		POINT pts[4];

		pts[0] = ptCenter;
		pts[1] = ptCenter;
		pts[2] = ptCenter;
		pts[3] = ptCenter;

		pts[0].x -= d;
		pts[1].y -= d;
		pts[2].x += d;
		pts[3].y += d;
		dc.Polygon(pts, 4);
	}
	else
	{
		dc.RoundRect(&rectSel, pt);
	}
}

CBwGanttDrawContext::CBwGanttDrawContext(const CBwGanttItem & item, CDC & dc)
	: m_Item(item)
	, m_DC(dc)
	, m_clrFill(globalData.clrBtnFace)
	, m_clrFill2(globalData.clrBtnShadow)
	, m_clrBorder(globalData.clrWindowFrame)
	, m_clrRealFill(globalData.clrBtnFace)
	, m_clrRealFill2(globalData.clrBtnShadow)
{
}
