#include "stdafx.h"
#include "ProjectCalendar.h"
#include "ProjectFile.h"
#include "ProjectCalendarHours.h"
#include "common\DateHelper.h"
#include "common\NumberHelper.h"
#include "Duration.h"
#include "Resource.h"

const CString ProjectCalendar::DEFAULT_BASE_CALENDAR_NAME = _T("Standard");

/**
* Default constructor.
*
* @param file the parent file to which this record belongs.
*/
ProjectCalendar::ProjectCalendar(ProjectFile* file)
{
    m_projectFile = file;

    if (file->getAutoCalendarUniqueID() == true)
    {
        setUniqueID(file->getCalendarUniqueID());
    }
}

/**
* Create a calendar based on the intersection of a task calendar and a resource calendar.
*
* @param file the parent file to which this record belongs.
* @param taskCalendar task calendar to merge
* @param resourceCalendar resource calendar to merge
*/
ProjectCalendar::ProjectCalendar(ProjectFile* file, ProjectCalendar* taskCalendar, ProjectCalendar* resourceCalendar)
{
	m_projectFile = file;

	// Set the resource
	setResource2(resourceCalendar->getResource2());

	// Merge the exceptions

	// Merge the hours
	for (int i = 1; i <= 7; i++)
	{
		Day day = (Day)i;// Day::getInstance(i);

		// Set working/non-working days
		setWorkingDay(day, taskCalendar->isWorkingDay(day) && resourceCalendar->isWorkingDay(day));

		ProjectCalendarHoursPtr hours = addCalendarHours(day);

		int taskIndex = 0;
		int resourceIndex = 0;

		ProjectCalendarHoursPtr taskHours = taskCalendar->getHours(day);
		ProjectCalendarHoursPtr resourceHours = resourceCalendar->getHours(day);

		DateRangePtr range1;
		DateRangePtr range2;

		DatePtr start = NULL;
		DatePtr end = NULL;

		DatePtr start1 = NULL;
		DatePtr start2 = NULL;
		DatePtr end1 = NULL;
		DatePtr end2 = NULL;
		while (true)
		{
			// Find next range start
			if (taskHours->getRangeCount() > taskIndex)
			{
				range1 = taskHours->getRange(taskIndex);
			}
			else
			{
				break;
			}

			if (resourceHours->getRangeCount() > resourceIndex)
			{
				range2 = resourceHours->getRange(resourceIndex);
			}
			else
			{
				break;
			}

			start1 = range1->getStart();
			start2 = range2->getStart();
			end1 = range1->getEnd();
			end2 = range2->getEnd();

			// Get the later start
			if (start1->compareTo(start2.get()) > 0)
			{
				start = start1;
			}
			else
			{
				start = start2;
			}

			// Get the earlier end
			if (end1->compareTo(end2.get()) < 0)
			{
				end = end1;
				taskIndex++;
			}
			else
			{
				end = end2;
				resourceIndex++;
			}

			if (end != NULL && end->compareTo(start.get()) > 0)
			{
				// Found a block
				hours->addRange(DateRangePtr(new DateRange(start, end)));
			}
		}
	}
	// For now just combine the exceptions. Probably overkill (although would be more accurate) to also merge the exceptions.
	m_exceptions.Append(*taskCalendar->getCalendarExceptions());
	m_exceptions.Append(*resourceCalendar->getCalendarExceptions());
	m_exceptionsSorted = false;
}

/**
* Add an empty work week.
* 
* @return new work week
*/
ProjectCalendarWeekPtr ProjectCalendar::addWorkWeek()
{
    ProjectCalendarWeekPtr week (new ProjectCalendarWeek());
	week->setParent(shared_from_this());
	m_workWeeks.Add(week);
    return week;
}

/**
* Retrieve the work weeks associated with this calendar.
* 
* @return list of work weeks
*/
List<ProjectCalendarWeekPtr>* ProjectCalendar::getWorkWeeks()
{
    return &m_workWeeks;
}

/**
* Used to add exceptions to the calendar. The MPX standard defines
* a limit of 250 exceptions per calendar.
*
* @param fromDate exception start Date
* @param toDate exception end Date
* @return ProjectCalendarException instance
*/
ProjectCalendarExceptionPtr ProjectCalendar::addCalendarException(DatePtr fromDate, DatePtr toDate)
{
    ProjectCalendarExceptionPtr bce( new ProjectCalendarException(fromDate, toDate));
    m_exceptions.Add(bce);
    m_exceptionsSorted = false;
    clearWorkingDateCache();
    return (bce);
}

/**
* This method retrieves a list of exceptions to the current calendar.
*
* @return List of calendar exceptions
*/
List<ProjectCalendarExceptionPtr>* ProjectCalendar::getCalendarExceptions()
{
    if (!m_exceptionsSorted)
    {
		Collections::sort_object_list(m_exceptions);
        //Collections.sort(m_exceptions);
    }
    return &m_exceptions;
}

/**
* Used to add working hours to the calendar. Note that the MPX file
* definition allows a maximum of 7 calendar hours records to be added to
* a single calendar.
*
* @param day day number
* @return new ProjectCalendarHours instance
*/
ProjectCalendarHoursPtr ProjectCalendar::addCalendarHours(Day day)
{
    clearWorkingDateCache();
	return __super::addCalendarHours(day);
}

/**
* Attaches a pre-existing set of hours to the correct
* day within the calendar.
*
* @param hours calendar hours instance
*/
void ProjectCalendar::attachHoursToDay(ProjectCalendarHoursPtr hours)
{
    clearWorkingDateCache();
	__super::attachHoursToDay(hours);
}

/**
* Removes a set of calendar hours from the day to which they
* are currently attached.
*
* @param hours calendar hours instance
*/
void ProjectCalendar::removeHoursFromDay(ProjectCalendarHoursPtr hours)
{
    clearWorkingDateCache();
	__super::removeHoursFromDay(hours);
}

/**
* Sets the ProjectCalendar* instance from which this calendar is derived.
*
* @param calendar base calendar instance
*/
void ProjectCalendar::setParent(ProjectCalendarPtr calendar)
{
    if (getParent() != NULL)
    {
        getParent()->removeDerivedCalendar(this);
    }

	__super::setParent(calendar);

    if (calendar != NULL)
    {
        calendar->addDerivedCalendar(this);
    }
    clearWorkingDateCache();

}

