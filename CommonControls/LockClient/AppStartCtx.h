
#pragma once

#pragma pack (push, 8)

//实例上下文
typedef struct tag_AppstartCtx
{
	// 这里使用(解决32与64位内存共享大小问题)
	INT hWnd;		// 当前运行的CAD主窗口句柄
	TCHAR filePath[MAX_PATH * 2]; // 正在打开的文件路径
	INT hPrjDesignWnd; // 项目设计主界面
	bool bReaded;

	tag_AppstartCtx()
		:filePath(TEXT(""))
	{
		hWnd = NULL;
		hPrjDesignWnd = NULL;
		bReaded = false;
	}

	void SetFilePath(const CString& filePathIn)
	{
		lstrcpyn(filePath, filePathIn.GetString(), MAX_PATH * 2);
	}

	/**
	*  @brief    设置同步完成的标记
	*
	*  @return   void
	*/
	void SetReaded()
	{
		bReaded = true;
	}
}AppstartCtx;
