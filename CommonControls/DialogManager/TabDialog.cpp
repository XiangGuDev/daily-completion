#include "stdafx.h"
#include "TabDialog.h"

#define TAB_ID 1024
#define TOP_MARIN 3
#define MAXMIZEOFFSET 8

BEGIN_MESSAGE_MAP(CTabDialog, CDialog)
	ON_WM_SIZE()
	ON_CONTROL(TCN_SELCHANGE, TAB_ID, OnTabSelChange)
END_MESSAGE_MAP()
CTabDialog::CTabDialog(TabPageContextArray& arrSubPage, DialogSetting dlgSetting) :\
CDialog(IDD), _arrSubPages(arrSubPage), _dlgSetting(dlgSetting)
{
	_pLastSelPage = NULL;
}

CTabDialog::~CTabDialog()
{
}

BOOL CTabDialog::OnInitDialog()
{
	auto retVal = __super::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_SOFT_ICON);
	this->SetIcon(hIcon, FALSE);

	_tabCtrl.CreateWnd(this, CRect(0, 0, 0, 0), TAB_ID);
	_tabCtrl.SetDisplayStyle(0);
	// 根据tabContext创建页签
	for (auto page : _arrSubPages)
	{
		HWND hChild = page->GetWndHandle(_tabCtrl.GetSafeHwnd());
		_tabCtrl.InsertItem(page->GetTabName(), CWnd::FromHandle(hChild));
	}
	if (_dlgSetting.posType == byNameFrame)
	{
		CRect rtWnd;
		AfxGetMainWnd()->GetWindowRect(rtWnd);
		rtWnd.left -= MAXMIZEOFFSET;
		rtWnd.top -= MAXMIZEOFFSET;
		rtWnd.right += MAXMIZEOFFSET;
		MoveWindow(rtWnd);
	}
	else if (_dlgSetting.posType == centerWnd)
	{
		CenterWindow();
	}
	SetWindowText(_dlgSetting.strCaptionName);
	return retVal;
}

void CTabDialog::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	CRect rtWnd;
	GetClientRect(rtWnd);
	rtWnd.top += TOP_MARIN;
	if (_tabCtrl.GetSafeHwnd())
		_tabCtrl.MoveWindow(rtWnd);
}

void CTabDialog::OnTabSelChange()
{
	int nIndex = _tabCtrl.GetCurSel();
	
	auto curSelPage = nIndex >= 0 ? _arrSubPages[nIndex] : NULL;
	if (_pLastSelPage)
		_pLastSelPage->Leave();
	if (curSelPage)
		curSelPage->Enter();
	_pLastSelPage = curSelPage;
}

void CTabDialog::OnOK()
{

}

void CTabDialog::OnCancel()
{
	d3s::views::view::IOsgViewPort *pActiveView = CGISCommonTool::GetActiveViewPort();
	UINT uThreadId = pActiveView->GetThreadID();

	HANDLE hdThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, uThreadId);
	ResumeThread(hdThread);
	CloseHandle(hdThread);
	__super::OnCancel();
}