ProjectCalendarPtr ProjectCalendar::getParent()
{
	return std::dynamic_pointer_cast<ProjectCalendar>(__super::getParent());
}

/**
* Method indicating whether a day is a working or non-working day.
*
* @param day required day
* @return true if this is a working day
*/
bool ProjectCalendar::isWorkingDay(Day day)
{
    DayType value = getWorkingDay(day);
    bool result = false;

	if (value == DayType::DEFAULT)
    {
        ProjectCalendarPtr cal = getParent();
        if (cal)
        {
			result = cal->isWorkingDay(day);
        }
        else
        {
			result = (day != Day::SATURDAY && day != Day::SUNDAY);
        }
    }
    else
    {
        result = (value == DayType::WORKING);
    }

    return (result);
}

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
DurationPtr ProjectCalendar::getDuration(DatePtr startDate, DatePtr endDate)
{
    CalendarPtr cal = Calendar::getInstance();
    cal->setTime(startDate);
    int days = getDaysInRange(startDate, endDate);
    int duration = 0;
    Day day = (Day) cal->get(Calendar::DAY_OF_WEEK);

    while (days > 0)
    {
        if (isWorkingDate(cal->getTime(), day) == true)
        {
        ++duration;
        }

        --days;
		day = DateHelper::getNextDay(day);// .getNextDay();
		cal->set(Calendar::DAY_OF_YEAR, cal->get(Calendar::DAY_OF_YEAR) + 1);
    }

    return (Duration2::getInstance(duration, TimeUnit::DAYS));
}

/**
* Retrieves the time at which work starts on the given Date, or returns
* NULL if this is a non-working day.
* 
* @param DatePtr DatePtr instance
* @return start time, or NULL for non-working day
*/
DatePtr ProjectCalendar::getStartTime(DatePtr date)
{
	DatePtr result = NULL;// m_startTimeCache.get(date);

	bool lookupVal = false;
	{
		BwProj::toolkit::CCriticalSectionSync sync2(m_startTimeCacheSyncHandle);
		//lookupVal = m_startTimeCache.Lookup(date->getTime(), result);
		auto itMap = m_startTimeCache.find(date->getTime());
		if (itMap != m_startTimeCache.end())
		{
			lookupVal = true;
			result = itMap->second;
		}
	}

	if (!lookupVal)
    {
        ProjectCalendarDateRangesPtr ranges = getRanges(date, NULL, (Day)NULL);
        if (ranges == NULL)
        {
			result = getParentFile()->getProjectHeader()->getDefaultStartTime();
        }
        else
        {
			auto range = ranges->getRange(0);
			if(range != NULL)
				result = range->getStart();
        }
        result = DateHelper::getCanonicalTime(result);

		BwProj::toolkit::CCriticalSectionSync sync2(m_startTimeCacheSyncHandle);
		//m_startTimeCache.put(date->getTime(), result);
		m_startTimeCache[date->getTime()] = result;
    }
    return result;
}

