#include "stdafx.h"
#include "MSPDITimephasedWorkNormaliser.h"
#include "..\Duration.h"
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
void MSPDITimephasedWorkNormaliser::normalise(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
    //dumpList("raw", result);
    splitDays(calendar, list);
    //dumpList("split days", result);
    mergeSameDay(calendar, list);
    //dumpList("mergeSameDay", result);
    mergeSameWork(list);
    //dumpList("mergeSameWork", result);
    validateSameDay(calendar, list);
    convertToHours(list);
}

/*
    void dumpList(String label, TimephasedWorkListPtr list)
    {
        System.out.println(label);
        for (TimephasedWork assignment : list)
        {
        System.out.println(assignment);
        }
    }
    */

/**
* This method breaks down spans of time into individual days.
* 
* @param calendar current project calendar
* @param list list of assignment data
*/
void MSPDITimephasedWorkNormaliser::splitDays(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
    TimephasedWorkListPtr result (new TimephasedWorkList());
	CalendarPtr cal = Calendar::getInstance();

	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = list->GetAt(i);

        while (assignment != NULL)
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

			TimephasedWorkListPtr split = splitFirstDay(calendar, assignment);
			if (split->GetCount() && split->GetAt(0) != NULL)
			{
				result->Add(split->GetAt(0));
			}
			assignment = split->GetAt(1);
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
TimephasedWorkListPtr MSPDITimephasedWorkNormaliser::splitFirstDay(ProjectCalendarPtr calendar, TimephasedWorkPtr assignment)
{
	TimephasedWorkPtr result[2] = { 0 };

    //
    // Retrieve data used to calculate the pro-rata work split
    //
    DatePtr assignmentStart = assignment->getStart();
	DatePtr assignmentFinish = assignment->getFinish();
	DurationPtr calendarWork = calendar->getWork(assignmentStart, assignmentFinish, TimeUnit::MINUTES);
	DurationPtr assignmentWork = assignment->getTotalAmount();

	if (calendarWork->getDuration() != 0)
    {
        //
        // Split the first day
        //
        DatePtr splitFinish = NULL;
        double splitMinutes = 0;
		if (calendar->isWorkingDate(assignmentStart))
        {
			DatePtr splitStart = assignmentStart;
			DatePtr splitFinishTime = calendar->getFinishTime(splitStart);
			splitFinish = DateHelper::setTime(splitStart, splitFinishTime);
			ATLASSERT(splitFinish != NULL);
			splitMinutes = calendar->getWork(splitStart, splitFinish, TimeUnit::MINUTES)->getDuration();

			splitMinutes *= assignmentWork->getDuration();
			splitMinutes /= calendarWork->getDuration();
			splitMinutes = NumberHelper::truncate(splitMinutes, 2);

			DurationPtr splitWork = Duration2::getInstance(splitMinutes, TimeUnit::MINUTES);

			TimephasedWorkPtr split (new TimephasedWork());
			split->setStart(splitStart);
			split->setFinish(splitFinish);
			split->setTotalAmount(splitWork);

			result[0] = split;
        }
        else
        {
			splitFinish = assignmentStart;
			splitMinutes = 0;
        }

        //
        // Split the remainder
        //
		DatePtr splitStart = calendar->getNextWorkStart(splitFinish);
        splitFinish = assignmentFinish;
        TimephasedWorkPtr split =	NULL;

        if (splitStart->getTime() <= splitFinish->getTime())
        {
			splitMinutes = assignmentWork->getDuration() - splitMinutes;
			DurationPtr splitWork = Duration2::getInstance(splitMinutes, TimeUnit::MINUTES);

			split.reset (new TimephasedWork());
			split->setStart(splitStart);
			split->setFinish(splitFinish);
			split->setTotalAmount(splitWork);
        }

        result[1] = split;
    }

	TimephasedWorkListPtr arr (new TimephasedWorkList());
	arr->Add(result[0]);
	arr->Add(result[1]);

	return arr;
}

/**
* This method merges together assignment data for the same day.
* 
* @param calendar current calendar
* @param list assignment data
*/
void MSPDITimephasedWorkNormaliser::mergeSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
    TimephasedWorkListPtr result (new TimephasedWorkList());

    TimephasedWorkPtr previousAssignment = NULL;
	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = list->GetAt(i);

        if (previousAssignment == NULL)
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
				DurationPtr previousAssignmentWork = previousAssignment->getTotalAmount();
				DurationPtr assignmentWork = assignment->getTotalAmount();

				if (previousAssignmentWork->getDuration() != 0 && assignmentWork->getDuration() == 0)
				{
					continue;
				}

				result->removeLast();

				if (previousAssignmentWork->getDuration() != 0 && assignmentWork->getDuration() != 0)
				{
					double work = previousAssignment->getTotalAmount()->getDuration();
					work += assignment->getTotalAmount()->getDuration();
					DurationPtr totalWork = Duration2::getInstance(work, TimeUnit::MINUTES);

					TimephasedWorkPtr merged (new TimephasedWork());
					merged->setStart(previousAssignment->getStart());
					merged->setFinish(assignment->getFinish());
					merged->setTotalAmount(totalWork);
					assignment = merged;
				}
				else
				{
					if (assignmentWork->getDuration() == 0)
					{
						assignment = previousAssignment;
					}
				}
			}

			assignment->setAmountPerDay(assignment->getTotalAmount());
			result->Add(assignment);
        }

		DurationPtr calendarWork = calendar->getWork(assignment->getStart(), assignment->getFinish(), TimeUnit::MINUTES);
		DurationPtr assignmentWork = assignment->getTotalAmount();
		if (calendarWork->getDuration() == 0 && assignmentWork->getDuration() == 0)
        {
			result->removeLast();
        }
        else
        {
			previousAssignment = assignment;
        }
    }

	list->RemoveAll();
    list->Append(*result);
}

/**
* Ensures that the start and end dates for ranges fit within the
* working times for a given day.
* 
* @param calendar current calendar
* @param list assignment data
*/
void MSPDITimephasedWorkNormaliser::validateSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = list->GetAt(i);

		DatePtr assignmentStart = assignment->getStart();
		DatePtr calendarStartTime = calendar->getStartTime(assignmentStart);
        DatePtr assignmentStartTime = DateHelper::getCanonicalTime(assignmentStart);
		DatePtr assignmentFinish = assignment->getFinish();
		DatePtr calendarFinishTime = calendar->getFinishTime(assignmentFinish);
        DatePtr assignmentFinishTime = DateHelper::getCanonicalTime(assignmentFinish);
		double totalWork = assignment->getTotalAmount()->getDuration();

		if (assignmentStartTime != NULL && calendarStartTime != NULL)
        {
			if ((totalWork == 0 && assignmentStartTime->getTime() != calendarStartTime->getTime()) ||
				(assignmentStartTime->getTime() < calendarStartTime->getTime()))
			{
				assignmentStart = DateHelper::setTime(assignmentStart, calendarStartTime);
				assignment->setStart(assignmentStart);
			}
        }

        if (assignmentFinishTime != NULL && calendarFinishTime != NULL)
        {
			if ((totalWork == 0 && assignmentFinishTime->getTime() != calendarFinishTime->getTime()) ||
				(assignmentFinishTime->getTime() > calendarFinishTime->getTime()))
			{
				assignmentFinish = DateHelper::setTime(assignmentFinish, calendarFinishTime);
				assignment->setFinish(assignmentFinish);
			}
        }
    }
}
