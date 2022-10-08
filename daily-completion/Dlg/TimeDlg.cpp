#include "pch.h"
#include "../framework.h"
#include "../daily-completion.h"
#include "TimeDlg.h"
#include "afxdialogex.h"
#include "../Utility/AutoStartUtility.h"
#include "../System/TaskSystem.h"
#include "../System/TimeSystem.h"

#define ID_TIMER 1

BEGIN_MESSAGE_MAP(CTimeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_ICON, &CTimeDlg::OnClickIcon)
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

CTimeDlg::CTimeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_bStart = false;
}

void CTimeDlg::Reset()
{
	_bStart = false;
	UpdateTime();
}

void CTimeDlg::Start()
{
	_bStart = true;
}

void CTimeDlg::Pause()
{
	_bStart = false;
}

void CTimeDlg::SetTip(const CString & tip)
{
	_btnIcon.SetTooltip(tip);
}

void CTimeDlg::UpdateTime()
{
	int m, s;
	GetSystem<CTimeSystem>()->GetTime(m, s);
	CString str;
	str.Format(L"%02d:%02d", m, s);
	_content.SetWindowText(str);
}

void CTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

// CTimeDlg 消息处理程序
BOOL CTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	static CFont font;
	font.DeleteObject();
	font.CreatePointFont(180, _T("微软雅黑"));

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.left += 5;
	rcClient.right -= 5;
	rcClient.top += 5;
	rcClient.bottom -= 5;

	// 图标按钮
	{
		CRect rcBtn = rcClient;
		rcBtn.right = rcBtn.left + rcClient.Height();
		_btnIcon.Create(L"", WS_CHILD | WS_VISIBLE, rcBtn, this, IDC_BTN_ICON);
		HICON hIconOK = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);//加载图标，第四、五个参数为设置图标大小
		_btnIcon.SetIcon(hIconOK);
		_btnIcon.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_NOBORDERS;
		_btnIcon.ShowWindow(SW_SHOW);
		_btnIcon.SetFont(&font);//设置字体
	}
	// 内容
	{
		CRect rcContent = rcClient;
		rcContent.left += rcClient.Height();
		_content.Create(L"00:00", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, rcContent, this, IDC_CONTENT);
		_content.ShowWindow(SW_SHOW);
		_content.SetFont(&font);//设置字体
	}

	SetTimer(ID_TIMER, 1000, NULL);
	Reset();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTimeDlg::OnPaint()
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
HCURSOR CTimeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTimeDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(ID_TIMER);
}

void CTimeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_TIMER && _bStart)
	{
		GetSystem<CTimeSystem>()->GoStep(-1);
		UpdateTime();
	}
	__super::OnTimer(nIDEvent);
}

void CTimeDlg::OnClickIcon()
{
	_bStart ? Pause() : Start();
}

#define  IDC_CLOSE 1000
void CTimeDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	tagPOINT lpoint ;
	::GetCursorPos(&lpoint);//得到鼠标位置
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, IDC_CLOSE, L"关闭");
	//确定弹出式菜单的位置
	SetForegroundWindow();
	UINT nCmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, lpoint.x, lpoint.y, this);
	OutputDebugString(CConvert::Int2Text(nCmd));
	if (nCmd == IDC_CLOSE)
	{
		ShowWindow(SW_HIDE);
	}
	//资源回收
	HMENU hmenu = menu.Detach();
	menu.DestroyMenu();
	__super::OnRButtonUp(nFlags, point);
}
