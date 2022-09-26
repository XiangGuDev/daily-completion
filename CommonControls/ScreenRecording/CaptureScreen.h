#ifndef _CCAPTURE_SCREEN_HH
#define _CCAPTURE_SCREEN_HH

#include<time.h>
#include <d3d9.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>

#include <tchar.h>
#include <winbase.h>
#include <winreg.h>
#include <Strsafe.h>

class CScreenRecorder;

//
// ---抓屏类----
//
class COMMONCONTROLS_API CCaptureScreen
{
public:
	CCaptureScreen(CScreenRecorder *pSR);
	CCaptureScreen(CScreenRecorder *pSR, int width, int height);
	~CCaptureScreen(void);

public:
	/*-----------定义外部调用函数-----------*/
	int Init();//初始化
	void GetSize(int &nWidth, int &nHeight); // 获取宽高
	BYTE* CaptureImage(); //抓取屏幕
	BYTE* CaptureImage(int left, int top, int width, int height); //抓取屏幕
	void ShowMouse(bool bShow = true); // 显示鼠标
private:
	/*-----------定义内部调用函数-----------*/
	void AddText(PRGBTRIPLE hdib, int left, int top, int width, int height); // 添加文字
	void* CaptureScreenFrame(int, int, int, int);//抓屏
	void DrawMouse(int left = 0, int top = 0); // 绘制鼠标
	HCURSOR FetchCursorHandle(); //获取鼠标光标

private:
	/*-----------定义私有变量-----------*/
	int m_width;
	int m_height;
	UINT   wLineLen;
	DWORD  dwSize;
	DWORD  wColSize;

	//设备句柄
	HDC hScreenDC;
	HDC hMemDC;
	//图像RGB内存缓存
	//位图头信息结构体
	BITMAPINFO pbi;

	HBITMAP hbm;
	//鼠标光标
	HCURSOR m_hSavedCursor;

	bool _bFullScreen = true;
	HCURSOR _hcur = nullptr;
	ICONINFO _iconinfo;
	bool _bShowMouse = true;
	CScreenRecorder *_pSR; // 录屏控制类指针
};

#endif //--_CCAPTURE_SCREEN_HH