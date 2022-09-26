#include "stdafx.h"
#include "MSPDIWriter.h"
#include "..\common\AssignmentFieldLists.h"
#include "..\common\TaskFieldLists.h"
#include "..\common\ResourceFieldLists.h"
#include "..\common\DateHelper.h"
#include "..\common\MPPTaskField.h"
#include "..\common\MPPResourceField.h"
#include "..\common\MPPAssignmentField.h"
#include "..\TaskField.h"
#include "..\ResourceField.h"
#include "..\AssignmentField.h"
#include "..\common\NumberHelper.h"
#include "..\common\StringHelper.h"

MSPDIWriter::MSPDIWriter()
	:m_projectFile(NULL),
	m_AssignmentFieldExtAttrs(0), 
	m_TaskFieldExtAttrs(0),
	m_Resource2FieldExtAttrs(0)
{

}

/**
* Sets a flag to control whether timephased assignment data is split 
* into days. The default is true.
* 
* @param flag bool flag
*/
void MSPDIWriter::setSplitTimephasedAsDays(bool flag)
{
    m_splitTimephasedAsDays = flag;
}

/**
* Retrieves a flag to control whether timephased assignment data is split 
* into days. The default is true. 
* 
* @return bool true
*/
bool MSPDIWriter::getSplitTimephasedAsDays()
{
    return m_splitTimephasedAsDays;
}

/**
* Sets a flag to control whether timephased resource assignment data
* is written to the file. The default is false.
* 
* @param value bool flag
*/
void MSPDIWriter::setWriteTimephasedData(bool value)
{
    m_writeTimphasedData = value;
}

/**
* Retrieves the state of the flag which controls whether timephased 
* resource assignment data is written to the file. The default is false.
* 
* @return bool flag
*/
bool MSPDIWriter::getWriteTimephasedData()
{
    return m_writeTimphasedData;
}

/**
* Set the save version to use when generating an MSPDI file.
* 
* @param version save version
*/
void MSPDIWriter::setSaveVersion(SaveVersion version)
{
    m_saveVersion = version;
}

/**
* Retrieve the save version current set.
* 
* @return current save version
*/
SaveVersion MSPDIWriter::getSaveVersion()
{
    return m_saveVersion;
}

void MSPDIWriter::reset()
{
	m_projectFile = NULL;
	m_taskExtendedAttributes.clear();
	m_resourceExtendedAttributes.clear();
	m_assignmentExtendedAttributes.clear();
}

/**
* {@inheritDoc}
*/
void MSPDIWriter::write(ProjectFile* projectFile, BwProj::toolkit::IDataWriteStream* stream)
{
	//projectFile->renumberResource2IDs();
	// ProjectFile 转换成 Project
    m_projectFile = projectFile;
    m_projectFile->validateUniqueIDsForMicrosoftProject();

	m_taskExtendedAttributes.clear();
	m_resourceExtendedAttributes.clear();
	m_assignmentExtendedAttributes.clear();

	std::shared_ptr<mspdi::schema::Project> project (m_factory.createProject());

    writeProjectHeader(project);
    writeCalendars(project);
    writeResource2s(project);
    writeTasks(project);
    writeAssignments(project);
    writeProjectExtendedAttributes(project);

    m_dataConverter.setParentFile(m_projectFile);

	// 输出到Document，再转换成 stream
	TiXmlDocument xmldoc;
	TiXmlDeclaration decl("1.0", "UTF-8", "yes");
	xmldoc.InsertEndChild(decl);
	TiXmlElement xmlroot("Project");
	xmlroot.SetAttribute("xmlns", "http://schemas.microsoft.com/project");
	xmldoc.InsertEndChild(xmlroot);
	project->saveElement(xmldoc.RootElement());

	TiXmlPrinter printer;
	xmldoc.Accept(&printer);
	
	// 进行编码转换
	CString xmlResult = CA2W(printer.CStr());
	CW2A utf8XmlBuffer(xmlResult, CP_UTF8);
	size_t len = strlen((LPCSTR)utf8XmlBuffer);

	stream->Write((LPCSTR)utf8XmlBuffer, len);
}

/**
* This method writes project header data to an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIWriter::writeProjectHeader(std::shared_ptr<mspdi::schema::Project> project)
{
	ProjectHeader* header = m_projectFile->getProjectHeader();

    project->set("ActualsInSync", header->getActualsInSync());
	project->set("AdminProject", header->getAdminProject());
	project->set("Author", header->getAuthor());
	project->set("AutoAddNewResource2sAndTasks", header->getAutoAddNewResource2sAndTasks());
	project->set("Autolink", header->getAutolink());
	project->set("BaselineForEarnedValue", header->getBaselineForEarnedValue());
	project->set("CalendarUID", m_projectFile->getCalendar() == NULL ? 1 : m_projectFile->getCalendar()->getUniqueID());
	project->set("Category", header->getCategory());
	project->set("Company", header->getCompany());
	project->set("CreationDate", m_dataConverter.printDateTime(header->getCreationDate()));
	project->set("CriticalSlackLimit", header->getCriticalSlackLimit());
	project->set("CurrencyCode", header->getCurrencyCode());
	project->set("CurrencyDigits", header->getCurrencyDigits()->intValue());
	project->set("CurrencySymbol", header->getCurrencySymbol());
	project->set("CurrencySymbolPosition", header->getSymbolPosition());
	project->set("CurrentDate", m_dataConverter.printDateTime(header->getCurrentDate()));
	project->set("DaysPerMonth", header->getDaysPerMonth());
	project->set("DefaultFinishTime", m_dataConverter.printTime2T(header->getDefaultEndTime()));
	project->set("DefaultFixedCostAccrual", header->getDefaultFixedCostAccrual());
	project->set("DefaultOvertimeRate", m_dataConverter.printRate(header->getDefaultOvertimeRate()));
	project->set("DefaultStandardRate", m_dataConverter.printRate(header->getDefaultStandardRate()));
	project->set("DefaultStartTime", m_dataConverter.printTime2T(header->getDefaultStartTime()));
	project->set("DefaultTaskEVMethod", m_dataConverter.printEarnedValueMethod(header->getDefaultTaskEarnedValueMethod()));
	project->set("DefaultTaskType", header->getDefaultTaskType());
	project->set("DurationFormat", m_dataConverter.printDurationTimeUnits(header->getDefaultDurationUnits(), false));
	project->set("EarnedValueMethod", m_dataConverter.printEarnedValueMethod(header->getEarnedValueMethod()));
	project->set("EditableActualCosts", header->getEditableActualCosts());
	project->set("ExtendedCreationDate", m_dataConverter.printDateTime(header->getExtendedCreationDate()));
	project->set("FinishDate", m_dataConverter.printDateTime(header->getFinishDate()));
	project->set("FiscalYearStart", header->getFiscalYearStart());
	project->set("FYStartDate", header->getFiscalYearStartMonth());
	project->set("HonorConstraints", header->getHonorConstraints());
	project->set("InsertedProjectsLikeSummary", header->getInsertedProjectsLikeSummary());
	project->set("LastSaved", m_dataConverter.printDateTime(header->getLastSaved()));
	project->set("Manager", header->getManager());
	project->set("MicrosoftProjectServerURL", header->getMicrosoftProjectServerURL());
	project->set("MinutesPerDay", header->getMinutesPerDay());
	project->set("MinutesPerWeek", header->getMinutesPerWeek());
	project->set("MoveCompletedEndsBack", header->getMoveCompletedEndsBack());
	project->set("MoveCompletedEndsForward", header->getMoveCompletedEndsForward());
	project->set("MoveRemainingStartsBack", header->getMoveRemainingStartsBack());
	project->set("MoveRemainingStartsForward", header->getMoveRemainingStartsForward());
	project->set("MultipleCriticalPaths", header->getMultipleCriticalPaths());
	project->set("Name", header->getName());
	project->set("NewTasksEffortDriven", header->getNewTasksEffortDriven());
	project->set("NewTasksEstimated", header->getNewTasksEstimated());
	project->set("NewTaskStartDate", header->getNewTaskStartIsProjectStart() == true ? 0 : 1);
	project->set("ProjectExternallyEdited", header->getProjectExternallyEdited());
	project->set("RemoveFileProperties", header->getRemoveFileProperties());
	project->set("Revision", header->getRevision());
	project->set("SaveVersion", (int) m_saveVersion);
	project->set("ScheduleFromStart", header->getScheduleFrom() == ScheduleFrom::SCHEDULE_START);
	project->set("SplitsInProgressTasks", header->getSplitInProgressTasks());
	project->set("SpreadActualCost", header->getSpreadActualCost());
	project->set("SpreadPercentComplete", header->getSpreadPercentComplete());
	project->set("StartDate", m_dataConverter.printDateTime(header->getStartDate()));
	project->set("StatusDate", m_dataConverter.printDateTime(header->getStatusDate()));
	project->set("Subject", header->getSubject());
	project->set("TaskUpdatesResource2", header->getUpdatingTaskStatusUpdatesResource2Status());
	project->set("Title", header->getProjectTitle());
	project->set("UID", header->getUniqueID());
	project->set("WeekStartDay", m_dataConverter.printDay(header->getWeekStartDay()));
	project->set("WorkFormat", m_dataConverter.printWorkUnits(header->getDefaultWorkUnits()));
}

/**
* This method writes project extended attribute data into an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIWriter::writeProjectExtendedAttributes(std::shared_ptr<mspdi::schema::Project> project)
{
    std::shared_ptr<mspdi::schema::ExtendedAttributes> attributes (m_factory.createProjectExtendedAttributes());
    project->set("ExtendedAttributes", attributes);
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> list = attributes->getExtendedAttribute();

    writeTaskFieldAliases(list);
    writeResource2FieldAliases(list);
}

/**
* Writes field aliases.
* 
* @param list field alias list
*/
void MSPDIWriter::writeTaskFieldAliases(std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> list)
{
	std::map<TaskField, CString>* fieldAliasMap = m_projectFile->getTaskFieldAliasMap();

	for (size_t i = 0; i < getAllTaskExtendedAttributes()->GetCount(); i ++)
    {
		TaskField key = getAllTaskExtendedAttributes()->GetAt(i);

        int fieldID = (int)(MPPTaskField::getID(key) | MPPTaskField::TASK_FIELD_BASE);

		CString name = TaskFieldClass::getInstance(key)->getName(); //key.getName();
		CString alias; 
		//fieldAliasMap->Lookup(key, alias);
		auto it = fieldAliasMap->find(key);
		if (it != fieldAliasMap->end())
			alias = it->second;

        if (m_taskExtendedAttributes.contains(key) || alias.GetLength() > 0)
        {
			std::shared_ptr<mspdi::schema::ExtendedAttribute> attribute (m_factory.createProjectExtendedAttributesExtendedAttribute());
			list->Add(attribute);
			attribute->set("FieldID", fieldID);
			if (name.GetLength()) attribute->set("FieldName", name);
			if (alias.GetLength()) attribute->set("Alias", alias);
        }
    }
}

