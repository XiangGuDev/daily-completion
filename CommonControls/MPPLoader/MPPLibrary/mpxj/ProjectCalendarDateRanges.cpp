#include "stdafx.h"
#include "ProjectCalendarDateRanges.h"

/**
* Add a Date range to the list of Date ranges->
*
* @param range Date range
*/
void ProjectCalendarDateRanges::addRange(DateRangePtr range)
{
	m_ranges.Add(range);
}

/**
* Retrieve the Date range at the specified index.
* The index is zero based, and this method will return
* NULL if the requested Date range does not exist.
*
* @param index range index
* @return Date range instance
*/
DateRangePtr ProjectCalendarDateRanges::getRange(int index)
{
    if (index >= 0 && index < (int) m_ranges.GetCount())
    {
        return m_ranges[index];
    }

    return DateRange::EMPTY_RANGE;
}

/**
* Replace a Date range at the specified index.
* 
* @param index range index
* @param value DateRange instance
*/
void ProjectCalendarDateRanges::setRange(int index, DateRangePtr value)
{
    m_ranges[index] = value;
}

///**
//* Retrieve an iterator to allow the list of Date ranges to be traversed.
//*
//* @return iterator.
//*/
//virtual Iterator<DateRange> iterator()
//{
//    return (m_ranges.iterator());
//}

/**
* Returns the number of Date ranges associated with this instance.
* 
* @return number of Date ranges
*/
int ProjectCalendarDateRanges::getRangeCount()
{
	return (m_ranges.GetCount());
}