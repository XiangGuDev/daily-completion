#include "stdafx.h"
#include "Project.h"
#include "..\DatatypeConverter.h"
#include "..\..\common\NumberHelper.h"

namespace mspdi{
	namespace schema{
		///////////////////////////////////////////////////////////////////////////////////////////////////
		// NewXmlAdapterT
		//
		template<class T>
		class NewXmlAdapterT : public NewXmlAdapter
		{
		public:
			virtual Object2* NewObject(TiXmlElement* element)
			{
				return new T(element);
			}
		};

		static void init_project_objects()
		{
			FieldValue::registerType("OutlineCodes", new NewXmlAdapterT<OutlineCodes>());
			FieldValue::registerType("WBSMasks", new NewXmlAdapterT<WBSMasks>());
			FieldValue::registerType("ExtendedAttributes", new NewXmlAdapterT<ExtendedAttributes>());
			FieldValue::registerType("Calendars", new NewXmlAdapterT<Calendars>());
			FieldValue::registerType("Tasks", new NewXmlAdapterT<Tasks>());
			FieldValue::registerType("Baseline", new NewXmlAdapterT<Baseline>());
			FieldValue::registerType("Resource2s", new NewXmlAdapterT<Resource2s>());
			FieldValue::registerType("Assignments", new NewXmlAdapterT<Assignments>());
			FieldValue::registerType("ExtendedAttribute", new NewXmlAdapterT<ExtendedAttribute>());
			FieldValue::registerType("Calendar", new NewXmlAdapterT<Calendar>());
			FieldValue::registerType("WeekDays", new NewXmlAdapterT<WeekDays>());
			FieldValue::registerType("Exceptions", new NewXmlAdapterT<Exceptions>());
			FieldValue::registerType("WorkWeeks", new NewXmlAdapterT<WorkWeeks>());
			FieldValue::registerType("Assignment", new NewXmlAdapterT<Assignment>());
			FieldValue::registerType("TimePeriod", new NewXmlAdapterT<TimePeriod>());
			FieldValue::registerType("WorkingTimes", new NewXmlAdapterT<WorkingTimes>());

			FieldValue::registerType("Task", new NewXmlAdapterT<Task>());
			FieldValue::registerType("AvailabilityPeriod", new NewXmlAdapterT<AvailabilityPeriod>());
			FieldValue::registerType("AvailabilityPeriods", new NewXmlAdapterT<AvailabilityPeriods>());
			FieldValue::registerType("Rate", new NewXmlAdapterT<Rate>());
			FieldValue::registerType("Rates", new NewXmlAdapterT<Rates>());
			FieldValue::registerType("Resource2", new NewXmlAdapterT<Resource2>());
			FieldValue::registerType("Mask", new NewXmlAdapterT<Mask>());
			FieldValue::registerType("OutlineCode", new NewXmlAdapterT<OutlineCode>());
			FieldValue::registerType("Value", new NewXmlAdapterT<Value>());
			FieldValue::registerType("WeekDay", new NewXmlAdapterT<WeekDay>());
			FieldValue::registerType("Exception", new NewXmlAdapterT<Exception>());
			FieldValue::registerType("WorkingTime", new NewXmlAdapterT<WorkingTime>());
			FieldValue::registerType("WorkWeek", new NewXmlAdapterT<WorkWeek>());
			FieldValue::registerType("PredecessorLink", new NewXmlAdapterT<PredecessorLink>());
			FieldValue::registerType("TimephasedData", new NewXmlAdapterT<TimephasedDataType>());
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
		// Project
		//
		Project::Project(TiXmlElement* element) :FieldValue(element)
		{
			// 注册类对象
			if (!FieldValue::isRegisterType("Task"))
			{
				init_project_objects();
			}
		}

		// 获取有序的字段名称列表
		LPCTSTR* Project::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"SaveVersion", // required = true", // BigInteger saveVersion;
				L"UID", // String uid;
				L"Name", // String name;
				L"Title", // String title;
				L"Subject", // String subject;
				L"Category", // String category;
				L"Company", // String company;
				L"Manager", // String manager;
				L"Author", // String author;
				L"CreationDate", // type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType( L"dateTime", // java.util.Calendar creationDate;
				L"Revision", // BigInteger revision;
				L"LastSaved", // type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType( L"dateTime", // java.util.Calendar lastSaved;
				L"ScheduleFromStart", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean scheduleFromStart;
				L"StartDate", // type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType( L"dateTime", // java.util.Calendar startDate;
				L"FinishDate", // type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType( L"dateTime", // java.util.Calendar finishDate;
				L"FYStartDate", // BigInteger fyStartDate;
				L"CriticalSlackLimit", // BigInteger criticalSlackLimit;
				L"CurrencyDigits", // BigInteger currencyDigits;
				L"CurrencySymbol", // String currencySymbol;
				L"CurrencyCode", // required = true", // String currencyCode;
				L"CurrencySymbolPosition", // type = String.class) @XmlJavaTypeAdapter(Adapter6.class", // CurrencySymbolPosition currencySymbolPosition;
				L"CalendarUID", // BigInteger calendarUID;
				L"DefaultStartTime", // type = String.class) @XmlJavaTypeAdapter(Adapter3.class) @XmlSchemaType( L"time", // java.util.Calendar defaultStartTime;
				L"DefaultFinishTime", // type = String.class) @XmlJavaTypeAdapter(Adapter3.class) @XmlSchemaType( L"time", // java.util.Calendar defaultFinishTime;
				L"MinutesPerDay", // BigInteger minutesPerDay;
				L"MinutesPerWeek", // BigInteger minutesPerWeek;
				L"DaysPerMonth", // BigInteger daysPerMonth;
				L"DefaultTaskType", // type = String.class, defaultValue = L"1", //) @XmlJavaTypeAdapter(Adapter7.class", // TaskType defaultTaskType;
				L"DefaultFixedCostAccrual", // type = String.class) @XmlJavaTypeAdapter(Adapter8.class", // AccrueType defaultFixedCostAccrual;
				L"DefaultStandardRate", // type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType( L"float", // BigDecimal defaultStandardRate;
				L"DefaultOvertimeRate", // type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType( L"float", // BigDecimal defaultOvertimeRate;
				L"DurationFormat", // BigInteger durationFormat;
				L"WorkFormat", // BigInteger workFormat;
				L"EditableActualCosts", // type = String.class, defaultValue = L"false", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean editableActualCosts;
				L"HonorConstraints", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean honorConstraints;
				L"EarnedValueMethod", // BigInteger earnedValueMethod;
				L"InsertedProjectsLikeSummary", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean insertedProjectsLikeSummary;
				L"MultipleCriticalPaths", // type = String.class, defaultValue = L"false", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean multipleCriticalPaths;
				L"NewTasksEffortDriven", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean newTasksEffortDriven;
				L"NewTasksEstimated", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean newTasksEstimated;
				L"SplitsInProgressTasks", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean splitsInProgressTasks;
				L"SpreadActualCost", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean spreadActualCost;
				L"SpreadPercentComplete", // type = String.class, defaultValue = L"false", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean spreadPercentComplete;
				L"TaskUpdatesResource2", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean taskUpdatesResource2;
				L"FiscalYearStart", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean fiscalYearStart;
				L"WeekStartDay", // BigInteger weekStartDay;
				L"MoveCompletedEndsBack", // type = String.class, defaultValue = L"false", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean moveCompletedEndsBack;
				L"MoveRemainingStartsBack", // type = String.class, defaultValue = L"false", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean moveRemainingStartsBack;
				L"MoveRemainingStartsForward", // type = String.class, defaultValue = L"false", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean moveRemainingStartsForward;
				L"MoveCompletedEndsForward", // type = String.class, defaultValue = L"false", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean moveCompletedEndsForward;
				L"BaselineForEarnedValue", // BigInteger baselineForEarnedValue;
				L"AutoAddNewResource2sAndTasks", // type = String.class, defaultValue = L"true", //) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean autoAddNewResource2sAndTasks;
				L"StatusDate", // type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType( L"dateTime", // java.util.Calendar statusDate;
				L"CurrentDate", // type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType( L"dateTime", // java.util.Calendar currentDate;
				L"MicrosoftProjectServerURL", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean microsoftProjectServerURL;
				L"Autolink", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean autolink;
				L"NewTaskStartDate", // BigInteger newTaskStartDate;
				L"DefaultTaskEVMethod", // BigInteger defaultTaskEVMethod;
				L"ProjectExternallyEdited", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean projectExternallyEdited;
				L"ExtendedCreationDate", // type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType( L"dateTime", // java.util.Calendar extendedCreationDate;
				L"ActualsInSync", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean actualsInSync;
				L"RemoveFileProperties", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean removeFileProperties;
				L"AdminProject", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean adminProject;
				L"BaslineCalendar", // String baslineCalendar;
				L"NewTasksAreManual", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean newTasksAreManual;
				L"UpdateManuallyScheduledTasksWhenEditingLinks", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean updateManuallyScheduledTasksWhenEditingLinks;
				L"KeepTaskOnNearestWorkingTimeWhenMadeAutoScheduled", // type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType( L"boolean", // Boolean keepTaskOnNearestWorkingTimeWhenMadeAutoScheduled;
				L"OutlineCodes", // Project.OutlineCodes outlineCodes;
				L"WBSMasks", // Project.WBSMasks wbsMasks;
				L"ExtendedAttributes", // Project.ExtendedAttributes extendedAttributes;
				L"Calendars", // Project.Calendars calendars;
				L"Tasks", // Project.Tasks tasks;
				L"Resource2s", // Project.Resource2s resources;
				L"Assignments", // Project.Assignments assignments;
				NULL
			};

