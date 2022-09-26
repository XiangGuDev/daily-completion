#pragma once

#include "..\reader\AbstractProjectReader.h"
#include "..\common\SplitTaskFactory.h"
#include "schema\Project.h"
#include "DatatypeConverter.h"
#include "..\..\..\Toolkit\include\tinyxml.h"

//using namespace mspdi::schema;

/**
 * This class creates a new ProjectFile instance by reading an MSPDI file.
 */
class MSPDIReader : public AbstractProjectReader
{
public:
	MSPDIReader();

	/**
	 * {@inheritDoc}
	 */
	virtual void addProjectListener(ProjectListener* listener);

	/**
	 * {@inheritDoc}
	 */
	virtual ProjectFilePtr read(BwProj::toolkit::IDataReadStream* stream);

	/**
	* Sets a flag indicating that this class will attempt to correct
	* and read XML which is not compliant with the XML Schema. This
	* behaviour matches that of Microsoft Project* when reading the
	* same data.
	*
	* @param flag input compatibility flag
	*/
	void setMicrosoftProjectCompatibleInput(bool flag);

	/**
	* Retrieves a flag indicating that this class will attempt to correct
	* and read XML which is not compliant with the XML Schema. This
	* behaviour matches that of Microsoft Project* when reading the
	* same data.
	*
	* @return Boolean flag
	*/
	bool getMicrosoftProjectCompatibleInput();

private:
	// 验证是否是project xml文件格式
	bool validateXmlFormat(TiXmlDocument* document);

	/**
	 * This method extracts project header data from an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void readProjectHeader(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method extracts calendar data from an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 * @param map Map of calendar UIDs to names
	 */
	void readCalendars(std::shared_ptr<mspdi::schema::Project> project, Map2<int, ProjectCalendarPtr>* map);

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
	static void updateBaseCalendarNames(List<std::pair<ProjectCalendarPtr, int>>* baseCalendars,
		Map2<int, ProjectCalendarPtr>* map);

	/**
	 * This method extracts data for a single calendar from an MSPDI file.
	 *
	 * @param calendar Calendar data
	 * @param map Map of calendar UIDs to names
	 * @param baseCalendars list of base calendars
	 */
	void readCalendar(std::shared_ptr<mspdi::schema::Calendar> calendar, Map2<int, ProjectCalendarPtr>* map,
		List<std::pair<ProjectCalendarPtr, int>>* baseCalendars);

	/**
	 * This method extracts data for a single day from an MSPDI file.
	 *
	 * @param calendar Calendar data
	 * @param day Day data
	 */
	void readDay(ProjectCalendarPtr calendar, std::shared_ptr<mspdi::schema::WeekDay> day);

	/**
	 * This method extracts data for a normal working day from an MSPDI file.
	 *
	 * @param calendar Calendar data
	 * @param weekDay Day data
	 */
	void readNormalDay(ProjectCalendarPtr calendar, std::shared_ptr<mspdi::schema::WeekDay> weekDay);

	/**
	 * This method extracts data for an exception day from an MSPDI file.
	 *
	 * @param calendar Calendar data
	 * @param day Day data
	 */
	void readExceptionDay(ProjectCalendarPtr calendar, std::shared_ptr<mspdi::schema::WeekDay> day);

	/**
	 * Reads any exceptions present in the file. This is only used in MSPDI
	 * file versions saved by Project* 2007 and later.
	 *
	 * @param calendar XML calendar
	 * @param bc MPXJ calendar
	 */
	void readExceptions(std::shared_ptr<mspdi::schema::Calendar> calendar, ProjectCalendarPtr bc);

	/**
	 * Read the work weeks associated with this calendar.
	 *
	 * @param xmlCalendar XML calendar object
	 * @param mpxjCalendar MPXJ calendar object
	 */
	void readWorkWeeks(std::shared_ptr<mspdi::schema::Calendar> xmlCalendar, ProjectCalendarPtr mpxjCalendar);

	/**
	 * This method extracts project extended attribute data from an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void readProjectExtendedAttributes(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * Read a single field alias from an extended attribute.
	 *
	 * @param attribute extended attribute
	 */
	void readFieldAlias(std::shared_ptr<mspdi::schema::ExtendedAttribute> attribute);

	/**
	 * This method extracts resource data from an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 * @param calendarMap Map of calendar UIDs to names
	 */
	void readResource2s(std::shared_ptr<mspdi::schema::Project> project, Map2<int, ProjectCalendarPtr>* calendarMap);

	/**
	 * This method extracts data for a single resource from an MSPDI file.
	 *
	 * @param xml Resource2* data
	 * @param calendarMap Map of calendar UIDs to names
	 */
	void readResource2(std::shared_ptr<mspdi::schema::Resource2> xml, Map2<int, ProjectCalendarPtr>* calendarMap);

