#pragma once

#include "Project.h"

/**
 * This object contains factory methods for each
 * Java content interface and Java element interface
 * generated in the net.sf.mpxj.mspdi.schema package.
 * <p>An ObjectFactory allows you to programatically
 * construct new instances of the Java representation
 * for XML content. The Java representation of XML
 * content can consist of schema derived interfaces
 * and classes representing the binding of schema
 * type definitions, element declarations and model
 * groups.  Factory methods for each of these are
 * provided in this class.
 *
 */
class ObjectFactory
{
public:
	/**
	 * Create an instance of {@link mspdi.schema::WorkWeek* }
	 *
	 */
	mspdi::schema::WorkWeek* createProjectCalendarsCalendarWorkWeeksWorkWeek();

	mspdi::schema::WeekDay* createProjectCalendarsCalendarWeekDaysWeekDay();
	mspdi::schema::WeekDay* createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDaysWeekDay();

	mspdi::schema::Task* createProjectTasksTask();
	
	mspdi::schema::ValueList* createProjectOutlineCodesOutlineCodeValues();
	mspdi::schema::ValueList* createProjectExtendedAttributesExtendedAttributeValueList();

	mspdi::schema::TimePeriod* createProjectCalendarsCalendarWeekDaysWeekDayTimePeriod();
	mspdi::schema::TimePeriod* createProjectCalendarsCalendarWorkWeeksWorkWeekTimePeriod();
	mspdi::schema::TimePeriod* createProjectCalendarsCalendarExceptionsExceptionTimePeriod();

	mspdi::schema::Assignments* createProjectAssignments();
	mspdi::schema::AvailabilityPeriod* createProjectResource2sResource2AvailabilityPeriodsAvailabilityPeriod();
	mspdi::schema::WBSMasks* createProjectWBSMasks();
	mspdi::schema::Exceptions* createProjectCalendarsCalendarExceptions();

	mspdi::schema::WorkingTimes* createProjectCalendarsCalendarExceptionsExceptionWorkingTimes();
	mspdi::schema::WorkingTimes* createProjectCalendarsCalendarWeekDaysWeekDayWorkingTimes();
	mspdi::schema::WorkingTimes* createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDaysWeekDayWorkingTimes();

	mspdi::schema::Rates* createProjectResource2sResource2Rates();

	mspdi::schema::WeekDays* createProjectCalendarsCalendarWeekDays();
	mspdi::schema::WeekDays* createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDays();

	mspdi::schema::OutlineCode* createProjectOutlineCodesOutlineCode();
	mspdi::schema::OutlineCode* createProjectTasksTaskOutlineCode();
	mspdi::schema::OutlineCode* createProjectResource2sResource2OutlineCode();

	mspdi::schema::ExtendedAttribute* createProjectExtendedAttributesExtendedAttribute();
	mspdi::schema::ExtendedAttribute* createProjectTasksTaskExtendedAttribute();
	mspdi::schema::ExtendedAttribute* createProjectAssignmentsAssignmentExtendedAttribute();
	mspdi::schema::ExtendedAttribute* createProjectResource2sResource2ExtendedAttribute();

	mspdi::schema::Value* createProjectExtendedAttributesExtendedAttributeValueListValue();
	mspdi::schema::Value* createProjectOutlineCodesOutlineCodeValuesValue();
	
	mspdi::schema::WBSMask* createProjectWBSMasksWBSMask();
	mspdi::schema::TimephasedDataType* createTimephasedDataType();
	mspdi::schema::Exception* createProjectCalendarsCalendarExceptionsException();
	mspdi::schema::Calendar* createProjectCalendarsCalendar();
	mspdi::schema::AvailabilityPeriods* createProjectResource2sResource2AvailabilityPeriods();

	mspdi::schema::WorkingTime* createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDaysWeekDayWorkingTimesWorkingTime();
	mspdi::schema::WorkingTime* createProjectCalendarsCalendarWeekDaysWeekDayWorkingTimesWorkingTime();
	mspdi::schema::WorkingTime* createProjectCalendarsCalendarExceptionsExceptionWorkingTimesWorkingTime();

	mspdi::schema::PredecessorLink* createProjectTasksTaskPredecessorLink();
	mspdi::schema::Calendars* createProjectCalendars();

	mspdi::schema::Baseline* createProjectAssignmentsAssignmentBaseline();
	mspdi::schema::Baseline* createProjectResource2sResource2Baseline();
	mspdi::schema::Baseline* createProjectTasksTaskBaseline();

	mspdi::schema::ExtendedAttributes* createProjectExtendedAttributes();

	mspdi::schema::Masks* createProjectOutlineCodesOutlineCodeMasks();

	mspdi::schema::Project* createProject();
	mspdi::schema::OutlineCodes* createProjectOutlineCodes();
	mspdi::schema::Resource2s* createProjectResource2s();
	mspdi::schema::Tasks* createProjectTasks();

	mspdi::schema::Mask* createProjectOutlineCodesOutlineCodeMasksMask();
	
	mspdi::schema::Assignment* createProjectAssignmentsAssignment();
	mspdi::schema::WorkWeeks* createProjectCalendarsCalendarWorkWeeks();
	mspdi::schema::Resource2* createProjectResource2sResource2();
	mspdi::schema::Rate* createProjectResource2sResource2RatesRate();
};