/**
* Retrieves the time at which work finishes on the given Date, or returns
* NULL if this is a non-working day.
* 
* @param DatePtr DatePtr instance
* @return finish time, or NULL for non-working day
*/
DatePtr ProjectCalendar::getFinishTime(DatePtr date)
{
    DatePtr result;

    if (date != NULL)
    {
        ProjectCalendarDateRangesPtr ranges = getRanges(date, NULL, (Day)NULL);
        if (ranges == NULL)
        {
			result = getParentFile()->getProjectHeader()->getDefaultEndTime();
			result = DateHelper::getCanonicalTime(result);
        }
        else
        {
			DatePtr rangeStart = result = ranges->getRange(0)->getStart();
			DatePtr rangeFinish = ranges->getRange(ranges->getRangeCount() - 1)->getEnd();
			DatePtr startDay = DateHelper::getDayStartDate(rangeStart);
			DatePtr finishDay = DateHelper::getDayStartDate(rangeFinish);

			result = DateHelper::getCanonicalTime(rangeFinish);
			//
			// Handle the case where the end of the range is at midnight -
			// this will show up as the start and end days not matching
			//
			if (startDay != NULL && finishDay != NULL && startDay->getTime() != finishDay->getTime())
			{
				CalendarPtr calendar = Calendar::getInstance();
				calendar->setTime(result);
				calendar->add(Calendar::DAY_OF_YEAR, 1);
				result = calendar->getTime();
			}
        }
    }
    return result;
}

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
DatePtr ProjectCalendar::getDate(DatePtr startDate, DurationPtr duration, bool returnNextWorkStart)
{
	ProjectHeader* header = getParentFile()->getProjectHeader();
    // Note: Using a double allows us to handle DatePtr values that are accurate up to seconds.
    //       However, it also means we need to truncate the value to 2 decimals to make the
    //       comparisons work as sometimes the double ends up with some extra e.g. .0000000000003
    //       that wreak havoc on the comparisons.
	//欧阳友章改 : 截取2位小数改为截取3位小数,计算得到的值更精确 2020/12/22
	//修改原因:若任务开始时间为8:30 工期设置为0.3工作日(一天8小时)   正确的结束时间为10:54  截取2位小数导致计算的结束时间为 10:50
    double remainingMinutes = NumberHelper::truncate(duration->convertUnits(TimeUnit::MINUTES, header)->getDuration(), 3);
	CalendarPtr cal = Calendar::getInstance(); 
    cal->setTime(startDate);
	CalendarPtr endCal = Calendar::getInstance();

    while (remainingMinutes > 0)
    {
        //
        // Get the current DatePtr and time and determine how many 
        // working hours remain
        //
		DatePtr currentDate = cal->getTime(); 
        endCal->setTime(currentDate);
		endCal->add(Calendar::DAY_OF_YEAR, 1); 
		DatePtr currentDateEnd = DateHelper::getDayStartDate(endCal->getTime());
        double currentDateWorkingMinutes = getWork(currentDate, currentDateEnd, TimeUnit::MINUTES)->getDuration();

        //
        // We have more than enough hours left
        //
        if (remainingMinutes > currentDateWorkingMinutes)
        {
			//
			// Deduct this day's hours from our total
			//
			remainingMinutes = NumberHelper::truncate(remainingMinutes - currentDateWorkingMinutes, 2);

			//
			// Move the calendar forward to the next working day
			//            
			Day day;
			int nonWorkingDayCount = 0;
			do
			{
				cal->add(Calendar::DAY_OF_YEAR, 1);
				day = (Day)cal->get(Calendar::DAY_OF_WEEK);
				++nonWorkingDayCount;
				if (nonWorkingDayCount > MAX_NONWORKING_DAYS)
				{
					cal->setTime(startDate);
					cal->add(Calendar::DAY_OF_YEAR, 1);
					remainingMinutes = 0;
					break;
				}
			}
			while (!isWorkingDate(cal->getTime(), day));

			//
			// Retrieve the start time for this day
			//
			DatePtr startTime = getStartTime(cal->getTime());
			DateHelper::setTime(cal, startTime);
        }
        else
        {
			//
			// We have less hours to allocate than there are working hours
			// in this day. We need to calculate the time of day at which 
			// our work ends.
			//
			ProjectCalendarDateRangesPtr ranges = getRanges(cal->getTime(), cal, (Day)NULL);

			//
			// Now we have the range of working hours for this day,
			// step through it to work out the end point
			//
			DatePtr endTime = NULL;
			DatePtr currentDateStartTime = DateHelper::getCanonicalTime(currentDate);
			bool firstRange = true;
			for (int i = 0; i < ranges->getRangeCount(); i++)
			{
				DateRangePtr range = ranges->getRange(i);
				//
				// Skip this range if its end is before our start time
				//
				DatePtr rangeStart = range->getStart();
				DatePtr rangeEnd = range->getEnd();

				if (rangeStart == NULL || rangeEnd == NULL)
				{
					continue;
				}

				DatePtr canonicalRangeEnd = DateHelper::getCanonicalTime(rangeEnd);
				DatePtr canonicalRangeStart = DateHelper::getCanonicalTime(rangeStart);

				DatePtr rangeStartDay = DateHelper::getDayStartDate(rangeStart);
				DatePtr rangeEndDay = DateHelper::getDayStartDate(rangeEnd);

				if (rangeStartDay->getTime() != rangeEndDay->getTime())
				{
					CalendarPtr calendar = Calendar::getInstance();
					calendar->setTime(canonicalRangeEnd);
					calendar->add(Calendar::DAY_OF_YEAR, 1);
					canonicalRangeEnd = calendar->getTime();
				}

				if (firstRange && canonicalRangeEnd->getTime() < currentDateStartTime->getTime())
				{
					continue;
				}

				//
				// Move the start of the range if our current start is 
				// past the range start
				//               
				if (firstRange && canonicalRangeStart->getTime() < currentDateStartTime->getTime())
				{
					canonicalRangeStart = currentDateStartTime;
				}
				firstRange = false;

				double rangeMinutes;

				rangeMinutes = (double) (canonicalRangeEnd->getTime() - canonicalRangeStart->getTime());
				rangeMinutes /= (1000 * 60);

				if (remainingMinutes > rangeMinutes)
				{
					remainingMinutes = NumberHelper::truncate(remainingMinutes - rangeMinutes, 2);
				}
				else
				{
					if (Duration2::durationValueEquals(remainingMinutes, rangeMinutes))
					{
						endTime = canonicalRangeEnd;
						if (rangeStartDay->getTime() != rangeEndDay->getTime())
						{
						// The range ends the next day, so let's adjust our DatePtr accordingly.
						cal->add(Calendar::DAY_OF_YEAR, 1);
						}
					}
					else
					{
						endTime = DatePtr(new Date(
							(time_t)(canonicalRangeStart->getTime() + (remainingMinutes * (60 * 1000)))));
						returnNextWorkStart = false;
					}
					remainingMinutes = 0;
					break;
				}
			}

			DateHelper::setTime(cal, endTime);
        }
    }

    if (returnNextWorkStart)
    {
        updateToNextWorkStart(cal);
    }

    return cal->getTime();
}

