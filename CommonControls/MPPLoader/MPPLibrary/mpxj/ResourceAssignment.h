#pragma once

#include "FieldContainer.h"
#include "ProjectEntity.h"
#include "WorkContour.h"
#include "ResourceAssignmentWorkgroupFields.h"
#include "AssignmentField.h"
#include "TimephasedWorkContainer.h"
#include "CostRateTable.h"
#include "TimephasedCostContainer.h"
#include "common\DefaultTimephasedWorkContainer.h"
#include "ProjectCalendar.h"

class Task;
class Resource2;
//class ProjectCalendar;

/**
 * This class represents a resource assignment record from an MPX file->
 */
class COMMONCONTROLS_API Resource2Assignment : public ProjectEntity, public FieldContainer,
	public std::enable_shared_from_this<Resource2Assignment>
{
public:
   /**
    * Constructor.
    *
    * @param file The parent file to which this record belongs.
    */
	Resource2Assignment(ProjectFile* file);

   /**
    * Constructor.
    *
    * @param file The parent file to which this record belongs.
    * @param task The task to which this assignment is being made
    */
	Resource2Assignment(ProjectFile* file, Task* task);

   /**
    * This method allows a resource assignment workgroup fields record
    * to be added to the current resource assignment. A maximum of
    * one of these records can be added to a resource assignment record.
    *
    * @return Resource2AssignmentWorkgroupFields object
    * @throws MPXJException if MSP defined limit of 1 is exceeded
    */
	Resource2AssignmentWorkgroupFields* addWorkgroupAssignment();

   /**
    * Retrieve the unique ID of this resource assignment.
    * 
    * @return resource assignment unique ID
    */
	NumberPtr getUniqueID();

   /**
    * Set the unique ID of this resource assignment. 
    * 
    * @param uniqueID resource assignment unique ID
    */
	void setUniqueID(NumberPtr uniqueID);

   /**
    * Returns the units of this resource assignment.
    *
    * @return units
    */
	NumberPtr getUnits();

   /**
    * Sets the units for this resource assignment.
    *
    * @param val units
    */
	void setUnits(NumberPtr val);

   /**
    * Returns the work of this resource assignment.
    *
    * @return work
    */
	DurationPtr getWork();

   /**
    * Sets the work for this resource assignment.
    *
    * @param dur work
    */
	void setWork(DurationPtr dur);

   /**
    * Retrieve the baseline start Date.
    * 
    * @return baseline start Date
    */
	DatePtr getBaselineStart();

   /**
    * Set the baseline start Date.
    * 
    * @param start baseline start Date
    */
	void setBaselineStart(DatePtr start);

   /**
    * Retrieve the actual start Date.
    * 
    * @return actual start Date
    */
	DatePtr getActualStart();

   /**
    * Set the actual start Date.
    * 
    * @param start actual start Date
    */
	void setActualStart(DatePtr start);

   /**
    * Retrieve the baseline finish Date.
    * 
    * @return baseline finish Date
    */
	DatePtr getBaselineFinish();

   /**
    * Set the baseline finish Date.
    * 
    * @param finish baseline finish
    */
	void setBaselineFinish(DatePtr finish);

   /**
    * Retrieve the actual finish Date.
    * 
    * @return actual finish Date
    */
	DatePtr getActualFinish();

   /**
    * Set the actual finish Date.
    * 
    * @param finish actual finish
    */
	void setActualFinish(DatePtr finish);

   /**
    * Returns the baseline work of this resource assignment.
    *
    * @return planned work
    */
	DurationPtr getBaselineWork();

   /**
    * Sets the baseline work for this resource assignment.
    *
    * @param val planned work
    */
	void setBaselineWork(DurationPtr val);

   /**
    * Returns the actual completed work of this resource assignment.
    *
    * @return completed work
    */
	DurationPtr getActualWork();

   /**
    * Sets the actual completed work for this resource assignment.
    *
    * @param val actual completed work
    */
	void setActualWork(DurationPtr val);

   /**
    * Returns the overtime work done of this resource assignment.
    *
    * @return overtime work
    */
	DurationPtr getOvertimeWork();

   /**
    * Sets the overtime work for this resource assignment.
    *
    * @param overtimeWork overtime work
    */
	void setOvertimeWork(DurationPtr overtimeWork);

   /**
    * Returns the cost  of this resource assignment.
    *
    * @return cost
    */
	NumberPtr getCost();

   /**
    * Sets the cost for this resource assignment.
    *
    * @param cost cost
    */
	void setCost(NumberPtr cost);

   /**
    * Returns the planned cost for this resource assignment.
    *
    * @return planned cost
    */
	NumberPtr getBaselineCost();

   /**
    * Sets the planned cost for this resource assignment.
    *
    * @param val planned cost
    */
	void setBaselineCost(NumberPtr val);

   /**
    * Returns the actual cost for this resource assignment.
    *
    * @return actual cost
    */
	NumberPtr getActualCost();

   /**
    * Sets the actual cost so far incurred for this resource assignment.
    *
    * @param actualCost actual cost
    */
	void setActualCost(NumberPtr actualCost);

   /**
    * Returns the start of this resource assignment.
    *
    * @return start Date
    */
	DatePtr getStart();

   /**
    * Sets the start DatePtr for this resource assignment.
    *
    * @param val start Date
    */
	void setStart(DatePtr val);

   /**
    * Returns the finish DatePtr for this resource assignment.
    *
    * @return finish Date
    */
	DatePtr getFinish();

   /**
    * Sets the finish DatePtr for this resource assignment.
    *
    * @param val finish Date
    */
	void setFinish(DatePtr val);

   /**
    * Returns the delay for this resource assignment.
    *
    * @return delay
    */
	DurationPtr getDelay();

   /**
    * Sets the delay for this resource assignment.
    *
    * @param dur delay
    */
	void setDelay(DurationPtr dur);

   /**
    * Returns the resources unique id for this resource assignment.
    *
    * @return resources unique id
    */
	NumberPtr getResource2UniqueID();

   /**
    * Sets the resources unique id for this resource assignment.
    *
    * @param val resources unique id
    */
	void setResource2UniqueID(int val);

   /**
    * Gets the Resource2 Assignment Workgroup Fields if one exists.
    *
    * @return workgroup assignment object
    */
	Resource2AssignmentWorkgroupFields* getWorkgroupAssignment();

   /**
    * This method retrieves a reference to the task with which this
    * assignment is associated.
    *
    * @return task
    */
	Task* getTask();

   /**
    * This method retrieves a reference to the resource with which this
    * assignment is associated.
    *
    * @return resource
    */
	std::shared_ptr<Resource2> getResource2();

   /**
    * This method returns the Work Contour type of this Assignment.
    *
    * @return the Work Contour type
    */
	WorkContour getWorkContour();

   /**
    * This method sets the Work Contour type of this Assignment.
    *
    * @param workContour the Work Contour type
    */
	void setWorkContour(WorkContour workContour);

   /**
    * Removes this resource assignment from the project.
    */
	void remove();

   /**
    * Returns the remaining work for this resource assignment.
    *
    * @return remaining work
    */
	DurationPtr getRemainingWork();

   /**
    * Sets the remaining work for this resource assignment.
    *
    * @param remainingWork remaining work
    */
	void setRemainingWork(DurationPtr remainingWork);

   /**
    * Retrieves the leveling delay for this resource assignment.
    * 
    * @return leveling delay
    */
	DurationPtr getLevelingDelay();

   /**
    * Sets the leveling delay for this resource assignment.
    * 
    * @param levelingDelay leveling delay
    */
	void setLevelingDelay(DurationPtr levelingDelay);

   /**
    * Retrieves the timephased breakdown of the completed work for this
    * resource assignment. 
    * 
    * @return timephased completed work
    */
	TimephasedWorkListPtr getTimephasedActualWork();

   /**
    * Sets the timephased breakdown of the completed work for this
    * resource assignment.
    * 
    * @param data timephased data
    */
	void setTimephasedActualWork(TimephasedWorkContainerPtr data);

   /**
    * Retrieves the timephased breakdown of the planned work for this
    * resource assignment. 
    * 
    * @return timephased planned work
    */
	TimephasedWorkListPtr getTimephasedWork();

   /**
    * Sets the timephased breakdown of the planned work for this
    * resource assignment.
    * 
    * @param data timephased data 
    */
	void setTimephasedWork(DefaultTimephasedWorkContainerPtr data);

   /**
    * Retrieves the timephased breakdown of the planned overtime work for this
    * resource assignment. 
    * 
    * @return timephased planned work
    */
	TimephasedWorkListPtr getTimephasedOvertimeWork();

   /**
    * Sets the timephased breakdown of the actual overtime work
    * for this assignment.
    * 
    * @param data timephased work
    */
	void setTimephasedActualOvertimeWork(TimephasedWorkContainerPtr data);

   /**
    * Retrieves the timephased breakdown of the actual overtime work for this
    * resource assignment. 
    * 
    * @return timephased planned work
    */
	TimephasedWorkListPtr getTimephasedActualOvertimeWork();

   /**
    * Retrieves the timephased breakdown of cost.
    * 
    * @return timephased cost
    */
	TimephasedCostListPtr getTimephasedCost();

   /**
    * Retrieves the timephased breakdown of actual cost.
    * 
    * @return timephased actual cost
    */
	TimephasedCostListPtr getTimephasedActualCost();

private:
   /**
    * Generates timephased costs from timephased work where a single cost rate
    * applies to the whole assignment.
    * 
    * @param standardWorkList timephased work
    * @param overtimeWorkList timephased work 
    * @return timephased cost
    */
	TimephasedCostListPtr getTimephasedCostSingleRate(
		TimephasedWorkListPtr standardWorkList,
		TimephasedWorkListPtr overtimeWorkList);

   /**
    * Generates timephased costs from timephased work where multiple cost rates
    * apply to the assignment.
    * 
    * @param standardWorkList timephased work
    * @param overtimeWorkList timephased work
    * @return timephased cost
    */
	TimephasedCostListPtr getTimephasedCostMultipleRates(
		TimephasedWorkListPtr standardWorkList,
		TimephasedWorkListPtr overtimeWorkList);

   /**
    * Generates timephased costs from the assignment's cost value. Used for Cost type Resource2s.
    * 
    * @return timephased cost
    */
	TimephasedCostListPtr getTimephasedCostFixedAmount();

   /**
    * Generates timephased actual costs from the assignment's cost value. Used for Cost type Resource2s.
    * 
    * @return timephased cost
    */
	TimephasedCostListPtr getTimephasedActualCostFixedAmount();

   /**
    * Used for Cost type Resource2s. 
    * 
    * Generates a TimphasedCost block for the total amount on the start Date. This is useful
    * for Cost resources that have an AccrueAt value of Start. 
    * 
    * @param calendar calendar used by this assignment
    * @param totalAmount cost amount for this block
    * @param start start DatePtr of the timephased cost block
    * @return timephased cost
    */
	TimephasedCostPtr splitCostStart(ProjectCalendarPtr calendar, double totalAmount, DatePtr start);

   /**
    * Used for Cost type Resource2s. 
    * 
    * Generates a TimphasedCost block for the total amount on the finish Date. This is useful
    * for Cost resources that have an AccrueAt value of End. 
    * 
    * @param calendar calendar used by this assignment
    * @param totalAmount cost amount for this block
    * @param finish finish DatePtr of the timephased cost block
    * @return timephased cost
    */
	TimephasedCostPtr splitCostEnd(ProjectCalendarPtr calendar, double totalAmount, DatePtr finish);

   /**
    * Used for Cost type Resource2s. 
    * 
    * Generates up to two TimephasedCost blocks for a cost amount. The first block will contain
    * all the days using the standardAmountPerDay, and a second block will contain any
    * final amount that is not enough for a complete day. This is useful for Cost resources
    * who have an AccrueAt value of Prorated.
    * 
    * @param calendar calendar used by this assignment
    * @param totalAmount cost amount to be prorated
    * @param standardAmountPerDay cost amount for a normal working day
    * @param start DatePtr of the first timephased cost block
    * @return timephased cost
    */
	TimephasedCostListPtr splitCostProrated(ProjectCalendarPtr calendar,
		double totalAmount, double standardAmountPerDay, DatePtr start);

   /**
    * Splits timephased work segments in line with cost rates. Note that this is
    * an approximation - where a rate changes during a working day, the second 
    * rate is used for the whole day.
    * 
    * @param table cost rate table
    * @param calendar calendar used by this assignment
    * @param work timephased work segment
    * @param rateIndex rate applicable at the start of the timephased work segment
    * @return list of segments which replace the one supplied by the caller
    */
	TimephasedWorkListPtr splitWork(CostRateTablePtr table,
		ProjectCalendarPtr calendar, TimephasedWorkPtr work, int rateIndex);

   /**
    * Used to determine if multiple cost rates apply to this assignment.
    * 
    * @return true if multiple cost rates apply to this assignment
    */
	bool hasMultipleCostRates();

   /**
    * Retrieves the cost rate table entry active on a given Date.
    * 
    * @param DatePtr target Date
    * @return cost rate table entry
    */
	CostRateTableEntryPtr getCostRateTableEntry(DatePtr Date);

   /**
    * Retrieves the index of a cost rate table entry active on a given Date.
    * 
    * @param DatePtr target Date
    * @return cost rate table entry index
    */
	int getCostRateTableEntryIndex(DatePtr Date);

public:
   /**
    * Retrieve a flag indicating if this resource assignment has timephased 
    * data associated with it.
    * 
    * @return true if this resource assignment has timephased data
    */
	bool getHasTimephasedData();

   /**
    * Set timephased baseline work. Note that index 0 represents "Baseline",
    * index 1 represents "Baseline1" and so on.
    * 
    * @param index baseline index
    * @param data timephased data
    */
	void setTimephasedBaselineWork(int index, TimephasedWorkContainerPtr data);

   /**
    * Set timephased baseline cost. Note that index 0 represents "Baseline",
    * index 1 represents "Baseline1" and so on.
    * 
    * @param index baseline index
    * @param data timephased data
    */
	void setTimephasedBaselineCost(int index, TimephasedCostContainerPtr data);

   /** 
    * Retrieve timephased baseline work. Note that index 0 represents "Baseline",
    * index 1 represents "Baseline1" and so on.
    * 
    * @param index baseline index
    * @return timephased work, or NULL if no baseline is present
    */
	TimephasedWorkListPtr getTimephasedBaselineWork(int index);

   /** 
    * Retrieve timephased baseline cost. Note that index 0 represents "Baseline",
    * index 1 represents "Baseline1" and so on.
    * 
    * @param index baseline index
    * @return timephased work, or NULL if no baseline is present
    */
	TimephasedCostListPtr getTimephasedBaselineCost(int index);

   /**
    * Retrieves the calendar used for this resource assignment.
    * 
    * @return ProjectCalendar* instance
    */
	ProjectCalendarPtr getCalendar();

   /**
    * Retrieve the variable rate time units, NULL if fixed rate.
    * 
    * @return variable rate time units
    */
	TimeUnit getVariableRateUnits();

   /**
    * Set the variable rate time units, NULL if fixed rate.
    * 
    * @param variableRateUnits variable rate units
    */
	void setVariableRateUnits(TimeUnit variableRateUnits);

   /**
    * Set the parent task unique ID.
    * 
    * @param id task unique ID
    */
	void setTaskUniqueID(int id);

   /**
    * Retrieve the parent task unique ID.
    * 
    * @return task unique ID
    */
	int getTaskUniqueID();

   /**
    * Retrieves the budget cost.
    * 
    * @return budget cost
    */
	NumberPtr getBudgetCost();

   /**
    * Sets the budget cost.
    * 
    * @param cost budget cost
    */
	void setBudgetCost(NumberPtr cost);

   /**
    * Retrieves the budget work value.
    * 
    * @return budget work
    */
	DurationPtr getBudgetWork();

   /**
    * Sets the budget work value.
    * 
    * @param work budget work
    */
	void setBudgetWork(DurationPtr work);

   /**
    * Retrieves the baseline budget cost.
    * 
    * @return baseline budget cost
    */
	NumberPtr getBaselineBudgetCost();

   /**
    * Sets the baseline budget cost.
    * 
    * @param cost baseline budget cost
    */
	void setBaselineBudgetCost(NumberPtr cost);

   /**
    * Retrieves the baseline budget work value.
    * 
    * @return baseline budget work
    */
	DurationPtr getBaselineBudgetWork();

   /**
    * Sets the baseline budget work value.
    * 
    * @param work baseline budget work
    */
	void setBaselineBudgetWork(DurationPtr work);

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
	void setBaselineWork(int baselineNumber, DurationPtr value);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	DurationPtr getBaselineWork(int baselineNumber);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	NumberPtr getBaselineCost(int baselineNumber);

   /**
    * Set a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @param value baseline value
    */
	void setBaselineStart(int baselineNumber, DatePtr value);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	DatePtr getBaselineStart(int baselineNumber);

   /**
    * Set a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @param value baseline value
    */
	void setBaselineFinish(int baselineNumber, DatePtr value);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	DatePtr getBaselineFinish(int baselineNumber);

   /**
    * Set a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @param value baseline value
    */
	void setBaselineBudgetCost(int baselineNumber, NumberPtr value);

   /**
    * Set a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @param value baseline value
    */
	void setBaselineBudgetWork(int baselineNumber, DurationPtr value);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	DurationPtr getBaselineBudgetWork(int baselineNumber);

   /**
    * Retrieve a baseline value.
    * 
    * @param baselineNumber baseline index (1-10)
    * @return baseline value
    */
	NumberPtr getBaselineBudgetCost(int baselineNumber);

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
    * Set an enterprise cost value.
    * 
    * @param index cost index (1-30)
    * @param value cost value
    */
	void setEnterpriseCost(int index, NumberPtr value);

   /**
    * Retrieve an enterprise cost value.
    * 
    * @param index cost index (1-30)
    * @return cost value
    */
	NumberPtr getEnterpriseCost(int index);

   /**
    * Set an enterprise DatePtr value.
    * 
    * @param index DatePtr index (1-30)
    * @param value DatePtr value
    */
	void setEnterpriseDate(int index, DatePtr value);

   /**
    * Retrieve an enterprise DatePtr value.
    * 
    * @param index DatePtr index (1-30)
    * @return DatePtr value
    */
	DatePtr getEnterpriseDate(int index);

   /**
    * Set an enterprise duration value.
    * 
    * @param index duration index (1-30)
    * @param value duration value
    */
	void setEnterpriseDuration(int index, DurationPtr value);

   /**
    * Retrieve an enterprise duration value.
    * 
    * @param index duration index (1-30)
    * @return duration value
    */
	DurationPtr getEnterpriseDuration(int index);

   /**
    * Set an enterprise flag value.
    * 
    * @param index flag index (1-20)
    * @param value flag value
    */
	void setEnterpriseFlag(int index, bool value);

   /**
    * Retrieve an enterprise flag value.
    * 
    * @param index flag index (1-20)
    * @return flag value
    */
	bool getEnterpriseFlag(int index);

   /**
    * Set an enterprise number value.
    * 
    * @param index number index (1-40)
    * @param value number value
    */
	void setEnterpriseNumber(int index, NumberPtr value);

   /**
    * Retrieve an enterprise number value.
    * 
    * @param index number index (1-40)
    * @return number value
    */
	NumberPtr getEnterpriseNumber(int index);

   /**
    * Set an enterprise text value.
    * 
    * @param index text index (1-40)
    * @param value text value
    */
	void setEnterpriseText(int index, CString value);

   /**
    * Retrieve an enterprise text value.
    * 
    * @param index text index (1-40)
    * @return text value
    */
	CString getEnterpriseText(int index);

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
    * Returns the regular work of this resource assignment.
    *
    * @return work
    */
	DurationPtr getRegularWork();

   /**
    * Sets the regular work for this resource assignment.
    *
    * @param dur work
    */
	void setRegularWork(DurationPtr dur);

   /**
    * Returns the actual overtime work of this resource assignment.
    *
    * @return work
    */
	DurationPtr getActualOvertimeWork();

   /**
    * Sets the actual overtime work for this resource assignment.
    *
    * @param dur work
    */
	void setActualOvertimeWork(DurationPtr dur);

   /**
    * Returns the remaining overtime work of this resource assignment.
    *
    * @return work
    */
	DurationPtr getRemainingOvertimeWork();

   /**
    * Sets the remaining overtime work for this resource assignment.
    *
    * @param dur work
    */
	void setRemainingOvertimeWork(DurationPtr dur);

   /**
    * Returns the overtime cost of this resource assignment.
    *
    * @return cost
    */
	NumberPtr getOvertimeCost();

   /**
    * Sets the overtime cost for this resource assignment.
    *
    * @param cost cost
    */
	void setOvertimeCost(NumberPtr cost);

   /**
    * Returns the remaining cost of this resource assignment.
    *
    * @return cost
    */
	NumberPtr getRemainingCost();

   /**
    * Sets the remaining cost for this resource assignment.
    *
    * @param cost cost
    */
	void setRemainingCost(NumberPtr cost);

   /**
    * Returns the actual overtime cost of this resource assignment.
    *
    * @return cost
    */
	NumberPtr getActualOvertimeCost();

   /**
    * Sets the actual overtime cost for this resource assignment.
    *
    * @param cost cost
    */
	void setActualOvertimeCost(NumberPtr cost);

   /**
    * Returns the remaining overtime cost of this resource assignment.
    *
    * @return cost
    */
	NumberPtr getRemainingOvertimeCost();

   /**
    * Sets the remaining overtime cost for this resource assignment.
    *
    * @param cost cost
    */
	void setRemainingOvertimeCost(NumberPtr cost);

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
    * @param val the amount to set
    */
	void setBCWS(NumberPtr val);

   /**
    * The BCWS (budgeted cost of work scheduled) field contains the cumulative
    * timephased baseline costs up to the status DatePtr or today's Date.
    *
    * @return currency amount as float
    */
	NumberPtr getBCWS();

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
    * The SV (earned value schedule variance) field shows the difference
    * in cost terms between the current progress and the baseline plan
    * of the task up to the status DatePtr or today's Date. You can use SV
    * to check costs to determine whether tasks are on schedule.
    * @param val - currency amount
    */
	void setSV(NumberPtr val);

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
    * The CV (earned value cost variance) field shows the difference
    * between how much it should have cost to achieve the current level of
    * completion on the task, and how much it has actually cost to achieve the
    * current level of completion up to the status DatePtr or today's Date.
    *
    * @param val value to set
    */
	void setCV(NumberPtr val);

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
    * The Cost Variance field shows the difference between the
    * baseline cost and total cost for a task. The total cost is the
    * current estimate of costs based on actual costs and remaining costs.
    * This is also referred to as variance at completion (VAC).
    *
    * @param val amount
    */
	void setCostVariance(NumberPtr val);

   /**
    * The Cost Variance field shows the difference between the baseline cost
    * and total cost for a task. The total cost is the current estimate of costs
    * based on actual costs and remaining costs. This is also referred to as
    * variance at completion (VAC).
    *
    * @return amount
    */
	NumberPtr getCostVariance();

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
    * The % Work Complete field contains the current status of a task,
    * expressed as the percentage of the task's work that has been completed.
    * You can enter percent work complete, or you can have Microsoft Project
    * calculate it for you based on actual work on the task.
    *
    * @return percentage as float
    */
	NumberPtr getPercentageWorkComplete();

   /**
    * This method is used to add notes to the current task.
    *
    * @param notes notes to be added
    */
	void setNotes(CString notes);

   /**
    * The Notes field contains notes that you can enter about a task.
    * You can use task notes to help maintain a history for a task.
    *
    * @return notes
    */
	CString getNotes();

   /**
    * The Confirmed field indicates whether all resources assigned to a task have
    * accepted or rejected the task assignment in response to a TeamAssign message
    * regarding their assignments.
    *
    * @param val bool value
    */
	void setConfirmed(bool val);

   /**
    * The Confirmed field indicates whether all resources assigned to a task
    * have accepted or rejected the task assignment in response to a TeamAssign
    * message regarding their assignments.
    *
    * @return bool
    */
	bool getConfirmed();

   /**
    * The UpDate Needed field indicates whether a TeamUpDate message should
    * be sent to the assigned resources because of changes to the start Date,
    * finish Date, or resource reassignments of the task.
    *
    * @param val - bool
    */
	void setUpDateNeeded(bool val);

   /**
    * The UpDate Needed field indicates whether a TeamUpDate message
    * should be sent to the assigned resources because of changes to the
    * start Date, finish Date, or resource reassignments of the task.
    *
    * @return true if needed.
    */
	bool getUpDateNeeded();

   /**
    * The Linked Fields field indicates whether there are OLE links to the task,
    * either from elsewhere in the active project, another Microsoft Project
    * file, or from another program.
    *
    * @param flag bool value
    */
	void setLinkedFields(bool flag);

   /**
    * The Linked Fields field indicates whether there are OLE links to the task,
    * either from elsewhere in the active project, another Microsoft Project file,
    * or from another program.
    *
    * @return bool
    */
	bool getLinkedFields();

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
    * The Work Variance field contains the difference between a task's baseline
    * work and the currently scheduled work.
    *
    * @param val - duration
    */
	void setWorkVariance(DurationPtr val);

   /**
    * The Work Variance field contains the difference between a task's
    * baseline work and the currently scheduled work.
    *
    * @return Duration representing duration.
    */
	DurationPtr getWorkVariance();

   /**
    * The Start Variance field contains the amount of time that represents the
    * difference between a task's baseline start DatePtr and its currently
    * scheduled start Date.
    *
    * @param val - duration
    */
	void setStartVariance(DurationPtr val);

   /**
    * Calculate the start variance.
    * 
    * @return start variance
    */
	DurationPtr getStartVariance();

   /**
    * The Finish Variance field contains the amount of time that represents the
    * difference between a task's baseline finish DatePtr and its forecast
    * or actual finish Date.
    *
    * @param duration duration value
    */
	void setFinishVariance(DurationPtr duration);

   /**
    * Calculate the finish variance.
    *
    * @return finish variance
    */
	DurationPtr getFinishVariance();

   /**
    * The Created field contains the DatePtr and time when a task was added
    * to the project.
    *
    * @return Date
    */
	DatePtr getCreateDate();

   /**
    * The Created field contains the DatePtr and time when a task was
    * added to the project.
    *
    * @param val Date
    */
	void setCreateDate(DatePtr val);

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
    * Sets a flag to indicate if a response has been received from a resource
    * assigned to a task.
    *
    * @param val bool value
    */
	void setResponsePending(bool val);

   /**
    * Retrieves a flag to indicate if a response has been received from a resource
    * assigned to a task.
    *
    * @return bool value
    */
	bool getResponsePending();

   /**
    * Sets a flag to indicate if a response has been received from a resource
    * assigned to a task.
    *
    * @param val bool value
    */
	void setTeamStatusPending(bool val);

   /**
    * Retrieves a flag to indicate if a response has been received from a resource
    * assigned to a task.
    *
    * @return bool value
    */
	bool getTeamStatusPending();

   /**
    * Sets VAC for this resource assignment.
    *
    * @param value VAC value
    */
	void setVAC(NumberPtr value);

   /**
    * Returns the VAC for this resource assignment.
    *
    * @return VAC value
    */
	NumberPtr getVAC();

   /**
    * Sets the index of the cost rate table for this assignment.
    *
    * @param index cost rate table index
    */
	void setCostRateTableIndex(int index);

   /**
    * Returns the cost rate table index for this assignment.
    *
    * @return cost rate table index
    */
	int getCostRateTableIndex();

   /**
    * Returns the cost rate table for this assignment.
    *
    * @return cost rate table index
    */
	CostRateTablePtr getCostRateTable();

   /**
    * Retrieves the hyperlink screen tip attribute.
    *
    * @return hyperlink screen tip attribute
    */
	CString getHyperlinkScreenTip();

   /**
    * Sets the hyperlink screen tip attribute.
    *
    * @param text hyperlink screen tip attribute
    */
	void setHyperlinkScreenTip(CString text);

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

   /**
    * {@inheritDoc}
    */
	virtual void set(FieldType* field, ObjectPtr value);
	void set(AssignmentField field, ObjectPtr value);

public:
	/**
	* {@inheritDoc}
	*/
	virtual void addFieldListener(FieldListener* listener);

	/**
	* {@inheritDoc}
	*/
	virtual void removeFieldListener(FieldListener* listener);

	/**
	* {@inheritDoc}
	*/
	virtual ObjectPtr getCachedValue(FieldType* field);
	ObjectPtr getCachedValue(AssignmentField field);

	/**
	* {@inheritDoc}
	*/
	virtual ObjectPtr getCurrentValue(FieldType* field);

	/**
	* Disable events firing when fields are upDated.
	*/
	void disableEvents();

	/**
	* Enable events firing when fields are upDated. This is the default state.
	*/
	void enableEvents();

	/**
	* Default units value: 100%.
	*/
	static const double DEFAULT_UNITS;// = 100;

private:
   /**
    * This method inserts a name value pair into internal storage.
    *
    * @param field task field
    * @param value attribute value
    */
	void set(FieldType* field, bool value);

   /**
    * Handle the change in a field value. Reset any cached calculated
    * values affected by this change, pass on the event to any external
    * listeners.
    * 
    * @param field field changed
    * @param oldValue old field value
    * @param newValue new field value
    */
	void fireFieldChangeEvent(AssignmentField field, ObjectPtr oldValue, ObjectPtr newValue);

	/**
	* Maps a field index to an AssignmentField instance.
	*
	* @param fields array of fields used as the basis for the mapping.
	* @param index required field index
	* @return AssignmnetField instance
	*/
	AssignmentField selectField(const AssignmentField fields[], int index);

private:
   /**
    * Array of field values.
    */
	ObjectPtrArray m_array;// [AssignmentField::AF_FINISH + 1];// = new Object[AssignmentField.MAX_VALUE];

   bool m_eventsEnabled = true;

   DefaultTimephasedWorkContainerPtr m_timephasedWork;
   TimephasedCostListPtr m_timephasedCost;

   TimephasedWorkContainerPtr m_timephasedActualWork;
   TimephasedCostListPtr m_timephasedActualCost;

   TimephasedWorkContainerPtr m_timephasedOvertimeWork;
   TimephasedWorkContainerPtr m_timephasedActualOvertimeWork;

   List<FieldListener*> m_listeners;
   TimephasedWorkContainerPtr m_timephasedBaselineWork[11];// = new TimephasedWorkContainer[11];
   TimephasedCostContainerPtr m_timephasedBaselineCost[11];// = new TimephasedCostContainer[11];

   /**
    * Reference to the parent task of this assignment.
    */
   Task* m_task;

   /**
    *  Child record for Workgroup fields.
    */
   Resource2AssignmentWorkgroupFields* m_workgroup;
};

typedef std::shared_ptr<Resource2Assignment> Resource2AssignmentPtr;
