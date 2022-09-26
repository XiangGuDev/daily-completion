#pragma once

//package net.sf.mpxj;
//
//import java.util.Calendar;
//import java.util.Date;
//import java.util.Map;

#include "MpxjEnum.h"
#include "ProjectEntity.h"
#include "Rate.h"
#include "Duration.h"

class ProjectFile;

/**
 * This class represents the ProjectHeader record as found in an MPX
 * file-> This record contains details of global settings relevant to the
 * project plan. Note that a number of the fields in this record are
 * calculated by Microsoft Project, and will therefore be ignored on import.
 */
class COMMONCONTROLS_API ProjectHeader : ProjectEntity
{
public:
   /**
    * Default constructor.
    *
    * @param file the parent file to which this record belongs.
    */
	ProjectHeader(ProjectFile* file);

   /**
    * Gets Default DurationPtr units. The constants used to define the
    * duration units are defined by the <code>TimeUnit</code> class.
    *
    * @return int constant
    * @see TimeUnit
    */
	TimeUnit getDefaultDurationUnits();

   /**
    * Default duration units. The constants used to define the
    * duration units are defined by the <code>TimeUnit</code> class.
    *
    * @param units default time units
    * @see TimeUnit
    */
	void setDefaultDurationUnits(TimeUnit units);

   /**
    * Retrieves a flag indicating if the default duration type is fixed.
    *
    * @return bool flag
    */
	bool getDefaultDurationIsFixed();

   /**
    * Sets a flag indicating if the default duration type is fixed.
    *
    * @param fixed bool flag
    */
	void setDefaultDurationIsFixed(bool fixed);

   /**
    * Default work units. The constants used to define the
    * work units are defined by the <code>TimeUnit</code> class.
    *
    * @return int representing default
    * @see TimeUnit
    */
	TimeUnit getDefaultWorkUnits();

   /**
    * Default work units. The constants used to define the
    * work units are defined by the <code>TimeUnit</code> class.
    *
    * @param units  int representing default
    * @see TimeUnit
    */
	void setDefaultWorkUnits(TimeUnit units);

   /**
    * Retrieves the default standard rate.
    *
    * @return default standard rate
    */
	RatePtr getDefaultStandardRate();

   /**
    * Sets the default standard rate.
    *
    * @param rate default standard rate
    */
	void setDefaultStandardRate(RatePtr rate);

   /**
    * Get overtime rate.
    *
    * @return rate
    */
	RatePtr getDefaultOvertimeRate();

   /**
    * Set default overtime rate.
    *
    * @param rate default overtime rate
    */
	void setDefaultOvertimeRate(RatePtr rate);

   /**
    * Flags whether updating Task status also upDates resource status.
    *
    * @return bool flag
    */
	bool getUpdatingTaskStatusUpdatesResource2Status();

   /**
    * Flags whether updating Task status also upDates resource status.
    *
    * @param flag bool flag
    */
	void setUpdatingTaskStatusUpdatesResource2Status(bool flag);

   /**
    * Flag representing whether or not to split in-progress tasks.
    *
    * @return bool value
    */
	bool getSplitInProgressTasks();

   /**
    * Flag representing whether or not to split in-progress tasks.
    *
    * @param flag bool value
    */
	void setSplitInProgressTasks(bool flag);

   /**
    * Gets constant representing set Date order eg DMY, MDY.
    *
    * @return constant value for Date order
    */
	DateOrder getDateOrder();

   /**
    * Sets constant representing set Date order eg DMY, MDY.
    *
    * @param DateOrder Date order value
    */
	void setDateOrder(DateOrder DateOrder);

   /**
    * Gets constant representing the Time Format.
    *
    * @return time format constant
    */
	ProjectTimeFormat getTimeFormat();

   /**
    * Sets constant representing the time format.
    *
    * @param timeFormat constant value
    */
	void setTimeFormat(ProjectTimeFormat timeFormat);

   /**
    * This internal method is used to convert from an integer representing
    * minutes past midnight into a Date instance whose time component
    * represents the start time.
    *
    * @param time integer representing the start time in minutes past midnight
    */
	void setIntegerDefaultStartTime(int time);

   /**
    * Retrieve the default start time, specified using the Java Date type.
    * Note that this assumes that the value returned from
    * the getTime method starts at zero... i.e. the Date part
    * of the Date/time value has not been set.
    *
    * @return default start time
    */
	DatePtr getDefaultStartTime();

   /**
    * Set the default start time, specified using the Java Date type.
    * Note that this assumes that the value returned from
    * the getTime method starts at zero... i.e. the Date part
    * of the Date/time value has not been set.
    *
    * @param defaultStartTime default time
    */
	void setDefaultStartTime(DatePtr defaultStartTime);

   /**
    * Gets the Date separator.
    *
    * @return Date separator as set.
    */
	char getDateSeparator();

   /**
    * Sets the Date separator.
    *
    * @param DateSeparator Date separator as set.
    */
	void setDateSeparator(char DateSeparator);

   /**
    * Gets the time separator.
    *
    * @return time separator as set.
    */
	char getTimeSeparator();

