#include "stdafx.h"
#include "UnityViewPort.h"
#include "IListener/IUnityMsgListener.h"
#include <Tlhelp32.h>

#define MSG_MAX_LENGTH 256
#define UNITY_WND_STYLE WS_POPUP

HWND CUnityViewPort::apphWnd = NULL;

CUnityViewPort::CUnityViewPort(CWnd* pParent)	// 标准构造函数
	: CBaseDialog(CUnityViewPort::IDD, pParent, false)
	, _bMouseTracking(false)
	, _pUnityWnd(NULL)
{

}


void CUnityViewPort::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}

IMPL_SINGLE_CLASS2(CUnityViewPort, L"CUnityViewPort")

BEGIN_MESSAGE_MAP(CUnityViewPort, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CString CUnityViewPort::GetUnityExecutePath()
{
	// 固定路径
	if (_strUnityExecutePath.IsEmpty())
	{
		return CPathConfig::GetSystemPath() + _T("UnityViewer\\MyProject1.exe");
	}

	return _strUnityExecutePath;
}

void CUnityViewPort::SetUnityExecutePath(const CString & strPath)
{
	_strUnityExecutePath = strPath;
}

void CUnityViewPort::CloseDialog()
{
	OnCancel();
}

BOOL CUnityViewPort::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	ShowWindow(SW_HIDE);
	Init();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

bool CUnityViewPort::AddUnityMsgListener(IUnityMsgListener * pListener)
{
	if (pListener == NULL)
	{
		return false;
	}
	_listenerList.push_back(pListener);
}

bool CUnityViewPort::RemoveUnityMsgListener(IUnityMsgListener * pListener)
{
	if (pListener == NULL)
	{
		return false;
	}
	auto iter = std::find(_listenerList.begin(), _listenerList.end(), pListener);
	if (iter == _listenerList.end())
	{
		return false;
	}

	_listenerList.erase(iter);
	return true;
}

bool CUnityViewPort::ClearUnityMsgListener(IUnityMsgListener * pListener)
{
	_listenerList.clear();
	return true;
}

void CUnityViewPort::SendMsgToUnity(const std::string szMsg)
{
	send(_socket, szMsg.data(), strlen(szMsg.data()), 0);
}

void CUnityViewPort::SendMsgToSelf(const std::string szMsg)
{
	// mfc要进行的处理
	for (auto &listener : _listenerList)
	{
		listener->OnReceiveMessage(szMsg);
	}
}

HANDLE CUnityViewPort::StartProcess(LPCTSTR program, LPCTSTR args)
{
	HANDLE hPro = NULL;
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	::ZeroMemory(&startupInfo, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);
	//设置进程创建时不显示窗口
	startupInfo.dwFlags = /*STARTF_USEPOSITION*/STARTF_USESHOWWINDOW;
	/*startupInfo.dwX = 600;
	startupInfo.dwY = 900;*/
	startupInfo.wShowWindow = SW_HIDE;
	if (::CreateProcess(program, (LPTSTR)args,
		NULL,  // process security
		NULL,  // thread security
		FALSE, // no inheritance
		0,     // no startup flags
		NULL,  // no special environment
		NULL,  // default startup directory
		&startupInfo,
		&processInfo))
	{
		//延迟0.5s，等待进程创建成功
		Sleep(500);

		hPro = processInfo.hProcess;

	}
	return hPro;
}

bool CUnityViewPort::Init()
{
	ModifyStyle(NULL, UNITY_WND_STYLE | SS_NOTIFY);

	// 是否在unity下进行调试[在该处下断点然后往下拖即可]
	bool bTest = false;

	// 嵌入unity
	if (!bTest && !EmbedUnity())
	{
		return false;
	}

	// 连接Unity
	InitSocket();
	Connect();

	return true;
}

void CUnityViewPort::OnDestroy()
{
	CBaseDialog::OnDestroy();

	// 关闭线程句柄
	CloseHandle(_hMsgLoopThread);
	CloseHandle(_hSocketConnectThread);

	// 结束unity进程
	TerminateProcess(hProcess, 0);
	WaitForSingleObject(hProcess, INFINITE);

	//卸载Winsock库，并释放所有资源
	WSACleanup();   

	apphWnd = NULL;
}


void CUnityViewPort::OnSize(UINT nFlags, int cx, int cy)
{
	// 更新窗口apphWnd
	CBaseDialog::OnSize(nFlags, cx, cy);
	_pUnityWnd = CWnd::FromHandle(apphWnd);
	if (_pUnityWnd)
	{
		bool bVisible = _pUnityWnd->IsWindowVisible();
		auto pParent  = _pUnityWnd->GetParent();
		bool bIsChild = _pUnityWnd->GetParent() == CUnityViewPort::Instance();
		static bool bTest = true;
		if (!bVisible && bTest)
		{
			// 容错，保证unity视口显示
			SetWindowLong(apphWnd, GWL_STYLE, WS_VISIBLE);//eraze title of ms paint window.
			_pUnityWnd->SetParent(CUnityViewPort::Instance());
			_pUnityWnd->BringWindowToTop();
			_pUnityWnd->ShowWindow(SW_SHOW);
		}

		_pUnityWnd->SetWindowPos(&wndTop, 0, 0, cx, cy, 0);
	}
}

void CUnityViewPort::RestartUnity()
{
	// 清理Unity进程
	KillUnityTask();

	//获取Unity应用程序目录
	CString strPath = GetUnityExecutePath();
	if (!CFileToolkitEx::FileExist(strPath))
	{
		MessageBox(L"File Not Exist!");
		return;
	}

	hProcess = StartProcess(strPath, L"");//Start ms paint
}

bool CUnityViewPort::EmbedUnity()
{
	CRect rect;
	GetClientRect(&rect);

	// 打开unity程序
	RestartUnity();

	long long startTick = GetTickCount();
	while (apphWnd == NULL)
	{
		// 两秒没找到unity窗口就重启unity
		int during = ((long long)GetTickCount()) - startTick;
		if (during > 2000)
		{
			startTick = GetTickCount();
			RestartUnity();
		}

		CWnd *pWnd = FindWindow(L"UnityWndClass", L"MyProject001");
		if (pWnd)
		{
			apphWnd = pWnd->GetSafeHwnd();
			pWnd->ModifyStyle(NULL, UNITY_WND_STYLE);
		}
	}

	if (apphWnd == NULL)
	{
		MessageBox(L"Cannot find Window");
		return false;
	}

	//::SetParent(apphWnd, m_hWnd);//set parent of ms paint to our dialog.
	_pUnityWnd = CWnd::FromHandle(apphWnd);
	_pUnityWnd->ModifyStyle(NULL, UNITY_WND_STYLE | WM_NOTIFY);
	_pUnityWnd->SetParent(this);
	SetWindowLong(apphWnd, GWL_STYLE, WS_VISIBLE);//eraze title of ms paint window.
													//Positioning ms paint.
	::MoveWindow(apphWnd, rect.left, rect.top, rect.right, rect.bottom, true);
	//::SendMessage(apphWnd, WM_SIZE, 0, 0);
	//窗口重绘，（因创建exe时，设置为SW_HIDE,导致exe窗口会被父窗口覆盖一部分)
	Invalidate();
	::UpdateWindow(apphWnd);
	_pUnityWnd->ShowWindow(SW_SHOW);
	_pUnityWnd->BringWindowToTop();

	bool bVisible = _pUnityWnd->IsWindowVisible();
	auto pParent = _pUnityWnd->GetParent();
	bool bIsChild = _pUnityWnd->GetParent() == CUnityViewPort::Instance();
	
	CString strDebug; 
	strDebug.Format(L"\n_pUnityWnd:%X\n已完成unity初始化\nbVisible : %s\npParent : %s\nbIsChild  : %s", _pUnityWnd, bVisible ? L"是" : L"否", pParent ? L"是" : L"否", bIsChild ? L"是" : L"否");
	OutputDebugString(strDebug);

	return true;
}

BOOL CUnityViewPort::InitSocket()
{
	// TODO: 在此添加额外的初始化代码
	WSADATA wsaData;
	::WSAStartup(MAKEWORD(2, 2), &wsaData);
	//初始化套接字
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == _socket)
	{
		MessageBox(L"套接字创建失败！");
		return FALSE;
	}

	char name[155];//定义用于存放获得主机名的变量
	PHOSTENT hostinfo; //调用MAKEWORD()获得Winsocl版本的正确值，用于下面的加载Winscok库
	if (gethostname(name, sizeof(name)) == 0)
	{
		//这是获取主机，如果获得主机名成功的话，将返回一个指针，指向hostinfo,hostinfo为PHOSTENT型的变量。
		if ((hostinfo = gethostbyname(name)) != NULL)
		{
			struct in_addr * stAddress = (struct in_addr *)*hostinfo->h_addr_list;
			_ipAddress = (DWORD)stAddress->s_addr; //inet_addr()函数把地址串转换为IP地址
		}
	}

	return TRUE;
}