/**
* Given a finish DatePtr and a duration, this method calculates backwards to the 
* start Date. It takes account of working hours in each day, non working
* days and calendar exceptions. 
* 
* @param finishDate finish Date
* @param duration duration
* @return start Date
*/
DatePtr ProjectCalendar::getStartDate(DatePtr finishDate, DurationPtr duration)
{
	ProjectHeader* header = getParentFile()->getProjectHeader();
    // Note: Using a double allows us to handle DatePtr values that are accurate up to seconds.
    //       However, it also means we need to truncate the value to 2 decimals to make the
    //       comparisons work as sometimes the double ends up with some extra e.g. .0000000000003
    //       that wreak havoc on the comparisons.
    double remainingMinutes = NumberHelper::truncate(duration->convertUnits(TimeUnit::MINUTES, header)->getDuration(), 3);
    CalendarPtr cal = Calendar::getInstance();
    cal->setTime(finishDate);
    CalendarPtr startCal = Calendar::getInstance();

    while (remainingMinutes > 0)
    {
        //
        // Get the current DatePtr and time and determine how many 
        // working hours remain
        //
        DatePtr currentDate = cal->getTime();
        startCal->setTime(currentDate);
        startCal->add(Calendar::DAY_OF_YEAR, -1);
        DatePtr currentDateEnd = DateHelper::getDayEndDate(startCal->getTime());
        double currentDateWorkingMinutes = getWork(currentDateEnd, currentDate, TimeUnit::MINUTES)->getDuration();

        //
        // We have more than enough hours left
        //
        if (remainingMinutes > currentDateWorkingMinutes)
        {
			//
			// Deduct this day's hours from our total
			//
			remainingMinutes = NumberHelper::truncate(remainingMinutes - currentDateWorkingMinutes, 2);

			//
			// Move the calendar backward to the previous working day
			//            
			int count = 0;
			Day day;
			do
			{
				if (count > 7)
				{
					//break; // Protect against a calendar with all days non-working
				}
				count++;
				cal->add(Calendar::DAY_OF_YEAR, -1);
				day = (Day)cal->get(Calendar::DAY_OF_WEEK);
			}
			while (!isWorkingDate(cal->getTime(), day));

			if (count > 7)
			{
				// We have a calendar with no working days.
				//return NULL;
			}

			//
			// Retrieve the finish time for this day
			//
			DatePtr finishTime = getFinishTime(cal->getTime());
			DateHelper::setTime(cal, finishTime);
        }
        else
        {
			//
			// We have less hours to allocate than there are working hours
			// in this day. We need to calculate the time of day at which 
			// our work starts.
			//
			ProjectCalendarDateRangesPtr ranges = getRanges(cal->getTime(), cal, (Day)NULL);
	
			//
			// Now we have the range of working hours for this day,
			// step through it to work out the start point
			//
			DatePtr startTime = NULL;
			DatePtr currentDateFinishTime = DateHelper::getCanonicalTime(currentDate);
			bool firstRange = true;
			// Traverse from end to start
			for (int i = ranges->getRangeCount() - 1; i >= 0; i--)
			{
				DateRangePtr range = ranges->getRange(i);
				//
				// Skip this range if its start is after our end time
				//
				DatePtr rangeStart = range->getStart();
				DatePtr rangeEnd = range->getEnd();

				if (rangeStart == NULL || rangeEnd == NULL)
				{
					continue;
				}

				DatePtr canonicalRangeEnd = DateHelper::getCanonicalTime(rangeEnd);
				DatePtr canonicalRangeStart = DateHelper::getCanonicalTime(rangeStart);

				DatePtr rangeStartDay = DateHelper::getDayStartDate(rangeStart);
				DatePtr rangeEndDay = DateHelper::getDayStartDate(rangeEnd);

				if (rangeStartDay->getTime() != rangeEndDay->getTime())
				{
					CalendarPtr calendar = Calendar::getInstance();
					calendar->setTime(canonicalRangeEnd);
					calendar->add(Calendar::DAY_OF_YEAR, 1);
					canonicalRangeEnd = calendar->getTime();
				}

				if (firstRange && canonicalRangeStart->getTime() > currentDateFinishTime->getTime())
				{
					continue;
				}

				//
				// Move the end of the range if our current end is 
				// before the range end
				//               
				if (firstRange && canonicalRangeEnd->getTime() > currentDateFinishTime->getTime())
				{
					canonicalRangeEnd = currentDateFinishTime;
				}
				firstRange = false;

				double rangeMinutes;

				rangeMinutes = (double) (canonicalRangeEnd->getTime() - canonicalRangeStart->getTime());
				rangeMinutes /= (1000 * 60);

				if (remainingMinutes > rangeMinutes)
				{
					remainingMinutes *= 1000; rangeMinutes *= 1000;
					remainingMinutes = NumberHelper::truncate(remainingMinutes - rangeMinutes, 2);
					remainingMinutes /= 1000; rangeMinutes /= 1000;
				}
				else
				{
					if (Duration2::durationValueEquals(remainingMinutes, rangeMinutes))
					{
						startTime = canonicalRangeStart;
					}
					else
					{
						startTime = DatePtr(new Date((time_t)(
							canonicalRangeEnd->getTime() - (remainingMinutes * (60 * 1000)))));
					}
					remainingMinutes = 0;
					break;
				}
			}

			DateHelper::setTime(cal, startTime);
        }
    }

    return cal->getTime();
}

/**
* This method finds the start of the next working period.
* 
* @param cal current CalendarPtr instance
*/
void ProjectCalendar::updateToNextWorkStart(CalendarPtr cal)
{
    DatePtr originalDate = cal->getTime();

    //
    // Find the DatePtr ranges for the current day
    //      
	ProjectCalendarDateRangesPtr ranges = getRanges(originalDate, cal, (Day)NULL);

	if (ranges == NULL)
		return;

    //
    // Do we have a start time today?
    // 
    DatePtr calTime = DateHelper::getCanonicalTime(cal->getTime());
    DatePtr startTime = NULL;
	for (int i = 0; i < ranges->getRangeCount(); i ++)
    {
		DateRangePtr range = ranges->getRange(i);

		DatePtr rangeStart = DateHelper::getCanonicalTime(range->getStart());
		DatePtr rangeEnd = DateHelper::getCanonicalTime(range->getEnd());
		DatePtr rangeStartDay = DateHelper::getDayStartDate(range->getStart());
		DatePtr rangeEndDay = DateHelper::getDayStartDate(range->getEnd());

		if (rangeStartDay->getTime() != rangeEndDay->getTime())
		{
			CalendarPtr calendar = Calendar::getInstance();
			calendar->setTime(rangeEnd);
			calendar->add(Calendar::DAY_OF_YEAR, 1);
			rangeEnd = calendar->getTime();
		}

		if (calTime->getTime() < rangeEnd->getTime())
		{
			if (calTime->getTime() > rangeStart->getTime())
			{
				startTime = calTime;
			}
			else
			{
				startTime = rangeStart;
			}
			break;
		}
    }

    //
    // If we don't have a start time today - find the next working day
    // then retrieve the start time.
    //
    if (startTime == NULL)
    {
		Day day;
		int nonWorkingDayCount = 0;
		do
		{
			cal->set(Calendar::DAY_OF_YEAR, cal->get(Calendar::DAY_OF_YEAR) + 1);
			day = (Day)cal->get(Calendar::DAY_OF_WEEK);
			++nonWorkingDayCount;
			if (nonWorkingDayCount > MAX_NONWORKING_DAYS)
			{
				cal->setTime(originalDate);
				break;
			}
		}
		while (!isWorkingDate(cal->getTime(), day));

		startTime = getStartTime(cal->getTime());
    }

    DateHelper::setTime(cal, startTime);
}

