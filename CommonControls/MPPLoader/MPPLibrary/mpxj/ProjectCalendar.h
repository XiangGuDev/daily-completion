#pragma once

#include "ProjectCalendarWeek.h"
#include "Duration.h"
#include "ProjectCalendarException.h"
//#include "..\Toolkit\include\CriticalSection.h"

class ProjectFile;
class Resource2;

/**
 * This class represents the a CalendarPtr Definition record. Both base calendars
 * and calendars derived from base calendars are represented by instances
 * of this class. The class is used to define the working and non-working days
 * of the week. The default calendar defines Monday to Friday as working days.
 */
class COMMONCONTROLS_API ProjectCalendar : public ProjectCalendarWeek
{
public:
	/**
	* Create a calendar based on the intersection of a task calendar and a resource calendar.
	*
	* @param file the parent file to which this record belongs.
	* @param taskCalendar task calendar to merge
	* @param resourceCalendar resource calendar to merge
	*/
	ProjectCalendar(ProjectFile* file, ProjectCalendar* taskCalendar, ProjectCalendar* resourceCalendar);

	/**
	 * Default constructor.
	 *
	 * @param file the parent file to which this record belongs.
	 */
	ProjectCalendar(ProjectFile* file);

	/**
	 * Add an empty work week.
	 *
	 * @return new work week
	 */
	ProjectCalendarWeekPtr addWorkWeek();

	/**
	 * Retrieve the work weeks associated with this calendar.
	 *
	 * @return list of work weeks
	 */
	List<ProjectCalendarWeekPtr>* getWorkWeeks();

	/**
	 * Used to add exceptions to the calendar. The MPX standard defines
	 * a limit of 250 exceptions per calendar.
	 *
	 * @param fromDate exception start Date
	 * @param toDate exception end Date
	 * @return ProjectCalendarException instance
	 */
	ProjectCalendarExceptionPtr addCalendarException(DatePtr fromDate, DatePtr toDate);

	/**
	 * This method retrieves a list of exceptions to the current calendar.
	 *
	 * @return List of calendar exceptions
	 */
	List<ProjectCalendarExceptionPtr>* getCalendarExceptions();

	/**
	 * Used to add working hours to the calendar. Note that the MPX file
	 * definition allows a maximum of 7 calendar hours records to be added to
	 * a single calendar.
	 *
	 * @param day day number
	 * @return new ProjectCalendarHours instance
	 */
	//virtual 
	virtual ProjectCalendarHoursPtr addCalendarHours(Day day);

	/**
	 * Attaches a pre-existing set of hours to the correct
	 * day within the calendar.
	 *
	 * @param hours calendar hours instance
	 */
	//virtual 
	virtual void attachHoursToDay(ProjectCalendarHoursPtr hours);

	/**
	 * Removes a set of calendar hours from the day to which they
	 * are currently attached.
	 *
	 * @param hours calendar hours instance
	 */
	//virtual 
	virtual void removeHoursFromDay(ProjectCalendarHoursPtr hours);

	/**
	 * Sets the ProjectCalendar instance from which this calendar is derived.
	 *
	 * @param calendar base calendar instance
	 */
	void setParent(std::shared_ptr<ProjectCalendar> calendar);

	//virtual 
	std::shared_ptr<ProjectCalendar> getParent();

	/**
	 * Method indicating whether a day is a working or non-working day.
	 *
	 * @param day required day
	 * @return true if this is a working day
	 */
	bool isWorkingDay(Day day);

	/**
	 * This method is provided to allow an absolute period of time
	 * represented by start and end Dates into a duration in working
	 * days based on this calendar instance. This method takes account
	 * of any exceptions defined for this calendar.
	 *
	 * @param startDate start of the period
	 * @param endDate end of the period
	 * @return new Duration object
	 */
	DurationPtr getDuration(DatePtr startDate, DatePtr endDate);

	/**
	 * Retrieves the time at which work starts on the given Date, or returns
	 * NULL if this is a non-working day.
	 *
	 * @param DatePtr DatePtr instance
	 * @return start time, or NULL for non-working day
	 */
	DatePtr getStartTime(DatePtr date);

	/**
	 * Retrieves the time at which work finishes on the given Date, or returns
	 * NULL if this is a non-working day.
	 *
	 * @param DatePtr DatePtr instance
	 * @return finish time, or NULL for non-working day
	 */
	DatePtr getFinishTime(DatePtr date);

	/**
	 * Given a start DatePtr and a duration, this method calculates the
	 * end Date. It takes account of working hours in each day, non working
	 * days and calendar exceptions. If the returnNextWorkStart parameter is
	 * set to true, the method will return the start DatePtr and time of the next
	 * working period if the end DatePtr is at the end of a working period.
	 *
	 * @param startDate start Date
	 * @param duration duration
	 * @param returnNextWorkStart if set to true will return start of next working period
	 * @return end Date
	 */
	DatePtr getDate(DatePtr startDate, DurationPtr duration, bool returnNextWorkStart);

