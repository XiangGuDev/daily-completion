#include "stdafx.h"
#include "MPPTimephasedBaselineCostNormaliser.h"
#include "..\java\Calendar.h"
#include "..\common\DateHelper.h"
#include "..\common\NumberHelper.h"

/**
* This method converts the internal representation of timephased 
* resource assignment data used by MS Project into a standardised
* format to make it easy to work with. 
* 
* @param calendar current calendar
* @param list list of assignment data
*/
void MPPTimephasedBaselineCostNormaliser::normalise(ProjectCalendarPtr calendar, TimephasedCostListPtr list)
{
    if (!list->isEmpty())
    {
        //dumpList(list);
        splitDays(calendar, list);
        //dumpList(list);
        mergeSameDay(list);
        //dumpList(list);
        mergeSameCost(list);
        //dumpList(list);
    }
}

/**
* This method breaks down spans of time into individual days.
* 
* @param calendar current project calendar
* @param list list of assignment data
*/
void MPPTimephasedBaselineCostNormaliser::splitDays(ProjectCalendarPtr calendar, TimephasedCostListPtr list)
{
    TimephasedCostListPtr result (new TimephasedCostList());
    bool remainderInserted = false;
    CalendarPtr cal = Calendar::getInstance();

	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedCostPtr assignment = list->GetAt(i);

        if (remainderInserted)
        {
			cal->setTime(assignment->getStart());
			cal->add(Calendar::DAY_OF_YEAR, 1);
			assignment->setStart(cal->getTime());
			remainderInserted = false;
        }

        while (assignment)
        {
			DatePtr startDay = DateHelper::getDayStartDate(assignment->getStart());
			DatePtr finishDay = DateHelper::getDayStartDate(assignment->getFinish());

			// special case - when the finishday time is midnight, it's really the previous day...                 
			if (assignment->getFinish()->getTime() == finishDay->getTime())
			{
				cal->setTime(finishDay);
				cal->add(Calendar::DAY_OF_YEAR, -1);
				finishDay = cal->getTime();
			}

			if (startDay->getTime() == finishDay->getTime())
			{
				result->Add(assignment);
				break;
			}

			TimephasedCostArrayPtr split = splitFirstDay(calendar, assignment);
			if ((*split)[0])
			{
				result->Add((*split)[0]);
			}

			if (assignment->equals(&*(*split)[1]))
			{
				break;
			}

			assignment = (*split)[1];
        }
    }

    list->RemoveAll();
    list->Append(*result);
}

/**
* This method splits the first day off of a time span.
* 
* @param calendar current calendar
* @param assignment timephased assignment span
* @return first day and remainder assignments
*/
TimephasedCostArrayPtr MPPTimephasedBaselineCostNormaliser::splitFirstDay(
						ProjectCalendarPtr calendar, TimephasedCostPtr assignment)
{
	TimephasedCostArrayPtr result (new TimephasedCostArray(2));

    //
    // Retrieve data used to calculate the pro-rata work split
    //
    DatePtr assignmentStart = assignment->getStart();
    DatePtr assignmentFinish = assignment->getFinish();
    DurationPtr calendarWork = calendar->getWork(assignmentStart, assignmentFinish, TimeUnit::MINUTES);

    if (calendarWork->getDuration() != 0)
    {
        //
        // Split the first day
        //
        DatePtr splitFinish;
        double splitCost;
        if (calendar->isWorkingDate(assignmentStart))
        {
			DatePtr splitStart = assignmentStart;
			DatePtr splitFinishTime = calendar->getFinishTime(splitStart);
			splitFinish = DateHelper::setTime(splitStart, splitFinishTime);
			DurationPtr calendarSplitWork = calendar->getWork(splitStart, splitFinish, TimeUnit::MINUTES);
			splitCost = (assignment->getTotalAmount()->doubleValue() * calendarSplitWork->getDuration()) / calendarWork->getDuration();

			TimephasedCostPtr split (new TimephasedCost());
			split->setStart(splitStart);
			split->setFinish(splitFinish);
			split->setTotalAmount(NumberPtr(new Number(splitCost)));

			(*result)[0] = split;
        }
        else
        {
			splitFinish = assignmentStart;
			splitCost = 0;
        }

        //
        // Split the remainder
        //
        DatePtr splitStart = calendar->getNextWorkStart(splitFinish);
        splitFinish = assignmentFinish;
        TimephasedCostPtr split;
        if (!(splitStart->getTime() > splitFinish->getTime()))
        {
			splitCost = assignment->getTotalAmount()->doubleValue() - splitCost;

			split.reset (new TimephasedCost());
			split->setStart(splitStart);
			split->setFinish(splitFinish);
			split->setTotalAmount(NumberPtr(new Number(splitCost)));
			split->setAmountPerDay(assignment->getAmountPerDay());
        }

		(*result)[1] = split;
    }
    return result;
}

