#pragma once

#include "..\reader\AbstractProjectWriter.h"
#include "schema\Project.h"
#include "schema\ObjectFactory.h"
#include "DatatypeConverter.h"

/**
 * This class creates a new MSPDI file from the contents of an ProjectFile* instance.
 */
class MSPDIWriter : public AbstractProjectWriter
{
public:
	MSPDIWriter();

	/**
	 * Sets a flag to control whether timephased assignment data is split
	 * into days. The default is true.
	 *
	 * @param flag bool flag
	 */
	void setSplitTimephasedAsDays(bool flag);

	/**
	 * Retrieves a flag to control whether timephased assignment data is split
	 * into days. The default is true.
	 *
	 * @return bool true
	 */
	bool getSplitTimephasedAsDays();

	/**
	 * Sets a flag to control whether timephased resource assignment data
	 * is written to the file. The default is false.
	 *
	 * @param value bool flag
	 */
	void setWriteTimephasedData(bool value);

	/**
	 * Retrieves the state of the flag which controls whether timephased
	 * resource assignment data is written to the file. The default is false.
	 *
	 * @return bool flag
	 */
	bool getWriteTimephasedData();

	/**
	 * Set the save version to use when generating an MSPDI file.
	 *
	 * @param version save version
	 */
	void setSaveVersion(SaveVersion version);

	/**
	 * Retrieve the save version current set.
	 *
	 * @return current save version
	 */
	SaveVersion getSaveVersion();

	/**
	 * {@inheritDoc}
	 */
	virtual void write(ProjectFile* projectFile, BwProj::toolkit::IDataWriteStream* stream);

	/**
	* Package-accessor method used to retrieve the project file
	* currently being processed by this writer.
	*
	* @return project file instance
	*/
	ProjectFile* getProjectFile();

private:
	void reset();

	/**
	 * This method writes project header data to an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void writeProjectHeader(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method writes project extended attribute data into an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void writeProjectExtendedAttributes(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * Writes field aliases.
	 *
	 * @param list field alias list
	 */
	void writeTaskFieldAliases(std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> list);

	/**
	 * Writes field aliases.
	 *
	 * @param list field alias list
	 */
	void writeResource2FieldAliases(std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::ExtendedAttribute>>> list);

	/**
	 * This method writes calendar data to an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void writeCalendars(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method writes data for a single calendar to an MSPDI file.
	 *
	 * @param bc Base calendar data
	 * @return New MSPDI calendar instance
	 */
	std::shared_ptr<mspdi::schema::Calendar> writeCalendar(ProjectCalendarPtr bc);

	/**
	 * Main entry point used to determine the format used to write
	 * calendar exceptions.
	 *
	 * @param calendar parent calendar
	 * @param dayList list of calendar days
	 * @param exceptions list of exceptions
	 */
	void writeExceptions(std::shared_ptr<mspdi::schema::Calendar> calendar,
		std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WeekDay>>> dayList,
		std::shared_ptr<List<ProjectCalendarExceptionPtr>> exceptions);

	/**
	 * Write exceptions in the format used by MSPDI files prior to Project* 2007.
	 *
	 * @param dayList list of calendar days
	 * @param shared_ptr< list of exceptions
	 */
	void writeExcepions9(std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::WeekDay>>> dayList,
		std::shared_ptr<List<ProjectCalendarExceptionPtr>> exceptions);

	/**
	 * Write exceptions into the format used by MSPDI files from
	 * Project* 2007 onwards.
	 *
	 * @param calendar parent calendar
	 * @param exceptions list of exceptions
	 */
	void writeExcepions12(std::shared_ptr<mspdi::schema::Calendar> calendar, 
		std::shared_ptr<List<ProjectCalendarExceptionPtr>> exceptions);

