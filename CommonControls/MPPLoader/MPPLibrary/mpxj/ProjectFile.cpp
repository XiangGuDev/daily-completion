#include "stdafx.h"
#include "ProjectFile.h"

ProjectFile::ProjectFile()
	: m_taskUniqueID (0),
	m_calendarUniqueID(0),
	m_assignmentUniqueID(0),
	m_taskID(0),
	m_resourceUniqueID(0),
	m_resourceID(0),
	m_fileCreationRecord(NULL),
	m_viewState(NULL),
	m_autoFilter(false),
	m_resourceSubProject(NULL),
	m_mppFileType(0),
	m_encoded(false),
	m_encryptionKey(0)
{
	m_projectHeader.reset (new ProjectHeader(this));
}

ProjectFile::~ProjectFile()
{
	m_projectHeader.reset();

	//m_taskFieldValueList.RemoveAll();
	m_taskFieldValueList.clear();
	//m_taskFieldDescriptionList.RemoveAll();
	m_taskFieldDescriptionList.clear();
	m_taskUniqueIDMap.RemoveAll();
	m_taskIDMap.RemoveAll();
	m_resourceUniqueIDMap.RemoveAll();
	m_resourceIDMap.RemoveAll();
	m_calendarUniqueIDMap.RemoveAll();

	m_allResource2Assignments.RemoveAll();
	m_allResource2s.RemoveAll();
	m_calendars.RemoveAll();

	removeAllTask();
}
/**
* Accessor method to retrieve the current file delimiter character.
*
* @return delimiter character
*/
char ProjectFile::getDelimiter()
{
	return (m_delimiter);
}

/**
* Modifier method used to set the delimiter character.
*
* @param delimiter delimiter character
*/
void ProjectFile::setDelimiter(char delimiter)
{
	m_delimiter = delimiter;
}

/**
* This method is provided to allow child tasks that have been created
* programmatically to be added as a record to the main file->
*
* @param task task created as a child of another task
*/
void ProjectFile::addTask(Task* task)
{
	m_allTasks.Add(task);
}

/**
* This method allows a task to be added to the file programatically.
*
* @return new task object
*/
Task* ProjectFile::addTask()
{
	std::auto_ptr<Task> task (new Task(this, NULL));
	m_allTasks.Add(task.get());
	m_childTasks.Add(task.get());
	return task.release();
}

/**
* This method is used to remove a task from the project.
*
* @param task task to be removed
*/
void ProjectFile::removeTask(Task* task)
{
	//
	// Remove the task from the file and its parent task
	//
	m_allTasks.remove(task);
	m_taskUniqueIDMap.RemoveKey(task->getUniqueID());
	m_taskIDMap.RemoveKey(task->getID());

	Task* parentTask = task->getParentTask();
	if (parentTask != NULL)
	{
		parentTask->removeChildTask(task);
	}
	else
	{
		m_childTasks.remove(task);
	}

	//
	// Remove all resource assignments
	//
	for (int i = (int) m_allResource2Assignments.GetCount() - 1; i >= 0; i--)
	{
		Resource2AssignmentPtr assignment = m_allResource2Assignments[i];
		if (assignment->getTask() == task)
		{
			Resource2Ptr resource = assignment->getResource2();
			if (resource != NULL)
			{
				resource->removeResource2Assignment(assignment);
			}
			m_allResource2Assignments.RemoveAt(i);
		}
	}

	//
	// Recursively remove any child tasks
	//
	while (true)
	{
		List<Task*>* childTaskList = task->getChildTasks();
		if (childTaskList->isEmpty() == true)
		{
			break;
		}
		removeTask(childTaskList->GetAt(0));
		//removeTask(pAt);
		//pAt = NULL;
	}

	// delete
	delete task;
	task = NULL;
}

void ProjectFile::removeAllTask()
{
	while (m_allTasks.GetCount()) {
		removeTask(m_allTasks[m_allTasks.GetCount()-1]);
	}

	m_childTasks.RemoveAll();
	m_allTasks.RemoveAll();
}

/**
* This method can be called to ensure that the IDs of all
* tasks in this project are sequential, and start from an
* appropriate point. If tasks are added to and removed from
* the list of tasks, then the project is loaded into Microsoft
* project, if the ID values have gaps in the sequence, there will
* be blank task rows shown.
*/
void ProjectFile::renumberTaskIDs()
{
	if (m_allTasks.isEmpty() == false)
	{
		Collections::sort_ptr_list(m_allTasks);
		//Collections.sort(m_allTasks);
		Task* firstTask = m_allTasks.GetAt(0);
		int id = firstTask->getID();
		if (id != 0)
		{
			id = 1;
		}

		for (size_t i = 0; i < m_allTasks.GetCount(); i ++)
		{
			Task* task = m_allTasks[i];

			task->setID(id++);
		}
	}
}

/**
* This method can be called to ensure that the IDs of all
* resources in this project are sequential, and start from an
* appropriate point. If resources are added to and removed from
* the list of resources, then the project is loaded into Microsoft
* project, if the ID values have gaps in the sequence, there will
* be blank resource rows shown.
*/
void ProjectFile::renumberResource2IDs()
{
	if (m_allResource2s.isEmpty() == false)
	{
		Collections::sort_object_list(m_allResource2s);
		//Collections.sort(m_allResource2s);
		int id = 1;

		for (size_t i = 0; i < m_allResource2s.GetCount(); i ++)
		{
			Resource2Ptr resource = m_allResource2s[i];
			resource->setID(id++);
		}
	}
}

/**
* Renumbers all task unique IDs.
*/
void ProjectFile::renumberTaskUniqueIDs()
{
	Task* firstTask = getTaskByID(0);
	int uid = (firstTask == NULL ? 1 : 0);

	for (size_t i = 0; i < m_allTasks.GetCount(); i ++)
	{
		Task* task = m_allTasks[i];
		task->setUniqueID(uid++);
	}
}

/**
* Renumbers all resource unique IDs.
*/
void ProjectFile::renumberResource2UniqueIDs()
{
	int uid = 1;

	for (size_t i = 0; i < m_allResource2s.GetCount(); i++)
	{
		Resource2Ptr resource = m_allResource2s[i];
		resource->setUniqueID(uid++);
	}
}

/**
* Renumbers all assignment unique IDs.
*/
void ProjectFile::renumberAssignmentUniqueIDs()
{
	int uid = 1;
	for (size_t i = 0; i < m_allResource2Assignments.GetCount(); i++)
	{
		Resource2AssignmentPtr assignment = m_allResource2Assignments[i];
		assignment->setUniqueID(NumberPtr(new Number(uid++)));
	}
}

/**
* Renumbers all calendar unique IDs.
*/
void ProjectFile::renumberCalendarUniqueIDs()
{
	int uid = 1;

	for (size_t i = 0; i < m_calendars.GetCount(); i++)
	{
		ProjectCalendarPtr calendar = m_calendars[i];
		calendar->setUniqueID(uid++);
	}
}