BOOL CUnityViewPort::Connect()
{
	// 临时保存IP
	char cHostName[256] = { 0 };
	gethostname(cHostName, sizeof(cHostName));
	struct hostent *pHost = gethostbyname(cHostName);
	in_addr addr;
	memcpy(&addr, pHost->h_addr_list[0], sizeof(in_addr));
 
	// 启动socket连接线程
	unsigned int sockConnectthreadID;
	_infoSocketConnect._pSocket = &_socket;
	_infoSocketConnect._addrTo.sin_family = AF_INET;
	_infoSocketConnect._addrTo.sin_port = htons(8000); // .sin_port = 非0值,Windows防火墙会弹出提示
	_infoSocketConnect._addrTo.sin_addr.S_un.S_addr = addr.s_addr;
	_hSocketConnectThread = (HANDLE)_beginthreadex(NULL, 0, &SocketConnectLoop, (LPVOID)&_infoSocketConnect, 0, &sockConnectthreadID);
	
	// 启动消息响应线程
	unsigned int threadID;
	_infoMsgLoop._pSocket = &_socket;
	_infoMsgLoop._pListenerList= &_listenerList;
	_hMsgLoopThread = (HANDLE)_beginthreadex(NULL, 0, &MessageLoop, (LPVOID)&_infoMsgLoop, 0, &threadID);

	return TRUE;
}

