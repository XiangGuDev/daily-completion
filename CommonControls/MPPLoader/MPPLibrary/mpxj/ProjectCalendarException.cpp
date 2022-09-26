#include "stdafx.h"
#include "ProjectCalendarException.h"
#include "common\DateHelper.h"

/**
* Package private constructor.
* 
* @param fromDate exception start date
* @param toDate exception end date
*/
ProjectCalendarException::ProjectCalendarException(DatePtr fromDate, DatePtr toDate)
{
    m_fromDate = DateHelper::getDayStartDate(fromDate);
    m_toDate = DateHelper::getDayEndDate(toDate);

	ATLASSERT(m_fromDate && m_fromDate->getTime() != NULL);
	ATLASSERT(m_toDate && m_toDate->getTime() != NULL);
}

/**
* Returns the from date.
*
* @return Date
*/
DatePtr ProjectCalendarException::getFromDate()
{
    return (m_fromDate);
}

/**
* Get to date.
*
* @return Date
*/
DatePtr ProjectCalendarException::getToDate()
{
    return (m_toDate);
}

/**
* Gets working status.
*
* @return bool value
*/
bool ProjectCalendarException::getWorking()
{
    return (getRangeCount() != 0);
}

/**
* This method determines whether the given date falls in the range of
* dates covered by this exception. Note that this method assumes that both
* the start and end date of this exception have been set.
*
* @param date Date to be tested
* @return bool value
*/
bool ProjectCalendarException::contains(DatePtr date)
{
    bool result = false;

    if (date != NULL)
    {
        result = (DateHelper::compare(getFromDate(), getToDate(), date) == 0);
    }

    return (result);
}

/**
* {@inheritDoc}
*/
int ProjectCalendarException::compareTo(ProjectCalendarException* o)
{
	time_t fromTime1 = m_fromDate->getTime();
	time_t fromTime2 = o->m_fromDate->getTime();
    return ((fromTime1 < fromTime2) ? (-1) : ((fromTime1 == fromTime2) ? 0 : 1));
}

CString ProjectCalendarException::GetNote()
{
	return m_strExceptionNote;
}

void ProjectCalendarException::SetNote(CString strNote)
{
	m_strExceptionNote = strNote;
}

///**
//* {@inheritDoc}
//*/
//CString ProjectCalendarException::toString()
//{
//    StringBuilder sb = new StringBuilder();
//    sb.append("[ProjectCalendarException");
//    sb.append(" working=" + getWorking());
//    sb.append(" fromDate=" + m_fromDate);
//    sb.append(" toDate=" + m_toDate);
//
//    for (DateRange range : this)
//    {
//        sb.append(range.toString());
//    }
//
//    sb.append("]");
//    return (sb.toString());
//}