#include "stdafx.h"
#include "MSPDIReader.h"
#include "..\common\MPPAssignmentField.h"
#include "..\common\MPPResourceField.h"
#include "..\common\MPPTaskField.h"
#include "MSPDITimephasedWorkNormaliser.h"
#include "..\..\..\Toolkit\include\MemoryDataStream2.h"

MSPDIReader::MSPDIReader()
	:m_projectFile(NULL)
{
}

/**
* {@inheritDoc}
*/
void MSPDIReader::addProjectListener(ProjectListener* listener)
{
	m_projectListeners.Add(listener);
}

const TiXmlElement* GetChildElement(const TiXmlElement* root, char* name)
{
	CString findName(name);
	findName = findName.MakeLower();
	const TiXmlElement* child = root->FirstChildElement();
	while (child) {
		CString childName(child->Value());
		if (childName.MakeLower() == findName)
			break;
		child = child->NextSiblingElement();
	}

	return child;
}

// 验证是否是project xml文件格式
bool MSPDIReader::validateXmlFormat(TiXmlDocument* document)
{
	TiXmlElement* root = document->RootElement();

	CString rootName (root->Value());
	if (rootName.MakeLower() != L"project")
		return false;

	if (!GetChildElement(root, "SaveVersion"))
		return false;

	if (!GetChildElement(root, "StartDate"))
		return false;

	if (!GetChildElement(root, "FinishDate"))
		return false;

	return true;
}

/**
* {@inheritDoc}
*/
ProjectFilePtr MSPDIReader::read(BwProj::toolkit::IDataReadStream* stream)
{
	// 加载XML，转换到内存中
	BwProj::toolkit::CMemoryDataWriteStream2 memStream;
	BYTE pBuffer[10240]; DWORD nBytesRead = 0;
	while (stream->Read(pBuffer, 10240, nBytesRead) && nBytesRead > 0) {
		memStream.Write(pBuffer, nBytesRead);
	}
	memStream.Write("\0", 1);
	BwProj::toolkit::c_buffer_ptr memBuffer = BwProj::toolkit::c_buffer_ptr::alloc(memStream.GetLength());
	memStream.CopyBuffer(memBuffer.get());

	// 转换成Project对象，出错会抛出bad_exception
	TiXmlDocument document;

	// 进行编码转换（解决UTF8中的汉字乱码问题）
	const char* xmlBuffer = static_cast<char*>((void*)memBuffer.get());

	document.Parse(xmlBuffer, NULL, TIXML_ENCODING_UTF8);
	if (document.Error())
	{
		throw toolkit::Exception(CA2W(document.ErrorDesc()));
	}

	// 验证格式
	if (!validateXmlFormat(&document)) {
		throw toolkit::Exception(L"无效的Project XML文件。");
	}

	std::shared_ptr<mspdi::schema::Project> project (new mspdi::schema::Project(document.RootElement()));
	ATLASSERT(project != NULL);

	m_projectFile = new ProjectFile();

	m_projectFile->addProjectListeners(&m_projectListeners);
	m_projectFile->setAutoTaskID(false);
	m_projectFile->setAutoTaskUniqueID(false);
	m_projectFile->setAutoResource2ID(false);
	m_projectFile->setAutoResource2UniqueID(false);
	m_projectFile->setAutoOutlineLevel(false);
	m_projectFile->setAutoOutlineNumber(false);
	m_projectFile->setAutoWBS(false);
	m_projectFile->setAutoCalendarUniqueID(false);
	m_projectFile->setAutoAssignmentUniqueID(false);

	m_dataConverter.setParentFile(m_projectFile);

	Map2<int, ProjectCalendarPtr> calendarMap;// (new Map<int, ProjectCalendarPtr>());

	readProjectHeader(project); 
	readProjectExtendedAttributes(project);
	readCalendars(project, &calendarMap);
	readResource2s(project, &calendarMap);
	readTasks(project);
	readAssignments(project);

	//
	// Ensure that the unique ID counters are correct
	//
	m_projectFile->updateUniqueCounters();

	//
	// Ensure that the default calendar name is set in the project header
	//
	ProjectCalendarPtr defaultCalendar;
	if (calendarMap.Lookup(project->getInt("CalendarUID"), defaultCalendar))
	{
		m_projectFile->getProjectHeader()->setCalendarName(defaultCalendar->getName());
	}

	calendarMap.RemoveAll();
	return ProjectFilePtr(m_projectFile);
}

/**
* This method extracts project header data from an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIReader::readProjectHeader(std::shared_ptr<mspdi::schema::Project> project)
{
	ProjectHeader* header = m_projectFile->getProjectHeader();

	header->setActualsInSync(project->getBool("ActualsInSync"));
	header->setAdminProject(project->getBool("AdminProject"));
	header->setAuthor(project->getString("Author"));
	header->setAutoAddNewResource2sAndTasks(project->getBool("AutoAddNewResource2sAndTasks"));
	header->setAutolink(project->getBool("Autolink"));
	header->setBaselineForEarnedValue(project->getInt("BaselineForEarnedValue"));
	header->setCalendarName(project->getString("CalendarUID"));// () == NULL ? NULL : project->getCalendarUID().toString());
	header->setCategory(project->getString("Category"));
	header->setCompany(project->getString("Company"));
	header->setCreationDate(DatePtr(project->getDate("CreationDate")));
	header->setCriticalSlackLimit(project->getInt("CriticalSlackLimit"));
	header->setCurrencyDigits(NumberPtr(new Number(project->getInt("CurrencyDigits"))));
	header->setCurrencyCode(project->getString("CurrencyCode"));
	header->setCurrencySymbol(project->getString("CurrencySymbol"));
	header->setCurrentDate(DatePtr(project->getDate("CurrentDate")));
	header->setDaysPerMonth(NumberPtr(new Number(project->getInt("DaysPerMonth"))));
	header->setDefaultDurationUnits(m_dataConverter.parseDurationTimeUnits(NumberPtr(new Number(project->getInt("DurationFormat")))));
	header->setDefaultEndTime(DatePtr(project->getTime("DefaultFinishTime")));
	header->setDefaultFixedCostAccrual((AccrueType) project->getInt("DefaultFixedCostAccrual"));
	header->setDefaultOvertimeRate(m_dataConverter.parseRate(NumberPtr(new Number(project->getDouble("DefaultOvertimeRate")))));
	header->setDefaultStandardRate(m_dataConverter.parseRate(NumberPtr(new Number(project->getDouble("DefaultStandardRate")))));
	header->setDefaultStartTime(DatePtr(project->getTime("DefaultStartTime")));
	header->setDefaultTaskEarnedValueMethod(m_dataConverter.parseEarnedValueMethod(
		NumberPtr(new Number(project->getInt("DefaultTaskEVMethod")))));
	header->setDefaultTaskType((TaskType) project->getInt("DefaultTaskType"));
	header->setDefaultWorkUnits(m_dataConverter.parseWorkUnits(NumberPtr(new Number(project->getInt("WorkFormat")))));
	header->setEarnedValueMethod(m_dataConverter.parseEarnedValueMethod(
		NumberPtr(new Number(project->getInt("EarnedValueMethod")))));
	header->setEditableActualCosts(project->getBool("EditableActualCosts"));
	header->setExtendedCreationDate(DatePtr(project->getDate("ExtendedCreationDate")));
	header->setFinishDate(DatePtr(project->getDate("FinishDate")));
	header->setFiscalYearStart(project->getBool("FiscalYearStart"));
	header->setFiscalYearStartMonth(project->getInt("FYStartDate"));
	header->setHonorConstraints(project->getBool("HonorConstraints"));
	header->setInsertedProjectsLikeSummary(project->getBool("InsertedProjectsLikeSummary"));
	header->setLastSaved(DatePtr(project->getDate("LastSaved")));
	header->setManager(project->getString("Manager"));
	header->setMicrosoftProjectServerURL(project->getBool("MicrosoftProjectServerURL"));
	header->setMinutesPerDay(NumberPtr(new Number(project->getInt("MinutesPerDay"))));
	header->setMinutesPerWeek(NumberPtr(new Number(project->getInt("MinutesPerWeek"))));
	header->setMoveCompletedEndsBack(project->getBool("MoveCompletedEndsBack"));
	header->setMoveCompletedEndsForward(project->getBool("MoveCompletedEndsForward"));
	header->setMoveRemainingStartsBack(project->getBool("MoveRemainingStartsBack"));
	header->setMoveRemainingStartsForward(project->getBool("MoveRemainingStartsForward"));
	header->setMultipleCriticalPaths(project->getBool("MultipleCriticalPaths"));
	header->setName(project->getString("Name"));
	header->setNewTasksEffortDriven(project->getBool("NewTasksEffortDriven"));
	header->setNewTasksEstimated(project->getBool("NewTasksEstimated"));
	header->setNewTaskStartIsProjectStart(project->getInt("NewTaskStartDate") == 0);
	header->setProjectExternallyEdited(project->getBool("ProjectExternallyEdited"));
	header->setProjectTitle(project->getString("Title"));
	header->setRemoveFileProperties(project->getBool("RemoveFileProperties"));
	header->setRevision(project->getInt("Revision"));
	header->setScheduleFrom(project->getBool("ScheduleFromStart") ? ScheduleFrom::SCHEDULE_START : ScheduleFrom::SCHEDULE_FINISH);
	header->setSubject(project->getString("Subject"));
	header->setSplitInProgressTasks(project->getBool("SplitsInProgressTasks"));
	header->setSpreadActualCost(project->getBool("SpreadActualCost"));
	header->setSpreadPercentComplete(project->getBool("SpreadPercentComplete"));
	header->setStartDate(DatePtr(project->getDate("StartDate")));
	header->setStatusDate(DatePtr(project->getDate("StatusDate")));
	header->setSymbolPosition((CurrencySymbolPosition)project->getInt("CurrencySymbolPosition"));
	header->setUniqueID(project->getString("UID"));
	header->setUpdatingTaskStatusUpdatesResource2Status(project->getBool("TaskUpdatesResource2"));
	header->setWeekStartDay(m_dataConverter.parseDay(NumberPtr(new Number(project->getInt("WeekStartDay")))));
}

/**
* This method extracts calendar data from an MSPDI file.
*
* @param project Root node of the MSPDI file
* @param map Map of calendar UIDs to names
*/
void MSPDIReader::readCalendars(std::shared_ptr<mspdi::schema::Project> project, 
	Map2<int, ProjectCalendarPtr>* map)
{
	std::shared_ptr<mspdi::schema::Calendars> calendars = project->getCalendars();
	if (calendars != NULL)
	{
		List<std::pair<ProjectCalendarPtr, int>> baseCalendars;// (
									//new List<std::pair<ProjectCalendarPtr, int>>());

		for (size_t i = 0; i < calendars->getCalendar()->GetCount(); i ++)
		{
			std::shared_ptr<mspdi::schema::Calendar> cal = calendars->getCalendar()->GetAt(i);
			readCalendar(cal, map, &baseCalendars);
		}
		updateBaseCalendarNames(&baseCalendars, map);

		baseCalendars.RemoveAll();
	}

	try
	{
		ProjectHeader* header = m_projectFile->getProjectHeader();
		BigIntegerPtr calendarID (new BigInteger(header->getCalendarName()));
		ProjectCalendarPtr calendar;
		map->Lookup(calendarID->intValue(), calendar);
		if (calendar == nullptr)
		{
			calendar = m_projectFile->addDefaultBaseCalendar();
		}
		
		m_projectFile->setCalendar(calendar);
	}
	catch (toolkit::Exception&)
	{
		// Ignore exceptions
	}
}

