#include "stdafx.h"
#include "ResourceAssignment.h"
#include "ProjectFile.h"
#include "Task.h"
#include "Duration.h"
#include "TimephasedCost.h"
#include "common\AssignmentFieldLists.h"
#include "common\DateHelper.h"
#include "listener\FieldListener.h"

const double Resource2Assignment::DEFAULT_UNITS = 100;

/**
* Constructor->
*
* @param file The parent file to which this record belongs->
*/
Resource2Assignment::Resource2Assignment(ProjectFile* file)
:ProjectEntity(file), m_task(NULL), m_array(AssignmentField::AF_FINISH + 1)
{
    if (file->getAutoAssignmentUniqueID() == true)
    {
		setUniqueID(NumberPtr(new Number(file->getAssignmentUniqueID())));
    }
}

/**
* Constructor->
*
* @param file The parent file to which this record belongs->
* @param task The task to which this assignment is being made
*/
Resource2Assignment::Resource2Assignment(ProjectFile* file, Task* task)
:ProjectEntity(file), m_array(AssignmentField::AF_FINISH + 1)
{
    m_task = task;
}

/**
* This method allows a resource assignment workgroup fields record
* to be added to the current resource assignment-> A maximum of
* one of these records can be added to a resource assignment record->
*
* @return Resource2AssignmentWorkgroupFields object
* @throws MPXJException if MSP defined limit of 1 is exceeded
*/
Resource2AssignmentWorkgroupFields* Resource2Assignment::addWorkgroupAssignment()// throws MPXJException
{
    if (m_workgroup != NULL)
    {
		throw std::bad_exception();// MPXJException(MPXJException->MAXIMUM_RECORDS);
    }

    m_workgroup = new Resource2AssignmentWorkgroupFields();

    return (m_workgroup);
}

/**
* Retrieve the unique ID of this resource assignment->
* 
* @return resource assignment unique ID
*/
NumberPtr Resource2Assignment::getUniqueID()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_UNIQUE_ID));
}

/**
* Set the unique ID of this resource assignment-> 
* 
* @param uniqueID resource assignment unique ID
*/
void Resource2Assignment::setUniqueID(NumberPtr uniqueID)
{
    set(AssignmentField::AF_UNIQUE_ID, uniqueID);
}

/**
* Returns the units of this resource assignment->
*
* @return units
*/
NumberPtr Resource2Assignment::getUnits()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_ASSIGNMENT_UNITS));
}

/**
* Sets the units for this resource assignment->
*
* @param val units
*/
void Resource2Assignment::setUnits(NumberPtr val)
{
    set(AssignmentField::AF_ASSIGNMENT_UNITS, val);
}

/**
* Returns the work of this resource assignment->
*
* @return work
*/
DurationPtr Resource2Assignment::getWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_WORK));
}

/**
* Sets the work for this resource assignment->
*
* @param dur work
*/
void Resource2Assignment::setWork(DurationPtr dur)
{
    set(AssignmentField::AF_WORK, dur);
}

/**
* Retrieve the baseline start Date->
* 
* @return baseline start Date
*/
DatePtr Resource2Assignment::getBaselineStart()
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(AssignmentField::AF_BASELINE_START));
}

/**
* Set the baseline start Date->
* 
* @param start baseline start Date
*/
void Resource2Assignment::setBaselineStart(DatePtr start)
{
    set(AssignmentField::AF_BASELINE_START, start);
}

/**
* Retrieve the actual start Date->
* 
* @return actual start Date
*/
DatePtr Resource2Assignment::getActualStart()
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(AssignmentField::AF_ACTUAL_START));
}

/**
* Set the actual start Date->
* 
* @param start actual start Date
*/
void Resource2Assignment::setActualStart(DatePtr start)
{
    set(AssignmentField::AF_ACTUAL_START, start);
}

/**
* Retrieve the baseline finish Date->
* 
* @return baseline finish Date
*/
DatePtr Resource2Assignment::getBaselineFinish()
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(AssignmentField::AF_BASELINE_FINISH));
}

/**
* Set the baseline finish Date->
* 
* @param finish baseline finish
*/
void Resource2Assignment::setBaselineFinish(DatePtr finish)
{
    set(AssignmentField::AF_BASELINE_FINISH, finish);
}

/**
* Retrieve the actual finish Date->
* 
* @return actual finish Date
*/
DatePtr Resource2Assignment::getActualFinish()
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(AssignmentField::AF_ACTUAL_FINISH));
}

/**
* Set the actual finish Date->
* 
* @param finish actual finish
*/
void Resource2Assignment::setActualFinish(DatePtr finish)
{
    set(AssignmentField::AF_ACTUAL_FINISH, finish);
}

/**
* Returns the baseline work of this resource assignment->
*
* @return planned work
*/
DurationPtr Resource2Assignment::getBaselineWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_BASELINE_WORK));
}

/**
* Sets the baseline work for this resource assignment->
*
* @param val planned work
*/
void Resource2Assignment::setBaselineWork(DurationPtr val)
{
    set(AssignmentField::AF_BASELINE_WORK, val);
}

/**
* Returns the actual completed work of this resource assignment->
*
* @return completed work
*/
DurationPtr Resource2Assignment::getActualWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_ACTUAL_WORK));
}

/**
* Sets the actual completed work for this resource assignment->
*
* @param val actual completed work
*/
void Resource2Assignment::setActualWork(DurationPtr val)
{
    set(AssignmentField::AF_ACTUAL_WORK, val);
}

/**
* Returns the overtime work done of this resource assignment->
*
* @return overtime work
*/
DurationPtr Resource2Assignment::getOvertimeWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_OVERTIME_WORK));
}

/**
* Sets the overtime work for this resource assignment->
*
* @param overtimeWork overtime work
*/
void Resource2Assignment::setOvertimeWork(DurationPtr overtimeWork)
{
    set(AssignmentField::AF_OVERTIME_WORK, overtimeWork);
}

/**
* Returns the cost  of this resource assignment->
*
* @return cost
*/
NumberPtr Resource2Assignment::getCost()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_COST));
}

/**
* Sets the cost for this resource assignment->
*
* @param cost cost
*/
void Resource2Assignment::setCost(NumberPtr cost)
{
	set(AssignmentField::AF_COST, cost);
}

/**
* Returns the planned cost for this resource assignment->
*
* @return planned cost
*/
NumberPtr Resource2Assignment::getBaselineCost()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_BASELINE_COST));
}

/**
* Sets the planned cost for this resource assignment->
*
* @param val planned cost
*/
void Resource2Assignment::setBaselineCost(NumberPtr val)
{
    set(AssignmentField::AF_BASELINE_COST, val);
}

/**
* Returns the actual cost for this resource assignment->
*
* @return actual cost
*/
NumberPtr Resource2Assignment::getActualCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_ACTUAL_COST));
}

/**
* Sets the actual cost so far incurred for this resource assignment->
*
* @param actualCost actual cost
*/
void Resource2Assignment::setActualCost(NumberPtr actualCost)
{
    set(AssignmentField::AF_ACTUAL_COST, actualCost);
}

/**
* Returns the start of this resource assignment->
*
* @return start Date
*/
DatePtr Resource2Assignment::getStart()
{
    DatePtr result = std::dynamic_pointer_cast<Date>(getCachedValue(AssignmentField::AF_START));
    if (result == NULL)
    {
		result = getTask()->getStart();
    }
    return result;
}

/**
* Sets the start DatePtr for this resource assignment->
*
* @param val start Date
*/
void Resource2Assignment::setStart(DatePtr val)
{
    set(AssignmentField::AF_START, val);
}

/**
* Returns the finish DatePtr for this resource assignment->
*
* @return finish Date
*/
DatePtr Resource2Assignment::getFinish()
{
	ObjectPtr obj = getCachedValue(AssignmentField::AF_FINISH);

	DatePtr result = std::dynamic_pointer_cast<Date>(obj);
    if (!result)
    {
        result = getTask()->getFinish();
    }
    return result;
}

/**
* Sets the finish DatePtr for this resource assignment->
*
* @param val finish Date
*/
void Resource2Assignment::setFinish(DatePtr val)
{
    set(AssignmentField::AF_FINISH, val);
}

/**
* Returns the delay for this resource assignment->
*
* @return delay
*/
DurationPtr Resource2Assignment::getDelay()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_ASSIGNMENT_DELAY));
}

/**
* Sets the delay for this resource assignment->
*
* @param dur delay
*/
void Resource2Assignment::setDelay(DurationPtr dur)
{
    set(AssignmentField::AF_ASSIGNMENT_DELAY, dur);
}

/**
* Returns the resources unique id for this resource assignment->
*
* @return resources unique id
*/
NumberPtr Resource2Assignment::getResource2UniqueID()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_RESOURCE_UNIQUE_ID));
}

/**
* Sets the resources unique id for this resource assignment->
*
* @param val resources unique id
*/
void Resource2Assignment::setResource2UniqueID(int val)
{
	set(AssignmentField::AF_RESOURCE_UNIQUE_ID, VariantPtr (new Variant(val)));
}

/**
* Gets the Resource2 Assignment Workgroup Fields if one exists->
*
* @return workgroup assignment object
*/
Resource2AssignmentWorkgroupFields* Resource2Assignment::getWorkgroupAssignment()
{
    return (m_workgroup);
}

/**
* This method retrieves a reference to the task with which this
* assignment is associated->
*
* @return task
*/
Task* Resource2Assignment::getTask()
{
	if (!m_task)
    {
        m_task = getParentFile()->getTaskByUniqueID(getTaskUniqueID());
    }
    return m_task;
}

