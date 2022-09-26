#include "stdafx.h"
#include "ProjectHeader.h"
#include "ProjectFile.h"

/**
* Default constructor.
*
* @param file the parent file to which this record belongs.
*/
ProjectHeader::ProjectHeader(ProjectFile* file) : ProjectEntity(file)
{
	///**
	//* Default work value.
	//*/
	//DEFAULT_WORK = DurationPtr::getInstance(0, TimeUnit::HOURS);

	///**
	//* Default duration value.
	//*/
	//DEFAULT_DURATION = DurationPtr::getInstance(0, TimeUnit::DAYS);


	//
	// Configure Date Time Settings and Currency Settings Records
	//
	setCurrencySymbol(_T("$"));
	setSymbolPosition(CurrencySymbolPosition::BEFORE);
	setCurrencyDigits(NumberPtr(new Number(2)));
	setThousandsSeparator(',');
	setDecimalSeparator('.');

	setDateOrder(DateOrder::DMY);
	setTimeFormat(ProjectTimeFormat::TWELVE_HOUR);
	setIntegerDefaultStartTime(480);
	setDateSeparator('/');
	setTimeSeparator(':');
	setAMText(_T("am"));
	setPMText(_T("pm"));
	setDateFormat(ProjectDateFormat::DD_MM_YYYY);
	setBarTextDateFormat(ProjectDateFormat::DD_MM_YYYY);

	//
	// Configure Default Settings Record
	//
	setDefaultDurationUnits(TimeUnit::DAYS);
	setDefaultDurationIsFixed(false);
	setDefaultWorkUnits(TimeUnit::HOURS);
	setMinutesPerDay(NumberPtr(new Number(480)));
	setMinutesPerWeek(NumberPtr(new Number(2400)));
	setDefaultStandardRate(RatePtr(new Rate(10, TimeUnit::HOURS)));
	setDefaultOvertimeRate(RatePtr(new Rate(15, TimeUnit::HOURS)));
	setUpdatingTaskStatusUpdatesResource2Status(true);
	setSplitInProgressTasks(false);

	//
	// Configure Project Header Record
	//
	setProjectTitle(_T("Project1"));
	setCompany(_T(""));
	setManager(_T(""));
	setCalendarName(DEFAULT_CALENDAR_NAME);
	//setStartDate(NULL);
	//setFinishDate(NULL);
	setScheduleFrom(DEFAULT_SCHEDULE_FROM);
	setCurrentDate(DatePtr(new Date()));
	//setComments(NULL);
	setCost(NumberPtr(new Number(DEFAULT_COST)));
	setBaselineCost(NumberPtr(new Number(DEFAULT_COST)));
	setActualCost(NumberPtr(new Number(DEFAULT_COST)));
	setWork(DEFAULT_WORK);
	setBaselineWork(DEFAULT_WORK);
	setActualWork(DEFAULT_WORK);
	setWork2(NumberPtr(new Number(DEFAULT_WORK2)));
	setDuration(DEFAULT_DURATION);
	setBaselineDuration(DEFAULT_DURATION);
	setActualDuration(DEFAULT_DURATION);
	setPercentageComplete(NumberPtr(new Number(DEFAULT_PERCENT_COMPLETE)));
	setBaselineStart(NULL);
	setBaselineFinish(NULL);
	setActualStart(NULL);
	setActualFinish(NULL);
	setStartVariance(DEFAULT_DURATION);
	setFinishVariance(DEFAULT_DURATION);
	setSubject(_T(""));
	setAuthor(_T(""));
	setKeywords(_T(""));

	//
	// Configure non-MPX attributes
	//
	setProjectExternallyEdited(false);
	setMinutesPerDay(NumberPtr(new Number(DEFAULT_MINUTES_PER_DAY)));
	setDaysPerMonth(NumberPtr(new Number(DEFAULT_DAYS_PER_MONTH)));
	setMinutesPerWeek(NumberPtr(new Number(DEFAULT_MINUTES_PER_WEEK)));
	setFiscalYearStart(false);
	setDefaultTaskEarnedValueMethod(EarnedValueMethod::PERCENT_COMPLETE);
	setNewTasksEstimated(true);
	setAutoAddNewResource2sAndTasks(true);
	setAutolink(true);
	setMicrosoftProjectServerURL(true);
	setDefaultTaskType(TaskType::FIXED_UNITS);
	setDefaultFixedCostAccrual(AccrueType::ACCRUE_END);
	setCriticalSlackLimit(DEFAULT_CRITICAL_SLACK_LIMIT);
	setBaselineForEarnedValue(DEFAULT_BASELINE_FOR_EARNED_VALUE);
	setFiscalYearStartMonth(DEFAULT_FISCAL_YEAR_START_MONTH);
	setNewTaskStartIsProjectStart(true);
	setWeekStartDay(DEFAULT_WEEK_START_DAY);
}

/**
* Gets Default DurationPtr units. The constants used to define the
* duration units are defined by the <code>TimeUnit</code> class.
*
* @return int constant
* @see TimeUnit
*/
TimeUnit ProjectHeader::getDefaultDurationUnits()
{
	return (m_defaultDurationUnits);
}

/**
* Default duration units. The constants used to define the
* duration units are defined by the <code>TimeUnit</code> class.
*
* @param units default time units
* @see TimeUnit
*/
void ProjectHeader::setDefaultDurationUnits(TimeUnit units)
{
	m_defaultDurationUnits = units;
}

/**
* Retrieves a flag indicating if the default duration type is fixed.
*
* @return bool flag
*/
bool ProjectHeader::getDefaultDurationIsFixed()
{
	return (m_defaultDurationIsFixed);
}

/**
* Sets a flag indicating if the default duration type is fixed.
*
* @param fixed bool flag
*/
void ProjectHeader::setDefaultDurationIsFixed(bool fixed)
{
	m_defaultDurationIsFixed = fixed;
}

/**
* Default work units. The constants used to define the
* work units are defined by the <code>TimeUnit</code> class.
*
* @return int representing default
* @see TimeUnit
*/
TimeUnit ProjectHeader::getDefaultWorkUnits()
{
	return (m_defaultWorkUnits);
}

/**
* Default work units. The constants used to define the
* work units are defined by the <code>TimeUnit</code> class.
*
* @param units  int representing default
* @see TimeUnit
*/
void ProjectHeader::setDefaultWorkUnits(TimeUnit units)
{
	m_defaultWorkUnits = units;
}