/**
* Writes field aliases.
* 
* @param list field alias list
*/
void MSPDIWriter::writeResource2FieldAliases(std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> list)
{
	std::map<Resource2Field, CString>* fieldAliasMap = m_projectFile->getResource2FieldAliasMap();

	for (size_t i = 0; i < getAllResource2ExtendedAttributes()->GetCount(); i ++)
    {
		Resource2Field key = getAllResource2ExtendedAttributes()->GetAt(i);

        int fieldID = (int)(MPPResource2Field::getID(key) | MPPResource2Field::RESOURCE_FIELD_BASE);
		CString name = Resource2FieldClass::getInstance(key)->getName();
		CString alias; 
		//fieldAliasMap->Lookup(key, alias);
		auto it = fieldAliasMap->find(key);
		if (it != fieldAliasMap->end())
			alias = it->second;
		
        if (m_resourceExtendedAttributes.contains(key) || alias.GetLength())
        {
			std::shared_ptr<mspdi::schema::ExtendedAttribute> attribute (m_factory.createProjectExtendedAttributesExtendedAttribute());
			list->Add(attribute);
			attribute->set("FieldID",fieldID);
			attribute->set("FieldName", name);
			attribute->set("Alias", alias);
        }
    }
}

/**
* This method writes calendar data to an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIWriter::writeCalendars(std::shared_ptr<mspdi::schema::Project> project)
{
    //
    // Create the new MSPDI calendar list
    //
	std::shared_ptr<mspdi::schema::Calendars> calendars (m_factory.createProjectCalendars());
	project->set("Calendars", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(calendars));
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::Calendar>>> calendar = calendars->getCalendar();

    //
    // Process each calendar in turn
    //
	for (size_t i = 0; i < m_projectFile->getCalendars()->GetCount(); i ++)
    {
		ProjectCalendarPtr cal = m_projectFile->getCalendars()->GetAt(i);
        calendar->Add(writeCalendar(cal));
    }
}

/**
* This method writes data for a single calendar to an MSPDI file.
*
* @param bc Base calendar data
* @return New MSPDI calendar instance
*/
std::shared_ptr<mspdi::schema::Calendar> MSPDIWriter::writeCalendar(ProjectCalendarPtr bc)
{
    //
    // Create a calendar
    //
	std::shared_ptr<mspdi::schema::Calendar> calendar (m_factory.createProjectCalendarsCalendar());
    calendar->set("UID", bc->getUniqueID());
	calendar->set("IsBaseCalendar", !bc->isDerived());

    ProjectCalendarPtr base = bc->getParent();
    if (base)
    {
		calendar->set("BaseCalendarUID", base->getUniqueID());
    }

	calendar->set("Name", bc->getName());

    //
    // Create a list of normal days
    //
	std::shared_ptr<mspdi::schema::WeekDays> days (m_factory.createProjectCalendarsCalendarWeekDays());
	std::shared_ptr<mspdi::schema::WorkingTime> time;
    ProjectCalendarHoursPtr bch;

	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WeekDay>>> dayList = days->getWeekDay();

    for (int loop = 1; loop < 8; loop++)
    {
		DayType workingFlag = bc->getWorkingDay((Day)loop);

        if (workingFlag != DayType::DEFAULT)
        {
			std::shared_ptr<mspdi::schema::WeekDay> day (m_factory.createProjectCalendarsCalendarWeekDaysWeekDay());
			dayList->Add(day);
			day->set("DayType", loop);
			day->set("DayWorking", workingFlag == DayType::WORKING);

			if (workingFlag != DayType::WORKING)
				continue;

			std::shared_ptr<mspdi::schema::WorkingTimes> times (m_factory.createProjectCalendarsCalendarWeekDaysWeekDayWorkingTimes());
			day->set("WorkingTimes", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(times));
			std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WorkingTime>>> timesList = times->getWorkingTime();

			bch = bc->getCalendarHours((Day)loop);
			for (int i = 0; bch && i < bch->getRangeCount(); i++)
			{
				DateRangePtr range = bch->getRange(i);
				if (range == NULL)
					continue;
				time.reset (m_factory.createProjectCalendarsCalendarWeekDaysWeekDayWorkingTimesWorkingTime());
				timesList->Add(time);

				time->set("FromTime", m_dataConverter.printTime2T(range->getStart()));
				time->set("ToTime", m_dataConverter.printTime2T(range->getEnd()));
			}
        }
    }

    //
    // Create a list of exceptions
    //
    // A quirk of MS Project* is that these exceptions must be
    // in date order in the file, otherwise they are ignored
    //
	std::shared_ptr<List<ProjectCalendarExceptionPtr>> exceptions (new List<ProjectCalendarExceptionPtr>());
	exceptions->Append(*bc->getCalendarExceptions());
    if (!exceptions->isEmpty())
    {
		Collections::sort_object_list(*exceptions);
        writeExceptions(calendar, dayList, exceptions);
    }

    //
    // Do not add a weekdays tag to the calendar unless it
    // has valid entries.
    // Fixes SourceForge bug 1854747: MPXJ and MSP 2007 XML formats
    //
    if (!dayList->isEmpty())
    {
		calendar->set("WeekDays", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(days));
    }

    writeWorkWeeks(calendar, bc);

    m_projectFile->fireCalendarWrittenEvent(bc);

    return (calendar);
}

/**
* Main entry point used to determine the format used to write 
* calendar exceptions.
* 
* @param calendar parent calendar
* @param dayList list of calendar days
* @param exceptions list of exceptions
*/
void MSPDIWriter::writeExceptions(std::shared_ptr<mspdi::schema::Calendar> calendar,
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WeekDay>>> dayList,
	std::shared_ptr<List<ProjectCalendarExceptionPtr>> exceptions)
{
    if (m_saveVersion < SaveVersion::Project2007)
    {
        writeExcepions9(dayList, exceptions);
    }
    else
    {
        writeExcepions12(calendar, exceptions);
    }
}

/**
* Write exceptions in the format used by MSPDI files prior to Project* 2007.
* 
* @param dayList list of calendar days
* @param exceptions list of exceptions
*/
void MSPDIWriter::writeExcepions9(
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WeekDay>>> dayList,
	std::shared_ptr<List<ProjectCalendarExceptionPtr>> exceptions)
{
	for (size_t i = 0; i < exceptions->GetCount(); i++)
    {
		ProjectCalendarExceptionPtr exception = exceptions->GetAt(i);

        bool working = exception->getWorking();

		std::shared_ptr<mspdi::schema::WeekDay> day (m_factory.createProjectCalendarsCalendarWeekDaysWeekDay());
        dayList->Add(day);
        day->set("DayType",BIGINTEGER_ZERO);
		day->set("DayWorking", working);

		std::shared_ptr<mspdi::schema::TimePeriod> period (m_factory.createProjectCalendarsCalendarWeekDaysWeekDayTimePeriod());
		day->set("TimePeriod", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(period));
		period->set("FromDate", m_dataConverter.printDateTime(exception->getFromDate()));
		period->set("ToDate", m_dataConverter.printDateTime(exception->getToDate()));

        if (working)
        {
			std::shared_ptr<mspdi::schema::WorkingTimes> times (m_factory.createProjectCalendarsCalendarWeekDaysWeekDayWorkingTimes());
			day->set("WorkingTimes", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(times));
			std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WorkingTime>>> timesList = times->getWorkingTime();

			for (int i = 0; i < exception->getRangeCount(); i ++)
			{
				DateRangePtr range = exception->getRange(i);
				std::shared_ptr<mspdi::schema::WorkingTime> time (
					m_factory.createProjectCalendarsCalendarWeekDaysWeekDayWorkingTimesWorkingTime());
				timesList->Add(time);

				time->set("FromTime", m_dataConverter.printTime2T(range->getStart()));
				time->set("ToTime", m_dataConverter.printTime2T(range->getEnd()));
			}
        }
    }
}

/**
* Write exceptions into the format used by MSPDI files from
* Project* 2007 onwards.
* 
* @param calendar parent calendar
* @param exceptions list of exceptions
*/
void MSPDIWriter::writeExcepions12(
	std::shared_ptr<mspdi::schema::Calendar> calendar, 
	std::shared_ptr<List<ProjectCalendarExceptionPtr>> exceptions)
{
	std::shared_ptr<mspdi::schema::Exceptions> ce(m_factory.createProjectCalendarsCalendarExceptions());
	calendar->set("Exceptions", ce);
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::Exception>>> el = ce->getException();

	for (size_t i = 0; i < exceptions->GetCount(); i ++)
    {
		ProjectCalendarExceptionPtr exception = exceptions->GetAt(i);

		std::shared_ptr<mspdi::schema::Exception> ex (m_factory.createProjectCalendarsCalendarExceptionsException());
        el->Add(ex);

		ex->set("EnteredByOccurrences", false);
		ex->set("Occurrences", 1);
		ex->set("Type", 1);

        bool working = exception->getWorking();
		ex->set("DayWorking", working);

		std::shared_ptr<mspdi::schema::TimePeriod> period (m_factory.createProjectCalendarsCalendarExceptionsExceptionTimePeriod());
		ex->set("TimePeriod", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(period));
		period->set("FromDate", m_dataConverter.printDateTime(exception->getFromDate()));
		period->set("ToDate", m_dataConverter.printDateTime(exception->getToDate()));

        if (working)
        {
			std::shared_ptr<mspdi::schema::WorkingTimes> times (m_factory.createProjectCalendarsCalendarExceptionsExceptionWorkingTimes());
			ex->set("WorkingTimes", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(times));
			std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WorkingTime>>> timesList = times->getWorkingTime();

			for (int j = 0; j < exception->getRangeCount(); j++)
			{
				DateRangePtr range = exception->getRange(j);
				std::shared_ptr<mspdi::schema::WorkingTime> time (m_factory.createProjectCalendarsCalendarExceptionsExceptionWorkingTimesWorkingTime());
				timesList->Add(time);

				time->set("FromTime", m_dataConverter.printTime2T(range->getStart()));
				time->set("ToTime", m_dataConverter.printTime2T(range->getEnd()));
			}
        }
    }
}

