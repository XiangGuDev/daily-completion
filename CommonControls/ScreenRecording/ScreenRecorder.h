//*****************************************************
//    
//    @copyright      
//    @version        v1.0
//    @file           CScreenRecording.h
//    @author         yf
//    @data           2020/10/29  
//    @brief          屏幕录制类
//
//*****************************************************

#pragma once

#include<memory>

//FFPEG定义类型
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVStream;
struct AVFrame;
struct AVPacket;
struct CCaptureScreen;
class COMMONCONTROLS_API CScreenRecorder
{
public:
	CScreenRecorder();

	/**
	*  @brief    带参构造
	*
	*  @param    strSavePath 保存路径
	*  @param    rect 截取矩形
	*  @param    pWnd 截取窗口
	*  @return   
	*/
	CScreenRecorder(const CString & strSavePath, const CRect & rect);
	~CScreenRecorder();
public:

	/**
	*  @brief    开始录像
	*
	*  @param    
	*  @return   
	*/
	void StartRecording();

	/**
	*  @brief    结束录像
	*
	*  @param    
	*  @return   
	*/
	void StopRecording();

	/**
	*  @brief    添加背景音乐
	*
	*  @param	stVideoFilePath 视频文件路径
	*  @param	strMusicFilePath 背景音乐文件路径
	*  @param	strTargetFilePath 合成文件生成路径
	*  @return
	*/
	void AddBGMusic(const CString &strVideoFilePath, const CString &strMusicFilePath, const CString &strTargetFilePath);

	/**
	*  @brief    获取上一个视频的保存路径
	*
	*  @return	CString
	*/
	CString GetLastVideoPath();

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
	*  @brief   获取字幕文字位置
	*
	*  @param
	*  @return
	*/
	CSize GetTextPos();

	/**
	*  @brief   设置字幕文字位置
	*
	*  @param
	*  @return
	*/
	void SetTextPos(const CSize &pos);

	/**
	*  @brief   获取字幕文字
	*
	*  @param
	*  @return
	*/
	const CString &GetText();

	/**
	*  @brief   设置字幕文字
	*
	*  @param
	*  @return
	*/
	void SetText(const CString &strText);

	/**
	*  @brief    初始化抓拍
	*
	*  @return   bool
	*/
	bool InitCapture();
	
	/**
	*  @brief    抓拍一帧
	*
	*  @return   bool
	*/
	bool Capture();
	
	/**
	*  @brief    帧写入文件
	*
	*  @return   bool
	*/
	bool WriteFrame();

	/**
	*  @brief    获取当前窗口指针
	*
	*  @return   CWnd *
	*/
	CWnd * GetCurWnd();
private:
	/**
	*  @brief   抓取线程
	*
	*  @param    
	*  @return
	*/
	static unsigned __stdcall CaptureThread(void *pParam);

	/**
	*  @brief   写入线程
	*
	*  @param    
	*  @return
	*/
	static unsigned __stdcall WriteThread(void *pParam);

	/**
	*  @brief   初始化线程数据
	*
	*  @param
	*  @return bool
	*/
	bool InitRecording();

	/**
	*  @brief   录像结束后数据处理
	*
	*  @param
	*  @return
	*/
	void AfterRecording();

	/**
	*  @brief   初始化数据
	*
	*  @param
	*  @return
	*/
	void InitData();

	
private:
	CString _strSaveFileName;	// 保存路径
	CRect _rect;				// 截屏矩形
	int _nDefFps;				// 额定帧率	
	int _nCurFps;				// 当前帧率
	volatile bool _bStop = true;// 是否停止线程
	bool _bFullScreen = true;	// 是否全屏
	bool _bShowMouse = true;	//是否显示鼠标
	HANDLE _capThread;			// 抓取线程句柄
	HANDLE _wrtThread;			// 写入线程句柄
private:

	std::queue<BYTE*> _ImageQueue;		//帧队列
	CRITICAL_SECTION _Section;			//帧队列临界量
	
	// 录屏相关属性
	int _Width = 0;
	int _Height = 0;

	CWnd *_pCurWnd; // 当前窗口

	//FFMPEG相关录屏对象
	AVFormatContext* _avFormCtx_Out = nullptr;
	AVCodecContext* _avCodecCtx_Out = nullptr;
	AVCodec* _avCodec = nullptr;
	AVStream* _avStream = nullptr;
	AVFrame* _avframe = nullptr;
	AVPacket* _packet = nullptr;
	BYTE* _outbuffer = nullptr;
	std::shared_ptr<CCaptureScreen> _CaptureScreen = nullptr;//抓取屏幕截图类
	int _frameNumber = 0;//当前帧序号

	// 帧率计算相关
	double _dDefOneFrameTime = 0; // 默认每帧时间
	long _nTotaltime = 0; // 总时间
	long _nStartTime = 0; // 开始时间
	long _nLastTime = 0; // 上次时间

	CString _strLastVideoPath; // 上一个视频的路径

	CSize _txtPos; // 文字位置
	CString _strText; // 字幕内容
};

