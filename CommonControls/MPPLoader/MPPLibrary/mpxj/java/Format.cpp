#include "stdafx.h"
#include "Format.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// DateFormat
//

Format::Format(LPCTSTR fmt)
{
	_fmt = fmt;
}

DateFormat::DateFormat(LPCTSTR fmt)
:Format(fmt)
{
	_ignoreDate = false;
	_ignoreTime = false;
}

CString DateFormat::format(DatePtr value) const
{
	ATLASSERT(value != NULL);
	CString str = value->Format(_fmt);

	return str;
}

void DateFormat::setIgnoreDate(bool val)
{
	_ignoreDate = val;
}

void DateFormat::setIgnoreTime(bool val)
{
	_ignoreTime = val;
}

DatePtr DateFormat::parse(CString str) const
{
	COleDateTime date;
	str.Replace(L"T", L" ");
	int dwFlags = 0;
	if (_ignoreDate) dwFlags = VAR_TIMEVALUEONLY;
	if (_ignoreTime) dwFlags = VAR_DATEVALUEONLY;

	if (date.ParseDateTime(str, dwFlags))
	{
		// 如果忽略了日期，则设置为当前日期，防止 getTime()出错
		if (_ignoreDate)
		{
			COleDateTime cur = COleDateTime::GetCurrentTime();
			date.SetDateTime(cur.GetYear(), cur.GetMonth(), cur.GetDay(),
				date.GetHour(), date.GetMinute(), date.GetSecond());
		}

		return DatePtr (new Date(date));
	}
		
	ATLASSERT(FALSE);
	return DatePtr();// NULL;
}

NumberFormat::NumberFormat(LPCTSTR fmt)
:Format(fmt)
{
}

CString NumberFormat::format(double value) const
{
	CString s;
	s.Format(L"%f", value);
	return CStringToolkit::TrimNumberString(s);
}

double NumberFormat::parse(CString str) const
{
	ATLASSERT(false);
	return 0;
}