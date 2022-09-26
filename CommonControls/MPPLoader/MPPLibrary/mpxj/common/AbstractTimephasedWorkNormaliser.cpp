#include "stdafx.h"
#include "AbstractTimephasedWorkNormaliser.h"
#include "..\ProjectCalendar.h"
#include "..\common\NumberHelper.h"

/**
* Merges individual days together into time spans where the
* same work is undertaken each day.
* 
* @param list assignment data
*/
void AbstractTimephasedWorkNormaliser::mergeSameWork(TimephasedWorkListPtr list)
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
			DurationPtr previousAssignmentWork = previousAssignment->getAmountPerDay();
			DurationPtr assignmentWork = assignment->getTotalAmount();

			if (NumberHelper::equals(previousAssignmentWork->getDuration(), assignmentWork->getDuration(), 0.01))
			{
				DatePtr assignmentStart = previousAssignment->getStart();
				DatePtr assignmentFinish = assignment->getFinish();
				double total = previousAssignment->getTotalAmount()->getDuration();
				total += assignmentWork->getDuration();
				DurationPtr totalWork = Duration2::getInstance(total, TimeUnit::MINUTES);

				TimephasedWorkPtr merged (new TimephasedWork());
				merged->setStart(assignmentStart);
				merged->setFinish(assignmentFinish);
				merged->setAmountPerDay(assignmentWork);
				merged->setTotalAmount(totalWork);

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

/**
* Converts assignment duration values from minutes to hours.
* 
* @param list assignment data
*/
void AbstractTimephasedWorkNormaliser::convertToHours(TimephasedWorkListPtr list)
{
	for (size_t i = 0; i < list->GetCount(); i++)
	{
		TimephasedWorkPtr assignment = list->GetAt(i);

        DurationPtr totalWork = assignment->getTotalAmount();
		DurationPtr workPerDay = assignment->getAmountPerDay();
		totalWork = Duration2::getInstance(totalWork->getDuration() / 60, TimeUnit::HOURS);
		workPerDay = Duration2::getInstance(workPerDay->getDuration() / 60, TimeUnit::HOURS);
        assignment->setTotalAmount(totalWork);
		assignment->setAmountPerDay(workPerDay);
    }
}