/**
* Write the work weeks associated with this calendar->
* 
* @param xmlCalendar XML calendar instance
* @param mpxjCalendar MPXJ calendar instance
*/
void MSPDIWriter::writeWorkWeeks(std::shared_ptr<mspdi::schema::Calendar> xmlCalendar, 
									ProjectCalendarPtr mpxjCalendar)
{
    List<ProjectCalendarWeekPtr>* weeks = mpxjCalendar->getWorkWeeks();
	if (!weeks->GetCount())
		return;

	std::shared_ptr<mspdi::schema::WorkWeeks> xmlWorkWeeks (m_factory.createProjectCalendarsCalendarWorkWeeks());
	xmlCalendar->set("WorkWeeks", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(xmlWorkWeeks));
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WorkWeek>>> xmlWorkWeekList = xmlWorkWeeks->getWorkWeek();

	for (size_t i = 0; i < weeks->GetCount(); i ++)
    {
		ProjectCalendarWeekPtr week = weeks->GetAt(i);

		std::shared_ptr<mspdi::schema::WorkWeek> xmlWeek (m_factory.createProjectCalendarsCalendarWorkWeeksWorkWeek());
		xmlWorkWeekList->Add(xmlWeek);

		xmlWeek->set("Name", week->getName());
		std::shared_ptr<mspdi::schema::TimePeriod> xmlTimePeriod (m_factory.createProjectCalendarsCalendarWorkWeeksWorkWeekTimePeriod());
		xmlWeek->set("TimePeriod", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(xmlTimePeriod));
		xmlTimePeriod->set("FromDate", m_dataConverter.printDateTime(week->getDateRange()->getStart()));
		xmlTimePeriod->set("ToDate", m_dataConverter.printDateTime(week->getDateRange()->getEnd()));

		std::shared_ptr<mspdi::schema::WeekDays> xmlWeekDays (m_factory.createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDays());
		xmlWeek->set("WeekDays", xmlWeekDays);

		std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WeekDay>>> dayList = xmlWeekDays->getWeekDay();

		for (int loop = 1; loop < 8; loop++)
		{
			DayType workingFlag = week->getWorkingDay((Day)loop);

			if (workingFlag == DayType::DEFAULT)
				continue;

			std::shared_ptr<mspdi::schema::WeekDay> day (
				m_factory.createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDaysWeekDay());
			dayList->Add(day);
			day->set("DayType",loop);
			day->set("DayWorking", workingFlag == DayType::WORKING);

			if (workingFlag != DayType::WORKING)
				continue;

			std::shared_ptr<mspdi::schema::WorkingTimes> times (
				m_factory.createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDaysWeekDayWorkingTimes());
			day->set("WorkingTimes", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(times));
			std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WorkingTime>>> timesList = times->getWorkingTime();

			ProjectCalendarHoursPtr bch = week->getCalendarHours((Day)loop);
			for (int j = 0; bch && j < bch->getRangeCount(); j++)
			{
				DateRangePtr range = bch->getRange(j);
				if (range == NULL)
					continue;
				std::shared_ptr<mspdi::schema::WorkingTime> time (
					m_factory.createProjectCalendarsCalendarWorkWeeksWorkWeekWeekDaysWeekDayWorkingTimesWorkingTime());
				timesList->Add(time);

				time->set("FromTime", m_dataConverter.printTime2T(range->getStart()));
				time->set("ToTime", m_dataConverter.printTime2T(range->getEnd()));
			}
		}
    }
}

/**
* This method writes resource data to an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIWriter::writeResource2s(std::shared_ptr<mspdi::schema::Project> project)
{
	std::shared_ptr<mspdi::schema::Resource2s> resources (m_factory.createProjectResource2s());
	project->set("Resource2s", std::dynamic_pointer_cast<mspdi::schema::FieldValue>(resources));
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::Resource2>>> list = resources->getResource2();

	for (size_t i = 0; i < m_projectFile->getAllResource2s()->GetCount(); i ++)
    {
		Resource2Ptr resource = m_projectFile->getAllResource2s()->GetAt(i);
        list->Add(writeResource2(resource));
    }
}

/**
* This method writes data for a single resource to an MSPDI file.
*
* @param mpx Resource2Ptr data
* @return New MSPDI resource instance
*/
std::shared_ptr<mspdi::schema::Resource2> MSPDIWriter::writeResource2(Resource2Ptr mpx)
{
	std::shared_ptr<mspdi::schema::Resource2> xml (m_factory.createProjectResource2sResource2());
    ProjectCalendarPtr cal = mpx->getResource2Calendar();
    if (cal)
    {
		xml->set("CalendarUID", cal->getUniqueID());
    }

	xml->set("AccrueAt", mpx->getAccrueAt());
	xml->set("ActiveDirectoryGUID", mpx->getActiveDirectoryGUID());
	xml->set("ActualCost", m_dataConverter.printCurrency(mpx->getActualCost()));
	xml->set("ActualOvertimeCost", m_dataConverter.printCurrency(mpx->getActualOvertimeCost()));
	xml->set("ActualOvertimeWork", m_dataConverter.printDuration(this, mpx->getActualOvertimeWork()));
	xml->set("ActualOvertimeWorkProtected", m_dataConverter.printDuration(this, mpx->getActualOvertimeWorkProtected()));
	xml->set("ActualWork", m_dataConverter.printDuration(this, mpx->getActualWork()));
	xml->set("ActualWorkProtected", m_dataConverter.printDuration(this, mpx->getActualWorkProtected()));
	xml->set("ACWP", m_dataConverter.printCurrency(mpx->getACWP()));
	xml->set("AvailableFrom", m_dataConverter.printDateTime(mpx->getAvailableFrom()));
	xml->set("AvailableTo", m_dataConverter.printDateTime(mpx->getAvailableTo()));
	xml->set("BCWS", m_dataConverter.printCurrency(mpx->getBCWS()));
	xml->set("BCWP", m_dataConverter.printCurrency(mpx->getBCWP()));
	xml->set("BookingType", m_dataConverter.printBookingType(mpx->getBookingType()));
	xml->set("IsBudget", mpx->getBudget());
	xml->set("CanLevel", mpx->getCanLevel());
	xml->set("Code", mpx->getCode());
	xml->set("Cost", m_dataConverter.printCurrency(mpx->getCost()));
	xml->set("CostPerUse", m_dataConverter.printCurrency(mpx->getCostPerUse()));
	xml->set("CostVariance", m_dataConverter.printCurrency(mpx->getCostVariance()));
	xml->set("CreationDate", m_dataConverter.printDateTime(mpx->getCreationDate()));
	xml->set("CV", m_dataConverter.printCurrency(mpx->getCV()));
	xml->set("EmailAddress", mpx->getEmailAddress());
	xml->set("Finish", m_dataConverter.printDateTime(mpx->getFinish()));
	xml->set("Group2", mpx->getGroup2());
	xml->set("Hyperlink", mpx->getHyperlink());
	xml->set("HyperlinkAddress", mpx->getHyperlinkAddress());
	xml->set("HyperlinkSubAddress", mpx->getHyperlinkSubAddress());
	xml->set("ID", mpx->getID());
	xml->set("Initials", mpx->getInitials());
	xml->set("IsEnterprise", mpx->getEnterprise());
	xml->set("IsGeneric", mpx->getGeneric());
	xml->set("IsInactive", mpx->getInactive());
	xml->set("IsNull", mpx->getNull());
	xml->set("MaterialLabel", mpx->getMaterialLabel());
	xml->set("MaxUnits", m_dataConverter.printUnits(mpx->getMaxUnits()));
	xml->set("Name", mpx->getName());

    if (mpx->getNotes().GetLength())
    {
		xml->set("Notes", mpx->getNotes());
    }

	xml->set("NTAccount", mpx->getNtAccount());
	xml->set("OverAllocated", mpx->getOverAllocated());
	xml->set("OvertimeCost", m_dataConverter.printCurrency(mpx->getOvertimeCost()));
	xml->set("OvertimeRate", m_dataConverter.printRate(mpx->getOvertimeRate()));
	xml->set("OvertimeRateFormat", m_dataConverter.printTimeUnit(mpx->getOvertimeRateUnits()));
	xml->set("OvertimeWork", m_dataConverter.printDuration(this, mpx->getOvertimeWork()));
	xml->set("PeakUnits", m_dataConverter.printUnits(mpx->getPeakUnits()));
	xml->set("PercentWorkComplete", mpx->getPercentWorkComplete());
	xml->set("Phonetics", mpx->getPhonetics());
	xml->set("RegularWork", m_dataConverter.printDuration(this, mpx->getRegularWork()));
	xml->set("RemainingCost", m_dataConverter.printCurrency(mpx->getRemainingCost()));
	xml->set("RemainingOvertimeCost", m_dataConverter.printCurrency(mpx->getRemainingOvertimeCost()));
	xml->set("RemainingOvertimeWork", m_dataConverter.printDuration(this, mpx->getRemainingOvertimeWork()));
	xml->set("RemainingWork", m_dataConverter.printDuration(this, mpx->getRemainingWork()));
	xml->set("StandardRate", m_dataConverter.printRate(mpx->getStandardRate()));
	xml->set("StandardRateFormat", m_dataConverter.printTimeUnit(mpx->getStandardRateUnits()));
	xml->set("Start", m_dataConverter.printDateTime(mpx->getStart()));
	xml->set("SV", m_dataConverter.printCurrency(mpx->getSV()));
	xml->set("UID", mpx->getUniqueID());
	xml->set("Work", m_dataConverter.printDuration(this, mpx->getWork()));
	xml->set("WorkGroup2", mpx->getWorkGroup2());
	xml->set("WorkVariance", m_dataConverter.printDurationInDecimalThousandthsOfMinutes(mpx->getWorkVariance()));

    if (mpx->getType() == Resource2Type::RT_COST)
    {
		xml->set("Type", Resource2Type::RT_MATERIAL);
		xml->set("IsCostResource2", true);
    }
    else
    {
		xml->set("Type", mpx->getType());
    }

    writeResource2ExtendedAttributes(xml, mpx);

    writeResource2Baselines(xml, mpx);

    writeCostRateTables(xml, mpx);

    writeAvailability(xml, mpx);

    return (xml);
}

