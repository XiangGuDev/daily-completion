#pragma once

#include "..\JavaJREImpl.h"
#include "TaskContainer.h"
#include "FileCreationRecord.h"
#include "ProjectCalendar.h"
#include "ProjectHeader.h"
#include "Resource.h"
#include "SubProject.h"
#include "ResourceAssignment.h"
#include "Duration.h"
#include "Relation.h"
#include "listener\ProjectListener.h"
#include "TaskField.h"
#include "ResourceField.h"
#include "View.h"
#include "Table.h"
#include "Filter.h"
#include "ViewState.h"
//#include "GraphicalIndicator.h"
#include "Group.h"
#include "Task.h"
#include "mpp\CustomFieldValueItem.h"

/**
 * This class represents a project plan.
 */
class COMMONCONTROLS_API ProjectFile : public TaskContainer
{
	friend Task;
	friend Resource2Assignment;
	friend Resource2;
	friend ProjectCalendar;
public:
	ProjectFile();
	~ProjectFile();

   /**
    * Accessor method to retrieve the current file delimiter character.
    *
    * @return delimiter character
    */
	char getDelimiter();

   /**
    * Modifier method used to set the delimiter character.
    *
    * @param delimiter delimiter character
    */
	void setDelimiter(char delimiter);

protected:
   /**
    * This method is provided to allow child tasks that have been created
    * programmatically to be added as a record to the main file->
    *
    * @param task task created as a child of another task
    */
	void addTask(Task* task);

public:
   /**
    * This method allows a task to be added to the file programatically.
    *
    * @return new task object
    */
	Task* addTask();

   /**
    * This method is used to remove a task from the project.
    *
    * @param task task to be removed
    */
	void removeTask(Task* task);
	void removeAllTask();

   /**
    * This method can be called to ensure that the IDs of all
    * tasks in this project are sequential, and start from an
    * appropriate point. If tasks are added to and removed from
    * the list of tasks, then the project is loaded into Microsoft
    * project, if the ID values have gaps in the sequence, there will
    * be blank task rows shown.
    */
	void renumberTaskIDs();

   /**
    * This method can be called to ensure that the IDs of all
    * resources in this project are sequential, and start from an
    * appropriate point. If resources are added to and removed from
    * the list of resources, then the project is loaded into Microsoft
    * project, if the ID values have gaps in the sequence, there will
    * be blank resource rows shown.
    */
	void renumberResource2IDs();

   /**
    * This method is called to ensure that all unique ID values
    * held by MPXJ are within the range supported by MS Project.
    * If any of these values fall outside of this range, the unique IDs
    * of the relevant entities are renumbered.
    */
	void validateUniqueIDsForMicrosoftProject();

   /**
    * Microsoft Project bases the order of tasks displayed on their ID
    * value. This method takes the hierarchical structure of tasks
    * represented in MPXJ and renumbers the ID values to ensure that
    * this structure is displayed as expected in Microsoft Project. This
    * is typically used to deal with the case where a hierarchical task
    * structure has been created programatically in MPXJ.  
    */
	void synchronizeTaskIDToHierarchy();

   /**
    * This method is used to retrieve a list of all of the top level tasks
    * that are defined in this project file->
    *
    * @return list of tasks
    */
	List<Task*>* getChildTasks();

   /**
    * This method is used to retrieve a list of all of the tasks
    * that are defined in this project file->
    *
    * @return list of all tasks
    */
	List<Task*>* getAllTasks();

   /**
    * Used to set whether WBS numbers are automatically created.
    *
    * @param flag true if automatic WBS required.
    */
	void setAutoWBS(bool flag);

   /**
    * Used to set whether outline level numbers are automatically created.
    *
    * @param flag true if automatic outline level required.
    */
	void setAutoOutlineLevel(bool flag);

   /**
    * Used to set whether outline numbers are automatically created.
    *
    * @param flag true if automatic outline number required.
    */
	void setAutoOutlineNumber(bool flag);

   /**
    * Used to set whether the task unique ID field is automatically populated.
    *
    * @param flag true if automatic unique ID required.
    */
	void setAutoTaskUniqueID(bool flag);

   /**
    * Used to set whether the calendar unique ID field is automatically populated.
    *
    * @param flag true if automatic unique ID required.
    */
	void setAutoCalendarUniqueID(bool flag);

   /**
    * Used to set whether the assignment unique ID field is automatically populated.
    *
    * @param flag true if automatic unique ID required.
    */
	void setAutoAssignmentUniqueID(bool flag);

