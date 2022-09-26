

namespace BwProj{ namespace toolkit {

template <bool t_bManaged>
CThreadT<t_bManaged>::CThreadT(HANDLE hThread = NULL, DWORD dwThreadId = 0)
: m_hThread(hThread), m_dwThreadId(dwThreadId)
{
#if WINVER >= 0x0502
	if ( m_hThread != NULL && m_dwThreadId == 0 )
		m_dwThreadId = ::GetThreadId(m_hThread);
#endif
}

template <bool t_bManaged>
CThreadT<t_bManaged>::CThreadT(const CThreadT& otherThread)
{
	m_dwThreadId = otherThread.m_dwThreadId;
	if ( t_bManaged && otherThread.m_hThread != NULL )
	{
		HANDLE hProcess = GetCurrentProcess();
		DuplicateHandle( hProcess, otherThread.m_hThread, hProcess, &m_hThread,
			0, FALSE, DUPLICATE_SAME_ACCESS );
		ATLASSERT( m_hThread != NULL );
	}
	else
		m_hThread = otherThread.m_hThread;
}

template <bool t_bManaged>
CThreadT<t_bManaged>::~CThreadT()
{
	if ( t_bManaged && m_hThread != NULL )
		CloseHandle(m_hThread);
}

template <bool t_bManaged>
CThreadT<t_bManaged> CThreadT<t_bManaged>::Create(LPTHREAD_START_ROUTINE pThreadProc, LPVOID pParam = NULL,
					   DWORD dwCreationFlags = 0, LPSECURITY_ATTRIBUTES pSecurityAttr = NULL,
					   DWORD dwStackSize = 0)
{
	DWORD dwThreadId = 0;
#ifdef _ATL_MIN_CRT
	HANDLE hThread = CreateThread(pSecurityAttr, dwStackSize, pThreadProc, pParam,
		dwCreationFlags, &dwThreadId);
#else
	HANDLE hThread = (HANDLE) _beginthreadex(pSecurityAttr, dwStackSize,
		(unsigned (__stdcall*)(void*)) pThreadProc,
		pParam, dwCreationFlags, (unsigned*) &dwThreadId);
#endif
	return CThreadT(hThread, dwThreadId);
}

template <bool t_bManaged>
CThreadT<t_bManaged> CThreadT<t_bManaged>::Open(DWORD dwThreadId, DWORD dwDesiredAccess,
					 BOOL bInheritHandle)
{
	HANDLE hThread = OpenThread(dwDesiredAccess, bInheritHandle, dwThreadId);
	return CThreadT(hThread, dwThreadId);
}


template <bool t_bManaged>
DWORD CThreadT<t_bManaged>::GetId() const
{
	return m_dwThreadId;
}

template <bool t_bManaged>
HANDLE CThreadT<t_bManaged>::GetHandle() const
{
	return m_hThread;
}

template <bool t_bManaged>
int CThreadT<t_bManaged>::GetPriority() const
{
	ATLASSERT( m_hThread != NULL );
	return GetThreadPriority(m_hThread);
}

template <bool t_bManaged>
BOOL CThreadT<t_bManaged>::SetPriority(int nPriority)
{
	ATLASSERT( m_hThread != NULL );
	return SetThreadPriority(m_hThread, nPriority);
}

template <bool t_bManaged>
DWORD CThreadT<t_bManaged>::GetExitCode() const
{
	ATLASSERT( m_hThread != NULL );
	DWORD dwExitCode = 0;
	if ( GetExitCodeThread(m_hThread, &dwExitCode) )
		return dwExitCode;
	else
		return (DWORD) -1;
}
template <bool t_bManaged>
BOOL CThreadT<t_bManaged>::HasExit (void) const
{
	DWORD code = 0;
	code = this->GetExitCode ();

	if (code != -1 && code != STILL_ACTIVE)
		return TRUE;
	else
		return FALSE;
}
template <bool t_bManaged>
BOOL CThreadT<t_bManaged>::GetThreadTimes(LPFILETIME pCreationTime, LPFILETIME pExitTime, LPFILETIME pKernelTime,
					LPFILETIME pUserTime) const
{
	ATLASSERT( m_hThread != NULL );
	return ::GetThreadTimes(m_hThread, pCreationTime, pExitTime, pKernelTime, pUserTime);
}


template <bool t_bManaged>
BOOL CThreadT<t_bManaged>::IsIOPending() const
{
	ATLASSERT( m_hThread != NULL );
	BOOL bIOPending = FALSE;
	GetThreadIOPendingFlag(m_hThread, &bIOPending);
	return bIOPending;
}


template <bool t_bManaged>
DWORD CThreadT<t_bManaged>::Resume()
{
	ATLASSERT( m_hThread != NULL );
	return ResumeThread(m_hThread);
}

template <bool t_bManaged>
DWORD CThreadT<t_bManaged>::Suspend()
{
	ATLASSERT( m_hThread != NULL );
	return SuspendThread(m_hThread);
}

template <bool t_bManaged>
BOOL CThreadT<t_bManaged>::Terminate(DWORD dwExitCode = 0)
{
	ATLASSERT( m_hThread != NULL );
	return TerminateThread(m_hThread, dwExitCode);
}

template <bool t_bManaged>
void CThreadT<t_bManaged>::Exit(DWORD dwExitCode = 0)
{
	// Make sure this is only called from the thread that this object represents
	ATLASSERT( m_dwThreadId == ::GetCurrentThreadId() );

#ifdef _ATL_MIN_CRT
	ExitThread(dwExitCode);
#else
	_endthreadex(dwExitCode);
#endif
}

template <bool t_bManaged>
DWORD CThreadT<t_bManaged>::Join(DWORD dwWaitMilliseconds = INFINITE) const
{
	ATLASSERT( m_hThread != NULL );
	return WaitForSingleObject(m_hThread, dwWaitMilliseconds);
}

template <class T>
/*__declspec(naked) */DWORD WINAPI _ThreadProcThunk(void* lpParameter)
{
	T* pThis = (T*)(lpParameter);
	return pThis->Run();
	//__asm
	//{
	//	pop		eax
	//	pop		ecx
	//	push	eax
	//	xor		eax, eax
	//	jmp T::Run
	//}
}

template <class T, class TBase>
CThreadImpl<T, TBase>::CThreadImpl(DWORD dwCreationFlags) : TBase(NULL, NULL)
{
	m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) _ThreadProcThunk<T>,
		this, dwCreationFlags, &m_dwThreadId);
}