/**
* Writes resource baseline data.
* 
* @param xmlResource2 MSPDI resource
* @param mpxjResource2 MPXJ resource
*/
void MSPDIWriter::writeResource2Baselines(std::shared_ptr<mspdi::schema::Resource2> xmlResource2, Resource2Ptr mpxjResource2)
{
	std::shared_ptr<mspdi::schema::Baseline> baseline (m_factory.createProjectResource2sResource2Baseline());
    bool populated = false;

    NumberPtr cost = mpxjResource2->getBaselineCost();
    if (cost != NULL && cost->intValue() != 0)
    {
        populated = true;
		baseline->set("Cost", m_dataConverter.printCurrency(cost));
    }

    DurationPtr work = mpxjResource2->getBaselineWork();
    if (work != NULL && work->getDuration() != 0)
    {
        populated = true;
		baseline->set("Work", m_dataConverter.printDuration(this, work));
    }

    if (populated)
    {
        xmlResource2->getBaseline()->Add(baseline);
		baseline->set("Number", 0);
    }

    for (int loop = 1; loop <= 10; loop++)
    {
        baseline.reset (m_factory.createProjectResource2sResource2Baseline());
        populated = false;

        cost = mpxjResource2->getBaselineCost(loop);
        if (cost != NULL && cost->intValue() != 0)
        {
			populated = true;
			baseline->set("Cost", m_dataConverter.printCurrency(cost));
        }

        work = mpxjResource2->getBaselineWork(loop);
        if (work != NULL && work->getDuration() != 0)
        {
			populated = true;
			baseline->set("Work", m_dataConverter.printDuration(this, work));
        }

        if (populated)
        {
			xmlResource2->getBaseline()->Add(baseline);
			baseline->set("Number", loop);
        }
    }
}

/**
* This method writes extended attribute data for a resource.
*
* @param xml MSPDI resource
* @param mpx MPXJ resource
*/
void MSPDIWriter::writeResource2ExtendedAttributes(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx)
{
	std::shared_ptr<mspdi::schema::ExtendedAttribute> attrib;
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> extendedAttributes = xml->getExtendedAttribute();

	for (size_t i = 0; i < getAllResource2ExtendedAttributes()->GetCount(); i ++)
    {
		Resource2Field mpxFieldID = getAllResource2ExtendedAttributes()->GetAt(i);

        ObjectPtr value = mpx->getCachedValue(mpxFieldID);

		if (writeExtendedAttribute(value, Resource2FieldClass::getInstance(mpxFieldID)))
		{
			m_resourceExtendedAttributes.add(mpxFieldID);

			int xmlFieldID = (int)(MPPResource2Field::getID(mpxFieldID) | MPPResource2Field::RESOURCE_FIELD_BASE);

			attrib.reset (m_factory.createProjectResource2sResource2ExtendedAttribute());
			extendedAttributes->Add(attrib);
			attrib->set("FieldID", StringHelper::toString(xmlFieldID));
			attrib->set("Value", m_dataConverter.printExtendedAttribute(this, value,
				Resource2FieldClass::getInstance(mpxFieldID)->getDataType()));
			attrib->set("DurationFormat", printExtendedAttributeDurationFormat(value));
        }
    }
}

/**
* This method is called to determine if an extended attribute
* should be written to the file, or whether the default value
* can be assumed.
* 
* @param value extended attribute value
* @param type extended attribute data type
* @return bool flag
*/
bool MSPDIWriter::writeExtendedAttribute(ObjectPtr value, FieldType* type)
{
	if (value == NULL)
		return false;

	// 测试是否是空字符串
	VariantPtr var = std::dynamic_pointer_cast<Variant>(value);
	if (var == nullptr)
		return false;
	if (var->type() == VT_BSTR && var->stringValue().GetLength() == 0)
		return false;

	bool write = true;
    DataType dataType = type->getDataType();
    switch (dataType)
    {
		case DataType::DT_BOOLEAN:
		{
			write = NumberHelper::getBool(value);
			break;
		}

		case DataType::DT_CURRENCY:
		case DataType::DT_NUMERIC:
		{
			write = NumberHelper::getInt(value) != 0;
			break;
		}

		case DataType::DT_DURATION:
		{
			DurationPtr duration = std::dynamic_pointer_cast<Duration2>(value);
			write = duration->getDuration() != 0;
			break;
		}

		default:
		{
			break;
		}
    }

    return write;
}

/**
* Writes a resource's cost rate tables.
* 
* @param xml MSPDI resource
* @param mpx MPXJ resource
*/
void MSPDIWriter::writeCostRateTables(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx)
{
    //Rates* rates = m_factory.createProjectResource2sResource2Rates();
    //xml->setRates(rates);
    //ObjectList<mspdi::schema::Rate> ratesList = rates.getRate();

	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::Rate>>> ratesList;

    for (int tableIndex = 0; tableIndex < 5; tableIndex++)
    {
        CostRateTablePtr table = mpx->getCostRateTable(tableIndex);
		if (table == NULL)
			continue;

		DatePtr from = DateHelper::getFIRST_DATE();
		for (size_t i = 0; i < table->GetCount(); i ++)
		{
			CostRateTableEntryPtr entry = table->GetAt(i);
			if (!costRateTableWriteRequired(entry, from))
				continue;

			if (ratesList == NULL)
			{
				std::shared_ptr<mspdi::schema::Rates> rates (m_factory.createProjectResource2sResource2Rates());
				xml->set("Rates", rates);
				ratesList = rates->getRate();
			}

			std::shared_ptr<mspdi::schema::Rate> rate (m_factory.createProjectResource2sResource2RatesRate());
			ratesList->Add(rate);

			rate->set("CostPerUse", m_dataConverter.printCurrency(entry->getCostPerUse()));
			rate->set("OvertimeRate", m_dataConverter.printRate(entry->getOvertimeRate()));
			rate->set("OvertimeRateFormat", m_dataConverter.printTimeUnit(entry->getOvertimeRateFormat()));
			rate->set("RatesFrom", m_dataConverter.printDateTime(from));
			from = entry->getEndDate();
			rate->set("RatesTo", m_dataConverter.printDateTime(from));
			rate->set("RateTable", tableIndex);
			rate->set("StandardRate", m_dataConverter.printRate(entry->getStandardRate()));
			rate->set("StandardRateFormat", m_dataConverter.printTimeUnit(entry->getStandardRateFormat()));
		}
    }
}

/**
* This method determines whether the cost rate table should be written.
* A default cost rate table should not be written to the file.
* 
* @param entry cost rate table entry
* @param from from date
* @return bool flag
*/
bool MSPDIWriter::costRateTableWriteRequired(CostRateTableEntryPtr entry, DatePtr from)
{
    bool fromDate = (DateHelper::compare(from, DateHelper::getFIRST_DATE()) > 0);
    bool toDate = (DateHelper::compare(entry->getEndDate(), DateHelper::getLAST_DATE()) > 0);
    bool costPerUse = (NumberHelper::getDouble(entry->getCostPerUse()) != 0);
    bool overtimeRate = (entry->getOvertimeRate() != NULL && entry->getOvertimeRate()->getAmount() != 0);
    bool standardRate = (entry->getStandardRate() != NULL && entry->getStandardRate()->getAmount() != 0);
    return (fromDate || toDate || costPerUse || overtimeRate || standardRate);
}

/**
* This method writes a resource's availability table.
* 
* @param xml MSPDI resource
* @param mpx MPXJ resource
*/
void MSPDIWriter::writeAvailability(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx)
{
	std::shared_ptr<mspdi::schema::AvailabilityPeriods> periods (m_factory.createProjectResource2sResource2AvailabilityPeriods());
    xml->set("AvailabilityPeriods", periods);
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::AvailabilityPeriod>>> list = periods->getAvailabilityPeriod();

	for (size_t i = 0; i < mpx->getAvailability()->GetCount(); i ++)
    {
		AvailabilityPtr availability = mpx->getAvailability()->GetAt(i);

		std::shared_ptr<mspdi::schema::AvailabilityPeriod> period (
			m_factory.createProjectResource2sResource2AvailabilityPeriodsAvailabilityPeriod());
        list->Add(period);
        DateRange* range = availability->getRange();

		period->set("AvailableFrom", m_dataConverter.printDateTime(range->getStart()));
		period->set("AvailableTo", m_dataConverter.printDateTime(range->getEnd()));
		period->set("AvailableUnits", m_dataConverter.printUnits(availability->getUnits()));
    }
}