			return fieldList;
		}

		std::shared_ptr<OutlineCodes> Project::getOutlineCodes()
		{
			return std::dynamic_pointer_cast<OutlineCodes>(getChildObject("OutlineCodes"));
		}

		std::shared_ptr<WBSMasks> Project::getWBSMasks()
		{
			return std::dynamic_pointer_cast<WBSMasks>(getChildObject("WBSMasks"));
		}

		std::shared_ptr<ExtendedAttributes> Project::getExtendedAttributes()
		{
			return std::dynamic_pointer_cast<ExtendedAttributes>(getChildObject("ExtendedAttributes"));
		}

		std::shared_ptr<Calendars> Project::getCalendars()
		{
			return std::dynamic_pointer_cast<Calendars>(getChildObject("Calendars"));
		}

		std::shared_ptr<Tasks> Project::getTasks()
		{
			return std::dynamic_pointer_cast<Tasks>(getChildObject("Tasks"));
		}

		std::shared_ptr<Resource2s> Project::getResource2s()
		{
			return std::dynamic_pointer_cast<Resource2s>(getChildObject("Resource2s"));
		}

		std::shared_ptr<Assignments> Project::getAssignments()
		{
			return std::dynamic_pointer_cast<Assignments>(getChildObject("Assignments"));
		}