/**
* This method finds the finish of the previous working period.
* 
* @param cal current CalendarPtr instance
*/
void ProjectCalendar::upDateToPreviousWorkFinish(CalendarPtr cal)
{
    DatePtr originalDate = cal->getTime();

    //
    // Find the DatePtr ranges for the current day
    //      
	ProjectCalendarDateRangesPtr ranges = getRanges(originalDate, cal, (Day)NULL);
    if (ranges != NULL)
    {
        //
        // Do we have a start time today?
        // 
        DatePtr calTime = DateHelper::getCanonicalTime(cal->getTime());
        DatePtr finishTime = NULL;
		for (int i = 0; i < ranges->getRangeCount(); i++)
		{
			DateRangePtr range = ranges->getRange(i);
			DatePtr rangeEnd = DateHelper::getCanonicalTime(range->getEnd());
			DatePtr rangeStartDay = DateHelper::getDayStartDate(range->getStart());
			DatePtr rangeEndDay = DateHelper::getDayStartDate(range->getEnd());

			if (rangeStartDay->getTime() != rangeEndDay->getTime())
			{
				CalendarPtr calendar = Calendar::getInstance();
				calendar->setTime(rangeEnd);
				calendar->add(Calendar::DAY_OF_YEAR, 1);
				rangeEnd = calendar->getTime();
			}

			if (calTime->getTime() >= rangeEnd->getTime())
			{
				finishTime = rangeEnd;
				break;
			}
        }

        //
        // If we don't have a finish time today - find the previous working day
        // then retrieve the finish time.
        //
        if (finishTime == NULL)
        {
        Day day;
        int nonWorkingDayCount = 0;
        do
        {
            cal->set(Calendar::DAY_OF_YEAR, cal->get(Calendar::DAY_OF_YEAR) - 1);
			day = (Day)cal->get(Calendar::DAY_OF_WEEK);
            ++nonWorkingDayCount;
            if (nonWorkingDayCount > MAX_NONWORKING_DAYS)
            {
                cal->setTime(originalDate);
                break;
            }
        }
        while (!isWorkingDate(cal->getTime(), day));

        finishTime = getFinishTime(cal->getTime());
        }

        DateHelper::setTime(cal, finishTime);
    }
}

/**
* Utility method to retrieve the next working DatePtr start time, given
* a DatePtr and time as a starting point.
* 
* @param DatePtr DatePtr and time start point
* @return DatePtr and time of next work start
*/
DatePtr ProjectCalendar::getNextWorkStart(DatePtr Date)
{
    CalendarPtr cal = Calendar::getInstance();
    cal->setTime(Date);
    updateToNextWorkStart(cal);
    return cal->getTime();
}

/**
* Utility method to retrieve the previous working DatePtr finish time, given
* a DatePtr and time as a starting point.
* 
* @param DatePtr DatePtr and time start point
* @return DatePtr and time of previous work finish
*/
DatePtr ProjectCalendar::getPreviousWorkFinish(DatePtr Date)
{
    CalendarPtr cal = Calendar::getInstance();
    cal->setTime(Date);
    upDateToPreviousWorkFinish(cal);
    return cal->getTime();
}

/**
* This method allows the caller to determine if a given DatePtr is a
* working day. This method takes account of calendar exceptions.
*
* @param DatePtr DatePtr to be tested
* @return bool value
*/
bool ProjectCalendar::isWorkingDate(DatePtr Date)
{
    CalendarPtr cal = Calendar::getInstance();
    cal->setTime(Date);
	Day day = (Day)cal->get(Calendar::DAY_OF_WEEK);
    return (isWorkingDate(Date, day));
}

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
bool ProjectCalendar::isWorkingDate(DatePtr Date, Day day)
{
    ProjectCalendarDateRangesPtr ranges = getRanges(Date, NULL, day);
    return ranges->getRangeCount() != 0;
}

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
int ProjectCalendar::getDaysInRange(DatePtr startDate, DatePtr endDate)
{
    int result = 0;
    CalendarPtr cal = Calendar::getInstance();
    cal->setTime(endDate);
    int endDateYear = cal->get(Calendar::YEAR);
    int endDateDayOfYear = cal->get(Calendar::DAY_OF_YEAR);

    cal->setTime(startDate);

    if (endDateYear == cal->get(Calendar::YEAR))
    {
        result = (endDateDayOfYear - cal->get(Calendar::DAY_OF_YEAR)) + 1;
    }
    else
    {
        result = 0;
        do
        {
			result += (cal->getActualMaximum(Calendar::DAY_OF_YEAR) - cal->get(Calendar::DAY_OF_YEAR)) + 1;
			cal->set(Calendar::DAY_OF_YEAR, 1);
			cal->roll(Calendar::YEAR, 1);
        }
        while (cal->get(Calendar::YEAR) < endDateYear);
        result += endDateDayOfYear;
    }

    return (result);
}

/**
* Modifier method to set the unique ID of this calendar.
*
* @param uniqueID unique identifier
*/
void ProjectCalendar::setUniqueID(int uniqueID)
{
	ProjectFile* parent = getParentFile();

    if (m_uniqueID != NULL)
    {
        parent->unmapCalendarUniqueID(m_uniqueID);
    }

	parent->mapCalendarUniqueID(uniqueID, std::dynamic_pointer_cast<ProjectCalendar>(shared_from_this()));

    m_uniqueID = uniqueID;
}

/**
* Accessor method to retrieve the unique ID of this calendar.
*
* @return calendar unique identifier
*/
int ProjectCalendar::getUniqueID()
{
    return (m_uniqueID);
}

/**
* Retrieve the resource to which this calendar is linked.
*
* @return resource instance
*/
Resource2Ptr ProjectCalendar::getResource2()
{
    return m_resource.lock();
}

/**
* Sets the resource to which this calendar is linked. Note that this
* method upDates the calendar's name to be the same as the resource name.
* If the resource does not yet have a name, then the calendar is given
* a default name.
*
* @param resource resource instance
*/
void ProjectCalendar::setResource2(Resource2Ptr resource)
{
    m_resource = resource;
	CString name;
	if (!m_resource.expired())
		name = m_resource.lock()->getName();
    if (name.GetLength() == 0)
    {
        name = L"Unnamed Resource2";
    }
    setName(name);
}

/**
* Removes this calendar from the project.
*/
void ProjectCalendar::remove()
{
	getParentFile()->removeCalendar(std::dynamic_pointer_cast<ProjectCalendar>(shared_from_this()));
}

