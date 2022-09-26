#include "stdafx.h"
#include "..\include\Exception.h"


using namespace BwProj;

Exception::Exception (LPCTSTR info, LPCTSTR detail)
{
	_info = info;
	if (detail)
		_detail = detail;
}

Exception::Exception(const Exception& e)
{
	_info = e.what();
	_detail = e.detail();
}

Exception::~Exception () {}

LPCTSTR	Exception::what (void) const
{
	return _info;
}

LPCTSTR	Exception::detail (void) const
{
	return _detail;
}