/**
* The way calendars are stored in an MSPDI file means that there
* can be forward references between the base calendar unique ID for a
* derived calendar, and the base calendar itself. To get around this,
* we initially populate the base calendar name attribute with the
* base calendar unique ID, and now in this method we can convert those
* ID values into the correct names.
*
* @param baseCalendars list of calendars and base calendar IDs
* @param map map of calendar ID values and calendar objects
*/
void MSPDIReader::updateBaseCalendarNames(List<std::pair<ProjectCalendarPtr, int>>* baseCalendars,
	Map2<int, ProjectCalendarPtr>* map)
{
	for (size_t i = 0; i < baseCalendars->GetCount(); i ++)
	{
		std::pair<ProjectCalendarPtr, int> pair = baseCalendars->GetAt(i);

		ProjectCalendarPtr cal = pair.first;
		int baseCalendarID = pair.second;
		ProjectCalendarPtr baseCal;
		if (map->Lookup(baseCalendarID, baseCal))
		{
			cal->setParent(baseCal);
		}
	}

}

/**
* This method extracts data for a single calendar from an MSPDI file.
*
* @param calendar Calendar data
* @param map Map of calendar UIDs to names
* @param baseCalendars list of base calendars
*/
void MSPDIReader::readCalendar(std::shared_ptr<mspdi::schema::Calendar> calendar, 
	Map2<int, ProjectCalendarPtr>* map, List<std::pair<ProjectCalendarPtr, int>>* baseCalendars)
{
	ProjectCalendarPtr bc = m_projectFile->addCalendar();
	bc->setUniqueID(calendar->getInt("UID"));
	bc->setName(calendar->getString("Name"));
	VariantPtr baseCalendarID = calendar->getField("BaseCalendarUID");
	if (baseCalendarID != NULL)
	{
		baseCalendars->Add(std::pair<ProjectCalendarPtr, int>(bc, baseCalendarID->intValue()));
	}

	std::shared_ptr<mspdi::schema::WeekDays> days = calendar->getWeekDays();
	if (days)
	{
		for (size_t i = 0; i < days->getWeekDay()->GetCount(); i ++)
		{
			std::shared_ptr<mspdi::schema::WeekDay> weekDay = days->getWeekDay()->GetAt(i);
			readDay(bc, weekDay);
		}
	}
	else
	{
		bc->setWorkingDay(Day::SUNDAY, DayType::DEFAULT);
		bc->setWorkingDay(Day::MONDAY, DayType::DEFAULT);
		bc->setWorkingDay(Day::TUESDAY, DayType::DEFAULT);
		bc->setWorkingDay(Day::WEDNESDAY, DayType::DEFAULT);
		bc->setWorkingDay(Day::THURSDAY, DayType::DEFAULT);
		bc->setWorkingDay(Day::FRIDAY, DayType::DEFAULT);
		bc->setWorkingDay(Day::SATURDAY, DayType::DEFAULT);
	}

	readExceptions(calendar, bc);

	readWorkWeeks(calendar, bc);

	map->put(calendar->getInt("UID"), bc);

	m_projectFile->fireCalendarReadEvent(bc);
}

/**
* This method extracts data for a single day from an MSPDI file.
*
* @param calendar Calendar data
* @param day Day data
*/
void MSPDIReader::readDay(ProjectCalendarPtr calendar, 
	std::shared_ptr<mspdi::schema::WeekDay> day)
{
	VariantPtr dayType = day->getField("DayType");
	if (dayType != NULL)
	{
		if (dayType->intValue() == 0)
		{
			readExceptionDay(calendar, day);
		}
		else
		{
			readNormalDay(calendar, day);
		}
	}
}

/**
* This method extracts data for a normal working day from an MSPDI file.
*
* @param calendar Calendar data
* @param weekDay Day data
*/
void MSPDIReader::readNormalDay(ProjectCalendarPtr calendar, 
	std::shared_ptr<mspdi::schema::WeekDay> weekDay)
{
	int dayNumber = weekDay->getInt("DayType");// ().intValue();
	Day day = (Day)(dayNumber);
	calendar->setWorkingDay(day, weekDay->getBool("DayWorking"));
	ProjectCalendarHoursPtr hours = calendar->addCalendarHours(day);

	std::shared_ptr<mspdi::schema::WorkingTimes> times = weekDay->getWorkingTimes();
	if (times)
	{
		for (size_t i = 0; i < times->getWorkingTime()->GetCount(); i ++)
		{
			std::shared_ptr<mspdi::schema::WorkingTime> period = times->getWorkingTime()->GetAt(i);

			DatePtr startTime = period->getFromTime();
			DatePtr endTime = period->getToTime();

			if (startTime != NULL && endTime != NULL)
			{
				if (startTime->getTime() >= endTime->getTime())
				{
					CalendarPtr cal = Calendar::getInstance();
					cal->setTime(endTime);
					cal->add(Calendar::DAY_OF_YEAR, 1);
					endTime = cal->getTime();
				}

				hours->addRange(DateRangePtr(new DateRange(startTime, endTime)));
			}
		}
	}
}