/**
* Retrieve a calendar calendar exception which applies to this Date.
*
* @param DatePtr target Date
* @return calendar exception, or NULL if none match this Date
*/
ProjectCalendarExceptionPtr ProjectCalendar::getException(DatePtr date)
{
    ProjectCalendarExceptionPtr exception = NULL;
    if (!m_exceptions.isEmpty())
    {
        if (!m_exceptionsSorted)
        {
			Collections::sort_object_list(m_exceptions);
			//Collections.sort(m_exceptions);
			m_exceptionsSorted = true;
        }

        int low = 0;
        int high = m_exceptions.GetCount() - 1;
		time_t targetDate = date->getTime();

        while (low <= high)
        {
			int mid = (low + high) >> 1;
			ProjectCalendarExceptionPtr midVal = m_exceptions.GetAt(mid);
			int cmp = 0 - DateHelper::compare(midVal->getFromDate(), midVal->getToDate(), targetDate);

			if (cmp < 0)
			{
				low = mid + 1;
			}
			else
			{
				if (cmp > 0)
				{
					high = mid - 1;
				}
				else
				{
					exception = midVal;
					break;
				}
			}
        }
    }

    if (exception == NULL && getParent() != NULL)
    {
        // Check base calendar as well for an exception.
        exception = getParent()->getException(date);
    }
    return (exception);
}

/**
* Retrieves the amount of work on a given day, and
* returns it in the specified format.
* 
* @param DatePtr target Date
* @param format required format
* @return work duration
*/
DurationPtr ProjectCalendar::getWork(DatePtr Date, TimeUnit format)
{
	ProjectCalendarDateRangesPtr ranges = getRanges(Date, NULL, (Day)NULL);
    time_t time = getTotalTime(ranges);
    return convertFormat(time, format);
}

/**
* This method retrieves a Duration instance representing the amount of
* work between two Dates based on this calendar.
* 
* @param startDate start Date
* @param endDate end Date
* @param format required duration format
* @return amount of work
*/
DurationPtr ProjectCalendar::getWork(DatePtr startDate, DatePtr endDate, TimeUnit format)
{
    DateRangePtr range (new DateRange(startDate, endDate));
	time_t cachedResult = 0;// = m_workingDateCache.get(range);
	time_t totalTime = 0;

	{
		BwProj::toolkit::CCriticalSectionSync sync(m_workingDateCacheSyncHandle);
// 		if (m_workingDateCache.Lookup(range->toString(), cachedResult))
// 			return convertFormat(cachedResult, format);

		auto itMap = m_workingDateCache.find(range->toString());
		if(itMap != m_workingDateCache.end())
			return convertFormat(itMap->second, format);
	}
    //
    // We want the start DatePtr to be the earliest Date, and the end DatePtr 
    // to be the latest Date. Set a flag here to indicate if we have swapped
    // the order of the supplied Date.
    //        
    bool invert = false;
    if (startDate->getTime() > endDate->getTime())
    {
		invert = true;
		DatePtr temp = startDate;
		startDate = endDate;
		endDate = temp;
    }

    DatePtr canonicalStartDate = DateHelper::getDayStartDate(startDate);
    DatePtr canonicalEndDate = DateHelper::getDayStartDate(endDate);

    if (canonicalStartDate->getTime() == canonicalEndDate->getTime())
    {
		ProjectCalendarDateRangesPtr ranges = getRanges(startDate, NULL, (Day)NULL);
		if (ranges->getRangeCount() != 0)
		{
			totalTime = getTotalTime(ranges, startDate, endDate);
		}
    }
    else
    {
		//
		// Find the first working day in the range
		//
		DatePtr currentDate = startDate;
		CalendarPtr cal = Calendar::getInstance();
		cal->setTime(startDate);
		Day day = (Day)cal->get(Calendar::DAY_OF_WEEK);

		while (isWorkingDate(currentDate, day) == false && currentDate->getTime() < canonicalEndDate->getTime())
		{
			cal->add(Calendar::DAY_OF_YEAR, 1);
			currentDate = cal->getTime();
			day = DateHelper::getNextDay(day);
		}

		if (currentDate->getTime() < canonicalEndDate->getTime())
		{
			//
			// Calculate the amount of working time for this day
			//
			totalTime += getTotalTime(getRanges(currentDate, NULL, day), currentDate, true);

			//
			// Process each working day until we reach the last day
			//
			while (true)
			{
				cal->add(Calendar::DAY_OF_YEAR, 1);
				currentDate = cal->getTime();
				day = DateHelper::getNextDay(day);

				//
				// We have reached the last day
				//
				if (currentDate->getTime() >= canonicalEndDate->getTime())
				{
					break;
				}

				//
				// Skip this day if it has no working time
				//
				ProjectCalendarDateRangesPtr ranges = getRanges(currentDate, NULL, day);
				if (ranges->getRangeCount() == 0)
				{
					continue;
				}

				//
				// Add the working time for the whole day
				//
				totalTime += getTotalTime(ranges);
			}
		}

		//
		// We are now at the last day
		//
		ProjectCalendarDateRangesPtr ranges = getRanges(endDate, NULL, day);
		if (ranges->getRangeCount() != 0)
		{
			totalTime += getTotalTime(ranges, DateHelper::getDayStartDate(endDate), endDate);
		}
    }

    if (invert)
    {
		totalTime = -totalTime;
    }

	{
		BwProj::toolkit::CCriticalSectionSync sync(m_workingDateCacheSyncHandle);
		//m_workingDateCache.put(range->toString(), totalTime);
		m_workingDateCache[range->toString()] = totalTime;
	}

    return convertFormat(totalTime, format);
}

/**
* Utility method used to convert an integer time representation into a 
* Duration instance.
* 
* @param totalTime integer time representation
* @param format required time format
* @return new Duration instance
*/
DurationPtr ProjectCalendar::convertFormat(time_t totalTime, TimeUnit format)
{
    double duration = (double) totalTime;
    double minutesPerDay = getParentFile()->getProjectHeader()->getMinutesPerDay()->doubleValue();
    double minutesPerWeek = getParentFile()->getProjectHeader()->getMinutesPerWeek()->doubleValue();
    double daysPerMonth = getParentFile()->getProjectHeader()->getDaysPerMonth()->doubleValue();

    switch (format)
    {
        case MINUTES:
        case ELAPSED_MINUTES:
        {
			duration /= (60 * 1000);
			break;
        }

        case HOURS:
        case ELAPSED_HOURS:
        {
			duration /= (60 * 60 * 1000);
			break;
        }

        case DAYS:
        {
			if (minutesPerDay != 0)
			{
				duration /= (minutesPerDay * 60 * 1000);
			}
			else
			{
				duration = 0;
			}
			break;
        }

        case WEEKS:
        {
			if (minutesPerWeek != 0)
			{
				duration /= (minutesPerWeek * 60 * 1000);
			}
			else
			{
				duration = 0;
			}
			break;
        }

        case MONTHS:
        {
			if (daysPerMonth != 0 && minutesPerDay != 0)
			{
				duration /= (daysPerMonth * minutesPerDay * 60 * 1000);
			}
			else
			{
				duration = 0;
			}
			break;
        }

        case ELAPSED_DAYS:
        {
			duration /= (24 * 60 * 60 * 1000);
			break;
        }

        case ELAPSED_WEEKS:
        {
			duration /= 604800000;// (7 * 24 * 60 * 60 * 1000);
			break;
        }

        case ELAPSED_MONTHS:
        {
			duration /= 2592000000;// (30 * 24 * 60 * 60 * 1000);
			break;
        }

        default:
        {
			throw std::bad_exception();// new IllegalArgumentException("TimeUnit " + format + " not supported");
        }
    }

    return (Duration2::getInstance(duration, format));
}

