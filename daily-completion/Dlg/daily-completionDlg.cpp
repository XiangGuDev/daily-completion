
// daily-completionDlg.cpp: 实现文件
//

#include "pch.h"
#include "../framework.h"
#include "../daily-completion.h"
#include "daily-completionDlg.h"
#include "afxdialogex.h"
#include "../System/TaskSystem.h"
#include "../Model/Task.h"
#include "../Adapters/TaskAdapter.h"
#include "../Control/CTaskListCtrl.h"
#include "SettingsDlg.h"
#include "TimeSettingDlg.h"
#include "../Model/GlobalModel.h"
#include "../Model/UpdateGridEvent.h"
#include "TimeDlg.h"

using namespace YFramework;
using namespace ControlUI;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CdailycompletionDlg, CBaseTaskDlg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_MENU_BTN, &CdailycompletionDlg::OnClickMenu)
	ON_BN_CLICKED(IDC_TIMESETTING_BTN, &CdailycompletionDlg::OnClickTimeSetting)
	ON_EN_CHANGE(IDC_SEARCH, &CdailycompletionDlg::OnSearchKeyChanged)
	ON_NOTIFY(LCN_ENDEDITDONE, IDC_TASKLIST, &CdailycompletionDlg::OnTaskListEdit)
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_HOTKEY()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CdailycompletionDlg::CdailycompletionDlg(CWnd* pParent /*=nullptr*/)
	: CBaseTaskDlg(IDD_DAILYCOMPLETION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_bDontHide = false;
}

void CdailycompletionDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseTaskDlg::DoDataExchange(pDX);
}

// CdailycompletionDlg 消息处理程序
BOOL CdailycompletionDlg::OnInitDialog()
{
	CBaseTaskDlg::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	_settingsDlg = std::make_shared<CSettingsDlg>(this);
	_timeDlg = std::make_shared<CTimeSettingDlg>(this);

	static CFont font;
	font.DeleteObject();
	font.CreatePointFont(135, _T("微软雅黑"));

	int btnLen = 10; // 按钮边长
	
	// 窗口置顶
	{
		::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	}
	// 时间
	{
		CTimeDlg::Instance()->Create(CTimeDlg::IDD);
		CTimeDlg::Instance()->CenterWindow();
		CTimeDlg::Instance()->ShowWindow(SW_HIDE);
	}
	// 任务列表
	{
		CRect rcTree;
		GetClientRect(&rcTree);
		btnLen = rcTree.Height() / 10;
		rcTree.bottom -= btnLen;

		_taskList = std::make_shared<CTaskListCtrl>();
		_taskList->Create(rcTree, this, IDC_TASKLIST);
		CString strAppPath = CPathConfig::GetAppStartPath();
		strAppPath.Append(L"Config/界面配置.xml");
		if (!_taskList->LoadConfig(strAppPath, L"任务列表"))
		{
			::MessageBox(::GetActiveWindow(), L"配置文件读取失败！", L"提示", MB_OK);
		}
		_taskList->SetNofityWnd(m_hWnd);
		_taskList->ShowHeader(false);
		_taskList->SetHasGrid(false);
		_taskList->SetAutoColumnWidth();
		_taskList->LoadIcon({ CPathConfig::GetAppStartPath()+L"Config/Fixed.ico" });
		_taskList->OnUpdateGrid(std::make_shared<UpdateGridEvent>());
	}
	// 搜索
	{
		CRect rcSc;
		GetClientRect(&rcSc);
		rcSc.top += rcSc.Height() * 9 / 10;
		rcSc.right -= btnLen * 2;
		_editSearch.Create(WS_BORDER | WS_CHILD | WS_VISIBLE, rcSc, this, IDC_SEARCH);
		_editSearch.ShowWindow(SW_SHOW);
		_editSearch.SetFont(&font);//设置字体
		_editSearch.SetPrompt(_T("搜索"));
		auto colTheme = _editSearch.GetColorTheme();
		colTheme.m_clrBackground = RGB(240, 240, 240);
		_editSearch.SetColorTheme(colTheme);
	}
	// 时间设置按钮
	{
		CRect rcBtn;
		GetClientRect(&rcBtn);
		rcBtn.top += rcBtn.Height() * 9 / 10;
		rcBtn.left = rcBtn.right - btnLen * 2;
		rcBtn.right -= btnLen;
		_btnTimeSetting.Create(L"", WS_BORDER | WS_CHILD | WS_VISIBLE, rcBtn, this, IDC_TIMESETTING_BTN);
		HICON hIconOK=(HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 0,0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);//加载图标，第四、五个参数为设置图标大小
		_btnTimeSetting.SetIcon(hIconOK);
		_btnTimeSetting.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_NOBORDERS;
		_btnTimeSetting.ShowWindow(SW_SHOW);
		_btnTimeSetting.SetFont(&font);//设置字体
	}
	// 菜单
	{
		CRect rcMenu;
		GetClientRect(&rcMenu);
		rcMenu.top += rcMenu.Height() * 9 / 10;
		rcMenu.left = rcMenu.right - btnLen;
		_btnMenu.Create(L"···", WS_BORDER | WS_CHILD | WS_VISIBLE, rcMenu, this, IDC_MENU_BTN);
		_btnMenu.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_NOBORDERS;
		_btnMenu.ShowWindow(SW_SHOW);
		_btnMenu.SetFont(&font);//设置字体
	}
	// 日期

	RegisterHotKey(m_hWnd, 1001, MOD_CONTROL | MOD_SHIFT, 'A');
	CenterWindow();
	HideToTaskbar();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CdailycompletionDlg::OnDestroy()
{
	CBaseTaskDlg::OnDestroy();
	_btnMenu.DestroyWindow();
	_editSearch.DestroyWindow();
	_taskList->DestroyWindow();

	UnregisterHotKey(m_hWnd, 1001);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdailycompletionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBaseTaskDlg::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdailycompletionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdailycompletionDlg::OnClickTimeSetting()
{
	_bDontHide = true;
	_timeDlg->DoModal();
	_bDontHide = false;
}

void CdailycompletionDlg::OnClickMenu()
{
	CPoint curPt;
	GetCursorPos(&curPt);
	CMenu rightMenu;
	if (!rightMenu.LoadMenu(IDR_MENU))
		return;

	CMenu *pSubMenu = rightMenu.GetSubMenu(0);
	if (pSubMenu == NULL)
		return;

	UINT nCmd = pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
		curPt.x, curPt.y, this);
	if (nCmd == IDC_SETTINGS)
	{
		_bDontHide = true;
		_settingsDlg->DoModal();
		_bDontHide = false;
	}
	else if (nCmd == IDC_ABOUT)
	{
		MessageBox(L"YanF制作");
	}
	else if (nCmd == IDC_QUIT)
	{
		EndDialog(IDOK);
	}
}


void CdailycompletionDlg::OnSearchKeyChanged()
{
	CString strKey;
	_editSearch.GetWindowText(strKey);
	GetModel<CGlobalModel>()->SearchKey.Set(strKey);
}



void CdailycompletionDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nHotKeyId == 1001)
	{
		ShowFromTaskbar();
	}

	__super::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CdailycompletionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);
	if (WA_INACTIVE == nState && !_bDontHide)
	{
		HideToTaskbar();
	}
}

void CdailycompletionDlg::OnTaskListEdit(NMHDR * pNMHDR, LRESULT * pResult)
{
	GetSystem<CTaskSystem>()->Save();
}