   /**
    * Used to set whether the task ID field is automatically populated.
    *
    * @param flag true if automatic ID required.
    */
	void setAutoTaskID(bool flag);

   /**
    * Retrieve the flag that determines whether WBS is generated
    * automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoWBS();

   /**
    * Retrieve the flag that determines whether outline level is generated
    * automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoOutlineLevel();

   /**
    * Retrieve the flag that determines whether outline numbers are generated
    * automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoOutlineNumber();

   /**
    * Retrieve the flag that determines whether the task unique ID
    * is generated automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoTaskUniqueID();

   /**
    * Retrieve the flag that determines whether the calendar unique ID
    * is generated automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoCalendarUniqueID();

   /**
    * Retrieve the flag that determines whether the assignment unique ID
    * is generated automatically.
    *
    * @return bool, default is true.
    */
	bool getAutoAssignmentUniqueID();

   /**
    * Retrieve the flag that determines whether the task ID
    * is generated automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoTaskID();

   /**
    * This method is used to retrieve the next unique ID for a task.
    *
    * @return next unique ID
    */
	int getTaskUniqueID();

   /**
    * This method is used to retrieve the next unique ID for a calendar.
    *
    * @return next unique ID
    */
	int getCalendarUniqueID();

protected:
   /**
    * This method is used to retrieve the next unique ID for an assignment.
    *
    * @return next unique ID
    */
	int getAssignmentUniqueID();

public:
   /**
    * This method is used to retrieve the next ID for a task.
    *
    * @return next ID
    */
	int getTaskID();

   /**
    * Used to set whether the resource unique ID field is automatically populated.
    *
    * @param flag true if automatic unique ID required.
    */
	void setAutoResource2UniqueID(bool flag);

   /**
    * Used to set whether the resource ID field is automatically populated.
    *
    * @param flag true if automatic ID required.
    */
	void setAutoResource2ID(bool flag);

   /**
    * Retrieve the flag that determines whether the resource unique ID
    * is generated automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoResource2UniqueID();

   /**
    * Retrieve the flag that determines whether the resource ID
    * is generated automatically.
    *
    * @return bool, default is false.
    */
	bool getAutoResource2ID();

   /**
    * This method is used to retrieve the next unique ID for a resource.
    *
    * @return next unique ID
    */
	int getResource2UniqueID();

   /**
    * This method is used to retrieve the next ID for a resource.
    *
    * @return next ID
    */
	int getResource2ID();

   /**
    * Retrieves the file creation record.
    *
    * @return file creation record.
    */
	FileCreationRecord* getFileCreationRecord();

   /**
    * This method is used to add a new calendar to the file->
    *
    * @return new calendar object
    */
	ProjectCalendarPtr addCalendar();

   /**
    * Removes a calendar.
    *
    * @param calendar calendar to be removed
    */
	void removeCalendar(ProjectCalendarPtr calendar);

   /**
    * This is a convenience method used to add a calendar called
    * "Standard" to the file, and populate it with a default working week
    * and default working hours.
    *
    * @return a new default calendar
    */
	ProjectCalendarPtr addDefaultBaseCalendar();

   /**
    * This is a protected convenience method to add a default derived
    * calendar.
    *
    * @return new ProjectCalendar instance
    */
	ProjectCalendarPtr addDefaultDerivedCalendar();

   /**
    * This method retrieves the list of calendars defined in
    * this file->
    *
    * @return list of calendars
    */
	List<ProjectCalendarPtr>* getCalendars();

   /**
    * This method is used to retrieve the project header record.
    *
    * @return project header object
    */
	ProjectHeader* getProjectHeader();

   /**
    * This method is used to add a new resource to the file->
    *
    * @return new resource object
    */
	Resource2Ptr addResource2();

   /**
    * This method is used to remove a resource from the project.
    *
    * @param resource resource to be removed
    */
	void removeResource2(Resource2Ptr resource);

   /**
    * Retrieves a list of all resources in this project.
    *
    * @return list of all resources
    */
	List<Resource2Ptr>* getAllResource2s();

   /**
    * Retrieves a list of all resource assignments in this project.
    *
    * @return list of all resources
    */
	List<Resource2AssignmentPtr>* getAllResource2Assignments();

protected:
   /**
    * This method is provided to allow resource assignments that have been
    * created programatically to be added as a record to the main file->
    *
    * @param assignment Resource2Ptr assignment created as part of a task
    */
	void addResource2Assignment(Resource2AssignmentPtr assignment);

