#include "stdafx.h"
#include "BatchOperationDlg.h"
#include "BatchOperDlg.h"


CBatchOperDlg::CBatchOperDlg(LPCTSTR strMess, LPCTSTR strTitle, CWnd* pParent /*= NULL*/)
	: _pDlg(NULL)
{
	InitData(strMess, strTitle, pParent);
}

CBatchOperDlg::~CBatchOperDlg()
{
	SAFE_DELETE(_pDlg);
}

void CBatchOperDlg::InitData(LPCTSTR strMess, LPCTSTR strTitle, CWnd* pParent /*= NULL*/)
{
	_pDlg = new CBatchOperationDlg(strMess, strTitle, pParent);
}

int CBatchOperDlg::DoModal()
{
	INIT_RESHANDLE();
	return _pDlg->DoModal();
}
