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
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CTimeDlg::CTimeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_bStart = false;
}

void CTimeDlg::Reset()
{
	KillTimer(ID_TIMER);
	_bStart = false;
	UpdateTime();
	UpdateIcon();
}

void CTimeDlg::Start()
{
	KillTimer(ID_TIMER);
	SetTimer(ID_TIMER, 1000, NULL);
	_bStart = true;
	UpdateIcon();
}

void CTimeDlg::Pause()
{
	KillTimer(ID_TIMER);
	_bStart = false;
	UpdateIcon();
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
	if (_bStart && m == 0 && s == 0)
	{
		// 任务结束提示
		Pause();
		CString str;
		_btnIcon.GetTooltip(str);
		::MessageBox(::GetActiveWindow(), L"「" + str + "」已结束！", CPathConfig::GetProjectName(), MB_TOPMOST | MB_OK | MB_ICONINFORMATION);
		ShowWindow(SW_HIDE);
	}
}

void CTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CTimeDlg::UpdateIcon()
{
	_btnIcon.SetIcon(_iconList[_bStart ? 0 : 1]);
	_btnIcon.RedrawWindow();
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

	// 窗口置顶
	{
		::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	}

	// 圆角
	{
		/*CRect rc;
		GetWindowRect(&rc);
		CRgn rgn;
		rc -= rc.TopLeft();
		rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 10, 10);
		SetWindowRgn(rgn, TRUE);*/
	}

	// 窗口透明
	{
		SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		::SetLayeredWindowAttributes(GetSafeHwnd(), 0, 128, LWA_ALPHA);
		ModifyStyleEx(WS_EX_TRANSPARENT, 0, SWP_FRAMECHANGED);
	}

	// 图标按钮
	{
		CRect rcBtn = rcClient;
		rcBtn.right = rcBtn.left + rcClient.Height();
		_btnIcon.Create(L"", WS_CHILD | WS_VISIBLE, rcBtn, this, IDC_BTN_ICON);
		_iconList.push_back((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR /*| LR_CREATEDIBSECTION*/));//加载图标，第四、五个参数为设置图标大小
		_iconList.push_back((HICON)LoadImage(AfxGetResourceHandle(), CPathConfig::GetAppStartPath()+L"Config/stop.ico", IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR /*| LR_CREATEDIBSECTION */| LR_LOADFROMFILE));
		_btnIcon.SetIcon(_iconList[0]);
		_btnIcon.m_nFlatStyle = CBCGPButton::BUTTONSTYLE_NOBORDERS;
		_btnIcon.ShowWindow(SW_SHOW);
		_btnIcon.SetFont(&font);//设置字体
		_btnIcon.m_bTransparent = true;
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
		__super::OnPaint();
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

	for (auto icon : _iconList)
	{
		DestroyIcon(icon);
	}
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
	if (_bStart)
	{
		Pause();
	}
	else
	{
		Start();
	}
	UpdateIcon();
}

#define  IDC_CLOSE 1000
void CTimeDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	tagPOINT lpoint;
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


LRESULT CTimeDlg::OnNcHitTest(CPoint point)
{
	return __super::OnNcHitTest(point);
}


BOOL CTimeDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);        //取得窗体窗户区区域
	CRgn myrgn1, myrgn2;
	myrgn1.CreateRoundRectRgn(0, 0, rect.Width(), rect.Height(), 10, 10);//圆角的窗体区域
	CBrush frameBrush, titlebrush, bgbrush;
	bgbrush.CreateSolidBrush(RGB(217, 210, 233));//主窗体背景色画刷
	frameBrush.CreateSolidBrush(RGB(200, 200, 200));
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->FillRgn(&myrgn1, &bgbrush);
	pDC->FrameRgn(&myrgn1, &frameBrush, 1, 1);
	pDC->SetBkMode(TRANSPARENT);

	return true;
}

HBRUSH CTimeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_BTN_ICON || pWnd->GetDlgCtrlID() == IDC_CONTENT)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return   (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CTimeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 拖拽效果
	__super::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}