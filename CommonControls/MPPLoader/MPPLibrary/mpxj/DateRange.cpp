#include "stdafx.h"
#include "DateRange.h"
#include "common\DateHelper.h"

DateRangePtr DateRange::EMPTY_RANGE(new DateRange());

DateRange::DateRange()
{
	// Пе m_start == m_end
	m_end = m_start;
}
/**
* Constructor.
*
* @param startDate start Date
* @param endDate end Date
*/
DateRange::DateRange(DatePtr startDate, DatePtr endDate)
{
	m_start = startDate;
	m_end = endDate;
}

/**
* Retrieve the Date at the start of the range.
*
* @return start Date
*/
DatePtr DateRange::getStart()
{
    return m_start;
}

/**
* Retrieve the Date at the end of the range.
*
* @return end Date
*/
DatePtr DateRange::getEnd()
{
    return m_end;
}

/**
* This method compares a target Date with a Date range. The method will
* return 0 if the Date is within the range, less than zero if the Date
* is before the range starts, and greater than zero if the Date is after
* the range ends. 
* 
* @param Date target Date
* @return comparison result
*/
int DateRange::compareTo(DatePtr Date)
{
    return DateHelper::compare(m_start, m_end, Date);
}

/**
* {@inheritDoc}
*/
int DateRange::compareTo(DateRange* o)
{
    int result = DateHelper::compare(m_start, o->m_start);
    if (result == 0)
    {
		result = DateHelper::compare(m_end, o->m_end);
    }
    return result;
}

///**
//* {@inheritDoc}
//*/
//bool DateRange::equals(Object o)
//{
//    bool result = false;
//    if (o instanceof DateRange)
//    {
//        DateRange rhs = (DateRange) o;
//        result = (compareTo(rhs) == 0);
//    }
//    return result;
//}
//
///**
//* {@inheritDoc}
//*/
//int DateRange::hashCode()
//{
//    long start = m_start.getTime();
//    long end = m_end.getTime();
//    int result = ((int) start ^ (int) (start >> 32)) ^ ((int) end ^ (int) (end >> 32));
//    return result;
//}

/**
* {@inheritDoc}
*/
CString DateRange::toString()
{
	CString str;
	time_t start = m_start == NULL ? 0 : m_start->getTime();
	time_t end = m_end == NULL ? 0 : m_end->getTime();
	str.Format(L"DateRange start=%lld end=%lld", (long long)start, (long long)end);
	return str;
}
