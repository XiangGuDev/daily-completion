////////////////////////////////////////////////////////////////////////////////
// Code for CPP file
#include "pch.h"
#include "LayoutUtility.h"

LayoutUtility::LayoutUtility()
{
}

LayoutUtility::~LayoutUtility()
{
}

void LayoutUtility::SetControl(CWnd * wnd, LayoutUtility::LAYOUT_ANCHOR flag)
{
	ASSERT(wnd != NULL);
	if (!wnd)return;

	int width = GetSystemMetrics(SM_CXSCREEN); //»ñÈ¡ÆÁÄ»XÏñËØ
	int height = GetSystemMetrics(SM_CYSCREEN);  //»ñÈ¡ÆÁÄ»YÏñËØ

	CRect rcWnd;
	wnd->GetWindowRect(rcWnd);

	if (flag == ANCHOR_CENTER)
	{
		int x = width / 2 - rcWnd.Width() / 2;
		int y = height / 2 - rcWnd.Height() / 2;
		rcWnd.MoveToXY({ x,y });
	}
	else if (flag == ANCHOR_LEFTTOP)
	{
		rcWnd.MoveToXY({ 0 ,0 });
	}
	else if (flag == ANCHOR_RIGHTTOP)
	{
		rcWnd.MoveToXY({ width - rcWnd.Width(),0 });
	}
	else if (flag == ANCHOR_LEFTBOTTOM)
	{
		rcWnd.MoveToXY({ 0 , height - rcWnd.Height() });
	}
	else if (flag == ANCHOR_RIGHTBOTTOM)
	{
		rcWnd.MoveToXY({ width - rcWnd.Width(),height - rcWnd.Height() });
	}

	wnd->MoveWindow(rcWnd);
}



