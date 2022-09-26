#pragma once

#include "ProjectEntity.h"
#include "FieldContainer.h"
#include "TaskContainer.h"
#include "TaskField.h"
#include "Priority.h"
#include "ProjectCalendar.h"
#include "SubProject.h"
#include "RecurringTask.h"

//class ProjectCalendar;
class Resource2;
class Resource2Assignment;
class SubProject;
class Relation;

class DateRange;

/**
 * This class represents a task record from an project file->
 */
class COMMONCONTROLS_API Task : public ProjectEntity, public FieldContainer, public TaskContainer, public ComparableT<Task>
{
public:
	/**
	 * Default constructor.
	 *
	 * @param file Parent file to which this record belongs.
	 * @param parent Parent task
	 */
	Task(ProjectFile* file, Task* parent);
	~Task();

	/**
	 * This method is used to automatically generate a value
	 * for the WBS field of this task.
	 *
	 * @param parent Parent Task*
	 */
	void generateWBS(Task* parent);

	/**
	 * This method is used to automatically generate a value
	 * for the Outline NumberPtr field of this task.
	 *
	 * @param parent Parent Task*
	 */
	void generateOutlineNumber(Task* parent);

	/**
	 * This method is used to add notes to the current task.
	 *
	 * @param notes notes to be added
	 */
	void setNotes(CString notes);

	/**
	 * This method allows nested tasks to be added, with the WBS being
	 * completed automatically.
	 *
	 * @return new task
	 */
	Task* addTask();

	/**
	 * This method is used to associate a child task with the current
	 * task instance. It has package access, and has been designed to
	 * allow the hierarchical outline structure of tasks in an MPX
	 * file to be constructed as the file is read in.
	 *
	 * @param child Child task.
	 * @param childOutlineLevel Outline level of the child task.
	 */
	void addChildTask(Task* child, int childOutlineLevel);

	/**
	 * This method is used to associate a child task with the current
	 * task instance. It has been designed to
	 * allow the hierarchical outline structure of tasks in an MPX
	 * file to be upDated once all of the task data has been read.
	 *
	 * @param child child task
	 */
	void addChildTask(Task* child);

	/**
	 * Inserts a child task prior to a given sibling task.
	 *
	 * @param child new child task
	 * @param previousSibling sibling task
	 */
	void addChildTaskBefore(Task* child, Task* previousSibling);

	/**
	 * Removes a child task.
	 *
	 * @param child child task instance
	 */
	void removeChildTask(Task* child);

	/**
	 * This method allows the list of child tasks to be cleared in preparation
	 * for the hierarchical task structure to be built.
	 */
	void clearChildTasks();

	/**
	 * This method allows recurring task details to be added to the
	 * current task.
	 *
	 * @return RecurringTask* object
	 */
	RecurringTaskPtr addRecurringTask();

	/**
	 * This method retrieves the recurring task record. If the current
	 * task is not a recurring task, then this method will return NULL.
	 *
	 * @return Recurring task record.
	 */
	RecurringTaskPtr getRecurringTask();

	/**
	 * This method allows a resource assignment to be added to the
	 * current task.
	 *
	 * @param resource the resource to assign
	 * @return std::shared_ptr<Resource2Assignment> object
	 */
	std::shared_ptr<Resource2Assignment> addResource2Assignment(std::shared_ptr<Resource2> resource);

	/**
	 * Add a resource assignment which has been populated elsewhere.
	 *
	 * @param assignment resource assignment
	 */
	void addResource2Assignment(std::shared_ptr<Resource2Assignment> assignment);

private:
	/**
	 * Retrieves an existing resource assignment if one is present,
	 * to prevent duplicate resource assignments being added.
	 *
	 * @param resource resource to test for
	 * @return existing resource assignment
	 */
	std::shared_ptr<Resource2Assignment> getExistingResource2Assignment(std::shared_ptr<Resource2> resource);

public:
	/**
	 * This method allows the list of resource assignments for this
	 * task to be retrieved.
	 *
	 * @return list of resource assignments
	 */
	List<std::shared_ptr<Resource2Assignment>>* getResource2Assignments();

	/**
	 * Internal method used as part of the process of removing a
	 * resource assignment.
	 *
	 * @param assignment resource assignment to be removed
	 */
	void removeResource2Assignment(std::shared_ptr<Resource2Assignment> assignment);

	/**
	 * This method allows a predecessor relationship to be added to this
	 * task instance.
	 *
	 * @param targetTask the predecessor task
	 * @param type relation type
	 * @param lag relation lag
	 * @return relationship
	 */
	std::shared_ptr<Relation> addPredecessor(Task* targetTask, RelationType type, DurationPtr lag);

	/**
	 * The % Complete field contains the current status of a task, expressed
	 * as the percentage of the
	 * task's duration that has been completed. You can enter percent complete,
	 * or you can have
	 * Microsoft Project calculate it for you based on actual duration.
	 *
	 * @param val value to be set
	 */
	void setPercentageComplete(NumberPtr val);

	/**
	 * The % Work Complete field contains the current status of a task,
	 * expressed as the
	 * percentage of the task's work that has been completed. You can enter
	 * percent work
	 * complete, or you can have Microsoft Project calculate it for you
	 * based on actual
	 * work on the task.
	 *
	 * @param val value to be set
	 */
	void setPercentageWorkComplete(NumberPtr val);

	/**
	 * The Actual Cost field shows costs incurred for work already performed
	 * by all resources
	 * on a task, along with any other recorded costs associated with the task.
	 * You can enter
	 * all the actual costs or have Microsoft Project calculate them for you.
	 *
	 * @param val value to be set
	 */
	void setActualCost(NumberPtr val);

	/**
	 * The Actual DurationPtr field shows the span of actual working time for a
	 * task so far,
	 * based on the scheduled duration and current remaining work or
	 * completion percentage.
	 *
	 * @param val value to be set
	 */
	void setActualDuration(DurationPtr val);

	/**
	 * The Actual Finish field shows the DatePtr and time that a task actually
	 * finished.
	 * Microsoft Project sets the Actual Finish field to the scheduled finish
	 * DatePtr if
	 * the completion percentage is 100. This field contains "NA" until you
	 * enter actual
	 * information or set the completion percentage to 100.
	 *
	 * @param val value to be set
	 */
	void setActualFinish(DatePtr val);

	/**
	 * The Actual Start field shows the DatePtr and time that a task actually began.
	 * When a task is first created, the Actual Start field contains "NA." Once you
	 * enter the first actual work or a completion percentage for a task, Microsoft
	 * Project sets the actual start DatePtr to the scheduled start Date.
	 * @param val value to be set
	 */
	void setActualStart(DatePtr val);

	/**
	 * The Actual Work field shows the amount of work that has already been
	 * done by the
	 * resources assigned to a task.
	 * @param val value to be set
	 */
	void setActualWork(DurationPtr val);

	/**
	 * The Baseline Cost field shows the total planned cost for a task.
	 * Baseline cost is also referred to as budget at completion (BAC).
	 *
	 * @param val the amount to be set
	 */
	void setBaselineCost(NumberPtr val);

	/**
	 * The Baseline DurationPtr field shows the original span of time planned to
	 * complete a task.
	 *
	 * @param val duration
	 */
	void setBaselineDuration(DurationPtr val);

	/**
	 * The Baseline Finish field shows the planned completion DatePtr for a
	 * task at the time
	 * you saved a baseline. Information in this field becomes available
	 * when you set a
	 * baseline for a task.
	 *
	 * @param val DatePtr to be set
	 */
	void setBaselineFinish(DatePtr val);

	/**
	 * The Baseline Start field shows the planned beginning DatePtr for a task at
	 * the time
	 * you saved a baseline. Information in this field becomes available when you
	 * set a baseline.
	 *
	 * @param val DatePtr to be set
	 */
	void setBaselineStart(DatePtr val);

	/**
	 * The Baseline Work field shows the originally planned amount of work to
	 * be performed
	 * by all resources assigned to a task. This field shows the planned
	 * person-hours
	 * scheduled for a task. Information in the Baseline Work field
	 * becomes available
	 * when you set a baseline for the project.
	 *
	 * @param val the duration to be set.
	 */
	void setBaselineWork(DurationPtr val);

	/**
	 * The BCWP (budgeted cost of work performed) field contains the
	 * cumulative value
	 * of the assignment's timephased percent complete multiplied by
	 * the assignments
	 * timephased baseline cost. BCWP is calculated up to the status
	 * DatePtr or todays
	 * Date. This information is also known as earned value.
	 *
	 * @param val the amount to be set
	 */
	void setBCWP(NumberPtr val);

	/**
	 * The BCWS (budgeted cost of work scheduled) field contains the cumulative
	 * timephased baseline costs up to the status DatePtr or today's Date.
	 *
	 * @param val the amount to set
	 */
	void setBCWS(NumberPtr val);

	/**
	 * The Confirmed field indicates whether all resources assigned to a task have
	 * accepted or rejected the task assignment in response to a TeamAssign message
	 * regarding their assignments.
	 *
	 * @param val bool value
	 */
	void setConfirmed(bool val);