		Assignments::Assignments(TiXmlElement* element) :FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Assignment>>> Assignments::getAssignment()
		{
			return getChildList<Assignment>("Assignment");
		}

		std::shared_ptr<ObjectList<std::shared_ptr<TimephasedDataType>>> Assignments::getTimephasedData()
		{
			return getChildList<TimephasedDataType>("TimephasedData");
		}

		Assignment::Assignment(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* Assignment::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"UID",// required = true) protected BigInteger uid;
				L"TaskUID",// protected BigInteger taskUID;
				L"Resource2UID",// protected BigInteger resourceUID;
				L"PercentWorkComplete",// protected BigInteger percentWorkComplete;
				L"ActualCost",// protected BigDecimal actualCost;
				L"ActualFinish",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar actualFinish;
				L"ActualOvertimeCost",// protected BigDecimal actualOvertimeCost;
				L"ActualOvertimeWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualOvertimeWork;
				L"ActualStart",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar actualStart;
				L"ActualWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualWork;
				L"ACWP",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal acwp;
				L"Confirmed",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean confirmed;
				L"Cost",// protected BigDecimal cost;
				L"CostRateTable",// protected BigInteger costRateTable;
				L"CostVariance",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal costVariance;
				L"CV",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal cv;
				L"Delay",// protected BigInteger delay;
				L"Finish",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar finish;
				L"FinishVariance",// protected BigInteger finishVariance;
				L"Hyperlink",// protected String hyperlink;
				L"HyperlinkAddress",// protected String hyperlinkAddress;
				L"HyperlinkSubAddress",// protected String hyperlinkSubAddress;
				L"WorkVariance",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal workVariance;
				L"HasFixedRateUnits",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean hasFixedRateUnits;
				L"FixedMaterial",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean fixedMaterial;
				L"LevelingDelay",// protected BigInteger levelingDelay;
				L"LevelingDelayFormat",// protected BigInteger levelingDelayFormat;
				L"LinkedFields",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean linkedFields;
				L"Milestone",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean milestone;
				L"Notes",// protected String notes;
				L"Overallocated",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean overallocated;
				L"OvertimeCost",// protected BigDecimal overtimeCost;
				L"OvertimeWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String overtimeWork;
				L"PeakUnits",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal peakUnits;
				L"RateScale",// protected BigInteger rateScale;
				L"RegularWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String regularWork;
				L"RemainingCost",// protected BigDecimal remainingCost;
				L"RemainingOvertimeCost",// protected BigDecimal remainingOvertimeCost;
				L"RemainingOvertimeWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String remainingOvertimeWork;
				L"RemainingWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String remainingWork;
				L"ResponsePending",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean responsePending;
				L"Start",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar start;
				L"Stop",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar stop;
				L"Resume",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar resume;
				L"StartVariance",// protected BigInteger startVariance;
				L"Summary",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean summary;
				L"SV",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal sv;
				L"Units",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal units;
				L"UpdateNeeded",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean updateNeeded;
				L"VAC",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal vac;
				L"Work",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String work;
				L"WorkContour",// type = String.class) @XmlJavaTypeAdapter(Adapter9.class) protected WorkContour workContour;
				L"BCWS",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal bcws;
				L"BCWP",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal bcwp;
				L"BookingType",// protected BigInteger bookingType;
				L"ActualWorkProtected",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualWorkProtected;
				L"ActualOvertimeWorkProtected",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualOvertimeWorkProtected;
				L"CreationDate",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar creationDate;
				L"AssnOwner",// protected String assnOwner;
				L"AssnOwnerGuid",// protected String assnOwnerGuid;
				L"BudgetCost",// protected BigDecimal budgetCost;
				L"BudgetWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String budgetWork;
				L"ExtendedAttribute",// protected ObjectList<Project.Assignments.Assignment.ExtendedAttribute> extendedAttribute;
				L"Baseline",// protected ObjectList<Project.Assignments.Assignment.Baseline> baseline;
				NULL
			};
			return fieldList;
		}

		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> Assignment::getExtendedAttribute()
		{
			return getChildList<ExtendedAttribute>("ExtendedAttribute");
		}

		std::shared_ptr<ObjectList<std::shared_ptr<Baseline>>> Assignment::getBaseline()
		{
			return getChildList<Baseline>("Baseline");
		}

		std::shared_ptr<ObjectList<std::shared_ptr<TimephasedDataType>>> Assignment::getTimephasedData()
		{
			return getChildList<TimephasedDataType>("TimephasedData");
		}

		Baseline::Baseline(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* Baseline::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"TimephasedData",// protected ObjectList<TimephasedDataType> timephasedData;
				L"Number",// protected BigInteger number;
				L"Interim",// type = String.class, defaultValue = "false",// @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean interim;
				L"Start",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar start;
				L"Finish",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar finish;
				L"Duration",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String duration;
				L"DurationFormat",// protected BigInteger durationFormat;
				L"EstimatedDuration",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean estimatedDuration;
				L"Work",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String work;
				L"Cost",// protected BigDecimal cost;
				L"BCWS",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal bcws;
				L"BCWP",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal bcwp;
				L"FixedCost",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal fixedCost;
				NULL
			};

			return fieldList;
		}

		Calendars::Calendars(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Calendar>>> Calendars::getCalendar()
		{
			return getChildList<Calendar>("Calendar");
		}

		Calendar::Calendar(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* Calendar::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"UID",// required = true) protected BigInteger uid;
				L"Name",//) protected String name;
				L"IsBaseCalendar",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean") protected Boolean isBaseCalendar;
				L"IsBaselineCalendar",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean") protected Boolean isBaselineCalendar;
				L"BaseCalendarUID",//) protected BigInteger baseCalendarUID;
				L"WeekDays",//) protected Project.Calendars.Calendar.WeekDays weekDays;
				L"Exceptions",//) protected Project.Calendars.Calendar.Exceptions exceptions;
				L"WorkWeeks",//) protected Project.Calendars.Calendar.WorkWeeks workWeeks;
				NULL
			};

			return fieldList;
		}

		std::shared_ptr<WeekDays> Calendar::getWeekDays()
		{
			return std::dynamic_pointer_cast<WeekDays>(getChildObject("WeekDays"));
		}

		std::shared_ptr<Exceptions> Calendar::getExceptions()
		{
			return std::dynamic_pointer_cast<Exceptions>(getChildObject("Exceptions"));
		}

		std::shared_ptr<WorkWeeks> Calendar::getWorkWeeks()
		{
			return std::dynamic_pointer_cast<WorkWeeks>(getChildObject("WorkWeeks"));
		}

		Exceptions::Exceptions(TiXmlElement* element) :FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Exception>>> Exceptions::getException()
		{
			return getChildList<Exception>("Exception");
		}


		Exception::Exception(TiXmlElement* element) :FieldValue(element)
		{

		}

		std::shared_ptr<TimePeriod> Exception::getTimePeriod()
		{
			return std::dynamic_pointer_cast<TimePeriod>(getChildObject("TimePeriod"));
		}

		std::shared_ptr<WorkingTimes> Exception::getWorkingTimes()
		{
			return std::dynamic_pointer_cast<WorkingTimes>(getChildObject("WorkingTimes"));
		}

		WeekDays::WeekDays(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<WeekDay>>> WeekDays::getWeekDay()
		{
			return getChildList<WeekDay>("WeekDay");
		}

		WeekDay::WeekDay(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* WeekDay::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"DayType",// required = true) protected BigInteger dayType;
				L"DayWorking",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean") protected Boolean dayWorking;
				L"TimePeriod",//) protected Project.Calendars.Calendar.WeekDays.WeekDay.TimePeriod timePeriod;
				L"WorkingTimes",//) protected Project.Calendars.Calendar.WeekDays.WeekDay.WorkingTimes workingTimes;
				NULL
			};

			return fieldList;
		}

		std::shared_ptr<TimePeriod> WeekDay::getTimePeriod()
		{
			return std::dynamic_pointer_cast<TimePeriod>(getChildObject("TimePeriod"));
		}

		std::shared_ptr<WorkingTimes> WeekDay::getWorkingTimes()
		{
			return std::dynamic_pointer_cast<WorkingTimes>(getChildObject("WorkingTimes"));
		}

		TimePeriod::TimePeriod(TiXmlElement* element) :FieldValue(element)
		{

		}

		DatePtr TimePeriod::getFromDate()
		{
			return getDate("FromDate");
		}

		DatePtr TimePeriod::getToDate()
		{
			return getDate("ToDate");
		}

		WorkingTimes::WorkingTimes(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<WorkingTime>>> WorkingTimes::getWorkingTime()
		{
			return getChildList<WorkingTime>("WorkingTime");
		}


		WorkingTime::WorkingTime(TiXmlElement* element) :FieldValue(element)
		{

		}

		DatePtr WorkingTime::getFromTime()
		{
			return getTime("FromTime");
		}

		DatePtr WorkingTime::getToTime()
		{
			return getTime("ToTime");
		}

		WorkWeeks::WorkWeeks(TiXmlElement* element) :FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<WorkWeek>>> WorkWeeks::getWorkWeek()
		{
			return getChildList<WorkWeek>("WorkWeek");
		}

		WorkWeek::WorkWeek(TiXmlElement* element) :FieldValue(element)
		{

		}

		std::shared_ptr<TimePeriod> WorkWeek::getTimePeriod()
		{
			return std::dynamic_pointer_cast<TimePeriod>(getChildObject("TimePeriod"));
		}

		std::shared_ptr<WeekDays> WorkWeek::getWeekDays()
		{
			return std::dynamic_pointer_cast<WeekDays>(getChildObject("WeekDays"));
		}

		CString WorkWeek::getName()
		{
			return getString("Name");
		}

		ExtendedAttributes::ExtendedAttributes(TiXmlElement* element) :FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> ExtendedAttributes::getExtendedAttribute()
		{
			return getChildList<ExtendedAttribute>("ExtendedAttribute");
		}

		ExtendedAttribute::ExtendedAttribute(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* ExtendedAttribute::getFieldList()
		{
			static LPCTSTR fieldList[] =
			{
				L"FieldID",//) protected String fieldID;
				L"Value",//) protected String value;
				L"ValueGUID",//) protected BigInteger valueGUID;
				L"DurationFormat",//) protected BigInteger durationFormat;
				NULL
			};
			return fieldList;
		}

		ValueList::ValueList(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Value>>> ValueList::getValue()
		{
			return getChildList<Value>("Value");
		}


		Value::Value(TiXmlElement* element) :FieldValue(element)
		{

		}

		OutlineCodes::OutlineCodes(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<OutlineCode>>> OutlineCodes::getOutlineCode()
		{
			return getChildList<OutlineCode>("OutlineCode");
		}

		OutlineCode::OutlineCode(TiXmlElement* element) :FieldValue(element)
		{

		}

		Masks::Masks(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Mask>>> Masks::getMask()
		{
			return getChildList<Mask>("Mask");
		}

		Mask::Mask(TiXmlElement* element) :FieldValue(element)
		{

		}

		Resource2s::Resource2s(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Resource2>>> Resource2s::getResource2()
		{
			return getChildList<Resource2>("Resource2");
		}


		Resource2::Resource2(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* Resource2::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"UID",// required = true, type = String.class) @XmlJavaTypeAdapter(Adapter10.class) @XmlSchemaType(name = "integer",// protected Integer uid;
				L"ID",// protected BigInteger id;
				L"Name",// protected String name;
				L"Type",// type = String.class) @XmlJavaTypeAdapter(Adapter11.class) protected Resource2Type type;
				L"IsNull",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean isNull;
				L"Initials",// protected String initials;
				L"Phonetics",// protected String phonetics;
				L"NTAccount",// protected String ntAccount;
				L"MaterialLabel",// protected String materialLabel;
				L"Code",// protected String code;
				L"Group2",// protected String group;
				L"WorkGroup2",// type = String.class) @XmlJavaTypeAdapter(Adapter12.class) protected WorkGroup2 workGroup2;
				L"EmailAddress",// protected String emailAddress;
				L"Hyperlink",// protected String hyperlink;
				L"HyperlinkAddress",// protected String hyperlinkAddress;
				L"HyperlinkSubAddress",// protected String hyperlinkSubAddress;
				L"MaxUnits",// type = String.class, defaultValue = "1.0",// @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal maxUnits;
				L"PeakUnits",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal peakUnits;
				L"OverAllocated",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean overAllocated;
				L"AvailableFrom",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar availableFrom;
				L"AvailableTo",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar availableTo;
				L"Start",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar start;
				L"Finish",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar finish;
				L"CanLevel",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean canLevel;
				L"AccrueAt",// type = String.class) @XmlJavaTypeAdapter(Adapter13.class) protected AccrueType accrueAt;
				L"Work",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String work;
				L"RegularWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String regularWork;
				L"OvertimeWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String overtimeWork;
				L"ActualWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualWork;
				L"RemainingWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String remainingWork;
				L"ActualOvertimeWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualOvertimeWork;
				L"RemainingOvertimeWork",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String remainingOvertimeWork;
				L"PercentWorkComplete",// protected BigInteger percentWorkComplete;
				L"StandardRate",// protected BigDecimal standardRate;
				L"StandardRateFormat",// protected BigInteger standardRateFormat;
				L"Cost",// protected BigDecimal cost;
				L"OvertimeRate",// protected BigDecimal overtimeRate;
				L"OvertimeRateFormat",// protected BigInteger overtimeRateFormat;
				L"OvertimeCost",// protected BigDecimal overtimeCost;
				L"CostPerUse",// protected BigDecimal costPerUse;
				L"ActualCost",// protected BigDecimal actualCost;
				L"ActualOvertimeCost",// protected BigDecimal actualOvertimeCost;
				L"RemainingCost",// protected BigDecimal remainingCost;
				L"RemainingOvertimeCost",// protected BigDecimal remainingOvertimeCost;
				L"WorkVariance",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal workVariance;
				L"CostVariance",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal costVariance;
				L"SV",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal sv;
				L"CV",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal cv;
				L"ACWP",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal acwp;
				L"CalendarUID",// protected BigInteger calendarUID;
				L"Notes",// protected String notes;
				L"BCWS",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal bcws;
				L"BCWP",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float",// protected BigDecimal bcwp;
				L"IsGeneric",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean isGeneric;
				L"IsInactive",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean isInactive;
				L"IsEnterprise",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean isEnterprise;
				L"BookingType",// type = String.class) @XmlJavaTypeAdapter(Adapter14.class) protected BookingType bookingType;
				L"ActualWorkProtected",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualWorkProtected;
				L"ActualOvertimeWorkProtected",// @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration",// protected String actualOvertimeWorkProtected;
				L"ActiveDirectoryGUID",// protected String activeDirectoryGUID;
				L"CreationDate",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime",// protected java.util.Calendar creationDate;
				L"ExtendedAttribute",// protected ObjectList<Project.Resource2s.Resource2.ExtendedAttribute> extendedAttribute;
				L"Baseline",// protected ObjectList<Project.Resource2s.Resource2.Baseline> baseline;
				L"OutlineCode",// protected ObjectList<Project.Resource2s.Resource2.OutlineCode> outlineCode;
				L"IsCostResource2",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean isCostResource2;
				L"AssnOwner",// protected String assnOwner;
				L"AssnOwnerGuid",// protected String assnOwnerGuid;
				L"IsBudget",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean",// protected Boolean isBudget;
				L"AvailabilityPeriods",// protected Project.Resource2s.Resource2.AvailabilityPeriods availabilityPeriods;
				L"Rates",// protected Project.Resource2s.Resource2.Rates rates;
				L"TimephasedData",// protected ObjectList<TimephasedDataType> timephasedData;
				NULL
			};
			return fieldList;
		}

		std::shared_ptr<AvailabilityPeriods> Resource2::getAvailabilityPeriods()
		{
			return std::dynamic_pointer_cast<AvailabilityPeriods>(getChildObject("AvailabilityPeriods"));
		}

		std::shared_ptr<Rates> Resource2::getRates()
		{
			return std::dynamic_pointer_cast<Rates>(getChildObject("Rates"));
		}

		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> Resource2::getExtendedAttribute()
		{
			return getChildList<ExtendedAttribute>("ExtendedAttribute");
		}

		std::shared_ptr<ObjectList<std::shared_ptr<Baseline>>> Resource2::getBaseline()
		{
			return getChildList<Baseline>("Baseline");
		}

		Rates::Rates(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Rate>>> Rates::getRate()
		{
			return getChildList<Rate>("Rate");
		}

		Rate::Rate(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* Rate::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"RatesFrom",// required = true, type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime") protected java.util.Calendar ratesFrom;
				L"RatesTo",// required = true, type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime") protected java.util.Calendar ratesTo;
				L"RateTable",//) protected BigInteger rateTable;
				L"StandardRate",//) protected BigDecimal standardRate;
				L"StandardRateFormat",//) protected BigInteger standardRateFormat;
				L"OvertimeRate",//) protected BigDecimal overtimeRate;
				L"OvertimeRateFormat",//) protected BigInteger overtimeRateFormat;
				L"CostPerUse",//) protected BigDecimal costPerUse;
				NULL
			};
			return fieldList;
		}

		AvailabilityPeriods::AvailabilityPeriods(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<AvailabilityPeriod>>> AvailabilityPeriods::getAvailabilityPeriod()
		{
			return getChildList<AvailabilityPeriod>("AvailabilityPeriod");
		}

		AvailabilityPeriod::AvailabilityPeriod(TiXmlElement* element) :FieldValue(element)
		{

		}

		// 获取有序的字段名称列表
		LPCTSTR* AvailabilityPeriod::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"AvailableFrom",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime") protected java.util.Calendar availableFrom;
				L"AvailableTo",// type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime") protected java.util.Calendar availableTo;
				L"AvailableUnits",// type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float") protected BigDecimal availableUnits;
				NULL
			};
			return fieldList;
		}

		Tasks::Tasks(TiXmlElement* element) : FieldValue(element)
		{

		}

		std::shared_ptr<ObjectList<std::shared_ptr<Task>>> Tasks::getTask()
		{
			return getChildList<Task>("Task");
		}


		Task::Task(TiXmlElement* element) :FieldValue(element)
		{
		}

		// 获取有序的字段名称列表
		LPCTSTR* Task::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"UID", //required = true, type = String.class) @XmlJavaTypeAdapter(Adapter15.class) @XmlSchemaType(name = "integer", //) protected Integer uid;
				L"ID", //) protected BigInteger id;
				L"Name", //) protected String name;
				L"Type", //type = String.class) @XmlJavaTypeAdapter(Adapter16.class, //) protected TaskType type;
				L"IsNull", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean isNull;
				L"CreateDate", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar createDate;
				L"Contact", //) protected String contact;
				L"WBS", //) protected String wbs;
				L"WBSLevel", //) protected String wbsLevel;
				L"OutlineNumber", //) protected String outlineNumber;
				L"OutlineLevel", //) protected BigInteger outlineLevel;
				L"Priority", //) protected BigInteger priority;
				L"Start", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar start;
				L"Finish", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar finish;
				L"Duration", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String duration;
				L"DurationFormat", //) protected BigInteger durationFormat;
				L"Work", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String work;
				L"Stop", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar stop;
				L"Resume", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar resume;
				L"ResumeValid", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean resumeValid;
				L"EffortDriven", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean effortDriven;
				L"Recurring", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean recurring;
				L"OverAllocated", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean overAllocated;
				L"Estimated", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean estimated;
				L"Milestone", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean milestone;
				L"Summary", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean summary;
				L"DisplayAsSummary", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean displayAsSummary;
				L"Critical", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean critical;
				L"IsSubproject", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean isSubproject;
				L"IsSubprojectReadOnly", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean isSubprojectReadOnly;
				L"SubprojectName", //) protected String subprojectName;
				L"ExternalTask", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean externalTask;
				L"ExternalTaskProject", //) protected String externalTaskProject;
				L"EarlyStart", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar earlyStart;
				L"EarlyFinish", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar earlyFinish;
				L"LateStart", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar lateStart;
				L"LateFinish", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar lateFinish;
				L"StartVariance", //) protected BigInteger startVariance;
				L"FinishVariance", //) protected BigInteger finishVariance;
				L"WorkVariance", //type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float", //) protected BigDecimal workVariance;
				L"FreeSlack", //) protected BigInteger freeSlack;
				L"StartSlack", //) protected BigInteger startSlack;
				L"FinishSlack", //) protected BigInteger finishSlack;
				L"TotalSlack", //) protected BigInteger totalSlack;
				L"FixedCost", //type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float", //) protected BigDecimal fixedCost;
				L"FixedCostAccrual", //type = String.class) @XmlJavaTypeAdapter(Adapter17.class, //) protected AccrueType fixedCostAccrual;
				L"PercentComplete", //) protected BigInteger percentComplete;
				L"PercentWorkComplete", //) protected BigInteger percentWorkComplete;
				L"Cost", //) protected BigDecimal cost;
				L"OvertimeCost", //) protected BigDecimal overtimeCost;
				L"OvertimeWork", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String overtimeWork;
				L"ActualStart", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar actualStart;
				L"ActualFinish", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar actualFinish;
				L"ActualDuration", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String actualDuration;
				L"ActualCost", //) protected BigDecimal actualCost;
				L"ActualOvertimeCost", //) protected BigDecimal actualOvertimeCost;
				L"ActualWork", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String actualWork;
				L"ActualOvertimeWork", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String actualOvertimeWork;
				L"RegularWork", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String regularWork;
				L"RemainingDuration", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String remainingDuration;
				L"RemainingCost", //) protected BigDecimal remainingCost;
				L"RemainingWork", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String remainingWork;
				L"RemainingOvertimeCost", //) protected BigDecimal remainingOvertimeCost;
				L"RemainingOvertimeWork", //) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String remainingOvertimeWork;
				L"ACWP", //type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float", //) protected BigDecimal acwp;
				L"CV", //type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float", //) protected BigDecimal cv;
				L"ConstraintType", //) protected BigInteger constraintType;
				L"CalendarUID", //) protected BigInteger calendarUID;
				L"ConstraintDate", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar constraintDate;
				L"Deadline", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar deadline;
				L"LevelAssignments", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean levelAssignments;
				L"LevelingCanSplit", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean levelingCanSplit;
				L"LevelingDelay", //) protected BigInteger levelingDelay;
				L"LevelingDelayFormat", //) protected BigInteger levelingDelayFormat;
				L"PreLeveledStart", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar preLeveledStart;
				L"PreLeveledFinish", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar preLeveledFinish;
				L"Hyperlink", //) protected String hyperlink;
				L"HyperlinkAddress", //) protected String hyperlinkAddress;
				L"HyperlinkSubAddress", //) protected String hyperlinkSubAddress;
				L"IgnoreResource2Calendar", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean ignoreResource2Calendar;
				L"Notes", //) protected String notes;
				L"HideBar", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean hideBar;
				L"Rollup", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean rollup;
				L"BCWS", //type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float", //) protected BigDecimal bcws;
				L"BCWP", //type = String.class) @XmlJavaTypeAdapter(Adapter2.class) @XmlSchemaType(name = "float", //) protected BigDecimal bcwp;
				L"PhysicalPercentComplete", //) protected BigInteger physicalPercentComplete;
				L"EarnedValueMethod", //) protected BigInteger earnedValueMethod;
				L"PredecessorLink", //) protected ObjectList<Project.Tasks.Task.PredecessorLink> predecessorLink;
				L"ActualWorkProtected",//) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String actualWorkProtected;
				L"ActualOvertimeWorkProtected",//) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String actualOvertimeWorkProtected;
				L"ExtendedAttribute", //) protected ObjectList<Project.Tasks.Task.ExtendedAttribute> extendedAttribute;
				L"Baseline", //) protected ObjectList<Project.Tasks.Task.Baseline> baseline;
				L"OutlineCode", //) protected ObjectList<Project.Tasks.Task.OutlineCode> outlineCode;
				L"IsPublished", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean isPublished;
				L"StatusManager", //) protected String statusManager;
				L"CommitmentStart", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar commitmentStart;
				L"CommitmentFinish", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar commitmentFinish;
				L"CommitmentType", //) protected BigInteger commitmentType;
				L"Active", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean active;
				L"Manual", //type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean", //) protected Boolean manual;
				L"StartText", //) protected String startText;
				L"FinishText", //) protected String finishText;
				L"DurationText", //) protected String durationText;
				L"ManualStart", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar manualStart;
				L"ManualFinish", //type = String.class) @XmlJavaTypeAdapter(Adapter1.class) @XmlSchemaType(name = "dateTime", //) protected java.util.Calendar manualFinish;
				L"ManualDuration",//) @XmlJavaTypeAdapter(Adapter5.class) @XmlSchemaType(name = "duration", //) protected String manualDuration;
				L"TimephasedData", //) protected ObjectList<TimephasedDataType> timephasedData;
				NULL
			};

			return fieldList;
		}

		std::shared_ptr<ObjectList<std::shared_ptr<PredecessorLink>>> Task::getPredecessorLink()
		{
			return getChildList<PredecessorLink>("PredecessorLink");
		}

		std::shared_ptr<ObjectList<std::shared_ptr<TimephasedDataType>>> Task::getTimephasedData()
		{
			return getChildList<TimephasedDataType>("TimephasedData");
		}

		std::shared_ptr<ObjectList<std::shared_ptr<Baseline>>> Task::getBaseline()
		{
			return getChildList<Baseline>("Baseline");
		}

		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> Task::getExtendedAttribute()
		{
			return getChildList<ExtendedAttribute>("ExtendedAttribute");
		}

		TimephasedDataType::TimephasedDataType(TiXmlElement* element) : FieldValue(element)
		{

		}

		PredecessorLink::PredecessorLink(TiXmlElement* element) : FieldValue(element)
		{
		}

		// 获取有序的字段名称列表
		LPCTSTR* PredecessorLink::getFieldList()
		{
			static LPCTSTR fieldList[] = {
				L"PredecessorUID",//) protected BigInteger predecessorUID;
				L"Type",//) protected BigInteger type;
				L"CrossProject",// type = String.class) @XmlJavaTypeAdapter(Adapter4.class) @XmlSchemaType(name = "boolean") protected Boolean crossProject;
				L"CrossProjectName",//) protected String crossProjectName;
				L"LinkLag",//) protected BigInteger linkLag;
				L"LagFormat",//) protected BigInteger lagFormat;
				NULL
			};
			return fieldList;
		}

		WBSMasks::WBSMasks(TiXmlElement* element) : FieldValue(element)
		{

		}

		WBSMask::WBSMask(TiXmlElement* element) : FieldValue(element)
		{

		}

	}// end namespace mspdi::schema
}