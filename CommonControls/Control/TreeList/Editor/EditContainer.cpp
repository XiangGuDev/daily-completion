#include "StdAfx.h"
#include "EditContainer.h"

CEditContainer::CEditContainer()
{
	_bkColor = RGB(255, 255, 255);
}

CEditContainer::~CEditContainer()
{
}

BEGIN_MESSAGE_MAP(CEditContainer, CStatic)
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CEditContainer::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			CWnd* pChildWnd = GetWindow(GW_CHILD);
			IEditBase* pEditBase = dynamic_cast<IEditBase*>(pChildWnd);
			if (NULL != pEditBase)
			{
				pEditBase->CancelEdit();
				return TRUE;
			}
		}
		if (pMsg->wParam == VK_RETURN )
		{
			CWnd* pParent = GetParent();
			if (NULL != pParent)
			{
				pParent->SetFocus();
				return TRUE;
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CEditContainer::Create(CRect& rect, CWnd* pParent)
{
	return __super::Create(NULL, WS_VISIBLE | WS_CHILD | SS_NOTIFY | WS_CLIPCHILDREN, rect, pParent);
}

BOOL CEditContainer::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, _bkColor);

	return true;
}