   /**
    * This method removes a resource assignment from the internal storage
    * maintained by the project file->
    *
    * @param assignment resource assignment to remove
    */
	void removeResource2Assignment(Resource2AssignmentPtr assignment);

public:
   /**
    * This method has been provided to allow the subclasses to
    * instantiate ResourecAssignment instances.
    *
    * @param task parent task
    * @return new resource assignment instance
    */
	Resource2AssignmentPtr newResource2Assignment(Task* task);

   /**
    * Retrieves the named calendar. This method will return
    * NULL if the named calendar is not located.
    *
    * @param calendarName name of the required calendar
    * @return ProjectCalendar instance
    */
	ProjectCalendarPtr getCalendarByName(CString calendarName);

   /**
    * Retrieves the calendar referred to by the supplied unique ID
    * value. This method will return NULL if the required calendar is not
    * located.
    *
    * @param calendarID calendar unique ID
    * @return ProjectCalendar instance
    */
	ProjectCalendarPtr getCalendarByUniqueID(int calendarID);

   /**
    * This method is used to calculate the duration of work between two fixed
    * Dates according to the work schedule defined in the named calendar. The
    * calendar used is the "Standard" calendar. If this calendar does not exist,
    * and exception will be thrown.
    *
    * @param startDate start of the period
    * @param endDate end of the period
    * @return new Duration object
    * @throws MPXJException normally when no Standard calendar is available
    */
	DurationPtr getDuration(DatePtr startDate, DatePtr endDate);

   /**
    * This method is used to calculate the duration of work between two fixed
    * Dates according to the work schedule defined in the named calendar.
    * The name of the calendar to be used is passed as an argument.
    *
    * @param calendarName name of the calendar to use
    * @param startDate start of the period
    * @param endDate end of the period
    * @return new Duration object
    * @throws MPXJException normally when no Standard calendar is available
    */
	DurationPtr getDuration(CString calendarName, DatePtr startDate, DatePtr endDate);

   /**
    * This method allows an arbitrary task to be retrieved based
    * on its ID field.
    *
    * @param id task identified
    * @return the requested task, or NULL if not found
    */
	Task* getTaskByID(int id);

   /**
    * This method allows an arbitrary task to be retrieved based
    * on its UniqueID field.
    *
    * @param id task identified
    * @return the requested task, or NULL if not found
    */
	Task* getTaskByUniqueID(int id);

   /**
    * This method allows an arbitrary resource to be retrieved based
    * on its ID field.
    *
    * @param id resource identified
    * @return the requested resource, or NULL if not found
    */
	Resource2Ptr getResource2ByID(int id);

   /**
    * This method allows an arbitrary resource to be retrieved based
    * on its UniqueID field.
    *
    * @param id resource identified
    * @return the requested resource, or NULL if not found
    */
	Resource2Ptr getResource2ByUniqueID(int id);

   /**
    * This method is used to recreate the hierarchical structure of the
    * project file from scratch. The method sorts the list of all tasks,
    * then iterates through it creating the parent-child structure defined
    * by the outline level field.
    */
	void updateStructure();

	void debugPrintAllTasks();

   /**
    * This method is called to ensure that after a project file has been
    * read, the cached unique ID values used to generate new unique IDs
    * start after the end of the existing set of unique IDs.
    */
	void updateUniqueCounters();

   /**
    * Find the earliest task start Date. We treat this as the
    * start DatePtr for the project.
    *
    * @return start Date
    */
	DatePtr getStartDate();

   /**
    * Find the latest task finish Date. We treat this as the
    * finish DatePtr for the project.
    *
    * @return finish Date
    */
	DatePtr getFinishDate();

   /**
    * This method is called to alert project listeners to the fact that
    * a task has been read from a project file->
    *
    * @param task task instance
    */
	void fireTaskReadEvent(Task* task);

   /**
    * This method is called to alert project listeners to the fact that
    * a task has been written to a project file->
    *
    * @param task task instance
    */
	void fireTaskWrittenEvent(Task* task);

   /**
    * This method is called to alert project listeners to the fact that
    * a resource has been read from a project file->
    *
    * @param resource resource instance
    */
	void fireResource2ReadEvent(Resource2Ptr resource);

