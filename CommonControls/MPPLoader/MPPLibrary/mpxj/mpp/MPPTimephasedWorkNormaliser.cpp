#include "stdafx.h"
#include "MPPTimephasedWorkNormaliser.h"
#include "..\common\DateHelper.h"

/**
* This method merges together assignment data for the same day->
* 
* @param calendar current calendar
* @param list assignment data
*/
void MPPTimephasedWorkNormaliser::mergeSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
    TimephasedWorkListPtr result (new TimephasedWorkList());

    TimephasedWorkPtr previousAssignment;

	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = list->GetAt(i);

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
				DurationPtr previousAssignmentWork = previousAssignment->getTotalAmount();
				DurationPtr assignmentWork = assignment->getTotalAmount();

				if (previousAssignmentWork->getDuration() != 0 && assignmentWork->getDuration() == 0)
				{
					continue;
				}

				DatePtr previousAssignmentFinish = previousAssignment->getFinish();

				if (previousAssignmentFinish->getTime() == assignmentStart->getTime() || 
					calendar->getNextWorkStart(previousAssignmentFinish)->getTime() == assignmentStart->getTime())
				{
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