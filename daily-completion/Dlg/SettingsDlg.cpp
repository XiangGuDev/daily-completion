
// daily-completionDlg.cpp: 实现文件
//

#include "pch.h"
#include "../framework.h"
#include "../daily-completion.h"
#include "SettingsDlg.h"
#include "afxdialogex.h"
#include "../Utility/AutoStartUtility.h"
#include "../System/TaskSystem.h"

using namespace YFramework;
using namespace ControlUI;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGS_DLG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE, _dateCtrl);
}

BEGIN_MESSAGE_MAP(CSettingsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AUTOSTART, &CSettingsDlg::OnClickAutoStart)
	ON_BN_CLICKED(IDC_CHANGE, &CSettingsDlg::OnClickChange)
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE, &CSettingsDlg::OnDtnDatetimechangeDate)
END_MESSAGE_MAP()


// CSettingsDlg 消息处理程序
BOOL CSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	auto date = GetSystem<CTaskSystem>()->GetDate();
	_dateCtrl.SetTime(date);

	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSettingsDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSettingsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSettingsDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CSettingsDlg::OnClickAutoStart()
{
	AutoStartUtility::SetAutoStart(true);
}

void CSettingsDlg::OnClickChange()
{
	COleDateTime time;
	_dateCtrl.GetTime(time);
	GetSystem<CTaskSystem>()->SetDate(time.GetYear(), time.GetMonth(), time.GetDay());
}


void CSettingsDlg::OnDtnDatetimechangeDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 不要在此处下断点，易卡死
	COleDateTime time;
	_dateCtrl.GetTime(time);
	GetSystem<CTaskSystem>()->SetDate(time.GetYear(), time.GetMonth(), time.GetDay());

	*pResult = 0;
}
