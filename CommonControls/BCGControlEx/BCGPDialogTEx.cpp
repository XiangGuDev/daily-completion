#include "stdafx.h"
#include "BCGPDialogTEx.h"
#include "ClosePopupDialogImpl.h"
using namespace controls;
IMPLEMENT_DYNCREATE(CBCGPDialogTEx, CBCGPDialog)
BEGIN_MESSAGE_MAP(CBCGPDialogTEx, CBCGPDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

controls::CBCGPDialogTEx::CBCGPDialogTEx()
{
	CommonConstruct();
	_pParentCtl = NULL;
}

controls::CBCGPDialogTEx::CBCGPDialogTEx(UINT nIDTemplate, CWnd *pParent/*= NULL*/)
	:CBCGPDialog(nIDTemplate, pParent)

{
	CommonConstruct();
	_pParentCtl = NULL;
}

controls::CBCGPDialogTEx::CBCGPDialogTEx(LPCTSTR lpszTemplateName, CWnd *pParentWnd/*= NULL*/)
	:CBCGPDialog(lpszTemplateName, pParentWnd)
{
	CommonConstruct();
	_pParentCtl = NULL;
}

void controls::CBCGPDialogTEx::OnDestroy()
{	
	CBCGPDialog::OnDestroy();

	if (NULL != _pParentCtl)
	{
		delete _pParentCtl;
		_pParentCtl = NULL;
	}
}