/**
* This method is called to ensure that all unique ID values
* held by MPXJ are within the range supported by MS Project.
* If any of these values fall outside of this range, the unique IDs
* of the relevant entities are renumbered.
*/
void ProjectFile::validateUniqueIDsForMicrosoftProject()
{
	if (!m_allTasks.isEmpty())
	{
		for (size_t i = 0; i <m_allTasks.GetCount(); i ++)
		{
			Task* task = m_allTasks[i];

			if (task->getUniqueID() > MS_PROJECT_MAX_UNIQUE_ID)
			{
				renumberTaskUniqueIDs();
				break;
			}
		}
	}

	if (!m_allResource2s.isEmpty())
	{
		for (size_t i = 0; i < m_allResource2s.GetCount(); i ++)
		{
			Resource2Ptr resource = m_allResource2s[i];
			if (resource->getUniqueID() > MS_PROJECT_MAX_UNIQUE_ID)
			{
				renumberResource2UniqueIDs();
				break;
			}
		}
	}

	if (!m_allResource2Assignments.isEmpty())
	{
		for (size_t i = 0; i < m_allResource2Assignments.GetCount(); i ++)
		{
			Resource2AssignmentPtr assignment = m_allResource2Assignments[i];
			if (NumberHelper::getInt(assignment->getUniqueID()) > MS_PROJECT_MAX_UNIQUE_ID)
			{
				renumberAssignmentUniqueIDs();
				break;
			}
		}
	}

	if (!m_calendars.isEmpty())
	{
		for (size_t i = 0; i < m_calendars.GetCount(); i++)
		{
			ProjectCalendarPtr calendar = m_calendars[i];
			if (calendar->getUniqueID() > MS_PROJECT_MAX_UNIQUE_ID)
			{
				renumberCalendarUniqueIDs();
				break;
			}
		}
	}
}

/**
* Microsoft Project bases the order of tasks displayed on their ID
* value. This method takes the hierarchical structure of tasks
* represented in MPXJ and renumbers the ID values to ensure that
* this structure is displayed as expected in Microsoft Project. This
* is typically used to deal with the case where a hierarchical task
* structure has been created programatically in MPXJ.
*/
void ProjectFile::synchronizeTaskIDToHierarchy()
{
	m_allTasks.RemoveAll();

	int currentID = (getTaskByID(0) == NULL ? 1 : 0);
	for (size_t i = 0; i < m_childTasks.GetCount(); i ++)
	{
		Task* task = m_childTasks[i];

		task->setID(currentID++);
		m_allTasks.Add(task);
		currentID = synchroizeTaskIDToHierarchy(task, currentID);
	}
}

/**
* Called recursively to renumber child task IDs.
*
* @param parentTask parent task instance
* @param currentID current task ID
* @return upDated current task ID
*/
int ProjectFile::synchroizeTaskIDToHierarchy(Task* parentTask, int currentID)
{
	for (size_t i = 0; i < parentTask->getChildTasks()->GetCount(); i ++)
	{
		Task* task = parentTask->getChildTasks()->GetAt(i);

		task->setID(currentID++);
		m_allTasks.Add(task);
		currentID = synchroizeTaskIDToHierarchy(task, currentID);
	}
	return currentID;
}

/**
* This method is used to retrieve a list of all of the top level tasks
* that are defined in this project file->
*
* @return list of tasks
*/
List<Task*>* ProjectFile::getChildTasks()
{
	return &m_childTasks;
}

/**
* This method is used to retrieve a list of all of the tasks
* that are defined in this project file->
*
* @return list of all tasks
*/
List<Task*>* ProjectFile::getAllTasks()
{
	return &m_allTasks;
}

/**
* Used to set whether WBS numbers are automatically created.
*
* @param flag true if automatic WBS required.
*/
void ProjectFile::setAutoWBS(bool flag)
{
	m_autoWBS = flag;
}

/**
* Used to set whether outline level numbers are automatically created.
*
* @param flag true if automatic outline level required.
*/
void ProjectFile::setAutoOutlineLevel(bool flag)
{
	m_autoOutlineLevel = flag;
}

/**
* Used to set whether outline numbers are automatically created.
*
* @param flag true if automatic outline number required.
*/
void ProjectFile::setAutoOutlineNumber(bool flag)
{
	m_autoOutlineNumber = flag;
}

/**
* Used to set whether the task unique ID field is automatically populated.
*
* @param flag true if automatic unique ID required.
*/
void ProjectFile::setAutoTaskUniqueID(bool flag)
{
	m_autoTaskUniqueID = flag;
}

/**
* Used to set whether the calendar unique ID field is automatically populated.
*
* @param flag true if automatic unique ID required.
*/
void ProjectFile::setAutoCalendarUniqueID(bool flag)
{
	m_autoCalendarUniqueID = flag;
}

/**
* Used to set whether the assignment unique ID field is automatically populated.
*
* @param flag true if automatic unique ID required.
*/
void ProjectFile::setAutoAssignmentUniqueID(bool flag)
{
	m_autoAssignmentUniqueID = flag;
}

/**
* Used to set whether the task ID field is automatically populated.
*
* @param flag true if automatic ID required.
*/
void ProjectFile::setAutoTaskID(bool flag)
{
	m_autoTaskID = flag;
}

/**
* Retrieve the flag that determines whether WBS is generated
* automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoWBS()
{
	return (m_autoWBS);
}

/**
* Retrieve the flag that determines whether outline level is generated
* automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoOutlineLevel()
{
	return (m_autoOutlineLevel);
}

/**
* Retrieve the flag that determines whether outline numbers are generated
* automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoOutlineNumber()
{
	return (m_autoOutlineNumber);
}

/**
* Retrieve the flag that determines whether the task unique ID
* is generated automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoTaskUniqueID()
{
	return (m_autoTaskUniqueID);
}

/**
* Retrieve the flag that determines whether the calendar unique ID
* is generated automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoCalendarUniqueID()
{
	return (m_autoCalendarUniqueID);
}

/**
* Retrieve the flag that determines whether the assignment unique ID
* is generated automatically.
*
* @return bool, default is true.
*/
bool ProjectFile::getAutoAssignmentUniqueID()
{
	return (m_autoAssignmentUniqueID);
}

/**
* Retrieve the flag that determines whether the task ID
* is generated automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoTaskID()
{
	return (m_autoTaskID);
}

/**
* This method is used to retrieve the next unique ID for a task.
*
* @return next unique ID
*/
int ProjectFile::getTaskUniqueID()
{
	return (++m_taskUniqueID);
}

/**
* This method is used to retrieve the next unique ID for a calendar->
*
* @return next unique ID
*/
int ProjectFile::getCalendarUniqueID()
{
	return (++m_calendarUniqueID);
}

