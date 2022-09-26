#include "stdafx.h"
#include "..\include\OnlyOneInstance.h"

using namespace BwProj::toolkit;

COnlyOneInstance::COnlyOneInstance (LPCTSTR MutexName)
	{
		_Mutex = CreateMutex( NULL, FALSE, MutexName);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			ReleaseMutex(_Mutex);
			::CloseHandle (_Mutex);
			_Mutex = NULL;
		}
	}

COnlyOneInstance::~COnlyOneInstance (void)
{
	if (_Mutex)
	{
		ReleaseMutex(_Mutex);
		::CloseHandle (_Mutex);
		_Mutex = NULL;
	}
}

/**
* 返回设置单实例是否成功
* @return 返回设置单实例是否成功
*/
bool COnlyOneInstance::GetSuccess (void)
{
	return _Mutex != NULL;
}