/**
* This method retrieves a reference to the resource with which this
* assignment is associated->
*
* @return resource
*/
Resource2Ptr Resource2Assignment::getResource2()
{
	NumberPtr id = getResource2UniqueID();
	if (!id)
		return NULL;
	return (getParentFile()->getResource2ByUniqueID(NumberHelper::getInt(id)));
}

/**
* This method returns the Work Contour type of this Assignment->
*
* @return the Work Contour type
*/
WorkContour Resource2Assignment::getWorkContour()
{
	return (WorkContour) NumberHelper::getInt(getCachedValue(AssignmentField::AF_WORK_CONTOUR));
}

/**
* This method sets the Work Contour type of this Assignment->
*
* @param workContour the Work Contour type
*/
void Resource2Assignment::setWorkContour(WorkContour workContour)
{
	set(AssignmentField::AF_WORK_CONTOUR, VariantPtr(new Variant((int)workContour)));
}

/**
* Removes this resource assignment from the project->
*/
void Resource2Assignment::remove()
{
    getParentFile()->removeResource2Assignment(shared_from_this());
}

/**
* Returns the remaining work for this resource assignment->
*
* @return remaining work
*/
DurationPtr Resource2Assignment::getRemainingWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_REMAINING_WORK));
}

/**
* Sets the remaining work for this resource assignment->
*
* @param remainingWork remaining work
*/
void Resource2Assignment::setRemainingWork(DurationPtr remainingWork)
{
    set(AssignmentField::AF_REMAINING_WORK, remainingWork);
}

/**
* Retrieves the leveling delay for this resource assignment->
* 
* @return leveling delay
*/
DurationPtr Resource2Assignment::getLevelingDelay()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_LEVELING_DELAY));
}

/**
* Sets the leveling delay for this resource assignment->
* 
* @param levelingDelay leveling delay
*/
void Resource2Assignment::setLevelingDelay(DurationPtr levelingDelay)
{
    set(AssignmentField::AF_LEVELING_DELAY, levelingDelay);
}

/**
* Retrieves the timephased breakdown of the completed work for this
* resource assignment-> 
* 
* @return timephased completed work
*/
TimephasedWorkListPtr Resource2Assignment::getTimephasedActualWork()
{
	return m_timephasedActualWork == NULL ? NULL : m_timephasedActualWork->getData();
}

/**
* Sets the timephased breakdown of the completed work for this
* resource assignment->
* 
* @param data timephased data
*/
void Resource2Assignment::setTimephasedActualWork(TimephasedWorkContainerPtr data)
{
    m_timephasedActualWork = data;
}

/**
* Retrieves the timephased breakdown of the planned work for this
* resource assignment-> 
* 
* @return timephased planned work
*/
TimephasedWorkListPtr Resource2Assignment::getTimephasedWork()
{
	return m_timephasedWork == NULL ? NULL : m_timephasedWork->getData();
}

/**
* Sets the timephased breakdown of the planned work for this
* resource assignment->
* 
* @param data timephased data 
*/
void Resource2Assignment::setTimephasedWork(DefaultTimephasedWorkContainerPtr data)
{
    m_timephasedWork = data;
}

/**
* Retrieves the timephased breakdown of the planned overtime work for this
* resource assignment-> 
* 
* @return timephased planned work
*/
TimephasedWorkListPtr Resource2Assignment::getTimephasedOvertimeWork()
{
	if (m_timephasedOvertimeWork == NULL && m_timephasedWork != NULL && getOvertimeWork() != NULL)
    {
        double perDayFactor = getRemainingOvertimeWork()->getDuration() / 
			(getRemainingWork()->getDuration() - getRemainingOvertimeWork()->getDuration());
        double totalFactor = getRemainingOvertimeWork()->getDuration() / getRemainingWork()->getDuration();

        //perDayFactor = Double->isNaN(perDayFactor) ? 0 : perDayFactor;
        //totalFactor = Double->isNaN(totalFactor) ? 0 : totalFactor;

        m_timephasedOvertimeWork.reset (
			new DefaultTimephasedWorkContainer(m_timephasedWork.get(), perDayFactor, totalFactor));
    }
	return m_timephasedOvertimeWork == NULL ? NULL : m_timephasedOvertimeWork->getData();
}

/**
* Sets the timephased breakdown of the actual overtime work
* for this assignment->
* 
* @param data timephased work
*/
void Resource2Assignment::setTimephasedActualOvertimeWork(TimephasedWorkContainerPtr data)
{
    m_timephasedActualOvertimeWork = data;
}

/**
* Retrieves the timephased breakdown of the actual overtime work for this
* resource assignment-> 
* 
* @return timephased planned work
*/
TimephasedWorkListPtr Resource2Assignment::getTimephasedActualOvertimeWork()
{
	return m_timephasedActualOvertimeWork == NULL ? NULL : m_timephasedActualOvertimeWork->getData();
}

/**
* Retrieves the timephased breakdown of cost->
* 
* @return timephased cost
*/
TimephasedCostListPtr Resource2Assignment::getTimephasedCost()
{
    if (m_timephasedCost == NULL)
    {
        Resource2Ptr r = getResource2();
        Resource2Type type = r != NULL ? r->getType() : Resource2Type::RT_WORK;

        //for Work and Material resources, we will calculate in the normal way
        if (type != Resource2Type::RT_COST)
        {
        if (m_timephasedWork != NULL && m_timephasedWork->hasData())
        {
            if (hasMultipleCostRates())
            {
                m_timephasedCost = getTimephasedCostMultipleRates(getTimephasedWork(), getTimephasedOvertimeWork());
            }
            else
            {
                m_timephasedCost = getTimephasedCostSingleRate(getTimephasedWork(), getTimephasedOvertimeWork());
            }
        }
        }
        else
        {
        m_timephasedCost = getTimephasedCostFixedAmount();
        }

    }
    return m_timephasedCost;
}

/**
* Retrieves the timephased breakdown of actual cost->
* 
* @return timephased actual cost
*/
TimephasedCostListPtr Resource2Assignment::getTimephasedActualCost()
{
    if (m_timephasedActualCost == NULL)
    {
        Resource2Ptr r = getResource2();
        Resource2Type type = r != NULL ? r->getType() : Resource2Type::RT_WORK;

        //for Work and Material resources, we will calculate in the normal way
        if (type != Resource2Type::RT_COST)
        {
        if (m_timephasedActualWork != NULL && m_timephasedActualWork->hasData())
        {
            if (hasMultipleCostRates())
            {
                m_timephasedActualCost = getTimephasedCostMultipleRates(getTimephasedActualWork(), getTimephasedActualOvertimeWork());
            }
            else
            {
                m_timephasedActualCost = getTimephasedCostSingleRate(getTimephasedActualWork(), getTimephasedActualOvertimeWork());
            }
        }
        }
        else
        {
        m_timephasedActualCost = getTimephasedActualCostFixedAmount();
        }

    }

    return m_timephasedActualCost;
}

/**
* Generates timephased costs from timephased work where a single cost rate
* applies to the whole assignment->
* 
* @param standardWorkList timephased work
* @param overtimeWorkList timephased work 
* @return timephased cost
*/
TimephasedCostListPtr Resource2Assignment::getTimephasedCostSingleRate(
	TimephasedWorkListPtr standardWorkList, TimephasedWorkListPtr overtimeWorkList)
{
    TimephasedCostListPtr result (new TimephasedCostList());

    //just return an empty list if there is no timephased work passed in
    if (standardWorkList == NULL)
    {
        return result;
    }

    //takes care of the situation where there is no timephased overtime work
	Iterator<TimephasedWorkPtr> overtimeIterator(NULL);// = overtimeWorkList == NULL ? java.util.Collections.<TimephasedWork> emptyList()->iterator() : overtimeWorkList->iterator();
	if (overtimeWorkList)
		overtimeIterator = overtimeWorkList->iterator();

	for (size_t i = 0; standardWorkList && i < standardWorkList->GetCount(); i++)
    {
		TimephasedWorkPtr standardWork = standardWorkList->GetAt(i);

		CostRateTableEntryPtr rate = getCostRateTableEntry(standardWork->getStart());
        double standardRateValue = rate->getStandardRate()->getAmount();
        TimeUnit standardRateUnits = rate->getStandardRate()->getUnits();
        double overtimeRateValue = 0;
        TimeUnit overtimeRateUnits = standardRateUnits;

        if (rate->getOvertimeRate() != NULL)
        {
			overtimeRateValue = rate->getOvertimeRate()->getAmount();
			overtimeRateUnits = rate->getOvertimeRate()->getUnits();
        }

        TimephasedWorkPtr overtimeWork = overtimeIterator.hasNext() ? overtimeIterator.next() : NULL;

        DurationPtr standardWorkPerDay = standardWork->getAmountPerDay();
        if (standardWorkPerDay->getUnits() != standardRateUnits)
        {
			standardWorkPerDay = standardWorkPerDay->convertUnits(standardRateUnits, getParentFile()->getProjectHeader());
        }

        DurationPtr totalStandardWork = standardWork->getTotalAmount();
        if (totalStandardWork->getUnits() != standardRateUnits)
        {
			totalStandardWork = totalStandardWork->convertUnits(standardRateUnits, getParentFile()->getProjectHeader());
        }

        DurationPtr overtimeWorkPerDay;
		DurationPtr totalOvertimeWork;

        if (overtimeWork == NULL || overtimeWork->getTotalAmount()->getDuration() == 0)
        {
			overtimeWorkPerDay = Duration2::getInstance(0, standardWorkPerDay->getUnits());
			totalOvertimeWork = Duration2::getInstance(0, standardWorkPerDay->getUnits());
        }
        else
        {
			overtimeWorkPerDay = overtimeWork->getAmountPerDay();
			if (overtimeWorkPerDay->getUnits() != overtimeRateUnits)
			{
				overtimeWorkPerDay = overtimeWorkPerDay->convertUnits(overtimeRateUnits, getParentFile()->getProjectHeader());
			}

			totalOvertimeWork = overtimeWork->getTotalAmount();
			if (totalOvertimeWork->getUnits() != overtimeRateUnits)
			{
				totalOvertimeWork = totalOvertimeWork->convertUnits(overtimeRateUnits, getParentFile()->getProjectHeader());
			}
        }

        double costPerDay = (standardWorkPerDay->getDuration() * standardRateValue) + (overtimeWorkPerDay->getDuration() * overtimeRateValue);
        double totalCost = (totalStandardWork->getDuration() * standardRateValue) + (totalOvertimeWork->getDuration() * overtimeRateValue);

        //if the overtime work does not span the same number of days as the work,
        //then we have to split this into two TimephasedCost values
        if (overtimeWork == NULL || (overtimeWork->getFinish()->equals(standardWork->getFinish().get())))
        {
			//normal way
			TimephasedCostPtr cost (new TimephasedCost());
			cost->setStart(standardWork->getStart());
			cost->setFinish(standardWork->getFinish());
			cost->setModified(standardWork->getModified());
			cost->setAmountPerDay(VariantPtr (new Variant(costPerDay)));
			cost->setTotalAmount(VariantPtr (new Variant(totalCost)));
			result->Add(cost);
        }
        else
        {
			//prorated way
			result->Append(*splitCostProrated(getCalendar(), totalCost, costPerDay, standardWork->getStart()));
        }

    }

    return result;
}