   /**
    * This method is called to alert project listeners to the fact that
    * a resource has been written to a project file->
    *
    * @param resource resource instance
    */
	void fireResource2WrittenEvent(Resource2Ptr resource);

   /**
    * This method is called to alert project listeners to the fact that
    * a calendar has been read from a project file->
    *
    * @param calendar calendar instance
    */
	void fireCalendarReadEvent(ProjectCalendarPtr calendar);

   /**
    * This method is called to alert project listeners to the fact that
    * a resource assignment has been read from a project file->
    *
    * @param resourceAssignment resourceAssignment instance
    */
	void fireAssignmentReadEvent(Resource2AssignmentPtr resourceAssignment);

   /**
    * This method is called to alert project listeners to the fact that
    * a resource assignment has been written to a project file->
    *
    * @param resourceAssignment resourceAssignment instance
    */
	void fireAssignmentWrittenEvent(Resource2AssignmentPtr resourceAssignment);

   /**
    * This method is called to alert project listeners to the fact that
    * a relation has been read from a project file->
    *
    * @param relation relation instance
    */
	void fireRelationReadEvent(RelationPtr relation);

   /**
    * This method is called to alert project listeners to the fact that
    * a relation has been written to a project file->
    *
    * @param relation relation instance
    */
	void fireRelationWrittenEvent(RelationPtr relation);

   /**
    * This method is called to alert project listeners to the fact that
    * a calendar has been written to a project file->
    *
    * @param calendar calendar instance
    */
	void fireCalendarWrittenEvent(ProjectCalendarPtr calendar);

   /**
    * Adds a listener to this project file->
    *
    * @param listener listener instance
    */
	void addProjectListener(ProjectListener* listener);

   /**
    * Adds a collection of listeners to the current project.
    * 
    * @param listeners collection of listeners
    */
	void addProjectListeners(List<ProjectListener*>* listeners);

   /**
    * Removes a listener from this project file->
    *
    * @param listener listener instance
    */
	void removeProjectListener(ProjectListener* listener);

   /**
    * Associates an alias with a custom task field number.
    *
    * @param field custom field number
    * @param alias alias text
    */
	void setTaskFieldAlias(TaskField field, CString alias);

   /**
    * Retrieves the alias associated with a custom task field.
    * This method will return NULL if no alias has been defined for
    * this field.
    *
    * @param field task field instance
    * @return alias text
    */
	CString getTaskFieldAlias(TaskField field);

   /**
    * Retrieves a task field instance based on its alias. If the
    * alias is not recognised, this method will return NULL.
    *
    * @param alias alias text
    * @return task field instance
    */
	TaskField getAliasTaskField(CString alias);

   /**
    * Associates a value list with a custom task field number.
    *
    * @param field custom field number
    * @param values values for the value list
    */
	void setTaskFieldValueList(TaskField field, std::shared_ptr<List<ObjectPtr>> values);

   /**
    * Retrieves the value list associated with a custom task field.
    * This method will return NULL if no value list has been defined for
    * this field.
    *
    * @param field task field instance
    * @return alias text
    */
	std::shared_ptr<List<ObjectPtr>> getTaskFieldValueList(TaskField field);

   /**
    * Associates a descriptions for value list with a custom task field number.
    *
    * @param field custom field number
    * @param descriptions descriptions for the value list
    */
	void setTaskFieldDescriptionList(TaskField field, std::shared_ptr<List<CString>> descriptions);

   /**
    * Retrieves the description value list associated with a custom task field.
    * This method will return NULL if no descriptions for the value list has been defined for
    * this field.
    *
    * @param field task field instance
    * @return alias text
    */
	std::shared_ptr<List<CString>> getTaskFieldDescriptionList(TaskField field);

   /**
    * Associates an alias with a custom resource field number.
    *
    * @param field custom field number
    * @param alias alias text
    */
	void setResource2FieldAlias(Resource2Field field, CString alias);

   /**
    * Retrieves the alias associated with a custom resource field.
    * This method will return NULL if no alias has been defined for
    * this field.
    *
    * @param field field number
    * @return alias text
    */
	CString getResource2FieldAlias(Resource2Field field);

   /**
    * Retrieves a resource field based on its alias. If the
    * alias is not recognised, this method will return NULL.
    *
    * @param alias alias text
    * @return resource field instance
    */
	Resource2Field getAliasResource2Field(CString alias);