/**
* This method extracts data for an exception day from an MSPDI file.
*
* @param calendar Calendar data
* @param day Day data
*/
void MSPDIReader::readExceptionDay(ProjectCalendarPtr calendar, 
	std::shared_ptr<mspdi::schema::WeekDay> day)
{
	std::shared_ptr<mspdi::schema::TimePeriod> timePeriod = day->getTimePeriod();
	DatePtr fromDate = timePeriod->getFromDate();
	DatePtr toDate = timePeriod->getToDate();
	std::shared_ptr<mspdi::schema::WorkingTimes> times = day->getWorkingTimes();
	ProjectCalendarExceptionPtr exception = calendar->addCalendarException(fromDate, toDate);

	if (times == NULL)
		return;

	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WorkingTime>>> time = times->getWorkingTime();
	for (size_t i = 0; i < time->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::WorkingTime> period = time->GetAt(i);

		DatePtr startTime = period->getFromTime();
		DatePtr endTime = period->getToTime();

		if (startTime && endTime)
		{
			if (startTime->getTime() >= endTime->getTime())
			{
				CalendarPtr cal = Calendar::getInstance();
				cal->setTime(endTime);
				cal->add(Calendar::DAY_OF_YEAR, 1);
				endTime = cal->getTime();
			}

			exception->addRange(DateRangePtr(new DateRange(startTime, endTime)));
		}
	}
}

/**
* Reads any exceptions present in the file. This is only used in MSPDI
* file versions saved by Project* 2007 and later.
*
* @param calendar XML calendar
* @param bc MPXJ calendar
*/
void MSPDIReader::readExceptions(std::shared_ptr<mspdi::schema::Calendar> calendar, ProjectCalendarPtr bc)
{
	std::shared_ptr<mspdi::schema::Exceptions> exceptions = calendar->getExceptions();
	if (!exceptions)
		return;

	for (size_t i = 0; i < exceptions->getException()->GetCount(); i++)
	{
		std::shared_ptr<mspdi::schema::Exception> exception = exceptions->getException()->GetAt(i);

		DatePtr fromDate = exception->getTimePeriod()->getFromDate();
		DatePtr toDate = exception->getTimePeriod()->getToDate();
		ProjectCalendarExceptionPtr bce = bc->addCalendarException(fromDate, toDate);

		std::shared_ptr<mspdi::schema::WorkingTimes> times = exception->getWorkingTimes();
		if (!times)
			continue;

		std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WorkingTime>>> time = times->getWorkingTime();
		for (size_t i = 0; i < time->GetCount(); i ++)
		{
			std::shared_ptr<mspdi::schema::WorkingTime> period = time->GetAt(i);

			DatePtr startTime = period->getFromTime();
			DatePtr endTime = period->getToTime();

			if (startTime != NULL && endTime != NULL)
			{
				if (startTime->getTime() >= endTime->getTime())
				{
					CalendarPtr cal = Calendar::getInstance();
					cal->setTime(endTime);
					cal->add(Calendar::DAY_OF_YEAR, 1);
					endTime = cal->getTime();
				}

				bce->addRange(DateRangePtr(new DateRange(startTime, endTime)));
			}
		}
	}
}

/**
* Read the work weeks associated with this calendar->
*
* @param xmlCalendar XML calendar object
* @param mpxjCalendar MPXJ calendar object
*/
void MSPDIReader::readWorkWeeks(std::shared_ptr<mspdi::schema::Calendar> xmlCalendar, 
	ProjectCalendarPtr mpxjCalendar)
{
	std::shared_ptr<mspdi::schema::WorkWeeks> ww = xmlCalendar->getWorkWeeks();
	if (ww == NULL) return;

	for (size_t i = 0; i < ww->getWorkWeek()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::WorkWeek> xmlWeek = ww->getWorkWeek()->GetAt(i);

		ProjectCalendarWeekPtr week = mpxjCalendar->addWorkWeek();
		week->setName(xmlWeek->getName());
		DatePtr startTime = xmlWeek->getTimePeriod()->getFromDate();
		DatePtr endTime = xmlWeek->getTimePeriod()->getToDate();
		week->setDateRange(new DateRange(startTime, endTime));

		std::shared_ptr<mspdi::schema::WeekDays> xmlWeekDays = xmlWeek->getWeekDays();
		if (!xmlWeekDays) continue;

		for (size_t j = 0; j < xmlWeekDays->getWeekDay()->GetCount(); j ++)
		{
			std::shared_ptr<mspdi::schema::WeekDay> xmlWeekDay = xmlWeekDays->getWeekDay()->GetAt(j);

			int dayNumber = xmlWeekDay->getInt("DayType");// ().intValue();
			Day day = (Day)(dayNumber);
			week->setWorkingDay(day, xmlWeekDay->getBool("DayWorking"));
			ProjectCalendarHoursPtr hours = week->addCalendarHours(day);

			std::shared_ptr<mspdi::schema::WorkingTimes> times = xmlWeekDay->getWorkingTimes();
			if (!times) continue;

			for (size_t k = 0; k < times->getWorkingTime()->GetCount(); k ++)
			{
				std::shared_ptr<mspdi::schema::WorkingTime> period = times->getWorkingTime()->GetAt(k);
				startTime = period->getFromTime();
				endTime = period->getToTime();

				if (startTime != NULL && endTime != NULL)
				{
					if (startTime->getTime() >= endTime->getTime())
					{
						CalendarPtr cal = Calendar::getInstance();
						cal->setTime(endTime);
						cal->add(Calendar::DAY_OF_YEAR, 1);
						endTime = cal->getTime();
					}

					hours->addRange(DateRangePtr(new DateRange(startTime, endTime)));
				}
			}
		}
	}
}

/**
* This method extracts project extended attribute data from an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIReader::readProjectExtendedAttributes(std::shared_ptr<mspdi::schema::Project> project)
{
	std::shared_ptr<mspdi::schema::ExtendedAttributes> attributes = project->getExtendedAttributes();
	for (size_t i = 0; attributes && i < attributes->getExtendedAttribute()->GetCount(); i++)
	{
		readFieldAlias(attributes->getExtendedAttribute()->GetAt(i));
	}
}

/**
* Read a single field alias from an extended attribute.
*
* @param attribute extended attribute
*/
void MSPDIReader::readFieldAlias(std::shared_ptr<mspdi::schema::ExtendedAttribute> attribute)
{
	CString alias = attribute->getString("Alias");

	if (alias.GetLength() == 0) return;

	int id = attribute->getInt("FieldID");
	int base = id & 0xFFFF0000;
	int index = id & 0x0000FFFF;

	switch (base)
	{
	case MPPTaskField::TASK_FIELD_BASE:
		{
			TaskField taskField = MPPTaskField::getInstance(index);
			if (taskField != NULL)
			{
				m_projectFile->setTaskFieldAlias(taskField, alias);
			}
			break;
		}

	case MPPResource2Field::RESOURCE_FIELD_BASE:
		{
			Resource2Field resourceField = MPPResource2Field::getInstance(index);
			if (resourceField != NULL)
			{
				m_projectFile->setResource2FieldAlias(resourceField, alias);
			}
			break;
		}
	}
}

/**
* This method extracts resource data from an MSPDI file.
*
* @param project Root node of the MSPDI file
* @param calendarMap Map of calendar UIDs to names
*/
void MSPDIReader::readResource2s(std::shared_ptr<mspdi::schema::Project> project, 
	Map2<int, ProjectCalendarPtr>* calendarMap)
{
	std::shared_ptr<mspdi::schema::Resource2s> resources = project->getResource2s();
	for (size_t i = 0; resources && i < resources->getResource2()->GetCount(); i++)
	{
		readResource2(resources->getResource2()->GetAt(i), calendarMap);
	}
}