/**
* Generates timephased costs from timephased work where multiple cost rates
* apply to the assignment->
* 
* @param standardWorkList timephased work
* @param overtimeWorkList timephased work
* @return timephased cost
*/
TimephasedCostListPtr Resource2Assignment::getTimephasedCostMultipleRates(TimephasedWorkListPtr standardWorkList, TimephasedWorkListPtr overtimeWorkList)
{
    TimephasedWorkListPtr standardWorkResult (new TimephasedWorkList());
    TimephasedWorkListPtr overtimeWorkResult (new TimephasedWorkList());
    CostRateTablePtr table = getCostRateTable();
    ProjectCalendarPtr calendar = getCalendar();

    Iterator<TimephasedWorkPtr> iter = overtimeWorkList->iterator();
    //for (TimephasedWork standardWork : standardWorkList)
	for (size_t i = 0; i < standardWorkList->GetCount(); i ++)
    {
		TimephasedWorkPtr standardWork = standardWorkList->GetAt(i);

        TimephasedWorkPtr overtimeWork = iter.hasNext() ? iter.next() : NULL;

        int startIndex = getCostRateTableEntryIndex(standardWork->getStart());
        int finishIndex = getCostRateTableEntryIndex(standardWork->getFinish());

        if (startIndex == finishIndex)
        {
			standardWorkResult->Add(standardWork);
			if (overtimeWork != NULL)
			{
				overtimeWorkResult->Add(overtimeWork);
			}
        }
        else
        {
			standardWorkResult->Append(*splitWork(table, calendar, standardWork, startIndex));
			if (overtimeWork != NULL)
			{
				overtimeWorkResult->Append(*splitWork(table, calendar, overtimeWork, startIndex));
			}
        }
    }

    return getTimephasedCostSingleRate(standardWorkResult, overtimeWorkResult);
}

/**
* Generates timephased costs from the assignment's cost value-> Used for Cost type Resource2s->
* 
* @return timephased cost
*/
TimephasedCostListPtr Resource2Assignment::getTimephasedCostFixedAmount()
{
    TimephasedCostListPtr result (new TimephasedCostList());

	ProjectCalendarPtr cal = getCalendar();

    double remainingCost = getRemainingCost()->doubleValue();

    if (remainingCost > 0)
    {
        AccrueType accrueAt = getResource2()->getAccrueAt();

		if (accrueAt == AccrueType::ACCRUE_START)
        {
			result->Add(splitCostStart(cal, remainingCost, getStart()));
        }
        else if (accrueAt == AccrueType::ACCRUE_END)
        {
			result->Add(splitCostEnd(cal, remainingCost, getFinish()));
        }
        else
        {
            //for prorated, we have to deal with it differently depending on whether or not
            //any actual has been entered, since we want to mimic the other timephased data
            //where planned and actual values do not overlap
            double numWorkingDays = cal->getWork(getStart(), getFinish(), TimeUnit::DAYS)->getDuration();
            double standardAmountPerDay = getCost()->doubleValue() / numWorkingDays;

            if (getActualCost()->intValue() > 0)
            {
                //need to get three possible blocks of data: one for the possible partial amount
                //overlap with timephased actual cost; one with all the standard amount days
                //that happen after the actual cost stops; and one with any remaining
                //partial day cost amount

                int numActualDaysUsed = (int) ceil(getActualCost()->doubleValue() / standardAmountPerDay);
                DatePtr actualWorkFinish = cal->getDate(getStart(), Duration2::getInstance(numActualDaysUsed, TimeUnit::DAYS), false);

                double partialDayActualAmount = (long)getActualCost()->doubleValue() % (long)standardAmountPerDay;

                if (partialDayActualAmount > 0)
                {
                    double dayAmount = standardAmountPerDay < remainingCost ? standardAmountPerDay - partialDayActualAmount : remainingCost;

					result->Add(splitCostEnd(cal, dayAmount, actualWorkFinish));

                    remainingCost -= dayAmount;
                }

                //see if there's anything left to work with
                if (remainingCost > 0)
                {
                    //have to split up the amount into standard prorated amount days and whatever is left
                    result->Append(*splitCostProrated(cal, remainingCost, standardAmountPerDay, cal->getNextWorkStart(actualWorkFinish)));
                }

            }
            else
            {
                //no actual cost to worry about, so just a standard split from the beginning of the assignment
				result->Append(*splitCostProrated(cal, remainingCost, standardAmountPerDay, getStart()));
            }
        }
    }

    return result;
}

/**
* Generates timephased actual costs from the assignment's cost value-> Used for Cost type Resource2s->
* 
* @return timephased cost
*/
TimephasedCostListPtr Resource2Assignment::getTimephasedActualCostFixedAmount()
{
    TimephasedCostListPtr result (new TimephasedCostList());

    double actualCost = getActualCost()->doubleValue();

    if (actualCost > 0)
    {
        AccrueType accrueAt = getResource2()->getAccrueAt();

		if (accrueAt == AccrueType::ACCRUE_START)
        {
			result->Add(splitCostStart(getCalendar(), actualCost, getActualStart()));
        }
        else
		if (accrueAt == AccrueType::ACCRUE_END)
        {
			result->Add(splitCostEnd(getCalendar(), actualCost, getActualFinish()));
        }
        else
        {
            //for prorated, we have to deal with it differently; have to 'fill up' each
            //day with the standard amount before going to the next one
            double numWorkingDays = getCalendar()->getWork(getStart(), getFinish(), TimeUnit::DAYS)->getDuration();
            double standardAmountPerDay = getCost()->doubleValue() / numWorkingDays;

			result->Append(*splitCostProrated(getCalendar(), actualCost, standardAmountPerDay, getActualStart()));
        }
    }

    return result;
}

/**
* Used for Cost type Resource2s-> 
* 
* Generates a TimphasedCost block for the total amount on the start Date-> This is useful
* for Cost resources that have an AccrueAt value of Start-> 
* 
* @param calendar calendar used by this assignment
* @param totalAmount cost amount for this block
* @param start start DatePtr of the timephased cost block
* @return timephased cost
*/
TimephasedCostPtr Resource2Assignment::splitCostStart(ProjectCalendarPtr calendar, double totalAmount, DatePtr start)
{
    TimephasedCostPtr cost ( new TimephasedCost());
    cost->setStart(start);
    cost->setFinish(calendar->getDate(start, Duration2::getInstance(1, TimeUnit::DAYS), false));
	cost->setAmountPerDay(NumberPtr (new Number(totalAmount)));
	cost->setTotalAmount(NumberPtr (new Number(totalAmount)));

    return cost;
}

/**
* Used for Cost type Resource2s-> 
* 
* Generates a TimphasedCost block for the total amount on the finish Date-> This is useful
* for Cost resources that have an AccrueAt value of End-> 
* 
* @param calendar calendar used by this assignment
* @param totalAmount cost amount for this block
* @param finish finish DatePtr of the timephased cost block
* @return timephased cost
*/
TimephasedCostPtr Resource2Assignment::splitCostEnd(ProjectCalendarPtr calendar, double totalAmount, DatePtr finish)
{
    TimephasedCostPtr cost (new TimephasedCost());
    cost->setStart(calendar->getStartDate(finish, Duration2::getInstance(1, TimeUnit::DAYS)));
    cost->setFinish(finish);
	cost->setAmountPerDay(NumberPtr (new Number(totalAmount)));
	cost->setTotalAmount(NumberPtr(new Number(totalAmount)));

    return cost;
}

