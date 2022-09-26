#include "StdAfx.h"
#include "CCustomCEdit.h"
IMPLEMENT_DYNAMIC(CCustomCEdit, CValidEditEx)

BEGIN_MESSAGE_MAP(CCustomCEdit, CValidEditEx)
	ON_WM_NCPAINT()
	ON_WM_PAINT()
END_MESSAGE_MAP()

CCustomCEdit::CCustomCEdit() : CValidEditEx()
{
	_bStatus = false;
}

CCustomCEdit::~CCustomCEdit()
{

}

void CCustomCEdit::SetStatus(bool bStatus)
{
	_bStatus = bStatus;
}

void CCustomCEdit::OnNcPaint()
{
	CValidEditEx::OnNcPaint();
	// 绘制矩形边框
	CRect rcBorder;
	GetWindowRect(rcBorder);
	ScreenToClient(rcBorder);

	// 处理偏差问题 (top left 为负数的情况下)
	if (rcBorder.top < 0)
	{
		rcBorder.top = 0;
	}

	if (rcBorder.left < 0)
	{
		rcBorder.left = 0;
	}
	rcBorder.bottom += 1;
	rcBorder.right += 1;

	CBrush	borderBrush;
	if (_bStatus)
	{
		borderBrush.CreateSolidBrush(RGB(255, 0, 0));
		CWindowDC dc(this);
		dc.FrameRect(rcBorder, &borderBrush);
	}
	else
	{
		borderBrush.CreateSolidBrush(RGB(192, 192, 192));
		CWindowDC dc(this);
		dc.FrameRect(rcBorder, &borderBrush);
	}
}

void CCustomCEdit::OnPaint()
{
	CValidEditEx::OnNcPaint();
	// 绘制矩形边框
	CRect rcBorder;
	GetWindowRect(rcBorder);
	ScreenToClient(rcBorder);

	// 处理偏差问题 (top left 为负数的情况下)
	if (rcBorder.top < 0)
	{
		rcBorder.top = 0;
	}

	if (rcBorder.left < 0)
	{
		rcBorder.left = 0;
	}
	rcBorder.bottom += 1;
	rcBorder.right += 1;

	CBrush	borderBrush;
	if (_bStatus)
	{
		borderBrush.CreateSolidBrush(RGB(255, 0, 0));
		CWindowDC dc(this);
		dc.FrameRect(rcBorder, &borderBrush);
	}
	else
	{
		borderBrush.CreateSolidBrush(RGB(192, 192, 192));
		CWindowDC dc(this);
		dc.FrameRect(rcBorder, &borderBrush);
	}
}