   /**
    * Sets the time separator.
    *
    * @param timeSeparator time separator
    */
	void setTimeSeparator(char timeSeparator);

 //  /**
 //   * Sets the time separator.
 //   *
 //   * @param timeSeparator time separator
 //   */
	//void setTimeSeparator(Character timeSeparator);

   /**
    * Gets the AM text.
    *
    * @return AM Text as set.
    */
	CString getAMText();

   /**
    * Sets the AM text.
    *
    * @param amText AM Text as set.
    */
	void setAMText(CString amText);

   /**
    * Gets the PM text.
    *
    * @return PM Text as set.
    */
	CString getPMText();

   /**
    * Sets the PM text.
    *
    * @param pmText PM Text as set.
    */
	void setPMText(CString pmText);

   /**
    * Gets the set Date Format.
    *
    * @return int representing Date Format
    */
	ProjectDateFormat getDateFormat();

   /**
    * Sets the set Date Format.
    *
    * @param DateFormat int representing Date Format
    */
	void setDateFormat(ProjectDateFormat DateFormat);

   /**
    * Gets Bar Text Date Format.
    *
    * @return int value
    */
	ProjectDateFormat getBarTextDateFormat();

   /**
    * Sets Bar Text Date Format.
    *
    * @param DateFormat value to be set
    */
	void setBarTextDateFormat(ProjectDateFormat DateFormat);

   /**
    * Retrieves the default end time.
    *
    * @return End time
    */
	DatePtr getDefaultEndTime();

   /**
    * Sets the default end time.
    *
    * @param Date End time
    */
	void setDefaultEndTime(DatePtr Date);

   /**
    * Sets the project title.
    *
    * @param projectTitle project title
    */
	void setProjectTitle(CString projectTitle);

   /**
    * Gets the project title.
    *
    * @return project title
    */
	CString getProjectTitle();

   /**
    * Sets the company name.
    *
    * @param company company name
    */
	void setCompany(CString company);

   /**
    * Retrieves the company name.
    *
    * @return company name
    */
	CString getCompany();

   /**
    * Sets the manager name.
    *
    * @param manager manager name
    */
	void setManager(CString manager);

   /**
    * Retrieves the manager name.
    *
    * @return manager name
    */
	CString getManager();

   /**
    * Sets the CalendarPtr used. 'Standard' if no value is set.
    *
    * @param calendarName CalendarPtr name
    */
	void setCalendarName(CString calendarName);

   /**
    * Gets the CalendarPtr used. 'Standard' if no value is set.
    *
    * @return CalendarPtr name
    */
	CString getCalendarName();

   /**
    * Sets the project start Date.
    *
    * @param startDate project start Date
    */
	void setStartDate(DatePtr startDate);

   /**
    * Retrieves the project start Date. If an explicit start Date has not been
    * set, this method calculates the start Date by looking for
    * the earliest task start Date.
    *
    * @return project start Date
    */
	DatePtr getStartDate();

   /**
    * Retrieves the project finish Date. If an explicit finish Date has not been
    * set, this method calculates the finish Date by looking for
    * the latest task finish Date.
    *
    * @return Finish Date
    */
	DatePtr getFinishDate();

   /**
    * Sets the project finish Date.
    *
    * @param finishDate project finish Date
    */
	void setFinishDate(DatePtr finishDate);

   /**
    * Retrieves an enumerated value indicating if tasks in this project are
    * scheduled from a start or a finish Date.
    *
    * @return schedule from flag
    */
	ScheduleFrom getScheduleFrom();

   /**
    * Sets an enumerated value indicating if tasks in this project are
    * scheduled from a start or a finish Date.
    *
    * @param scheduleFrom schedule from value
    */
	void setScheduleFrom(ScheduleFrom scheduleFrom);

   /**
    * Retrieves the current Date.
    *
    * @return current Date
    */
	DatePtr getCurrentDate();

   /**
    * Sets the current Date.
    *
    * @param currentDate current Date
    */
	void setCurrentDate(DatePtr currentDate);

   /**
    * Returns any comments.
    *
    * @return comments attached to the Project Header
    */
	CString getComments();

   /**
    * Set comment text.
    *
    * @param comments comment text
    */
	void setComments(CString comments);

   /**
    * Retrieves the project cost.
    *
    * @return project cost
    */
	NumberPtr getCost();

   /**
    * Sets the project cost.
    *
    * @param cost project cost
    */
	void setCost(NumberPtr cost);

   /**
    * Sets the baseline project cost.
    *
    * @param baselineCost baseline project cost
    */
	void setBaselineCost(NumberPtr baselineCost);

   /**
    * Retrieves the baseline project cost.
    *
    * @return baseline project cost
    */
	NumberPtr getBaselineCost();

   /**
    * Sets the actual project cost.
    *
    * @param actualCost actual project cost
    */
	void setActualCost(NumberPtr actualCost);

   /**
    * Retrieves the actual project cost.
    *
    * @return actual project cost
    */
	NumberPtr getActualCost();