/**
* Used for Cost type Resource2s-> 
* 
* Generates up to two TimephasedCost blocks for a cost amount-> The first block will contain
* all the days using the standardAmountPerDay, and a second block will contain any
* final amount that is not enough for a complete day-> This is useful for Cost resources
* who have an AccrueAt value of Prorated->
* 
* @param calendar calendar used by this assignment
* @param totalAmount cost amount to be prorated
* @param standardAmountPerDay cost amount for a normal working day
* @param start DatePtr of the first timephased cost block
* @return timephased cost
*/
TimephasedCostListPtr Resource2Assignment::splitCostProrated(ProjectCalendarPtr calendar,
	double totalAmount, double standardAmountPerDay, DatePtr start)
{
    TimephasedCostListPtr result (new TimephasedCostList());

    double numStandardAmountDays = floor(totalAmount / standardAmountPerDay);
    double amountForLastDay = (long)totalAmount % (long)standardAmountPerDay;

    //first block contains all the normal work at the beginning of the assignments life, if any

    if (numStandardAmountDays > 0)
    {
        DatePtr finishStandardBlock = calendar->getDate(start, Duration2::getInstance(numStandardAmountDays, TimeUnit::DAYS), false);

        TimephasedCostPtr standardBlock (new TimephasedCost());
		standardBlock->setAmountPerDay(NumberPtr(new Number(standardAmountPerDay)));
        standardBlock->setStart(start);
        standardBlock->setFinish(finishStandardBlock);
		standardBlock->setTotalAmount(NumberPtr(new Number(numStandardAmountDays * standardAmountPerDay)));

        result->Add(standardBlock);

        start = calendar->getNextWorkStart(finishStandardBlock);
    }

    //next block contains the partial day amount, if any
    if (amountForLastDay > 0)
    {
        TimephasedCostPtr nextBlock (new TimephasedCost());
		nextBlock->setAmountPerDay(NumberPtr(new Number(amountForLastDay)));
		nextBlock->setTotalAmount(NumberPtr(new Number(amountForLastDay)));
        nextBlock->setStart(start);
        nextBlock->setFinish(calendar->getDate(start, Duration2::getInstance(1, TimeUnit::DAYS), false));

		result->Add(nextBlock);
    }

    return result;
}

/**
* Splits timephased work segments in line with cost rates-> Note that this is
* an approximation - where a rate changes during a working day, the second 
* rate is used for the whole day->
* 
* @param table cost rate table
* @param calendar calendar used by this assignment
* @param work timephased work segment
* @param rateIndex rate applicable at the start of the timephased work segment
* @return list of segments which replace the one supplied by the caller
*/
TimephasedWorkListPtr Resource2Assignment::splitWork(CostRateTablePtr table, 
	ProjectCalendarPtr calendar, TimephasedWorkPtr work, int rateIndex)
{
    TimephasedWorkListPtr result (new TimephasedWorkList());
    work->setTotalAmount(Duration2::getInstance(0, work->getAmountPerDay()->getUnits()));

    while (true)
    {
		CostRateTableEntryPtr rate = table->GetAt(rateIndex);
        DatePtr splitDate = rate->getEndDate();
        if (splitDate->getTime() >= work->getFinish()->getTime())
        {
			result->Add(work);
			break;
        }

        DatePtr currentPeriodEnd = calendar->getPreviousWorkFinish(splitDate);

        TimephasedWorkPtr currentPeriod (new TimephasedWork(work));
        currentPeriod->setFinish(currentPeriodEnd);
		result->Add(currentPeriod);

        DatePtr nextPeriodStart = calendar->getNextWorkStart(splitDate);
        work->setStart(nextPeriodStart);

        ++rateIndex;
    }

    return result;
}

/**
* Used to determine if multiple cost rates apply to this assignment->
* 
* @return true if multiple cost rates apply to this assignment
*/
bool Resource2Assignment::hasMultipleCostRates()
{
    bool result = false;
    CostRateTablePtr table = getCostRateTable();
    if (table != NULL)
    {
        //
        // We assume here that if there is just one entry in the cost rate
        // table, this is an open ended rate which covers any work, it won't
        // have specific Dates attached to it->
        //
        if (table->GetCount() > 1)
        {
			//
			// If we have multiple rates in the table, see if the same rate
			// is in force at the start and the end of the aaaignment->
			//
			CostRateTableEntryPtr startEntry = table->getEntryByDate(getStart());
			CostRateTableEntryPtr finishEntry = table->getEntryByDate(getFinish());
			result = (startEntry != finishEntry);
        }
    }
    return result;
}

/**
* Retrieves the cost rate table entry active on a given Date->
* 
* @param DatePtr target Date
* @return cost rate table entry
*/
CostRateTableEntryPtr Resource2Assignment::getCostRateTableEntry(DatePtr Date)
{
	CostRateTableEntryPtr result;

    CostRateTablePtr table = getCostRateTable();
    if (table == NULL)
    {
        Resource2Ptr resource = getResource2();
        result.reset (new CostRateTableEntry(resource->getStandardRate(), TimeUnit::HOURS, 
			resource->getOvertimeRate(), TimeUnit::HOURS, resource->getCostPerUse(), NULL));
    }
    else
    {
        if (table->GetCount() == 1)
        {
			result = table->GetAt(0);
        }
        else
        {
			result = table->getEntryByDate(Date);
        }
    }

    return result;
}

/**
* Retrieves the index of a cost rate table entry active on a given Date->
* 
* @param DatePtr target Date
* @return cost rate table entry index
*/
int Resource2Assignment::getCostRateTableEntryIndex(DatePtr Date)
{
    int result = -1;

    CostRateTablePtr table = getCostRateTable();
    if (table != NULL)
    {
		if (table->GetCount() == 1)
        {
			result = 0;
        }
        else
        {
			result = table->getIndexByDate(Date);
        }
    }

    return result;
}

/**
* Retrieve a flag indicating if this resource assignment has timephased 
* data associated with it->
* 
* @return true if this resource assignment has timephased data
*/
bool Resource2Assignment::getHasTimephasedData()
{
    return (m_timephasedWork != NULL && m_timephasedWork->hasData()) || (m_timephasedActualWork != NULL && m_timephasedActualWork->hasData());
}

/**
* Set timephased baseline work-> Note that index 0 represents "Baseline",
* index 1 represents "Baseline1" and so on->
* 
* @param index baseline index
* @param data timephased data
*/
void Resource2Assignment::setTimephasedBaselineWork(int index, TimephasedWorkContainerPtr data)
{
    m_timephasedBaselineWork[index] = data;
}

/**
* Set timephased baseline cost-> Note that index 0 represents "Baseline",
* index 1 represents "Baseline1" and so on->
* 
* @param index baseline index
* @param data timephased data
*/
void Resource2Assignment::setTimephasedBaselineCost(int index, TimephasedCostContainerPtr data)
{
    m_timephasedBaselineCost[index] = data;
}

/** 
* Retrieve timephased baseline work-> Note that index 0 represents "Baseline",
* index 1 represents "Baseline1" and so on->
* 
* @param index baseline index
* @return timephased work, or NULL if no baseline is present
*/
TimephasedWorkListPtr Resource2Assignment::getTimephasedBaselineWork(int index)
{
    return m_timephasedBaselineWork[index] == NULL ? NULL : m_timephasedBaselineWork[index]->getData();
}

/** 
* Retrieve timephased baseline cost-> Note that index 0 represents "Baseline",
* index 1 represents "Baseline1" and so on->
* 
* @param index baseline index
* @return timephased work, or NULL if no baseline is present
*/
TimephasedCostListPtr Resource2Assignment::getTimephasedBaselineCost(int index)
{
    return m_timephasedBaselineCost[index] == NULL ? NULL : m_timephasedBaselineCost[index]->getData();
}

/**
* Retrieves the calendar used for this resource assignment->
* 
* @return ProjectCalendar* instance
*/
ProjectCalendarPtr Resource2Assignment::getCalendar()
{
	ProjectCalendarPtr calendar;
    Resource2Ptr resource = getResource2();
    if (resource != NULL)
    {
        calendar = resource->getResource2Calendar();
    }

    Task* task = getTask();
    if (calendar == NULL || task->getIgnoreResource2Calendar())
    {
        calendar = task->getCalendar();
    }

    if (calendar == NULL)
    {
        calendar = getParentFile()->getCalendar();
    }

    return calendar;
}

/**
* Retrieve the variable rate time units, NULL if fixed rate->
* 
* @return variable rate time units
*/
TimeUnit Resource2Assignment::getVariableRateUnits()
{
	return (TimeUnit) NumberHelper::getInt(getCachedValue(AssignmentField::AF_VARIABLE_RATE_UNITS));
}

/**
* Set the variable rate time units, NULL if fixed rate->
* 
* @param variableRateUnits variable rate units
*/
void Resource2Assignment::setVariableRateUnits(TimeUnit variableRateUnits)
{
	set(AssignmentField::AF_VARIABLE_RATE_UNITS, VariantPtr (new Variant((int)variableRateUnits)));
}

/**
* Set the parent task unique ID->
* 
* @param id task unique ID
*/
void Resource2Assignment::setTaskUniqueID(int id)
{
	set(AssignmentField::AF_TASK_UNIQUE_ID, VariantPtr(new Variant(id)));
}

/**
* Retrieve the parent task unique ID->
* 
* @return task unique ID
*/
int Resource2Assignment::getTaskUniqueID()
{
	return NumberHelper::getInt(getCachedValue(AssignmentField::AF_TASK_UNIQUE_ID));
}

/**
* Retrieves the budget cost->
* 
* @return budget cost
*/
NumberPtr Resource2Assignment::getBudgetCost()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_BUDGET_COST));
}

