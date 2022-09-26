//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           UnityViewPort.h
//    @author         yf
//    @data           2021/08/06  
//    @brief          unity视图
//
//*****************************************************

#pragma once

#define IDD_UNITY_VIEWPORT              126

// CUnityViewPort 对话框
class IUnityMsgListener;
class COMMONCONTROLS_API CUnityViewPort
	: public CBaseDialog
	, public CSingletonBase
{
	DECLARE_SINGLE_CLASS2(CUnityViewPort, L"CUnityViewPort")
	// 构造
public:
	CUnityViewPort(CWnd* pParent = NULL);	// 标准构造函数
	enum {
		IDD = IDD_UNITY_VIEWPORT
	};

	// 消息循环信息
	struct UNITY_MESSAGE_LOOP_INFO
	{
		SOCKET  *_pSocket;
		std::list<IUnityMsgListener *> *_pListenerList;
	};

	// socket连接信息
	struct SOCKET_CONNECT_INFO
	{
		SOCKET  *_pSocket;
		SOCKADDR_IN _addrTo;
	};

public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	/**
	* 添加unity消息监听器
	* @return
	*/
	bool AddUnityMsgListener(IUnityMsgListener *pListener);
	
	/**
	* 移除unity消息监听器
	* @return
	*/
	bool RemoveUnityMsgListener(IUnityMsgListener *pListener);

	/**
	* 清除所有unity消息监听器
	* @return
	*/
	bool ClearUnityMsgListener(IUnityMsgListener *pListener);

	/**
	* 向unity发出消息
	* @return
	*/
	void SendMsgToUnity(const std::string szMsg);

	/**
	* 向自身（mfc）发出消息
	* @return
	*/
	void SendMsgToSelf(const std::string szMsg);

	/**
	* 初始化
	* @return
	*/
	virtual bool Init();

	/**
	* 获取unity可执行路径
	* @return
	*/
	CString GetUnityExecutePath();

	/**
	* 设置unity可执行路径
	* @return
	*/
	void SetUnityExecutePath(const CString &strPath);

	/**
	* 关闭对话框
	* @return
	*/
	virtual void CloseDialog() override;

	/**
	* 重启unity
	* @return BOOL
	*/
	void RestartUnity();

	/**
	* 嵌入unity
	* @return BOOL
	*/
	bool EmbedUnity();

	/**
	* 移除unity并隐藏
	* @return BOOL
	*/
	void RemoveUnityDlg();

	/**
	* 移回unity窗口并显示
	* @return BOOL
	*/
	void TakeBackUnityDlg();
protected:
	/**
	* 初始化套接字
	* @return BOOL
	*/
	BOOL InitSocket();

	/**
	* 连接Unity
	* @return BOOL
	*/
	BOOL Connect();

	/**
	* 清除unity进程
	* @return BOOL
	*/
	BOOL KillUnityTask();

	/**
	* 消息循环unity线程
	* @return BOOL
	*/
	static unsigned __stdcall MessageLoop(void * pParam);

	/**
	* socket连接线程
	* @return BOOL
	*/
	static unsigned __stdcall SocketConnectLoop(void * pParam);
// 实现
protected:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	HANDLE StartProcess(LPCTSTR program, LPCTSTR args);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
	SOCKET  _socket; //套接字描述符变量
	HANDLE hProcess; // unity进程句柄
	static HWND apphWnd; // unity窗口句柄
	CString _strUnityExecutePath; // unity执行路径
	DWORD _ipAddress; // 本机ip

	std::list<IUnityMsgListener *> _listenerList; // 监听器列表
	HANDLE _hMsgLoopThread; // 消息循环线程
	HANDLE _hSocketConnectThread; // socket连接线程
	UNITY_MESSAGE_LOOP_INFO _infoMsgLoop; // 消息循环信息
	SOCKET_CONNECT_INFO _infoSocketConnect; // socket连接信息

	bool _bMouseTracking; // 是否检测鼠标
	CWnd *_pUnityWnd;
};
