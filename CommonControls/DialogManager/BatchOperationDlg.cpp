#include "stdafx.h"
#include "BatchOperationDlg.h"

#define ID_YES							1
#define ID_ALLYES						2
#define ID_NO							3
#define ID_ALLNO						4

CBatchOperationDlg::CBatchOperationDlg(LPCTSTR strMess, LPCTSTR strTitle, CWnd* pParent /*= NULL*/)
	: CDialog(IDD, pParent)
{
	_strMess = strMess;
	_strTitle = strTitle;
}

CBatchOperationDlg::~CBatchOperationDlg()
{
}

void CBatchOperationDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK1, _checkBox);
	DDX_Control(pDX, IDC_STATIC_MESS, _staticMess);
}

BOOL CBatchOperationDlg::OnInitDialog()
{
	BOOL bRet = __super::OnInitDialog();

	// 修改对话框标题和提示信息
	this->SetWindowText(_strTitle);
	_staticMess.SetWindowText(_strMess);

	// 初始化勾选框状态
	_checkBox.SetState(FALSE);

	return bRet;

}

void CBatchOperationDlg::OnOK()
{
	int nState = _checkBox.GetState();
	if (nState == 0)
	{
		this->EndDialog(ID_YES);
	}
	else if (nState == 1)
	{
		this->EndDialog(ID_ALLYES);
	}
	else
	{
		return;
	}
}

void CBatchOperationDlg::OnCancel()
{
	int nState = _checkBox.GetState();
	if (nState == 0)
	{
		this->EndDialog(ID_NO);
	}
	else if (nState == 1)
	{
		this->EndDialog(ID_ALLNO);
	}
	else
	{
		return;
	}
}

BEGIN_MESSAGE_MAP(CBatchOperationDlg, CDialog)
	

END_MESSAGE_MAP()