/**
* This method extracts data for a single resource from an MSPDI file.
*
* @param xml Resource2* data
* @param calendarMap Map of calendar UIDs to names
*/
void MSPDIReader::readResource2(std::shared_ptr<mspdi::schema::Resource2> xml, 
	Map2<int, ProjectCalendarPtr>* calendarMap)
{
	Resource2Ptr mpx = m_projectFile->addResource2();

	mpx->setAccrueAt((AccrueType)xml->getInt("AccrueAt"));
	mpx->setActveDirectoryGUID(xml->getString("ActiveDirectoryGUID"));
	mpx->setActualCost(m_dataConverter.parseCurrency(xml->getNumber("ActualCost")));
	mpx->setActualOvertimeCost(m_dataConverter.parseCurrency(xml->getNumber("ActualOvertimeCost")));
	mpx->setActualOvertimeWork(m_dataConverter.parseDuration(
		m_projectFile, (TimeUnit)NULL, xml->getString("ActualOvertimeWork")));
	mpx->setActualOvertimeWorkProtected(m_dataConverter.parseDuration(m_projectFile, 
		(TimeUnit)NULL, xml->getString("ActualOvertimeWorkProtected")));
	mpx->setActualWork(m_dataConverter.parseDuration(
		m_projectFile, (TimeUnit)NULL, xml->getString("ActualWork")));
	mpx->setActualWorkProtected(m_dataConverter.parseDuration(
		m_projectFile, (TimeUnit)NULL, xml->getString("ActualWorkProtected")));
	mpx->setACWP(m_dataConverter.parseCurrency(xml->getNumber("ACWP")));
	mpx->setAvailableFrom(xml->getDate("AvailableFrom"));
	mpx->setAvailableTo(xml->getDate("AvailableTo"));
	mpx->setBCWS(m_dataConverter.parseCurrency(xml->getNumber("BCWS")));
	mpx->setBCWP(m_dataConverter.parseCurrency(xml->getNumber("BCWP")));
	mpx->setBookingType((BookingType)xml->getInt("BookingType"));
	//mpx->setBaseCalendar ();
	//mpx->setBaselineCost();
	//mpx->setBaselineWork();
	mpx->setBudget(xml->getBool("IsBudget"));
	mpx->setCanLevel(xml->getBool("CanLevel"));
	mpx->setCode(xml->getString("Code"));
	mpx->setCost(m_dataConverter.parseCurrency(xml->getNumber("Cost")));
	mpx->setCostPerUse(m_dataConverter.parseCurrency(xml->getNumber("CostPerUse")));
	mpx->setCostVariance(m_dataConverter.parseCurrency(xml->getNumber("CostVariance")));
	mpx->setCreationDate(xml->getDate("CreationDate"));
	mpx->setCV(m_dataConverter.parseCurrency(xml->getNumber("CV")));
	mpx->setEmailAddress(xml->getString("EmailAddress"));
	mpx->setGroup2(xml->getString("Group2"));
	mpx->setHyperlink(xml->getString("Hyperlink"));
	mpx->setHyperlinkAddress(xml->getString("HyperlinkAddress"));
	mpx->setHyperlinkSubAddress(xml->getString("HyperlinkSubAddress"));
	mpx->setID(xml->getInt("ID"));
	mpx->setInitials(xml->getString("Initials"));
	mpx->setIsEnterprise(xml->getBool("IsEnterprise"));
	mpx->setIsGeneric(xml->getBool("IsGeneric"));
	mpx->setIsInactive(xml->getBool("IsInactive"));
	mpx->setIsNull(xml->getBool("IsNull"));
	//mpx->setLinkedFields();
	mpx->setMaterialLabel(xml->getString("MaterialLabel"));
	mpx->setMaxUnits(m_dataConverter.parseUnits(xml->getNumber("MaxUnits")));
	mpx->setName(xml->getString("Name"));
	if (xml->getString("Notes").GetLength() > 0)
	{
		mpx->setNotes(xml->getString("Notes"));
	}
	mpx->setNtAccount(xml->getString("NTAccount"));
	//mpx->setObjects();      
	mpx->setOvertimeCost(m_dataConverter.parseCurrency(xml->getNumber("OvertimeCost")));
	mpx->setOvertimeRate(m_dataConverter.parseRate(xml->getNumber("OvertimeRate")));
	mpx->setOvertimeRateUnits(m_dataConverter.parseTimeUnit(xml->getNumber("OvertimeRateFormat")));
	mpx->setOvertimeWork(m_dataConverter.parseDuration(
		m_projectFile, (TimeUnit)NULL, xml->getString("OvertimeWork")));
	mpx->setPeakUnits(m_dataConverter.parseUnits(xml->getNumber("PeakUnits")));
	mpx->setPercentWorkComplete(xml->getNumber("PercentWorkComplete"));
	mpx->setPhonetics(xml->getString("Phonetics"));
	mpx->setRegularWork(m_dataConverter.parseDuration(
		m_projectFile, (TimeUnit)NULL, xml->getString("RegularWork")));
	mpx->setRemainingCost(m_dataConverter.parseCurrency(xml->getNumber("RemainingCost")));
	mpx->setRemainingOvertimeCost(m_dataConverter.parseCurrency(xml->getNumber("RemainingOvertimeCost")));
	mpx->setRemainingWork(m_dataConverter.parseDuration(
		m_projectFile, (TimeUnit)NULL, xml->getString("RemainingWork")));
	mpx->setRemainingOvertimeWork(m_dataConverter.parseDuration(
		m_projectFile, (TimeUnit)NULL, xml->getString("RemainingOvertimeWork")));
	mpx->setStandardRate(m_dataConverter.parseRate(xml->getNumber("StandardRate")));
	mpx->setStandardRateUnits(m_dataConverter.parseTimeUnit(xml->getNumber("StandardRateFormat")));
	mpx->setSV(m_dataConverter.parseCurrency(xml->getNumber("SV")));
	mpx->setType((Resource2Type) xml->getInt("Type"));
	mpx->setUniqueID(xml->getInt("UID"));
	mpx->setWork(m_dataConverter.parseDuration(m_projectFile, (TimeUnit)NULL, xml->getString("Work")));
	mpx->setWorkGroup2((WorkGroup2) xml->getInt("WorkGroup2"));
	mpx->setWorkVariance(m_dataConverter.parseDurationInThousanthsOfMinutes(xml->getNumber("WorkVariance")));

	if (mpx->getType() == Resource2Type::RT_MATERIAL && xml->getBool("IsCostResource2"))
	{
		mpx->setType(Resource2Type::RT_COST);
	}

	readResource2ExtendedAttributes(xml, mpx);

	readResource2Baselines(xml, mpx);

	ProjectCalendarPtr calendar;
	if (calendarMap->Lookup(xml->getInt("CalendarUID"), calendar))
		mpx->setResource2Calendar(calendar);

	// ensure that we cache this value
	mpx->setOverAllocated(xml->getBool("OverAllocated"));

	readCostRateTables(mpx, xml->getRates());

	readAvailabilityTable(mpx, xml->getAvailabilityPeriods());

	m_projectFile->fireResource2ReadEvent(mpx);
}

/**
* Reads baseline values for the current resource->
*
* @param xmlResource2 MSPDI resource instance
* @param mpxjResource2 MPXJ resource instance
*/
void MSPDIReader::readResource2Baselines(std::shared_ptr<mspdi::schema::Resource2> xmlResource2, Resource2Ptr mpxjResource2)
{
	for (size_t i = 0; i < xmlResource2->getBaseline()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::Baseline> baseline = xmlResource2->getBaseline()->GetAt(i);
		int number = baseline->getInt("Number");

		NumberPtr cost = m_dataConverter.parseCurrency(baseline->getNumber("Cost"));
		DurationPtr work = m_dataConverter.parseDuration(
			m_projectFile, TimeUnit::HOURS, baseline->getString("Work"));

		if (number == 0)
		{
			mpxjResource2->setBaselineCost(cost);
			mpxjResource2->setBaselineWork(work);
		}
		else
		{
			mpxjResource2->setBaselineCost(number, cost);
			mpxjResource2->setBaselineWork(number, work);
		}
	}
}

/**
* This method processes any extended attributes associated with a resource->
*
* @param xml MSPDI resource instance
* @param mpx MPX resource instance
*/
void MSPDIReader::readResource2ExtendedAttributes(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx)
{
	for (size_t i = 0; i < xml->getExtendedAttribute()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::ExtendedAttribute> attrib = xml->getExtendedAttribute()->GetAt(i);
		int xmlFieldID = attrib->getInt("FieldID") & 0x0000FFFF;
		Resource2Field mpxFieldID = MPPResource2Field::getInstance(xmlFieldID);
		TimeUnit durationFormat = m_dataConverter.parseDurationTimeUnits(
			attrib->getNumber("DurationFormat"), (TimeUnit)NULL);
		m_dataConverter.parseExtendedAttribute(m_projectFile, mpx.get(), 
			attrib->getString("Value"), Resource2FieldClass::getInstance(mpxFieldID), durationFormat);
	}
}