/**
* Retrieves the amount of time represented by a calendar exception
* before or after an intersection point.
* 
* @param exception calendar exception
* @param DatePtr intersection time
* @param after true to report time after intersection, false to report time before
* @return length of time in milliseconds
*/
time_t ProjectCalendar::getTotalTime(ProjectCalendarDateRangesPtr exception, DatePtr Date, bool after)
{
    time_t currentTime = DateHelper::getCanonicalTime(Date)->getTime();
	time_t total = 0;
	for (int i = 0; i < exception->getRangeCount(); i ++)
    {
		DateRangePtr range = exception->getRange(i);
		if (range == nullptr)
			continue;

        total += getTime(range->getStart(), range->getEnd(), currentTime, after);
    }
    return (total);
}

/**
* Retrieves the amount of working time represented by
* a calendar exception.
* 
* @param exception calendar exception
* @return length of time in milliseconds
*/
time_t ProjectCalendar::getTotalTime(ProjectCalendarDateRangesPtr exception)
{
	time_t total = 0;
	for (int i = 0; i < exception->getRangeCount(); i++)
	{
		DateRangePtr range = exception->getRange(i);
		if (range == nullptr)
			continue;

        total += getTime(range->getStart(), range->getEnd());
    }
    return (total);
}

/**
* This method calculates the total amount of working time in a single
* day, which intersects with the supplied time range->
* 
* @param hours collection of working hours in a day
* @param startDate time range start
* @param endDate time range end
* @return length of time in milliseconds
*/
time_t ProjectCalendar::getTotalTime(ProjectCalendarDateRangesPtr hours, DatePtr startDate, DatePtr endDate)
{
	time_t total = 0;
	if (startDate->getTime() == endDate->getTime())
		return total;

    DatePtr start = DateHelper::getCanonicalTime(startDate);
    DatePtr end = DateHelper::getCanonicalTime(endDate);

	for (int i = 0; i < hours->getRangeCount(); i++)
	{
		DateRangePtr range = hours->getRange(i);
		if (range == NULL)
			continue;

		DatePtr rangeStart = range->getStart();
		DatePtr rangeEnd = range->getEnd();
		if (rangeStart != NULL && rangeEnd != NULL)
		{
			DatePtr canoncialRangeStart = DateHelper::getCanonicalTime(rangeStart);
			DatePtr canonicalRangeEnd = DateHelper::getCanonicalTime(rangeEnd);

			DatePtr startDay = DateHelper::getDayStartDate(rangeStart);
			DatePtr finishDay = DateHelper::getDayStartDate(rangeEnd);

			//
			// Handle the case where the end of the range is at midnight -
			// this will show up as the start and end days not matching
			//
			if (startDay->getTime() != finishDay->getTime())
			{
				CalendarPtr calendar = Calendar::getInstance();
				calendar->setTime(canonicalRangeEnd);
				calendar->add(Calendar::DAY_OF_YEAR, 1);
				canonicalRangeEnd = calendar->getTime();
			}

			if (canoncialRangeStart->getTime() == canonicalRangeEnd->getTime() && rangeEnd->getTime() > rangeStart->getTime())
			{
				total += (24 * 60 * 60 * 1000);
			}
			else
			{
				total += getTime(start, end, canoncialRangeStart, canonicalRangeEnd);
			}
		}
    }

    return (total);
}

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
time_t ProjectCalendar::getTime(DatePtr start, DatePtr end, time_t target, bool after)
{
    time_t total = 0;
    if (start != NULL && end != NULL)
    {
        DatePtr startTime = DateHelper::getCanonicalTime(start);
        DatePtr endTime = DateHelper::getCanonicalTime(end);

        DatePtr startDay = DateHelper::getDayStartDate(start);
        DatePtr finishDay = DateHelper::getDayStartDate(end);

        //
        // Handle the case where the end of the range is at midnight -
        // this will show up as the start and end days not matching
        //
        if (startDay->getTime() != finishDay->getTime())
        {
			CalendarPtr calendar = Calendar::getInstance();
			calendar->setTime(endTime);
			calendar->add(Calendar::DAY_OF_YEAR, 1);
			endTime = calendar->getTime();
        }

        int diff = DateHelper::compare(startTime, endTime, target);
        if (diff == 0)
        {
			if (after == true)
			{
				total = (endTime->getTime() - target);
			}
			else
			{
				total = (target - startTime->getTime());
			}
        }
        else
        {
			if ((after == true && diff < 0) || (after == false && diff > 0))
			{
				total = (endTime->getTime() - startTime->getTime());
			}
        }
    }
    return (total);
}