   /**
    * Sets the project work duration.
    *
    * @param work project work duration
    */
	void setWork(DurationPtr work);

   /**
    * Retrieves the project work duration.
    *
    * @return project work duration
    */
	DurationPtr getWork();

   /**
    * Set the baseline project work duration.
    *
    * @param baselineWork baseline project work duration
    */
	void setBaselineWork(DurationPtr baselineWork);

   /**
    * Retrieves the baseline project work duration.
    *
    * @return baseline project work duration
    */
	DurationPtr getBaselineWork();

   /**
    * Sets the actual project work duration.
    *
    * @param actualWork actual project work duration
    */
	void setActualWork(DurationPtr actualWork);

   /**
    * Retrieves the actual project work duration.
    *
    * @return actual project work duration
    */
	DurationPtr getActualWork();

   /**
    * Retrieves the project's "Work 2" attribute.
    *
    * @return Work 2 attribute
    */
	NumberPtr getWork2();

   /**
    * Sets the project's "Work 2" attribute.
    *
    * @param work2 work2 percentage value
    */
	void setWork2(NumberPtr work2);

   /**
    * Retrieves the project duration.
    *
    * @return project duration
    */
	DurationPtr getDuration();

   /**
    * Sets the project duration.
    *
    * @param duration project duration
    */
	void setDuration(DurationPtr duration);

   /**
    * Retrieves the baseline duration value.
    *
    * @return baseline project duration value
    */
	DurationPtr getBaselineDuration();

   /**
    * Sets the baseline project duration value.
    *
    * @param baselineDuration baseline project duration
    */
	void setBaselineDuration(DurationPtr baselineDuration);

   /**
    * Retrieves the actual project duration.
    *
    * @return actual project duration
    */
	DurationPtr getActualDuration();

   /**
    * Sets the actual project duration.
    *
    * @param actualDuration actual project duration
    */
	void setActualDuration(DurationPtr actualDuration);

   /**
    * Retrieves the project percentage complete.
    *
    * @return percentage value
    */
	NumberPtr getPercentageComplete();

   /**
    * Sets project percentage complete.
    *
    * @param percentComplete project percent complete
    */
	void setPercentageComplete(NumberPtr percentComplete);

   /**
    * Sets the baseline project start Date.
    *
    * @param baselineStartDate baseline project start Date
    */
	void setBaselineStart(DatePtr baselineStartDate);

   /**
    * Retrieves the baseline project start Date.
    *
    * @return baseline project start Date
    */
	DatePtr getBaselineStart();

   /**
    * Sets the baseline project finish Date.
    *
    * @param baselineFinishDate baseline project finish Date
    */
	void setBaselineFinish(DatePtr baselineFinishDate);

   /**
    * Retrieves the baseline project finish Date.
    *
    * @return baseline project finish Date
    */
	DatePtr getBaselineFinish();

   /**
    * Sets the actual project start Date.
    *
    * @param actualStartDate actual project start Date
    */
	void setActualStart(DatePtr actualStartDate);

   /**
    * Retrieves the actual project start Date.
    *
    * @return actual project start Date
    */
	DatePtr getActualStart();

   /**
    * Sets the actual project finish Date.
    *
    * @param actualFinishDate actual project finish Date
    */
	void setActualFinish(DatePtr actualFinishDate);

   /**
    * Retrieves the actual project finish Date.
    *
    * @return actual project finish Date
    */
	DatePtr getActualFinish();

   /**
    * Retrieves the start variance duration.
    *
    * @return start Date variance
    */
	DurationPtr getStartVariance();

   /**
    * Sets the start variance duration.
    *
    * @param startVariance the start Date variance
    */
	void setStartVariance(DurationPtr startVariance);

   /**
    * Retrieves the project finish variance duration.
    *
    * @return project finish variance duration
    */
	DurationPtr getFinishVariance();

   /**
    * Sets the project finish variance duration.
    *
    * @param finishVariance project finish variance duration
    */
	void setFinishVariance(DurationPtr finishVariance);

   /**
    * Returns the project subject text.
    *
    * @return subject text
    */
	CString getSubject();

   /**
    * Sets the project subject text.
    *
    * @param subject subject text
    */
	void setSubject(CString subject);

   /**
    * Retrieves the project author text.
    *
    * @return author text
    */
	CString getAuthor();

   /**
    * Sets the project author text.
    *
    * @param author project author text
    */
	void setAuthor(CString author);

   /**
    * Retrieves the project keyword text.
    *
    * @return project keyword text
    */
	CString getKeywords();

   /**
    * Sets the project keyword text.
    *
    * @param keywords project keyword text
    */
	void setKeywords(CString keywords);

   /**
    * Sets currency symbol.
    *
    * @param symbol currency symbol
    */
	void setCurrencySymbol(CString symbol);

   /**
    * Retrieves the currency symbol.
    *
    * @return currency symbol
    */
	CString getCurrencySymbol();

   /**
    * Sets the position of the currency symbol.
    *
    * @param posn currency symbol position.
    */
	void setSymbolPosition(CurrencySymbolPosition posn);