/**
* This method is used to retrieve the next unique ID for an assignment->
*
* @return next unique ID
*/
int ProjectFile::getAssignmentUniqueID()
{
	return (++m_assignmentUniqueID);
}

/**
* This method is used to retrieve the next ID for a task.
*
* @return next ID
*/
int ProjectFile::getTaskID()
{
	return (++m_taskID);
}

/**
* Used to set whether the resource unique ID field is automatically populated.
*
* @param flag true if automatic unique ID required.
*/
void ProjectFile::setAutoResource2UniqueID(bool flag)
{
	m_autoResource2UniqueID = flag;
}

/**
* Used to set whether the resource ID field is automatically populated.
*
* @param flag true if automatic ID required.
*/
void ProjectFile::setAutoResource2ID(bool flag)
{
	m_autoResource2ID = flag;
}

/**
* Retrieve the flag that determines whether the resource unique ID
* is generated automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoResource2UniqueID()
{
	return (m_autoResource2UniqueID);
}

/**
* Retrieve the flag that determines whether the resource ID
* is generated automatically.
*
* @return bool, default is false.
*/
bool ProjectFile::getAutoResource2ID()
{
	return (m_autoResource2ID);
}

/**
* This method is used to retrieve the next unique ID for a resource->
*
* @return next unique ID
*/
int ProjectFile::getResource2UniqueID()
{
	return (++m_resourceUniqueID);
}

/**
* This method is used to retrieve the next ID for a resource->
*
* @return next ID
*/
int ProjectFile::getResource2ID()
{
	return (++m_resourceID);
}

/**
* Retrieves the file creation record.
*
* @return file creation record.
*/
FileCreationRecord* ProjectFile::getFileCreationRecord()
{
	return (m_fileCreationRecord);
}

/**
* This method is used to add a new calendar to the file->
*
* @return new calendar object
*/
ProjectCalendarPtr ProjectFile::addCalendar()
{
	ProjectCalendarPtr calendar (new ProjectCalendar(this));
	m_calendars.Add(calendar);
	return (calendar);
}

/**
* Removes a calendar->
*
* @param calendar calendar to be removed
*/
void ProjectFile::removeCalendar(ProjectCalendarPtr calendar)
{
	if (m_calendars.contains(calendar))
	{
		m_calendars.remove(calendar);
	}

	Resource2Ptr resource = calendar->getResource2();
	if (resource)
	{
		resource->setResource2Calendar(NULL);
	}

	calendar->setParent(NULL);
}

/**
* This is a convenience method used to add a calendar called
* "Standard" to the file, and populate it with a default working week
* and default working hours.
*
* @return a new default calendar
*/
ProjectCalendarPtr ProjectFile::addDefaultBaseCalendar()
{
	ProjectCalendarPtr calendar = addCalendar();

	calendar->setName(ProjectCalendar::DEFAULT_BASE_CALENDAR_NAME);

	calendar->setWorkingDay(Day::SUNDAY, false);
	calendar->setWorkingDay(Day::MONDAY, true);
	calendar->setWorkingDay(Day::TUESDAY, true);
	calendar->setWorkingDay(Day::WEDNESDAY, true);
	calendar->setWorkingDay(Day::THURSDAY, true);
	calendar->setWorkingDay(Day::FRIDAY, true);
	calendar->setWorkingDay(Day::SATURDAY, false);

	calendar->addDefaultCalendarHours();

	return (calendar);
}

/**
* This is a protected convenience method to add a default derived
* calendar->
*
* @return new ProjectCalendar* instance
*/
ProjectCalendarPtr ProjectFile::addDefaultDerivedCalendar()
{
	ProjectCalendarPtr calendar = addCalendar();

	calendar->setWorkingDay(Day::SUNDAY, DayType::DEFAULT);
	calendar->setWorkingDay(Day::MONDAY, DayType::DEFAULT);
	calendar->setWorkingDay(Day::TUESDAY, DayType::DEFAULT);
	calendar->setWorkingDay(Day::WEDNESDAY, DayType::DEFAULT);
	calendar->setWorkingDay(Day::THURSDAY, DayType::DEFAULT);
	calendar->setWorkingDay(Day::FRIDAY, DayType::DEFAULT);
	calendar->setWorkingDay(Day::SATURDAY, DayType::DEFAULT);

	return (calendar);
}

/**
* This method retrieves the list of calendars defined in
* this file->
*
* @return list of calendars
*/
List<ProjectCalendarPtr>* ProjectFile::getCalendars()
{
	return &m_calendars;
}

/**
* This method is used to retrieve the project header record.
*
* @return project header object
*/
ProjectHeader* ProjectFile::getProjectHeader()
{
	return m_projectHeader.get();
}

/**
* This method is used to add a new resource to the file->
*
* @return new resource object
*/
Resource2Ptr ProjectFile::addResource2()
{
	Resource2Ptr resource (new Resource2(this));
	m_allResource2s.Add(resource);
	return resource;
}

/**
* This method is used to remove a resource from the project.
*
* @param resource resource to be removed
*/
void ProjectFile::removeResource2(Resource2Ptr resource)
{
	m_allResource2s.remove(Resource2Ptr(resource));
	m_resourceUniqueIDMap.RemoveKey(resource->getUniqueID());
	m_resourceIDMap.RemoveKey(resource->getID());

	//Iterator<Resource2Assignment> iter = m_allResource2Assignments.iterator();
	int resourceUniqueID = resource->getUniqueID();

	//欧阳友章修改  size_t做倒序for循环出现问题 改为int
	//for (size_t i = m_allResource2Assignments.GetCount() - 1; i >= 0; i --)
	for (int i = (int)m_allResource2Assignments.GetCount() - 1; i >= 0; i--)
	{
		Resource2AssignmentPtr assignment = m_allResource2Assignments[i];
		NumberPtr assignmentResUniqueId = assignment->getResource2UniqueID();
		if (assignmentResUniqueId && assignmentResUniqueId->intValue() == resourceUniqueID)
		{
			assignment->getTask()->removeResource2Assignment(assignment);
			//iter.remove();
			m_allResource2Assignments.RemoveAt(i);
		}
	}

	ProjectCalendarPtr calendar = resource->getResource2Calendar();
	if (calendar)
	{
		calendar->remove();
	}
}

/**
* Retrieves a list of all resources in this project.
*
* @return list of all resources
*/
List<Resource2Ptr>* ProjectFile::getAllResource2s()
{
	return &m_allResource2s;
}

/**
* Retrieves a list of all resource assignments in this project.
*
* @return list of all resources
*/
List<Resource2AssignmentPtr>* ProjectFile::getAllResource2Assignments()
{
	return &m_allResource2Assignments;
}