/**
* This method writes task data to an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIWriter::writeTasks(std::shared_ptr<mspdi::schema::Project> project)
{
	std::shared_ptr<mspdi::schema::Tasks> tasks (m_factory.createProjectTasks());
    project->set("Tasks", tasks);
	std::shared_ptr< ObjectList<std::shared_ptr<mspdi::schema::Task>>> list = tasks->getTask();

	size_t taskCount = m_projectFile->getAllTasks()->GetCount();
	for (size_t i = 0; i < taskCount; i++)
    {
		Task* task = m_projectFile->getAllTasks()->GetAt(i);
        list->Add(writeTask(task));
    }
}

/**
* This method writes data for a single task to an MSPDI file.
*
* @param mpx Task* data
* @return new task instance
*/
std::shared_ptr<mspdi::schema::Task> MSPDIWriter::writeTask(Task* mpx)
{
	std::shared_ptr<mspdi::schema::Task> xml (m_factory.createProjectTasksTask());

    xml->set("Active", mpx->getActive());
	xml->set("ActualCost", m_dataConverter.printCurrency(mpx->getActualCost()));
	xml->set("ActualDuration", m_dataConverter.printDuration(this, mpx->getActualDuration()));
	xml->set("ActualFinish", m_dataConverter.printDateTime(mpx->getActualFinish()));
	xml->set("ActualOvertimeCost", m_dataConverter.printCurrency(mpx->getActualOvertimeCost()));
	xml->set("ActualOvertimeWork", m_dataConverter.printDuration(this, mpx->getActualOvertimeWork()));
	xml->set("ActualOvertimeWorkProtected", m_dataConverter.printDuration(this, mpx->getActualOvertimeWorkProtected()));
	xml->set("ActualStart", m_dataConverter.printDateTime(mpx->getActualStart()));
	xml->set("ActualWork", m_dataConverter.printDuration(this, mpx->getActualWork()));
	xml->set("ActualWorkProtected", m_dataConverter.printDuration(this, mpx->getActualWorkProtected()));
	xml->set("ACWP", m_dataConverter.printCurrency(mpx->getACWP()));
	xml->set("BCWP", m_dataConverter.printCurrency(mpx->getBCWP()));
	xml->set("BCWS", m_dataConverter.printCurrency(mpx->getBCWS()));
	xml->set("CalendarUID", getTaskCalendarID(mpx));
	xml->set("ConstraintDate", m_dataConverter.printDateTime(mpx->getConstraintDate()));
	xml->set("ConstraintType", m_dataConverter.printConstraintType(mpx->getConstraintType()));
	xml->set("Contact", mpx->getContact());
	xml->set("Cost", m_dataConverter.printCurrency(mpx->getCost()));
	xml->set("CreateDate", m_dataConverter.printDateTime(mpx->getCreateDate()));
	xml->set("Critical", mpx->getCritical());
	xml->set("CV", m_dataConverter.printCurrency(mpx->getCV()));
	xml->set("Deadline", m_dataConverter.printDateTime(mpx->getDeadline()));
	xml->set("Duration", m_dataConverter.printDurationMandatory(this, mpx->getDuration()));
	xml->set("DurationText", mpx->getDurationText());
	xml->set("DurationFormat", m_dataConverter.printDurationTimeUnits(mpx->getDuration(), mpx->getEstimated()));
	xml->set("EarlyFinish", m_dataConverter.printDateTime(mpx->getEarlyFinish()));
	xml->set("EarlyStart", m_dataConverter.printDateTime(mpx->getEarlyStart()));
	xml->set("EarnedValueMethod", m_dataConverter.printEarnedValueMethod(mpx->getEarnedValueMethod()));
	xml->set("EffortDriven", mpx->getEffortDriven());
	xml->set("Estimated", mpx->getEstimated());
	xml->set("ExternalTask", mpx->getExternalTask());
	xml->set("ExternalTaskProject", mpx->getProject());
	xml->set("Finish", m_dataConverter.printDateTime(mpx->getFinish()));
	xml->set("FinishText", mpx->getFinishText());
	xml->set("FinishVariance", m_dataConverter.printDurationInIntegerThousandthsOfMinutes(mpx->getFinishVariance()));
	xml->set("FixedCost", m_dataConverter.printCurrency(mpx->getFixedCost()));

    AccrueType fixedCostAccrual = mpx->getFixedCostAccrual();
    if (fixedCostAccrual == NULL)
    {
		fixedCostAccrual = AccrueType::ACCRUE_PRORATED;
    }
	xml->set("FixedCostAccrual", fixedCostAccrual);

    // This is not correct
    //xml->setFreeSlack(BigInteger.valueOf((long)m_dataConverter.printDurationInMinutes(mpx->getFreeSlack())*1000));
    //xml->setFreeSlack(BIGINTEGER_ZERO);
	xml->set("HideBar", mpx->getHideBar());
	xml->set("IsNull", mpx->getNull());
	xml->set("IsSubproject", mpx->getSubProject() != NULL);
	xml->set("IsSubprojectReadOnly", mpx->getSubprojectReadOnly());
	xml->set("Hyperlink", mpx->getHyperlink());
	xml->set("HyperlinkAddress", mpx->getHyperlinkAddress());
	xml->set("HyperlinkSubAddress", mpx->getHyperlinkSubAddress());
	xml->set("ID", mpx->getID());
	xml->set("IgnoreResource2Calendar", mpx->getIgnoreResource2Calendar());
	xml->set("LateFinish", m_dataConverter.printDateTime(mpx->getLateFinish()));
	xml->set("LateStart", m_dataConverter.printDateTime(mpx->getLateStart()));
	xml->set("LevelAssignments", mpx->getLevelAssignments());
	xml->set("LevelingCanSplit", mpx->getLevelingCanSplit());

    if (mpx->getLevelingDelay() != NULL)
    {
        DurationPtr levelingDelay = mpx->getLevelingDelay();
        double tenthMinutes = 10.0 * Duration2::convertUnits(levelingDelay->getDuration(), 
			levelingDelay->getUnits(), TimeUnit::MINUTES, m_projectFile->getProjectHeader())->getDuration();
		xml->set("LevelingDelay", (long)tenthMinutes);
		xml->set("LevelingDelayFormat", m_dataConverter.printDurationTimeUnits(levelingDelay, false));
    }

	xml->set("Manual", mpx->getTaskMode() == TaskMode::MANUALLY_SCHEDULED);

    if (mpx->getTaskMode() == TaskMode::MANUALLY_SCHEDULED)
    {
		xml->set("ManualDuration", m_dataConverter.printDuration(this, mpx->getDuration()));
		xml->set("ManualFinish", m_dataConverter.printDateTime(mpx->getFinish()));
		xml->set("ManualStart", m_dataConverter.printDateTime(mpx->getStart()));
    }

	xml->set("Milestone", mpx->getMilestone());
	xml->set("Name", mpx->getName());

    if (mpx->getNotes().GetLength())
    {
		xml->set("Notes", mpx->getNotes());
    }

	xml->set("OutlineLevel", mpx->getOutlineLevel());
	xml->set("OutlineNumber", mpx->getOutlineNumber());
	xml->set("OverAllocated", mpx->getOverAllocated());
	xml->set("OvertimeCost", m_dataConverter.printCurrency(mpx->getOvertimeCost()));
	xml->set("OvertimeWork", m_dataConverter.printDuration(this, mpx->getOvertimeWork()));
	xml->set("PercentComplete", mpx->getPercentageComplete());
	xml->set("PercentWorkComplete", mpx->getPercentageWorkComplete());
	xml->set("PhysicalPercentComplete", mpx->getPhysicalPercentComplete());
	xml->set("Priority", m_dataConverter.printPriority(mpx->getPriority()));
	xml->set("Recurring", mpx->getRecurring());
	xml->set("RegularWork", m_dataConverter.printDuration(this, mpx->getRegularWork()));
	xml->set("RemainingCost", m_dataConverter.printCurrency(mpx->getRemainingCost()));

    if (mpx->getRemainingDuration() == NULL)
    {
        DurationPtr duration = mpx->getDuration();

        if (duration)
        {
			double amount = duration->getDuration();
			amount -= ((amount * NumberHelper::getDouble(mpx->getPercentageComplete())) / 100);
			xml->set("RemainingDuration", 
				m_dataConverter.printDuration(this, Duration2::getInstance(amount, duration->getUnits())));
        }
    }
    else
    {
		xml->set("RemainingDuration", m_dataConverter.printDuration(this, mpx->getRemainingDuration()));
    }

	xml->set("RemainingOvertimeCost", m_dataConverter.printCurrency(mpx->getRemainingOvertimeCost()));
	xml->set("RemainingOvertimeWork", m_dataConverter.printDuration(this, mpx->getRemainingOvertimeWork()));
	xml->set("RemainingWork", m_dataConverter.printDuration(this, mpx->getRemainingWork()));
	xml->set("Resume", m_dataConverter.printDateTime(mpx->getResume()));
	xml->set("ResumeValid", mpx->getResumeValid());
	xml->set("Rollup", mpx->getRollup());
	xml->set("Start", m_dataConverter.printDateTime(mpx->getStart()));
	xml->set("StartText", mpx->getStartText());
	xml->set("StartVariance", m_dataConverter.printDurationInIntegerThousandthsOfMinutes(mpx->getStartVariance()));
	xml->set("Stop", m_dataConverter.printDateTime(mpx->getStop()));
	xml->set("SubprojectName", mpx->getSubprojectName());
	xml->set("Summary", mpx->getSummary());
	xml->set("TotalSlack", m_dataConverter.printDurationInIntegerThousandthsOfMinutes(mpx->getTotalSlack()));
	xml->set("Type", mpx->getType());
	xml->set("UID", mpx->getUniqueID());
	xml->set("WBS", mpx->getWBS());
	xml->set("WBSLevel", mpx->getWBSLevel());
	xml->set("Work", m_dataConverter.printDuration(this, mpx->getWork()));
	xml->set("WorkVariance", m_dataConverter.printDurationInDecimalThousandthsOfMinutes(mpx->getWorkVariance()));

    if (mpx->getTaskMode() == TaskMode::MANUALLY_SCHEDULED)
    {
		xml->set("ManualDuration", m_dataConverter.printDuration(this, mpx->getManualDuration()));
    }

    writePredecessors(xml, mpx);

    writeTaskExtendedAttributes(xml, mpx);

    writeTaskBaselines(xml, mpx);

    return (xml);
}

