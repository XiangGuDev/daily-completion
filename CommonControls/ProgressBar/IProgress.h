//*****************************************************
//    
//    @copyright      三维设计组
//    @version        v1.0
//    @file           IProgress.H
//    @author         ZSX
//    @data           2018/8/29 11:48
//    @brief          进度响应
//*****************************************************

#pragma once

//messagebox样式
typedef struct MessageBoxInfo
{
	CString strContent;            //内容
	CString strTitle = _T("提示");  //标题
	UINT uType = MB_OK;            //按键组合
	LRESULT lResult = 0;           //MessageBox返回值
}MessageBoxInfo; 

// 模态框信息
typedef struct ModalProgressInfo
{
	ModalProgressInfo() :
		hParent(0), hProgressHwnd(0), bComplete(false),
		bAutoAdd(true), bPause(false), nCurStep(0), nMaxStep(100),
		bChangeText(false), strTitle(L""), strText(L"正在加载，请稍等..."),
		pDefinePara(NULL), pThreadPt(NULL)
	{}

	HWND hParent;		// 模态框父窗口句柄
	HWND hProgressHwnd; // 进度条窗口句柄
	bool bComplete;		// 完成标志位
	
	bool bAutoAdd;		// 自动增加进度（不读nCurStep）
	bool bPause;        // 暂停进度
	int nCurStep;		// 当前进度
	int nMaxStep;		// 最大进度

	bool bChangeText;	// 需要修改文字
	CString strTitle;	// 标题文字
	CString strText;	// 内容文字

	void *pDefinePara;  // 自定义参数
	_beginthreadex_proc_type pThreadPt;  // 线程执行函数指针
}ModalProgressInfo;



/**
*  @class    IProgressListen
*
*  @brief    进度监听接口
*/
class COMMONCONTROLS_API IProgressListener
{
public:
	virtual void OnStep(int nStep) = 0;
};

/**
*  @class    IProgress
*
*  @brief    进度接口
*/
class COMMONCONTROLS_API IProgress
{
public:
	IProgress();

	virtual ~IProgress();

public:
	/**
	*  @brief    进度开始
	*
	*  @return   void
	*/
	virtual void OnStart();

	/**
	*  @brief    任务停止
	*
	*  @return   void
	*/
	virtual void OnStop();

	/**
	*  @brief    进度完成
	*
	*  @return   void
	*/
	virtual void OnComplete();

	/**
	*  @brief    设置进度
	*
	*  @param    USHORT step
	*  @return   void
	*/
	virtual void OnStep(int nStep);

	/**
	*  函数介绍 增加进度
	*
	*  函数参数 int nStep
	*  返回值   void
	*/
	virtual void IncrementStep(int nStep, int nMaxLimit = 100);

	/**
	*  @brief    获取进度
	*
	*  @return   int
	*/
	virtual int GetStep();

	/**
	*  函数介绍 是否完成
	*
	*  返回值   bool
	*/
	virtual bool IsComplete();

	/**
	*  @brief    设置监听响应
	*
	*  @param    IProgressListen* notify
	*  @return   void
	*/
	virtual void SetNotify(IProgressListener* pListener);

protected:
	int _useStep;					///< 进度
	IProgressListener *_pListener;	///< 绑定监听响应
};

class CProgressDlg;

class COMMONCONTROLS_API CProgressAgentStopEvent
{
public:
	CProgressAgentStopEvent() {};
	virtual ~CProgressAgentStopEvent(){};
public:

	//启动停止
	virtual  bool StopParser() = 0;

	//获取是否已停止
	virtual  bool IsStopedParser() = 0;

};

/**
*  @class    CProgress
*
*  @brief    进度条（会显示进度条对话框）
*/
class COMMONCONTROLS_API CProgressAgent : public IProgress
{
public:
	/**
	*  函数介绍 构造进度条(非模态)
	*
	*  函数参数 const CString& sTitle	标题
	*  函数参数 const CString& sText		显示内容
	*  函数参数 CWnd * pParent			父窗口
	*  返回值   
	*/
	CProgressAgent(const CString& sTitle = L"", 
		const CString& sText = L"正在加载，请稍等...", CWnd *pParent = NULL);
		
	virtual ~CProgressAgent();

public:

	/**
	*  函数介绍 构造进度条(模态卡主线程)
	*
	*  函数参数 ModalProgressInfo& proInfo
	*  函数参数 bCancel			是否可以中断线程
	*  返回值
	*/
	static int ModalProgress(ModalProgressInfo& proInfo, bool bCancel = false, CProgressAgentStopEvent* pEvent = nullptr);

	/*
	* 函数介绍： 进度条弹模态Messagebox
	* 输入参数： ModalProgressInfo& proInfo 进度条窗口信息
	* 输入参数： CString strContent messagebox内容
	* 输入参数： CString strTitle messagebox标题
	* 输入参数： UINT uType messagebox按钮组合
	* 返回值: LRESULT messagebox返回值
	*/
	static LRESULT ModalMessageBox(ModalProgressInfo* proInfo, CString strContent, CString strTitle, UINT uType);

private:
	CProgressDlg* _pdlg;	// 进度条对话框
	CProgressAgentStopEvent * pStopEvent;//中止处理消息
};
