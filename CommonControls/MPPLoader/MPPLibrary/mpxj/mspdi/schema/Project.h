#pragma once

//#include "..\..\java\Variant.h"
#include "XmlFieldParse.h"

namespace mspdi{ namespace schema{

	class OutlineCodes;
	class WBSMasks;
	class ExtendedAttributes;
	class Calendars;
	class Tasks;
	class Baseline;
	class Resource2s;
	class Assignments;
	class ExtendedAttribute;
	class Calendar;
	class WeekDays;
	class Exceptions;
	class WorkWeeks;
	class Assignment;
	class TimePeriod;
	class WorkingTimes;
	class Task;
	class AvailabilityPeriod;
	class AvailabilityPeriods;
	class Rate;
	class Rates;
	class Resource2;
	class Mask;
	class OutlineCode;
	class Value;
	class WeekDay;
	class Exception;
	class WorkingTime;
	class WorkWeek;
	class PredecessorLink;
	class TimephasedDataType;

	class Project : public FieldValue
	{
	public:
		Project(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

		std::shared_ptr<OutlineCodes> getOutlineCodes();
		std::shared_ptr<WBSMasks> getWBSMasks();
		std::shared_ptr<ExtendedAttributes> getExtendedAttributes();
		std::shared_ptr<Calendars> getCalendars();
		std::shared_ptr<Tasks> getTasks();
		std::shared_ptr<Resource2s> getResource2s();
		std::shared_ptr<Assignments> getAssignments();
	};

	class Assignments : public FieldValue
	{
	public:
		Assignments(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Assignment>>> getAssignment();
		std::shared_ptr<ObjectList<std::shared_ptr<TimephasedDataType>>> getTimephasedData();
	};

	class Assignment : public FieldValue
	{
	public:
		Assignment(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> getExtendedAttribute();
		std::shared_ptr<ObjectList<std::shared_ptr<Baseline>>> getBaseline();
		std::shared_ptr<ObjectList<std::shared_ptr<TimephasedDataType>>> getTimephasedData();
	};

	class Baseline : public FieldValue
	{
	public:
		Baseline(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();
	};

	class Calendars : public FieldValue
	{
	public:
		Calendars(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Calendar>>> getCalendar();

	};

	class Calendar : public FieldValue
	{
	public:
		Calendar(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

		std::shared_ptr<WeekDays> getWeekDays();
		std::shared_ptr<Exceptions> getExceptions();
		std::shared_ptr<WorkWeeks> getWorkWeeks();
	};

	class Exceptions : public FieldValue
	{
	public:
		Exceptions(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Exception>>> getException();
	};

	class Exception : public FieldValue
	{
	public:
		Exception(TiXmlElement* element);

		std::shared_ptr<TimePeriod> getTimePeriod();
		std::shared_ptr<WorkingTimes> getWorkingTimes();
	};

	class WeekDays : public FieldValue
	{
	public:
		WeekDays(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<WeekDay>>> getWeekDay();
	};

	class WeekDay : public FieldValue
	{
	public:
		WeekDay(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

		std::shared_ptr<TimePeriod> getTimePeriod();
		std::shared_ptr<WorkingTimes> getWorkingTimes();
	};

	class TimePeriod : public FieldValue
	{
	public:
		TimePeriod(TiXmlElement* element);

		DatePtr getFromDate();
		DatePtr getToDate();

		//void setToDate(DatePtr value);
		//void setFromDate(DatePtr value);
	};

	class WorkingTimes : public FieldValue
	{
	public:
		WorkingTimes(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<WorkingTime>>> getWorkingTime();
	};

	class WorkingTime : public FieldValue
	{
	public:
		WorkingTime(TiXmlElement* element);

		DatePtr getFromTime();
		DatePtr getToTime();
		//void setFromTime(DatePtr value);
		//void setToTime(DatePtr value);
	};

	class WorkWeeks : public FieldValue
	{
	public:
		WorkWeeks(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<WorkWeek>>> getWorkWeek();
	};

	class WorkWeek : public FieldValue
	{
	public:
		WorkWeek(TiXmlElement* element);

		std::shared_ptr<TimePeriod> getTimePeriod();
		std::shared_ptr<WeekDays> getWeekDays();
		CString getName();
	};
	
	class ExtendedAttributes : public FieldValue
	{
	public:
		ExtendedAttributes(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> getExtendedAttribute();
	};

	class ExtendedAttribute : public FieldValue
	{
	public:
		ExtendedAttribute(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();
	};

	class ValueList : public FieldValue
	{
	public:
		ValueList(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Value>>> getValue();

	};

	class Value : public FieldValue
	{
	public:
		Value(TiXmlElement* element);

	};

	class OutlineCodes : public FieldValue
	{
	public:
		OutlineCodes(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<OutlineCode>>> getOutlineCode();
	};

	class OutlineCode : public FieldValue
	{
	public:
		OutlineCode(TiXmlElement* element);

	};

	class Masks : public FieldValue
	{
	public:
		Masks(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Mask>>> getMask();
	};

	class Mask : public FieldValue
	{
	public:
		Mask(TiXmlElement* element);

	};

	class Resource2s : public FieldValue
	{
	public:
		Resource2s(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Resource2>>> getResource2();
	};

	class Resource2 : public FieldValue
	{
	public:
		Resource2(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

		std::shared_ptr<AvailabilityPeriods> getAvailabilityPeriods();
		std::shared_ptr<Rates> getRates();
		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> getExtendedAttribute();
		std::shared_ptr<ObjectList<std::shared_ptr<Baseline>>> getBaseline();
	};

	class Rates : public FieldValue
	{
	public:
		Rates(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Rate>>> getRate();
	};

	class Rate : public FieldValue
	{
	public:
		Rate(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

	};

	class AvailabilityPeriods : public FieldValue
	{
	public:
		AvailabilityPeriods(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<AvailabilityPeriod>>> getAvailabilityPeriod();
	};

	class AvailabilityPeriod : public FieldValue
	{
	public:
		AvailabilityPeriod(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

	};

	class Tasks : public FieldValue
	{
	public:
		Tasks(TiXmlElement* element);

		std::shared_ptr<ObjectList<std::shared_ptr<Task>>> getTask();
	};

	class Task : public FieldValue
	{
	public:
		Task(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

		std::shared_ptr<ObjectList<std::shared_ptr<PredecessorLink>>> getPredecessorLink();

		std::shared_ptr<ObjectList<std::shared_ptr<TimephasedDataType>>> getTimephasedData();

		std::shared_ptr<ObjectList<std::shared_ptr<Baseline>>> getBaseline();

		std::shared_ptr<ObjectList<std::shared_ptr<ExtendedAttribute>>> getExtendedAttribute();
	};

	class TimephasedDataType : public FieldValue
	{
	public:
		TimephasedDataType(TiXmlElement* element);

	};

	class PredecessorLink : public FieldValue
	{
	public:
		PredecessorLink(TiXmlElement* element);

		// 获取有序的字段名称列表
		virtual LPCTSTR* getFieldList();

	};

	class WBSMasks : public FieldValue
	{
	public:
		WBSMasks(TiXmlElement* element);
	};

	class WBSMask : public FieldValue
	{
	public:
		WBSMask(TiXmlElement* element);

	};

}}	// end namespace mspdi::schema