   /**
    * Retrieves a constant representing the position of the currency symbol.
    *
    * @return position
    */
	CurrencySymbolPosition getSymbolPosition();

   /**
    * Sets no of currency digits.
    *
    * @param currDigs Available values, 0,1,2
    */
	void setCurrencyDigits(NumberPtr currDigs);

   /**
    * Gets no of currency digits.
    *
    * @return Available values, 0,1,2
    */
	NumberPtr getCurrencyDigits();

   /**
    * Sets the thousands separator.
    * Note that this separator defines the thousands separator for all decimal
    * numbers that appear in the MPX file->
    *
    * @param sep character
    */
	void setThousandsSeparator(char sep);

   /**
    * Gets the thousands separator.
    * Note that this separator defines the thousands separator for all decimal
    * numbers that appear in the MPX file->
    *
    * @return character
    */
	char getThousandsSeparator();

   /**
    * Sets the decimal separator.
    * Note that this separator defines the decimal separator for all decimal
    * numbers that appear in the MPX file->
    *
    * @param decSep character
    */
	void setDecimalSeparator(char decSep);

   /**
    * Gets the decimal separator.
    * Note that this separator defines the decimal separator for all decimal
    * numbers that appear in the MPX file->
    *
    * @return character
    */
	char getDecimalSeparator();

   /**
    * Retrieve the externally edited flag.
    *
    * @return externally edited flag
    */
	bool getProjectExternallyEdited();

   /**
    * Set the externally edited flag.
    *
    * @param projectExternallyEdited externally edited flag
    */
	void setProjectExternallyEdited(bool projectExternallyEdited);

   /**
    * Retrieves the category text.
    *
    * @return category text
    */
	CString getCategory();

   /**
    * Sets the category text.
    *
    * @param category category text
    */
	void setCategory(CString category);

   /**
    * Retrieve the number of days per month.
    *
    * @return days per month
    */
	NumberPtr getDaysPerMonth();

   /**
    * Set the number of days per month.
    *
    * @param daysPerMonth days per month
    */
	void setDaysPerMonth(NumberPtr daysPerMonth);

   /**
    * Retrieve the number of minutes per day.
    *
    * @return minutes per day
    */
	NumberPtr getMinutesPerDay();

   /**
    * Set the number of minutes per day.
    *
    * @param minutesPerDay minutes per day
    */
	void setMinutesPerDay(NumberPtr minutesPerDay);

   /**
    * Retrieve the number of minutes per week.
    *
    * @return minutes per week
    */
	NumberPtr getMinutesPerWeek();

   /**
    * Set the number of minutes per week.
    *
    * @param minutesPerWeek minutes per week
    */
	void setMinutesPerWeek(NumberPtr minutesPerWeek);

   /**
    * Retrieve the fiscal year start flag.
    *
    * @return fiscal year start flag
    */
	bool getFiscalYearStart();

   /**
    * Set the fiscal year start flag.
    *
    * @param fiscalYearStart fiscal year start
    */
	void setFiscalYearStart(bool fiscalYearStart);

   /**
    * Retrieves the default task earned value method.
    *
    * @return default task earned value method
    */
	EarnedValueMethod getDefaultTaskEarnedValueMethod();

   /**
    * Sets the default task earned value method.
    *
    * @param defaultTaskEarnedValueMethod default task earned value method
    */
	void setDefaultTaskEarnedValueMethod(EarnedValueMethod defaultTaskEarnedValueMethod);

   /**
    * Retrieve the remove file properties flag.
    *
    * @return remove file properties flag
    */
	bool getRemoveFileProperties();

   /**
    * Set the remove file properties flag.
    *
    * @param removeFileProperties remove file properties flag
    */
	void setRemoveFileProperties(bool removeFileProperties);

   /**
    * Retrieve the move completed ends back flag.
    *
    * @return move completed ends back flag
    */
	bool getMoveCompletedEndsBack();

   /**
    * Set the move completed ends back flag.
    *
    * @param moveCompletedEndsBack move completed ends back flag
    */
	void setMoveCompletedEndsBack(bool moveCompletedEndsBack);

   /**
    * Retrieve the new tasks estimated flag.
    *
    * @return new tasks estimated flag
    */
	bool getNewTasksEstimated();

   /**
    * Set the new tasks estimated flag.
    *
    * @param newTasksEstimated new tasks estimated flag
    */
	void setNewTasksEstimated(bool newTasksEstimated);

   /**
    * Retrieve the spread actual cost flag.
    *
    * @return spread actual cost flag
    */
	bool getSpreadActualCost();

   /**
    * Set the spread actual cost flag.
    *
    * @param spreadActualCost spread actual cost flag
    */
	void setSpreadActualCost(bool spreadActualCost);

   /**
    * Retrieve the multiple critical paths flag.
    *
    * @return multiple critical paths flag
    */
	bool getMultipleCriticalPaths();

   /**
    * Set the multiple critical paths flag.
    *
    * @param multipleCriticalPaths multiple critical paths flag
    */
	void setMultipleCriticalPaths(bool multipleCriticalPaths);

