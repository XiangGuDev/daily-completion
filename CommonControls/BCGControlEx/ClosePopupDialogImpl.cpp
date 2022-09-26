#include "stdafx.h"
#include "ClosePopupDialogImpl.h"

controls::CClosePopupDialogImpl::CClosePopupDialogImpl()
	:_bHaveChildDlg(FALSE)
{

}

controls::CClosePopupDialogImpl::~CClosePopupDialogImpl()
{

}

BOOL controls::CClosePopupDialogImpl::IsHaveChildDlg() const
{
	return _bHaveChildDlg;
}

void controls::CClosePopupDialogImpl::SetChildDlg(BOOL bHaveChildDlg)
{
	_bHaveChildDlg = bHaveChildDlg;
}

void controls::CClosePopupDialogImpl::ClosePopupDlg(LPCTSTR lpszEditValue, BOOL bOK, DWORD_PTR dwUserData)
{

}