	/**
	 * Reads baseline values for the current resource.
	 *
	 * @param xmlResource2 MSPDI resource instance
	 * @param mpxjResource2 MPXJ resource instance
	 */
	void readResource2Baselines(std::shared_ptr<mspdi::schema::Resource2> xmlResource2, Resource2Ptr mpxjResource2);

	/**
	 * This method processes any extended attributes associated with a resource.
	 *
	 * @param xml MSPDI resource instance
	 * @param mpx MPX resource instance
	 */
	void readResource2ExtendedAttributes(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx);

	/**
	 * Reads the cost rate tables from the file.
	 *
	 * @param resource parent resource
	 * @param rates XML cot rate tables
	 */
	void readCostRateTables(Resource2Ptr resource, std::shared_ptr<mspdi::schema::Rates> rates);

	/**
	 * Reads the availability table from the file.
	 *
	 * @param resource MPXJ resource instance
	 * @param periods MSPDI availability periods
	 */
	void readAvailabilityTable(Resource2Ptr resource, std::shared_ptr<mspdi::schema::AvailabilityPeriods> periods);

	/**
	 * This method extracts task data from an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void readTasks(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method extracts data for a single task from an MSPDI file.
	 *
	 * @param xml Task* data
	 */
	void readTask(std::shared_ptr<mspdi::schema::Task> xml);

	/**
	 * Reads baseline values for the current task.
	 *
	 * @param xmlTask MSPDI task instance
	 * @param mpxjTask MPXJ task instance
	 * @param durationFormat duration format to use
	 */
	void readTaskBaselines(std::shared_ptr<mspdi::schema::Task> xmlTask, Task* mpxjTask, TimeUnit durationFormat);

	/**
	 * This method processes any extended attributes associated with a task.
	 *
	 * @param xml MSPDI task instance
	 * @param mpx MPX task instance
	 */
	void readTaskExtendedAttributes(std::shared_ptr<mspdi::schema::Task> xml, Task* mpx);

	/**
	 * This method is used to retrieve the calendar associated
	 * with a task. If no calendar is associated with a task, this method
	 * returns null.
	 *
	 * @param task MSPDI task
	 * @return calendar instance
	 */
	ProjectCalendarPtr getTaskCalendar(std::shared_ptr<mspdi::schema::Task> task);

	/**
	 * This method extracts predecessor data from an MSPDI file.
	 *
	 * @param task Task* data
	 */
	void readPredecessors(std::shared_ptr<mspdi::schema::Task> task);

	/**
	 * This method extracts data for a single predecessor from an MSPDI file.
	 *
	 * @param currTask Current task object
	 * @param link Predecessor data
	 */
	void readPredecessor(Task* currTask, std::shared_ptr<mspdi::schema::PredecessorLink> link);

	/**
	 * This method extracts assignment data from an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void readAssignments(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method extracts data for a single assignment from an MSPDI file.
	 *
	 * @param assignment Assignment data
	 * @param splitFactory split task handling
	 * @param normaliser timephased resource assignment normaliser
	 */
	void readAssignment(std::shared_ptr<mspdi::schema::Assignment> assignment, 
		SplitTaskFactory* splitFactory, TimephasedWorkNormaliserPtr normaliser);

	/**
	 * Extracts assignment baseline data.
	 *
	 * @param assignment xml assignment
	 * @param mpx mpxj assignment
	 */
	void readAssignmentBaselines(std::shared_ptr<mspdi::schema::Assignment> assignment, Resource2AssignmentPtr mpx);

	/**
	 * This method processes any extended attributes associated with a
	 * resource assignment.
	 *
	 * @param xml MSPDI resource assignment instance
	 * @param mpx MPX task instance
	 */
	void readAssignmentExtendedAttributes(std::shared_ptr<mspdi::schema::Assignment> xml, Resource2AssignmentPtr mpx);

	/**
	 * Test to determine if this is a split task.
	 *
	 * @param calendar current calendar
	 * @param list timephased resource assignment list
	 * @return bool flag
	 */
	bool isSplit(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);

	/**
	 * Reads timephased assignment data.
	 *
	 * @param calendar current calendar
	 * @param assignment assignment data
	 * @param type flag indicating if this is planned or complete work
	 * @return list of timephased resource assignment instances
	 */
	std::shared_ptr<TimephasedWorkList> readTimephasedAssignment(ProjectCalendarPtr calendar,
		std::shared_ptr<mspdi::schema::Assignment> assignment, int type);

private:
	bool m_compatibleInput = true;

	ProjectFile* m_projectFile;
	List<ProjectListener*> m_projectListeners;
	DatatypeConverter m_dataConverter;
};