   /**
    * Allows derived classes to gain access to the mapping between
    * task fields and aliases.
    *
    * @return task field to alias map
    */
	std::map<TaskField, CString>* getTaskFieldAliasMap();

   /**
    * Allows callers to gain access to the mapping between
    * resource field numbers and aliases.
    *
    * @return resource field to alias map
    */
	std::map<Resource2Field, CString>* getResource2FieldAliasMap();

protected:
   /**
    * Removes an id-to-task mapping.
    *
    * @param id task unique ID
    */
	void unmapTaskUniqueID(int id);

   /**
    * Adds an id-to-task mapping.
    *
    * @param id task unique ID
    * @param task task instance
    */
	void mapTaskUniqueID(int id, Task* task);

   /**
    * Removes an id-to-task mapping.
    *
    * @param id task ID
    */
	void unmapTaskID(int id);

   /**
    * Adds an id-to-task mapping.
    *
    * @param id task ID
    * @param task task instance
    */
	void mapTaskID(int id, Task* task);

   /**
    * Removes an id-to-resource mapping.
    *
    * @param id resource unique ID
    */
	void unmapResource2UniqueID(int id);

   /**
    * Adds an id-to-resource mapping.
    *
    * @param id resource unique ID
    * @param resource resource instance
    */
	void mapResource2UniqueID(int id, Resource2Ptr resource);

   /**
    * Removes an id-to-resource mapping.
    *
    * @param id resource ID
    */
	void unmapResource2ID(int id);

   /**
    * Adds an id-to-resource mapping.
    *
    * @param id resource ID
    * @param resource resource instance
    */
	void mapResource2ID(int id, Resource2Ptr resource);

   /**
    * Removes an id-to-calendar mapping.
    *
    * @param id calendar unique ID
    */
	void unmapCalendarUniqueID(int id);

   /**
    * Adds an id-to-calendar mapping.
    *
    * @param id calendar unique ID
    * @param calendar calendar instance
    */
	void mapCalendarUniqueID(int id, ProjectCalendarPtr calendar);

public:
   /**
    * This method retrieves a value representing the type of MPP file
    * that has been read. Currently this method will return the value 8 for
    * an MPP8 file (Project 98), 9 for an MPP9 file (Project 2000 and
    * Project 2002), 12 for an MPP12 file (Project 2003, Project 2007) and 14 for an
    * MPP14 file (Project 2010 and Project 2013).
    *
    * @return integer representing the file type
    */
	int getMppFileType();

   /**
    * Used internally to set the file type.
    *
    * @param fileType file type
    */
	void setMppFileType(int fileType);

   /**
    * Retrieves the name of the application used to create this project data.
    * 
    * @return application name
    */
	CString getApplicationName();

   /**
    * Sets the name of the application used to create this projectdata.
    * 
    * @param name application name
    */
	void setApplicationName(CString name);

   /**
    * Package-private method used to add views to this MPP file->
    *
    * @param view view data
    */
	void addView(ViewPtr view);

   /**
    * This method returns a list of the views defined in this MPP file->
    *
    * @return list of views
    */
	List<ViewPtr>* getViews();

   /**
    * Package-private method used to add tables to this MPP file->
    *
    * @param table table data
    */
	void addTable(TablePtr table);

   /**
    * This method returns a list of the tables defined in this MPP file->
    *
    * @return list of tables
    */
	List<TablePtr>* getTables();

   /**
    * Adds a filter definition to this project file->
    * 
    * @param filter filter definition
    */
	void addFilter(FilterPtr filter);

   /**
    * Removes a filter from this project file->
    *
    * @param filterName The name of the filter
    */
	void removeFilter(CString filterName);

   /**
    * Retrieves a list of all resource filters.
    * 
    * @return list of all resource filters
    */
	List<FilterPtr>* getAllResource2Filters();

   /**
    * Retrieves a list of all task filters.
    * 
    * @return list of all task filters
    */
	List<FilterPtr>* getAllTaskFilters();

   /**
    * Retrieve a given filter by name.
    * 
    * @param name filter name
    * @return filter instance
    */
	FilterPtr getFilterByName(CString name);

   /**
    * Retrieve a given filter by ID.
    * 
    * @param id filter ID
    * @return filter instance
    */
	FilterPtr getFilterByID(int id);

   /**
    * Retrieves a list of all groups.
    * 
    * @return list of all groups
    */
	List<Group2Ptr>* getAllGroup2s();