/**
* Sets the budget cost->
* 
* @param cost budget cost
*/
void Resource2Assignment::setBudgetCost(NumberPtr cost)
{
    set(AssignmentField::AF_BUDGET_COST, cost);
}

/**
* Retrieves the budget work value->
* 
* @return budget work
*/
DurationPtr Resource2Assignment::getBudgetWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_BUDGET_WORK));
}

/**
* Sets the budget work value->
* 
* @param work budget work
*/
void Resource2Assignment::setBudgetWork(DurationPtr work)
{
    set(AssignmentField::AF_BUDGET_WORK, work);
}

/**
* Retrieves the baseline budget cost->
* 
* @return baseline budget cost
*/
NumberPtr Resource2Assignment::getBaselineBudgetCost()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_BASELINE_BUDGET_COST));
}

/**
* Sets the baseline budget cost->
* 
* @param cost baseline budget cost
*/
void Resource2Assignment::setBaselineBudgetCost(NumberPtr cost)
{
    set(AssignmentField::AF_BASELINE_BUDGET_COST, cost);
}

/**
* Retrieves the baseline budget work value->
* 
* @return baseline budget work
*/
DurationPtr Resource2Assignment::getBaselineBudgetWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_BASELINE_BUDGET_WORK));
}

/**
* Sets the baseline budget work value->
* 
* @param work baseline budget work
*/
void Resource2Assignment::setBaselineBudgetWork(DurationPtr work)
{
    set(AssignmentField::AF_BASELINE_BUDGET_WORK, work);
}

/**
* Set a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2Assignment::setBaselineCost(int baselineNumber, NumberPtr value)
{
    set(selectField(AssignmentFieldLists::BASELINE_COSTS, baselineNumber), value);
}

/**
* Set a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2Assignment::setBaselineWork(int baselineNumber, DurationPtr value)
{
    set(selectField(AssignmentFieldLists::BASELINE_WORKS, baselineNumber), value);
}

/**
* Retrieve a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
DurationPtr Resource2Assignment::getBaselineWork(int baselineNumber)
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(selectField(AssignmentFieldLists::BASELINE_WORKS, baselineNumber)));
}

/**
* Retrieve a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
NumberPtr Resource2Assignment::getBaselineCost(int baselineNumber)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(
		selectField(AssignmentFieldLists::BASELINE_COSTS, baselineNumber)));
}

/**
* Set a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2Assignment::setBaselineStart(int baselineNumber, DatePtr value)
{
    set(selectField(AssignmentFieldLists::BASELINE_STARTS, baselineNumber), value);
}

/**
* Retrieve a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
DatePtr Resource2Assignment::getBaselineStart(int baselineNumber)
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(
		selectField(AssignmentFieldLists::BASELINE_STARTS, baselineNumber)));
}

/**
* Set a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2Assignment::setBaselineFinish(int baselineNumber, DatePtr value)
{
    set(selectField(AssignmentFieldLists::BASELINE_FINISHES, baselineNumber), value);
}

/**
* Retrieve a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
DatePtr Resource2Assignment::getBaselineFinish(int baselineNumber)
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(
		selectField(AssignmentFieldLists::BASELINE_FINISHES, baselineNumber)));
}

/**
* Set a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2Assignment::setBaselineBudgetCost(int baselineNumber, NumberPtr value)
{
    set(selectField(AssignmentFieldLists::BASELINE_BUDGET_COSTS, baselineNumber), value);
}

/**
* Set a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @param value baseline value
*/
void Resource2Assignment::setBaselineBudgetWork(int baselineNumber, DurationPtr value)
{
    set(selectField(AssignmentFieldLists::BASELINE_BUDGET_WORKS, baselineNumber), value);
}

/**
* Retrieve a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
DurationPtr Resource2Assignment::getBaselineBudgetWork(int baselineNumber)
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(
		selectField(AssignmentFieldLists::BASELINE_BUDGET_WORKS, baselineNumber)));
}

/**
* Retrieve a baseline value->
* 
* @param baselineNumber baseline index (1-10)
* @return baseline value
*/
NumberPtr Resource2Assignment::getBaselineBudgetCost(int baselineNumber)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(
		selectField(AssignmentFieldLists::BASELINE_BUDGET_COSTS, baselineNumber)));
}

/**
* Set a text value->
* 
* @param index text index (1-30)
* @param value text value
*/
void Resource2Assignment::setText(int index, CString value)
{
	set(selectField(AssignmentFieldLists::CUSTOM_TEXT, index), VariantPtr (new Variant(value)));
}

/**
* Retrieve a text value->
* 
* @param index text index (1-30)
* @return text value
*/
CString Resource2Assignment::getText(int index)
{
    return NumberHelper::getString (getCachedValue(selectField(AssignmentFieldLists::CUSTOM_TEXT, index)));
}

/**
* Set a start value->
* 
* @param index start index (1-10)
* @param value start value
*/
void Resource2Assignment::setStart(int index, DatePtr value)
{
    set(selectField(AssignmentFieldLists::CUSTOM_START, index), value);
}

/**
* Retrieve a start value->
* 
* @param index start index (1-10)
* @return start value
*/
DatePtr Resource2Assignment::getStart(int index)
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(selectField(AssignmentFieldLists::CUSTOM_START, index)));
}

/**
* Set a finish value->
* 
* @param index finish index (1-10)
* @param value finish value
*/
void Resource2Assignment::setFinish(int index, DatePtr value)
{
    set(selectField(AssignmentFieldLists::CUSTOM_FINISH, index), value);
}

/**
* Retrieve a finish value->
* 
* @param index finish index (1-10)
* @return finish value
*/
DatePtr Resource2Assignment::getFinish(int index)
{
    return std::dynamic_pointer_cast<Date>(getCachedValue(selectField(AssignmentFieldLists::CUSTOM_FINISH, index)));
}

/**
* Set a DatePtr value->
* 
* @param index DatePtr index (1-10)
* @param value DatePtr value
*/
void Resource2Assignment::setDate(int index, DatePtr value)
{
    set(selectField(AssignmentFieldLists::CUSTOM_DATE, index), value);
}

/**
* Retrieve a DatePtr value->
* 
* @param index DatePtr index (1-10)
* @return DatePtr value
*/
DatePtr Resource2Assignment::getDate(int index)
{
    return std::dynamic_pointer_cast<Date>(getCachedValue(selectField(AssignmentFieldLists::CUSTOM_DATE, index)));
}

/**
* Set a number value->
* 
* @param index number index (1-20)
* @param value number value
*/
void Resource2Assignment::setNumber(int index, NumberPtr value)
{
    set(selectField(AssignmentFieldLists::CUSTOM_NUMBER, index), value);
}

/**
* Retrieve a number value->
* 
* @param index number index (1-20)
* @return number value
*/
NumberPtr Resource2Assignment::getNumber(int index)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(AssignmentFieldLists::CUSTOM_NUMBER, index)));
}

/**
* Set a duration value->
* 
* @param index duration index (1-10)
* @param value duration value
*/
void Resource2Assignment::setDuration(int index, DurationPtr value)
{
    set(selectField(AssignmentFieldLists::CUSTOM_DURATION, index), value);
}

/**
* Retrieve a duration value->
* 
* @param index duration index (1-10)
* @return duration value
*/
DurationPtr Resource2Assignment::getDuration(int index)
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(selectField(AssignmentFieldLists::CUSTOM_DURATION, index)));
}

/**
* Set a cost value->
* 
* @param index cost index (1-10)
* @param value cost value
*/
void Resource2Assignment::setCost(int index, NumberPtr value)
{
    set(selectField(AssignmentFieldLists::CUSTOM_COST, index), value);
}

/**
* Retrieve a cost value->
* 
* @param index cost index (1-10)
* @return cost value
*/
NumberPtr Resource2Assignment::getCost(int index)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(AssignmentFieldLists::CUSTOM_COST, index)));
}

/**
* Set a flag value->
* 
* @param index flag index (1-20)
* @param value flag value
*/
void Resource2Assignment::setFlag(int index, bool value)
{
	set(selectField(AssignmentFieldLists::CUSTOM_FLAG, index), VariantPtr (new Variant(value)));
}

/**
* Retrieve a flag value->
* 
* @param index flag index (1-20)
* @return flag value
*/
bool Resource2Assignment::getFlag(int index)
{
    return NumberHelper::getBool(getCachedValue(selectField(AssignmentFieldLists::CUSTOM_FLAG, index)));
}

/**
* Set an enterprise cost value->
* 
* @param index cost index (1-30)
* @param value cost value
*/
void Resource2Assignment::setEnterpriseCost(int index, NumberPtr value)
{
    set(selectField(AssignmentFieldLists::ENTERPRISE_COST, index), value);
}

/**
* Retrieve an enterprise cost value->
* 
* @param index cost index (1-30)
* @return cost value
*/
NumberPtr Resource2Assignment::getEnterpriseCost(int index)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(AssignmentFieldLists::ENTERPRISE_COST, index)));
}

/**
* Set an enterprise DatePtr value->
* 
* @param index DatePtr index (1-30)
* @param value DatePtr value
*/
void Resource2Assignment::setEnterpriseDate(int index, DatePtr value)
{
    set(selectField(AssignmentFieldLists::ENTERPRISE_DATE, index), value);
}

/**
* Retrieve an enterprise DatePtr value->
* 
* @param index DatePtr index (1-30)
* @return DatePtr value
*/
DatePtr Resource2Assignment::getEnterpriseDate(int index)
{
	return std::dynamic_pointer_cast<Date>(getCachedValue(selectField(AssignmentFieldLists::ENTERPRISE_DATE, index)));
}