	/**
	 * Write the work weeks associated with this calendar.
	 *
	 * @param xmlCalendar XML calendar instance
	 * @param mpxjCalendar MPXJ calendar instance
	 */
	void writeWorkWeeks(std::shared_ptr<mspdi::schema::Calendar> xmlCalendar, ProjectCalendarPtr mpxjCalendar);

	/**
	 * This method writes resource data to an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void writeResource2s(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method writes data for a single resource to an MSPDI file.
	 *
	 * @param mpx Resource2Ptr data
	 * @return New MSPDI resource instance
	 */
	std::shared_ptr<mspdi::schema::Resource2> writeResource2(Resource2Ptr mpx);

	/**
	 * Writes resource baseline data.
	 *
	 * @param xmlResource2 MSPDI resource
	 * @param mpxjResource2 MPXJ resource
	 */
	void writeResource2Baselines(std::shared_ptr<mspdi::schema::Resource2> xmlResource2, Resource2Ptr mpxjResource2);

	/**
	 * This method writes extended attribute data for a resource.
	 *
	 * @param xml MSPDI resource
	 * @param mpx MPXJ resource
	 */
	void writeResource2ExtendedAttributes(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx);

	/**
	 * This method is called to determine if an extended attribute
	 * should be written to the file, or whether the default value
	 * can be assumed.
	 *
	 * @param value extended attribute value
	 * @param type extended attribute data type
	 * @return bool flag
	 */
	bool writeExtendedAttribute(ObjectPtr value, FieldType* type);

	/**
	 * Writes a resource's cost rate tables.
	 *
	 * @param xml MSPDI resource
	 * @param mpx MPXJ resource
	 */
	void writeCostRateTables(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx);

	/**
	 * This method determines whether the cost rate table should be written.
	 * A default cost rate table should not be written to the file.
	 *
	 * @param entry cost rate table entry
	 * @param from from date
	 * @return bool flag
	 */
	bool costRateTableWriteRequired(CostRateTableEntryPtr entry, DatePtr from);

	/**
	 * This method writes a resource's availability table.
	 *
	 * @param xml MSPDI resource
	 * @param mpx MPXJ resource
	 */
	void writeAvailability(std::shared_ptr<mspdi::schema::Resource2> xml, Resource2Ptr mpx);

	/**
	 * This method writes task data to an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void writeTasks(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method writes data for a single task to an MSPDI file.
	 *
	 * @param mpx Task* data
	 * @return new task instance
	 */
	std::shared_ptr<mspdi::schema::Task> writeTask(Task* mpx);

	/**
	 * Writes task baseline data.
	 *
	 * @param xmlTask MSPDI task
	 * @param mpxjTask MPXJ task
	 */
	void writeTaskBaselines(std::shared_ptr<mspdi::schema::Task> xmlTask, Task* mpxjTask);

	/**
	 * This method writes extended attribute data for a task.
	 *
	 * @param xml MSPDI task
	 * @param mpx MPXJ task
	 */
	void writeTaskExtendedAttributes(std::shared_ptr<mspdi::schema::Task> xml, Task* mpx);

	/**
	 * Converts a duration to duration time units.
	 *
	 * @param value duration value
	 * @return duration time units
	 */
	BigIntegerPtr printExtendedAttributeDurationFormat(ObjectPtr value);

	/**
	 * This method retrieves the UID for a calendar associated with a task.
	 *
	 * @param mpx MPX Task* instance
	 * @return calendar UID
	 */
	BigIntegerPtr getTaskCalendarID(Task* mpx);

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
	void writePredecessors(std::shared_ptr<mspdi::schema::Task> xml, Task* mpx);

	/**
	 * This method writes a single predecessor link to the MSPDI file.
	 *
	 * @param taskID The task UID
	 * @param type The predecessor type
	 * @param lag The lag duration
	 * @return A new link to be added to the MSPDI file
	 */
	std::shared_ptr<mspdi::schema::PredecessorLink> writePredecessor(
		int taskID, RelationType type, DurationPtr lag);