/**
* Retrieves the default standard rate.
*
* @return default standard rate
*/
RatePtr ProjectHeader::getDefaultStandardRate()
{
	return (m_defaultStandardRate);
}

/**
* Sets the default standard rate.
*
* @param rate default standard rate
*/
void ProjectHeader::setDefaultStandardRate(RatePtr rate)
{
	m_defaultStandardRate = rate;
}

/**
* Get overtime rate.
*
* @return rate
*/
RatePtr ProjectHeader::getDefaultOvertimeRate()
{
	return (m_defaultOvertimeRate);
}

/**
* Set default overtime rate.
*
* @param rate default overtime rate
*/
void ProjectHeader::setDefaultOvertimeRate(RatePtr rate)
{
	m_defaultOvertimeRate = rate;
}

/**
* Flags whether updating Task status also upDates resource status.
*
* @return bool flag
*/
bool ProjectHeader::getUpdatingTaskStatusUpdatesResource2Status()
{
	return (m_updatingTaskStatusUpdatesResource2Status);
}

/**
* Flags whether updating Task status also upDates resource status.
*
* @param flag bool flag
*/
void ProjectHeader::setUpdatingTaskStatusUpdatesResource2Status(bool flag)
{
	m_updatingTaskStatusUpdatesResource2Status = flag;
}

/**
* Flag representing whether or not to split in-progress tasks.
*
* @return bool value
*/
bool ProjectHeader::getSplitInProgressTasks()
{
	return (m_splitInProgressTasks);
}

/**
* Flag representing whether or not to split in-progress tasks.
*
* @param flag bool value
*/
void ProjectHeader::setSplitInProgressTasks(bool flag)
{
	m_splitInProgressTasks = flag;
}

/**
* Gets constant representing set Date order eg DMY, MDY.
*
* @return constant value for Date order
*/
DateOrder ProjectHeader::getDateOrder()
{
	return (m_DateOrder);
}

/**
* Sets constant representing set Date order eg DMY, MDY.
*
* @param DateOrder Date order value
*/
void ProjectHeader::setDateOrder(DateOrder DateOrder)
{
	m_DateOrder = DateOrder;
}

/**
* Gets constant representing the Time Format.
*
* @return time format constant
*/
ProjectTimeFormat ProjectHeader::getTimeFormat()
{
	return (m_timeFormat);
}

/**
* Sets constant representing the time format.
*
* @param timeFormat constant value
*/
void ProjectHeader::setTimeFormat(ProjectTimeFormat timeFormat)
{
	m_timeFormat = timeFormat;
}

/**
* This internal method is used to convert from an integer representing
* minutes past midnight into a Date instance whose time component
* represents the start time.
*
* @param time integer representing the start time in minutes past midnight
*/
void ProjectHeader::setIntegerDefaultStartTime(int time)
{
	//if (time != NULL)
	//{
	//	int minutes = time.intValue();
	//	int hours = minutes / 60;
	//	minutes -= (hours * 60);

	//	CalendarPtr cal = Calendar::getInstance();
	//	cal->set(Calendar::MILLISECOND, 0);
	//	cal->set(Calendar::SECOND, 0);
	//	cal->set(Calendar::MINUTE, minutes);
	//	cal->set(Calendar::HOUR_OF_DAY, hours);

	//	Date Date = cal->getTime();
	//	setDefaultStartTime(Date);
	//}

	int minutes = time;
	int hours = minutes / 60;
	minutes -= (hours * 60);

	DatePtr Date(new Date());
	Date->SetDate(0, 0, 0);
	Date->SetTime(hours, minutes, 0);
	setDefaultStartTime(Date);
}

/**
* Retrieve the default start time, specified using the Java Date type.
* Note that this assumes that the value returned from
* the getTime method starts at zero... i.e. the Date part
* of the Date/time value has not been set.
*
* @return default start time
*/
DatePtr ProjectHeader::getDefaultStartTime()
{
	return (m_defaultStartTime);
}

/**
* Set the default start time, specified using the Java Date type.
* Note that this assumes that the value returned from
* the getTime method starts at zero... i.e. the Date part
* of the Date/time value has not been set.
*
* @param defaultStartTime default time
*/
void ProjectHeader::setDefaultStartTime(DatePtr defaultStartTime)
{
	m_defaultStartTime = defaultStartTime;
}

/**
* Gets the Date separator.
*
* @return Date separator as set.
*/
char ProjectHeader::getDateSeparator()
{
	return (m_DateSeparator);
}

/**
* Sets the Date separator.
*
* @param DateSeparator Date separator as set.
*/
void ProjectHeader::setDateSeparator(char DateSeparator)
{
	m_DateSeparator = DateSeparator;
}

/**
* Gets the time separator.
*
* @return time separator as set.
*/
char ProjectHeader::getTimeSeparator()
{
	return (m_timeSeparator);
}

/**
* Sets the time separator.
*
* @param timeSeparator time separator
*/
void ProjectHeader::setTimeSeparator(char timeSeparator)
{
	m_timeSeparator = timeSeparator;
}

///**
//* Sets the time separator.
//*
//* @param timeSeparator time separator
//*/
//void ProjectHeader::setTimeSeparator(Character timeSeparator)
//{
//	setTimeSeparator((timeSeparator == NULL ? DEFAULT_TIME_SEPARATOR : timeSeparator.charValue()));
//}

/**
* Gets the AM text.
*
* @return AM Text as set.
*/
CString ProjectHeader::getAMText()
{
	return (m_amText);
}

/**
* Sets the AM text.
*
* @param amText AM Text as set.
*/
void ProjectHeader::setAMText(CString amText)
{
	m_amText = amText;
}

/**
* Gets the PM text.
*
* @return PM Text as set.
*/
CString ProjectHeader::getPMText()
{
	return (m_pmText);
}

/**
* Sets the PM text.
*
* @param pmText PM Text as set.
*/
void ProjectHeader::setPMText(CString pmText)
{
	m_pmText = pmText;
}

/**
* Gets the set Date Format.
*
* @return int representing Date Format
*/
ProjectDateFormat ProjectHeader::getDateFormat()
{
	return (m_DateFormat);
}

