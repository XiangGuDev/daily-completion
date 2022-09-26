#include "stdafx.h"
#include "StaticEx.h"

CStaticEx::CStaticEx(void)
{
	_pParent = NULL;
}

CStaticEx::~CStaticEx(void)
{

}

BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	ON_BN_CLICKED(ID_GIMCHECK_BUTTON, &CStaticEx::OnBtnClick)
	ON_EN_CHANGE(ID_GIMCHECK_EDIT, &CStaticEx::OnEnChangeEdit)
	ON_WM_SIZE()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CStaticEx, CStatic)

void CStaticEx::SetMsgParent( CWnd* pParent )
{
	_pParent = pParent;
}

void CStaticEx::OnBtnClick()
{
	CWnd* pParent = GetParent();
	if (NULL != _pParent)
		pParent = _pParent;
	if (pParent == NULL)
		return;
	pParent->PostMessage(GIMCHECK_BUTTONCHICK, (WPARAM)m_hWnd, 0);
}

void CStaticEx::OnEnChangeEdit()
{
	CWnd* pParent = GetParent();
	if (NULL != _pParent)
		pParent = _pParent;
	if (pParent == NULL)
		return;
	pParent->PostMessage(GIMCHECK_EDITCHANGE, (WPARAM)m_hWnd, 0);
}

void CStaticEx::OnSize( UINT nType, int cx, int cy )
{
	CWnd* pParent = GetParent();
	if (NULL != _pParent)
		pParent = _pParent;
	if (pParent == NULL)
		return;
	pParent->PostMessage(STATIC_ONSIZE, (WPARAM)m_hWnd, 0);
}