	/**
	 * This method writes assignment data to an MSPDI file.
	 *
	 * @param project Root node of the MSPDI file
	 */
	void writeAssignments(std::shared_ptr<mspdi::schema::Project> project);

	/**
	 * This method writes data for a single assignment to an MSPDI file.
	 *
	 * @param mpx Resource2Ptr assignment data
	 * @return New MSPDI assignment instance
	 */
	std::shared_ptr<mspdi::schema::Assignment> writeAssignment(Resource2AssignmentPtr mpx);

	/**
	 * Writes assignment baseline data.
	 *
	 * @param xml MSPDI assignment
	 * @param mpxj MPXJ assignment
	 */
	void writeAssignmentBaselines(std::shared_ptr<mspdi::schema::Assignment> xml, Resource2AssignmentPtr mpxj);

	/**
	 * This method writes extended attribute data for an assignment.
	 *
	 * @param xml MSPDI assignment
	 * @param mpx MPXJ assignment
	 */
	void writeAssignmentExtendedAttributes(std::shared_ptr<mspdi::schema::Assignment> xml, Resource2AssignmentPtr mpx);

	/**
	 * Writes the timephased data for a resource assignment.
	 *
	 * @param mpx MPXJ assignment
	 * @param xml MSDPI assignment
	 */
	void writeAssignmentTimephasedData(Resource2AssignmentPtr mpx, std::shared_ptr<mspdi::schema::Assignment> xml);

	/**
	 * Splits timephased data into individual days.
	 *
	 * @param calendar current calendar
	 * @param list list of timephased assignment data
	 * @param first first planned assignment
	 * @param last last completed assignment
	 * @return list of timephased data ready for output
	 */
	TimephasedWorkListPtr splitDays(ProjectCalendarPtr calendar, 
		TimephasedWorkListPtr list, TimephasedWorkPtr first, TimephasedWorkPtr last);

	/**
	 * Writes a list of timephased data to the MSPDI file.
	 *
	 * @param assignmentID current assignment ID
	 * @param list output list of timephased data items
	 * @param data input list of timephased data
	 * @param type list type (planned or completed)
	 */
	void writeAssignmentTimephasedData(BigIntegerPtr assignmentID,
		std::shared_ptr<ObjectList<std::shared_ptr<mspdi::schema::TimephasedDataType>>> list,
		TimephasedWorkListPtr data, int type);

	/**
	 * Retrieve list of assignment extended attributes.
	 *
	 * @return list of extended attributes
	 */
	std::shared_ptr<List<AssignmentField>> getAllAssignmentExtendedAttributes();

	/**
	 * Retrieve list of task extended attributes.
	 *
	 * @return list of extended attributes
	 */
	std::shared_ptr<List<TaskField>> getAllTaskExtendedAttributes();

	/**
	 * Retrieve list of resource extended attributes.
	 *
	 * @return list of extended attributes
	 */
	std::shared_ptr<List<Resource2Field>> getAllResource2ExtendedAttributes();

private:
	std::shared_ptr<List<AssignmentField>> m_AssignmentFieldExtAttrs;
	std::shared_ptr<List<TaskField>> m_TaskFieldExtAttrs;
	std::shared_ptr<List<Resource2Field>> m_Resource2FieldExtAttrs;

	ObjectFactory m_factory;

	ProjectFile* m_projectFile;

	Set<TaskField> m_taskExtendedAttributes;

	Set<Resource2Field> m_resourceExtendedAttributes;

	Set<AssignmentField> m_assignmentExtendedAttributes;

	bool m_splitTimephasedAsDays = true;

	bool m_writeTimphasedData;

	SaveVersion m_saveVersion = SaveVersion::Project2002;

	DatatypeConverter m_dataConverter;

	static const int BIGINTEGER_ZERO = 0;// = BigInteger.valueOf(0);

	static const int NULL_RESOURCE_ID = (-65535);
};