/**
* Reads the cost rate tables from the file.
*
* @param resource parent resource
* @param rates XML cot rate tables
*/
void MSPDIReader::readCostRateTables(Resource2Ptr resource, std::shared_ptr<mspdi::schema::Rates> rates)
{
	if (rates == NULL)
	{
		CostRateTablePtr table (new CostRateTable());
		table->Add(CostRateTableEntry::getDEFAULT_ENTRY());
		resource->setCostRateTable(0, table);

		table.reset (new CostRateTable());
		table->Add(CostRateTableEntry::getDEFAULT_ENTRY());
		resource->setCostRateTable(1, table);

		table.reset(new CostRateTable());
		table->Add(CostRateTableEntry::getDEFAULT_ENTRY());
		resource->setCostRateTable(2, table);

		table.reset(new CostRateTable());
		table->Add(CostRateTableEntry::getDEFAULT_ENTRY());
		resource->setCostRateTable(3, table);

		table.reset(new CostRateTable());
		table->Add(CostRateTableEntry::getDEFAULT_ENTRY());
		resource->setCostRateTable(4, table);
	}
	else
	{
		//Set<CostRateTable*> tables = new HashSet<CostRateTable*>();
		Map2<CostRateTable*, int> tables;

		for (size_t i = 0; i < rates->getRate()->GetCount(); i ++)
		{
			std::shared_ptr<mspdi::schema::Rate> rate = rates->getRate()->GetAt(i);
			RatePtr standardRate = m_dataConverter.parseRate(rate->getNumber("StandardRate"));
			TimeUnit standardRateFormat = m_dataConverter.parseTimeUnit(rate->getNumber("StandardRateFormat"));
			RatePtr overtimeRate = m_dataConverter.parseRate(rate->getNumber("OvertimeRate"));
			TimeUnit overtimeRateFormat = m_dataConverter.parseTimeUnit(rate->getNumber("OvertimeRateFormat"));
			NumberPtr costPerUse = m_dataConverter.parseCurrency(rate->getNumber("CostPerUse"));
			DatePtr endDate = rate->getDate("RatesTo");

			CostRateTableEntryPtr entry (new CostRateTableEntry(
				standardRate, standardRateFormat, overtimeRate, overtimeRateFormat, costPerUse, endDate));

			int tableIndex = rate->getInt("RateTable");
			CostRateTablePtr table = resource->getCostRateTable(tableIndex);
			if (table == NULL)
			{
				table.reset (new CostRateTable());
				resource->setCostRateTable(tableIndex, table);
			}
			table->Add(entry);
			tables.put(table.get(), 0);
			//tables.add(table);
		}

		POSITION pos = tables.GetStartPosition();
		while (pos)
		{
			CostRateTable* table = tables.GetNextKey(pos);
			Collections::sort_object_list(*table);
		}
		tables.RemoveAll();
	}
}

/**
* Reads the availability table from the file.
*
* @param resource MPXJ resource instance
* @param periods MSPDI availability periods
*/
void MSPDIReader::readAvailabilityTable(Resource2Ptr resource, std::shared_ptr<mspdi::schema::AvailabilityPeriods> periods)
{
	if (periods == NULL) return;

	AvailabilityTablePtr table = resource->getAvailability();
	std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::AvailabilityPeriod>>> list = periods->getAvailabilityPeriod();

	for (size_t i = 0; i < list->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::AvailabilityPeriod> period = list->GetAt(i);
		DatePtr start = period->getDate("AvailableFrom");
		DatePtr end = period->getDate("AvailableTo");
		NumberPtr units = m_dataConverter.parseUnits(period->getNumber("AvailableUnits"));
		AvailabilityPtr availability (new Availability(start, end, units));
		table->Add(availability);
	}

	Collections::sort_object_list(*table);
}