/**
* Sets the set Date Format.
*
* @param DateFormat int representing Date Format
*/
void ProjectHeader::setDateFormat(ProjectDateFormat DateFormat)
{
	m_DateFormat = DateFormat;
}

/**
* Gets Bar Text Date Format.
*
* @return int value
*/
ProjectDateFormat ProjectHeader::getBarTextDateFormat()
{
	return (m_barTextDateFormat);
}

/**
* Sets Bar Text Date Format.
*
* @param DateFormat value to be set
*/
void ProjectHeader::setBarTextDateFormat(ProjectDateFormat DateFormat)
{
	m_barTextDateFormat = DateFormat;
}

/**
* Retrieves the default end time.
*
* @return End time
*/
DatePtr ProjectHeader::getDefaultEndTime()
{
	return (m_defaultEndTime);
}

/**
* Sets the default end time.
*
* @param Date End time
*/
void ProjectHeader::setDefaultEndTime(DatePtr Date)
{
	m_defaultEndTime = Date;
}

/**
* Sets the project title.
*
* @param projectTitle project title
*/
void ProjectHeader::setProjectTitle(CString projectTitle)
{
	m_projectTitle = projectTitle;
}

/**
* Gets the project title.
*
* @return project title
*/
CString ProjectHeader::getProjectTitle()
{
	return (m_projectTitle);
}

/**
* Sets the company name.
*
* @param company company name
*/
void ProjectHeader::setCompany(CString company)
{
	m_company = company;
}

/**
* Retrieves the company name.
*
* @return company name
*/
CString ProjectHeader::getCompany()
{
	return (m_company);
}

/**
* Sets the manager name.
*
* @param manager manager name
*/
void ProjectHeader::setManager(CString manager)
{
	m_manager = manager;
}

/**
* Retrieves the manager name.
*
* @return manager name
*/
CString ProjectHeader::getManager()
{
	return (m_manager);
}

/**
* Sets the CalendarPtr used. 'Standard' if no value is set.
*
* @param calendarName CalendarPtr name
*/
void ProjectHeader::setCalendarName(CString calendarName)
{
	if (calendarName.GetLength() == 0)
	{
		calendarName = DEFAULT_CALENDAR_NAME;
	}

	m_calendarName = calendarName;
}

/**
* Gets the CalendarPtr used. 'Standard' if no value is set.
*
* @return CalendarPtr name
*/
CString ProjectHeader::getCalendarName()
{
	return (m_calendarName);
}

/**
* Sets the project start Date.
*
* @param startDate project start Date
*/
void ProjectHeader::setStartDate(DatePtr startDate)
{
	m_startDate = startDate;
}

/**
* Retrieves the project start Date. If an explicit start Date has not been
* set, this method calculates the start Date by looking for
* the earliest task start Date.
*
* @return project start Date
*/
DatePtr ProjectHeader::getStartDate()
{
	if (m_startDate == NULL)
	{
		return getParentFile()->getStartDate();
	}
	return m_startDate;
}

/**
* Retrieves the project finish Date. If an explicit finish Date has not been
* set, this method calculates the finish Date by looking for
* the latest task finish Date.
*
* @return Finish Date
*/
DatePtr ProjectHeader::getFinishDate()
{
	if (m_finishDate == NULL)
	{
		return getParentFile()->getFinishDate();
	}
	return m_finishDate;
}

/**
* Sets the project finish Date.
*
* @param finishDate project finish Date
*/
void ProjectHeader::setFinishDate(DatePtr finishDate)
{
	m_finishDate = finishDate;
}

/**
* Retrieves an enumerated value indicating if tasks in this project are
* scheduled from a start or a finish Date.
*
* @return schedule from flag
*/
ScheduleFrom ProjectHeader::getScheduleFrom()
{
	return (m_scheduleFrom);
}

/**
* Sets an enumerated value indicating if tasks in this project are
* scheduled from a start or a finish Date.
*
* @param scheduleFrom schedule from value
*/
void ProjectHeader::setScheduleFrom(ScheduleFrom scheduleFrom)
{
	m_scheduleFrom = scheduleFrom;
}

/**
* Retrieves the current Date.
*
* @return current Date
*/
DatePtr ProjectHeader::getCurrentDate()
{
	return (m_currentDate);
}

/**
* Sets the current Date.
*
* @param currentDate current Date
*/
void ProjectHeader::setCurrentDate(DatePtr currentDate)
{
	m_currentDate = currentDate;
}

/**
* Returns any comments.
*
* @return comments attached to the Project Header
*/
CString ProjectHeader::getComments()
{
	return (m_comments);
}

/**
* Set comment text.
*
* @param comments comment text
*/
void ProjectHeader::setComments(CString comments)
{
	m_comments = comments;
}

/**
* Retrieves the project cost.
*
* @return project cost
*/
NumberPtr ProjectHeader::getCost()
{
	return (m_cost);
}

/**
* Sets the project cost.
*
* @param cost project cost
*/
void ProjectHeader::setCost(NumberPtr cost)
{
	m_cost = cost;
}

/**
* Sets the baseline project cost.
*
* @param baselineCost baseline project cost
*/
void ProjectHeader::setBaselineCost(NumberPtr baselineCost)
{
	m_baselineCost = baselineCost;
}

/**
* Retrieves the baseline project cost.
*
* @return baseline project cost
*/
NumberPtr ProjectHeader::getBaselineCost()
{
	return (m_baselineCost);
}

/**
* Sets the actual project cost.
*
* @param actualCost actual project cost
*/
void ProjectHeader::setActualCost(NumberPtr actualCost)
{
	m_actualCost = actualCost;
}

/**
* Retrieves the actual project cost.
*
* @return actual project cost
*/
NumberPtr ProjectHeader::getActualCost()
{
	return (m_actualCost);
}

/**
* Sets the project work duration.
*
* @param work project work duration
*/
void ProjectHeader::setWork(DurationPtr work)
{
	m_work = work;
}

/**
* Retrieves the project work duration.
*
* @return project work duration
*/
DurationPtr ProjectHeader::getWork()
{
	return (m_work);
}

/**
* Set the baseline project work duration.
*
* @param baselineWork baseline project work duration
*/
void ProjectHeader::setBaselineWork(DurationPtr baselineWork)
{
	m_baselineWork = baselineWork;
}

/**
* Retrieves the baseline project work duration.
*
* @return baseline project work duration
*/
DurationPtr ProjectHeader::getBaselineWork()
{
	return (m_baselineWork);
}