	/**
	 * The Constraint DatePtr field shows the specific DatePtr associated with certain
	 * constraint types,
	 *  such as Must Start On, Must Finish On, Start No Earlier Than,
	 *  Start No Later Than,
	 *  Finish No Earlier Than, and Finish No Later Than.
	 *  SEE class constants
	 *
	 * @param val DatePtr to be set
	 */
	void setConstraintDate(DatePtr val);

	/**
	 * Private method for dealing with string parameters from File.
	 *
	 * @param type string constraint type
	 */
	void setConstraintType(ConstraintType type);

	/**
	 * The Contact field contains the name of an individual
	 * responsible for a task.
	 *
	 * @param val value to be set
	 */
	void setContact(CString val);

	/**
	 * The Cost field shows the total scheduled, or projected, cost for a task,
	 * based on costs already incurred for work performed by all resources assigned
	 * to the task, in addition to the costs planned for the remaining work for the
	 * assignment. This can also be referred to as estimate at completion (EAC).
	 *
	 * @param val amount
	 */
	void setCost(NumberPtr val);

	/**
	 * Set a cost value.
	 *
	 * @param index cost index (1-10)
	 * @param value cost value
	 */
	void setCost(int index, NumberPtr value);

	/**
	 * Retrieve a cost value.
	 *
	 * @param index cost index (1-10)
	 * @return cost value
	 */
	NumberPtr getCost(int index);

	/**
	 * The Cost Variance field shows the difference between the
	 * baseline cost and total cost for a task. The total cost is the
	 * current estimate of costs based on actual costs and remaining costs.
	 *
	 * @param val amount
	 */
	void setCostVariance(NumberPtr val);

	/**
	 * The Created field contains the DatePtr and time when a task was
	 * added to the project.
	 *
	 * @param val Date
	 */
	void setCreateDate(DatePtr val);

	/**
	 * The Critical field indicates whether a task has any room in the
	 * schedule to slip,
	 * or if a task is on the critical path. The Critical field contains
	 * Yes if the task
	 * is critical and No if the task is not critical.
	 *
	 * @param val whether task is critical or not
	 */
	void setCritical(bool val);

	/**
	 * The CV (earned value cost variance) field shows the difference
	 * between how much it should have cost to achieve the current level of
	 * completion on the task, and how much it has actually cost to achieve the
	 * current level of completion up to the status DatePtr or today's Date.
	 *
	 * @param val value to set
	 */
	void setCV(NumberPtr val);

	/**
	 * Set amount of delay as elapsed real time.
	 *
	 * @param val elapsed time
	 */
	void setLevelingDelay(DurationPtr val);

	/**
	 * The DurationPtr field is the total span of active working time for a task.
	 * This is generally the amount of time from the start to the finish of a task.
	 * The default for new tasks is 1 day (1d).
	 *
	 * @param val duration
	 */
	void setDuration(DurationPtr val);

	/**
	 * Set the duration text used for a manually scheduled task.
	 *
	 * @param val text
	 */
	void setDurationText(CString val);

	/**
	 * Set the manual duration attribute.
	 *
	 * @param dur manual duration
	 */
	void setManualDuration(DurationPtr dur);

	/**
	 * Read the manual duration attribute.
	 *
	 * @return manual duration
	 */
	DurationPtr getManualDuration();

	/**
	 * The DurationPtr Variance field contains the difference between the
	 * baseline duration of a task and the forecast or actual duration
	 * of the task.
	 *
	 * @param duration duration value
	 */
	void setDurationVariance(DurationPtr duration);

	/**
	 * The Early Finish field contains the earliest DatePtr that a task
	 * could possibly finish, based on early finish Dates of predecessor
	 * and successor tasks, other constraints, and any leveling delay.
	 *
	 * @param DatePtr DatePtr value
	 */
	void setEarlyFinish(DatePtr date);

	/**
	 * The Early Start field contains the earliest DatePtr that a task could
	 * possibly begin, based on the early start Dates of predecessor and
	 * successor tasks, and other constraints.
	 *
	 * @param DatePtr DatePtr value
	 */
	void setEarlyStart(DatePtr date);

	/**
	 * The Finish field shows the DatePtr and time that a task is scheduled to be
	 * completed. MS project allows a finish DatePtr to be entered, and will
	 * calculate the duration, or a duration can be supplied and MS Project
	 * will calculate the finish Date.
	 *
	 * @param DatePtr DatePtr value
	 */
	void setFinish(DatePtr date);

	/**
	 * Set the finish text used for a manually scheduled task.
	 *
	 * @param val text
	 */
	void setFinishText(CString val);

	/**
	 * The Finish Variance field contains the amount of time that represents the
	 * difference between a task's baseline finish DatePtr and its forecast
	 * or actual finish Date.
	 *
	 * @param duration duration value
	 */
	void setFinishVariance(DurationPtr duration);

	/**
	 * The Fixed Cost field shows any task expense that is not associated
	 * with a resource cost.
	 *
	 * @param val amount
	 */
	void setFixedCost(NumberPtr val);

	/**
	 * The Free Slack field contains the amount of time that a task can be
	 * delayed without delaying any successor tasks. If the task has no
	 * successors, free slack is the amount of time that a task can be delayed
	 * without delaying the entire project's finish Date.
	 *
	 * @param duration duration value
	 */
	void setFreeSlack(DurationPtr duration);

	/**
	 * The Hide Bar flag indicates whether the Gantt bars and CalendarPtr bars
	 * for a task are hidden when this project's data is displayed in MS Project.
	 *
	 * @param flag bool value
	 */
	void setHideBar(bool flag);

	/**
	 * The ID field contains the identifier number that Microsoft Project
	 * automatically assigns to each task as you add it to the project.
	 * The ID indicates the position of a task with respect to the other tasks.
	 *
	 * @param val ID
	 */
	void setID(int val);

	/**
	 * The Late Finish field contains the latest DatePtr that a task can finish
	 * without delaying the finish of the project. This DatePtr is based on the
	 * task's late start Date, as well as the late start and late finish Dates
	 * of predecessor and successor tasks, and other constraints.
	 *
	 * @param DatePtr DatePtr value
	 */
	void setLateFinish(DatePtr Date);

	/**
	 * The Late Start field contains the latest DatePtr that a task can start
	 * without delaying the finish of the project. This DatePtr is based on the
	 * task's start Date, as well as the late start and late finish Dates of
	 * predecessor and successor tasks, and other constraints.
	 *
	 * @param DatePtr DatePtr value
	 */
	void setLateStart(DatePtr Date);

	/**
	 * The Linked Fields field indicates whether there are OLE links to the task,
	 * either from elsewhere in the active project, another Microsoft Project
	 * file, or from another program.
	 *
	 * @param flag bool value
	 */
	void setLinkedFields(bool flag);

	/**
	 * This is a user defined field used to mark a task for some form of
	 * additional action.
	 *
	 * @param flag bool value
	 */
	void setMarked(bool flag);

	/**
	 * The Milestone field indicates whether a task is a milestone.
	 *
	 * @param flag bool value
	 */
	void setMilestone(bool flag);

	/**
	 * The Name field contains the name of a task.
	 *
	 * @param name task name
	 */
	void setName(CString name);

	/**
	 * The Objects field contains the number of objects attached to a task.
	 *
	 * @param val - integer value
	 */
	void setObjects(int val);

	/**
	 * The Outline Level field contains the number that indicates the level of
	 * the task in the project outline hierarchy.
	 *
	 * @param val - int
	 */
	void setOutlineLevel(int val);

	/**
	 * The Outline NumberPtr field contains the number of the task in the structure
	 * of an outline. This number indicates the task's position within the
	 * hierarchical structure of the project outline. The outline number is
	 * similar to a WBS (work breakdown structure) number, except that the
	 * outline number is automatically entered by Microsoft Project.
	 *
	 * @param val - text
	 */
	void setOutlineNumber(CString val);

	/**
	 * The PriorityPtr field provides choices for the level of importance
	 * assigned to a task, which in turn indicates how readily a task can be
	 * delayed or split during resource leveling.
	 * The default priority is Medium. Those tasks with a priority
	 * of Do Not Level are never delayed or split when Microsoft Project levels
	 * tasks that have overallocated resources assigned.
	 *
	 * @param priority the priority value
	 */
	void setPriority(PriorityPtr priority);

	/**
	 * The Project field shows the name of the project from which a
	 * task originated.
	 * This can be the name of the active project file-> If there are
	 * other projects
	 * inserted into the active project file, the name of the
	 * inserted project appears
	 * in this field for the task.
	 *
	 * @param val - text
	 */
	void setProject(CString val);

	/**
	 * The Remaining Cost field shows the remaining scheduled expense of a task that
	 * will be incurred in completing the remaining scheduled work by all resources
	 * assigned to the task.
	 *
	 * @param val - currency amount
	 */
	void setRemainingCost(NumberPtr val);

	/**
	 * The Remaining DurationPtr field shows the amount of time required to complete
	 * the unfinished portion of a task.
	 *
	 * @param val - duration.
	 */
	void setRemainingDuration(DurationPtr val);