/**
* This method extracts task data from an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIReader::readTasks(std::shared_ptr<mspdi::schema::Project> project)
{
	std::shared_ptr<mspdi::schema::Tasks> tasks = project->getTasks();
	if (tasks == NULL) return;

	for (size_t i = 0; i < tasks->getTask()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::Task> task = tasks->getTask()->GetAt(i);
		readTask(task);
	}

	for (size_t i = 0; i < tasks->getTask()->GetCount(); i++)
	{
		std::shared_ptr<mspdi::schema::Task> task = tasks->getTask()->GetAt(i);
		readPredecessors(task);
	}

	m_projectFile->updateStructure();
}

/**
* This method extracts data for a single task from an MSPDI file.
*
* @param xml Task* data
*/
void MSPDIReader::readTask(std::shared_ptr<mspdi::schema::Task> xml)
{
	Task* mpx = m_projectFile->addTask();
	mpx->setNull(xml->getBool("IsNull"));
	mpx->setID(xml->getInt("ID"));
	mpx->setUniqueID(xml->getInt("UID"));

	if (!mpx->getNull())
	{
		//
		// Set the Duration* format up front as this is required later
		//
		TimeUnit durationFormat = m_dataConverter.parseDurationTimeUnits(xml->getNumber("DurationFormat"));

		mpx->setActive(xml->getField("Active") == NULL ? true : xml->getBool("Active"));
		mpx->setActualCost(m_dataConverter.parseCurrency(xml->getNumber("ActualCost")));
		mpx->setActualDuration(m_dataConverter.parseDuration(m_projectFile, durationFormat, xml->getString("ActualDuration")));
		mpx->setActualFinish(xml->getDate("ActualFinish"));
		mpx->setActualOvertimeCost(m_dataConverter.parseCurrency(xml->getNumber("ActualOvertimeCost")));
		mpx->setActualOvertimeWork(m_dataConverter.parseDuration(m_projectFile, durationFormat, xml->getString("ActualOvertimeWork")));
		mpx->setActualOvertimeWorkProtected(m_dataConverter.parseDuration(
			m_projectFile, durationFormat, xml->getString("ActualOvertimeWorkProtected")));
		mpx->setActualStart(xml->getDate("ActualStart"));
		mpx->setActualWork(m_dataConverter.parseDuration(m_projectFile, durationFormat, xml->getString("ActualWork")));
		mpx->setActualWorkProtected(m_dataConverter.parseDuration(m_projectFile, durationFormat, xml->getString("ActualWorkProtected")));
		mpx->setACWP(m_dataConverter.parseCurrency(xml->getNumber("ACWP")));
		//mpx->setBaselineCost();
		//mpx->setBaselineDuration();
		//mpx->setBaselineFinish();
		//mpx->setBaselineStart();
		//mpx->setBaselineWork();
		//mpx->setBCWP();
		//mpx->setBCWS();
		mpx->setCalendar(getTaskCalendar(xml));
		//mpx->setConfirmed();
		mpx->setConstraintDate(xml->getDate("ConstraintDate"));
		mpx->setConstraintType(m_dataConverter.parseConstraintType(xml->getNumber("ConstraintType")));
		mpx->setContact(xml->getString("Contact"));
		mpx->setCost(m_dataConverter.parseCurrency(xml->getNumber("Cost")));
		//mpx->setCost1();
		//mpx->setCost2();
		//mpx->setCost3();
		//mpx->setCostVariance();
		mpx->setCreateDate(xml->getDate("CreateDate"));
		mpx->setCV(m_dataConverter.parseCurrency(xml->getNumber("CV")));
		mpx->setDeadline(xml->getDate("Deadline"));
		//mpx->setDelay();
		mpx->setDuration(m_dataConverter.parseDuration(m_projectFile, durationFormat, xml->getString("Duration")));
		mpx->setDurationText(xml->getString("DurationText"));
		//mpx->setDuration1();
		//mpx->setDuration2();
		//mpx->setDuration3();
		//mpx->setDurationVariance();
		mpx->setEarlyFinish(xml->getDate("EarlyFinish"));
		mpx->setEarlyStart(xml->getDate("EarlyStart"));
		mpx->setEarnedValueMethod(m_dataConverter.parseEarnedValueMethod(xml->getNumber("EarnedValueMethod")));
		mpx->setEffortDriven(xml->getBool("EffortDriven"));
		mpx->setEstimated(xml->getBool("Estimated"));
		mpx->setExternalTask(xml->getBool("ExternalTask"));
		mpx->setProject(xml->getString("ExternalTaskProject"));
		mpx->setFinish(xml->getDate("Finish"));
		mpx->setFinishText(xml->getString("FinishText"));
		//mpx->setFinish1();
		//mpx->setFinish2();
		//mpx->setFinish3();
		//mpx->setFinish4();
		//mpx->setFinish5();
		mpx->setFinishVariance(m_dataConverter.parseDurationInThousanthsOfMinutes(xml->getNumber("FinishVariance")));
		//mpx->setFixed();
		mpx->setFixedCost(m_dataConverter.parseCurrency(xml->getNumber("FixedCost")));
		mpx->setFixedCostAccrual((AccrueType)xml->getInt("FixedCostAccrual"));
		//mpx->setFlag1();
		//mpx->setFlag2();
		//mpx->setFlag3();
		//mpx->setFlag4();
		//mpx->setFlag5();
		//mpx->setFlag6();
		//mpx->setFlag7();
		//mpx->setFlag8();
		//mpx->setFlag9();
		//mpx->setFlag10();
		// This is not correct?
		mpx->setFreeSlack(m_dataConverter.parseDurationInThousanthsOfMinutes(xml->getNumber("FreeSlack")));
		mpx->setHideBar(xml->getBool("HideBar"));
		mpx->setHyperlink(xml->getString("Hyperlink"));
		mpx->setHyperlinkAddress(xml->getString("HyperlinkAddress"));
			mpx->setHyperlinkSubAddress(xml->getString("HyperlinkSubAddress"));

		mpx->setIgnoreResource2Calendar(xml->getBool("IgnoreResource2Calendar"));
		mpx->setLateFinish(xml->getDate("LateFinish"));
		mpx->setLateStart(xml->getDate("LateStart"));
		mpx->setLevelAssignments(xml->getBool("LevelAssignments"));
		mpx->setLevelingCanSplit(xml->getBool("LevelingCanSplit"));
		mpx->setLevelingDelayFormat(m_dataConverter.parseDurationTimeUnits(xml->getNumber("LevelingDelayFormat")));
		if (xml->getNumber("LevelingDelay") != NULL && mpx->getLevelingDelayFormat() != NULL)
		{
			double duration = xml->getNumber("LevelingDelay")->doubleValue();
			if (((int)duration) != 0)
			{
				mpx->setLevelingDelay(Duration2::convertUnits(duration / 10, TimeUnit::MINUTES, 
								mpx->getLevelingDelayFormat(), m_projectFile->getProjectHeader()));
			}
		}

		//mpx->setLinkedFields();
		//mpx->setMarked();
		mpx->setMilestone(xml->getBool("Milestone"));
		mpx->setName(xml->getString("Name"));
		if (xml->getString("Notes").GetLength() != 0)
		{
			mpx->setNotes(xml->getString("Notes"));
		}
		//mpx->setNumber1();
		//mpx->setNumber2();
		//mpx->setNumber3();
		//mpx->setNumber4();
		//mpx->setNumber5();
		//mpx->setObjects();      
		mpx->setOutlineLevel(xml->getInt("OutlineLevel"));
		mpx->setOutlineNumber(xml->getString("OutlineNumber"));
		mpx->setOverAllocated(xml->getBool("OverAllocated"));
		mpx->setOvertimeCost(m_dataConverter.parseCurrency(xml->getNumber("OvertimeCost")));
		mpx->setOvertimeWork(m_dataConverter.parseDuration(
			m_projectFile, durationFormat, xml->getString("OvertimeWork")));
		mpx->setPercentageComplete(xml->getNumber("PercentComplete"));
		mpx->setPercentageWorkComplete(xml->getNumber("PercentWorkComplete"));
		mpx->setPhysicalPercentComplete(xml->getInt("PhysicalPercentComplete"));
		mpx->setPreleveledFinish(xml->getDate("PreLeveledFinish"));
		mpx->setPreleveledStart(xml->getDate("PreLeveledStart"));
		mpx->setPriority(PriorityPtr(m_dataConverter.parsePriority(xml->getNumber("Priority"))));
		//mpx->setProject();
		mpx->setRecurring(xml->getBool("Recurring"));
		mpx->setRegularWork(m_dataConverter.parseDuration(
			m_projectFile, durationFormat, xml->getString("RegularWork")));
		mpx->setRemainingCost(m_dataConverter.parseCurrency(xml->getNumber("RemainingCost")));
		mpx->setRemainingDuration(m_dataConverter.parseDuration(
			m_projectFile, durationFormat, xml->getString("RemainingDuration")));
		mpx->setRemainingOvertimeCost(m_dataConverter.parseCurrency(xml->getNumber("RemainingOvertimeCost")));
		mpx->setRemainingOvertimeWork(m_dataConverter.parseDuration(
			m_projectFile, durationFormat, xml->getString("RemainingOvertimeWork")));
		mpx->setRemainingWork(m_dataConverter.parseDuration(
			m_projectFile, durationFormat, xml->getString("RemainingWork")));
		//mpx->setResource2Group2();
		//mpx->setResource2Initials();
		//mpx->setResource2Names();
		mpx->setResume(xml->getDate("Resume"));
		mpx->setResumeValid(xml->getBool("ResumeValid"));
		//mpx->setResumeNoEarlierThan();
		mpx->setRollup(xml->getBool("Rollup"));
		mpx->setStart(xml->getDate("Start"));
		mpx->setStartText(xml->getString("StartText"));
		//mpx->setStart1();
		//mpx->setStart2();
		//mpx->setStart3();
		//mpx->setStart4();
		//mpx->setStart5();
		mpx->setStartVariance(m_dataConverter.parseDurationInThousanthsOfMinutes(xml->getNumber("StartVariance")));
		mpx->setStop(xml->getDate("Stop"));
		mpx->setSubProject(xml->getBool("IsSubproject") ? SubProjectPtr(new SubProject()) : NULL);
		mpx->setSubprojectName(xml->getString("SubprojectName"));
		mpx->setSubprojectReadOnly(xml->getBool("IsSubprojectReadOnly"));
		//mpx->setSuccessors();
		mpx->setSummary(xml->getBool("Summary"));
		//mpx->setSV();
		//mpx->setText1();
		//mpx->setText2();
		//mpx->setText3();
		//mpx->setText4();
		//mpx->setText5();
		//mpx->setText6();
		//mpx->setText7();
		//mpx->setText8();
		//mpx->setText9();
		//mpx->setText10();
		mpx->setTaskMode(xml->getBool("Manual") ? TaskMode::MANUALLY_SCHEDULED : TaskMode::AUTO_SCHEDULED);
		mpx->setType((TaskType) xml->getInt("Type"));
		//mpx->setUpdateNeeded();
		mpx->setWBS(xml->getString("WBS"));
		mpx->setWBSLevel(xml->getString("WBSLevel"));
		mpx->setWork(m_dataConverter.parseDuration(m_projectFile, durationFormat, xml->getString("Work")));
		mpx->setWorkVariance(Duration2::getInstance(xml->getDouble("WorkVariance") / 1000, TimeUnit::MINUTES));

		// read last to ensure correct caching
		mpx->setTotalSlack(m_dataConverter.parseDurationInThousanthsOfMinutes(xml->getNumber("TotalSlack")));
		mpx->setCritical(xml->getBool("Critical"));

		readTaskExtendedAttributes(xml, mpx);

		readTaskBaselines(xml, mpx, durationFormat);

		if (mpx->getTaskMode() == TaskMode::MANUALLY_SCHEDULED)
		{
			mpx->setManualDuration(m_dataConverter.parseDuration(
				m_projectFile, durationFormat, xml->getString("ManualDuration")));
		}
	}

	m_projectFile->fireTaskReadEvent(mpx);
}

/**
* Reads baseline values for the current task->
*
* @param xmlTask MSPDI task instance
* @param mpxjTask MPXJ task instance
* @param durationFormat Duration* format to use
*/
void MSPDIReader::readTaskBaselines(std::shared_ptr<mspdi::schema::Task> xmlTask, Task* mpxjTask, TimeUnit durationFormat)
{
	for (size_t i = 0; i < xmlTask->getBaseline()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::Baseline> baseline = xmlTask->getBaseline()->GetAt(i);

		int number = baseline->getInt("Number");

		NumberPtr cost = m_dataConverter.parseCurrency(baseline->getNumber("Cost"));
		DurationPtr duration = m_dataConverter.parseDuration(m_projectFile, durationFormat, baseline->getString("Duration"));
		DatePtr finish = baseline->getDate("Finish");
		DatePtr start = baseline->getDate("Start");
		DurationPtr work = m_dataConverter.parseDuration(m_projectFile, TimeUnit::HOURS, baseline->getString("Work"));

		if (number == 0)
		{
			mpxjTask->setBaselineCost(cost);
			mpxjTask->setBaselineDuration(duration);
			mpxjTask->setBaselineFinish(finish);
			mpxjTask->setBaselineStart(start);
			mpxjTask->setBaselineWork(work);
		}
		else
		{
			mpxjTask->setBaselineCost(number, cost);
			mpxjTask->setBaselineDuration(number, duration);
			mpxjTask->setBaselineFinish(number, finish);
			mpxjTask->setBaselineStart(number, start);
			mpxjTask->setBaselineWork(number, work);
		}
	}
}

