#include "stdafx.h"
#include "MPPAbstractTimephasedWorkNormaliser.h"
#include "..\java\Calendar.h"
#include "..\common\DateHelper.h"

const double MPPAbstractTimephasedWorkNormaliser::EQUALITY_DELTA = 0.1;

/**
* This method converts the internal representation of timephased 
* resource assignment data used by MS Project into a standardised
* format to make it easy to work with. 
* 
* @param calendar current calendar
* @param list list of assignment data
*/
void MPPAbstractTimephasedWorkNormaliser::normalise(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
    if (!list->isEmpty())
    {
        //dumpList(list);
        splitDays(calendar, list);
        //dumpList(list);
        mergeSameDay(calendar, list);
        //dumpList(list);
        mergeSameWork(list);
        //dumpList(list);         
        convertToHours(list);
        //dumpList(list);
    }
}

/**
* This method breaks down spans of time into individual days.
* 
* @param calendar current project calendar
* @param list list of assignment data
*/
void MPPAbstractTimephasedWorkNormaliser::splitDays(ProjectCalendarPtr calendar, TimephasedWorkListPtr list)
{
    TimephasedWorkListPtr result (new TimephasedWorkList());
    bool remainderInserted = false;
    CalendarPtr cal = Calendar::getInstance();

	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = list->GetAt(i);

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
				DurationPtr totalWork = assignment->getTotalAmount();
				DurationPtr assignmentWork = getAssignmentWork(calendar, assignment);
				if ((totalWork->getDuration() - assignmentWork->getDuration()) > EQUALITY_DELTA)
				{
					assignment->setTotalAmount(assignmentWork);
					result->Add(assignment);
					DurationPtr remainingWork = Duration2::getInstance(
						totalWork->getDuration() - assignmentWork->getDuration(), TimeUnit::MINUTES);

					cal->setTime(finishDay);
					cal->add(Calendar::DAY_OF_YEAR, 1);
					DatePtr remainderStart = cal->getTime();
					cal->add(Calendar::DAY_OF_YEAR, 1);
					DatePtr remainderFinish = cal->getTime();

					TimephasedWorkPtr remainder (new TimephasedWork());
					remainder->setStart(remainderStart);
					remainder->setFinish(remainderFinish);
					remainder->setTotalAmount(remainingWork);
					result->Add(remainder);

					remainderInserted = true;
				}
				else
				{
					result->Add(assignment);
				}
				break;
			}

			TimephasedWorkArrayPtr split = splitFirstDay(calendar, assignment);
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
TimephasedWorkArrayPtr MPPAbstractTimephasedWorkNormaliser::splitFirstDay(ProjectCalendarPtr calendar, TimephasedWorkPtr assignment)
{
    TimephasedWorkArrayPtr result (new TimephasedWorkArray(2));

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
        DatePtr splitFinish;
        double splitMinutes;
        if (calendar->isWorkingDate(assignmentStart))
        {
			DatePtr splitStart = assignmentStart;
			DatePtr splitFinishTime = calendar->getFinishTime(splitStart);
			splitFinish = DateHelper::setTime(splitStart, splitFinishTime);

			DurationPtr calendarSplitWork = calendar->getWork(splitStart, splitFinish, TimeUnit::MINUTES);
			DurationPtr calendarWorkPerDay = calendar->getWork(splitStart, TimeUnit::MINUTES);
			DurationPtr assignmentWorkPerDay = assignment->getAmountPerDay();
			DurationPtr splitWork;

			if (calendarSplitWork->durationComponentEquals(calendarWorkPerDay.get()))
			{
				{
					if (calendarSplitWork->durationComponentEquals(assignmentWorkPerDay.get()))
					{
						splitWork = assignmentWorkPerDay;
						splitMinutes = splitWork->getDuration();
					}
					else
					{
						splitMinutes = assignmentWorkPerDay->getDuration();
						splitMinutes *= calendarSplitWork->getDuration();
						splitMinutes /= (8 * 60); // this appears to be a fixed value
						splitWork = Duration2::getInstance(splitMinutes, TimeUnit::MINUTES);
					}
				}
			}
			else
			{
				splitMinutes = assignmentWorkPerDay->getDuration();
				splitMinutes *= calendarSplitWork->getDuration();
				splitMinutes /= (8 * 60); // this appears to be a fixed value
				splitWork = Duration2::getInstance(splitMinutes, TimeUnit::MINUTES);
			}

			TimephasedWorkPtr split (new TimephasedWork());
			split->setStart(splitStart);
			split->setFinish(splitFinish);
			split->setTotalAmount(splitWork);

			(*result)[0] = split;
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
        TimephasedWorkPtr split;
        if (!(splitStart->getTime() > splitFinish->getTime()))
        {
			splitMinutes = assignmentWork->getDuration() - splitMinutes;
			DurationPtr splitWork = Duration2::getInstance(splitMinutes, TimeUnit::MINUTES);

			split.reset (new TimephasedWork());
			split->setStart(splitStart);
			split->setFinish(splitFinish);
			split->setTotalAmount(splitWork);
			split->setAmountPerDay(assignment->getAmountPerDay());
        }

        (*result)[1] = split;
    }
    return result;
}

/**
* Retrieves the pro-rata work carried out on a given day.
* 
* @param calendar current calendar
* @param assignment current assignment->
* @return assignment work duration
*/
DurationPtr MPPAbstractTimephasedWorkNormaliser::getAssignmentWork(ProjectCalendarPtr calendar, TimephasedWorkPtr assignment)
{
    DatePtr assignmentStart = assignment->getStart();

    DatePtr splitStart = assignmentStart;
    DatePtr splitFinishTime = calendar->getFinishTime(splitStart);
    DatePtr splitFinish = DateHelper::setTime(splitStart, splitFinishTime);

    DurationPtr calendarSplitWork = calendar->getWork(splitStart, splitFinish, TimeUnit::MINUTES);
    DurationPtr assignmentWorkPerDay = assignment->getAmountPerDay();
    DurationPtr splitWork;

    double splitMinutes = assignmentWorkPerDay->getDuration();
    splitMinutes *= calendarSplitWork->getDuration();
    splitMinutes /= (8 * 60); // this appears to be a fixed value
    splitWork = Duration2::getInstance(splitMinutes, TimeUnit::MINUTES);
    return splitWork;
}
