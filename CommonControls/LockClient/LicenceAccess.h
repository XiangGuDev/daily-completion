//*****************************************************
//    
//    @copyright      配网设计组
//    @version        v1.0H
//    @file           LicenceAccess.H
//    @author         KG
//    @date           2015/09/14 15:26
//    @brief          锁组件访问
//*****************************************************

#pragma once

#include <map>
#include "ILicenceAccess.h"

//导入锁
#ifndef _WIN64
#import "..\..\..\ThirdPart\LockClientService\LockClientService.tlb" rename_namespace("BwLockClient")
#else
#import "..\..\..\ThirdPart\LockClientService\LockClientService64.tlb" rename_namespace("BwLockClient")
#endif

using namespace BwLockClient;

IUnknown* CreateLockObject(UUID uuidof);

// 支持的许可证列表
typedef std::map<int, CString> SupportLicMap;

/**
*  @class    锁组件访问类
*
*  @brief
*/
class CLicenceAccess
{
private:
	CLicenceAccess();

public:
	/*
	*	获取锁实例单例
	*/
	static CLicenceAccess* GetInstance();

	/*
	*	释放锁实例
	*/
	static void ReleaseInstance();

	/*
	*	获取本机锁服务
	*/
	static IBooLockClientServicePtr GetNatServer();

public:
	/*
	*	创建锁
	*/
	bool CreateLock();

	/*
	*	释放本地锁
	*/
	void ReleaseLock();

	/*
	*	测是锁是否已创建
	*/
	void IsLockCreate();

public:
	/*
	*	检测单击锁许可证情况
	*/
	bool CheckNatLock();

	/*
	*	检测网络锁许可证情况
	*/
	bool CheckNetLock();

	/*
	*	检测网络锁许可证情况
	*/
	bool AppNetLogin();

	/*
	*	获取错误信息
	*/
	CString GetLastError();

	/*
	*	设置锁模式
	*/
	void SetLockLicMode(int nMode);

	/*
	*	获取锁模式
	*/
	int GetLockLicMode();

	/*
	*	测试指定功能许可证是否存在
	*/
	bool CheckFunctionLicence(int nLockLicMode);

	/*
	*	获取支持的许可证列表
	*/
	SupportLicMap GetAppSupperLicenceMap();

private:
	/*
	*	取得本系统MODE对应的许可证字符
	*/
	CString GetSupportLicenceByMode(int nMode);

private:
	/*
	*	测试锁中是否通过
	*/
	bool CheckLicence(IBooLockClientManagerPtr pLockPtr, CString strLic,
		bool bAddLicence, bool bNativeLock);

	/*
	*	测试锁中是否存在指定的许可证
	*/
	bool IsLicenceExist(LPCTSTR licence, IBooLockClientManagerPtr pLockPtr, bool bNativeLock);

protected:
	bool _bNetLogin;	// 主许可证是否为网络登陆
	int _nLockLicMode;	// 锁许可证模式(多个模式组合)
	CString _strLastError;	// 错误信息
	IBooLockClientManagerPtr _pNetLock;	//网络锁
	IBooLockClientManagerPtr _pNatLock;	//本地锁
	static CLicenceAccess* s_pLicAccess; // 锁访问
};

/*
*	获取当前模块路径
*/
CString GetModulePath();

/*
*	从配置文件网络锁登陆信息
*/
void GetNetLockInfo(CString& strServer, CString& strAccount, CString& strPassword);