	/**
	 * The Remaining Work field shows the amount of time, or person-hours,
	 * still required by all assigned resources to complete a task.
	 * @param val  - duration
	 */
	void setRemainingWork(DurationPtr val);

	/**
	 * The std::shared_ptr<Resource2> Group2 field contains the list of resource groups to which the
	 * resources assigned to a task belong.
	 *
	 * @param val - CString list
	 */
	void setResource2Group2(CString val);

	/**
	 * The std::shared_ptr<Resource2> Initials field lists the abbreviations for the names of
	 * resources assigned to a task. These initials can serve as substitutes
	 * for the names.
	 *
	 * Note that MS Project 98 does no normally populate this field when
	 * it generates an MPX file, and will therefore not expect to see values
	 * in this field when it reads an MPX file-> Supplying values for this
	 * field will cause MS Project 98, 2000, and 2002 to create new resources
	 * and ignore any other resource assignments that have been defined
	 * in the MPX file->
	 *
	 * @param val CString containing a comma separated list of initials
	 */
	void setResource2Initials(CString val);

	/**
	 * The std::shared_ptr<Resource2> Names field lists the names of all resources
	 * assigned to a task.
	 *
	 * Note that MS Project 98 does not normally populate this field when
	 * it generates an MPX file, and will therefore not expect to see values
	 * in this field when it reads an MPX file-> Supplying values for this
	 * field when writing an MPX file will cause MS Project 98, 2000, and 2002
	 * to create new resources and ignore any other resource assignments
	 * that have been defined in the MPX file->
	 *
	 * @param val CString containing a comma separated list of names
	 */
	void setResource2Names(CString val);

	/**
	 * The Resume field shows the DatePtr that the remaining portion of a task is
	 * scheduled to resume after you enter a new value for the % Complete field.
	 * The Resume field is also recalculated when the remaining portion of a task
	 * is moved to a new Date.
	 *
	 * @param val - Date
	 */
	void setResume(DatePtr val);

	/**
	 * For subtasks, the Rollup field indicates whether information on the subtask
	 * Gantt bars will be rolled up to the summary task bar. For summary tasks, the
	 * Rollup field indicates whether the summary task bar displays rolled up bars.
	 * You must have the Rollup field for summary tasks set to Yes for any subtasks
	 * to roll up to them.
	 *
	 * @param val - bool
	 */
	void setRollup(bool val);

	/**
	 * The Start field shows the DatePtr and time that a task is scheduled to begin.
	 * You can enter the start DatePtr you want, to indicate the DatePtr when the task
	 * should begin. Or, you can have Microsoft Project calculate the start Date.
	 * @param val - Date
	 */
	void setStart(DatePtr val);

	/**
	 * Set the start text used for a manually scheduled task.
	 *
	 * @param val text
	 */
	void setStartText(CString val);

	/**
	 * The Start Variance field contains the amount of time that represents the
	 * difference between a task's baseline start DatePtr and its currently
	 * scheduled start Date.
	 *
	 * @param val - duration
	 */
	void setStartVariance(DurationPtr val);

	/**
	 * The Stop field shows the DatePtr that represents the end of the actual
	 * portion of a task. Typically, Microsoft Project calculates the stop Date.
	 * However, you can edit this DatePtr as well.
	 *
	 * @param val - Date
	 */
	void setStop(DatePtr val);

	/**
	 * The Subproject File field contains the name of a project inserted into
	 * the active project file-> The Subproject File field contains the inserted
	 * project's path and file name.
	 *
	 * @param val - CString
	 */
	void setSubprojectName(CString val);

	/**
	 * The Summary field indicates whether a task is a summary task.
	 *
	 * @param val - bool
	 */
	void setSummary(bool val);

	/**
	 * The SV (earned value schedule variance) field shows the difference
	 * in cost terms between the current progress and the baseline plan
	 * of the task up to the status DatePtr or today's Date. You can use SV
	 * to check costs to determine whether tasks are on schedule.
	 * @param val - currency amount
	 */
	void setSV(NumberPtr val);

	/**
	 * The Total Slack field contains the amount of time a task can be delayed
	 * without delaying the project's finish Date.
	 *
	 * @param val - duration
	 */
	void setTotalSlack(DurationPtr val);

	/**
	 * The Unique ID field contains the number that Microsoft Project
	 * automatically designates whenever a new task is created.
	 * This number indicates the sequence in which the task was created,
	 * regardless of placement in the schedule.
	 *
	 * @param val unique ID
	 */
	void setUniqueID(int val);

	/**
	 * The UpDate Needed field indicates whether a TeamUpDate message should
	 * be sent to the assigned resources because of changes to the start Date,
	 * finish Date, or resource reassignments of the task.
	 *
	 * @param val - bool
	 */
	void setUpDateNeeded(bool val);

	/**
	 * The work breakdown structure code. The WBS field contains an alphanumeric
	 * code you can use to represent the task's position within the hierarchical
	 * structure of the project. This field is similar to the outline number,
	 * except that you can edit it.
	 *
	 * @param val - CString
	 */
	void setWBS(CString val);

	/**
	 * The Work field shows the total amount of work scheduled to be performed
	 * on a task by all assigned resources. This field shows the total work,
	 * or person-hours, for a task.
	 *
	 * @param val - duration
	 */
	void setWork(DurationPtr val);

	/**
	 * The Work Variance field contains the difference between a task's baseline
	 * work and the currently scheduled work.
	 *
	 * @param val - duration
	 */
	void setWorkVariance(DurationPtr val);

	/**
	 * The % Complete field contains the current status of a task,
	 * expressed as the percentage of the task's duration that has been completed.
	 * You can enter percent complete, or you can have Microsoft Project calculate
	 * it for you based on actual duration.
	 * @return percentage as float
	 */
	NumberPtr getPercentageComplete();

	/**
	 * The % Work Complete field contains the current status of a task,
	 * expressed as the percentage of the task's work that has been completed.
	 * You can enter percent work complete, or you can have Microsoft Project
	 * calculate it for you based on actual work on the task.
	 *
	 * @return percentage as float
	 */
	NumberPtr getPercentageWorkComplete();

	/**
	 * The Actual Cost field shows costs incurred for work already performed
	 * by all resources on a task, along with any other recorded costs associated
	 * with the task. You can enter all the actual costs or have Microsoft Project
	 * calculate them for you.
	 *
	 * @return currency amount as float
	 */
	NumberPtr getActualCost();

	/**
	 * The Actual DurationPtr field shows the span of actual working time for a
	 * task so far, based on the scheduled duration and current remaining work
	 * or completion percentage.
	 *
	 * @return duration string
	 */
	DurationPtr getActualDuration();

	/**
	 * The Actual Finish field shows the DatePtr and time that a task actually
	 * finished. Microsoft Project sets the Actual Finish field to the scheduled
	 * finish DatePtr if the completion percentage is 100. This field contains "NA"
	 * until you enter actual information or set the completion percentage to 100.
	 * If "NA" is entered as value, arbitrary year zero DatePtr is used. Date(0);
	 *
	 * @return Date
	 */
	DatePtr getActualFinish();

	/**
	 * The Actual Start field shows the DatePtr and time that a task actually began.
	 * When a task is first created, the Actual Start field contains "NA." Once
	 * you enter the first actual work or a completion percentage for a task,
	 * Microsoft Project sets the actual start DatePtr to the scheduled start Date.
	 * If "NA" is entered as value, arbitrary year zero DatePtr is used. Date(0);
	 *
	 * @return Date
	 */
	DatePtr getActualStart();

	/**
	 * The Actual Work field shows the amount of work that has already been done
	 * by the resources assigned to a task.
	 *
	 * @return duration string
	 */
	DurationPtr getActualWork();

	/**
	 * The Baseline Cost field shows the total planned cost for a task.
	 * Baseline cost is also referred to as budget at completion (BAC).
	 * @return currency amount as float
	 */
	NumberPtr getBaselineCost();

	/**
	 * The Baseline DurationPtr field shows the original span of time planned
	 * to complete a task.
	 *
	 * @return  - duration string
	 */
	DurationPtr getBaselineDuration();

	/**
	 * Retrieves the text value for the baseline duration.
	 *
	 * @return baseline duration text
	 */
	CString getBaselineDurationText();

	/**
	 * Sets the baseline duration text value.
	 *
	 * @param value baseline duration text
	 */
	void setBaselineDurationText(CString value);

	/**
	 * The Baseline Finish field shows the planned completion DatePtr for a task
	 * at the time you saved a baseline. Information in this field becomes
	 * available when you set a baseline for a task.
	 *
	 * @return Date
	 */
	DatePtr getBaselineFinish();

	/**
	 * Retrieves the baseline finish text value.
	 *
	 * @return baseline finish text
	 */
	CString getBaselineFinishText();

	/**
	 * Sets the baseline finish text value.
	 *
	 * @param value baseline finish text
	 */
	void setBaselineFinishText(CString value);

	/**
	 * The Baseline Start field shows the planned beginning DatePtr for a task at
	 * the time you saved a baseline. Information in this field becomes available
	 * when you set a baseline.
	 *
	 * @return Date
	 */
	DatePtr getBaselineStart();