/**
* Writes task baseline data.
* 
* @param xmlTask MSPDI task
* @param mpxjTask MPXJ task
*/
void MSPDIWriter::writeTaskBaselines(std::shared_ptr<mspdi::schema::Task> xmlTask, Task* mpxjTask)
{
	std::shared_ptr<mspdi::schema::Baseline> baseline (m_factory.createProjectTasksTaskBaseline());
    bool populated = false;

    NumberPtr cost = mpxjTask->getBaselineCost();
    if (cost != NULL && cost->intValue() != 0)
    {
        populated = true;
		baseline->set("Cost", m_dataConverter.printCurrency(cost));
    }

    DurationPtr duration = mpxjTask->getBaselineDuration();
    if (duration && duration->getDuration() != 0)
    {
        populated = true;
		baseline->set("Duration", m_dataConverter.printDuration(this, duration));
		baseline->set("DurationFormat", m_dataConverter.printDurationTimeUnits(duration, false));
    }

    DatePtr date = mpxjTask->getBaselineFinish();
    if (date != NULL)
    {
        populated = true;
		baseline->set("Finish", m_dataConverter.printDateTime(date));
    }

    date = mpxjTask->getBaselineStart();
    if (date != NULL)
    {
        populated = true;
		baseline->set("Start", m_dataConverter.printDateTime(date));
    }

    duration = mpxjTask->getBaselineWork();
    if (duration != NULL && duration->getDuration() != 0)
    {
        populated = true;
		baseline->set("Work", m_dataConverter.printDuration(this, duration));
    }

    if (populated)
    {
		baseline->set("Number", 0);
        xmlTask->getBaseline()->Add(baseline);
    }

    for (int loop = 1; loop <= 10; loop++)
    {
        baseline.reset (m_factory.createProjectTasksTaskBaseline());
        populated = false;

        cost = mpxjTask->getBaselineCost(loop);
        if (cost != NULL && cost->intValue() != 0)
        {
			populated = true;
			baseline->set("Cost", m_dataConverter.printCurrency(cost));
        }

        duration = mpxjTask->getBaselineDuration(loop);
        if (duration != NULL && duration->getDuration() != 0)
        {
			populated = true;
			baseline->set("Duration", m_dataConverter.printDuration(this, duration));
			baseline->set("DurationFormat", m_dataConverter.printDurationTimeUnits(duration, false));
        }

        date = mpxjTask->getBaselineFinish(loop);
        if (date != NULL)
        {
			populated = true;
			baseline->set("Finish", m_dataConverter.printDateTime(date));
        }

        date = mpxjTask->getBaselineStart(loop);
        if (date != NULL)
        {
			populated = true;
			baseline->set("Start", m_dataConverter.printDateTime(date));
        }

        duration = mpxjTask->getBaselineWork(loop);
        if (duration != NULL && duration->getDuration() != 0)
        {
			populated = true;
			baseline->set("Work", m_dataConverter.printDuration(this, duration));
        }

        if (populated)
        {
			baseline->set("Number", loop);
			xmlTask->getBaseline()->Add(baseline);
        }
    }
}

/**
* This method writes extended attribute data for a task->
*
* @param xml MSPDI task
* @param mpx MPXJ task
*/
void MSPDIWriter::writeTaskExtendedAttributes(std::shared_ptr<mspdi::schema::Task> xml, Task* mpx)
{
	std::shared_ptr<mspdi::schema::ExtendedAttribute> attrib;
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> extendedAttributes = xml->getExtendedAttribute();

	for (size_t i = 0; i < getAllTaskExtendedAttributes()->GetCount(); i ++)
    {
		TaskField mpxFieldID = getAllTaskExtendedAttributes()->GetAt(i);

		ObjectPtr value = mpx->getCachedValue(mpxFieldID);

		if (writeExtendedAttribute(value, TaskFieldClass::getInstance(mpxFieldID)))
        {
			m_taskExtendedAttributes.add(mpxFieldID);

			int xmlFieldID = (int)(MPPTaskField::getID(mpxFieldID) | MPPTaskField::TASK_FIELD_BASE);

			attrib.reset (m_factory.createProjectTasksTaskExtendedAttribute());
			extendedAttributes->Add(attrib);
			attrib->set("FieldID", StringHelper::toString(xmlFieldID));
			attrib->set("Value", m_dataConverter.printExtendedAttribute(
				this, value, TaskFieldClass::getInstance(mpxFieldID)->getDataType()));
			attrib->set("DurationFormat", printExtendedAttributeDurationFormat(value));
        }
    }
}

/**
* Converts a duration to duration time units.
*
* @param value duration value
* @return duration time units
*/
BigIntegerPtr MSPDIWriter::printExtendedAttributeDurationFormat(ObjectPtr value)
{
    BigIntegerPtr result = NULL;
	if (std::dynamic_pointer_cast<Duration2>(value))
    {
		result = m_dataConverter.printDurationTimeUnits((std::dynamic_pointer_cast<Duration2>(value))->getUnits(), false);
    }
    return (result);
}

/**
* This method retrieves the UID for a calendar associated with a task->
*
* @param mpx MPX Task* instance
* @return calendar UID
*/
BigIntegerPtr MSPDIWriter::getTaskCalendarID(Task* mpx)
{
    BigIntegerPtr result = NULL;
    ProjectCalendarPtr cal = mpx->getCalendar();
    if (cal)
    {
		result = BigIntegerPtr(new BigInteger(cal->getUniqueID()));
    }
    else
    {
		result = BigIntegerPtr(new BigInteger(-1));
    }
    return (result);
}

/**
* This method writes predecessor data to an MSPDI file.
* We have to deal with a slight anomaly in this method that is introduced
* by the MPX file format. It would be possible for someone to create an
* MPX file with both the predecessor list and the unique ID predecessor
* list populated... which means that we must process both and avoid adding
* duplicate predecessors. Also interesting to note is that MSP98 populates
* the predecessor list, not the unique ID predecessor list, as you might
* expect.
*
* @param xml MSPDI task data
* @param mpx MPX task data
*/
void MSPDIWriter::writePredecessors(std::shared_ptr<mspdi::schema::Task> xml, Task* mpx)
{
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::PredecessorLink>>> list = xml->getPredecessorLink();

    std::shared_ptr<List<RelationPtr>> predecessors = mpx->getPredecessors();

	for (size_t i = 0; predecessors && i < predecessors->GetCount(); i++)
    {
		RelationPtr rel = predecessors->GetAt(i);

		int taskUniqueID = rel->getTargetTask()->getUniqueID();
		list->Add(writePredecessor(taskUniqueID, rel->getType(), rel->getLag()));
		m_projectFile->fireRelationWrittenEvent(rel);
    }
}

/**
* This method writes a single predecessor link to the MSPDI file.
*
* @param taskID The task UID
* @param type The predecessor type
* @param lag The lag duration
* @return A new link to be added to the MSPDI file
*/
std::shared_ptr<mspdi::schema::PredecessorLink> MSPDIWriter::writePredecessor(
	int taskID, RelationType type, DurationPtr lag)
{
	std::shared_ptr<mspdi::schema::PredecessorLink> link (m_factory.createProjectTasksTaskPredecessorLink());

	//if (taskID == 17)
	//{
	//	int i = 0;
	//}
	link->set("PredecessorUID", taskID);
	link->set("Type", (int) type);

    if (lag != NULL && lag->getDuration() != 0)
    {
        double linkLag = lag->getDuration();
        if (lag->getUnits() != TimeUnit::PERCENT)
        {
			linkLag = 10.0 * Duration2::convertUnits(linkLag, lag->getUnits(), 
				TimeUnit::MINUTES, m_projectFile->getProjectHeader())->getDuration();
        }
		link->set("LinkLag", (long) linkLag);
		link->set("LagFormat", m_dataConverter.printDurationTimeUnits(lag->getUnits(), false));
    }

    return (link);
}