/**
* Sets the actual project work duration.
*
* @param actualWork actual project work duration
*/
void ProjectHeader::setActualWork(DurationPtr actualWork)
{
	m_actualWork = actualWork;
}

/**
* Retrieves the actual project work duration.
*
* @return actual project work duration
*/
DurationPtr ProjectHeader::getActualWork()
{
	return (m_actualWork);
}

/**
* Retrieves the project's "Work 2" attribute.
*
* @return Work 2 attribute
*/
NumberPtr ProjectHeader::getWork2()
{
	return (m_work2);
}

/**
* Sets the project's "Work 2" attribute.
*
* @param work2 work2 percentage value
*/
void ProjectHeader::setWork2(NumberPtr work2)
{
	m_work2 = work2;
}

/**
* Retrieves the project duration.
*
* @return project duration
*/
DurationPtr ProjectHeader::getDuration()
{
	return (m_duration);
}

/**
* Sets the project duration.
*
* @param duration project duration
*/
void ProjectHeader::setDuration(DurationPtr duration)
{
	m_duration = duration;
}

/**
* Retrieves the baseline duration value.
*
* @return baseline project duration value
*/
DurationPtr ProjectHeader::getBaselineDuration()
{
	return (m_baselineDuration);
}

/**
* Sets the baseline project duration value.
*
* @param baselineDuration baseline project duration
*/
void ProjectHeader::setBaselineDuration(DurationPtr baselineDuration)
{
	m_baselineDuration = baselineDuration;
}

/**
* Retrieves the actual project duration.
*
* @return actual project duration
*/
DurationPtr ProjectHeader::getActualDuration()
{
	return (m_actualDuration);
}

/**
* Sets the actual project duration.
*
* @param actualDuration actual project duration
*/
void ProjectHeader::setActualDuration(DurationPtr actualDuration)
{
	m_actualDuration = actualDuration;
}

/**
* Retrieves the project percentage complete.
*
* @return percentage value
*/
NumberPtr ProjectHeader::getPercentageComplete()
{
	return (m_percentageComplete);
}

/**
* Sets project percentage complete.
*
* @param percentComplete project percent complete
*/
void ProjectHeader::setPercentageComplete(NumberPtr percentComplete)
{
	m_percentageComplete = percentComplete;
}

/**
* Sets the baseline project start Date.
*
* @param baselineStartDate baseline project start Date
*/
void ProjectHeader::setBaselineStart(DatePtr baselineStartDate)
{
	m_baselineStart = baselineStartDate;
}

/**
* Retrieves the baseline project start Date.
*
* @return baseline project start Date
*/
DatePtr ProjectHeader::getBaselineStart()
{
	return (m_baselineStart);
}

/**
* Sets the baseline project finish Date.
*
* @param baselineFinishDate baseline project finish Date
*/
void ProjectHeader::setBaselineFinish(DatePtr baselineFinishDate)
{
	m_baselineFinish = baselineFinishDate;
}

/**
* Retrieves the baseline project finish Date.
*
* @return baseline project finish Date
*/
DatePtr ProjectHeader::getBaselineFinish()
{
	return (m_baselineFinish);
}

/**
* Sets the actual project start Date.
*
* @param actualStartDate actual project start Date
*/
void ProjectHeader::setActualStart(DatePtr actualStartDate)
{
	m_actualStart = actualStartDate;
}

/**
* Retrieves the actual project start Date.
*
* @return actual project start Date
*/
DatePtr ProjectHeader::getActualStart()
{
	return (m_actualStart);
}

/**
* Sets the actual project finish Date.
*
* @param actualFinishDate actual project finish Date
*/
void ProjectHeader::setActualFinish(DatePtr actualFinishDate)
{
	m_actualFinish = actualFinishDate;
}

/**
* Retrieves the actual project finish Date.
*
* @return actual project finish Date
*/
DatePtr ProjectHeader::getActualFinish()
{
	return (m_actualFinish);
}

/**
* Retrieves the start variance duration.
*
* @return start Date variance
*/
DurationPtr ProjectHeader::getStartVariance()
{
	return (m_startVariance);
}

/**
* Sets the start variance duration.
*
* @param startVariance the start Date variance
*/
void ProjectHeader::setStartVariance(DurationPtr startVariance)
{
	m_startVariance = startVariance;
}

/**
* Retrieves the project finish variance duration.
*
* @return project finish variance duration
*/
DurationPtr ProjectHeader::getFinishVariance()
{
	return (m_finishVariance);
}

/**
* Sets the project finish variance duration.
*
* @param finishVariance project finish variance duration
*/
void ProjectHeader::setFinishVariance(DurationPtr finishVariance)
{
	m_finishVariance = finishVariance;
}

/**
* Returns the project subject text.
*
* @return subject text
*/
CString ProjectHeader::getSubject()
{
	return (m_subject);
}

/**
* Sets the project subject text.
*
* @param subject subject text
*/
void ProjectHeader::setSubject(CString subject)
{
	m_subject = subject;
}

/**
* Retrieves the project author text.
*
* @return author text
*/
CString ProjectHeader::getAuthor()
{
	return (m_author);
}

/**
* Sets the project author text.
*
* @param author project author text
*/
void ProjectHeader::setAuthor(CString author)
{
	m_author = author;
}

/**
* Retrieves the project keyword text.
*
* @return project keyword text
*/
CString ProjectHeader::getKeywords()
{
	return (m_keywords);
}

/**
* Sets the project keyword text.
*
* @param keywords project keyword text
*/
void ProjectHeader::setKeywords(CString keywords)
{
	m_keywords = keywords;
}

/**
* Sets currency symbol.
*
* @param symbol currency symbol
*/
void ProjectHeader::setCurrencySymbol(CString symbol)
{
	if (symbol.GetLength () == 0)
	{
		symbol = _T("$");
	}

	m_currencySymbol = symbol;
}

/**
* Retrieves the currency symbol.
*
* @return currency symbol
*/
CString ProjectHeader::getCurrencySymbol()
{
	return (m_currencySymbol);
}

/**
* Sets the position of the currency symbol.
*
* @param posn currency symbol position.
*/
void ProjectHeader::setSymbolPosition(CurrencySymbolPosition posn)
{
	m_symbolPosition = posn;
}