	/**
	 * Retrieves the baseline start text value.
	 *
	 * @return baseline start value
	 */
	CString getBaselineStartText();

	/**
	 * Sets the baseline start text value.
	 *
	 * @param value baseline start text
	 */
	void setBaselineStartText(CString value);

	/**
	 * The Baseline Work field shows the originally planned amount of work to be
	 * performed by all resources assigned to a task. This field shows the planned
	 * person-hours scheduled for a task. Information in the Baseline Work field
	 * becomes available when you set a baseline for the project.
	 *
	 * @return Duration
	 */
	DurationPtr getBaselineWork();

	/**
	 * The BCWP (budgeted cost of work performed) field contains
	 * the cumulative value of the assignment's timephased percent complete
	 * multiplied by the assignment's timephased baseline cost.
	 * BCWP is calculated up to the status DatePtr or today's Date.
	 * This information is also known as earned value.
	 *
	 * @return currency amount as float
	 */
	NumberPtr getBCWP();

	/**
	 * The BCWS (budgeted cost of work scheduled) field contains the cumulative
	 * timephased baseline costs up to the status DatePtr or today's Date.
	 *
	 * @return currency amount as float
	 */
	NumberPtr getBCWS();

	/**
	 * The Confirmed field indicates whether all resources assigned to a task
	 * have accepted or rejected the task assignment in response to a TeamAssign
	 * message regarding their assignments.
	 *
	 * @return bool
	 */
	bool getConfirmed();

	/**
	 * The Constraint DatePtr field shows the specific DatePtr associated with certain
	 * constraint types, such as Must Start On, Must Finish On,
	 * Start No Earlier Than,
	 * Start No Later Than, Finish No Earlier Than, and Finish No Later Than.
	 *
	 * @return Date
	 */
	DatePtr getConstraintDate();

	/**
	 * The Constraint Type field provides choices for the type of constraint you
	 * can apply for scheduling a task.
	 *
	 * @return constraint type
	 */
	ConstraintType getConstraintType();

	/**
	 * The Contact field contains the name of an individual
	 * responsible for a task.
	 *
	 * @return CString
	 */
	CString getContact();

	/**
	 * The Cost field shows the total scheduled, or projected, cost for a task,
	 * based on costs already incurred for work performed by all resources assigned
	 * to the task, in addition to the costs planned for the remaining work for the
	 * assignment. This can also be referred to as estimate at completion (EAC).
	 *
	 * @return cost amount
	 */
	NumberPtr getCost();

	/**
	 * The Cost Variance field shows the difference between the baseline cost
	 * and total cost for a task. The total cost is the current estimate of costs
	 * based on actual costs and remaining costs.
	 *
	 * @return amount
	 */
	NumberPtr getCostVariance();

	/**
	 * The Created field contains the DatePtr and time when a task was added
	 * to the project.
	 *
	 * @return Date
	 */
	DatePtr getCreateDate();

	/**
	 * The Critical field indicates whether a task has any room in the schedule
	 * to slip, or if a task is on the critical path. The Critical field contains
	 * Yes if the task is critical and No if the task is not critical.
	 *
	 * @return bool
	 */
	bool getCritical();

	/**
	 * The CV (earned value cost variance) field shows the difference between
	 * how much it should have cost to achieve the current level of completion
	 * on the task, and how much it has actually cost to achieve the current
	 * level of completion up to the status DatePtr or today's Date.
	 * How Calculated   CV is the difference between BCWP
	 * (budgeted cost of work performed) and ACWP
	 * (actual cost of work performed). Microsoft Project calculates
	 * the CV as follows: CV = BCWP - ACWP
	 *
	 * @return sum of earned value cost variance
	 */
	NumberPtr getCV();

	/**
	 * Delay , in MPX files as eg '0ed'. Use duration
	 *
	 * @return Duration
	 */
	DurationPtr getLevelingDelay();

	/**
	 * The DurationPtr field is the total span of active working time for a task.
	 * This is generally the amount of time from the start to the finish of a task.
	 * The default for new tasks is 1 day (1d).
	 *
	 * @return Duration
	 */
	DurationPtr getDuration();

	/**
	 * Retrieves the duration text of a manually scheduled task.
	 *
	 * @return duration text
	 */
	CString getDurationText();

	/**
	 * Set a duration value.
	 *
	 * @param index duration index (1-10)
	 * @param value duration value
	 */
	void setDuration(int index, DurationPtr value);

	/**
	 * Retrieve a duration value.
	 *
	 * @param index duration index (1-10)
	 * @return duration value
	 */
	DurationPtr getDuration(int index);

	/**
	 * The DurationPtr Variance field contains the difference between the
	 * baseline duration of a task and the total duration (current estimate)
	 * of a task.
	 *
	 * @return Duration
	 */
	DurationPtr getDurationVariance();

	/**
	 * The Early Finish field contains the earliest DatePtr that a task could
	 * possibly finish, based on early finish Dates of predecessor and
	 * successor tasks, other constraints, and any leveling delay.
	 *
	 * @return Date
	 */
	DatePtr getEarlyFinish();

	/**
	 * The Early Start field contains the earliest DatePtr that a task could
	 * possibly begin, based on the early start Dates of predecessor and
	 * successor tasks, and other constraints.
	 *
	 * @return Date
	 */
	DatePtr getEarlyStart();

	/**
	 * The Finish field shows the DatePtr and time that a task is scheduled to
	 * be completed. You can enter the finish DatePtr you want, to indicate the
	 * DatePtr when the task should be completed. Or, you can have Microsoft
	 * Project calculate the finish Date.
	 *
	 * @return Date
	 */
	DatePtr getFinish();

	/**
	 * Retrieves the finish text of a manually scheduled task.
	 *
	 * @return finish text
	 */
	CString getFinishText();

	/**
	 * Set a finish value.
	 *
	 * @param index finish index (1-10)
	 * @param value finish value
	 */
	void setFinish(int index, DatePtr value);

	/**
	 * Retrieve a finish value.
	 *
	 * @param index finish index (1-10)
	 * @return finish value
	 */
	DatePtr getFinish(int index);

	/**
	 * Calculate the finish variance.
	 *
	 * @return finish variance
	 */
	DurationPtr getFinishVariance();

	/**
	 * The Fixed Cost field shows any task expense that is not associated
	 * with a resource cost.
	 *
	 * @return currency amount
	 */
	NumberPtr getFixedCost();

	/**
	 * Set a flag value.
	 *
	 * @param index flag index (1-20)
	 * @param value flag value
	 */
	void setFlag(int index, bool value);

	/**
	 * Retrieve a flag value.
	 *
	 * @param index flag index (1-20)
	 * @return flag value
	 */
	bool getFlag(int index);

	/**
	 * The Free Slack field contains the amount of time that a task can be
	 * delayed without delaying any successor tasks. If the task has no
	 * successors, free slack is the amount of time that a task can be
	 * delayed without delaying the entire project's finish Date.
	 *
	 * @return Duration
	 */
	DurationPtr getFreeSlack();

	/**
	 * The Hide Bar field indicates whether the Gantt bars and CalendarPtr bars
	 * for a task are hidden. Click Yes in the Hide Bar field to hide the
	 * bar for the task. Click No in the Hide Bar field to show the bar
	 * for the task.
	 *
	 * @return bool
	 */
	bool getHideBar();

	/**
	 * The ID field contains the identifier number that Microsoft Project
	 * automatically assigns to each task as you add it to the project.
	 * The ID indicates the position of a task with respect to the other tasks.
	 *
	 * @return the task ID
	 */
	int getID();

	/**
	 * The Late Finish field contains the latest DatePtr that a task can finish
	 * without delaying the finish of the project. This DatePtr is based on the
	 * task's late start Date, as well as the late start and late finish
	 * Dates of predecessor and successor
	 * tasks, and other constraints.
	 *
	 * @return Date
	 */
	DatePtr getLateFinish();

	/**
	 * The Late Start field contains the latest DatePtr that a task can start
	 * without delaying the finish of the project. This DatePtr is based on
	 * the task's start Date, as well as the late start and late finish
	 * Dates of predecessor and successor tasks, and other constraints.
	 *
	 * @return Date
	 */
	DatePtr getLateStart();

	/**
	 * The Linked Fields field indicates whether there are OLE links to the task,
	 * either from elsewhere in the active project, another Microsoft Project file,
	 * or from another program.
	 *
	 * @return bool
	 */
	bool getLinkedFields();

	/**
	 * The Marked field indicates whether a task is marked for further action or
	 * identification of some kind. To mark a task, click Yes in the Marked field.
	 * If you don't want a task marked, click No.
	 *
	 * @return true for marked
	 */
	bool getMarked();

	/**
	 * The Milestone field indicates whether a task is a milestone.
	 *
	 * @return bool
	 */
	bool getMilestone();

	/**
	 * Retrieves the task name.
	 *
	 * @return task name
	 */
	CString getName();

	/**
	 * The Notes field contains notes that you can enter about a task.
	 * You can use task notes to help maintain a history for a task.
	 *
	 * @return notes
	 */
	CString getNotes();

