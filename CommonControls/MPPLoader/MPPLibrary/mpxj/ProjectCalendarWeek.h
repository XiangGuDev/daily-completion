#pragma once

#include "MpxjEnum.h"
#include "DateRange.h"
#include "ProjectCalendarHours.h"

//#define WEEK_DAYS 7	// 一星期7天
/**
 * This class represents a basic working week, with no exceptions.
 */
class COMMONCONTROLS_API ProjectCalendarWeek : public Object2,
	public std::enable_shared_from_this<ProjectCalendarWeek>
{
public:
	ProjectCalendarWeek();
	~ProjectCalendarWeek();

	const static int WEEK_DAYS = 7;	// 一星期7天

	/**
	 * CalendarPtr name.
	 *
	 * @param name calendar name
	 */
	void setName(CString name);

	/**
	 * CalendarPtr name.
	 *
	 * @return calendar name
	 */
	CString getName();

	/**
	 * Retrieves the data range for which this week is valid.
	 * Returns NULL if this is the default week.
	 *
	 * @return Date range, or NULL
	 */
	DateRange* getDateRange();

	/**
	 * Sets the Date range for this week. Set this to NULL to indicate
	 * the default week.
	 *
	 * @param range Date range, or NULL
	 */
	void setDateRange(DateRange* range);

	/**
	 * If this week is derived from a another week, this method
	 * will return the parent week.
	 *
	 * @return parent week
	 */
	std::shared_ptr<ProjectCalendarWeek> getParent();

	/**
	 * Set the parent from which this week is derived.
	 *
	 * @param parent parent week
	 */
	void setParent(std::shared_ptr<ProjectCalendarWeek> parent);

	/**
	 * Flag indicating if this week is derived from another week.
	 *
	 * @return true if this week is derived from another
	 */
	bool isDerived();

	/**
	 * Adds a set of hours to this calendar without assigning them to
	 * a particular day.
	 *
	 * @return calendar hours instance
	 */
	ProjectCalendarHoursPtr addCalendarHours();

	/**
	 * This method retrieves the calendar hours for the specified day.
	 * Note that this method only returns the hours specification for the
	 * current calendar.If this is a derived calendar, it does not refer to
	 * the base calendar.
	 *
	 * @param day Day instance
	 * @return calendar hours
	 */
	ProjectCalendarHoursPtr getCalendarHours(Day day);

	/**
	 * Retrieve an array representing all of the calendar hours defined
	 * by this calendar.
	 *
	 * @return array of calendar hours
	 */
	ProjectCalendarHoursArray* getHours();

	/**
	 * This method retrieves the calendar hours for the specified day.
	 * Note that if this is a derived calendar, then this method
	 * will refer to the base calendar where no hours are specified
	 * in the derived calendar.
	 *
	 * @param day Day instance
	 * @return calendar hours
	 */
	ProjectCalendarHoursPtr getHours(Day day);

	/**
	 * This is a convenience method used to add a default set of calendar
	 * hours to a calendar.
	 */
	void addDefaultCalendarHours();

	/**
	 * This is a convenience method used to add a default set of calendar
	 * hours to a calendar.
	 *
	 * @param day Day for which to add default hours for
	 */
	void addDefaultCalendarHours(Day day);

	/**
	 * Used to add working hours to the calendar. Note that the MPX file
	 * definition allows a maximum of 7 calendar hours records to be added to
	 * a single calendar.
	 *
	 * @param day day number
	 * @return new ProjectCalendarHours instance
	 */
	virtual ProjectCalendarHoursPtr addCalendarHours(Day day);

	/**
	 * Attaches a pre-existing set of hours to the correct
	 * day within the calendar.
	 *
	 * @param hours calendar hours instance
	 */
	virtual void attachHoursToDay(ProjectCalendarHoursPtr hours);

	/**
	 * Removes a set of calendar hours from the day to which they
	 * are currently attached.
	 *
	 * @param hours calendar hours instance
	 */
	virtual void removeHoursFromDay(ProjectCalendarHoursPtr hours);

	/**
	 * Retrieve an array representing the days of the week for this calendar.
	 *
	 * @return array of days of the week
	 */
	List<DayType>* getDays();
	//DayType[] getDays()

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
	DayType getWorkingDay(Day day);

	/**
	 * convenience method for setting working or non-working days.
	 *
	 * @param day required day
	 * @param working flag indicating if the day is a working day
	 */
	void setWorkingDay(Day day, bool working);

	/**
	 * This is a convenience method provided to allow a day to be set
	 * as working or non-working, by using the day number to
	 * identify the required day.
	 *
	 * @param day required day
	 * @param working flag indicating if the day is a working day
	 */
	void setWorkingDay(Day day, DayType working);

public:
	/**
	* Constants representing the default working morning and afternoon hours.
	*/
	static DateRangePtr DEFAULT_WORKING_MORNING;// = new DateRange(DateHelper::getTime(8, 0), DateHelper::getTime(12, 0));
	static DateRangePtr DEFAULT_WORKING_AFTERNOON;// = new DateRange(DateHelper::getTime(13, 0), DateHelper::getTime(17, 0));

private:
	/**
	 * CalendarPtr name.
	 */
	CString m_name;

	/**
	 * Date range for which this week is valid, NULL if this is the default week.
	 */
	DateRange* m_DateRange;

	/**
	 * Parent week from which this is derived, if any.
	 */
	std::weak_ptr<ProjectCalendarWeek> m_parent;
	//ProjectCalendarWeek* m_parent;

	/**
	 * Working hours for each day.
	 */
	//ProjectCalendarHours* m_hours[WEEK_DAYS];// = new ProjectCalendarHours[7];
	ProjectCalendarHoursArray m_hours;

	/**
	 * Working/non-working/default flag for each day.
	 */
	//DayType m_days[WEEK_DAYS];// = new DayType[7];
	List<DayType> m_days;
};

typedef std::shared_ptr<ProjectCalendarWeek> ProjectCalendarWeekPtr;