/**
* Retrieves a constant representing the position of the currency symbol.
*
* @return position
*/
CurrencySymbolPosition ProjectHeader::getSymbolPosition()
{
	return (m_symbolPosition);
}

/**
* Sets no of currency digits.
*
* @param currDigs Available values, 0,1,2
*/
void ProjectHeader::setCurrencyDigits(NumberPtr currDigs)
{
	m_currencyDigits = currDigs;
}

/**
* Gets no of currency digits.
*
* @return Available values, 0,1,2
*/
NumberPtr ProjectHeader::getCurrencyDigits()
{
	return (m_currencyDigits);
}

/**
* Sets the thousands separator.
* Note that this separator defines the thousands separator for all decimal
* numbers that appear in the MPX file->
*
* @param sep character
*/
void ProjectHeader::setThousandsSeparator(char sep)
{
	m_thousandsSeparator = sep;
}

/**
* Gets the thousands separator.
* Note that this separator defines the thousands separator for all decimal
* numbers that appear in the MPX file->
*
* @return character
*/
char ProjectHeader::getThousandsSeparator()
{
	return (m_thousandsSeparator);
}

/**
* Sets the decimal separator.
* Note that this separator defines the decimal separator for all decimal
* numbers that appear in the MPX file->
*
* @param decSep character
*/
void ProjectHeader::setDecimalSeparator(char decSep)
{
	m_decimalSeparator = decSep;
}

/**
* Gets the decimal separator.
* Note that this separator defines the decimal separator for all decimal
* numbers that appear in the MPX file->
*
* @return character
*/
char ProjectHeader::getDecimalSeparator()
{
	return (m_decimalSeparator);
}

/**
* Retrieve the externally edited flag.
*
* @return externally edited flag
*/
bool ProjectHeader::getProjectExternallyEdited()
{
	return (m_projectExternallyEdited);
}

/**
* Set the externally edited flag.
*
* @param projectExternallyEdited externally edited flag
*/
void ProjectHeader::setProjectExternallyEdited(bool projectExternallyEdited)
{
	m_projectExternallyEdited = projectExternallyEdited;
}

/**
* Retrieves the category text.
*
* @return category text
*/
CString ProjectHeader::getCategory()
{
	return (m_category);
}

/**
* Sets the category text.
*
* @param category category text
*/
void ProjectHeader::setCategory(CString category)
{
	m_category = category;
}

/**
* Retrieve the number of days per month.
*
* @return days per month
*/
NumberPtr ProjectHeader::getDaysPerMonth()
{
	return (m_daysPerMonth);
}

/**
* Set the number of days per month.
*
* @param daysPerMonth days per month
*/
void ProjectHeader::setDaysPerMonth(NumberPtr daysPerMonth)
{
	if (daysPerMonth)
	{
		m_daysPerMonth = daysPerMonth;
	}
}

/**
* Retrieve the number of minutes per day.
*
* @return minutes per day
*/
NumberPtr ProjectHeader::getMinutesPerDay()
{
	return (m_minutesPerDay);
}

/**
* Set the number of minutes per day.
*
* @param minutesPerDay minutes per day
*/
void ProjectHeader::setMinutesPerDay(NumberPtr minutesPerDay)
{
	if (minutesPerDay != NULL)
	{
		m_minutesPerDay = minutesPerDay;
	}
}

/**
* Retrieve the number of minutes per week.
*
* @return minutes per week
*/
NumberPtr ProjectHeader::getMinutesPerWeek()
{
	return m_minutesPerWeek;
}

/**
* Set the number of minutes per week.
*
* @param minutesPerWeek minutes per week
*/
void ProjectHeader::setMinutesPerWeek(NumberPtr minutesPerWeek)
{
	if (minutesPerWeek != NULL)
	{
		m_minutesPerWeek = minutesPerWeek;
	}
}

/**
* Retrieve the fiscal year start flag.
*
* @return fiscal year start flag
*/
bool ProjectHeader::getFiscalYearStart()
{
	return (m_fiscalYearStart);
}

/**
* Set the fiscal year start flag.
*
* @param fiscalYearStart fiscal year start
*/
void ProjectHeader::setFiscalYearStart(bool fiscalYearStart)
{
	m_fiscalYearStart = fiscalYearStart;
}

/**
* Retrieves the default task earned value method.
*
* @return default task earned value method
*/
EarnedValueMethod ProjectHeader::getDefaultTaskEarnedValueMethod()
{
	return m_defaultTaskEarnedValueMethod;
}

/**
* Sets the default task earned value method.
*
* @param defaultTaskEarnedValueMethod default task earned value method
*/
void ProjectHeader::setDefaultTaskEarnedValueMethod(EarnedValueMethod defaultTaskEarnedValueMethod)
{
	m_defaultTaskEarnedValueMethod = defaultTaskEarnedValueMethod;
}

/**
* Retrieve the remove file properties flag.
*
* @return remove file properties flag
*/
bool ProjectHeader::getRemoveFileProperties()
{
	return (m_removeFileProperties);
}

/**
* Set the remove file properties flag.
*
* @param removeFileProperties remove file properties flag
*/
void ProjectHeader::setRemoveFileProperties(bool removeFileProperties)
{
	m_removeFileProperties = removeFileProperties;
}

/**
* Retrieve the move completed ends back flag.
*
* @return move completed ends back flag
*/
bool ProjectHeader::getMoveCompletedEndsBack()
{
	return (m_moveCompletedEndsBack);
}

/**
* Set the move completed ends back flag.
*
* @param moveCompletedEndsBack move completed ends back flag
*/
void ProjectHeader::setMoveCompletedEndsBack(bool moveCompletedEndsBack)
{
	m_moveCompletedEndsBack = moveCompletedEndsBack;
}

/**
* Retrieve the new tasks estimated flag.
*
* @return new tasks estimated flag
*/
bool ProjectHeader::getNewTasksEstimated()
{
	return (m_newTasksEstimated);
}

/**
* Set the new tasks estimated flag.
*
* @param newTasksEstimated new tasks estimated flag
*/
void ProjectHeader::setNewTasksEstimated(bool newTasksEstimated)
{
	m_newTasksEstimated = newTasksEstimated;
}

/**
* Retrieve the spread actual cost flag.
*
* @return spread actual cost flag
*/
bool ProjectHeader::getSpreadActualCost()
{
	return (m_spreadActualCost);
}