	/**
	 * Set a number value.
	 *
	 * @param index number index (1-20)
	 * @param value number value
	 */
	void setNumber(int index, NumberPtr value);

	/**
	 * Retrieve a number value.
	 *
	 * @param index number index (1-20)
	 * @return number value
	 */
	NumberPtr getNumber(int index);

	/**
	 * The Objects field contains the number of objects attached to a task.
	 * Microsoft Project counts the number of objects linked or embedded to a task.
	 * However, objects in the Notes box in the std::shared_ptr<Resource2> Form are not included
	 * in this count.
	 *
	 * @return int
	 */
	int getObjects();

	/**
	 * The Outline Level field contains the number that indicates the level
	 * of the task in the project outline hierarchy.
	 *
	 * @return int
	 */
	int getOutlineLevel();

	/**
	 * The Outline NumberPtr field contains the number of the task in the structure
	 * of an outline. This number indicates the task's position within the
	 * hierarchical structure of the project outline. The outline number is
	 * similar to a WBS (work breakdown structure) number,
	 * except that the outline number is automatically entered by
	 * Microsoft Project.
	 *
	 * @return CString
	 */
	CString getOutlineNumber();

	/**
	 * Retrieves the list of predecessors for this task.
	 *
	 * @return list of predecessor std::shared_ptr<Relation> instances
	 */
	std::shared_ptr<List<std::shared_ptr<Relation>>> getPredecessors();

	/**
	 * Retrieves the list of succesors for this task.
	 *
	 * @return list of successor std::shared_ptr<Relation> instances
	 */
	std::shared_ptr<List<std::shared_ptr<Relation>>> getSuccessors();

	/**
	 * The PriorityPtr field provides choices for the level of importance
	 * assigned to a task, which in turn indicates how readily a task can be
	 * delayed or split during resource leveling.
	 * The default priority is Medium. Those tasks with a priority
	 * of Do Not Level are never delayed or split when Microsoft Project levels
	 * tasks that have overallocated resources assigned.
	 *
	 * @return priority class instance
	 */
	PriorityPtr getPriority();

	/**
	 * The Project field shows the name of the project from which a task
	 * originated.
	 * This can be the name of the active project file-> If there are other
	 * projects inserted
	 * into the active project file, the name of the inserted project appears
	 * in this field
	 * for the task.
	 *
	 * @return name of originating project
	 */
	CString getProject();

	/**
	 * The Remaining Cost field shows the remaining scheduled expense of a
	 * task that will be incurred in completing the remaining scheduled work
	 * by all resources assigned to the task.
	 *
	 * @return remaining cost
	 */
	NumberPtr getRemainingCost();

	/**
	 * The Remaining DurationPtr field shows the amount of time required
	 * to complete the unfinished portion of a task.
	 *
	 * @return Duration
	 */
	DurationPtr getRemainingDuration();

	/**
	 * The Remaining Work field shows the amount of time, or person-hours,
	 * still required by all assigned resources to complete a task.
	 *
	 * @return the amount of time still required to complete a task
	 */
	DurationPtr getRemainingWork();

	/**
	 * The std::shared_ptr<Resource2> Group2 field contains the list of resource groups to which
	 * the resources assigned to a task belong.
	 *
	 * @return single string list of groups
	 */
	CString getResource2Group2();

	/**
	 * The std::shared_ptr<Resource2> Initials field lists the abbreviations for the names of
	 * resources assigned to a task. These initials can serve as substitutes
	 * for the names.
	 *
	 * Note that MS Project 98 does not export values for this field when
	 * writing an MPX file, and the field is not currently populated by MPXJ
	 * when reading an MPP file->
	 *
	 * @return CString containing a comma separated list of initials
	 */
	CString getResource2Initials();

	/**
	 * The std::shared_ptr<Resource2> Names field lists the names of all resources assigned
	 * to a task.
	 *
	 * Note that MS Project 98 does not export values for this field when
	 * writing an MPX file, and the field is not currently populated by MPXJ
	 * when reading an MPP file->
	 *
	 * @return CString containing a comma separated list of names
	 */
	CString getResource2Names();

	/**
	 * The Resume field shows the DatePtr that the remaining portion of a task
	 * is scheduled to resume after you enter a new value for the % Complete
	 * field. The Resume field is also recalculated when the remaining portion
	 * of a task is moved to a new Date.
	 *
	 * @return Date
	 */
	DatePtr getResume();

	/**
	 * For subtasks, the Rollup field indicates whether information on the
	 * subtask Gantt bars
	 * will be rolled up to the summary task bar. For summary tasks, the
	 * Rollup field indicates
	 * whether the summary task bar displays rolled up bars. You must
	 * have the Rollup field for
	 * summary tasks set to Yes for any subtasks to roll up to them.
	 *
	 * @return bool
	 */
	bool getRollup();

	/**
	 * The Start field shows the DatePtr and time that a task is scheduled to begin.
	 * You can enter the start DatePtr you want, to indicate the DatePtr when the task
	 * should begin. Or, you can have Microsoft Project calculate the start Date.
	 *
	 * @return Date
	 */
	DatePtr getStart();

	/**
	 * Retrieve the start text for a manually scheduled task.
	 *
	 * @return start text
	 */
	CString getStartText();

	/**
	 * Set a start value.
	 *
	 * @param index start index (1-10)
	 * @param value start value
	 */
	void setStart(int index, DatePtr value);

	/**
	 * Retrieve a start value.
	 *
	 * @param index start index (1-10)
	 * @return start value
	 */
	DatePtr getStart(int index);

	/**
	 * Calculate the start variance.
	 *
	 * @return start variance
	 */
	DurationPtr getStartVariance();

	/**
	 * The Stop field shows the DatePtr that represents the end of the actual
	 * portion of a task. Typically, Microsoft Project calculates the stop Date.
	 * However, you can edit this DatePtr as well.
	 *
	 * @return Date
	 */
	DatePtr getStop();

	/**
	 * Contains the file name and path of the sub project represented by
	 * the current task.
	 *
	 * @return sub project file path
	 */
	CString getSubprojectName();

	/**
	 * The Summary field indicates whether a task is a summary task.
	 *
	 * @return bool, true-is summary task
	 */
	bool getSummary();

	/**
	 * The SV (earned value schedule variance) field shows the difference in
	 * cost terms between the current progress and the baseline plan of the
	 * task up to the status DatePtr or today's Date. You can use SV to
	 * check costs to determine whether tasks are on schedule.
	 *
	 * @return -earned value schedule variance
	 */
	NumberPtr getSV();

	/**
	 * Set a text value.
	 *
	 * @param index text index (1-30)
	 * @param value text value
	 */
	void setText(int index, CString value);

	/**
	 * Retrieve a text value.
	 *
	 * @param index text index (1-30)
	 * @return text value
	 */
	CString getText(int index);

	/**
	 * Set an outline code value.
	 *
	 * @param index outline code index (1-10)
	 * @param value outline code value
	 */
	void setOutlineCode(int index, CString value);

	/**
	 * Retrieve an outline code value.
	 *
	 * @param index outline code index (1-10)
	 * @return outline code value
	 */
	CString getOutlineCode(int index);

	/**
	 * The Total Slack field contains the amount of time a task can be
	 * delayed without delaying the project's finish Date.
	 *
	 * @return string representing duration
	 */
	DurationPtr getTotalSlack();

	/**
	 * The Unique ID field contains the number that Microsoft Project
	 * automatically designates whenever a new task is created. This number
	 * indicates the sequence in which the task was
	 * created, regardless of placement in the schedule.
	 *
	 * @return CString
	 */
	int getUniqueID();

	/**
	 * The UpDate Needed field indicates whether a TeamUpDate message
	 * should be sent to the assigned resources because of changes to the
	 * start Date, finish Date, or resource reassignments of the task.
	 *
	 * @return true if needed.
	 */
	bool getUpDateNeeded();

	/**
	 * The work breakdown structure code. The WBS field contains an
	 * alphanumeric code you can use to represent the task's position within
	 * the hierarchical structure of the project. This field is similar to
	 * the outline number, except that you can edit it.
	 *
	 * @return string
	 */
	CString getWBS();

	/**
	 * The Work field shows the total amount of work scheduled to be performed
	 * on a task by all assigned resources. This field shows the total work,
	 * or person-hours, for a task.
	 *
	 * @return DurationPtr representing duration .
	 */
	DurationPtr getWork();

	/**
	 * The Work Variance field contains the difference between a task's
	 * baseline work and the currently scheduled work.
	 *
	 * @return DurationPtr representing duration.
	 */
	DurationPtr getWorkVariance();

	/**
	 * This method retrieves a reference to the parent of this task, as
	 * defined by the outline level. If this task is at the top level,
	 * this method will return NULL.
	 *
	 * @return parent task
	 */
	Task* getParentTask();

	/**
	 * This method retrieves a list of child tasks relative to the
	 * current task, as defined by the outine level. If there
	 * are no child tasks, this method will return an empty list.
	 *
	 * @return child tasks
	 */
	virtual List<Task*>* getChildTasks();


