#include "stdafx.h"
#include "MPPTimephasedBaselineWorkNormaliser.h"
#include "..\common\DateHelper.h"

/**
* This method merges together assignment data for the same day.
* 
* @param calendar current calendar
* @param list assignment data
*/
void MPPTimephasedBaselineWorkNormaliser::mergeSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
	TimephasedWorkListPtr result(new TimephasedWorkList());

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
				result->removeLast();

				double work = previousAssignment->getTotalAmount()->getDuration();
				work += assignment->getTotalAmount()->getDuration();
				DurationPtr totalWork = Duration2::getInstance(work, TimeUnit::MINUTES);

				TimephasedWorkPtr merged (new TimephasedWork());
				merged->setStart(previousAssignment->getStart());
				merged->setFinish(assignment->getFinish());
				merged->setTotalAmount(totalWork);
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