BOOL CUnityViewPort::KillUnityTask()
{
	CString strProcessName = CFileToolkitEx::GetFileLogicName(GetUnityExecutePath());
	if (strProcessName.IsEmpty())
	{
		return FALSE;
	}

	HANDLE handle32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (INVALID_HANDLE_VALUE == handle32Snapshot)
	{
		return FALSE;
	}

	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(PROCESSENTRY32);

	//Search for all the process and terminate it

	if (Process32First(handle32Snapshot, &pEntry))
	{
		BOOL bFound = FALSE;
		if (!_tcsicmp(pEntry.szExeFile, strProcessName))
		{
			bFound = TRUE;
		}
		while ((!bFound) && Process32Next(handle32Snapshot, &pEntry))
		{
			if (!_tcsicmp(pEntry.szExeFile, strProcessName))
			{
				bFound = TRUE;
			}
		}
		if (bFound)
		{
			CloseHandle(handle32Snapshot);
			HANDLE handLe = OpenProcess(PROCESS_TERMINATE, FALSE, pEntry.th32ProcessID);
			BOOL bResult = TerminateProcess(handLe, 0);
			WaitForSingleObject(hProcess, INFINITE);
			return bResult;
		}
	}
	CloseHandle(handle32Snapshot);
	return FALSE;
}

void CUnityViewPort::RemoveUnityDlg()
{
	if (_pUnityWnd == NULL)
	{
		return;
	}
	_pUnityWnd->SetParent(NULL);
	_pUnityWnd->ShowWindow(SW_HIDE);
}


void CUnityViewPort::TakeBackUnityDlg()
{
	if (_pUnityWnd == NULL || _pUnityWnd->GetSafeHwnd() == NULL)
	{
		return;
	}
	_pUnityWnd->SetParent(this);
	_pUnityWnd->ShowWindow(SW_SHOW);
}

unsigned __stdcall CUnityViewPort::MessageLoop(void * pParam)
{
	UNITY_MESSAGE_LOOP_INFO *pInfo = (UNITY_MESSAGE_LOOP_INFO *)pParam;
	SOCKET *pSocket = pInfo->_pSocket;
	std::list<IUnityMsgListener *> *pListenerList = pInfo->_pListenerList;
	if (pInfo == NULL || pSocket == NULL || pListenerList == NULL)
	{
		ASSERT(false); // 初始参数错误
		return 0;
	}

	char szMsg[MSG_MAX_LENGTH];

	//发送,接收数据
	while (1) {
		memset(szMsg, 0, MSG_MAX_LENGTH);
		int recv_len = recv(*pSocket, szMsg, 100, 0);
		if (recv_len < 0) {
			Sleep(10);
			continue;
		}
		szMsg[recv_len] = 0;
		auto msgList = CStringToolkitEx::SplitString(szMsg, "@"); // 根据消息尾解析多个消息
		for (auto &msg : msgList)
		{
			// 防止最后一个消息尾被解析为空消息
			if (msg.empty())
			{
				continue;
			}

			if (strcmp(msg.data(), "has recieved") == 0)
			{
				CUnityViewPort::Instance()->SendMsgToUnity("update");
				continue;
			}

			// mfc要进行的处理
			for (auto &listener : *pListenerList)
			{
				listener->OnReceiveMessage(msg);
			}
		}
	}
	return 0;
}

unsigned __stdcall CUnityViewPort::SocketConnectLoop(void * pParam)
{
	SOCKET_CONNECT_INFO *pInfo = (SOCKET_CONNECT_INFO *)pParam;
	if (pInfo == NULL || pInfo->_pSocket == NULL)
	{
		ASSERT(false); // 初始参数错误
		return 0;
	}

	int iResult = SOCKET_ERROR;
	while (iResult == SOCKET_ERROR)
	{
		Sleep(10);
		iResult = connect(*pInfo->_pSocket, (SOCKADDR*)&pInfo->_addrTo, sizeof(pInfo->_addrTo));
	}

	//AfxMessageBox(L"CUnityViewPort连接服务器成功!");

	//if (iResult == SOCKET_ERROR)
	//{
	//	MessageBox(L"CUnityViewPort连接服务器不成功!");
	//	WSACleanup();
	//	return FALSE;
	//}

	return 0;
}

void CUnityViewPort::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	OutputDebugString(L"");
}


void CUnityViewPort::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	OutputDebugString(L"");
}


void CUnityViewPort::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_bMouseTracking = FALSE;
	CWnd *_pUnityWnd = CWnd::FromHandle(apphWnd);
	if (_pUnityWnd)
	{
		_pUnityWnd->SendMessage(WM_KILLFOCUS);
	}
	__super::OnMouseLeave();
}


void CUnityViewPort::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!_bMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;

		_bMouseTracking = _TrackMouseEvent(&tme);
	}
	__super::OnMouseMove(nFlags, point);
}