/**
* This method is provided to allow resource assignments that have been
* created programatically to be added as a record to the main file->
*
* @param assignment Resource2Ptr assignment created as part of a task
*/
void ProjectFile::addResource2Assignment(Resource2AssignmentPtr assignment)
{
	m_allResource2Assignments.Add(assignment);
}

/**
* This method removes a resource assignment from the internal storage
* maintained by the project file->
*
* @param assignment resource assignment to remove
*/
void ProjectFile::removeResource2Assignment(Resource2AssignmentPtr assignment)
{
	m_allResource2Assignments.remove(assignment);
	assignment->getTask()->removeResource2Assignment(assignment);
	Resource2Ptr resource = assignment->getResource2();
	if (resource != NULL)
	{
		resource->removeResource2Assignment(assignment);
	}
}

/**
* This method has been provided to allow the subclasses to
* instantiate ResourecAssignment instances.
*
* @param task parent task
* @return new resource assignment instance
*/
Resource2AssignmentPtr ProjectFile::newResource2Assignment(Task* task)
{
	return Resource2AssignmentPtr(new Resource2Assignment(this, task));
}

/**
* Retrieves the named calendar-> This method will return
* NULL if the named calendar is not located.
*
* @param calendarName name of the required calendar
* @return ProjectCalendar* instance
*/
ProjectCalendarPtr ProjectFile::getCalendarByName(CString calendarName)
{
	ProjectCalendarPtr calendar;

	if (calendarName.GetLength() != 0)
	{
		//Iterator<ProjectCalendar> iter = m_calendars.iterator();
		//while (iter.hasNext() == true)
		for (size_t i = 0; i < m_calendars.GetCount(); i ++)
		{
			calendar = m_calendars[i];
			CString name = calendar->getName();

			if (name.CompareNoCase(calendarName) == 0)
			{
				break;
			}

			calendar = NULL;
		}
	}

	return (calendar);
}

/**
* Retrieves the calendar referred to by the supplied unique ID
* value. This method will return NULL if the required calendar is not
* located.
*
* @param calendarID calendar unique ID
* @return ProjectCalendar* instance
*/
ProjectCalendarPtr ProjectFile::getCalendarByUniqueID(int calendarID)
{
	ProjectCalendarPtr cal;
	m_calendarUniqueIDMap.Lookup(calendarID, cal);
	return cal;
}

/**
* This method is used to calculate the duration of work between two fixed
* Dates according to the work schedule defined in the named calendar-> The
* calendar used is the "Standard" calendar-> If this calendar does not exist,
* and exception will be thrown.
*
* @param startDate start of the period
* @param endDate end of the period
* @return new Duration object
* @throws MPXJException normally when no Standard calendar is available
*/
DurationPtr ProjectFile::getDuration(DatePtr startDate, DatePtr endDate)
{
	return (getDuration(L"Standard", startDate, endDate));
}

/**
* This method is used to calculate the duration of work between two fixed
* Dates according to the work schedule defined in the named calendar->
* The name of the calendar to be used is passed as an argument.
*
* @param calendarName name of the calendar to use
* @param startDate start of the period
* @param endDate end of the period
* @return new Duration object
* @throws MPXJException normally when no Standard calendar is available
*/
DurationPtr ProjectFile::getDuration(CString calendarName, DatePtr startDate, DatePtr endDate)
{
	ProjectCalendarPtr calendar = getCalendarByName(calendarName);

	if (!calendar)
	{
		throw std::bad_exception();// new MPXJException(MPXJException.CALENDAR_ERROR + ": " + calendarName);
	}

	return (calendar->getDuration(startDate, endDate));
}

/**
* This method allows an arbitrary task to be retrieved based
* on its ID field.
*
* @param id task identified
* @return the requested task, or NULL if not found
*/
Task* ProjectFile::getTaskByID(int id)
{
	Task* task = NULL;
	m_taskIDMap.Lookup(id, task);
	return task;
}

/**
* This method allows an arbitrary task to be retrieved based
* on its UniqueID field.
*
* @param id task identified
* @return the requested task, or NULL if not found
*/
Task* ProjectFile::getTaskByUniqueID(int id)
{
	Task* task = NULL;
	m_taskUniqueIDMap.Lookup(id, task);
	return task;
}

/**
* This method allows an arbitrary resource to be retrieved based
* on its ID field.
*
* @param id resource identified
* @return the requested resource, or NULL if not found
*/
Resource2Ptr ProjectFile::getResource2ByID(int id)
{
	Resource2Ptr res = NULL;
	m_resourceIDMap.Lookup(id, res);
	return res;
}

/**
* This method allows an arbitrary resource to be retrieved based
* on its UniqueID field.
*
* @param id resource identified
* @return the requested resource, or NULL if not found
*/
Resource2Ptr ProjectFile::getResource2ByUniqueID(int id)
{
	Resource2Ptr res = NULL;
	m_resourceUniqueIDMap.Lookup(id, res);
	return res;
}

/**
* This method is used to recreate the hierarchical structure of the
* project file from scratch. The method sorts the list of all tasks,
* then iterates through it creating the parent-child structure defined
* by the outline level field.
*/
void ProjectFile::updateStructure()
{
	if (m_allTasks.GetCount() <= 1)
		return;
	//debugPrintAllTasks();
	Collections::sort_ptr_list(m_allTasks);
	//debugPrintAllTasks();
	m_childTasks.RemoveAll();

	Task* lastTask = NULL;
	int lastLevel = -1;

	for (size_t i = 0; i < m_allTasks.GetCount(); i ++)
	{
		Task* task = m_allTasks[i];

		task->clearChildTasks();
		Task* parent = NULL;
		if (!task->getNull())
		{
			int level = task->getOutlineLevel();

			if (lastTask != NULL)
			{
				if (level == lastLevel || task->getNull())
				{
					parent = lastTask->getParentTask();
					level = lastLevel;
				}
				else
				{
					if (level > lastLevel)
					{
						parent = lastTask;
					}
					else
					{
						while (level <= lastLevel)
						{
							parent = lastTask->getParentTask();

							if (parent == NULL)
							{
								break;
							}

							lastLevel = parent->getOutlineLevel();
							lastTask = parent;
						}
					}
				}
			}

			lastTask = task;
			lastLevel = level;

			if (getAutoWBS() || task->getWBS().GetLength() == 0)
			{
				task->generateWBS(parent);
			}

			if (getAutoOutlineNumber())
			{
				task->generateOutlineNumber(parent);
			}
		}

		if (parent == NULL)
		{
			m_childTasks.Add(task);
		}
		else
		{
			parent->addChildTask(task);
		}
	}
}

