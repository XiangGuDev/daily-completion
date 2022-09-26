#include "StdAfx.h"
#include "MoveBar.h"

/****************浮动工具栏**********************/
IMPLEMENT_DYNAMIC(CMoveBar, CToolBar)

BEGIN_MESSAGE_MAP(CMoveBar, CToolBar)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

LRESULT CMoveBar::OnNcHitTest(CPoint pt)
{
	// 如果在工具栏周边就可以拖动
	ScreenToClient(&pt);

	CRect rc;
	GetClientRect(&rc);

	if ((pt.x < 3 || pt.x > rc.Width() - 2) ||
		(pt.y < 3 || pt.y > rc.Height() - 2))
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		return HTCAPTION;
	}

	return __super::OnNcHitTest(pt);
}

