#pragma once

#include "stdafx.h"
#include "ProjectCalendarHours.h"
#include "ProjectCalendarWeek.h"

/**
* Default constructor.
*
* @param parentCalendar the parent calendar for this instance
*/
ProjectCalendarHours::ProjectCalendarHours(ProjectCalendarWeekPtr parentCalendar)
{
    m_parentCalendar = parentCalendar;
}

/**
* Retrieve the parent calendar for these hours.
*
* @return parent calendar
*/
ProjectCalendarWeekPtr ProjectCalendarHours::getParentCalendar()
{
	return m_parentCalendar.lock();
}

/**
* Get day.
*
* @return day instance
*/
Day ProjectCalendarHours::getDay()
{
    return (m_day);
}

/**
* Set day.
*
* @param d day instance
*/
void ProjectCalendarHours::setDay(Day d)
{
	ProjectCalendarWeekPtr parent = m_parentCalendar.lock();
    if (m_day != (Day)NULL)
    {
		parent->removeHoursFromDay(shared_from_this());
    }

    m_day = d;

	parent->attachHoursToDay(shared_from_this());
}

///**
// * {@inheritDoc}
// */
//virtual public CString toString()
//{
//   StringBuilder sb = new StringBuilder();
//   sb.append("[ProjectCalendarHours ");
//   for (DateRange range : this)
//   {
//      sb.append(range.toString());
//   }
//   sb.append("]");
//   return (sb.toString());
//}