   /**
    * Retrieve a given group by name.
    * 
    * @param name group name
    * @return Group2 instance
    */
	Group2Ptr getGroup2ByName(CString name);

   /**
    * Adds a group definition to this project file->
    * 
    * @param group group definition
    */
	void addGroup2(Group2Ptr group);

 //  /**
 //   * Adds the definition of a graphical indicator for a field type.
 //   * 
 //   * @param field field type
 //   * @param indicator graphical indicator definition
 //   */
	//void addGraphicalIndicator(FieldType* field, GraphicalIndicatorPtr indicator);

 //  /**
 //   * Retrieves the definition of any graphical indicators used for the
 //   * given field type.
 //   * 
 //   * @param field field type
 //   * @return graphical indicator definition
 //   */
	//GraphicalIndicatorPtr getGraphicalIndicator(FieldType* field);

   /**
    * Utility method to retrieve the definition of a task table by name.
    * This method will return NULL if the table name is not recognised.
    *
    * @param name table name
    * @return table instance
    */
	TablePtr getTaskTableByName(CString name);

   /**
    * Utility method to retrieve the definition of a resource table by name.
    * This method will return NULL if the table name is not recognised.
    *
    * @param name table name
    * @return table instance
    */
	TablePtr getResource2TableByName(CString name);

   /**
    * This package-private method is used to add resource sub project details.
    *
    * @param project sub project
    */
	void setResource2SubProject(SubProjectPtr project);

   /**
    * Retrieves details of the sub project file used as a resource pool.
    *
    * @return sub project details
    */
	SubProjectPtr getResource2SubProject();

   /**
    * This package-private method is used to add sub project details.
    *
    * @param project sub project
    */
	void addSubProject(SubProjectPtr project);

   /**
    * Retrieves all the subprojects for this MPX file->
    *
    * @return all sub project details
    */
	List<SubProjectPtr>* getAllSubProjects();

   /**
    * Retrieve a flag indicating if auto filter is enabled.
    * 
    * @return auto filter flag
    */
	bool getAutoFilter();

   /**
    * Sets a flag indicating if auto filter is enabled.
    * 
    * @param autoFilter bool flag
    */
	void setAutoFilter(bool autoFilter);

   /**
    * Set the saved view state associated with this file->
    * 
    * @param viewState view state
    */
	void setViewState(ViewStatePtr viewState);

   /**
    * Retrieve the saved view state associated with this file->
    * 
    * @return view state
    */
	ViewStatePtr getViewState();

   /**
    * Set whether the data in this file is encoded.
    * 
    * @param encoded True if the data is encoded in the file
    */
	void setEncoded(bool encoded);

   /**
    * Get whether the data in this file is encoded.
    * 
    * @return encoded
    */
	bool getEncoded();

   /**
    * Set the key with which this data is encrypted (can be decrypted) with.
    * 
    * @param encryptionKey Encryption key
    */
	void setEncryptionCode(BYTE encryptionKey);

   /**
    * Get the key with which this data is encrypted (can be decrypted) with.
    * 
    * @return m_encryptionKey
    */
	BYTE getEncryptionCode();

   /**
    * Sets the project file path.
    *
    * @param projectFilePath project file path
    */
	void setProjectFilePath(CString projectFilePath);

   /**
    * Gets the project file path.
    *
    * @return project file path
    */
	CString getProjectFilePath();

   /**
    * Add a custom field value list item.
    * 
    * @param item CustomFieldValueItem instance
    */
	void addCustomFieldValueItem(CustomFieldValueItemPtr item);

   /**
    * Get the custom field value list item with the given unique ID.
    * 
    * @param uniqueID unique ID
    * @return CustomFieldValueItem instance
    */
	CustomFieldValueItemPtr getCustomFieldValueItem(int uniqueID);

   /**
    * Retrieves the default calendar for this project based on the calendar name
    * given in the project header. If a calendar of this name cannot be found, then
    * the first calendar listed for the project will be returned. If the
    * project contains no calendars, then a default calendar is added. 
    * 
    * @return default projectCalendar instance
    */
	ProjectCalendarPtr getCalendar();

   /**
    * Sets the default calendar for this project.
    * 
    * @param calendar default calendar instance
    */
	void setCalendar(ProjectCalendarPtr calendar);

   /**
    * Retrieve the calendar used internally for timephased baseline calculation.
    * 
    * @return baseline calendar
    */
	ProjectCalendarPtr getBaselineCalendar();


private:

	/**
	* Called recursively to renumber child task IDs.
	*
	* @param parentTask parent task instance
	* @param currentID current task ID
	* @return upDated current task ID
	*/
	int synchroizeTaskIDToHierarchy(Task* parentTask, int currentID);
private:
	/**
	* Renumbers all task unique IDs.
	*/
	void renumberTaskUniqueIDs();

	/**
	* Renumbers all resource unique IDs.
	*/
	void renumberResource2UniqueIDs();

	/**
	* Renumbers all assignment unique IDs.
	*/
	void renumberAssignmentUniqueIDs();

	/**
	* Renumbers all calendar unique IDs.
	*/
	void renumberCalendarUniqueIDs();

private:
   CString m_projectFilePath;

   /**
    * Counter used to populate the unique ID field of a task.
    */
   int m_taskUniqueID;

   /**
    * Counter used to populate the unique ID field of a calendar.
    */
   int m_calendarUniqueID;

   /**
    * Counter used to populate the unique ID field of an assignment.
    */
   int m_assignmentUniqueID;

   /**
    * Counter used to populate the ID field of a task.
    */
   int m_taskID;

   /**
    * Counter used to populate the unique ID field of a resource.
    */
   int m_resourceUniqueID;

   /**
    * Counter used to populate the ID field of a resource.
    */
   int m_resourceID;

   /**
    * This list holds a reference to all resources defined in the
    * MPX file->
    */
   List<Resource2Ptr> m_allResource2s;// = new List<Resource2>();

   /**
    * This list holds a reference to all tasks defined in the
    * MPX file->
    */
   List<Task*> m_allTasks;// = new List<Task>();

   /**
    * List holding references to the top level tasks
    * as defined by the outline level.
    */
   List<Task*> m_childTasks;// = new List<Task>();

   /**
    * This list holds a reference to all resource assignments defined in the
    * MPX file->
    */
   List<Resource2AssignmentPtr> m_allResource2Assignments;// = new List<Resource2Assignment>();

   /**
    * List holding references to all calendars.
    */
   List<ProjectCalendarPtr> m_calendars;// = new List<ProjectCalendar>();

   /**
    * File creation record.
    */
   FileCreationRecord* m_fileCreationRecord;// = new FileCreationRecord(this);

   /**
    * Project header record.
    */
   ProjectHeaderPtr m_projectHeader;// = new ProjectHeader(this);

   /**
    * Character to be used as delimiter throughout this file->
    */
   char m_delimiter = ',';

   /**
    * Key with which this data is encrypted (can be decrypted) with.
    */
   BYTE m_encryptionKey;

   /**
    * Indicating whether the project data is encoded due to password protection.
    */
   bool m_encoded;

   /**
    * Indicating whether WBS value should be calculated on creation, or will
    * be manually set.
    */
   bool m_autoWBS = true;

   /**
    * Indicating whether the Outline Level value should be calculated on
    * creation, or will be manually set.
    */
   bool m_autoOutlineLevel = true;

   /**
    * Indicating whether the Outline NumberPtr value should be calculated on
    * creation, or will be manually set.
    */
   bool m_autoOutlineNumber = true;

   /**
    * Indicating whether the unique ID of a task should be
    * calculated on creation, or will be manually set.
    */
   bool m_autoTaskUniqueID = true;

   /**
    * Indicating whether the unique ID of a calendar should be
    * calculated on creation, or will be manually set.
    */
   bool m_autoCalendarUniqueID = true;

   /**
    * Indicating whether the unique ID of an assignment should be
    * calculated on creation, or will be manually set.
    */
   bool m_autoAssignmentUniqueID = true;

   /**
    * Indicating whether the ID of a task should be
    * calculated on creation, or will be manually set.
    */
   bool m_autoTaskID = true;

   /**
    * Indicating whether the unique ID of a resource should be
    * calculated on creation, or will be manually set.
    */
   bool m_autoResource2UniqueID = true;

   /**
    * Indicating whether the ID of a resource should be
    * calculated on creation, or will be manually set.
    */
   bool m_autoResource2ID = true;

   /**
    * Maps from a task field number to a task alias.
    */
   std::map<TaskField, CString> m_taskFieldAlias;// = new HashMap<TaskField, CString>();

   /**
    * Maps from a task field number to a value list.
    */
   std::map<TaskField, std::shared_ptr<List<ObjectPtr>>> m_taskFieldValueList;// = new HashMap<TaskField, List<Object>>();