	/**
	*	@brief    得到当前任务中所有的子任务(包括自己)
	*	@param    [in]		pTask
	*	@param    [out]		pTaskList
	*/
	void getAllTasks(Task* pTask, List<Task*>* pTaskList);

	/**
	*	@brief    得到与当前任务存在父级关系的所有任务
	*	@param    [in]		pTask
	*	@param    [out]		pTaskList
	*/
	void getAllParentTasks(Task* pTask, List<Task*>* pParentTaskList);

	/**
	*	@brief    计算该任务下所有子级任务的数量
	*	@param    [in]	pTask		目标任务
	*	@param    [out]	nCount	目标任务下所有子级任务的数量
	*/
	void getAllChildCount(Task* pTask, int &nCount);
	/**
	 * This method implements the only method in the Comparable interface.
	 * This allows Tasks to be compared and sorted based on their ID value.
	 * Note that if the MPX/MPP file has been generated by MSP, the ID value
	 * will always be in the correct sequence. The Unique ID value will not
	 * necessarily be in the correct sequence as task insertions and deletions
	 * will change the order.
	 *
	 * @param o object to compare this instance with
	 * @return result of comparison
	 */
	virtual int compareTo(Task* o);

	/**
	 * This method retrieves a flag indicating whether the duration of the
	 * task has only been estimated.
	 *
	 * @return bool
	 */
	bool getEstimated();

	/**
	 * This method retrieves a flag indicating whether the duration of the
	 * task has only been estimated.

	 * @param estimated bool flag
	 */
	void setEstimated(bool estimated);

	/**
	 * This method retrieves the deadline for this task.
	 *
	 * @return Task* deadline
	 */
	DatePtr getDeadline();

	/**
	 * This method sets the deadline for this task.
	 *
	 * @param deadline deadline Date
	 */
	void setDeadline(DatePtr deadline);

	/**
	 * This method retrieves the task type.
	 *
	 * @return int representing the task type
	 */
	TaskType getType();

	/**
	 * This method sets the task type.
	 *
	 * @param type task type
	 */
	void setType(TaskType type);

	/**
	 * Retrieves the flag indicating if this is a NULL task.
	 *
	 * @return bool flag
	 */
	bool getNull();

	/**
	 * Sets the flag indicating if this is a NULL task.
	 *
	 * @param isNull bool flag
	 */
	void setNull(bool isNull);

	/**
	 * Retrieve the WBS level.
	 *
	 * @return WBS level
	 */
	CString getWBSLevel();

	/**
	 * Set the WBS level.
	 *
	 * @param wbsLevel WBS level
	 */
	void setWBSLevel(CString wbsLevel);

	/**
	 * Retrieve the resume valid flag.
	 *
	 * @return resume valie flag
	 */
	bool getResumeValid();

	/**
	 * Set the resume valid flag.
	 *
	 * @param resumeValid resume valid flag
	 */
	void setResumeValid(bool resumeValid);

	/**
	 * Retrieve the recurring flag.
	 *
	 * @return recurring flag
	 */
	bool getRecurring();

	/**
	 * Set the recurring flag.
	 *
	 * @param recurring recurring flag
	 */
	void setRecurring(bool recurring);

	/**
	 * Retrieve the over allocated flag.
	 *
	 * @return over allocated flag
	 */
	bool getOverAllocated();

	/**
	 * Set the over allocated flag.
	 *
	 * @param overAllocated over allocated flag
	 */
	void setOverAllocated(bool overAllocated);

	/**
	 * Where a task in an MPP file represents a task from a subproject,
	 * this value will be non-zero. The value itself is the unique ID
	 * value shown in the parent project. To retrieve the value of the
	 * task unique ID in the child project, remove the top two bytes:
	 *
	 * taskID = (subprojectUniqueID & 0xFFFF)
	 *
	 * @return sub project unique task ID
	 */
	int getSubprojectTaskUniqueID();

	/**
	 * Sets the sub project unique task ID.
	 *
	 * @param subprojectUniqueTaskID subproject unique task ID
	 */
	void setSubprojectTaskUniqueID(int subprojectUniqueTaskID);

	/**
	 * Where a task in an MPP file represents a task from a subproject,
	 * this value will be non-zero. The value itself is the ID
	 * value shown in the parent project.
	 *
	 * @return sub project task ID
	 */
	int getSubprojectTaskID();

	/**
	 * Sets the sub project task ID.
	 *
	 * @param subprojectTaskID subproject task ID
	 */
	void setSubprojectTaskID(int subprojectTaskID);

	/**
	 * Sets the offset added to unique task IDs from sub projects
	 * to generate the task ID shown in the master project.
	 *
	 * @param offset unique ID offset
	 */
	void setSubprojectTasksUniqueIDOffset(int offset);

	/**
	 * Retrieves the offset added to unique task IDs from sub projects
	 * to generate the task ID shown in the master project.
	 *
	 * @return unique ID offset
	 */
	int getSubprojectTasksUniqueIDOffset();

	/**
	 * Retrieve the subproject read only flag.
	 *
	 * @return subproject read only flag
	 */
	bool getSubprojectReadOnly();

	/**
	 * Set the subproject read only flag.
	 *
	 * @param subprojectReadOnly subproject read only flag
	 */
	void setSubprojectReadOnly(bool subprojectReadOnly);

	/**
	 * Retrieves the external task flag.
	 *
	 * @return external task flag
	 */
	bool getExternalTask();

	/**
	 * Sets the external task flag.
	 *
	 * @param externalTask external task flag
	 */
	void setExternalTask(bool externalTask);

	/**
	 * Retrieves the external task project file name.
	 *
	 * @return external task project file name
	 */
	CString getExternalTaskProject();

	/**
	 * Sets the external task project file name.
	 *
	 * @param externalTaskProject external task project file name
	 */
	void setExternalTaskProject(CString externalTaskProject);

	/**
	 * Retrieve the ACWP value.
	 *
	 * @return ACWP value
	 */
	NumberPtr getACWP();

	/**
	 * Set the ACWP value.
	 *
	 * @param acwp ACWP value
	 */
	void setACWP(NumberPtr acwp);

	/**
	 * Retrieve the leveling delay format.
	 *
	 * @return leveling delay  format
	 */
	TimeUnit getLevelingDelayFormat();

	/**
	 * Set the leveling delay format.
	 *
	 * @param levelingDelayFormat leveling delay format
	 */
	void setLevelingDelayFormat(TimeUnit levelingDelayFormat);

	/**
	 * Retrieves the ignore resource celandar flag.
	 *
	 * @return ignore resource celandar flag
	 */
	bool getIgnoreResource2Calendar();

	/**
	 * Sets the ignore resource celandar flag.
	 *
	 * @param ignoreResource2Calendar ignore resource celandar flag
	 */
	void setIgnoreResource2Calendar(bool ignoreResource2Calendar);

	/**
	 * Retrieves the physical percent complete value.
	 *
	 * @return physical percent complete value
	 */
	int getPhysicalPercentComplete();

	/**
	 * Srts the physical percent complete value.
	 *
	 * @param physicalPercentComplete physical percent complete value
	 */
	void setPhysicalPercentComplete(int physicalPercentComplete);

	/**
	 * Retrieves the earned value method.
	 *
	 * @return earned value method
	 */
	EarnedValueMethod getEarnedValueMethod();

	/**
	 * Sets the earned value method.
	 *
	 * @param earnedValueMethod earned value method
	 */
	void setEarnedValueMethod(EarnedValueMethod earnedValueMethod);

	/**
	 * Retrieves the actual work protected value.
	 *
	 * @return actual work protected value
	 */
	DurationPtr getActualWorkProtected();

	/**
	 * Sets the actual work protected value.
	 *
	 * @param actualWorkProtected actual work protected value
	 */
	void setActualWorkProtected(DurationPtr actualWorkProtected);

	/**
	 * Retrieves the actual overtime work protected value.
	 *
	 * @return actual overtime work protected value
	 */
	DurationPtr getActualOvertimeWorkProtected();

	/**
	 * Sets the actual overtime work protected value.
	 *
	 * @param actualOvertimeWorkProtected actual overtime work protected value
	 */
	void setActualOvertimeWorkProtected(DurationPtr actualOvertimeWorkProtected);

	/**
	 * Retrieve the amount of regular work.
	 *
	 * @return amount of regular work
	 */
	DurationPtr getRegularWork();

	/**
	 * Set the amount of regular work.
	 *
	 * @param regularWork amount of regular work
	 */
	void setRegularWork(DurationPtr regularWork);

	/**
	 * Sets the effort driven flag.
	 *
	 * @param flag value
	 */
	void setEffortDriven(bool flag);

	/**
	 * Retrieves the effort driven flag.
	 *
	 * @return Flag value
	 */
	bool getEffortDriven();

	/**
	 * Set a DatePtr value.
	 *
	 * @param index DatePtr index (1-10)
	 * @param value DatePtr value
	 */
	void setDate(int index, DatePtr value);

	/**
	 * Retrieve a DatePtr value.
	 *
	 * @param index DatePtr index (1-10)
	 * @return DatePtr value
	 */
	DatePtr getDate(int index);