/**
* This method processes any extended attributes associated with a task->
*
* @param xml MSPDI task instance
* @param mpx MPX task instance
*/
void MSPDIReader::readTaskExtendedAttributes(std::shared_ptr<mspdi::schema::Task> xml, Task* mpx)
{
	for (size_t i = 0; i < xml->getExtendedAttribute()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::ExtendedAttribute> attrib = xml->getExtendedAttribute()->GetAt(i);

		int xmlFieldID = attrib->getInt("FieldID") & 0x0000FFFF;
		TaskField mpxFieldID = MPPTaskField::getInstance(xmlFieldID);
		TimeUnit durationFormat = m_dataConverter.parseDurationTimeUnits(
			attrib->getNumber("DurationFormat"), (TimeUnit)NULL);
		m_dataConverter.parseExtendedAttribute(m_projectFile, mpx, 
			attrib->getString("Value"), TaskFieldClass::getInstance(mpxFieldID), durationFormat);
	}
}

/**
* This method is used to retrieve the calendar associated
* with a task-> If no calendar is associated with a task, this method
* returns NULL.
*
* @param task MSPDI task
* @return calendar instance
*/
ProjectCalendarPtr MSPDIReader::getTaskCalendar(std::shared_ptr<mspdi::schema::Task> task)
{
	ProjectCalendarPtr calendar;

	BigIntegerPtr calendarID = task->getNumber("CalendarUID");
	if (calendarID)
	{
		calendar = m_projectFile->getCalendarByUniqueID(calendarID->intValue());
	}

	return (calendar);
}

/**
* This method extracts predecessor data from an MSPDI file.
*
* @param task Task* data
*/
void MSPDIReader::readPredecessors(std::shared_ptr<mspdi::schema::Task> task)
{
	NumberPtr uid = task->getNumber("UID");
	if (uid == NULL) return;

	Task* currTask = m_projectFile->getTaskByUniqueID(uid->intValue());
	if (currTask != NULL)
	{
		for (size_t i = 0; i < task->getPredecessorLink()->GetCount(); i ++)
		{
			std::shared_ptr<mspdi::schema::PredecessorLink> link = task->getPredecessorLink()->GetAt(i);

			readPredecessor(currTask, link);
		}
	}
}

/**
* This method extracts data for a single predecessor from an MSPDI file.
*
* @param currTask Current task object
* @param link Predecessor data
*/
void MSPDIReader::readPredecessor(Task* currTask, std::shared_ptr<mspdi::schema::PredecessorLink> link)
{
	BigIntegerPtr uid = link->getNumber("PredecessorUID");
	if (uid == NULL) return;

	//if (uid->intValue() == 17)
	//{
	//	int i = 0;
	//}

	Task* prevTask = m_projectFile->getTaskByUniqueID(uid->intValue());
	if (prevTask == NULL) return;

	RelationType type = RelationType::FINISH_START;
	if (link->getNumber("Type") != NULL)
	{
		type = (RelationType)link->getNumber("Type")->intValue();
	}

	int lag = 0;

	if (link->getNumber("LinkLag") != NULL)
	{
		lag = link->getNumber("LinkLag")->intValue() / 10;
	}

	TimeUnit lagUnits = m_dataConverter.parseDurationTimeUnits(link->getNumber("LagFormat"));
	DurationPtr lagDuration = Duration2::convertUnits(lag, 
		TimeUnit::MINUTES, lagUnits, m_projectFile->getProjectHeader());

	RelationPtr relation = currTask->addPredecessor(prevTask, type, lagDuration);
	m_projectFile->fireRelationReadEvent(relation);
}

/**
* This method extracts assignment data from an MSPDI file.
*
* @param project Root node of the MSPDI file
*/
void MSPDIReader::readAssignments(std::shared_ptr<mspdi::schema::Project> project)
{
	std::shared_ptr<mspdi::schema::Assignments> assignments = project->getAssignments();
	if (assignments == NULL) return;

	SplitTaskFactory splitFactory;// = new SplitTaskFactory();
	MSPDITimephasedWorkNormaliserPtr normaliser( new MSPDITimephasedWorkNormaliser());
	for (size_t i = 0; i < assignments->getAssignment()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::Assignment> assignment = assignments->getAssignment()->GetAt(i);
		readAssignment(assignment, &splitFactory, normaliser);
	}
}

/**
* This method extracts data for a single assignment from an MSPDI file.
*
* @param assignment Assignment data
* @param splitFactory split task handling
* @param normaliser timephased resource assignment normaliser
*/
void MSPDIReader::readAssignment(std::shared_ptr<mspdi::schema::Assignment> assignment,
	SplitTaskFactory* splitFactory, TimephasedWorkNormaliserPtr normaliser)
{
	BigIntegerPtr taskUID = assignment->getNumber("TaskUID");

	//_tprintf(L"readAssignment %s\n", taskUID->toString());

	BigIntegerPtr resourceUID = assignment->getNumber("Resource2UID");
	if (taskUID == NULL || resourceUID == NULL) return;

	Task* task = m_projectFile->getTaskByUniqueID(taskUID->intValue());
	Resource2Ptr resource = m_projectFile->getResource2ByUniqueID(resourceUID->intValue());

	//System.out.println(task);
	ProjectCalendarPtr calendar;
	if (resource != NULL)
	{
		calendar = resource->getResource2Calendar();
	}

	if (!calendar)
	{
		calendar = task->getCalendar();
	}

	if (!calendar)
	{
		calendar = m_projectFile->getCalendar();
	}

	std::shared_ptr<TimephasedWorkList> timephasedComplete = readTimephasedAssignment(calendar, assignment, 2);
	std::shared_ptr<TimephasedWorkList> timephasedPlanned = readTimephasedAssignment(calendar, assignment, 1);
	bool raw = true;

	if (isSplit(calendar, timephasedComplete) || isSplit(calendar, timephasedPlanned))
	{
		task->setSplits(DateRangeListPtr(new DateRangeList()));
		normaliser->normalise(calendar, timephasedComplete);
		normaliser->normalise(calendar, timephasedPlanned);
		splitFactory->processSplitData(task, timephasedComplete, timephasedPlanned);
		raw = false;
	}

	DefaultTimephasedWorkContainerPtr timephasedCompleteData (new DefaultTimephasedWorkContainer(
														calendar, normaliser, timephasedComplete, raw));
	DefaultTimephasedWorkContainerPtr timephasedPlannedData (new DefaultTimephasedWorkContainer(
														calendar, normaliser, timephasedPlanned, raw));

	if (task == NULL) return;

	Resource2AssignmentPtr mpx = task->addResource2Assignment(resource);

	mpx->setActualCost(m_dataConverter.parseCurrency(assignment->getNumber("ActualCost")));
	mpx->setActualFinish(assignment->getDate("ActualFinish"));
	mpx->setActualOvertimeCost(m_dataConverter.parseCurrency(assignment->getNumber("ActualOvertimeCost")));
	mpx->setActualOvertimeWork(m_dataConverter.parseDuration(m_projectFile, 
								TimeUnit::HOURS, assignment->getString("ActualOvertimeWork")));
	//assignment->getActualOvertimeWorkProtected()
	mpx->setActualStart(assignment->getDate("ActualStart"));
	mpx->setActualWork(m_dataConverter.parseDuration(m_projectFile, TimeUnit::HOURS, assignment->getString("ActualWork")));
	//assignment->getActualWorkProtected()
	mpx->setACWP(m_dataConverter.parseCurrency(assignment->getNumber("ACWP")));
	mpx->setBCWP(m_dataConverter.parseCurrency(assignment->getNumber("BCWP")));
	mpx->setBCWS(m_dataConverter.parseCurrency(assignment->getNumber("BCWS")));
	//assignment->getBookingType()
	mpx->setBudgetCost(m_dataConverter.parseCurrency(assignment->getNumber("BudgetCost")));
	mpx->setBudgetWork(m_dataConverter.parseDuration(m_projectFile, TimeUnit::HOURS, assignment->getString("BudgetWork")));
	mpx->setCost(m_dataConverter.parseCurrency(assignment->getNumber("Cost")));
	mpx->setCostRateTableIndex(assignment->getInt("CostRateTable"));
	mpx->setCreateDate(assignment->getDate("CreationDate"));
	mpx->setCV(m_dataConverter.parseCurrency(assignment->getNumber("CV")));
	mpx->setDelay(m_dataConverter.parseDurationInTenthsOfMinutes(assignment->getNumber("Delay")));
	mpx->setFinish(assignment->getDate("Finish"));
	mpx->setVariableRateUnits( assignment->getBool("HasFixedRateUnits") ? 
		(TimeUnit)NULL : m_dataConverter.parseTimeUnit(assignment->getNumber("RateScale")));
	mpx->setHyperlink(assignment->getString("Hyperlink"));
	mpx->setHyperlinkAddress(assignment->getString("HyperlinkAddress"));
		mpx->setHyperlinkSubAddress(assignment->getString("HyperlinkSubAddress"));
	mpx->setLevelingDelay(m_dataConverter.parseDurationInTenthsOfMinutes(
		m_projectFile->getProjectHeader(), assignment->getNumber("LevelingDelay"), 
		m_dataConverter.parseDurationTimeUnits(assignment->getNumber("LevelingDelayFormat"))));
	mpx->setNotes(assignment->getString("Notes"));
	mpx->setOvertimeCost(m_dataConverter.parseCurrency(assignment->getNumber("OvertimeCost")));
	mpx->setOvertimeWork(m_dataConverter.parseDuration(m_projectFile, TimeUnit::HOURS, assignment->getString("OvertimeWork")));
	mpx->setPercentageWorkComplete(assignment->getNumber("PercentWorkComplete"));
	//mpx->setPlannedCost();
	//mpx->setPlannedWork();
	mpx->setRegularWork(m_dataConverter.parseDuration(m_projectFile, TimeUnit::HOURS, assignment->getString("RegularWork")));
	mpx->setRemainingCost(m_dataConverter.parseCurrency(assignment->getNumber("RemainingCost")));
	mpx->setRemainingOvertimeCost(m_dataConverter.parseCurrency(assignment->getNumber("RemainingOvertimeCost")));
	mpx->setRemainingOvertimeWork(m_dataConverter.parseDuration(
		m_projectFile, TimeUnit::HOURS, assignment->getString("RemainingOvertimeWork")));
	mpx->setRemainingWork(m_dataConverter.parseDuration(
		m_projectFile, TimeUnit::HOURS, assignment->getString("RemainingWork")));
	//assignment->getResume()
	mpx->setStart(assignment->getDate("Start"));
	//assignment->getStop()
	mpx->setSV(m_dataConverter.parseCurrency(assignment->getNumber("SV")));
	mpx->setUniqueID(assignment->getNumber("UID"));

	mpx->setUnits(m_dataConverter.parseUnits(assignment->getNumber("Units")));
	mpx->setVAC(m_dataConverter.parseCurrency(assignment->getNumber("VAC")));
	mpx->setWork(m_dataConverter.parseDuration(m_projectFile, TimeUnit::HOURS, assignment->getString("Work")));
	mpx->setWorkContour((WorkContour) assignment->getInt("WorkContour"));

	mpx->setTimephasedActualWork(timephasedCompleteData);
	mpx->setTimephasedWork(timephasedPlannedData);

	readAssignmentExtendedAttributes(assignment, mpx);

	readAssignmentBaselines(assignment, mpx);

	// Read last to ensure caching works as expected
	mpx->setCostVariance(m_dataConverter.parseCurrency(assignment->getNumber("CostVariance")));
	mpx->setWorkVariance(m_dataConverter.parseDurationInThousanthsOfMinutes(
		m_projectFile->getProjectHeader(), assignment->getNumber("WorkVariance"), TimeUnit::HOURS));
	mpx->setStartVariance(m_dataConverter.parseDurationInTenthsOfMinutes(
		m_projectFile->getProjectHeader(), assignment->getNumber("StartVariance"), TimeUnit::DAYS));
	mpx->setFinishVariance(m_dataConverter.parseDurationInTenthsOfMinutes(
	m_projectFile->getProjectHeader(), assignment->getNumber("FinishVariance"), TimeUnit::DAYS));

	m_projectFile->fireAssignmentReadEvent(mpx);
}