/**
* Retrieves the amount of time between two DatePtr time values. Note that
* these values are converted into canonical values to remove the
* DatePtr component.
* 
* @param start start time
* @param end end time
* @return length of time
*/
time_t ProjectCalendar::getTime(DatePtr start, DatePtr end)
{
	time_t total = 0;
    if (start != NULL && end != NULL)
    {
        DatePtr startTime = DateHelper::getCanonicalTime(start);
        DatePtr endTime = DateHelper::getCanonicalTime(end);

        DatePtr startDay = DateHelper::getDayStartDate(start);
        DatePtr finishDay = DateHelper::getDayStartDate(end);

        //
        // Handle the case where the end of the range is at midnight -
        // this will show up as the start and end days not matching
        //
        if (startDay->getTime() != finishDay->getTime())
        {
			CalendarPtr calendar = Calendar::getInstance();
			calendar->setTime(endTime);
			calendar->add(Calendar::DAY_OF_YEAR, 1);
			endTime = calendar->getTime();
        }

        total = (endTime->getTime() - startTime->getTime());
    }
    return (total);
}

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
time_t ProjectCalendar::getTime(DatePtr start1, DatePtr end1, DatePtr start2, DatePtr end2)
{
	time_t total = 0;

    if (start1 != NULL && end1 != NULL && start2 != NULL && end2 != NULL)
    {
		time_t start = 0;
		time_t end = 0;

        if (start1->getTime() < start2->getTime())
        {
			start = start2->getTime();
        }
        else
        {
			start = start1->getTime();
        }

        if (end1->getTime() < end2->getTime())
        {
			end = end1->getTime();
        }
        else
        {
			end = end2->getTime();
        }

        if (start < end)
        {
			total = end - start;
        }
    }

    return (total);
}

/**
* Add a reference to a calendar derived from this one.
* 
* @param calendar derived calendar instance
*/
void ProjectCalendar::addDerivedCalendar(ProjectCalendar* calendar)
{
    m_derivedCalendars.Add(calendar);
}

/**
* Remove a reference to a derived calendar.
* 
* @param calendar derived calendar instance
*/
void ProjectCalendar::removeDerivedCalendar(ProjectCalendar* calendar)
{
    m_derivedCalendars.remove(calendar);
}

/**
* Retrieve a list of derived calendars.
* 
* @return list of derived calendars
*/
List<ProjectCalendar*>* ProjectCalendar::getDerivedCalendars()
{
    return &m_derivedCalendars;
}

///**
//* {@inheritDoc}
//*/
//CString toString()
//{
//    ByteArray2OutputStream os = new ByteArray2OutputStream();
//    PrintWriter pw = new PrintWriter(os);
//    pw.println("[ProjectCalendar");
//    pw.println("   ID=" + m_uniqueID);
//    pw.println("   name=" + getName());
//    pw.println("   baseCalendarName=" + (getParent() == NULL ? "" : getParent()->getName()));
//    pw.println("   resource=" + (m_resource == NULL ? "" : m_resource.getName()));
//
//    CString[] dayName =
//    {
//        "Sunday",
//        "Monday",
//        "Tuesday",
//        "Wednesday",
//        "Thursday",
//        "Friday",
//        "Saturday"
//    };
//
//    for (int loop = 0; loop < 7; loop++)
//    {
//        pw.println("   [Day " + dayName[loop]);
//        pw.println("      type=" + getDays()[loop]);
//        pw.println("      hours=" + getHours()[loop]);
//        pw.println("   ]");
//    }
//
//    if (!m_exceptions.isEmpty())
//    {
//        pw.println("   [Exceptions=");
//        for (ProjectCalendarException ex : m_exceptions)
//        {
//        pw.println("      " + ex.toString());
//        }
//        pw.println("   ]");
//    }
//
//    pw.println("]");
//    pw.flush();
//    return (os.toString());
//}

/**
* Copy the settings from another calendar to this calendar.
* 
* @param cal calendar data source
*/
void ProjectCalendar::copy(ProjectCalendar* cal)
{
    setName(cal->getName());
    setParent(cal->getParent());
	//memcpy(cal->getDays(), getDays(), sizeof(DayType)*ProjectCalendarWeek::WEEK_DAYS);

	cal->getDays()->RemoveAll();
	cal->getDays()->Append(*getDays());

	for (size_t i = 0; i < cal->m_exceptions.GetCount(); i ++)
    {
		ProjectCalendarExceptionPtr ex = cal->m_exceptions[i];
        addCalendarException(ex->getFromDate(), ex->getToDate());

		for (int j = 0; j < ex->getRangeCount(); j ++)
        {
			DateRangePtr range = ex->getRange(j);
			ex->addRange(DateRangePtr(new DateRange(range->getStart(), range->getEnd())));
        }
    }

	for (size_t i = 0; i < getHours()->GetCount(); i++)
    {
		ProjectCalendarHoursPtr hours = getHours()->GetAt(i);

        if (hours != NULL)
        {
			ProjectCalendarHoursPtr copyHours = cal->addCalendarHours(hours->getDay());
			for (int j = 0; j < hours->getRangeCount(); j++)
			{
				DateRangePtr range = hours->getRange(j);
				copyHours->addRange(DateRangePtr(new DateRange(range->getStart(), range->getEnd())));
			}
        }
    }
}

/**
* Utility method to clear cached calendar data.
*/
void ProjectCalendar::clearWorkingDateCache()
{
	BwProj::toolkit::CCriticalSectionSync sync(m_workingDateCacheSyncHandle);
	//m_workingDateCache.RemoveAll();
	m_workingDateCache.clear();

	BwProj::toolkit::CCriticalSectionSync sync2(m_startTimeCacheSyncHandle);
	//m_startTimeCache.RemoveAll();
	m_startTimeCache.clear();

	for (size_t i = 0; i < m_derivedCalendars.GetCount(); i ++)
    {
		ProjectCalendar* calendar = m_derivedCalendars[i];
        calendar->clearWorkingDateCache();
    }
}

/**
* Retrieves the working hours on the given Date.
* 
* @param DatePtr required Date
* @param cal optional calendar instance
* @param day optional day instance
* @return working hours
*/
ProjectCalendarDateRangesPtr ProjectCalendar::getRanges(DatePtr Date, CalendarPtr cal, Day day)
{
    ProjectCalendarDateRangesPtr ranges = getException(Date);
    if (ranges == NULL)
    {
        if (day == NULL)
        {
        if (cal == NULL)
        {
            cal = Calendar::getInstance();
            cal->setTime(Date);
        }
		day = (Day)cal->get(Calendar::DAY_OF_WEEK);
        }
        ranges = getHours(day);
    }
    return ranges;
}

/**
* Accessor method allowing retrieval of ProjectFile reference.
*
* @return reference to this the parent ProjectFile instance
*/
ProjectFile* ProjectCalendar::getParentFile()
{
    return (m_projectFile);
}