	/**
	 * Retrieves the overtime cost.
	 *
	 * @return Cost value
	 */
	NumberPtr getOvertimeCost();

	/**
	 * Sets the overtime cost value.
	 *
	 * @param number Cost value
	 */
	void setOvertimeCost(NumberPtr number);

	/**
	 * Retrieves the actual overtime cost for this task.
	 *
	 * @return actual overtime cost
	 */
	NumberPtr getActualOvertimeCost();

	/**
	 * Sets the actual overtime cost for this task.
	 *
	 * @param cost actual overtime cost
	 */
	void setActualOvertimeCost(NumberPtr cost);

	/**
	 * Retrieves the actual overtime work value.
	 *
	 * @return actual overtime work value
	 */
	DurationPtr getActualOvertimeWork();

	/**
	 * Sets the actual overtime work value.
	 *
	 * @param work actual overtime work value
	 */
	void setActualOvertimeWork(DurationPtr work);

	/**
	 * Retrieves the fixed cost accrual flag value.
	 *
	 * @return fixed cost accrual flag
	 */
	AccrueType getFixedCostAccrual();

	/**
	 * Sets the fixed cost accrual flag value.
	 *
	 * @param type fixed cost accrual type
	 */
	void setFixedCostAccrual(AccrueType type);

	/**
	 * Retrieves the task hyperlink attribute.
	 *
	 * @return hyperlink attribute
	 */
	CString getHyperlink();

	/**
	 * Retrieves the task hyperlink address attribute.
	 *
	 * @return hyperlink address attribute
	 */
	CString getHyperlinkAddress();

	/**
	 * Retrieves the task hyperlink sub-address attribute.
	 *
	 * @return hyperlink sub address attribute
	 */
	CString getHyperlinkSubAddress();

	/**
	 * Sets the task hyperlink attribute.
	 *
	 * @param text hyperlink attribute
	 */
	void setHyperlink(CString text);

	/**
	 * Sets the task hyperlink address attribute.
	 *
	 * @param text hyperlink address attribute
	 */
	void setHyperlinkAddress(CString text);

	/**
	 * Sets the task hyperlink sub address attribute.
	 *
	 * @param text hyperlink sub address attribute
	 */
	void setHyperlinkSubAddress(CString text);

	/**
	 * Retrieves the level assignments flag.
	 *
	 * @return level assignments flag
	 */
	bool getLevelAssignments();

	/**
	 * Sets the level assignments flag.
	 *
	 * @param flag level assignments flag
	 */
	void setLevelAssignments(bool flag);

	/**
	 * Retrieves the leveling can split flag.
	 *
	 * @return leveling can split flag
	 */
	bool getLevelingCanSplit();

	/**
	 * Sets the leveling can split flag.
	 *
	 * @param flag leveling can split flag
	 */
	void setLevelingCanSplit(bool flag);

	/**
	 * Retrieves the overtime work attribute.
	 *
	 * @return overtime work value
	 */
	DurationPtr getOvertimeWork();

	/**
	 * Sets the overtime work attribute.
	 *
	 * @param work overtime work value
	 */
	void setOvertimeWork(DurationPtr work);

	/**
	 * Retrieves the preleveled start attribute.
	 *
	 * @return preleveled start
	 */
	DatePtr getPreleveledStart();

	/**
	 * Retrieves the preleveled finish attribute.
	 *
	 * @return preleveled finish
	 */
	DatePtr getPreleveledFinish();

	/**
	 * Sets the preleveled start attribute.
	 *
	 * @param DatePtr preleveled start attribute
	 */
	void setPreleveledStart(DatePtr Date);

	/**
	 * Sets the preleveled finish attribute.
	 *
	 * @param DatePtr preleveled finish attribute
	 */
	void setPreleveledFinish(DatePtr Date);

	/**
	 * Retrieves the remaining overtime work attribute.
	 *
	 * @return remaining overtime work
	 */
	DurationPtr getRemainingOvertimeWork();

	/**
	 * Sets the remaining overtime work attribute.
	 *
	 * @param work remaining overtime work
	 */
	void setRemainingOvertimeWork(DurationPtr work);

	/**
	 * Retrieves the remaining overtime cost.
	 *
	 * @return remaining overtime cost value
	 */
	NumberPtr getRemainingOvertimeCost();

	/**
	 * Sets the remaining overtime cost value.
	 *
	 * @param cost overtime cost value
	 */
	void setRemainingOvertimeCost(NumberPtr cost);

	/**
	 * Retrieves the base calendar instance associated with this task.
	 * Note that this attribute appears in MPP9 and MSPDI files.
	 *
	 * @return ProjectCalendar* instance
	 */
	ProjectCalendarPtr getCalendar();

	/**
	 * Sets the name of the base calendar associated with this task.
	 * Note that this attribute appears in MPP9 and MSPDI files.
	 *
	 * @param calendar calendar instance
	 */
	void setCalendar(ProjectCalendarPtr calendar);

	/**
	 * Retrieve a flag indicating if the task is shown as expanded
	 * in MS Project. If this flag is set to true, any sub tasks
	 * for this current task will be visible. If this is false,
	 * any sub tasks will be hidden.
	 *
	 * @return bool flag
	 */
	bool getExpanded();

	/**
	 * Set a flag indicating if the task is shown as expanded
	 * in MS Project. If this flag is set to true, any sub tasks
	 * for this current task will be visible. If this is false,
	 * any sub tasks will be hidden.
	 *
	 * @param expanded bool flag
	 */
	void setExpanded(bool expanded);

	/**
	 * Set the start slack.
	 *
	 * @param duration start slack
	 */
	void setStartSlack(DurationPtr duration);

	/**
	 * Set the finish slack.
	 *
	 * @param duration finish slack
	 */
	void setFinishSlack(DurationPtr duration);

	/**
	 * Retrieve the start slack.
	 *
	 * @return start slack
	 */
	DurationPtr getStartSlack();

	/**
	 * Retrieve the finish slack.
	 *
	 * @return finish slack
	 */
	DurationPtr getFinishSlack();

	/**
	 * Retrieve the value of a field using its alias.
	 *
	 * @param alias field alias
	 * @return field value
	 */
	ObjectPtr getFieldByAlias(CString alias);

	/**
	 * Set the value of a field using its alias.
	 *
	 * @param alias field alias
	 * @param value field value
	 */
	void setFieldByAlias(CString alias, ObjectPtr value);

	/**
	 * This method retrieves a list of task splits. Each split is represented
	 * by a DateRange instance. The list will always follow the pattern
	 * task range, split range, task range and so on.
	 *
	 * Note that this method will return NULL if the task is not split.
	 *
	 * @return list of split times
	 */
	DateRangeListPtr getSplits();

	/**
	 * Internal method used to set the list of splits.
	 *
	 * @param splits list of split times
	 */
	void setSplits(DateRangeListPtr splits);

	/**
	 * Task* splits contain the time up to which the splits are completed.
	 *
	 * @return DurationPtr of completed time for the splits.
	 */
	DatePtr getSplitCompleteDuration();

	/**
	 * Set the time up to which the splits are completed.
	 *
	 * @param splitsComplete DurationPtr of completed time for the splits.
	 */
	void setSplitCompleteDuration(DatePtr splitsComplete);

	/**
	 * Removes this task from the project.
	 */
	void remove();

	/**
	 * Retrieve the sub project represented by this task.
	 *
	 * @return sub project
	 */
	SubProjectPtr getSubProject();

	/**
	 * Set the sub project represented by this task.
	 *
	 * @param subProject sub project
	 */
	void setSubProject(SubProjectPtr subProject);

	/**
	 * Retrieve an enterprise field value.
	 *
	 * @param index field index
	 * @return field value
	 */
	NumberPtr getEnterpriseCost(int index);

	/**
	 * Set an enterprise field value.
	 *
	 * @param index field index
	 * @param value field value
	 */
	void setEnterpriseCost(int index, NumberPtr value);

	/**
	 * Retrieve an enterprise field value.
	 *
	 * @param index field index
	 * @return field value
	 */
	DatePtr getEnterpriseDate(int index);

	/**
	 * Set an enterprise field value.
	 *
	 * @param index field index
	 * @param value field value
	 */
	void setEnterpriseDate(int index, DatePtr value);

	/**
	 * Retrieve an enterprise field value.
	 *
	 * @param index field index
	 * @return field value
	 */
	DurationPtr getEnterpriseDuration(int index);

	/**
	 * Set an enterprise field value.
	 *
	 * @param index field index
	 * @param value field value
	 */
	void setEnterpriseDuration(int index, DurationPtr value);

	/**
	 * Retrieve an enterprise field value.
	 *
	 * @param index field index
	 * @return field value
	 */
	bool getEnterpriseFlag(int index);

	/**
	 * Set an enterprise field value.
	 *
	 * @param index field index
	 * @param value field value
	 */
	void setEnterpriseFlag(int index, bool value);

	/**
	 * Retrieve an enterprise field value.
	 *
	 * @param index field index
	 * @return field value
	 */
	NumberPtr getEnterpriseNumber(int index);