   /**
    * Retrieve the auto add new resources and tasks flag.
    *
    * @return auto add new resources and tasks flag
    */
	bool getAutoAddNewResource2sAndTasks();

   /**
    * Set the auto add new resources and tasks flag.
    *
    * @param autoAddNewResource2sAndTasks auto add new resources and tasks flag
    */
	void setAutoAddNewResource2sAndTasks(bool autoAddNewResource2sAndTasks);

   /**
    * Retrieve the last saved Date.
    *
    * @return last saved Date
    */
	DatePtr getLastSaved();

   /**
    * Set the last saved Date.
    *
    * @param lastSaved last saved Date
    */
	void setLastSaved(DatePtr lastSaved);

   /**
    * Retrieve the status Date.
    *
    * @return status Date
    */
	DatePtr getStatusDate();

   /**
    * Set the status Date.
    *
    * @param statusDate status Date
    */
	void setStatusDate(DatePtr statusDate);

   /**
    * Retrieves the move remaining starts back flag.
    *
    * @return move remaining starts back flag
    */
	bool getMoveRemainingStartsBack();

   /**
    * Sets the move remaining starts back flag.
    *
    * @param moveRemainingStartsBack remaining starts back flag
    */
	void setMoveRemainingStartsBack(bool moveRemainingStartsBack);

   /**
    * Retrieves the autolink flag.
    *
    * @return autolink flag
    */
	bool getAutolink();

   /**
    * Sets the autolink flag.
    *
    * @param autolink autolink flag
    */
	void setAutolink(bool autolink);

   /**
    * Retrieves the Microsoft Project Server URL flag.
    *
    * @return Microsoft Project Server URL flag
    */
	bool getMicrosoftProjectServerURL();

   /**
    * Sets the Microsoft Project Server URL flag.
    *
    * @param microsoftProjectServerURL Microsoft Project Server URL flag
    */
	void setMicrosoftProjectServerURL(bool microsoftProjectServerURL);

   /**
    * Retrieves the honor constraints flag.
    *
    * @return honor constraints flag
    */
	bool getHonorConstraints();

   /**
    * Sets the honor constraints flag.
    *
    * @param honorConstraints honor constraints flag
    */
	void setHonorConstraints(bool honorConstraints);

   /**
    * Retrieve the admin project flag.
    *
    * @return admin project flag
    */
	bool getAdminProject();

   /**
    * Set the admin project flag.
    *
    * @param adminProject admin project flag
    */
	void setAdminProject(bool adminProject);

   /**
    * Retrieves the inserted projects like summary flag.
    *
    * @return inserted projects like summary flag
    */
	bool getInsertedProjectsLikeSummary();

   /**
    * Sets the inserted projects like summary flag.
    *
    * @param insertedProjectsLikeSummary inserted projects like summary flag
    */
	void setInsertedProjectsLikeSummary(bool insertedProjectsLikeSummary);

   /**
    * Retrieves the project name.
    *
    * @return project name
    */
	CString getName();

   /**
    * Sets the project name.
    *
    * @param name project name
    */
	void setName(CString name);

   /**
    * Retrieves the spread percent complete flag.
    *
    * @return spread percent complete flag
    */
	bool getSpreadPercentComplete();

   /**
    * Sets the spread percent complete flag.
    *
    * @param spreadPercentComplete spread percent complete flag
    */
	void setSpreadPercentComplete(bool spreadPercentComplete);

   /**
    * Retrieve the move completed ends forward flag.
    *
    * @return move completed ends forward flag
    */
	bool getMoveCompletedEndsForward();

   /**
    * Sets the move completed ends forward flag.
    *
    * @param moveCompletedEndsForward move completed ends forward flag
    */
	void setMoveCompletedEndsForward(bool moveCompletedEndsForward);

   /**
    * Retrieve the editable actual costs flag.
    *
    * @return editable actual costs flag
    */
	bool getEditableActualCosts();

   /**
    * Set the editable actual costs flag.
    *
    * @param editableActualCosts editable actual costs flag
    */
	void setEditableActualCosts(bool editableActualCosts);

   /**
    * Retrieve the unique ID for this project.
    *
    * @return unique ID
    */
	CString getUniqueID();

   /**
    * Set the unique ID for this project.
    *
    * @param uniqueID unique ID
    */
	void setUniqueID(CString uniqueID);

   /**
    * Retrieve the project revision number.
    *
    * @return revision number
    */
	int getRevision();

   /**
    * Retrieve the new tasks effort driven flag.
    *
    * @return new tasks effort driven flag
    */
	bool getNewTasksEffortDriven();

   /**
    * Sets the new tasks effort driven flag.
    *
    * @param newTasksEffortDriven new tasks effort driven flag
    */
	void setNewTasksEffortDriven(bool newTasksEffortDriven);

   /**
    * Set the project revision number.
    *
    * @param revision revision number
    */
	void setRevision(int revision);

   /**
    * Retrieve the move remaining starts forward flag.
    *
    * @return move remaining starts forward flag
    */
	bool getMoveRemainingStartsForward();

