#pragma once

#include "LicenceCtx.h"

class COMMONCONTROLS_API CLockClientService
{
public:
	DECLARE_SINGLE_CLASS(CLockClientService)

public:
	CLockClientService();
	~CLockClientService();

public:
	/**
	*  @brief    检查指定的许可证是否存在;
	*
	*  @param    nMode 代表可证的宏定义，参考GetSupportLicenceByMode实现
	*/
	static bool CheckLicence(int nMode = LOCK_LIC_MASTER);

public:

// 	/**
// 	*  @brief    标记初始化共享内存
// 	*
// 	*  @return   void
// 	*/
// 	void InitStartFlagAndInstance();

	/**
	*  @brief    开始锁检测
	*
	*  @return   void
	*/
	void InitLockDetect();

	static UINT LockInitial(LPVOID lpParam);
	
	static void CALLBACK LockDetectProc(HWND, UINT, UINT_PTR, DWORD);

	void LockFirst();

	/**
	*  @brief    function_description
	*
	*  @return   void
	*/
	void UninitLockDetect();

public:
	CWinThread*	_pLockInitThread; ///<锁检测线程
	static UINT_PTR s_nCheckTm; ///< 检测锁定时器
	static int s_nLockInitStatus; ///< 锁初始化状态
	static CAtlFileMapping<AppCtx> s_instanceMapping; ///< 标记软件已启动
	static bool _bNetLogin; // 是否使用网络锁登陆
};