   /**
    * Maps from a task field number to a description list.
    */
   std::map<TaskField, std::shared_ptr<List<CString>>> m_taskFieldDescriptionList;// = new HashMap<TaskField, List<CString>>();

   /**
    * Maps from a task field alias to a task field number.
    */
   std::map<CString, TaskField> m_aliasTaskField;// = new HashMap<CString, TaskField>();

   /**
    * Maps from a resource field number to a resource alias.
    */
   std::map<Resource2Field, CString> m_resourceFieldAlias;// = new HashMap<Resource2Field, CString>();

   /**
    * Maps from a resource field alias to a resource field number.
    */
   std::map<CString, Resource2Field> m_aliasResource2Field;// = new HashMap<CString, Resource2Field>();

   /**
    * Maps from a task unique ID to a task instance.
    */
   Map2<int, Task*> m_taskUniqueIDMap;// = new HashMap<int, Task>();

   /**
    * Maps from a task ID to a task instance.
    */
   Map2<int, Task*> m_taskIDMap;// = new HashMap<int, Task>();

   /**
    * Maps from a resource unique ID to a resource instance.
    */
   Map2<int, Resource2Ptr> m_resourceUniqueIDMap;// = new HashMap<int, Resource2>();

   /**
    * Maps from a resource ID to a resource instance.
    */
   Map2<int, Resource2Ptr> m_resourceIDMap;// = new HashMap<int, Resource2>();

   /**
    * Maps from a calendar unique ID to a calendar instance.
    */
   Map2<int, ProjectCalendarPtr> m_calendarUniqueIDMap;// = new HashMap<int, ProjectCalendar>();

   /**
    * List of project event listeners.
    */
   List<ProjectListener*> m_projectListeners;

   /**
    * This value is used to represent the type of MPP file that
    * has been read.
    */
   int m_mppFileType;

   /**
    * This field can be used to record the name of the application
    * used to produce the project data. At present only MppReader
    * populates this attribute.
    */
   CString m_applicationName;

   /**
    * List of views defined in this file->
    */
   List<ViewPtr> m_views;// = new ArrayList<View>();

   /**
    * List of tables defined in this file->
    */
   List<TablePtr> m_tables;// = new ArrayList<Table>();

   /**
    * Map of graphical indicator data.
    */
   //Map<FieldType*, GraphicalIndicatorPtr> m_graphicalIndicators;// = new HashMap<FieldType, GraphicalIndicator>();

   /**
   * Index of task tables by name.
   */
   std::map<CString, TablePtr> m_taskTablesByName;// = new HashMap<String, Table>();

   /**
    * Index of resource tables by name.
    */
   std::map<CString, TablePtr> m_resourceTablesByName;// = new HashMap<CString, Table>();

   /**
    * List of all task filters.
    */
   List<FilterPtr> m_taskFilters;// = new ArrayList<Filter>();

   /**
    * List of all resource filters.
    */
   List<FilterPtr> m_resourceFilters;// = new ArrayList<Filter>();

   /**
    * Index of filters by name.
    */
   std::map<CString, FilterPtr> m_filtersByName;// = new HashMap<CString, Filter>();

   /**
    * Index of filters by ID.
    */
   Map2<int, FilterPtr> m_filtersByID;// = new HashMap<int, Filter>();

   /**
    * List of all groups.
    */
   List<Group2Ptr> m_groups;// = new ArrayList<Group2>();

   /**
    * Index of groups by name.
    */
   std::map<CString, Group2Ptr> m_groupsByName;// = new HashMap<CString, Group2>();

   /**
    * Resource2Ptr sub project.
    */
   SubProjectPtr m_resourceSubProject;

   /**
    * This list holds a reference to all subprojects defined in the
    * MPX file->
    */
   List<SubProjectPtr> m_allSubProjects;// = new List<SubProject>();

   /**
    * Flag indicating if auto filter is enabled.
    */
   bool m_autoFilter;

   /**
    * Saved view state.
    */
   ViewStatePtr m_viewState;

   /***
    * Custom field value list items.
    */
   Map2<int, CustomFieldValueItemPtr> m_customFieldValueItems;// = new Map<int, CustomFieldValueItem>();

   /**
    * Maximum unique ID value MS Project will accept.
    */
   const int MS_PROJECT_MAX_UNIQUE_ID = 0x1FFFFF;
};

typedef std::shared_ptr<ProjectFile> ProjectFilePtr;