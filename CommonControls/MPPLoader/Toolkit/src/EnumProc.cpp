#include "stdafx.h"
#include "..\include\EnumProc.h"

using namespace BwProj::toolkit;

BOOL CWindowIterator::EnumProc(HWND hwnd, LPARAM lp)
{
	return (reinterpret_cast<CWindowIterator*>(lp))->OnEnumProc(hwnd);
}

// virtual enumerator
BOOL CWindowIterator::OnEnumProc(HWND hwnd)
{
	if (OnWindow(hwnd)) {
		if (m_count < m_nAlloc)
			m_hwnds[m_count++] = hwnd;
	}
	return TRUE; // keep looking
}

// override to filter different kinds of windows
BOOL CWindowIterator::OnWindow(HWND hwnd)
{
	return TRUE;
}

CWindowIterator::CWindowIterator(DWORD nAlloc)
{
	assert(nAlloc>0);
	m_current = m_count = 0;
	m_hwnds = new HWND [nAlloc];
	m_nAlloc = nAlloc;
}

CWindowIterator::~CWindowIterator()
{
	delete [] m_hwnds;
}

DWORD CWindowIterator::GetCount() 
{ 
	return m_count;
}

HWND CWindowIterator::First()
{
	::EnumWindows(EnumProc, (LPARAM)this);
	m_current = 0;
	return Next();
}

HWND CWindowIterator::Next() 
{
	return m_hwnds && m_current < m_count ? m_hwnds[m_current++] : NULL;
}

CChildWindowIterator::CChildWindowIterator (HWND hWndParent)
{
	_hWndParent = hWndParent;
}

HWND CChildWindowIterator::First()
{
	::EnumChildWindows(_hWndParent, EnumProc, (LPARAM)this);
	m_current = 0;
	return Next();
}

BOOL CMainWindowIterator::OnWindow(HWND hwnd)
{
	if (m_HitHideWindow || GetWindowLongPtr(hwnd,GWL_STYLE) & WS_VISIBLE) {
		DWORD pidwin;
		GetWindowThreadProcessId(hwnd, &pidwin);
		if (pidwin==m_pid)
			return TRUE;
	}
	return FALSE;
}


CMainWindowIterator::CMainWindowIterator(DWORD pid, DWORD nAlloc, BOOL HitHideWindow)
	: CWindowIterator(nAlloc)
{
	m_pid = pid;
	m_HitHideWindow = HitHideWindow;
}

CMainWindowIterator::~CMainWindowIterator() {}

CProcessIterator::CProcessIterator()
{
	m_count = 0;
	m_current = 0;
	m_pids = NULL;
}

CProcessIterator::~CProcessIterator()
{
	delete [] m_pids;
}

DWORD CProcessIterator::GetCount() { return m_count; }

DWORD CProcessIterator::First()
{
	m_current = (DWORD)-1;
	m_count = 0;
	DWORD nalloc = 1024;
	do {
		delete [] m_pids;
		m_pids = new DWORD [nalloc];
		if (EnumProcesses(m_pids, nalloc*sizeof(DWORD), &m_count)) 
		{
			m_count /= sizeof(DWORD);
			m_current = 1; // important: skip IDLE process with pid=0.
		}
	} while (nalloc <= m_count);

	return Next();
}

DWORD CProcessIterator::Next() {
	return m_pids && m_current < m_count ? m_pids[m_current++] : 0;
}

CProcessModuleIterator::CProcessModuleIterator(DWORD pid)
{
	m_hModules = NULL;
	m_count = 0;
	m_current = 0;
	// open the process
	m_hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
}

CProcessModuleIterator::~CProcessModuleIterator()
{
	CloseHandle(m_hProcess);
	delete [] m_hModules;
}

BOOL CProcessModuleIterator::SetPrivilege()
{
	HANDLE hProcess = NULL, hToken = NULL;
	TOKEN_PRIVILEGES NewState = { 0 };
	LUID luidPrivilegeLUID = { 0 };
	hProcess = GetCurrentProcess();
	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
		return FALSE;
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidPrivilegeLUID))
		return FALSE;
	NewState.PrivilegeCount = 1;
	NewState.Privileges[0].Luid = luidPrivilegeLUID;
	NewState.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &NewState, NULL, NULL, NULL))
		return FALSE;
	return TRUE;
}

HANDLE CProcessModuleIterator::GetProcessHandle()	{ return m_hProcess; }
DWORD CProcessModuleIterator::GetCount()				{ return m_count; }

HMODULE CProcessModuleIterator::First()
{
	m_count = 0;
	m_current = (DWORD)-1; 
	m_hModules = NULL;
	if (m_hProcess) {
		DWORD nalloc = 1024;
		do {
			delete [] m_hModules;
			m_hModules = new HMODULE [nalloc];
			if (EnumProcessModules(m_hProcess, m_hModules,
				nalloc*sizeof(DWORD), &m_count)) {
					m_count /= sizeof(HMODULE);
					m_current = 0;
			}
		} while (nalloc <= m_count);
	}
	return Next();
}

HMODULE CProcessModuleIterator::Next() {
	return m_hProcess && m_current < m_count ? m_hModules[m_current++] : 0;
}