void ProjectFile::debugPrintAllTasks()
{
	for (size_t i = 0; i < m_allTasks.GetCount(); i++)


	{
		Task* task = m_allTasks[i];
		if (!task) continue;
		int level = task->getOutlineLevel();
		CString name = task->getName();
		
		CString resName;
		List<Resource2AssignmentPtr>* ResList = task->getResource2Assignments();
		for (int i = 0; i < (int)ResList->GetCount(); i++) {
			Resource2Ptr res = ResList->GetAt(i)->getResource2();
			if (!res) continue;
			if (resName.GetLength() > 0) resName += L",";
			resName += res->getName();
		}

		ATLTRACE(L"%d  outline:%d  id:%d  %s\tres:%s\n", i + 1, level, task->getID(), name, resName);
	}
}

/**
* This method is called to ensure that after a project file has been
* read, the cached unique ID values used to generate new unique IDs
* start after the end of the existing set of unique IDs.
*/
void ProjectFile::updateUniqueCounters()
{
	//
	// UpDate task unique IDs
	//
	for (size_t i = 0; i < m_allTasks.GetCount(); i++)
	{
		Task* task = m_allTasks[i];
		int uniqueID = task->getUniqueID();
		if (uniqueID > m_taskUniqueID)
		{
			m_taskUniqueID = uniqueID;
		}
	}

	//
	// UpDate resource unique IDs
	//
	for (size_t i = 0; i < m_allResource2s.GetCount(); i++)
	{
		Resource2Ptr resource = m_allResource2s[i];
		int uniqueID = resource->getUniqueID();
		if (uniqueID > m_resourceUniqueID)
		{
			m_resourceUniqueID = uniqueID;
		}
	}

	//
	// UpDate calendar unique IDs
	//
	for (size_t i = 0; i < m_calendars.GetCount(); i++)
	{
		ProjectCalendarPtr calendar = m_calendars[i];
		int uniqueID = calendar->getUniqueID();
		if (uniqueID > m_calendarUniqueID)
		{
			m_calendarUniqueID = uniqueID;
		}
	}

	//
	// UpDate assignment unique IDs
	//
	for (size_t i = 0; i < m_allResource2Assignments.GetCount(); i++)
	{
		//size_t h = i;
		Resource2AssignmentPtr assignment = m_allResource2Assignments[i];
		if (nullptr == assignment)
			continue;
		NumberPtr uniqueID = assignment->getUniqueID();
		if (uniqueID && uniqueID->intValue() > m_assignmentUniqueID)
		{
			m_assignmentUniqueID = uniqueID->intValue();
		}
	}
}

/**
* Find the earliest task start Date. We treat this as the
* start DatePtr for the project.
*
* @return start Date
*/
DatePtr ProjectFile::getStartDate()
{
	DatePtr startDate = NULL;

	for (size_t i = 0; i < m_allTasks.GetCount(); i++)
	{
		Task* task = m_allTasks[i];
		//
		// If a hidden "summary" task is present we ignore it
		//
		if (task->getUniqueID() == 0)
		{
			continue;
		}

		//
		// Select the actual or forecast start Date. Note that the
		// behaviour is different for milestones. The milestone end Date
		// is always correct, the milestone start DatePtr may be different
		// to reflect a missed deadline.
		//
		DatePtr taskStartDate = NULL;
		if (task->getMilestone() == true)
		{
			taskStartDate = task->getActualFinish();
			if (taskStartDate == NULL)
			{
				taskStartDate = task->getFinish();
			}
		}
		else
		{
			taskStartDate = task->getActualStart();
			if (taskStartDate == NULL)
			{
				taskStartDate = task->getStart();
			}
		}

		if (taskStartDate != NULL)
		{
			if (startDate == NULL)
			{
				startDate = taskStartDate;
			}
			else
			{
				if (taskStartDate->getTime() < startDate->getTime())
				{
					startDate = taskStartDate;
				}
			}
		}
	}

	return (startDate);
}

/**
* Find the latest task finish Date. We treat this as the
* finish DatePtr for the project.
*
* @return finish Date
*/
DatePtr ProjectFile::getFinishDate()
{
	DatePtr finishDate = NULL;

	for (size_t i = 0; i < m_allTasks.GetCount(); i++)
	{
		Task* task = m_allTasks[i];
		//
		// If a hidden "summary" task is present we ignore it
		//
		if (task->getUniqueID() == 0)
		{
			continue;
		}

		//
		// Select the actual or forecast start Date
		//
		DatePtr taskFinishDate = NULL;
		taskFinishDate = task->getActualFinish();
		if (taskFinishDate == NULL)
		{
			taskFinishDate = task->getFinish();
		}

		if (taskFinishDate != NULL)
		{
			if (finishDate == NULL)
			{
				finishDate = taskFinishDate;
			}
			else
			{
				if (taskFinishDate->getTime() > finishDate->getTime())
				{
					finishDate = taskFinishDate;
				}
			}
		}
	}

	return (finishDate);
}

/**
* This method is called to alert project listeners to the fact that
* a task has been read from a project file->
*
* @param task task instance
*/
void ProjectFile::fireTaskReadEvent(Task* task)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		listener->taskRead(task);
	}
}

/**
* This method is called to alert project listeners to the fact that
* a task has been written to a project file->
*
* @param task task instance
*/
void ProjectFile::fireTaskWrittenEvent(Task* task)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		listener->taskWritten(task);
	}
}

/**
* This method is called to alert project listeners to the fact that
* a resource has been read from a project file->
*
* @param resource resource instance
*/
void ProjectFile::fireResource2ReadEvent(Resource2Ptr resource)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		listener->resourceRead(resource.get());
	}
}

/**
* This method is called to alert project listeners to the fact that
* a resource has been written to a project file->
*
* @param resource resource instance
*/
void ProjectFile::fireResource2WrittenEvent(Resource2Ptr resource)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		listener->resourceWritten(resource.get());
	}
}

/**
* This method is called to alert project listeners to the fact that
* a calendar has been read from a project file->
*
* @param calendar calendar instance
*/
void ProjectFile::fireCalendarReadEvent(ProjectCalendarPtr calendar)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		listener->calendarRead(calendar);
	}
}

/**
* This method is called to alert project listeners to the fact that
* a resource assignment has been read from a project file->
*
* @param resourceAssignment resourceAssignment instance
*/
void ProjectFile::fireAssignmentReadEvent(Resource2AssignmentPtr resourceAssignment)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		listener->assignmentRead(resourceAssignment.get());
	}
}

/**
* This method is called to alert project listeners to the fact that
* a resource assignment has been written to a project file->
*
* @param resourceAssignment resourceAssignment instance
*/
void ProjectFile::fireAssignmentWrittenEvent(Resource2AssignmentPtr resourceAssignment)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		listener->assignmentWritten(resourceAssignment.get());
	}
}

/**
* This method is called to alert project listeners to the fact that
* a relation has been read from a project file->
*
* @param relation relation instance
*/
void ProjectFile::fireRelationReadEvent(RelationPtr relation)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners.GetAt(i);
		listener->relationRead(relation);
	}
}

