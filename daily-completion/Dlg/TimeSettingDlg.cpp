
// daily-completionDlg.cpp: 实现文件
//

#include "pch.h"
#include "../framework.h"
#include "../daily-completion.h"
#include "TimeSettingDlg.h"
#include "afxdialogex.h"
#include "TimeDlg.h"
#include "../System/TimeSystem.h"
#include "../Utility/LayoutUtility.h"

CTimeSettingDlg::CTimeSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIMESETTING_DLG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMB_TIME, _combTime);
	DDX_Control(pDX, IDC_COMB_NAME, _combName);
	DDX_Control(pDX, IDC_COMB_SHOW, _combShow);
}

BEGIN_MESSAGE_MAP(CTimeSettingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDRESET, &CTimeSettingDlg::OnClickReset)
END_MESSAGE_MAP()


// CTimeDlg 消息处理程序
BOOL CTimeSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	_combTime.InsertString(0, L"1分钟");
	_combTime.InsertString(1, L"5分钟");
	_combTime.InsertString(2, L"10分钟");
	_combTime.InsertString(3, L"15分钟");
	_combTime.InsertString(4, L"20分钟");
	_combTime.InsertString(5, L"30分钟");
	_combTime.InsertString(6, L"45分钟");
	_combTime.InsertString(7, L"1小时");

	_combShow.InsertString(0, L"中心");
	_combShow.InsertString(1, L"右上");
	_combShow.InsertString(2, L"左上");
	_combShow.InsertString(3, L"右下");
	_combShow.InsertString(4, L"左下");

	for(auto name : _nameCache)
		_combName.InsertString(_combName.GetCount(), name);

	OnClickReset();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTimeSettingDlg::OnPaint()
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
HCURSOR CTimeSettingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTimeSettingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CTimeSettingDlg::OnClickReset()
{
	_combTime.SelectString(0, L"30分钟");
	_combShow.SetWindowText(L"");
	_combName.SetWindowText(L"");
}

void CTimeSettingDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	int m = 0, s = 0;
	CString str;
	_combTime.GetWindowText(str);
	int idx = str.Find(L"分");
	if (idx >= 1)
		m += CConvert::Text2Int(str.Left(idx));
	idx = str.Find(L"时");
	if (idx >= 1)
		m += 60 * CConvert::Text2Int(str.Left(idx));
	GetSystem<CTimeSystem>()->SetTime(m, s);

	_combShow.GetWindowText(str);
	if (str == L"中心")
		LayoutUtility::SetControl(CTimeDlg::Instance(), LayoutUtility::ANCHOR_CENTER);
	else if (str == L"" || str == L"右上")
		LayoutUtility::SetControl(CTimeDlg::Instance(), LayoutUtility::ANCHOR_RIGHTTOP);
	else if (str == L"左上")
		LayoutUtility::SetControl(CTimeDlg::Instance(), LayoutUtility::ANCHOR_LEFTTOP);
	else if (str == L"右下")
		LayoutUtility::SetControl(CTimeDlg::Instance(), LayoutUtility::ANCHOR_RIGHTBOTTOM);
	else if (str == L"左下")
		LayoutUtility::SetControl(CTimeDlg::Instance(), LayoutUtility::ANCHOR_LEFTBOTTOM);

	_combName.GetWindowText(str);
	if (!str.IsEmpty())
	{
		_nameCache.push_back(str);
	}
	CTimeDlg::Instance()->SetTip(str);
	CTimeDlg::Instance()->ShowWindow(SW_SHOW);
	CTimeDlg::Instance()->Reset();
	CTimeDlg::Instance()->Start();

	CDialogEx::OnOK();
}
