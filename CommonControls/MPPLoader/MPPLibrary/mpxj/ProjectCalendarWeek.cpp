#include "stdafx.h"
#include "ProjectCalendarWeek.h"
#include "common\DateHelper.h"
//#include "GarbageCollector.h"

DateRangePtr ProjectCalendarWeek::DEFAULT_WORKING_MORNING;
DateRangePtr ProjectCalendarWeek::DEFAULT_WORKING_AFTERNOON;

ProjectCalendarWeek::ProjectCalendarWeek()
	: m_DateRange(NULL)
{
	// 设置默认的一天的工作时间
	if (!DEFAULT_WORKING_MORNING.get())
	{
		DEFAULT_WORKING_MORNING.reset(new DateRange(DateHelper::getTime(8, 30), DateHelper::getTime(12, 0)));
		DEFAULT_WORKING_AFTERNOON.reset(new DateRange(DateHelper::getTime(13, 30), DateHelper::getTime(18, 0)));
		//GarbageCollector::add(new GcSharedPtr<DateRange>(DEFAULT_WORKING_MORNING));
		//GarbageCollector::add(new GcSharedPtr<DateRange>(DEFAULT_WORKING_AFTERNOON));
	}

	for (int i = 0; i < WEEK_DAYS; i++)
	{
		m_hours.Add(NULL);
		m_days.Add(NON_WORKING);
	}
}

ProjectCalendarWeek::~ProjectCalendarWeek()
{
	//DEFAULT_WORKING_MORNING.reset();
	//DEFAULT_WORKING_AFTERNOON.reset();
}

/**
* CalendarPtr name.
*
* @param name calendar name
*/
void ProjectCalendarWeek::setName(CString name)
{
	m_name = name;
}

/**
* CalendarPtr name.
*
* @return calendar name
*/
CString ProjectCalendarWeek::getName()
{
	return (m_name);
}

/**
* Retrieves the data range for which this week is valid.
* Returns NULL if this is the default week.
*
* @return Date range, or NULL
*/
DateRange* ProjectCalendarWeek::getDateRange()
{
	return m_DateRange;
}

/**
* Sets the Date range for this week. Set this to NULL to indicate
* the default week.
*
* @param range Date range, or NULL
*/
void ProjectCalendarWeek::setDateRange(DateRange* range)
{
	m_DateRange = range;
}

/**
* If this week is derived from a another week, this method
* will return the parent week.
*
* @return parent week
*/
ProjectCalendarWeekPtr ProjectCalendarWeek::getParent()
{
	//return ProjectCalendarWeekPtr(m_parent);
	return m_parent.lock();
}

/**
* Set the parent from which this week is derived.
*
* @param parent parent week
*/
void ProjectCalendarWeek::setParent(ProjectCalendarWeekPtr parent)
{
	m_parent = parent;

	for (int i = 0; i < WEEK_DAYS; i++)
	{
		//if (m_days[i] == NULL)
		//{
		m_days[i] = DayType::DEFAULT;
		//}
	}
}

/**
* Flag indicating if this week is derived from another week.
*
* @return true if this week is derived from another
*/
bool ProjectCalendarWeek::isDerived()
{
	return (getParent() != NULL);
}

/**
* Adds a set of hours to this calendar without assigning them to
* a particular day.
*
* @return calendar hours instance
*/
ProjectCalendarHoursPtr ProjectCalendarWeek::addCalendarHours()
{
	return ProjectCalendarHoursPtr (new ProjectCalendarHours(shared_from_this()));
}

/**
* This method retrieves the calendar hours for the specified day.
* Note that this method only returns the hours specification for the
* current calendar.If this is a derived calendar, it does not refer to
* the base calendar.
*
* @param day Day instance
* @return calendar hours
*/
ProjectCalendarHoursPtr ProjectCalendarWeek::getCalendarHours(Day day)
{
	if (day < SUNDAY || day > SATURDAY)
		return NULL;

	return (m_hours[((int)day) - 1]);
}

/**
* Retrieve an array representing all of the calendar hours defined
* by this calendar.
*
* @return array of calendar hours
*/
ProjectCalendarHoursArray* ProjectCalendarWeek::getHours()
{
	return &m_hours;
}