/**
* Set an enterprise duration value->
* 
* @param index duration index (1-30)
* @param value duration value
*/
void Resource2Assignment::setEnterpriseDuration(int index, DurationPtr value)
{
    set(selectField(AssignmentFieldLists::ENTERPRISE_DURATION, index), value);
}

/**
* Retrieve an enterprise duration value->
* 
* @param index duration index (1-30)
* @return duration value
*/
DurationPtr Resource2Assignment::getEnterpriseDuration(int index)
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(selectField(AssignmentFieldLists::ENTERPRISE_DURATION, index)));
}

/**
* Set an enterprise flag value->
* 
* @param index flag index (1-20)
* @param value flag value
*/
void Resource2Assignment::setEnterpriseFlag(int index, bool value)
{
	set(selectField(AssignmentFieldLists::ENTERPRISE_FLAG, index), VariantPtr (new Variant(value)));
}

/**
* Retrieve an enterprise flag value->
* 
* @param index flag index (1-20)
* @return flag value
*/
bool Resource2Assignment::getEnterpriseFlag(int index)
{
    return NumberHelper::getBool(getCachedValue(selectField(AssignmentFieldLists::ENTERPRISE_FLAG, index)));
}

/**
* Set an enterprise number value->
* 
* @param index number index (1-40)
* @param value number value
*/
void Resource2Assignment::setEnterpriseNumber(int index, NumberPtr value)
{
    set(selectField(AssignmentFieldLists::ENTERPRISE_NUMBER, index), value);
}

/**
* Retrieve an enterprise number value->
* 
* @param index number index (1-40)
* @return number value
*/
NumberPtr Resource2Assignment::getEnterpriseNumber(int index)
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(selectField(AssignmentFieldLists::ENTERPRISE_NUMBER, index)));
}

/**
* Set an enterprise text value->
* 
* @param index text index (1-40)
* @param value text value
*/
void Resource2Assignment::setEnterpriseText(int index, CString value)
{
	set(selectField(AssignmentFieldLists::ENTERPRISE_TEXT, index), VariantPtr (new Variant(value)));
}

/**
* Retrieve an enterprise text value->
* 
* @param index text index (1-40)
* @return text value
*/
CString Resource2Assignment::getEnterpriseText(int index)
{
    return NumberHelper::getString(getCachedValue(selectField(AssignmentFieldLists::ENTERPRISE_TEXT, index)));
}

/**
* Retrieve an enterprise custom field value->
* 
* @param index field index
* @return field value
*/
CString Resource2Assignment::getEnterpriseCustomField(int index)
{
    return NumberHelper::getString(getCachedValue(selectField(AssignmentFieldLists::ENTERPRISE_CUSTOM_FIELD, index)));
}

/**
* Set an enterprise custom field value->
* 
* @param index field index
* @param value field value
*/
void Resource2Assignment::setEnterpriseCustomField(int index, CString value)
{
	set(selectField(AssignmentFieldLists::ENTERPRISE_CUSTOM_FIELD, index), VariantPtr (new Variant(value)));
}

/**
* Returns the regular work of this resource assignment->
*
* @return work
*/
DurationPtr Resource2Assignment::getRegularWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_REGULAR_WORK));
}

/**
* Sets the regular work for this resource assignment->
*
* @param dur work
*/
void Resource2Assignment::setRegularWork(DurationPtr dur)
{
    set(AssignmentField::AF_REGULAR_WORK, dur);
}

/**
* Returns the actual overtime work of this resource assignment->
*
* @return work
*/
DurationPtr Resource2Assignment::getActualOvertimeWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_ACTUAL_OVERTIME_WORK));
}

/**
* Sets the actual overtime work for this resource assignment->
*
* @param dur work
*/
void Resource2Assignment::setActualOvertimeWork(DurationPtr dur)
{
    set(AssignmentField::AF_ACTUAL_OVERTIME_WORK, dur);
}

/**
* Returns the remaining overtime work of this resource assignment->
*
* @return work
*/
DurationPtr Resource2Assignment::getRemainingOvertimeWork()
{
	return std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_REMAINING_OVERTIME_WORK));
}

/**
* Sets the remaining overtime work for this resource assignment->
*
* @param dur work
*/
void Resource2Assignment::setRemainingOvertimeWork(DurationPtr dur)
{
    set(AssignmentField::AF_REMAINING_OVERTIME_WORK, dur);
}

/**
* Returns the overtime cost of this resource assignment->
*
* @return cost
*/
NumberPtr Resource2Assignment::getOvertimeCost()
{
	NumberPtr cost = std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_OVERTIME_COST));
    if (cost == NULL)
    {
        NumberPtr actual = getActualOvertimeCost();
        NumberPtr remaining = getRemainingOvertimeCost();
        if (actual != NULL && remaining != NULL)
        {
			cost = VariantPtr(new Variant(actual->doubleValue() + remaining->doubleValue()));
			set(AssignmentField::AF_OVERTIME_COST, cost);
        }
    }
    return (cost);
}

/**
* Sets the overtime cost for this resource assignment->
*
* @param cost cost
*/
void Resource2Assignment::setOvertimeCost(NumberPtr cost)
{
    set(AssignmentField::AF_OVERTIME_COST, cost);
}

/**
* Returns the remaining cost of this resource assignment->
*
* @return cost
*/
NumberPtr Resource2Assignment::getRemainingCost()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_REMAINING_COST));
}

/**
* Sets the remaining cost for this resource assignment->
*
* @param cost cost
*/
void Resource2Assignment::setRemainingCost(NumberPtr cost)
{
    set(AssignmentField::AF_REMAINING_COST, cost);
}

/**
* Returns the actual overtime cost of this resource assignment->
*
* @return cost
*/
NumberPtr Resource2Assignment::getActualOvertimeCost()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_ACTUAL_OVERTIME_COST));
}

/**
* Sets the actual overtime cost for this resource assignment->
*
* @param cost cost
*/
void Resource2Assignment::setActualOvertimeCost(NumberPtr cost)
{
    set(AssignmentField::AF_ACTUAL_OVERTIME_COST, cost);
}

/**
* Returns the remaining overtime cost of this resource assignment->
*
* @return cost
*/
NumberPtr Resource2Assignment::getRemainingOvertimeCost()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_REMAINING_OVERTIME_COST));
}

/**
* Sets the remaining overtime cost for this resource assignment->
*
* @param cost cost
*/
void Resource2Assignment::setRemainingOvertimeCost(NumberPtr cost)
{
    set(AssignmentField::AF_REMAINING_OVERTIME_COST, cost);
}

/**
* The BCWP (budgeted cost of work performed) field contains the
* cumulative value
* of the assignment's timephased percent complete multiplied by
* the assignments
* timephased baseline cost-> BCWP is calculated up to the status
* DatePtr or todays
* Date-> This information is also known as earned value->
*
* @param val the amount to be set
*/
void Resource2Assignment::setBCWP(NumberPtr val)
{
    set(AssignmentField::AF_BCWP, val);
}

/**
* The BCWP (budgeted cost of work performed) field contains
* the cumulative value of the assignment's timephased percent complete
* multiplied by the assignment's timephased baseline cost->
* BCWP is calculated up to the status DatePtr or today's Date->
* This information is also known as earned value->
*
* @return currency amount as float
*/
NumberPtr Resource2Assignment::getBCWP()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_BCWP));
}

/**
* The BCWS (budgeted cost of work scheduled) field contains the cumulative
* timephased baseline costs up to the status DatePtr or today's Date->
*
* @param val the amount to set
*/
void Resource2Assignment::setBCWS(NumberPtr val)
{
    set(AssignmentField::AF_BCWS, val);
}

/**
* The BCWS (budgeted cost of work scheduled) field contains the cumulative
* timephased baseline costs up to the status DatePtr or today's Date->
*
* @return currency amount as float
*/
NumberPtr Resource2Assignment::getBCWS()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_BCWS));
}

/**
* Retrieve the ACWP value->
*
* @return ACWP value
*/
NumberPtr Resource2Assignment::getACWP()
{
	return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_ACWP));
}

/**
* Set the ACWP value->
*
* @param acwp ACWP value
*/
void Resource2Assignment::setACWP(NumberPtr acwp)
{
    set(AssignmentField::AF_ACWP, acwp);
}

/**
* The SV (earned value schedule variance) field shows the difference
* in cost terms between the current progress and the baseline plan
* of the task up to the status DatePtr or today's Date-> You can use SV
* to check costs to determine whether tasks are on schedule->
* @param val - currency amount
*/
void Resource2Assignment::setSV(NumberPtr val)
{
    set(AssignmentField::AF_SV, val);
}

/**
* The SV (earned value schedule variance) field shows the difference in
* cost terms between the current progress and the baseline plan of the
* task up to the status DatePtr or today's Date-> You can use SV to
* check costs to determine whether tasks are on schedule->
*
* @return -earned value schedule variance
*/
NumberPtr Resource2Assignment::getSV()
{
	NumberPtr variance = std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_SV));
    if (variance == NULL)
    {
        NumberPtr bcwp = getBCWP();
        NumberPtr bcws = getBCWS();
        if (bcwp != NULL && bcws != NULL)
        {
			variance = VariantPtr(new Variant(bcwp->doubleValue() - bcws->doubleValue()));
			set(AssignmentField::AF_SV, variance);
        }
    }
    return (variance);
}