	/**
	 * Given a finish DatePtr and a duration, this method calculates backwards to the
	 * start Date. It takes account of working hours in each day, non working
	 * days and calendar exceptions.
	 *
	 * @param finishDate finish Date
	 * @param duration duration
	 * @return start Date
	 */
	DatePtr getStartDate(DatePtr finishDate, DurationPtr duration);

	/**
	 * This method finds the start of the next working period.
	 *
	 * @param cal current CalendarPtr instance
	 */
	void updateToNextWorkStart(CalendarPtr cal);

	/**
	 * This method finds the finish of the previous working period.
	 *
	 * @param cal current CalendarPtr instance
	 */
	void upDateToPreviousWorkFinish(CalendarPtr cal);

	/**
	 * Utility method to retrieve the next working DatePtr start time, given
	 * a DatePtr and time as a starting point.
	 *
	 * @param DatePtr DatePtr and time start point
	 * @return DatePtr and time of next work start
	 */
	DatePtr getNextWorkStart(DatePtr Date);

	/**
	 * Utility method to retrieve the previous working DatePtr finish time, given
	 * a DatePtr and time as a starting point.
	 *
	 * @param DatePtr DatePtr and time start point
	 * @return DatePtr and time of previous work finish
	 */
	DatePtr getPreviousWorkFinish(DatePtr Date);

	/**
	 * This method allows the caller to determine if a given DatePtr is a
	 * working day. This method takes account of calendar exceptions.
	 *
	 * @param DatePtr DatePtr to be tested
	 * @return bool value
	 */
	bool isWorkingDate(DatePtr Date);

	/**
	 * This method allows the caller to determine if a given DatePtr is a
	 * working day. This method takes account of calendar exceptions. It assumes
	 * that the caller has already calculated the day of the week on which
	 * the given day falls.
	 *
	 * @param DatePtr DatePtr to be tested
	 * @param day Day of the week for the DatePtr under test
	 * @return bool flag
	 */
	bool isWorkingDate(DatePtr Date, Day day);

	/**
	 * This method calculates the absolute number of days between two Dates.
	 * Note that where two DatePtr objects are provided that fall on the same
	 * day, this method will return one not zero. Note also that this method
	 * assumes that the Dates are passed in the correct order, i.e.
	 * startDate < endDate.
	 *
	 * @param startDate Start Date
	 * @param endDate End Date
	 * @return number of days in the DatePtr range
	 */
	int getDaysInRange(DatePtr startDate, DatePtr endDate);

	/**
	 * Modifier method to set the unique ID of this calendar.
	 *
	 * @param uniqueID unique identifier
	 */
	void setUniqueID(int uniqueID);

	/**
	 * Accessor method to retrieve the unique ID of this calendar.
	 *
	 * @return calendar unique identifier
	 */
	int getUniqueID();

	/**
	 * Retrieve the resource to which this calendar is linked.
	 *
	 * @return resource instance
	 */
	std::shared_ptr<Resource2> getResource2();

	/**
	 * Sets the resource to which this calendar is linked. Note that this
	 * method upDates the calendar's name to be the same as the resource name.
	 * If the resource does not yet have a name, then the calendar is given
	 * a default name.
	 *
	 * @param resource resource instance
	 */
	void setResource2(std::shared_ptr<Resource2> resource);

	/**
	 * Removes this calendar from the project.
	 */
	void remove();

	/**
	 * Retrieve a calendar calendar exception which applies to this Date.
	 *
	 * @param DatePtr target Date
	 * @return calendar exception, or NULL if none match this Date
	 */
	ProjectCalendarExceptionPtr getException(DatePtr Date);

	/**
	 * Retrieves the amount of work on a given day, and
	 * returns it in the specified format.
	 *
	 * @param DatePtr target Date
	 * @param format required format
	 * @return work duration
	 */
	DurationPtr getWork(DatePtr Date, TimeUnit format);

	/**
	 * This method retrieves a Duration instance representing the amount of
	 * work between two Dates based on this calendar.
	 *
	 * @param startDate start Date
	 * @param endDate end Date
	 * @param format required duration format
	 * @return amount of work
	 */
	DurationPtr getWork(DatePtr startDate, DatePtr endDate, TimeUnit format);

	/**
	 * Utility method used to convert an integer time representation into a
	 * Duration instance.
	 *
	 * @param totalTime integer time representation
	 * @param format required time format
	 * @return new Duration instance
	 */
	DurationPtr convertFormat(time_t totalTime, TimeUnit format);

	/**
	 * Retrieves the amount of time represented by a calendar exception
	 * before or after an intersection point.
	 *
	 * @param exception calendar exception
	 * @param DatePtr intersection time
	 * @param after true to report time after intersection, false to report time before
	 * @return length of time in milliseconds
	 */
	time_t getTotalTime(ProjectCalendarDateRangesPtr exception, DatePtr Date, bool after);

