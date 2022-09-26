#include "stdafx.h"
#include "DateHelper.h"
#include "..\ProjectFile.h"

Day DateHelper::getNextDay(Day day)
{
	int value = (int)day + 1;
	if (value > 7)
	{
		value = 1;
	}
	return (Day)value;
}

/**
* Returns a new DatePtr instance whose value
* represents the start of the day (i.e. the time of day is 00:00:00.000)
*
* @param DatePtr DatePtr to convert
* @return day start Date
*/
DatePtr DateHelper::getDayStartDate(DatePtr date)
{
 //   CalendarPtr cal = Calendar::getInstance();
 //   cal->setTime(Date);
 //   cal->set(Calendar::HOUR_OF_DAY, 0);
 //   cal->set(Calendar::MINUTE, 0);
 //   cal->set(Calendar::SECOND, 0);
 //   cal->set(Calendar::MILLISECOND, 0);
	//DatePtr = cal->getTime();
 //   return (Date);
	DatePtr dt (new Date());
	dt->SetDateTime(date->GetYear(), date->GetMonth(), date->GetDay(), 0, 0, 0);
#ifdef _DEBUG
	dt->toString();
#endif
	return dt;
}

/**
* Returns a new DatePtr instance whose value
* represents the end of the day (i.e. the time of days is 11:59:59.999)
*
* @param DatePtr DatePtr to convert
* @return day start Date
*/
DatePtr DateHelper::getDayEndDate(DatePtr date)
{
    //if (DatePtr != NULL)
    //{
    //    CalendarPtr cal = Calendar::getInstance();
    //    cal->setTime(Date);
    //    cal->set(Calendar::MILLISECOND, 999);
    //    cal->set(Calendar::SECOND, 59);
    //    cal->set(Calendar::MINUTE, 59);
    //    cal->set(Calendar::HOUR_OF_DAY, 23);
    //    DatePtr = cal->getTime();
    //}
    //return (Date);
	DatePtr dt (new Date(*date));
	dt->SetDateTime(date->GetYear(), date->GetMonth(), date->GetDay(), 23, 59, 59);
#ifdef _DEBUG
	dt->toString();
#endif
	return dt;
}

/**
* This method resets the DatePtr part of a DatePtr time value to
* a standard DatePtr (1/1/1). This is used to allow times to
* be compared and manipulated.
* 
* @param DatePtr DatePtr time value
* @return DatePtr time with DatePtr set to a standard value
*/
DatePtr DateHelper::getCanonicalTime(DatePtr date)
{
	if (date == NULL)
		return date;
    //if (DatePtr != NULL)
    //{
    //    CalendarPtr cal = Calendar::getInstance();
    //    cal->setTime(Date);
    //    cal->set(Calendar::DAY_OF_YEAR, 1);
    //    cal->set(Calendar::YEAR, 1);
    //    cal->set(Calendar::MILLISECOND, 0);
    //    DatePtr = cal->getTime();
    //}
    //return (Date);
	SYSTEMTIME sm = { 0 };
	date->GetAsSystemTime(sm);
	DatePtr dt (new Date());
	// java 可以设置为1/1/1，但C++不行，转成time_t时会出错，所以设置 为1971,1,1，小时1971时都也会出错
	dt->SetDateTime(1971, 1, 1, sm.wHour, sm.wMinute, sm.wSecond);
#ifdef _DEBUG
	dt->toString();
#endif
	return dt;
}

/**
* This method compares a target DatePtr with a DatePtr range. The method will
* return 0 if the DatePtr is within the range, less than zero if the Date
* is before the range starts, and greater than zero if the DatePtr is after
* the range ends.
* 
* @param startDate range start Date
* @param endDate range end Date
* @param targetDate target Date
* @return comparison result
*/
int DateHelper::compare(DatePtr startDate, DatePtr endDate, DatePtr targetDate)
{
	return compare(startDate, endDate, targetDate->getTime());
}

/**
* This method compares a target DatePtr with a DatePtr range. The method will
* return 0 if the DatePtr is within the range, less than zero if the Date
* is before the range starts, and greater than zero if the DatePtr is after
* the range ends.
* 
* @param startDate range start Date
* @param endDate range end Date
* @param targetDate target DatePtr in milliseconds
* @return comparison result
*/
int DateHelper::compare(DatePtr startDate, DatePtr endDate, time_t targetDate)
{
    int result = 0;
	if (targetDate < startDate->getTime())
    {
        result = -1;
    }
    else
    {
		if (targetDate > endDate->getTime())
        {
        result = 1;
        }
    }
    return (result);
}

/**
* Compare two Dates, handling NULL values.
* TODO: correct the comparison order to align with Date.compareTo 
* 
* @param d1 DatePtr instance
* @param d2 DatePtr instance
* @return int comparison result
*/
int DateHelper::compare(DatePtr d1, DatePtr d2)
{
    int result = 0;
    //if (d1 == NULL || d2 == NULL)
    //{
    //    result = (d1 == d2 ? 0 : (d1 == NULL ? 1 : -1));
    //}
    //else
    //{
	time_t diff = d1->getTime() - d2->getTime();// d1.getTime() - d2.getTime();
        result = ((diff == 0) ? 0 : ((diff > 0) ? 1 : -1));
    //}
    return (result);
}

