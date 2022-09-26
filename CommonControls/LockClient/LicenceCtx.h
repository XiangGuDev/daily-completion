
#pragma once

#pragma pack (push, 8)

//实例上下文
typedef struct tag_AppCtx
{
	// 这里使用(解决32与64位内存共享大小问题)
	INT hWnd;		// 当前运行的CAD主窗口句柄
	TCHAR filePath[MAX_PATH * 2] = {0}; // 正在打开的文件路径
	INT hPrjDesignWnd; // 项目设计主界面

	tag_AppCtx()
	{
		hWnd = NULL;
		hPrjDesignWnd = NULL;
	}

	void SetFilePath(const CString& filePathIn)
	{
		lstrcpyn(filePath, filePathIn.GetString(), MAX_PATH * 2);
	}
}AppCtx;

//登陆上下文
typedef struct tag_LoginCtx
{
	int  nlockLicMode;   // 软件启动锁许可模式
	bool bInitSuccess;	// 软件插件是否启动成功

	tag_LoginCtx()
	{
		nlockLicMode = 1; //LOCK_LIC_MODE1
		bInitSuccess = false;
	}
}LoginCtx;

#pragma pack (pop)

// 通知项目退出设计界面
#define UM_QUIT_APP (WM_USER+1400)

// 各模式采用或进行组合使用
#define LOCK_LIC_MASTER		1	// 主许可证检测标记
#define LOCK_LIC_MODE1		2	// 启动程序锁许可证模式1
#define LOCK_LIC_MODE2	4	// 启动程序锁许可证模式2
#define LOCK_LIC_MODE3	8	// 启动程序锁许可证模式3

class CLockTool
{
public:
	CLockTool(){}

	static CString GetLockCfgFile();

};