#include "stdafx.h"
#include "..\include\CriticalSection2.h"

namespace BwProj {
	namespace toolkit {
		CCriticalSectionHandle::CCriticalSectionHandle(void)
		{
#ifdef _DEBUG
			memset(&_Critical, NULL, sizeof(CRITICAL_SECTION));
#endif
			::InitializeCriticalSection(&_Critical);
		}

		CCriticalSectionHandle::~CCriticalSectionHandle(void)
		{
			::DeleteCriticalSection(&_Critical);
		}

		void CCriticalSectionHandle::Enter(void)
		{
			try
			{
				::EnterCriticalSection(&_Critical);
			}
			catch (...)
			{
				ATLASSERT(FALSE);
			}
		}

		void CCriticalSectionHandle::Leave(void)
		{
			::LeaveCriticalSection(&_Critical);
		}

		CCriticalSectionSync::CCriticalSectionSync(CCriticalSectionHandle& csc)
		{
			_csc = &csc;
			_csc->Enter();
		}

		CCriticalSectionSync::~CCriticalSectionSync(void)
		{
			_csc->Leave();
		}
	}
}