/**
* Set the spread actual cost flag.
*
* @param spreadActualCost spread actual cost flag
*/
void ProjectHeader::setSpreadActualCost(bool spreadActualCost)
{
	m_spreadActualCost = spreadActualCost;
}

/**
* Retrieve the multiple critical paths flag.
*
* @return multiple critical paths flag
*/
bool ProjectHeader::getMultipleCriticalPaths()
{
	return (m_multipleCriticalPaths);
}

/**
* Set the multiple critical paths flag.
*
* @param multipleCriticalPaths multiple critical paths flag
*/
void ProjectHeader::setMultipleCriticalPaths(bool multipleCriticalPaths)
{
	m_multipleCriticalPaths = multipleCriticalPaths;
}

/**
* Retrieve the auto add new resources and tasks flag.
*
* @return auto add new resources and tasks flag
*/
bool ProjectHeader::getAutoAddNewResource2sAndTasks()
{
	return (m_autoAddNewResource2sAndTasks);
}

/**
* Set the auto add new resources and tasks flag.
*
* @param autoAddNewResource2sAndTasks auto add new resources and tasks flag
*/
void ProjectHeader::setAutoAddNewResource2sAndTasks(bool autoAddNewResource2sAndTasks)
{
	m_autoAddNewResource2sAndTasks = autoAddNewResource2sAndTasks;
}

/**
* Retrieve the last saved Date.
*
* @return last saved Date
*/
DatePtr ProjectHeader::getLastSaved()
{
	return (m_lastSaved);
}

/**
* Set the last saved Date.
*
* @param lastSaved last saved Date
*/
void ProjectHeader::setLastSaved(DatePtr lastSaved)
{
	m_lastSaved = lastSaved;
}

/**
* Retrieve the status Date.
*
* @return status Date
*/
DatePtr ProjectHeader::getStatusDate()
{
	return (m_statusDate);
}

/**
* Set the status Date.
*
* @param statusDate status Date
*/
void ProjectHeader::setStatusDate(DatePtr statusDate)
{
	m_statusDate = statusDate;
}

/**
* Retrieves the move remaining starts back flag.
*
* @return move remaining starts back flag
*/
bool ProjectHeader::getMoveRemainingStartsBack()
{
	return (m_moveRemainingStartsBack);
}

/**
* Sets the move remaining starts back flag.
*
* @param moveRemainingStartsBack remaining starts back flag
*/
void ProjectHeader::setMoveRemainingStartsBack(bool moveRemainingStartsBack)
{
	m_moveRemainingStartsBack = moveRemainingStartsBack;
}

/**
* Retrieves the autolink flag.
*
* @return autolink flag
*/
bool ProjectHeader::getAutolink()
{
	return (m_autolink);
}

/**
* Sets the autolink flag.
*
* @param autolink autolink flag
*/
void ProjectHeader::setAutolink(bool autolink)
{
	m_autolink = autolink;
}

/**
* Retrieves the Microsoft Project Server URL flag.
*
* @return Microsoft Project Server URL flag
*/
bool ProjectHeader::getMicrosoftProjectServerURL()
{
	return (m_microsoftProjectServerURL);
}

/**
* Sets the Microsoft Project Server URL flag.
*
* @param microsoftProjectServerURL Microsoft Project Server URL flag
*/
void ProjectHeader::setMicrosoftProjectServerURL(bool microsoftProjectServerURL)
{
	m_microsoftProjectServerURL = microsoftProjectServerURL;
}

/**
* Retrieves the honor constraints flag.
*
* @return honor constraints flag
*/
bool ProjectHeader::getHonorConstraints()
{
	return (m_honorConstraints);
}

/**
* Sets the honor constraints flag.
*
* @param honorConstraints honor constraints flag
*/
void ProjectHeader::setHonorConstraints(bool honorConstraints)
{
	m_honorConstraints = honorConstraints;
}

/**
* Retrieve the admin project flag.
*
* @return admin project flag
*/
bool ProjectHeader::getAdminProject()
{
	return (m_adminProject);
}

/**
* Set the admin project flag.
*
* @param adminProject admin project flag
*/
void ProjectHeader::setAdminProject(bool adminProject)
{
	m_adminProject = adminProject;
}

/**
* Retrieves the inserted projects like summary flag.
*
* @return inserted projects like summary flag
*/
bool ProjectHeader::getInsertedProjectsLikeSummary()
{
	return (m_insertedProjectsLikeSummary);
}

/**
* Sets the inserted projects like summary flag.
*
* @param insertedProjectsLikeSummary inserted projects like summary flag
*/
void ProjectHeader::setInsertedProjectsLikeSummary(bool insertedProjectsLikeSummary)
{
	m_insertedProjectsLikeSummary = insertedProjectsLikeSummary;
}

/**
* Retrieves the project name.
*
* @return project name
*/
CString ProjectHeader::getName()
{
	return (m_name);
}

/**
* Sets the project name.
*
* @param name project name
*/
void ProjectHeader::setName(CString name)
{
	m_name = name;
}

/**
* Retrieves the spread percent complete flag.
*
* @return spread percent complete flag
*/
bool ProjectHeader::getSpreadPercentComplete()
{
	return (m_spreadPercentComplete);
}

/**
* Sets the spread percent complete flag.
*
* @param spreadPercentComplete spread percent complete flag
*/
void ProjectHeader::setSpreadPercentComplete(bool spreadPercentComplete)
{
	m_spreadPercentComplete = spreadPercentComplete;
}

/**
* Retrieve the move completed ends forward flag.
*
* @return move completed ends forward flag
*/
bool ProjectHeader::getMoveCompletedEndsForward()
{
	return (m_moveCompletedEndsForward);
}

/**
* Sets the move completed ends forward flag.
*
* @param moveCompletedEndsForward move completed ends forward flag
*/
void ProjectHeader::setMoveCompletedEndsForward(bool moveCompletedEndsForward)
{
	m_moveCompletedEndsForward = moveCompletedEndsForward;
}

/**
* Retrieve the editable actual costs flag.
*
* @return editable actual costs flag
*/
bool ProjectHeader::getEditableActualCosts()
{
	return (m_editableActualCosts);
}

/**
* Set the editable actual costs flag.
*
* @param editableActualCosts editable actual costs flag
*/
void ProjectHeader::setEditableActualCosts(bool editableActualCosts)
{
	m_editableActualCosts = editableActualCosts;
}