/**
* This method is called to alert project listeners to the fact that
* a relation has been written to a project file->
*
* @param relation relation instance
*/
void ProjectFile::fireRelationWrittenEvent(RelationPtr relation)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners.GetAt(i);
		listener->relationWritten(relation);
	}
}

/**
* This method is called to alert project listeners to the fact that
* a calendar has been written to a project file->
*
* @param calendar calendar instance
*/
void ProjectFile::fireCalendarWrittenEvent(ProjectCalendarPtr calendar)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners.GetAt(i);
		listener->calendarWritten(calendar);
	}
}

/**
* Adds a listener to this project file->
*
* @param listener listener instance
*/
void ProjectFile::addProjectListener(ProjectListener* listener)
{
	m_projectListeners.Add(listener);
}

/**
* Adds a collection of listeners to the current project.
*
* @param listeners collection of listeners
*/
void ProjectFile::addProjectListeners(List<ProjectListener*>* listeners)
{
	for (size_t i = 0; i < m_projectListeners.GetCount(); i++)
	{
		ProjectListener* listener = m_projectListeners[i];
		addProjectListener(listener);
	}
}

/**
* Removes a listener from this project file->
*
* @param listener listener instance
*/
void ProjectFile::removeProjectListener(ProjectListener* listener)
{
	m_projectListeners.remove(listener);
}

/**
* Associates an alias with a custom task field number.
*
* @param field custom field number
* @param alias alias text
*/
void ProjectFile::setTaskFieldAlias(TaskField field, CString alias)
{
	if (alias.GetLength() != 0)
	{
		//m_taskFieldAlias.put(field, alias);
		m_taskFieldAlias[field] = alias;
		//m_aliasTaskField.put(alias, field);
		m_aliasTaskField[alias] = field;
	}
}

/**
* Retrieves the alias associated with a custom task field.
* This method will return NULL if no alias has been defined for
* this field.
*
* @param field task field instance
* @return alias text
*/
CString ProjectFile::getTaskFieldAlias(TaskField field)
{
	CString alias;
	//m_taskFieldAlias.Lookup(field, alias);
	auto it = m_taskFieldAlias.find(field);
	if (it != m_taskFieldAlias.end())
		alias = it->second;
	return alias;
}

/**
* Retrieves a task field instance based on its alias. If the
* alias is not recognised, this method will return NULL.
*
* @param alias alias text
* @return task field instance
*/
TaskField ProjectFile::getAliasTaskField(CString alias)
{
	TaskField field = (TaskField)0;
	//m_aliasTaskField.Lookup(alias, field);
	auto it = m_aliasTaskField.find(alias);
	if (it != m_aliasTaskField.end())
		field = it->second;

	return field;
}

/**
* Associates a value list with a custom task field number.
*
* @param field custom field number
* @param values values for the value list
*/
void ProjectFile::setTaskFieldValueList(TaskField field, std::shared_ptr<List<ObjectPtr>> values)
{
	if ((values != NULL) && (values->GetCount() != 0))
	{
		//m_taskFieldValueList.put(field, values);
		m_taskFieldValueList[field] = values;
	}
}

/**
* Retrieves the value list associated with a custom task field.
* This method will return NULL if no value list has been defined for
* this field.
*
* @param field task field instance
* @return alias text
*/
std::shared_ptr<List<ObjectPtr>> ProjectFile::getTaskFieldValueList(TaskField field)
{
	std::shared_ptr<List<ObjectPtr>> list;
	//m_taskFieldValueList.Lookup(field, list);
	auto it = m_taskFieldValueList.find(field);
	if (it != m_taskFieldValueList.end())
		list = it->second;
	return list;
}

/**
* Associates a descriptions for value list with a custom task field number.
*
* @param field custom field number
* @param descriptions descriptions for the value list
*/
void ProjectFile::setTaskFieldDescriptionList(TaskField field, std::shared_ptr<List<CString>> descriptions)
{
	if ((descriptions != NULL) && (descriptions->GetCount() != 0))
	{
		//m_taskFieldDescriptionList.put(field, descriptions);
		m_taskFieldDescriptionList[field] = descriptions;
	}
}

/**
* Retrieves the description value list associated with a custom task field.
* This method will return NULL if no descriptions for the value list has been defined for
* this field.
*
* @param field task field instance
* @return alias text
*/
std::shared_ptr<List<CString>> ProjectFile::getTaskFieldDescriptionList(TaskField field)
{
	std::shared_ptr<List<CString>> list;
	auto it = m_taskFieldDescriptionList.find(field);
	if (it != m_taskFieldDescriptionList.end())
		list = it->second;
	//m_taskFieldDescriptionList.Lookup(field, list);
	return list;
}

/**
* Associates an alias with a custom resource field number.
*
* @param field custom field number
* @param alias alias text
*/
void ProjectFile::setResource2FieldAlias(Resource2Field field, CString alias)
{
	if (alias.GetLength() != 0)
	{
		//m_resourceFieldAlias.put(field, alias);
		//m_aliasResource2Field.put(alias, field);
		m_resourceFieldAlias[field] = alias;
		m_aliasResource2Field[alias] = field;
	}
}

/**
* Retrieves the alias associated with a custom resource field.
* This method will return NULL if no alias has been defined for
* this field.
*
* @param field field number
* @return alias text
*/
CString ProjectFile::getResource2FieldAlias(Resource2Field field)
{
	CString alias;
	auto it = m_resourceFieldAlias.find(field);
	if (it != m_resourceFieldAlias.end())
		alias = it->second;
	//m_resourceFieldAlias.Lookup(field, alias);
	return alias;
}

/**
* Retrieves a resource field based on its alias. If the
* alias is not recognised, this method will return NULL.
*
* @param alias alias text
* @return resource field instance
*/
Resource2Field ProjectFile::getAliasResource2Field(CString alias)
{
	Resource2Field field = (Resource2Field)(0);
	//m_aliasResource2Field.Lookup(alias, field);
	auto it = m_aliasResource2Field.find(alias);
	if (it != m_aliasResource2Field.end())
		return it->second;
	return field;
}

/**
* Allows derived classes to gain access to the mapping between
* task fields and aliases.
*
* @return task field to alias map
*/
std::map<TaskField, CString>* ProjectFile::getTaskFieldAliasMap()
{
	return &m_taskFieldAlias;
}

/**
* Allows callers to gain access to the mapping between
* resource field numbers and aliases.
*
* @return resource field to alias map
*/
std::map<Resource2Field, CString>* ProjectFile::getResource2FieldAliasMap()
{
	return &m_resourceFieldAlias;
}

/**
* Removes an id-to-task mapping.
*
* @param id task unique ID
*/
void ProjectFile::unmapTaskUniqueID(int id)
{
	m_taskUniqueIDMap.remove(id);
}