/**
* This method retrieves the calendar hours for the specified day.
* Note that if this is a derived calendar, then this method
* will refer to the base calendar where no hours are specified
* in the derived calendar.
*
* @param day Day instance
* @return calendar hours
*/
ProjectCalendarHoursPtr ProjectCalendarWeek::getHours(Day day)
{
	ProjectCalendarHoursPtr result = getCalendarHours(day);
	if (result)
		return result;

	//
	// If this is a base calendar and we have no hours, then we
	// have a problem - so we add the default hours and try again
	//
	if (!getParent())
	{
		// Only add default hours for the day that is 'missing' to avoid overwriting real calendar hours
		addDefaultCalendarHours(day);
		result = getCalendarHours(day);
	}
	else
	{
		result = getParent()->getHours(day);
	}

	return result;
}

/**
* This is a convenience method used to add a default set of calendar
* hours to a calendar.
*/
void ProjectCalendarWeek::addDefaultCalendarHours()
{
	for (int i = 1; i <= 7; i++)
	{
		addDefaultCalendarHours((Day) i);
	}
}

/**
* This is a convenience method used to add a default set of calendar
* hours to a calendar.
*
* @param day Day for which to add default hours for
*/
void ProjectCalendarWeek::addDefaultCalendarHours(Day day)
{
	ProjectCalendarHoursPtr hours = addCalendarHours(day);

	if (day != Day::SATURDAY && day != Day::SUNDAY)
	{
		hours->addRange(DEFAULT_WORKING_MORNING);
		hours->addRange(DEFAULT_WORKING_AFTERNOON);
	}
}

/**
* Used to add working hours to the calendar. Note that the MPX file
* definition allows a maximum of 7 calendar hours records to be added to
* a single calendar.
*
* @param day day number
* @return new ProjectCalendarHours instance
*/
ProjectCalendarHoursPtr ProjectCalendarWeek::addCalendarHours(Day day)
{
	ProjectCalendarHoursPtr bch (new ProjectCalendarHours(shared_from_this()));// shared_from_this()));
	bch->setDay(day);
	m_hours[((int)day) - 1] = bch;
	return (bch);
}

/**
* Attaches a pre-existing set of hours to the correct
* day within the calendar.
*
* @param hours calendar hours instance
*/
void ProjectCalendarWeek::attachHoursToDay(ProjectCalendarHoursPtr hours)
{
	if (hours->getParentCalendar().get() != this)
	{
		throw /*BwProj::*/toolkit::Exception(L"不能循环关联");
	}
	m_hours[((int)hours->getDay()) - 1] = hours;
}

/**
* Removes a set of calendar hours from the day to which they
* are currently attached.
*
* @param hours calendar hours instance
*/
void ProjectCalendarWeek::removeHoursFromDay(ProjectCalendarHoursPtr hours)
{
	if (hours->getParentCalendar().get() != this)
	{
		throw std::bad_exception();
	}
	//m_hours[hours.getDay().getValue() - 1] = NULL;
	m_hours[hours->getDay() - 1] = NULL;
}

/**
* Retrieve an array representing the days of the week for this calendar.
*
* @return array of days of the week
*/
List<DayType>* ProjectCalendarWeek::getDays()//int dayIndex)
{
	return &m_days;// (m_days[dayIndex]);
}

/**
* This method allows the retrieval of the actual working day flag,
* which can take the values DEFAULT, WORKING, or NONWORKING. This differs
* from the isWorkingDay method as it retrieves the actual flag value.
* The isWorkingDay method will always refer back to the base calendar
* to get a bool value if the underlying flag value is DEFAULT. If
* isWorkingDay were the only method available to access this flag,
* it would not be possible to determine that a resource calendar
* had one or more flags set to DEFAULT.
*
* @param day required day
* @return value of underlying working day flag
*/
DayType ProjectCalendarWeek::getWorkingDay(Day day)
{
	return (m_days[((int)day) - 1]);
}

/**
* convenience method for setting working or non-working days.
*
* @param day required day
* @param working flag indicating if the day is a working day
*/
void ProjectCalendarWeek::setWorkingDay(Day day, bool working)
{
	setWorkingDay(day, (working ? DayType::WORKING : DayType::NON_WORKING));
}

/**
* This is a convenience method provided to allow a day to be set
* as working or non-working, by using the day number to
* identify the required day.
*
* @param day required day
* @param working flag indicating if the day is a working day
*/
void ProjectCalendarWeek::setWorkingDay(Day day, DayType working)
{
	DayType value = working;

	//if (working == NULL)
	//{
	//	if (isDerived())
	//	{
	//		value = DayType::DEFAULT;
	//	}
	//	else
	//	{
	//		value = DayType::WORKING;
	//	}
	//}
	//else
	//{
		//value = working;
	//}

	m_days[((int)day) - 1] = value;
}
