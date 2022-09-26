#include "stdafx.h"
#include "Date.h"
#include "..\common\DateHelper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Date
//
Date::Date()
:COleDateTime(::_time64(NULL))
{
	// 默认是当前时间
}

Date::Date(DATE dtSrc)
:COleDateTime(dtSrc)
{
//#ifdef _DEBUG
//	toString();
//#endif
}

Date::Date(COleDateTime& dtSrc)
: COleDateTime(dtSrc)
{
//#ifdef _DEBUG
//	toString();
//#endif
}

Date::Date(time_t dtSrc)
: COleDateTime(dtSrc)
{
	ATLASSERT(GetStatus() == valid);

//#ifdef _DEBUG
//	toString();
//#endif
}

time_t Date::getTime()
{
	SYSTEMTIME systime = { 0 };
	VariantTimeToSystemTime(*this, &systime);

	CTime tm(systime);
	return tm.GetTime();
}

int Date::compareTo(Date* arg0)
{
	time_t l1 = getTime();
	time_t l2 = arg0->getTime();
	return l1 == l2 ? 0 : l1 < l2 ? -1 : 1;
}

CString Date::toString()
{
#ifdef _DEBUG
	_textDate = Format(L"%Y-%m-%d %H:%M:%S");
	return _textDate;
#else
	return Format(L"%Y-%m-%d %H:%M:%S");
#endif
}