/**
* Adds an id-to-task mapping.
*
* @param id task unique ID
* @param task task instance
*/
void ProjectFile::mapTaskUniqueID(int id, Task* task)
{
	m_taskUniqueIDMap.put(id, task);
}

/**
* Removes an id-to-task mapping.
*
* @param id task ID
*/
void ProjectFile::unmapTaskID(int id)
{
	m_taskIDMap.remove(id);
}

/**
* Adds an id-to-task mapping.
*
* @param id task ID
* @param task task instance
*/
void ProjectFile::mapTaskID(int id, Task* task)
{
	m_taskIDMap.put(id, task);
}

/**
* Removes an id-to-resource mapping.
*
* @param id resource unique ID
*/
void ProjectFile::unmapResource2UniqueID(int id)
{
	m_resourceUniqueIDMap.remove(id);
}

/**
* Adds an id-to-resource mapping.
*
* @param id resource unique ID
* @param resource resource instance
*/
void ProjectFile::mapResource2UniqueID(int id, Resource2Ptr resource)
{
	m_resourceUniqueIDMap.put(id, resource);
}

/**
* Removes an id-to-resource mapping.
*
* @param id resource ID
*/
void ProjectFile::unmapResource2ID(int id)
{
	m_resourceIDMap.remove(id);
}

/**
* Adds an id-to-resource mapping.
*
* @param id resource ID
* @param resource resource instance
*/
void ProjectFile::mapResource2ID(int id, Resource2Ptr resource)
{
	m_resourceIDMap.put(id, resource);
}

/**
* Removes an id-to-calendar mapping.
*
* @param id calendar unique ID
*/
void ProjectFile::unmapCalendarUniqueID(int id)
{
	m_calendarUniqueIDMap.remove(id);
}

/**
* Adds an id-to-calendar mapping.
*
* @param id calendar unique ID
* @param calendar calendar instance
*/
void ProjectFile::mapCalendarUniqueID(int id, ProjectCalendarPtr calendar)
{
	m_calendarUniqueIDMap.put(id, calendar);
}

/**
* This method retrieves a value representing the type of MPP file
* that has been read. Currently this method will return the value 8 for
* an MPP8 file (Project 98), 9 for an MPP9 file (Project 2000 and
* Project 2002), 12 for an MPP12 file (Project 2003, Project 2007) and 14 for an
* MPP14 file (Project 2010 and Project 2013).
*
* @return integer representing the file type
*/
int ProjectFile::getMppFileType()
{
	return (m_mppFileType);
}

/**
* Used internally to set the file type.
*
* @param fileType file type
*/
void ProjectFile::setMppFileType(int fileType)
{
	m_mppFileType = fileType;
}

/**
* Retrieves the name of the application used to create this project data.
*
* @return application name
*/
CString ProjectFile::getApplicationName()
{
	return m_applicationName;
}

/**
* Sets the name of the application used to create this projectdata.
*
* @param name application name
*/
void ProjectFile::setApplicationName(CString name)
{
	m_applicationName = name;
}

/**
* Package-method used to add views to this MPP file->
*
* @param view view data
*/
void ProjectFile::addView(ViewPtr view)
{
	m_views.Add(view);
}

/**
* This method returns a list of the views defined in this MPP file->
*
* @return list of views
*/
List<ViewPtr>* ProjectFile::getViews()
{
	return &m_views;
}

/**
* Package-method used to add tables to this MPP file->
*
* @param table table data
*/
void ProjectFile::addTable(TablePtr table)
{
	m_tables.Add(table);
	if (table->getResource2Flag() == false)
	{
		//m_taskTablesByName.put(table->getName(), table);
		m_taskTablesByName[table->getName()] = table;
	}
	else
	{
		//m_resourceTablesByName.put(table->getName(), table);
		m_resourceTablesByName[table->getName()] = table;
	}
}

/**
* This method returns a list of the tables defined in this MPP file->
*
* @return list of tables
*/
List<TablePtr>* ProjectFile::getTables()
{
	return &m_tables;
}

/**
* Adds a filter definition to this project file->
*
* @param filter filter definition
*/
void ProjectFile::addFilter(FilterPtr filter)
{
	if (filter->isTaskFilter())
	{
		m_taskFilters.Add(filter);
	}

	if (filter->isResource2Filter())
	{
		m_resourceFilters.Add(filter);
	}

	//m_filtersByName.put(filter->getName(), filter);
	m_filtersByName[filter->getName()] = filter;
	m_filtersByID.put(filter->getID(), filter);
}

/**
* Removes a filter from this project file->
*
* @param filterName The name of the filter
*/
void ProjectFile::removeFilter(CString filterName)
{
	FilterPtr filter = getFilterByName(filterName);
	if (filter != NULL)
	{
		if (filter->isTaskFilter())
		{
			m_taskFilters.remove(filter);
		}

		if (filter->isResource2Filter())
		{
			m_resourceFilters.remove(filter);
		}
		//m_filtersByName.RemoveKey(filterName);
		auto it = m_filtersByName.find(filterName);
		if (it != m_filtersByName.end())
			m_filtersByName.erase(it);
		m_filtersByID.RemoveKey(filter->getID());
	}
}

/**
* Retrieves a list of all resource filters.
*
* @return list of all resource filters
*/
List<FilterPtr>* ProjectFile::getAllResource2Filters()
{
	return &m_resourceFilters;
}

/**
* Retrieves a list of all task filters.
*
* @return list of all task filters
*/
List<FilterPtr>* ProjectFile::getAllTaskFilters()
{
	return &m_taskFilters;
}

/**
* Retrieve a given filter by name.
*
* @param name filter name
* @return filter instance
*/
FilterPtr ProjectFile::getFilterByName(CString name)
{
	FilterPtr filter = NULL;
// 	if (m_filtersByName.Lookup(name, filter))
// 		return filter;

	auto it = m_filtersByName.find(name);
	if (it != m_filtersByName.end())
		return it->second;
	return NULL;
}

/**
* Retrieve a given filter by ID.
*
* @param id filter ID
* @return filter instance
*/
FilterPtr ProjectFile::getFilterByID(int id)
{
	FilterPtr filter = NULL;
	m_filtersByID.Lookup(id, filter);
	return filter;
}

/**
* Retrieves a list of all groups.
*
* @return list of all groups
*/
List<Group2Ptr>* ProjectFile::getAllGroup2s()
{
	return &m_groups;
}

/**
* Retrieve a given group by name.
*
* @param name group name
* @return Group2 instance
*/
Group2Ptr ProjectFile::getGroup2ByName(CString name)
{
	Group2Ptr group = NULL;
	//m_groupsByName.Lookup(name, group);
	auto it = m_groupsByName.find(name);
	if (it != m_groupsByName.end())
		return it->second;
	return group;
}

