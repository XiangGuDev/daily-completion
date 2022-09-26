
// daily-completionDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "daily-completion.h"
#include "daily-completionDlg.h"
#include "afxdialogex.h"
#include "App/PointApp.h"
#include "Model/GameModel.h"
#include "Cmd/KillCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CdailycompletionDlg::CdailycompletionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DAILYCOMPLETION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdailycompletionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdailycompletionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_KILLENEMY, &CdailycompletionDlg::OnBnClickedKillenemy)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CdailycompletionDlg 消息处理程序
BOOL CdailycompletionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	_gameModel = PointApp::Instance()->Get<GameModel>("GameModel");
	_gameModel->_cnt.OnCountChanged += std::bind(&CdailycompletionDlg::OnEnemyCntChanged, this, std::placeholders::_1);
	// TODO: 在此添加额外的初始化代码
	UpdateLog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdailycompletionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CdailycompletionDlg::OnBnClickedKillenemy()
{
	std::make_shared<CKillCmd>()->Excute();
}

void CdailycompletionDlg::OnEnemyCntChanged(int val)
{
	UpdateLog();
}

void CdailycompletionDlg::UpdateLog()
{
	auto cnt = _gameModel->_cnt.Get();
	if (cnt == 0)
	{
		static_cast<CStatic *>(GetDlgItem(IDC_STATIC))->SetWindowText(L"游戏结束！");
	}
	else
	{
		CString strInfo;
		strInfo.Format(L"剩下%d只怪物！", cnt);
		static_cast<CStatic *>(GetDlgItem(IDC_STATIC))->SetWindowText(strInfo);
	}
}


void CdailycompletionDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	_gameModel->_cnt.OnCountChanged -= std::bind(&CdailycompletionDlg::OnEnemyCntChanged, this, std::placeholders::_1);
}
