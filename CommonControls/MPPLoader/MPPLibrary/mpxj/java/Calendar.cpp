#include "stdafx.h"
#include "Calendar.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// Calendar
// 基于COleDateTime、COleDateTimeSpan实现

Calendar::Calendar()
{
	// 设置为当前时间
	_time = ::_time64(NULL);

//#ifdef _DEBUG
//	toString();
//#endif
}

std::shared_ptr<Calendar> Calendar::getInstance()
{
	return CalendarPtr (new Calendar());
}

void Calendar::setTime(DatePtr date)
{
	ATLASSERT(date != NULL);
	if (date)
	{
		_time = date->getTime();
	}

	ATLASSERT(_time != 0);

//#ifdef _DEBUG
//	toString();
//#endif
}

DatePtr Calendar::getTime()
{
	return DatePtr(new Date(_time));
}

int Calendar::get(FIELD field)
{
	int value = 0;
	COleDateTime date(_time);
	switch (field)
	{
	case YEAR:
		value = date.GetYear();
		break;
	case DAY_OF_YEAR:
		value = date.GetDayOfYear();
		break;
	case DAY_OF_WEEK:
		value = date.GetDayOfWeek();
		break;
	case HOUR_OF_DAY:
		value = date.GetHour();
		break;
	case MINUTE:
		value = date.GetMinute();
		break;
	case SECOND:
		value = date.GetSecond();
		break;
	default:
		throw std::bad_exception();
	}

	return value;
}

void Calendar::set(FIELD field, int value)
{
	COleDateTime date(_time);

	switch (field)
	{
	case HOUR_OF_DAY:
		date.SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), value, date.GetMinute(), date.GetSecond());
		break;
	case MINUTE:
		date.SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), value, date.GetSecond());
		break;
	case SECOND:
		date.SetDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), date.GetHour(), date.GetMinute(), value);
		break;
	case DAY_OF_YEAR:
	{
		date.SetDateTime(date.GetYear(), 1, 1, date.GetHour(), date.GetMinute(), date.GetSecond());
		COleDateTimeSpan daySpan(value-1, 0, 0, 0);
		date += daySpan;
	}
		break;
	default:
		throw std::bad_exception();
	}

	_time = Date(date).getTime();

//#ifdef _DEBUG
//	toString();
//#endif
}

/**
* 得到指定字段的最大值
* @param field	目前只支持 DAY_OF_YEAR
*/
int Calendar::getActualMaximum(FIELD field)
{
	COleDateTime date(_time);

	int result = 0;

	switch (field)
	{
	case DAY_OF_YEAR:	// 返回一年的天数
	{
		COleDateTime date1(date.GetYear(), 1, 1, 0, 0, 0);
		COleDateTime date2(date.GetYear() + 1, 1, 1, 0, 0, 0);
		COleDateTimeSpan days = date2 - date1;
		result = (int)days.GetTotalDays();
	}
		break;
	default:
		throw std::bad_exception();
	}

	return result;
}

/**
* add()函数会在逻辑上改变其它字段，使结果正确。
* @param field	目前只支持 DAY_OF_YEAR
*/
void Calendar::add(FIELD field, int value)
{
	time_t old = _time;
	COleDateTime date(_time);

	int result = 0;

	switch (field)
	{
	case DAY_OF_YEAR:	// 日期加一天
	{
		COleDateTimeSpan span(value, 0, 0, 0);
		date += span;
	}
		break;
	default:
		throw std::bad_exception();
	}

	_time = Date(date).getTime();

//#ifdef _DEBUG
//	toString();
//#endif
}

/**
* roll()函数只会比相应的字段进行处理，不会智能的对其它字段也进行逻辑上的改变。
* @param field	目前只支持 YEAR
*/
void Calendar::roll(FIELD field, int value)
{
	COleDateTime date(_time);

	int result = 0;

	switch (field)
	{
	case YEAR:	// 设置年
	{
		int month = date.GetMonth();
		int day = date.GetDay();
		date.SetDateTime(date.GetYear() + value, date.GetMonth(), 
			date.GetDay(), date.GetHour(), date.GetMinute(), date.GetSecond());
		if (date.GetMonth() != month || date.GetDay() != day)
		{
			throw std::bad_exception();
		}
	}
		break;
	default:
		throw std::bad_exception();
	}

	_time = Date(_time).getTime();

//#ifdef _DEBUG
//	toString();
//#endif
}

CString Calendar::toString()
{
	COleDateTime date(_time);
#ifdef _DEBUG
	_textDate = date.Format(L"%Y-%m-%d %H:%M:%S");
	return _textDate;
#else
	return date.Format(L"%Y-%m-%d %H:%M:%S");
#endif
}