	/**
	 * Retrieves the amount of working time represented by
	 * a calendar exception.
	 *
	 * @param exception calendar exception
	 * @return length of time in milliseconds
	 */
	time_t getTotalTime(ProjectCalendarDateRangesPtr exception);

	/**
	 * This method calculates the total amount of working time in a single
	 * day, which intersects with the supplied time range.
	 *
	 * @param hours collection of working hours in a day
	 * @param startDate time range start
	 * @param endDate time range end
	 * @return length of time in milliseconds
	 */
	time_t getTotalTime(ProjectCalendarDateRangesPtr hours, DatePtr startDate, DatePtr endDate);

	/**
	 * Calculates how much of a time range is before or after a
	 * target intersection point.
	 *
	 * @param start time range start
	 * @param end time range end
	 * @param target target intersection point
	 * @param after true if time after target required, false for time before
	 * @return length of time in milliseconds
	 */
	time_t getTime(DatePtr start, DatePtr end, time_t target, bool after);

	/**
	 * Retrieves the amount of time between two DatePtr time values. Note that
	 * these values are converted into canonical values to remove the
	 * DatePtr component.
	 *
	 * @param start start time
	 * @param end end time
	 * @return length of time
	 */
	time_t getTime(DatePtr start, DatePtr end);

	/**
	 * This method returns the length of overlapping time between two time
	 * ranges->
	 *
	 * @param start1 start of first range
	 * @param end1 end of first range
	 * @param start2 start start of second range
	 * @param end2 end of second range
	 * @return overlapping time in milliseconds
	 */
	time_t getTime(DatePtr start1, DatePtr end1, DatePtr start2, DatePtr end2);

	/**
	 * Add a reference to a calendar derived from this one.
	 *
	 * @param calendar derived calendar instance
	 */
	void addDerivedCalendar(ProjectCalendar* calendar);

	/**
	 * Remove a reference to a derived calendar.
	 *
	 * @param calendar derived calendar instance
	 */
	void removeDerivedCalendar(ProjectCalendar* calendar);

	/**
	 * Retrieve a list of derived calendars.
	 *
	 * @return list of derived calendars
	 */
	List<ProjectCalendar*>* getDerivedCalendars();

	///**
	// * {@inheritDoc}
	// */
	//virtual public CString toString();

	/**
	 * Copy the settings from another calendar to this calendar.
	 *
	 * @param cal calendar data source
	 */
	void copy(ProjectCalendar* cal);

	/**
	 * Utility method to clear cached calendar data.
	 */
	void clearWorkingDateCache();

	/**
	 * Retrieves the working hours on the given Date.
	 *
	 * @param DatePtr required Date
	 * @param cal optional calendar instance
	 * @param day optional day instance
	 * @return working hours
	 */
	ProjectCalendarDateRangesPtr getRanges(DatePtr Date, CalendarPtr cal, Day day);

	/**
	 * Accessor method allowing retrieval of ProjectFile reference.
	 *
	 * @return reference to this the parent ProjectFile instance
	 */
	ProjectFile* getParentFile();

	/**
	* Default base calendar name to use when none is supplied.
	*/
	static const CString DEFAULT_BASE_CALENDAR_NAME;// = _T("Standard");

private:
	/**
	 * Reference to parent ProjectFile.
	 */
	ProjectFile* m_projectFile;

	/**
	 * Unique identifier of this calendar.
	 */
	int m_uniqueID;// = int.valueOf(0);

	/**
	 * List of exceptions to the base calendar.
	 */
	List<ProjectCalendarExceptionPtr> m_exceptions;// = new List<ProjectCalendarException>();

	/**
	 * Flag indicating if the list of exceptions is sorted.
	 */
	bool m_exceptionsSorted;

	/**
	 * This resource to which this calendar is attached.
	 */
	std::weak_ptr<Resource2> m_resource;

	/**
	 * List of calendars derived from this calendar instance.
	 */
	List<ProjectCalendar*> m_derivedCalendars;// = new ArrayList<ProjectCalendar>();

	/**
	 * Caches used to speed up DatePtr calculations.
	 */
	std::map<CString, time_t> m_workingDateCache;// = new WeakHashMap<DateRange, long>();
	BwProj::toolkit::CCriticalSectionHandle m_workingDateCacheSyncHandle;

	std::map<time_t, DatePtr> m_startTimeCache;// = new WeakHashMap<Date, Date>();
	BwProj::toolkit::CCriticalSectionHandle m_startTimeCacheSyncHandle;

	/**
	* Work week definitions.
	*/
	List<ProjectCalendarWeekPtr> m_workWeeks;// = new ArrayList<ProjectCalendarWeek>();

	/**
	 * It is possible for a project calendar to be configured with no working
	 * days. This will result in an infinite loop when looking for the next
	 * working day from a Date, so we use this constant to set a limit on the
	 * maximum number of non-working days we'll skip before we bail out
	 * and take an alternative approach.
	 */
	const int MAX_NONWORKING_DAYS = 1000;
};

typedef std::shared_ptr<ProjectCalendar> ProjectCalendarPtr;