/**
* Adds a group definition to this project file->
*
* @param group group definition
*/
void ProjectFile::addGroup2(Group2Ptr group)
{
	m_groups.Add(group);
	//m_groupsByName.put(group->getName(), group);
	m_groupsByName[group->getName()] = group;
}

///**
//* Adds the definition of a graphical indicator for a field type.
//*
//* @param field field type
//* @param indicator graphical indicator definition
//*/
//void ProjectFile::addGraphicalIndicator(FieldType* field, GraphicalIndicatorPtr indicator)
//{
//	m_graphicalIndicators.put(field, indicator);
//}
//
///**
//* Retrieves the definition of any graphical indicators used for the
//* given field type.
//*
//* @param field field type
//* @return graphical indicator definition
//*/
//GraphicalIndicatorPtr ProjectFile::getGraphicalIndicator(FieldType* field)
//{
//	GraphicalIndicatorPtr value = NULL;
//	m_graphicalIndicators.Lookup(field, value);
//	return value;
//}

/**
* Utility method to retrieve the definition of a task table by name.
* This method will return NULL if the table name is not recognised.
*
* @param name table name
* @return table instance
*/
TablePtr ProjectFile::getTaskTableByName(CString name)
{
	TablePtr table = NULL;
	//m_taskTablesByName.Lookup(name, table);
	auto it = m_taskTablesByName.find(name);
	if (it != m_taskTablesByName.end())
		return it->second;
	return table;
}

/**
* Utility method to retrieve the definition of a resource table by name.
* This method will return NULL if the table name is not recognised.
*
* @param name table name
* @return table instance
*/
TablePtr ProjectFile::getResource2TableByName(CString name)
{
	TablePtr table = NULL;
	//m_resourceTablesByName.Lookup(name, table);
	auto it = m_resourceTablesByName.find(name);
	if (it != m_resourceTablesByName.end())
		return it->second;
	return table;
}

/**
* This package-method is used to add resource sub project details.
*
* @param project sub project
*/
void ProjectFile::setResource2SubProject(SubProjectPtr project)
{
	m_resourceSubProject = project;
}

/**
* Retrieves details of the sub project file used as a resource pool.
*
* @return sub project details
*/
SubProjectPtr ProjectFile::getResource2SubProject()
{
	return (m_resourceSubProject);
}

/**
* This package-method is used to add sub project details.
*
* @param project sub project
*/
void ProjectFile::addSubProject(SubProjectPtr project)
{
	m_allSubProjects.Add(project);
}

/**
* Retrieves all the subprojects for this MPX file->
*
* @return all sub project details
*/
List<SubProjectPtr>* ProjectFile::getAllSubProjects()
{
	return &m_allSubProjects;
}

/**
* Retrieve a flag indicating if auto filter is enabled.
*
* @return auto filter flag
*/
bool ProjectFile::getAutoFilter()
{
	return (m_autoFilter);
}

/**
* Sets a flag indicating if auto filter is enabled.
*
* @param autoFilter bool flag
*/
void ProjectFile::setAutoFilter(bool autoFilter)
{
	m_autoFilter = autoFilter;
}

/**
* Set the saved view state associated with this file->
*
* @param viewState view state
*/
void ProjectFile::setViewState(ViewStatePtr viewState)
{
	m_viewState = viewState;
}

/**
* Retrieve the saved view state associated with this file->
*
* @return view state
*/
ViewStatePtr ProjectFile::getViewState()
{
	return (m_viewState);
}

/**
* Set whether the data in this file is encoded.
*
* @param encoded True if the data is encoded in the file
*/
void ProjectFile::setEncoded(bool encoded)
{
	m_encoded = encoded;
}

/**
* Get whether the data in this file is encoded.
*
* @return encoded
*/
bool ProjectFile::getEncoded()
{
	return (m_encoded);
}

/**
* Set the key with which this data is encrypted (can be decrypted) with.
*
* @param encryptionKey Encryption key
*/
void ProjectFile::setEncryptionCode(BYTE encryptionKey)
{
	if (encryptionKey != 0x00)
	{
		m_encryptionKey = (BYTE)(0xFF - encryptionKey);
	}
	else
	{
		m_encryptionKey = (BYTE)0x00;
	}
}

/**
* Get the key with which this data is encrypted (can be decrypted) with.
*
* @return m_encryptionKey
*/
BYTE ProjectFile::getEncryptionCode()
{
	return (m_encryptionKey);
}

/**
* Sets the project file path.
*
* @param projectFilePath project file path
*/
void ProjectFile::setProjectFilePath(CString projectFilePath)
{
	m_projectFilePath = projectFilePath;
}

/**
* Gets the project file path.
*
* @return project file path
*/
CString ProjectFile::getProjectFilePath()
{
	return (m_projectFilePath);
}

/**
* Add a custom field value list item.
*
* @param item CustomFieldValueItem instance
*/
void ProjectFile::addCustomFieldValueItem(CustomFieldValueItemPtr item)
{
	m_customFieldValueItems.put(item->getUniqueID(), item);
}

/**
* Get the custom field value list item with the given unique ID.
*
* @param uniqueID unique ID
* @return CustomFieldValueItem instance
*/
CustomFieldValueItemPtr ProjectFile::getCustomFieldValueItem(int uniqueID)
{
	CustomFieldValueItemPtr item = NULL;
	m_customFieldValueItems.Lookup(uniqueID, item);
	return item;
}

/**
* Retrieves the default calendar for this project based on the calendar name
* given in the project header. If a calendar of this name cannot be found, then
* the first calendar listed for the project will be returned. If the
* project contains no calendars, then a default calendar is added.
*
* @return default projectCalendar instance
*/
ProjectCalendarPtr ProjectFile::getCalendar()
{
	CString calendarName = m_projectHeader->getCalendarName();
	ProjectCalendarPtr calendar = getCalendarByName(calendarName);
	if (!calendar)
	{
		if (m_calendars.isEmpty())
		{
			calendar = addDefaultBaseCalendar();
		}
		else
		{
			calendar = m_calendars.GetAt(0);
		}
	}
	return calendar;
}

/**
* Sets the default calendar for this project.
*
* @param calendar default calendar instance
*/
void ProjectFile::setCalendar(ProjectCalendarPtr calendar)
{
	m_projectHeader->setCalendarName(calendar->getName());
}

/**
* Retrieve the calendar used internally for timephased baseline calculation.
*
* @return baseline calendar
*/
ProjectCalendarPtr ProjectFile::getBaselineCalendar()
{
	//
	// Attempt to locate the calendar normally used by baselines
	// If this isn't present, fall back to using the default 
	// project calendar->
	//
	ProjectCalendarPtr result = getCalendarByName(L"Used for Microsoft Project 98 Baseline Calendar");
	if (!result)
	{
		result = getCalendar();
	}
	return result;
}