	/**
	 * Set an enterprise field value.
	 *
	 * @param index field index
	 * @param value field value
	 */
	void setEnterpriseNumber(int index, NumberPtr value);

	/**
	 * Retrieve an enterprise field value.
	 *
	 * @param index field index
	 * @return field value
	 */
	CString getEnterpriseText(int index);

	/**
	 * Set an enterprise field value.
	 *
	 * @param index field index
	 * @param value field value
	 */
	void setEnterpriseText(int index, CString value);

	/**
	 * Retrieve an enterprise custom field value.
	 *
	 * @param index field index
	 * @return field value
	 */
	CString getEnterpriseCustomField(int index);

	/**
	 * Set an enterprise custom field value.
	 *
	 * @param index field index
	 * @param value field value
	 */
	void setEnterpriseCustomField(int index, CString value);

	/**
	 * Set a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @param value baseline value
	 */
	void setBaselineCost(int baselineNumber, NumberPtr value);

	/**
	 * Set a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @param value baseline value
	 */
	void setBaselineDuration(int baselineNumber, DurationPtr value);

	/**
	 * Set a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @param value baseline value
	 */
	void setBaselineFinish(int baselineNumber, DatePtr value);

	/**
	 * Set a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @param value baseline value
	 */
	void setBaselineStart(int baselineNumber, DatePtr value);

	/**
	 * Set a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @param value baseline value
	 */
	void setBaselineWork(int baselineNumber, DurationPtr value);

	/**
	 * Retrieve a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @return baseline value
	 */
	NumberPtr getBaselineCost(int baselineNumber);

	/**
	 * Retrieve a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @return baseline value
	 */
	DurationPtr getBaselineDuration(int baselineNumber);

	/**
	 * Retrieves the baseline duration text value.
	 *
	 * @param baselineNumber baseline number
	 * @return baseline duration text value
	 */
	CString getBaselineDurationText(int baselineNumber);

	/**
	 * Sets the baseline duration text value.
	 *
	 * @param baselineNumber baseline number
	 * @param value baseline duration text value
	 */
	void setBaselineDurationText(int baselineNumber, CString value);

	/**
	 * Retrieve a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @return baseline value
	 */
	DatePtr getBaselineFinish(int baselineNumber);

	/**
	 * Retrieves the baseline finish text value.
	 *
	 * @param baselineNumber baseline number
	 * @return baseline finish text value
	 */
	CString getBaselineFinishText(int baselineNumber);

	/**
	 * Sets the baseline finish text value.
	 *
	 * @param baselineNumber baseline number
	 * @param value baseline finish text value
	 */
	void setBaselineFinishText(int baselineNumber, CString value);

	/**
	 * Retrieve a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @return baseline value
	 */
	DatePtr getBaselineStart(int baselineNumber);

	/**
	 * Retrieves the baseline start text value.
	 *
	 * @param baselineNumber baseline number
	 * @return baseline start text value
	 */
	CString getBaselineStartText(int baselineNumber);

	/**
	 * Sets the baseline start text value.
	 *
	 * @param baselineNumber baseline number
	 * @param value baseline start text value
	 */
	void setBaselineStartText(int baselineNumber, CString value);

	/**
	 * Retrieve a baseline value.
	 *
	 * @param baselineNumber baseline index (1-10)
	 * @return baseline value
	 */
	DurationPtr getBaselineWork(int baselineNumber);

	/**
	 * Retrieve the "complete through" Date.
	 *
	 * @return complete through Date
	 */
	DatePtr getCompleteThrough();

	/**
	 * Retrieve the summary progress Date.
	 *
	 * @return summary progress Date
	 */
	DatePtr getSummaryProgress();

	/**
	 * Set the summary progress Date.
	 *
	 * @param value summary progress Date
	 */
	void setSummaryProgress(DatePtr value);

	/**
	 * Retrieve the task GUID2.
	 *
	 * @return task GUID2
	 */
	LPCGUID getGUID();

	/**
	 * Set the task GUID2.
	 *
	 * @param value task GUID2
	 */
	void setGUID(LPCGUID value);

	/**
	 * Retrieves the task mode.
	 *
	 * @return task mode
	 */
	TaskMode getTaskMode();

	/**
	 * Sets the task mode.
	 *
	 * @param mode task mode
	 */
	void setTaskMode(TaskMode mode);

	/**
	 * Retrieves the active flag.
	 *
	 * @return active flag value
	 */
	bool getActive();

	/**
	 * Sets the active flag.
	 *
	 * @param active active flag value
	 */
	void setActive(bool active);

	/**
	 * This method allows a predecessor relationship to be removed from this
	 * task instance.  It will only delete relationships that exactly match the
	 * given targetTask, type and lag time.
	 *
	 * @param targetTask the predecessor task
	 * @param type relation type
	 * @param lag relation lag
	 * @return returns true if the relation is found and removed
	 */
	bool removePredecessor(Task* targetTask, RelationType type, DurationPtr lag);

private:
	/**
	 * Internal method used to locate an remove an item from a list Relations.
	 *
	 * @param relationList list of std::shared_ptr<Relation> instances
	 * @param targetTask target relationship task
	 * @param type target relationship type
	 * @param lag target relationship lag
	 * @return true if a relationship was removed
	 */
	bool removeRelation(std::shared_ptr<List<std::shared_ptr<Relation>>> relationList, 
		Task* targetTask, RelationType type, DurationPtr lag);

	/**
	 * Maps a field index to a TaskField instance.
	 *
	 * @param fields array of fields used as the basis for the mapping.
	 * @param index required field index
	 * @return TaskField instance
	 */
	TaskField selectField(const TaskField fields[], int index);

public:
	/**
	 * {@inheritDoc}
	 */
	virtual ObjectPtr getCachedValue(FieldType* field);
	ObjectPtr getCachedValue(TaskField field);

	/**
	 * {@inheritDoc}
	 */
	virtual ObjectPtr getCurrentValue(FieldType* field);

	/**
	 * {@inheritDoc}
	 */
	virtual void set(FieldType* field, ObjectPtr value);
	void set(TaskField field, ObjectPtr value);

private:
	/**
	 * Handle the change in a field value. Reset any cached calculated
	 * values affected by this change, pass on the event to any external
	 * listeners.
	 *
	 * @param field field changed
	 * @param oldValue old field value
	 * @param newValue new field value
	 */
	void fireFieldChangeEvent(TaskField field, ObjectPtr oldValue, ObjectPtr newValue);


	/**
	* This method inserts a name value pair into internal storage.
	*
	* @param field task field
	* @param value attribute value
	*/
	void set(FieldType* field, bool value);

	/**
	* Internal method used to test for the existence of a relationship
	* with a task.
	*
	* @param task target task
	* @param list list of relationships
	* @return bool flag
	*/
	bool isRelated(Task* task, std::shared_ptr<List<std::shared_ptr<Relation>>> list);

public:
	/**
	 * {@inheritDoc}
	 */
	virtual void addFieldListener(FieldListener* listener);

	/**
	 * {@inheritDoc}
	 */
	virtual void removeFieldListener(FieldListener* listener);


	///**
	// * {@inheritDoc}
	// */
	//virtual CString toString();

	/**
	 * Utility method used to determine if the supplied task
	 * is a predecessor of the current task.
	 *
	 * @param task potential predecessor task
	 * @return bool flag
	 */
	bool isPredecessor(Task* task);

	/**
	 * Utility method used to determine if the supplied task
	 * is a successor of the current task.
	 *
	 * @param task potential successor task
	 * @return bool flag
	 */
	bool isSucessor(Task* task);

	/**
	 * Disable events firing when fields are upDated.
	 */
	void disableEvents();

	/**
	 * Enable events firing when fields are upDated. This is the default state.
	 */
	void enableEvents();

private:

	/**
	 * Array of field values.
	 */
	ObjectPtrArray m_array;// = new Object[TaskField.MAX_VALUE];

	/**
	 * This is a reference to the parent task, as specified by the
	 * outline level.
	 */
	Task* m_parent;

	/**
	 * This list holds references to all tasks that are children of the
	 * current task as specified by the outline level.
	 */
	List<Task*> m_children;// = new List<Task*>();

	List<Task*> m_Allchildren;// = new List<Task*>();

	/**
	 * List of resource assignments for this task.
	 */
	List<std::shared_ptr<Resource2Assignment>> m_assignments;// = new List<std::shared_ptr<Resource2Assignment>>();

	/**
	 * Recurring task details associated with this task.
	 */
	RecurringTaskPtr m_recurringTask;

	bool m_eventsEnabled = true;
	bool m_null;
	CString m_wbsLevel;
	bool m_resumeValid;
	CString m_externalTaskProject;
	TimeUnit m_levelingDelayFormat;
	EarnedValueMethod m_earnedValueMethod;
	DurationPtr m_actualWorkProtected;
	DurationPtr m_actualOvertimeWorkProtected;
	bool m_expanded = true;

	DateRangeListPtr m_splits;
	DatePtr m_splitsComplete;
	SubProjectPtr m_subProject;
	List<FieldListener*> m_listeners;
};
