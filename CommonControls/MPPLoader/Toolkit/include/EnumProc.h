////////////////////////////////////////////////////////////////
// MSDN Magazine -- July 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio 6.0 and Visual Studio .NET
// Runs in Windows XP and probably Windows 2000 too.
//
#pragma once

#include <assert.h>
#include <psapi.h>

namespace BwProj { namespace toolkit {

//////////////////
// Iterate the top-level windows. Encapsulates ::EnumWindows.
//
class CWindowIterator {
protected:
	HWND*	m_hwnds;		// array of hwnds for this PID
	DWORD m_nAlloc;			// size of array
	DWORD m_count;			// number of HWNDs found
	DWORD m_current;		// current HWND

	static BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lp);
	// virtual enumerator
	virtual BOOL OnEnumProc(HWND hwnd);
	// override to filter different kinds of windows
	virtual BOOL OnWindow(HWND hwnd);
public:

	CWindowIterator(DWORD nAlloc=1024);

	~CWindowIterator();
	
	DWORD GetCount();

	virtual HWND First();

	HWND Next();

};

//////////////////
// iterate the child windows. Encapsulates ::EnumChildWindows.
// lch add
class CChildWindowIterator : public CWindowIterator
{
public:
	HWND _hWndParent;

	CChildWindowIterator (HWND hWndParent);
	virtual HWND First();
};

//////////////////
// Iterate the top-level windows in a process.
//
class CMainWindowIterator : public CWindowIterator  {
protected:
	DWORD m_pid;							// process id
	BOOL m_HitHideWindow;

	virtual BOOL OnWindow(HWND hwnd);
public:
	CMainWindowIterator(DWORD pid, DWORD nAlloc=1024, BOOL HitHideWindow = FALSE);
	~CMainWindowIterator();
};

//////////////////
// Process iterator -- iterator over all system processes
// Always skips the first (IDLE) process with PID=0.
//
class CProcessIterator {
protected:
	DWORD*	m_pids;			// array of procssor IDs
	DWORD		m_count;			// size of array
	DWORD		m_current;		// next array item
public:
	CProcessIterator();
	~CProcessIterator();
	DWORD GetCount();

	DWORD First();
	DWORD Next();
};

//////////////////
// Iterate the modules in a process. Note that the first module is the
// main EXE that started the process.
//
class CProcessModuleIterator {
protected:
	HANDLE	m_hProcess;			// process handle
	HMODULE*	m_hModules;			// array of module handles
	DWORD		m_count;				// size of array
	DWORD		m_current;			// next module handle
public:
	CProcessModuleIterator(DWORD pid);
	~CProcessModuleIterator();
	// Ã·…˝»®œﬁ
	static BOOL SetPrivilege();
	HANDLE GetProcessHandle();
	DWORD GetCount();
	HMODULE First();
	HMODULE Next();
};

}}//end namespace