/**
* The CV (earned value cost variance) field shows the difference
* between how much it should have cost to achieve the current level of
* completion on the task, and how much it has actually cost to achieve the
* current level of completion up to the status DatePtr or today's Date->
*
* @param val value to set
*/
void Resource2Assignment::setCV(NumberPtr val)
{
    set(AssignmentField::AF_CV, val);
}

/**
* The CV (earned value cost variance) field shows the difference between
* how much it should have cost to achieve the current level of completion
* on the task, and how much it has actually cost to achieve the current
* level of completion up to the status DatePtr or today's Date->
* How Calculated   CV is the difference between BCWP
* (budgeted cost of work performed) and ACWP
* (actual cost of work performed)-> Microsoft Project calculates
* the CV as follows: CV = BCWP - ACWP
*
* @return sum of earned value cost variance
*/
NumberPtr Resource2Assignment::getCV()
{
	NumberPtr variance = std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_CV));
    if (variance == NULL)
    {
		variance = VariantPtr(new Variant(
			NumberHelper::getDouble(getBCWP()) - NumberHelper::getDouble(getACWP())));
        set(AssignmentField::AF_CV, variance);
    }
    return (variance);
}

/**
* The Cost Variance field shows the difference between the
* baseline cost and total cost for a task-> The total cost is the
* current estimate of costs based on actual costs and remaining costs->
* This is also referred to as variance at completion (VAC)->
*
* @param val amount
*/
void Resource2Assignment::setCostVariance(NumberPtr val)
{
    set(AssignmentField::AF_COST_VARIANCE, val);
}

/**
* The Cost Variance field shows the difference between the baseline cost
* and total cost for a task-> The total cost is the current estimate of costs
* based on actual costs and remaining costs-> This is also referred to as
* variance at completion (VAC)->
*
* @return amount
*/
NumberPtr Resource2Assignment::getCostVariance()
{
	NumberPtr variance = std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_COST_VARIANCE));
    if (variance == NULL)
    {
        NumberPtr cost = getCost();
        NumberPtr baselineCost = getBaselineCost();
        if (cost != NULL && baselineCost != NULL)
        {
			variance = VariantPtr(new Variant(cost->doubleValue() - baselineCost->doubleValue()));
			set(AssignmentField::AF_COST_VARIANCE, variance);
        }
    }
    return (variance);
}

/**
* The % Work Complete field contains the current status of a task,
* expressed as the
* percentage of the task's work that has been completed-> You can enter
* percent work
* complete, or you can have Microsoft Project calculate it for you
* based on actual
* work on the task->
*
* @param val value to be set
*/
void Resource2Assignment::setPercentageWorkComplete(NumberPtr val)
{
    set(AssignmentField::AF_PERCENT_WORK_COMPLETE, val);
}

/**
* The % Work Complete field contains the current status of a task,
* expressed as the percentage of the task's work that has been completed->
* You can enter percent work complete, or you can have Microsoft Project
* calculate it for you based on actual work on the task->
*
* @return percentage as float
*/
NumberPtr Resource2Assignment::getPercentageWorkComplete()
{
	NumberPtr pct = std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_PERCENT_WORK_COMPLETE));
    if (pct == NULL)
    {
        DurationPtr actualWork = getActualWork();
        DurationPtr work = getWork();
        if (actualWork != NULL && work != NULL && work->getDuration() != 0)
        {
			pct = VariantPtr(new Variant((
				actualWork->getDuration() * 100) / work->convertUnits(actualWork->getUnits(),
							getParentFile()->getProjectHeader())->getDuration()));
			set(AssignmentField::AF_PERCENT_WORK_COMPLETE, pct);
        }
    }
    return pct;
}

/**
* This method is used to add notes to the current task->
*
* @param notes notes to be added
*/
void Resource2Assignment::setNotes(CString notes)
{
	set(AssignmentField::AF_NOTES, VariantPtr (new Variant(notes)));
}

/**
* The Notes field contains notes that you can enter about a task->
* You can use task notes to help maintain a history for a task->
*
* @return notes
*/
CString Resource2Assignment::getNotes()
{
    return NumberHelper::getString(getCachedValue(AssignmentField::AF_NOTES));
}

/**
* The Confirmed field indicates whether all resources assigned to a task have
* accepted or rejected the task assignment in response to a TeamAssign message
* regarding their assignments->
*
* @param val bool value
*/
void Resource2Assignment::setConfirmed(bool val)
{
	set(AssignmentField::AF_CONFIRMED, VariantPtr (new Variant(val)));
}

/**
* The Confirmed field indicates whether all resources assigned to a task
* have accepted or rejected the task assignment in response to a TeamAssign
* message regarding their assignments->
*
* @return bool
*/
bool Resource2Assignment::getConfirmed()
{
    return NumberHelper::getBool(getCachedValue(AssignmentField::AF_CONFIRMED));
}

/**
* The UpDate Needed field indicates whether a TeamUpDate message should
* be sent to the assigned resources because of changes to the start Date,
* finish Date, or resource reassignments of the task->
*
* @param val - bool
*/
void Resource2Assignment::setUpDateNeeded(bool val)
{
	set(AssignmentField::AF_UPDATE_NEEDED, VariantPtr (new Variant(val)));
}

/**
* The UpDate Needed field indicates whether a TeamUpDate message
* should be sent to the assigned resources because of changes to the
* start Date, finish Date, or resource reassignments of the task->
*
* @return true if needed->
*/
bool Resource2Assignment::getUpDateNeeded()
{
	return NumberHelper::getBool(getCachedValue(AssignmentField::AF_UPDATE_NEEDED));
}

/**
* The Linked Fields field indicates whether there are OLE links to the task,
* either from elsewhere in the active project, another Microsoft Project
* file, or from another program->
*
* @param flag bool value
*/
void Resource2Assignment::setLinkedFields(bool flag)
{
	set(AssignmentField::AF_LINKED_FIELDS, VariantPtr (new Variant(flag)));
}

/**
* The Linked Fields field indicates whether there are OLE links to the task,
* either from elsewhere in the active project, another Microsoft Project file,
* or from another program->
*
* @return bool
*/
bool Resource2Assignment::getLinkedFields()
{
	return NumberHelper::getBool(getCachedValue(AssignmentField::AF_LINKED_FIELDS));
}

/**
* Retrieves the task hyperlink attribute->
*
* @return hyperlink attribute
*/
CString Resource2Assignment::getHyperlink()
{
	return NumberHelper::getString(getCachedValue(AssignmentField::AF_HYPERLINK));
}

/**
* Retrieves the task hyperlink address attribute->
*
* @return hyperlink address attribute
*/
CString Resource2Assignment::getHyperlinkAddress()
{
	return NumberHelper::getString(getCachedValue(AssignmentField::AF_HYPERLINK_ADDRESS));
}

/**
* Retrieves the task hyperlink sub-address attribute->
*
* @return hyperlink sub address attribute
*/
CString Resource2Assignment::getHyperlinkSubAddress()
{
	return NumberHelper::getString(getCachedValue(AssignmentField::AF_HYPERLINK_SUBADDRESS));
}

/**
* Sets the task hyperlink attribute->
*
* @param text hyperlink attribute
*/
void Resource2Assignment::setHyperlink(CString text)
{
	set(AssignmentField::AF_HYPERLINK, VariantPtr (new Variant(text)));
}

/**
* Sets the task hyperlink address attribute->
*
* @param text hyperlink address attribute
*/
void Resource2Assignment::setHyperlinkAddress(CString text)
{
	set(AssignmentField::AF_HYPERLINK_ADDRESS, VariantPtr (new Variant(text)));
}

/**
* Sets the task hyperlink sub address attribute->
*
* @param text hyperlink sub address attribute
*/
void Resource2Assignment::setHyperlinkSubAddress(CString text)
{
	set(AssignmentField::AF_HYPERLINK_SUBADDRESS, VariantPtr (new Variant(text)));
}

/**
* The Work Variance field contains the difference between a task's baseline
* work and the currently scheduled work->
*
* @param val - duration
*/
void Resource2Assignment::setWorkVariance(DurationPtr val)
{
    set(AssignmentField::AF_WORK_VARIANCE, val);
}

/**
* The Work Variance field contains the difference between a task's
* baseline work and the currently scheduled work->
*
* @return DurationPtr representing duration->
*/
DurationPtr Resource2Assignment::getWorkVariance()
{
	DurationPtr variance = std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_WORK_VARIANCE));
    if (!variance)
    {
        DurationPtr work = getWork();
        DurationPtr baselineWork = getBaselineWork();
        if (work != NULL && baselineWork != NULL)
        {
			variance = Duration2::getInstance(work->getDuration() -
				baselineWork->convertUnits(work->getUnits(), 
				getParentFile()->getProjectHeader())->getDuration(), work->getUnits());
			set(AssignmentField::AF_WORK_VARIANCE, variance);
        }
    }
    return (variance);
}

/**
* The Start Variance field contains the amount of time that represents the
* difference between a task's baseline start DatePtr and its currently
* scheduled start Date->
*
* @param val - duration
*/
void Resource2Assignment::setStartVariance(DurationPtr val)
{
    set(AssignmentField::AF_START_VARIANCE, val);
}

/**
* Calculate the start variance->
* 
* @return start variance
*/
DurationPtr Resource2Assignment::getStartVariance()
{
	DurationPtr variance = std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_START_VARIANCE));
    if (!variance)
    {
        TimeUnit format = getParentFile()->getProjectHeader()->getDefaultDurationUnits();
        variance = DateHelper::getVariance(getTask(), getBaselineStart(), getStart(), format);
        set(AssignmentField::AF_START_VARIANCE, variance);
    }
    return (variance);
}

