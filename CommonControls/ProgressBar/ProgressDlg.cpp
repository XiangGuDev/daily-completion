#include "stdafx.h"
#include "ProgressDlg.h"

const size_t nProgressTimeFlag = 2019;		// 进度条定时器标识
const size_t nTimInter = 100;				// 定时器间隔（毫秒）

// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROGRESS_DLG, pParent)
	, _pProInfo(NULL), _pThreadHandle(NULL)
{
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSBAR, m_ProgressBar);
	DDX_Control(pDX, IDC_SHOW_LABEL, _stShow);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
	ON_MESSAGE(USER_PROGRESSMSGBOX, &CProgressDlg::OnDisposeUserMsg)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CProgressDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	//消耗掉消息队列中的所有消息
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//WM_PAINT不能被REMOVE，需要Dispatch  
		if (msg.message = WM_PAINT)
		{
			::DispatchMessage(&msg);
		}
	}
}

void CProgressDlg::OnClose()
{
	if (_bCanStop)
	{
		if (_pEvent)
		{
			_pEvent->StopParser();
			SetTitle(L"正在停止...");
			SetText(L"正在停止...");
			_bIsStopping = true;
		}
		else
		{
			__super::OnCancel();
		}
	}
	
	return;
}

void CProgressDlg::OnOK()
{

}

void CProgressDlg::OnCancel()
{
	OnClose();
}

BOOL CProgressDlg::OnInitDialog()
{
	__super::OnInitDialog();
	HICON hIcon = CDefImage::LoadSysIcon();
	SetIcon(hIcon, TRUE);  // 设置大图标
	SetIcon(hIcon, FALSE);  // 设置小图标

	m_ProgressBar.SetRange32(0, 100);
	
	_ssFont.CreatePointFont(100, _T("微软雅黑"));
	_stShow.SetFont(&_ssFont);

	if (_pProInfo == NULL)
	{
		return TRUE;
	}

	// 模态框设置
	_pProInfo->hProgressHwnd = this->GetSafeHwnd();
	SetTimer(nProgressTimeFlag, nTimInter, NULL);
	_stShow.SetWindowText(_pProInfo->strText);
	this->SetWindowText(_pProInfo->strTitle);

	if (_pProInfo->pThreadPt != NULL)
	{
		// 创建线程，执行函数
		_pThreadHandle = (HANDLE)_beginthreadex(NULL, 0, _pProInfo->pThreadPt, _pProInfo, 0, NULL);
	}

	if (!_bCanStop)
	{
		this->GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);
	}

	return TRUE;
}

int CProgressDlg::GetCurPos()
{
	return m_ProgressBar.GetPos();
}

void CProgressDlg::OnStep(int nStep)
{
	m_ProgressBar.SetPos(nStep);
	m_ProgressBar.Invalidate(TRUE);

	// 运行消息循环
	CCommDlgToolkit::RunMessageLoop();
}

void CProgressDlg::SetTitle(const CString& sTitle)
{
	this->SetWindowText(sTitle);
}

void CProgressDlg::SetText(const CString& sText)
{
	_stShow.SetWindowText(sText);
}

/////**************************模态框相关******************************/////

CProgressDlg::CProgressDlg(ModalProgressInfo* pProInfo, bool bCancel, CProgressAgentStopEvent* pEvent)
	: CDialogEx(IDD_PROGRESS_DLG, CWnd::FromHandle(pProInfo->hParent)),_bCanStop(bCancel), _pEvent(pEvent)
{
	_pProInfo = pProInfo;
	_pThreadHandle = NULL;
}

void CProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case nProgressTimeFlag:
	{
		if (_bIsStopping && _pEvent)
		{
			if (_pEvent->IsStopedParser())
			{
				__super::OnCancel();
			}
		}
		else
		{
			ModalUpdate();
		}
		break;
	}
	default:
		break;
	}

	__super::OnTimer(nIDEvent);
}

LRESULT CProgressDlg::OnDisposeUserMsg(WPARAM wParam, LPARAM lParam)
{
	_pProInfo->bPause = true;
	MessageBoxInfo* pOperate = (MessageBoxInfo*)lParam;
	pOperate->lResult = MessageBox(pOperate->strContent, pOperate->strTitle, pOperate->uType);
	_pProInfo->bPause = false;
	return pOperate->lResult;
}

void CProgressDlg::ModalUpdate()
{
	if (_pProInfo == NULL)
	{
		return;
	}

	// 工作完成
	if (_pProInfo->bComplete)
	{
		_pProInfo->bComplete = false;
		m_ProgressBar.SetPos(_pProInfo->nMaxStep);
		EndDialog(IDOK);
		return;
	}

	// 线程结束
	if (_pThreadHandle != NULL)
	{
		DWORD dwExitCode = 0;
		GetExitCodeThread(_pThreadHandle, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
		{
			EndDialog(IDOK);
			return;
		}
	}

	// 实时修改文字内容
	if (_pProInfo->bChangeText)
	{
		_pProInfo->bChangeText = false;
		_stShow.SetWindowText(_pProInfo->strText);
		this->SetWindowText(_pProInfo->strTitle);
	}

	// 不超过预设最大值
	int nCurPos = m_ProgressBar.GetPos();
	if (nCurPos > _pProInfo->nMaxStep)
	{
		return;
	}

	// 自动增加和按照返回增加
	if (_pProInfo->bAutoAdd)
	{
		//暂停当前进度
		if (!_pProInfo->bPause)
		{
			m_ProgressBar.SetPos(nCurPos + 1);
		}
		else
		{
			m_ProgressBar.SetPos(nCurPos);
		}
	}
	else
	{
		m_ProgressBar.SetPos(_pProInfo->nCurStep);
	}
}

BOOL CProgressDlg::DestroyWindow()
{
	HICON hIcon = GetIcon(true);
	if (hIcon != nullptr)
	{
		DestroyIcon(hIcon);
		hIcon = nullptr;
	}
	hIcon = GetIcon(false);
	if (hIcon != nullptr)
	{
		DestroyIcon(hIcon);
		hIcon = nullptr;
	}
	return __super::DestroyWindow();
}