/**
* Retrieve the unique ID for this project.
*
* @return unique ID
*/
CString ProjectHeader::getUniqueID()
{
	return (m_uniqueID);
}

/**
* Set the unique ID for this project.
*
* @param uniqueID unique ID
*/
void ProjectHeader::setUniqueID(CString uniqueID)
{
	m_uniqueID = uniqueID;
}

/**
* Retrieve the project revision number.
*
* @return revision number
*/
int ProjectHeader::getRevision()
{
	return (m_revision);
}

/**
* Retrieve the new tasks effort driven flag.
*
* @return new tasks effort driven flag
*/
bool ProjectHeader::getNewTasksEffortDriven()
{
	return (m_newTasksEffortDriven);
}

/**
* Sets the new tasks effort driven flag.
*
* @param newTasksEffortDriven new tasks effort driven flag
*/
void ProjectHeader::setNewTasksEffortDriven(bool newTasksEffortDriven)
{
	m_newTasksEffortDriven = newTasksEffortDriven;
}

/**
* Set the project revision number.
*
* @param revision revision number
*/
void ProjectHeader::setRevision(int revision)
{
	m_revision = revision;
}

/**
* Retrieve the move remaining starts forward flag.
*
* @return move remaining starts forward flag
*/
bool ProjectHeader::getMoveRemainingStartsForward()
{
	return (m_moveRemainingStartsForward);
}

/**
* Set the move remaining starts forward flag.
*
* @param moveRemainingStartsForward move remaining starts forward flag
*/
void ProjectHeader::setMoveRemainingStartsForward(bool moveRemainingStartsForward)
{
	m_moveRemainingStartsForward = moveRemainingStartsForward;
}

/**
* Retrieve the actuals in sync flag.
*
* @return actuals in sync flag
*/
bool ProjectHeader::getActualsInSync()
{
	return (m_actualsInSync);
}

/**
* Set the actuals in sync flag.
*
* @param actualsInSync actuals in sync flag
*/
void ProjectHeader::setActualsInSync(bool actualsInSync)
{
	m_actualsInSync = actualsInSync;
}

/**
* Retrieve the default task type.
*
* @return default task type
*/
TaskType ProjectHeader::getDefaultTaskType()
{
	return (m_defaultTaskType);
}

/**
* Set the default task type.
*
* @param defaultTaskType default task type
*/
void ProjectHeader::setDefaultTaskType(TaskType defaultTaskType)
{
	m_defaultTaskType = defaultTaskType;
}

/**
* Retrieve the earned value method.
*
* @return earned value method
*/
EarnedValueMethod ProjectHeader::getEarnedValueMethod()
{
	return (m_earnedValueMethod);
}

/**
* Set the earned value method.
*
* @param earnedValueMethod earned value method
*/
void ProjectHeader::setEarnedValueMethod(EarnedValueMethod earnedValueMethod)
{
	m_earnedValueMethod = earnedValueMethod;
}

/**
* Retrieve the project creation Date.
*
* @return project creation Date
*/
DatePtr ProjectHeader::getCreationDate()
{
	return (m_creationDate);
}

/**
* Set the project creation Date.
*
* @param creationDate project creation Date
*/
void ProjectHeader::setCreationDate(DatePtr creationDate)
{
	m_creationDate = creationDate;
}

/**
* Retrieve the extended creation Date.
*
* @return extended creation Date
*/
DatePtr ProjectHeader::getExtendedCreationDate()
{
	return (m_extendedCreationDate);
}

/**
* Retrieve the default fixed cost accrual type.
*
* @return default fixed cost accrual type
*/
AccrueType ProjectHeader::getDefaultFixedCostAccrual()
{
	return (m_defaultFixedCostAccrual);
}

/**
* Sets the default fixed cost accrual type.
*
* @param defaultFixedCostAccrual default fixed cost accrual type
*/
void ProjectHeader::setDefaultFixedCostAccrual(AccrueType defaultFixedCostAccrual)
{
	m_defaultFixedCostAccrual = defaultFixedCostAccrual;
}

/**
* Set the extended creation Date.
*
* @param creationDate extended creation Date
*/
void ProjectHeader::setExtendedCreationDate(DatePtr creationDate)
{
	m_extendedCreationDate = creationDate;
}

/**
* Retrieve the critical slack limit.
*
* @return critical slack limit
*/
int ProjectHeader::getCriticalSlackLimit()
{
	return (m_criticalSlackLimit);
}

/**
* Set the critical slack limit.
*
* @param criticalSlackLimit critical slack limit
*/
void ProjectHeader::setCriticalSlackLimit(int criticalSlackLimit)
{
	m_criticalSlackLimit = criticalSlackLimit;
}

/**
* Retrieve the number of the baseline to use for earned value
* calculations.
*
* @return baseline for earned value
*/
int ProjectHeader::getBaselineForEarnedValue()
{
	return (m_baselineForEarnedValue);
}

/**
* Set the number of the baseline to use for earned value
* calculations.
*
* @param baselineForEarnedValue baseline for earned value
*/
void ProjectHeader::setBaselineForEarnedValue(int baselineForEarnedValue)
{
	m_baselineForEarnedValue = baselineForEarnedValue;
}

/**
* Retrieves the fiscal year start month (January=1, December=12).
*
* @return fiscal year start month
*/
int ProjectHeader::getFiscalYearStartMonth()
{
	return (m_fiscalYearStartMonth);
}

/**
* Sets the fiscal year start month (January=1, December=12).
*
* @param fiscalYearStartMonth fiscal year start month
*/
void ProjectHeader::setFiscalYearStartMonth(int fiscalYearStartMonth)
{
	m_fiscalYearStartMonth = fiscalYearStartMonth;
}

/**
* Retrieve the flag indicating if new tasks should default to the
* project start Date (true) or the current Date (false).
*
* @return new task start is project start
*/
bool ProjectHeader::getNewTaskStartIsProjectStart()
{
	return (m_newTaskStartIsProjectStart);
}

/**
* Sets the flag indicating if new tasks should default to the
* project start Date (true) or the current Date (false).
*
* @param newTaskStartIsProjectStart new task start is project start
*/
void ProjectHeader::setNewTaskStartIsProjectStart(bool newTaskStartIsProjectStart)
{
	m_newTaskStartIsProjectStart = newTaskStartIsProjectStart;
}

