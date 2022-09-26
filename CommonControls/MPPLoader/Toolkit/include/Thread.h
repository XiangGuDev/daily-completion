

// Thread classes for WTL
// Copyright (C) Till Krullmann.
//
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.TXT at the root of this distribution.
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license. You must not remove this notice, or
// any other, from this software.

#pragma once


#ifndef _ATL_MIN_CRT
#include <process.h>
#endif

namespace BwProj{ namespace toolkit {

/** @addtogroup CadreThread
* @{
*/

//! 线程对象
template <bool t_bManaged>
class CThreadT
{
protected:
	HANDLE m_hThread;
	DWORD m_dwThreadId;

public:
	CThreadT(HANDLE hThread = NULL, DWORD dwThreadId = 0);
	CThreadT(const CThreadT& otherThread);
	virtual ~CThreadT();
	static CThreadT Create(LPTHREAD_START_ROUTINE pThreadProc, LPVOID pParam = NULL,
		DWORD dwCreationFlags = 0, LPSECURITY_ATTRIBUTES pSecurityAttr = NULL,
		DWORD dwStackSize = 0);
	static CThreadT Open(DWORD dwThreadId, DWORD dwDesiredAccess = THREAD_ALL_ACCESS,
		BOOL bInheritHandle = FALSE);
	DWORD GetId() const;
	HANDLE GetHandle() const;
	int GetPriority() const;
	BOOL SetPriority(int nPriority);
	DWORD GetExitCode() const;

	// 测试是否退出
	BOOL HasExit (void) const;
	BOOL GetThreadTimes(LPFILETIME pCreationTime, LPFILETIME pExitTime, LPFILETIME pKernelTime,
		LPFILETIME pUserTime) const;
#if _WIN32_WINNT >= 0x0501
	BOOL IsIOPending() const;
#endif

	
	DWORD Resume();
	DWORD Suspend();
	BOOL Terminate(DWORD dwExitCode = 0);
	void Exit(DWORD dwExitCode = 0);
	DWORD Join(DWORD dwWaitMilliseconds = INFINITE) const;
};

/** 定义CThread类型 */
typedef CThreadT<true> CThread;
/** 定义CThreadHandle类型 */
typedef CThreadT<false> CThreadHandle;


//! 线程实现类
template <class T, class TBase = CThread>
class CThreadImpl : public TBase
{
	typedef CThreadImpl<T, TBase> thisClass;

public:
	CThreadImpl(DWORD dwCreationFlags = CREATE_SUSPENDED)/*: TBase(NULL, NULL)*/;
};


//#include <atlwin.h>
////#include <atlapp.h>
//
///**  CGuiThreadT */
//template <bool t_bManaged>
//class CGuiThreadT : public CThreadT<t_bManaged>
//{
//	typedef CThreadT<t_bManaged> baseClass;
//
//public:
//	const _ATL_MSG* m_pCurrentMsg;
//
//	CGuiThreadT(HANDLE hThread, DWORD dwThreadId);
//	CGuiThreadT(const CThreadT& otherThread);
//	BOOL PostThreadMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
//	
//	BOOL PostQuitMessage();
//};
//
//typedef CGuiThreadT<true> CGuiThread;
//typedef CGuiThreadT<false> CGuiThreadHandle;


///**  CGuiThreadImpl */
//template <class T, class TBase = CGuiThread>
//class CGuiThreadImpl :
//	public CThreadImpl<T, TBase>,
//	public CMessageFilter,
//	public CMessageMap
//{
//	DECLARE_EMPTY_MSG_MAP()
//
//private:
//	WTL::CAppModule* m_pModule;
//
//public:
//	CGuiThreadImpl(WTL::CAppModule* pModule, DWORD dwCreationFlags = 0)
//		/*: m_pModule(pModule), CThreadImpl<T, TBase>(dwCreationFlags)*/;
//
//	BOOL InitializeThread();
//	void CleanupThread(DWORD dwExitCode);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	DWORD Run();
//};

/** * @} */
}}// end namespace
/** @} */

#include "Thread.hpp"