/**
* This method merges together assignment data for the same day.
* 
* @param list assignment data
*/
void MPPTimephasedBaselineCostNormaliser::mergeSameDay(TimephasedCostListPtr list)
{
	TimephasedCostListPtr result(new TimephasedCostList());

    TimephasedCostPtr previousAssignment;

	for (size_t i = 0; i < list->GetCount();i ++)
    {
		TimephasedCostPtr assignment = list->GetAt(i);

        if (!previousAssignment)
        {
			assignment->setAmountPerDay(assignment->getTotalAmount());
			result->Add(assignment);
        }
        else
        {
			DatePtr previousAssignmentStart = previousAssignment->getStart();
			DatePtr previousAssignmentStartDay = DateHelper::getDayStartDate(previousAssignmentStart);
			DatePtr assignmentStart = assignment->getStart();
			DatePtr assignmentStartDay = DateHelper::getDayStartDate(assignmentStart);

			if (previousAssignmentStartDay->getTime() == assignmentStartDay->getTime())
			{
				result->removeLast();

				double cost = previousAssignment->getTotalAmount()->doubleValue();
				cost += assignment->getTotalAmount()->doubleValue();

				TimephasedCostPtr merged ( new TimephasedCost());
				merged->setStart(previousAssignment->getStart());
				merged->setFinish(assignment->getFinish());
				merged->setTotalAmount(NumberPtr(new Number(cost)));
				assignment = merged;
			}

			assignment->setAmountPerDay(assignment->getTotalAmount());
			result->Add(assignment);
        }

        previousAssignment = assignment;
    }

    list->RemoveAll();
    list->Append(*result);
}

/**
* This method merges together assignment data for the same cost.
* 
* @param list assignment data
*/
void MPPTimephasedBaselineCostNormaliser::mergeSameCost(TimephasedCostListPtr list)
{
    TimephasedCostListPtr result (new TimephasedCostList());

    TimephasedCostPtr previousAssignment;
	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedCostPtr assignment = list->GetAt(i);

        if (!previousAssignment)
        {
			assignment->setAmountPerDay(assignment->getTotalAmount());
			result->Add(assignment);
        }
        else
        {
			NumberPtr previousAssignmentCost = previousAssignment->getAmountPerDay();
			NumberPtr assignmentCost = assignment->getTotalAmount();

			if (NumberHelper::equals(previousAssignmentCost->doubleValue(), assignmentCost->doubleValue(), 0.01))
			{
				DatePtr assignmentStart = previousAssignment->getStart();
				DatePtr assignmentFinish = assignment->getFinish();
				double total = previousAssignment->getTotalAmount()->doubleValue();
				total += assignmentCost->doubleValue();

				TimephasedCostPtr merged (new TimephasedCost());
				merged->setStart(assignmentStart);
				merged->setFinish(assignmentFinish);
				merged->setAmountPerDay(assignmentCost);
				merged->setTotalAmount(DoublePtr(new Double(total)));

				result->removeLast();
				assignment = merged;
			}
			else
			{
				assignment->setAmountPerDay(assignment->getTotalAmount());
			}
			result->Add(assignment);
        }

        previousAssignment = assignment;
    }

    list->RemoveAll();
    list->Append(*result);
}