/**
* Retrieve the week start day.
*
* @return week start day
*/
Day ProjectHeader::getWeekStartDay()
{
	return (m_weekStartDay);
}

/**
* Set the week start day.
*
* @param weekStartDay week start day
*/
void ProjectHeader::setWeekStartDay(Day weekStartDay)
{
	m_weekStartDay = weekStartDay;
}

/**
* Sets the calculate multiple critical paths flag.
*
* @param flag bool flag
*/
void ProjectHeader::setCalculateMultipleCriticalPaths(bool flag)
{
	m_calculateMultipleCriticalPaths = flag;
}

/**
* Retrieves the calculate multiple critical paths flag.
*
* @return bool flag
*/
bool ProjectHeader::getCalculateMultipleCriticalPaths()
{
	return (m_calculateMultipleCriticalPaths);
}

/**
* Retrieve the currency code for this project.
*
* @return currency code
*/
CString ProjectHeader::getCurrencyCode()
{
	return (m_currencyCode);
}

/**
* Set the currency code for this project.
*
* @param currencyCode currency code
*/
void ProjectHeader::setCurrencyCode(CString currencyCode)
{
	m_currencyCode = currencyCode;
}

///**
//* Sets a map of custom document properties.
//*
//* @param customProperties The Document Summary Information Map
//*/
//void ProjectHeader::setCustomProperties(Map<CString, CString>& customProperties)
//{
//	m_customProperties = customProperties;
//}

/**
* Retrieve a map of custom document properties.
*
* @return the Document Summary Information Map
*/
const std::map<CString, CString>& ProjectHeader::getCustomProperties()
{
	return m_customProperties;
}

/**
* Sets the hyperlink base for this Project.
*
* @param hyperlinkBase Hyperlink base
*/
void ProjectHeader::setHyperlinkBase(CString hyperlinkBase)
{
	m_hyperlinkBase = hyperlinkBase;
}

/**
* Gets the hyperlink base for this Project. If any.
*
* @return Hyperlink base
*/
CString ProjectHeader::getHyperlinkBase()
{
	return (m_hyperlinkBase);
}

/**
* Retrieves the "show project summary task" flag.
*
* @return bool flag
*/
bool ProjectHeader::getShowProjectSummaryTask()
{
	return m_showProjectSummaryTask;
}

/**
* Sets the "show project summary task" flag.
*
* @param value bool flag
*/
void ProjectHeader::setShowProjectSummaryTask(bool value)
{
	m_showProjectSummaryTask = value;
}

/**
* Retrieve the Date for a specific baseline. Note that index 0 represents "Baseline",
* index 1 represents "Baseline1" and so on.
*
* @param index baseline index
* @return baseline Date, NULL if baseline not set
*/
DatePtr ProjectHeader::getBaselineDate(int index)
{
	return m_baselineDate[index];
}

/**
* Set the Date for a specific baseline. Note that index 0 represents "Baseline",
* index 1 represents "Baseline1" and so on.
*
* @param index baseline index
* @param Date baseline Date, NULL if baseline not set
*/
void ProjectHeader::setBaselineDate(int index, DatePtr Date)
{
	m_baselineDate[index] = Date;
}

/**
* Retrieve the template property.
*
* @return template property
*/
CString ProjectHeader::getTemplate()
{
	return m_template;
}

/**
* Set the template property.
*
* @param template property value
*/
void ProjectHeader::setTemplate(CString value)
{
	m_template = value;
}

/**
* Retrieve the project user property.
*
* @return project user property
*/
CString ProjectHeader::getLastAuthor()
{
	return m_lastAuthor;
}

/**
* Set the project user property.
*
* @param projectUser project user property
*/
void ProjectHeader::setLastAuthor(CString projectUser)
{
	m_lastAuthor = projectUser;
}

/**
* Retrieve the last printed property.
*
* @return last printed property
*/
DatePtr ProjectHeader::getLastPrinted()
{
	return m_lastPrinted;
}

/**
* Set the last printed property.
*
* @param lastPrinted property value
*/
void ProjectHeader::setLastPrinted(DatePtr lastPrinted)
{
	m_lastPrinted = lastPrinted;
}

/**
* Retrieve the application property.
*
* @return property value
*/
CString ProjectHeader::getApplicationName()
{
	return m_applicationName;
}

/**
* Set the application property.
*
* @param application property value
*/
void ProjectHeader::setApplicationName(CString application)
{
	m_applicationName = application;
}

/**
* Retrieve the editing time property.
*
* @return property value
*/
int ProjectHeader::getEditingTime()
{
	return m_editingTime;
}

/**
* Set the editing time property.
*
* @param editingTime editing time property
*/
void ProjectHeader::setEditingTime(int editingTime)
{
	m_editingTime = editingTime;
}

/**
* Retrieve the format property.
*
* @return property value
*/
CString ProjectHeader::getPresentationFormat()
{
	return m_presentationFormat;
}

/**
* Set the format property.
*
* @param format property value
*/
void ProjectHeader::setPresentationFormat(CString format)
{
	m_presentationFormat = format;
}

/**
* Retrieve the content type property.
*
* @return content type property
*/
CString ProjectHeader::getContentType()
{
	return m_contentType;
}

/**
* Set the content type property.
*
* @param contentType property value
*/
void ProjectHeader::setContentType(CString contentType)
{
	m_contentType = contentType;
}

/**
* Retrieve the content status property.
*
* @return property value
*/
CString ProjectHeader::getContentStatus()
{
	return m_contentStatus;
}

/**
* Set the content status property.
*
* @param contentStatus property value
*/
void ProjectHeader::setContentStatus(CString contentStatus)
{
	m_contentStatus = contentStatus;
}

/**
* Retrieve the language property.
*
* @return property value
*/
CString ProjectHeader::getLanguage()
{
	return m_language;
}

/**
* Set the language property.
*
* @param language property value
*/
void ProjectHeader::setLanguage(CString language)
{
	m_language = language;
}

/**
* Retrieve the document version property.
*
* @return property value
*/
CString ProjectHeader::getDocumentVersion()
{
	return m_documentVersion;
}

/**
* Set the document version property.
*
* @param documentVersion property value
*/
void ProjectHeader::setDocumentVersion(CString documentVersion)
{
	m_documentVersion = documentVersion;
}
