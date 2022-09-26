//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           FFmpegRecorder.h
//    @author         yf
//    @data           2020/10/29  
//    @brief          屏幕录制类
//
//*****************************************************

#pragma once

#ifdef COMMONCONTROLS_API
#define CFFmpegRecorder_API COMMONCONTROLS_API
#else
#define CFFmpegRecorder_API 
#endif 

class CFFmpegRecorder_API CFFmpegRecorder
{
public:
	// 录屏信息
	struct stFFMPEGInfo
	{
		int _x;						// 窗口左上角x坐标
		int _y;						// 窗口左上角y坐标
		int _width;					// 窗口宽
		int _height;				// 窗口高
		BOOL _bMouse;				// 是否显示鼠标
		CString _strTitle;			// 窗口名称
		CString _tempPath;			// 临时路径
		CString _savePath;			// 保存路径
		int _fps;					// 帧率
		PROCESS_INFORMATION _pi;	// 进程信息
		CWnd *_pCurWnd;				// 当前窗口
		bool _bFullScreen;			// 是否全屏
		CString _strFFmpegPath;		// ffmpeg应用程序路径
	};	
public:
	CFFmpegRecorder();
	~CFFmpegRecorder();
public:

	/**
	*  @brief    设置ffmpeg应用程序路径
	*
	*  @param
	*  @return
	*/
	void SetFFmpegPath(const CString &strPath);

	/**
	*  @brief    获取ffmpeg应用程序路径
	*
	*  @param
	*  @return
	*/
	CString GetFFmpegPath();

	/**
	*  @brief    开始录像
	*
	*  @param    
	*  @return   
	*/
	bool StartRecording();

	/**
	*  @brief    结束录像
	*
	*  @param    
	*  @return   
	*/
	void StopRecording();

	/**
	*  @brief    设置截取屏幕区域
	*
	*  @param    rect 屏幕区域
	*  @return   
	*/
	void SetScreenRect(const CRect & rect);

	/**
	*  @brief    设置截取屏幕区域
	*
	*  @param    rect 屏幕区域
	*  @return
	*/
	void SetScreenRect(CWnd *pWnd);

	/**
	*  @brief    设置要抓取的窗口的标题
	*
	*  @param    const CString &strTitle
	*  @return
	*/
	void SetCatchWndTitle(const CString &strTitle);

	/**
	*  @brief    设置文件存储路径
	*
	*  @param    strSavePath 文件存储路径
	*  @return   
	*/
	void SetSaveFileName(const CString &strSavePath);

	/**
	*  @brief   设置默认帧率 
	*
	*  @param    nFPS 每秒多少帧
	*  @return   
	*/
	void SetFPS(int nFPS);

	/**
	*  @brief   获取默认帧率
	*
	*  @param    nFPS 每秒多少帧
	*  @return
	*/
	int GetFPS();

	/**
	*  @brief   设置全屏
	*
	*  @param    bFullScreen 是否全屏
	*  @return
	*/
	void SetFullScreen(bool bFullScreen = true);

	/**
	*  @brief   显示鼠标
	*
	*  @param    bShow 是否显示
	*  @return
	*/
	void ShowMouse(bool bShow = true);

	/**
	*  @brief    获取当前窗口指针
	*
	*  @return   CWnd *
	*/
	CWnd * GetCurWnd();

private:
	/**
	*  @brief    转码压缩视屏
	*
	*  @return   BOOL
	*/
	BOOL TranscodingVideo();

	/**
	*  @brief   抓取线程
	*
	*  @param
	*  @return
	*/
	static unsigned __stdcall ReleaseThread(void *pParam);

	/**
	*  @brief    释放
	*
	*  @return   BOOL
	*/
	BOOL Release();
private:
	stFFMPEGInfo _ffmpegInfo; // 录屏信息
	HANDLE _releaseThread;	// 释放句柄
};