   /**
    * Set the move remaining starts forward flag.
    *
    * @param moveRemainingStartsForward move remaining starts forward flag
    */
	void setMoveRemainingStartsForward(bool moveRemainingStartsForward);

   /**
    * Retrieve the actuals in sync flag.
    *
    * @return actuals in sync flag
    */
	bool getActualsInSync();

   /**
    * Set the actuals in sync flag.
    *
    * @param actualsInSync actuals in sync flag
    */
	void setActualsInSync(bool actualsInSync);

   /**
    * Retrieve the default task type.
    *
    * @return default task type
    */
	TaskType getDefaultTaskType();

   /**
    * Set the default task type.
    *
    * @param defaultTaskType default task type
    */
	void setDefaultTaskType(TaskType defaultTaskType);

   /**
    * Retrieve the earned value method.
    *
    * @return earned value method
    */
	EarnedValueMethod getEarnedValueMethod();

   /**
    * Set the earned value method.
    *
    * @param earnedValueMethod earned value method
    */
	void setEarnedValueMethod(EarnedValueMethod earnedValueMethod);

   /**
    * Retrieve the project creation Date.
    *
    * @return project creation Date
    */
	DatePtr getCreationDate();

   /**
    * Set the project creation Date.
    *
    * @param creationDate project creation Date
    */
	void setCreationDate(DatePtr creationDate);

   /**
    * Retrieve the extended creation Date.
    *
    * @return extended creation Date
    */
	DatePtr getExtendedCreationDate();

   /**
    * Retrieve the default fixed cost accrual type.
    *
    * @return default fixed cost accrual type
    */
	AccrueType getDefaultFixedCostAccrual();

   /**
    * Sets the default fixed cost accrual type.
    *
    * @param defaultFixedCostAccrual default fixed cost accrual type
    */
	void setDefaultFixedCostAccrual(AccrueType defaultFixedCostAccrual);

   /**
    * Set the extended creation Date.
    *
    * @param creationDate extended creation Date
    */
	void setExtendedCreationDate(DatePtr creationDate);

   /**
    * Retrieve the critical slack limit.
    *
    * @return critical slack limit
    */
	int getCriticalSlackLimit();

   /**
    * Set the critical slack limit.
    *
    * @param criticalSlackLimit critical slack limit
    */
	void setCriticalSlackLimit(int criticalSlackLimit);

   /**
    * Retrieve the number of the baseline to use for earned value
    * calculations.
    *
    * @return baseline for earned value
    */
	int getBaselineForEarnedValue();

   /**
    * Set the number of the baseline to use for earned value
    * calculations.
    *
    * @param baselineForEarnedValue baseline for earned value
    */
	void setBaselineForEarnedValue(int baselineForEarnedValue);

   /**
    * Retrieves the fiscal year start month (January=1, December=12).
    *
    * @return fiscal year start month
    */
	int getFiscalYearStartMonth();

   /**
    * Sets the fiscal year start month (January=1, December=12).
    *
    * @param fiscalYearStartMonth fiscal year start month
    */
	void setFiscalYearStartMonth(int fiscalYearStartMonth);

   /**
    * Retrieve the flag indicating if new tasks should default to the
    * project start Date (true) or the current Date (false).
    *
    * @return new task start is project start
    */
	bool getNewTaskStartIsProjectStart();

   /**
    * Sets the flag indicating if new tasks should default to the
    * project start Date (true) or the current Date (false).
    *
    * @param newTaskStartIsProjectStart new task start is project start
    */
	void setNewTaskStartIsProjectStart(bool newTaskStartIsProjectStart);

   /**
    * Retrieve the week start day.
    *
    * @return week start day
    */
	Day getWeekStartDay();

   /**
    * Set the week start day.
    *
    * @param weekStartDay week start day
    */
	void setWeekStartDay(Day weekStartDay);

   /**
    * Sets the calculate multiple critical paths flag.
    *
    * @param flag bool flag
    */
	void setCalculateMultipleCriticalPaths(bool flag);

   /**
    * Retrieves the calculate multiple critical paths flag.
    *
    * @return bool flag
    */
	bool getCalculateMultipleCriticalPaths();

   /**
    * Retrieve the currency code for this project.
    * 
    * @return currency code
    */
	CString getCurrencyCode();

   /**
    * Set the currency code for this project.
    * 
    * @param currencyCode currency code
    */
	void setCurrencyCode(CString currencyCode);

 //  /**
 //   * Sets a map of custom document properties.
 //   *
 //   * @param customProperties The Document Summary Information Map
 //   */
	//void setCustomProperties(Map<CString, CString>& customProperties);

   /**
    * Retrieve a map of custom document properties.
    *
    * @return the Document Summary Information Map
    */
	const std::map<CString, CString>& getCustomProperties();

   /**
    * Sets the hyperlink base for this Project.
    *
    * @param hyperlinkBase Hyperlink base
    */
	void setHyperlinkBase(CString hyperlinkBase);

   /**
    * Gets the hyperlink base for this Project. If any.
    *
    * @return Hyperlink base
    */
	CString getHyperlinkBase();