/**
* This method writes assignment data to an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIWriter::writeAssignments(std::shared_ptr<mspdi::schema::Project> project)
{
	std::shared_ptr<mspdi::schema::Assignments> assignments (m_factory.createProjectAssignments());
    project->set("Assignments", assignments);
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::Assignment>>> list = assignments->getAssignment();

	for (size_t i = 0; i < m_projectFile->getAllResource2Assignments()->GetCount(); i ++)
    {
		Resource2AssignmentPtr assignment = m_projectFile->getAllResource2Assignments()->GetAt(i);
        list->Add(writeAssignment(assignment));
    }

    //
    // Check to see if we have any tasks that have a percent complete value
    // but do not have resource assignments. If any exist, then we must
    // write a dummy resource assignment record to ensure that the MSPDI
    // file shows the correct percent complete amount for the task->
    //
    bool autoUniqueID = m_projectFile->getAutoAssignmentUniqueID();
    if (!autoUniqueID)
    {
        m_projectFile->setAutoAssignmentUniqueID(true);
    }

	for (size_t i = 0; i < m_projectFile->getAllTasks()->GetCount(); i ++)
    {
		Task* task = m_projectFile->getAllTasks()->GetAt(i);

        double percentComplete = NumberHelper::getDouble(task->getPercentageComplete());
        if (((int)percentComplete) != 0 && task->getResource2Assignments()->isEmpty() == true)
        {
			Resource2AssignmentPtr dummy = m_projectFile->newResource2Assignment(task);
			DurationPtr duration = task->getDuration();
			if (!duration)
			{
				duration = Duration2::getInstance(0, TimeUnit::HOURS);
			}
			double durationValue = duration->getDuration();
			TimeUnit durationUnits = duration->getUnits();
			double actualWork = (durationValue * percentComplete) / 100;
			double remainingWork = durationValue - actualWork;

			dummy->setResource2UniqueID(NULL_RESOURCE_ID);
			dummy->setWork(duration);
			dummy->setActualWork(Duration2::getInstance(actualWork, durationUnits));
			dummy->setRemainingWork(Duration2::getInstance(remainingWork, durationUnits));

			list->Add(writeAssignment(dummy));
        }
    }

    m_projectFile->setAutoAssignmentUniqueID(autoUniqueID);
}

/**
* This method writes data for a single assignment to an MSPDI file.
*
* @param mpx Resource2Ptr assignment data 
* @return New MSPDI assignment instance
*/
std::shared_ptr<mspdi::schema::Assignment> MSPDIWriter::writeAssignment(Resource2AssignmentPtr mpx)
{
	std::shared_ptr<mspdi::schema::Assignment> xml (m_factory.createProjectAssignmentsAssignment());

    xml->set("ActualCost", m_dataConverter.printCurrency(mpx->getActualCost()));
	xml->set("ActualFinish", m_dataConverter.printDateTime(mpx->getActualFinish()));
	xml->set("ActualOvertimeCost", m_dataConverter.printCurrency(mpx->getActualOvertimeCost()));
	xml->set("ActualOvertimeWork", m_dataConverter.printDuration(this, mpx->getActualOvertimeWork()));
	xml->set("ActualStart", m_dataConverter.printDateTime(mpx->getActualStart()));
	xml->set("ActualWork", m_dataConverter.printDuration(this, mpx->getActualWork()));
	xml->set("ACWP", m_dataConverter.printCurrency(mpx->getACWP()));
	xml->set("BCWP", m_dataConverter.printCurrency(mpx->getBCWP()));
	xml->set("BCWS", m_dataConverter.printCurrency(mpx->getBCWS()));
	xml->set("BudgetCost", m_dataConverter.printCurrency(mpx->getBudgetCost()));
	xml->set("BudgetWork", m_dataConverter.printDuration(this, mpx->getBudgetWork()));
	xml->set("Cost", m_dataConverter.printCurrency(mpx->getCost()));

    if (mpx->getCostRateTableIndex() != 0)
    {
		xml->set("CostRateTable", mpx->getCostRateTableIndex());
    }

	xml->set("CreationDate", m_dataConverter.printDateTime(mpx->getCreateDate()));
	xml->set("CV", m_dataConverter.printCurrency(mpx->getCV()));
	xml->set("Delay", m_dataConverter.printDurationInIntegerTenthsOfMinutes(mpx->getDelay()));
	xml->set("Finish", m_dataConverter.printDateTime(mpx->getFinish()));
	xml->set("HasFixedRateUnits", mpx->getVariableRateUnits() == NULL);
	xml->set("FixedMaterial", mpx->getResource2() != NULL && mpx->getResource2()->getType() == Resource2Type::RT_MATERIAL);
	xml->set("Hyperlink", mpx->getHyperlink());
	xml->set("HyperlinkAddress", mpx->getHyperlinkAddress());
	xml->set("HyperlinkSubAddress", mpx->getHyperlinkSubAddress());
	xml->set("LevelingDelay", m_dataConverter.printDurationInIntegerTenthsOfMinutes(mpx->getLevelingDelay()));
	xml->set("LevelingDelayFormat", m_dataConverter.printDurationTimeUnits(mpx->getLevelingDelay(), false));

    if (mpx->getNotes().GetLength())
    {
		xml->set("Notes", mpx->getNotes());
    }

	xml->set("OvertimeCost", m_dataConverter.printCurrency(mpx->getOvertimeCost()));
	xml->set("OvertimeWork", m_dataConverter.printDuration(this, mpx->getOvertimeWork()));
	xml->set("PercentWorkComplete", mpx->getPercentageWorkComplete());
	xml->set("RateScale", mpx->getVariableRateUnits() == NULL ? NULL : m_dataConverter.printTimeUnit(mpx->getVariableRateUnits()));
	xml->set("RegularWork", m_dataConverter.printDuration(this, mpx->getRegularWork()));
	xml->set("RemainingCost", m_dataConverter.printCurrency(mpx->getRemainingCost()));
	xml->set("RemainingOvertimeCost", m_dataConverter.printCurrency(mpx->getRemainingOvertimeCost()));
	xml->set("RemainingOvertimeWork", m_dataConverter.printDuration(this, mpx->getRemainingOvertimeWork()));
	xml->set("RemainingWork", m_dataConverter.printDuration(this, mpx->getRemainingWork()));
	xml->set("Resource2UID", mpx->getResource2() == NULL ? NULL_RESOURCE_ID : NumberHelper::getInt(mpx->getResource2UniqueID()));

	xml->set("Start", m_dataConverter.printDateTime(mpx->getStart()));
	xml->set("SV", m_dataConverter.printCurrency(mpx->getSV()));
	xml->set("TaskUID", mpx->getTask()->getUniqueID());
	xml->set("UID", mpx->getUniqueID());

	xml->set("Units", m_dataConverter.printUnits(mpx->getUnits()));
	xml->set("VAC", m_dataConverter.printCurrency(mpx->getVAC()));
	xml->set("Work", m_dataConverter.printDuration(this, mpx->getWork()));
	xml->set("WorkContour", mpx->getWorkContour());

	xml->set("CostVariance", m_dataConverter.printCurrency(mpx->getCostVariance()));
	xml->set("WorkVariance", m_dataConverter.printDurationInDecimalThousandthsOfMinutes(mpx->getWorkVariance()));
	xml->set("StartVariance", m_dataConverter.printDurationInIntegerThousandthsOfMinutes(mpx->getStartVariance()));
	xml->set("FinishVariance", m_dataConverter.printDurationInIntegerThousandthsOfMinutes(mpx->getFinishVariance()));

    writeAssignmentBaselines(xml, mpx);

    writeAssignmentExtendedAttributes(xml, mpx);

    writeAssignmentTimephasedData(mpx, xml);

    m_projectFile->fireAssignmentWrittenEvent(mpx);

    return (xml);
}

/**
* Writes assignment baseline data.
* 
* @param xml MSPDI assignment
* @param mpxj MPXJ assignment
*/
void MSPDIWriter::writeAssignmentBaselines(
	std::shared_ptr<mspdi::schema::Assignment> xml, Resource2AssignmentPtr mpxj)
{
	std::shared_ptr<mspdi::schema::Baseline> baseline (m_factory.createProjectAssignmentsAssignmentBaseline());
    bool populated = false;

    NumberPtr cost = mpxj->getBaselineCost();
    if (cost != NULL && cost->intValue() != 0)
    {
        populated = true;
		baseline->set("Cost", m_dataConverter.printExtendedAttributeCurrency(cost));
    }

    DatePtr date = mpxj->getBaselineFinish();
    if (date)
    {
        populated = true;
		baseline->set("Finish", m_dataConverter.printExtendedAttributeDate(date));
    }

    date = mpxj->getBaselineStart();
    if (date)
    {
        populated = true;
		baseline->set("Start", m_dataConverter.printExtendedAttributeDate(date));
    }

    DurationPtr duration = mpxj->getBaselineWork();
    if (duration && duration->getDuration() != 0)
    {
        populated = true;
		baseline->set("Work", m_dataConverter.printDuration(this, duration));
    }

    if (populated)
    {
		baseline->set("Number", "0");
        xml->getBaseline()->Add(baseline);
    }

    for (int loop = 1; loop <= 10; loop++)
    {
        baseline.reset (m_factory.createProjectAssignmentsAssignmentBaseline());
        populated = false;

        cost = mpxj->getBaselineCost(loop);
        if (cost != NULL && cost->intValue() != 0)
        {
			populated = true;
			baseline->set("Cost", m_dataConverter.printExtendedAttributeCurrency(cost));
        }

        date = mpxj->getBaselineFinish(loop);
        if (date)
        {
			populated = true;
			baseline->set("Finish", m_dataConverter.printExtendedAttributeDate(date));
        }

        date = mpxj->getBaselineStart(loop);
        if (date)
        {
			populated = true;
			baseline->set("Start", m_dataConverter.printExtendedAttributeDate(date));
        }

        duration = mpxj->getBaselineWork(loop);
        if (duration != NULL && duration->getDuration() != 0)
        {
			populated = true;
			baseline->set("Work", m_dataConverter.printDuration(this, duration));
        }

        if (populated)
        {
			baseline->set("Number", loop);
			xml->getBaseline()->Add(baseline);
        }
    }
}

/**
* This method writes extended attribute data for an assignment->
*
* @param xml MSPDI assignment
* @param mpx MPXJ assignment
*/
void MSPDIWriter::writeAssignmentExtendedAttributes(
	std::shared_ptr<mspdi::schema::Assignment> xml, Resource2AssignmentPtr mpx)
{
	std::shared_ptr<mspdi::schema::ExtendedAttribute> attrib;
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> extendedAttributes = xml->getExtendedAttribute();

	for (size_t i = 0; i < getAllAssignmentExtendedAttributes()->GetCount(); i ++)
    {
		AssignmentField mpxFieldID = getAllAssignmentExtendedAttributes()->GetAt(i);

		ObjectPtr value = mpx->getCachedValue(mpxFieldID);

		if (writeExtendedAttribute(value, AssignmentFieldClass::getInstance(mpxFieldID)))
        {
			m_assignmentExtendedAttributes.add(mpxFieldID);

			int xmlFieldID = (int)(MPPAssignmentField::getID(mpxFieldID) | MPPAssignmentField::ASSIGNMENT_FIELD_BASE);

			attrib.reset (m_factory.createProjectAssignmentsAssignmentExtendedAttribute());
			extendedAttributes->Add(attrib);
			attrib->set("FieldID", xmlFieldID);
			attrib->set("Value", m_dataConverter.printExtendedAttribute(
				this, value, AssignmentFieldClass::getInstance(mpxFieldID)->getDataType()));
			attrib->set("DurationFormat", printExtendedAttributeDurationFormat(value));
        }
    }
}

