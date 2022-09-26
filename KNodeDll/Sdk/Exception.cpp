#include "StdAfx.h"
#include "Exception.h"

namespace CommBase
{
	Exception::Exception(LPCTSTR info, LPCTSTR detail)
	{
		_info = info;
		if (detail)
			_detail = detail;
	}

	Exception::~Exception()
	{
	}

	LPCTSTR	Exception::what(void)
	{
		return _info;
	}

	LPCTSTR	Exception::detail(void)
	{
		return _detail;
	}
}