   /**
    * Retrieves the "show project summary task" flag.
    * 
    * @return bool flag
    */
	bool getShowProjectSummaryTask();

   /**
    * Sets the "show project summary task" flag.
    * 
    * @param value bool flag
    */
	void setShowProjectSummaryTask(bool value);

   /**
    * Retrieve the Date for a specific baseline. Note that index 0 represents "Baseline",
    * index 1 represents "Baseline1" and so on.
    * 
    * @param index baseline index
    * @return baseline Date, NULL if baseline not set
    */
	DatePtr getBaselineDate(int index);

   /**
    * Set the Date for a specific baseline. Note that index 0 represents "Baseline",
    * index 1 represents "Baseline1" and so on.
    * 
    * @param index baseline index
    * @param Date baseline Date, NULL if baseline not set
    */
	void setBaselineDate(int index, DatePtr Date);

   /**
    * Retrieve the template property.
    * 
    * @return template property
    */
	CString getTemplate();

   /**
    * Set the template property.
    * 
    * @param template property value
    */
	void setTemplate(CString value);

   /**
    * Retrieve the project user property.
    * 
    * @return project user property
    */
	CString getLastAuthor();

   /**
    * Set the project user property.
    * 
    * @param projectUser project user property
    */
	void setLastAuthor(CString projectUser);

   /**
    * Retrieve the last printed property.
    * 
    * @return last printed property
    */
	DatePtr getLastPrinted();

   /**
    * Set the last printed property.
    * 
    * @param lastPrinted property value
    */
	void setLastPrinted(DatePtr lastPrinted);

   /**
    * Retrieve the application property.
    * 
    * @return property value
    */
	CString getApplicationName();

   /**
    * Set the application property.
    * 
    * @param application property value
    */
	void setApplicationName(CString application);

   /**
    * Retrieve the editing time property.
    * 
    * @return property value
    */
	int getEditingTime();

   /**
    * Set the editing time property.
    * 
    * @param editingTime editing time property
    */
	void setEditingTime(int editingTime);

   /**
    * Retrieve the format property.
    * 
    * @return property value
    */
	CString getPresentationFormat();

   /**
    * Set the format property.
    * 
    * @param format property value
    */
	void setPresentationFormat(CString format);

   /**
    * Retrieve the content type property.
    * 
    * @return content type property
    */
	CString getContentType();

   /**
    * Set the content type property.
    * 
    * @param contentType property value
    */
	void setContentType(CString contentType);

   /**
    * Retrieve the content status property.
    * 
    * @return property value
    */
	CString getContentStatus();

   /**
    * Set the content status property.
    * 
    * @param contentStatus property value
    */
	void setContentStatus(CString contentStatus);

   /**
    * Retrieve the language property.
    * 
    * @return property value
    */
	CString getLanguage();

   /**
    * Set the language property.
    * 
    * @param language property value
    */
	void setLanguage(CString language);

   /**
    * Retrieve the document version property.
    * 
    * @return property value
    */
	CString getDocumentVersion();

   /**
    * Set the document version property.
    * 
    * @param documentVersion property value
    */
	void setDocumentVersion(CString documentVersion);

private:
   CString m_currencySymbol;
   CurrencySymbolPosition m_symbolPosition = CurrencySymbolPosition::BEFORE;
   NumberPtr m_currencyDigits;
   char m_thousandsSeparator;
   char m_decimalSeparator;

   /**
    * Default Settings Attributes.
    */
   TimeUnit m_defaultDurationUnits = TimeUnit::DAYS;
   bool m_defaultDurationIsFixed;
   TimeUnit m_defaultWorkUnits;
   RatePtr m_defaultStandardRate;
   RatePtr m_defaultOvertimeRate;
   bool m_updatingTaskStatusUpdatesResource2Status;
   bool m_splitInProgressTasks;

   /**
    * Date Time Settings Attributes.
    */
   DateOrder m_DateOrder = DateOrder::MDY;
   ProjectTimeFormat m_timeFormat = ProjectTimeFormat::TWELVE_HOUR;
   DatePtr m_defaultStartTime;
   char m_DateSeparator;
   char m_timeSeparator;
   CString m_amText;
   CString m_pmText;
   ProjectDateFormat m_DateFormat = ProjectDateFormat::DD_MM_YY;
   ProjectDateFormat m_barTextDateFormat;

   /**
    * Project Header Attributes.
    */
   CString m_projectTitle;
   CString m_company;
   CString m_manager;
   CString m_calendarName;
   DatePtr m_startDate;
   DatePtr m_finishDate;
   ScheduleFrom m_scheduleFrom = ScheduleFrom::SCHEDULE_START;
   DatePtr m_currentDate;
   CString m_comments;
   NumberPtr m_cost;
   NumberPtr m_baselineCost;
   NumberPtr m_actualCost;
   DurationPtr m_work;
   DurationPtr m_baselineWork;
   DurationPtr m_actualWork;
   NumberPtr m_work2;
   DurationPtr m_duration;
   DurationPtr m_baselineDuration;
   DurationPtr m_actualDuration;
   NumberPtr m_percentageComplete;
   DatePtr m_baselineStart;
   DatePtr m_baselineFinish;
   DatePtr m_actualStart;
   DatePtr m_actualFinish;
   DurationPtr m_startVariance;
   DurationPtr m_finishVariance;
   CString m_subject;
   CString m_author;
   CString m_keywords;
   CString m_hyperlinkBase;