//template <bool t_bManaged>
//CGuiThreadT<t_bManaged>:: CGuiThreadT(HANDLE hThread, DWORD dwThreadId)
//: baseClass(hThread, dwThreadId)
//{ }
//template <bool t_bManaged>
//CGuiThreadT<t_bManaged>:: CGuiThreadT(const CThreadT& otherThread)
//: baseClass(otherThread)
//{ }
//template <bool t_bManaged>
//BOOL CGuiThreadT<t_bManaged>:: PostThreadMessage(UINT uMsg, WPARAM wParam , LPARAM lParam )
//{
//	DWORD dwThreadId = GetId();
//	ATLASSERT( dwThreadId != 0 );
//	return ::PostThreadMessage( dwThreadId, uMsg, wParam, lParam );
//}
//template <bool t_bManaged>
//BOOL CGuiThreadT<t_bManaged>:: PostQuitMessage()
//{
//	return PostThreadMessage(WM_QUIT);
//}

//template <class T, class TBase>
//CGuiThreadImpl<T, TBase>::CGuiThreadImpl(WTL::CAppModule* pModule, DWORD dwCreationFlags = 0)
//: m_pModule(pModule), CThreadImpl<T, TBase>(dwCreationFlags)
//{
//	ATLASSERT( pModule != NULL );
//}
//template <class T, class TBase>
//BOOL CGuiThreadImpl<T, TBase>::InitializeThread()
//{
//	return TRUE;
//}
//template <class T, class TBase>
//void CGuiThreadImpl<T, TBase>::CleanupThread(DWORD dwExitCode)
//{ }
//template <class T, class TBase>
//BOOL CGuiThreadImpl<T, TBase>::PreTranslateMessage(MSG* pMsg)
//{
//	if ( pMsg->hwnd != NULL )
//		return FALSE;
//
//	T* pT = static_cast<T*>(this);
//	LRESULT lResult = 0;
//	return pT->ProcessWindowMessage( NULL, pMsg->message, pMsg->wParam,
//		pMsg->lParam, lResult, 0 );
//}
//template <class T, class TBase>
//DWORD CGuiThreadImpl<T, TBase>::Run()
//{
//	CMessageLoop msgLoop;
//	m_pModule->AddMessageLoop(&msgLoop);
//
//	msgLoop.AddMessageFilter(this);
//
//	T* pT = static_cast<T*>(this);
//	if ( !pT->InitializeThread() )
//	{
//		m_pModule->RemoveMessageLoop();
//		return 1;
//	}
//
//	DWORD dwExitCode = msgLoop.Run();
//
//	pT->CleanupThread(dwExitCode);
//
//	m_pModule->RemoveMessageLoop();
//
//	return dwExitCode;
//}

}}// end namespace