/**
* Extracts assignment baseline data.
*
* @param assignment xml assignment
* @param mpx mpxj assignment
*/
void MSPDIReader::readAssignmentBaselines(
	std::shared_ptr<mspdi::schema::Assignment> assignment, Resource2AssignmentPtr mpx)
{
	for (size_t i = 0; i < assignment->getBaseline()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::Baseline> baseline = assignment->getBaseline()->GetAt(i);

		int number = baseline->getInt("Number");

		//baseline->getBCWP()
		//baseline->getBCWS()         
		NumberPtr cost = m_dataConverter.parseExtendedAttributeCurrency(baseline->getString("Cost"));
		DatePtr finish = m_dataConverter.parseExtendedAttributeDate (baseline->getString("Finish"));
		//baseline->getNumber()
		DatePtr start = m_dataConverter.parseExtendedAttributeDate (baseline->getString("Start"));
		DurationPtr work = m_dataConverter.parseDuration(m_projectFile, TimeUnit::HOURS, baseline->getString("Work"));

		if (number == 0)
		{
			mpx->setBaselineCost(cost);
			mpx->setBaselineFinish(finish);
			mpx->setBaselineStart(start);
			mpx->setBaselineWork(work);
		}
		else
		{
			mpx->setBaselineCost(number, cost);
			mpx->setBaselineWork(number, work);
			mpx->setBaselineStart(number, start);
			mpx->setBaselineFinish(number, finish);
		}
	}
}

/**
* This method processes any extended attributes associated with a
* resource assignment->
*
* @param xml MSPDI resource assignment instance
* @param mpx MPX task instance
*/
void MSPDIReader::readAssignmentExtendedAttributes(
	std::shared_ptr<mspdi::schema::Assignment> xml, Resource2AssignmentPtr mpx)
{
	for (size_t i = 0; i < xml->getExtendedAttribute()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::ExtendedAttribute> attrib = xml->getExtendedAttribute()->GetAt(i);

		int xmlFieldID = attrib->getInt("FieldID") & 0x0000FFFF;
		AssignmentField mpxFieldID = MPPAssignmentField::getInstance(xmlFieldID);
		TimeUnit durationFormat = m_dataConverter.parseDurationTimeUnits(attrib->getNumber("DurationFormat"), (TimeUnit)NULL);
		m_dataConverter.parseExtendedAttribute(m_projectFile, mpx.get(), attrib->getString("Value"), 
			AssignmentFieldClass::getInstance (mpxFieldID), durationFormat);
	}
}

/**
* Test to determine if this is a split task->
*
* @param calendar current calendar
* @param list timephased resource assignment list
* @return bool flag
*/
bool MSPDIReader::isSplit(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
	bool result = false;
	for (size_t i = 0; i < list->GetCount(); i ++)
	{
		TimephasedWorkPtr assignment = list->GetAt(i);

		if (calendar != NULL && assignment->getTotalAmount()->getDuration() == 0)
		{
			DurationPtr calendarWork = calendar->getWork(assignment->getStart(), 
									assignment->getFinish(), TimeUnit::MINUTES);
			if (calendarWork->getDuration() != 0)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}

/**
* Reads timephased assignment data.
*
* @param calendar current calendar
* @param assignment assignment data
* @param type flag indicating if this is planned or complete work
* @return list of timephased resource assignment instances
*/
std::shared_ptr<TimephasedWorkList> MSPDIReader::readTimephasedAssignment(
	ProjectCalendarPtr calendar, std::shared_ptr<mspdi::schema::Assignment> assignment, int type)
{
	std::shared_ptr<TimephasedWorkList> result (new TimephasedWorkList());

	for (size_t i = 0; i < assignment->getTimephasedData()->GetCount(); i ++)
	{
		std::shared_ptr<mspdi::schema::TimephasedDataType> item = assignment->getTimephasedData()->GetAt(i);

		if (item->getInt("Type") != type)
		{
			continue;
		}

		DatePtr startDate = item->getDate("Start");
		DatePtr finishDate = item->getDate("Finish");
		DurationPtr work = m_dataConverter.parseDuration(
			m_projectFile, TimeUnit::MINUTES, item->getString("Value"));
		if (work == NULL)
		{
			work = Duration2::getInstance(0, TimeUnit::MINUTES);
		}
		else
		{
			work = Duration2::getInstance(NumberHelper::truncate(work->getDuration(), 2), TimeUnit::MINUTES);
		}

		TimephasedWorkPtr tra (new TimephasedWork());
		tra->setStart(startDate);
		tra->setFinish(finishDate);
		tra->setTotalAmount(work);

		result->Add(tra);
	}

	return result;
}

/**
* Sets a flag indicating that this class will attempt to correct
* and read XML which is not compliant with the XML Schema. This
* behaviour matches that of Microsoft Project* when reading the
* same data.
*
* @param flag input compatibility flag
*/
void MSPDIReader::setMicrosoftProjectCompatibleInput(bool flag)
{
	m_compatibleInput = flag;
}

/**
* Retrieves a flag indicating that this class will attempt to correct
* and read XML which is not compliant with the XML Schema. This
* behaviour matches that of Microsoft Project* when reading the
* same data.
*
* @return Boolean flag
*/
bool MSPDIReader::getMicrosoftProjectCompatibleInput()
{
	return (m_compatibleInput);
}