/**
* The Finish Variance field contains the amount of time that represents the
* difference between a task's baseline finish DatePtr and its forecast
* or actual finish Date->
*
* @param duration duration value
*/
void Resource2Assignment::setFinishVariance(DurationPtr duration)
{
    set(AssignmentField::AF_FINISH_VARIANCE, duration);
}

/**
* Calculate the finish variance->
*
* @return finish variance
*/
DurationPtr Resource2Assignment::getFinishVariance()
{
	DurationPtr variance = std::dynamic_pointer_cast<Duration2>(getCachedValue(AssignmentField::AF_FINISH_VARIANCE));
    if (!variance)
    {
        TimeUnit format = getParentFile()->getProjectHeader()->getDefaultDurationUnits();
        variance = DateHelper::getVariance(getTask(), getBaselineFinish(), getFinish(), format);
        set(AssignmentField::AF_FINISH_VARIANCE, variance);
    }
    return (variance);
}

/**
* The Created field contains the DatePtr and time when a task was added
* to the project->
*
* @return Date
*/
DatePtr Resource2Assignment::getCreateDate()
{
    return std::dynamic_pointer_cast<Date>(getCachedValue(AssignmentField::AF_CREATED));
}

/**
* The Created field contains the DatePtr and time when a task was
* added to the project->
*
* @param val Date
*/
void Resource2Assignment::setCreateDate(DatePtr val)
{
    set(AssignmentField::AF_CREATED, val);
}

/**
* Retrieve the task GUID2->
* 
* @return task GUID2
*/
LPCGUID Resource2Assignment::getGUID()
{
	return *std::dynamic_pointer_cast<Variant>(getCachedValue(AssignmentField::AF_GUID));
}

/**
* Set the task GUID2->
* 
* @param value task GUID2
*/
void Resource2Assignment::setGUID(LPCGUID value)
{
	set(AssignmentField::AF_GUID, VariantPtr (new Variant(value)));
}

/**
* Sets a flag to indicate if a response has been received from a resource
* assigned to a task->
*
* @param val bool value
*/
void Resource2Assignment::setResponsePending(bool val)
{
	set(AssignmentField::AF_RESPONSE_PENDING, VariantPtr (new Variant(val)));
}

/**
* Retrieves a flag to indicate if a response has been received from a resource
* assigned to a task->
*
* @return bool value
*/
bool Resource2Assignment::getResponsePending()
{
    return NumberHelper::getBool(getCachedValue(AssignmentField::AF_RESPONSE_PENDING));
}

/**
* Sets a flag to indicate if a response has been received from a resource
* assigned to a task->
*
* @param val bool value
*/
void Resource2Assignment::setTeamStatusPending(bool val)
{
	set(AssignmentField::AF_TEAM_STATUS_PENDING, VariantPtr (new Variant(val)));
}

/**
* Retrieves a flag to indicate if a response has been received from a resource
* assigned to a task->
*
* @return bool value
*/
bool Resource2Assignment::getTeamStatusPending()
{
	return NumberHelper::getBool(getCachedValue(AssignmentField::AF_TEAM_STATUS_PENDING));
}

/**
* Sets VAC for this resource assignment->
*
* @param value VAC value
*/
void Resource2Assignment::setVAC(NumberPtr value)
{
    set(AssignmentField::AF_VAC, value);
}

/**
* Returns the VAC for this resource assignment->
*
* @return VAC value
*/
NumberPtr Resource2Assignment::getVAC()
{
    return std::dynamic_pointer_cast<Number>(getCachedValue(AssignmentField::AF_VAC));
}

/**
* Sets the index of the cost rate table for this assignment->
*
* @param index cost rate table index
*/
void Resource2Assignment::setCostRateTableIndex(int index)
{
	set(AssignmentField::AF_COST_RATE_TABLE, VariantPtr (new Variant(index)));
}

/**
* Returns the cost rate table index for this assignment->
*
* @return cost rate table index
*/
int Resource2Assignment::getCostRateTableIndex()
{
	return NumberHelper::getInt(getCachedValue(AssignmentField::AF_COST_RATE_TABLE));
}

/**
* Returns the cost rate table for this assignment->
*
* @return cost rate table index
*/
CostRateTablePtr Resource2Assignment::getCostRateTable()
{
    return getResource2() == NULL ? NULL : getResource2()->getCostRateTable(getCostRateTableIndex());
}

/**
* Retrieves the hyperlink screen tip attribute->
*
* @return hyperlink screen tip attribute
*/
CString Resource2Assignment::getHyperlinkScreenTip()
{
    return NumberHelper::getString(getCachedValue(AssignmentField::AF_HYPERLINK_SCREEN_TIP));
}

/**
* Sets the hyperlink screen tip attribute->
*
* @param text hyperlink screen tip attribute
*/
void Resource2Assignment::setHyperlinkScreenTip(CString text)
{
	set(AssignmentField::AF_HYPERLINK_SCREEN_TIP, VariantPtr (new Variant(text)));
}

/**
* Maps a field index to an AssignmentField instance->
* 
* @param fields array of fields used as the basis for the mapping->
* @param index required field index
* @return AssignmnetField instance
*/
AssignmentField Resource2Assignment::selectField(const AssignmentField fields[], int index)
{
    if (index < 1)// || index > fields->length)
    {
		throw std::bad_exception();// new IllegalArgumentException(index + " is not a valid field index");
    }
    return (fields[index - 1]);
}

///**
//* {@inheritDoc}
//*/
//CString Resource2Assignment::toString()
//{
//    return ("[Resource2 Assignment task=" + getTask()->getName() + " resource=" + (getResource2() == NULL ? "Unassigned" : getResource2()->getName()) + " start=" + getStart() + " finish=" + getFinish() + " duration=" + getWork() + " workContour=" + getWorkContour() + "]");
//}

/**
* {@inheritDoc}
*/
void Resource2Assignment::set(FieldType* field, ObjectPtr value)
{
    if (field != NULL)
    {
        int index = field->getValue();
        if (m_eventsEnabled)
        {
			fireFieldChangeEvent((AssignmentField) field->getValue(), m_array[index], value);
        }
        m_array[index] = value;
    }
}

void Resource2Assignment::set(AssignmentField field, ObjectPtr value)
{
	set(AssignmentFieldClass::getInstance(field), value);
}

/**
* This method inserts a name value pair into internal storage->
*
* @param field task field
* @param value attribute value
*/
void Resource2Assignment::set(FieldType* field, bool value)
{
	set(field, VariantPtr(new Variant(value ? true : false)));
}

/**
* Handle the change in a field value-> Reset any cached calculated
* values affected by this change, pass on the event to any external
* listeners->
* 
* @param field field changed
* @param oldValue old field value
* @param newValue new field value
*/
void Resource2Assignment::fireFieldChangeEvent(AssignmentField field, ObjectPtr oldValue, ObjectPtr newValue)
{
    //
    // Internal event handling
    //
    switch (field)
    {
        case AF_START:
		case AF_BASELINE_START:
        {
			m_array[AssignmentField::AF_START_VARIANCE] = NULL;
			break;
        }

		case AF_FINISH:
		case AF_BASELINE_FINISH:
        {
			m_array[AssignmentField::AF_FINISH_VARIANCE] = NULL;
			break;
        }

		case AF_BCWP:
		case AF_ACWP:
        {
			m_array[AssignmentField::AF_CV] = NULL;
			m_array[AssignmentField::AF_SV] = NULL;
			break;
        }

		case AF_COST:
		case AF_BASELINE_COST:
        {
			m_array[AssignmentField::AF_COST_VARIANCE] = NULL;
			break;
        }

		case AF_WORK:
		case AF_BASELINE_WORK:
        {
			m_array[AssignmentField::AF_WORK_VARIANCE] = NULL;
			break;
        }

		case AF_ACTUAL_OVERTIME_COST:
		case AF_REMAINING_OVERTIME_COST:
        {
			m_array[AssignmentField::AF_OVERTIME_COST] = NULL;
			break;
        }

        default:
        {
			break;
        }
    }

    //
    // External event handling
    //
	for (size_t i = 0; i < m_listeners.GetCount(); i ++)
    {
		FieldListener* listener = m_listeners[i];
		listener->fieldChange(this, AssignmentFieldClass::getInstance (field), oldValue, newValue);
    }
}

/**
* {@inheritDoc}
*/
void Resource2Assignment::addFieldListener(FieldListener* listener)
{
    m_listeners.Add(listener);
}

/**
* {@inheritDoc}
*/
void Resource2Assignment::removeFieldListener(FieldListener* listener)
{
	m_listeners.remove(listener);
}

/**
* {@inheritDoc}
*/
ObjectPtr Resource2Assignment::getCachedValue(FieldType* field)
{
    return (field == NULL ? NULL : m_array[field->getValue()]);
}

ObjectPtr Resource2Assignment::getCachedValue(AssignmentField field)
{
	return getCachedValue(AssignmentFieldClass::getInstance(field));
}

/**
* {@inheritDoc}
*/
ObjectPtr Resource2Assignment::getCurrentValue(FieldType* field)
{
	ObjectPtr result = NULL;

    if (field != NULL)
    {
        int fieldValue = field->getValue();

        result = m_array[fieldValue];
    }

    return (result);
}

/**
* Disable events firing when fields are upDated->
*/
void Resource2Assignment::disableEvents()
{
    m_eventsEnabled = false;
}

/**
* Enable events firing when fields are upDated-> This is the default state->
*/
void Resource2Assignment::enableEvents()
{
    m_eventsEnabled = true;
}