   /**
    * The following member variables are extended attributes. They are
    * do not form part of the MPX file format definition, and are neither
    * loaded from an MPX file, or saved to an MPX file-> Their purpose
    * is to provide storage for attributes which are defined by later versions
    * of Microsoft Project. This allows these attributes to be manipulated
    * when they have been retrieved from file formats other than MPX.
    */
   DatePtr m_defaultEndTime;
   bool m_projectExternallyEdited;
   CString m_category;
   NumberPtr m_minutesPerDay;
   NumberPtr m_daysPerMonth;
   NumberPtr m_minutesPerWeek;
   bool m_fiscalYearStart;
   EarnedValueMethod m_defaultTaskEarnedValueMethod;
   bool m_removeFileProperties;
   bool m_moveCompletedEndsBack;
   bool m_newTasksEstimated;
   bool m_spreadActualCost;
   bool m_multipleCriticalPaths;
   bool m_autoAddNewResource2sAndTasks;
   DatePtr m_lastSaved;
   DatePtr m_statusDate;
   bool m_moveRemainingStartsBack;
   bool m_autolink;
   bool m_microsoftProjectServerURL;
   bool m_honorConstraints;
   bool m_adminProject;
   bool m_insertedProjectsLikeSummary;
   CString m_name;
   bool m_spreadPercentComplete;
   bool m_moveCompletedEndsForward;
   bool m_editableActualCosts;
   CString m_uniqueID;
   int m_revision;
   bool m_newTasksEffortDriven;
   bool m_moveRemainingStartsForward;
   bool m_actualsInSync;
   TaskType m_defaultTaskType;
   EarnedValueMethod m_earnedValueMethod;
   DatePtr m_creationDate;
   DatePtr m_extendedCreationDate;
   AccrueType m_defaultFixedCostAccrual;
   int m_criticalSlackLimit;
   int m_baselineForEarnedValue;
   int m_fiscalYearStartMonth;
   bool m_newTaskStartIsProjectStart;
   Day m_weekStartDay;
   bool m_calculateMultipleCriticalPaths;
   std::map<CString, CString> m_customProperties;
   CString m_currencyCode;
   bool m_showProjectSummaryTask;
   DatePtr m_baselineDate[11];
   CString m_template;
   CString m_lastAuthor;
   DatePtr m_lastPrinted;
   CString m_applicationName;
   int m_editingTime;
   CString m_presentationFormat;
   CString m_contentType;
   CString m_contentStatus;
   CString m_language;
   CString m_documentVersion;

   /*
    * Missing MSPDI attributes
    *
       // this is probably the schedule from value, we could remove
       // the ScheduleFrom type, and replace it with a bool
       // we just need to ensure that the MPX read/write works OK
       void setScheduleFromStart(bool value);
    */

   /**
    * Default time separator character.
    */
   const char DEFAULT_TIME_SEPARATOR = ':';

   /**
    * Default cost value.
    */
   const double DEFAULT_COST = 0;

   /**
    * Default critical slack limit.
    */
   const int DEFAULT_CRITICAL_SLACK_LIMIT = 0;

   /**
    * Default baseline for earned value.
    */
   const int DEFAULT_BASELINE_FOR_EARNED_VALUE = 0;

   /**
    * Default fiscal year start month.
    */
   const int DEFAULT_FISCAL_YEAR_START_MONTH = 1;

   /**
    * Default week start day.
    */
   const Day DEFAULT_WEEK_START_DAY = Day::MONDAY;

   /**
    * Default work value.
    */
   DurationPtr DEFAULT_WORK = Duration2::getInstance(0, TimeUnit::HOURS);

   /**
    * Default work 2 value.
    */
   const double DEFAULT_WORK2 = 0;

   /**
    * Default duration value.
    */
   DurationPtr DEFAULT_DURATION = Duration2::getInstance(0, TimeUnit::DAYS);

   /**
    * Default schedule from value.
    */
   const ScheduleFrom DEFAULT_SCHEDULE_FROM = ScheduleFrom::SCHEDULE_START;

   /**
    * Default percent complete value.
    */
   const double DEFAULT_PERCENT_COMPLETE = 0;

   /**
    * Default calendar name.
    */
   const LPCTSTR DEFAULT_CALENDAR_NAME = _T("Standard");

   /**
    * Default minutes per day.
    */
   const int DEFAULT_MINUTES_PER_DAY = 480;

   /**
    * Default days per month.
    */
   const int DEFAULT_DAYS_PER_MONTH = 20;

   /**
    * Default minutes per week.
    */
   const int DEFAULT_MINUTES_PER_WEEK = 2400;
};

typedef std::shared_ptr<ProjectHeader> ProjectHeaderPtr;