/**
* Writes the timephased data for a resource assignment->
* 
* @param mpx MPXJ assignment
* @param xml MSDPI assignment
*/
void MSPDIWriter::writeAssignmentTimephasedData(Resource2AssignmentPtr mpx, 
	std::shared_ptr<mspdi::schema::Assignment> xml)
{
    if (m_writeTimphasedData && mpx->getHasTimephasedData())
    {
		std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::TimephasedDataType>>> list = xml->getTimephasedData();
        ProjectCalendarPtr calendar = mpx->getCalendar();
        BigIntegerPtr assignmentID = xml->getField("UID", VT_I4);

        TimephasedWorkListPtr complete = mpx->getTimephasedActualWork();
        TimephasedWorkListPtr planned = mpx->getTimephasedWork();

        if (m_splitTimephasedAsDays)
        {
			TimephasedWorkPtr lastComplete = NULL;
			if (!complete->isEmpty())
			{
				lastComplete = complete->GetAt(complete->GetCount() - 1);
			}

			TimephasedWorkPtr firstPlanned = NULL;
			if (!planned->isEmpty())
			{
				firstPlanned = planned->GetAt(0);
			}

			planned = splitDays(calendar, mpx->getTimephasedWork(), NULL, lastComplete);
			complete = splitDays(calendar, complete, firstPlanned, NULL);
        }

        writeAssignmentTimephasedData(assignmentID, list, planned, 1);
        writeAssignmentTimephasedData(assignmentID, list, complete, 2);
    }
}

/**
* Splits timephased data into individual days.
* 
* @param calendar current calendar
* @param list list of timephased assignment data
* @param first first planned assignment
* @param last last completed assignment
* @return list of timephased data ready for output
*/
TimephasedWorkListPtr MSPDIWriter::splitDays(ProjectCalendarPtr calendar, 
	TimephasedWorkListPtr list, TimephasedWorkPtr first, TimephasedWorkPtr last)
{
    TimephasedWorkListPtr result (new TimephasedWorkList());

	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = list->GetAt(i);

        DatePtr startDate = assignment->getStart();
        DatePtr finishDate = assignment->getFinish();
        DatePtr startDay = DateHelper::getDayStartDate(startDate);
        DatePtr finishDay = DateHelper::getDayStartDate(finishDate);
        if (startDay->getTime() == finishDay->getTime())
        {
			DatePtr startTime = calendar->getStartTime(startDay);
			DatePtr currentStart = DateHelper::setTime(startDay, startTime);
			if (startDate->getTime() > currentStart->getTime())
			{
				bool paddingRequired = true;

				if (last != NULL)
				{
					DatePtr lastFinish = last->getFinish();
					if (lastFinish->getTime() == startDate->getTime())
					{
						paddingRequired = false;
					}
					else
					{
						DatePtr lastFinishDay = DateHelper::getDayStartDate(lastFinish);
						if (startDay->getTime() == lastFinishDay->getTime())
						{
							currentStart = lastFinish;
						}
					}
				}

				if (paddingRequired)
				{
					DurationPtr zeroHours = Duration2::getInstance(0, TimeUnit::HOURS);
					TimephasedWorkPtr padding (new TimephasedWork());
					padding->setStart(currentStart);
					padding->setFinish(startDate);
					padding->setTotalAmount(zeroHours);
					padding->setAmountPerDay(zeroHours);
					result->Add(padding);
				}
			}

			result->Add(assignment);

			DatePtr endTime = calendar->getFinishTime(startDay);
			DatePtr currentFinish = DateHelper::setTime(startDay, endTime);
			if (finishDate->getTime() < currentFinish->getTime())
			{
				bool paddingRequired = true;

				if (first != NULL)
				{
					DatePtr firstStart = first->getStart();
					if (firstStart->getTime() == finishDate->getTime())
					{
						paddingRequired = false;
					}
					else
					{
						DatePtr firstStartDay = DateHelper::getDayStartDate(firstStart);
						if (finishDay->getTime() == firstStartDay->getTime())
						{
							currentFinish = firstStart;
						}
					}
				}

				if (paddingRequired)
				{
					DurationPtr zeroHours = Duration2::getInstance(0, TimeUnit::HOURS);
					TimephasedWorkPtr padding (new TimephasedWork());
					padding->setStart(finishDate);
					padding->setFinish(currentFinish);
					padding->setTotalAmount(zeroHours);
					padding->setAmountPerDay(zeroHours);
					result->Add(padding);
				}
			}
        }
        else
        {
			DatePtr currentStart = startDate;
			CalendarPtr cal = Calendar::getInstance();
			bool isWorking = calendar->isWorkingDate(currentStart);
			while (currentStart->getTime() < finishDate->getTime())
			{
				if (isWorking)
				{
					DatePtr endTime = calendar->getFinishTime(currentStart);
					DatePtr currentFinish = DateHelper::setTime(currentStart, endTime);
					if (currentFinish->getTime() > finishDate->getTime())
					{
						currentFinish = finishDate;
					}

					TimephasedWorkPtr split (new TimephasedWork());
					split->setStart(currentStart);
					split->setFinish(currentFinish);
					split->setTotalAmount(assignment->getAmountPerDay());
					split->setAmountPerDay(assignment->getAmountPerDay());
					result->Add(split);
				}

				cal->setTime(currentStart);
				cal->add(Calendar::DAY_OF_YEAR, 1);
				currentStart = cal->getTime();
				isWorking = calendar->isWorkingDate(currentStart);
				if (isWorking)
				{
					DatePtr startTime = calendar->getStartTime(currentStart);
					DateHelper::setTime(cal, startTime);
					currentStart = cal->getTime();
				}
			}
        }
    }

    return result;
}

/**
* Writes a list of timephased data to the MSPDI file.
* 
* @param assignmentID current assignment ID
* @param list output list of timephased data items 
* @param data input list of timephased data
* @param type list type (planned or completed)
*/
void MSPDIWriter::writeAssignmentTimephasedData(BigIntegerPtr assignmentID, 
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::TimephasedDataType>>> list, 
	TimephasedWorkListPtr data, int type)
{
	for (size_t i = 0; i < data->GetCount(); i ++)
    {
		TimephasedWorkPtr mpx = data->GetAt(i);
		std::shared_ptr<mspdi::schema::TimephasedDataType> xml (m_factory.createTimephasedDataType());
        list->Add(xml);

		xml->set("Start", m_dataConverter.printDateTime(mpx->getStart()));
		xml->set("Finish", m_dataConverter.printDateTime(mpx->getFinish()));
		xml->set("Type", type);
		xml->set("UID", assignmentID);
		xml->set("Unit", m_dataConverter.printDurationTimeUnits(mpx->getTotalAmount(), false));
		xml->set("Value", m_dataConverter.printDuration(this, mpx->getTotalAmount()));
    }
}

// 数据转换成列表
template<class T>
std::shared_ptr<List<T>> arrayToList(const T* arr, int endVal = -1)
{
	std::shared_ptr<List<T>> list (new List<T>());
	int i = 0;
	for (i = 0; i < 1024; i++)
	{
		T val = arr[i];
		if (val == -1)
			break;
		list->Add(val);
	}
	ATLASSERT(i < 100);
	return list;
}

/**
* Retrieve list of assignment extended attributes->
* 
* @return list of extended attributes
*/
std::shared_ptr<List<AssignmentField>> MSPDIWriter::getAllAssignmentExtendedAttributes()
{
	if (m_AssignmentFieldExtAttrs) return m_AssignmentFieldExtAttrs;

    std::shared_ptr<List<AssignmentField>> result (new List<AssignmentField>());

	result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_COST));
    result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_DATE));
    result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_DURATION));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_COST));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_DATE));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_DURATION));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_FLAG));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_NUMBER));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_RESOURCE_MULTI_VALUE));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_RESOURCE_OUTLINE_CODE));
    result->Append(*arrayToList(AssignmentFieldLists::ENTERPRISE_TEXT));
    result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_FINISH));
    result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_FLAG));
    result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_NUMBER));
    result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_START));
    result->Append(*arrayToList(AssignmentFieldLists::CUSTOM_TEXT));

	m_AssignmentFieldExtAttrs = result;
    return result;
}

/**
* Retrieve list of task extended attributes->
* 
* @return list of extended attributes
*/
std::shared_ptr<List<TaskField>> MSPDIWriter::getAllTaskExtendedAttributes()
{
	if (m_TaskFieldExtAttrs) return m_TaskFieldExtAttrs;

	std::shared_ptr<List<TaskField>> result (new List<TaskField>());
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_TEXT));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_START));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_FINISH));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_COST));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_DATE));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_FLAG));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_NUMBER));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_DURATION));
    result->Append(*arrayToList(TaskFieldLists::CUSTOM_OUTLINE_CODE));
    result->Append(*arrayToList(TaskFieldLists::ENTERPRISE_COST));
    result->Append(*arrayToList(TaskFieldLists::ENTERPRISE_DATE));
    result->Append(*arrayToList(TaskFieldLists::ENTERPRISE_DURATION));
    result->Append(*arrayToList(TaskFieldLists::ENTERPRISE_FLAG));
    result->Append(*arrayToList(TaskFieldLists::ENTERPRISE_NUMBER));
    result->Append(*arrayToList(TaskFieldLists::ENTERPRISE_TEXT));

	m_TaskFieldExtAttrs = result;
    return result;
}

/**
* Retrieve list of resource extended attributes->
* 
* @return list of extended attributes
*/
std::shared_ptr<List<Resource2Field>> MSPDIWriter::getAllResource2ExtendedAttributes()
{
	if (m_Resource2FieldExtAttrs) return m_Resource2FieldExtAttrs;

	std::shared_ptr<List<Resource2Field>> result (new List<Resource2Field>());
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_TEXT));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_START));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_FINISH));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_COST));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_DATE));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_FLAG));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_NUMBER));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_DURATION));
    result->Append(*arrayToList(Resource2FieldLists::CUSTOM_OUTLINE_CODE));
    result->Append(*arrayToList(Resource2FieldLists::ENTERPRISE_COST));
    result->Append(*arrayToList(Resource2FieldLists::ENTERPRISE_DATE));
    result->Append(*arrayToList(Resource2FieldLists::ENTERPRISE_DURATION));
    result->Append(*arrayToList(Resource2FieldLists::ENTERPRISE_FLAG));
    result->Append(*arrayToList(Resource2FieldLists::ENTERPRISE_NUMBER));
    result->Append(*arrayToList(Resource2FieldLists::ENTERPRISE_TEXT));

	m_Resource2FieldExtAttrs = result;
    return result;
}

/**
* Package-accessor method used to retrieve the project file
* currently being processed by this writer.
*
* @return project file instance
*/
ProjectFile* MSPDIWriter::getProjectFile()
{
    return (m_projectFile);
}