/**
* This utility method calculates the difference in working
* time between two Dates, given the context of a task.
* 
* @param task parent task
* @param Date1 first Date
* @param Date2 second Date
* @param format required format for the resulting duration
* @return difference in working time between the two Dates
*/
DurationPtr DateHelper::getVariance(Task* task, DatePtr Date1, DatePtr Date2, TimeUnit format)
{
    DurationPtr variance;

    if (Date1 && Date2)
    {
        ProjectCalendarPtr calendar = task->getCalendar();
        if (!calendar)
        {
			calendar = task->getParentFile()->getCalendar();
        }

        if (calendar)
        {
			variance = calendar->getWork(Date1, Date2, format);
        }
    }

    if (!variance)
    {
        variance = Duration2::getInstance(0, format);
    }

    return variance;
}

/**
* Creates a DatePtr from the equivalent long value. This conversion 
* takes account of the time zone.
* 
* @param DatePtr DatePtr expressed as a long integer
* @return new DatePtr instance
*/
DatePtr DateHelper::getDateFromLong(time_t d)
{
	//TimeZone tz = TimeZone.getDefault();
	//return (new Date(DatePtr - tz.getRawOffset()));
	long rawOffset = 0;
	_get_timezone(&rawOffset);
	return DatePtr(new Date(d - abs(rawOffset)));
}

/**
* Creates a timestamp from the equivalent long value. This conversion
* takes account of the time zone and any daylight savings time.
* 
* @param timestamp timestamp expressed as a long integer
* @return new DatePtr instance
*/
DatePtr DateHelper::getTimestampFromLong(time_t timestamp)
{
	//   TimeZone tz = TimeZone.getDefault();
	//   DatePtr result = new Date(timestamp - tz.getRawOffset());
	return getDateFromLong(timestamp);


	////测试是否是夏令时
 //   if (tz.inDaylightTime(result) == true)
 //   {
 //       int savings;
 //       if (HAS_DST_SAVINGS == true)
 //       {
 //       savings = tz.getDSTSavings();
 //       }
 //       else
 //       {
 //       savings = DEFAULT_DST_SAVINGS;
 //       }

 //       result = new Date(result.getTime() - savings);
 //   }
 //   return (result);
}

/**
* Create a DatePtr instance representing a specific time.
* 
* @param hour hour 0-23
* @param minutes minutes 0-59
* @return new DatePtr instance
*/
DatePtr DateHelper::getTime(int hour, int minutes)
{
    //CalendarPtr cal = Calendar::getInstance();
    //cal->set(Calendar::HOUR_OF_DAY, hour);
    //cal->set(Calendar::MINUTE, minutes);
    //cal->set(Calendar::SECOND, 0);
    //return (cal->getTime());
	DatePtr dt (new Date (Date::GetCurrentTime()));
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	dt->SetDateTime(sys.wYear, sys.wMonth, sys.wDay, hour, minutes, 0);
	return dt;
}

/**
* Given a DatePtr represented by a CalendarPtr instance, set the time
* component of the DatePtr based on the hours and minutes of the
* time supplied by the DatePtr instance.
* 
* @param cal CalendarPtr instance representing the Date
* @param time DatePtr instance representing the time of day
*/
void DateHelper::setTime(CalendarPtr cal, DatePtr time)
{
    if (time != NULL)
    {
        CalendarPtr startCalendar = Calendar::getInstance();
        startCalendar->setTime(time);
		cal->set(Calendar::HOUR_OF_DAY, startCalendar->get(Calendar::HOUR_OF_DAY));
		cal->set(Calendar::MINUTE, startCalendar->get(Calendar::MINUTE));
        cal->set(Calendar::SECOND, startCalendar->get(Calendar::SECOND));
    }
}

/**
* Given a DatePtr represented by a DatePtr instance, set the time
* component of the DatePtr based on the hours and minutes of the
* time supplied by the DatePtr instance.
* 
* @param DatePtr DatePtr instance representing the Date
* @param canonicalTime DatePtr instance representing the time of day
* @return new DatePtr instance with the required time set
*/
DatePtr DateHelper::setTime(DatePtr date, DatePtr canonicalTime)
{
    DatePtr result = NULL;
    if (canonicalTime == NULL)
    {
		result = date;
    }
    else
    {
		result = getDayStartDate(date);
		time_t offset = canonicalTime->getTime() - getCANONICAL_EPOCH()->getTime();
		result.reset (new Date(result->getTime() + offset));
    }
    return result;
}

/**
* First DatePtr supported by Microsoft Project: 一月 01 00:00:00 1984.
*/
DatePtr DateHelper::getFIRST_DATE()
{
	//DatePtr date (new Date());
	//date->SetDateTime(1984, 1, 1, 0, 0, 0);

	//time_t val = date->getTime();
	//return date;
	return getTimestampFromLong(441763200L/*000L*/);
}

/**
* Last DatePtr supported by Microsoft Project:  星期五 12月 31 23:59:00 2049.
*/
DatePtr DateHelper::getLAST_DATE()
{
	//DatePtr date (new Date());
	//date->SetDateTime(2049, 12, 31, 23, 59, 0);
	//return date;
	return getTimestampFromLong(2524607946L/*000L*/);
}

DatePtr DateHelper::getCANONICAL_EPOCH()
{
	return getCanonicalTime(getDayStartDate(DatePtr(new Date(Date::GetCurrentTime()))));
}

//#### Time_tToSystemTime ####
void DateHelper::TimetToSystemTime(time_t t, LPSYSTEMTIME pst)
{
	ULARGE_INTEGER uli = { 0 };
	uli.QuadPart = t;
	
	FILETIME ft;
	ft.dwLowDateTime = uli.LowPart;
	ft.dwHighDateTime = uli.HighPart;

	//FILETIME ft;
	//LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	//ft.dwLowDateTime = (DWORD)ll;
	//ft.dwHighDateTime = (DWORD)(ll >> 32);

	FileTimeToSystemTime(&ft, pst);
}
