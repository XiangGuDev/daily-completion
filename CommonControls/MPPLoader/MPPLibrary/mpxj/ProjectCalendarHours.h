#pragma once

#include "ProjectCalendarDateRanges.h"
#include "MpxjEnum.h"

class ProjectCalendarWeek;

/**
 * This class is used to represent the records in an MPX file that define
 * working hours in a calendar.
 */
class COMMONCONTROLS_API ProjectCalendarHours : public ProjectCalendarDateRanges,
	public std::enable_shared_from_this<ProjectCalendarHours>
{
public:
	/**
	 * Default constructor.
	 *
	 * @param parentCalendar the parent calendar for this instance
	 */
	ProjectCalendarHours(std::shared_ptr<ProjectCalendarWeek> parentCalendar);

	/**
	 * Retrieve the parent calendar for these hours.
	 *
	 * @return parent calendar
	 */
	std::shared_ptr<ProjectCalendarWeek> getParentCalendar();

	/**
	 * Get day.
	 *
	 * @return day instance
	 */
	Day getDay();

	/**
	 * Set day.
	 *
	 * @param d day instance
	 */
	void setDay(Day d);

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
private:
	std::weak_ptr<ProjectCalendarWeek> m_parentCalendar;
	//ProjectCalendarWeek* m_parentCalendar;
	Day m_day;
};

typedef std::shared_ptr<ProjectCalendarHours> ProjectCalendarHoursPtr;
typedef List<ProjectCalendarHoursPtr> ProjectCalendarHoursArray;
