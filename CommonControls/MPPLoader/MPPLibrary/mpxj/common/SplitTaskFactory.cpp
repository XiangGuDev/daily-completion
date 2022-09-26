#include "stdafx.h"
#include "SplitTaskFactory.h"
#include "..\DateRange.h"

/**
* Process the timephased resource assignment data to work out the 
* split structure of the task.
* 
* @param task parent task
* @param timephasedComplete completed resource assignment work
* @param timephasedPlanned planned resource assignment work
*/
void SplitTaskFactory::processSplitData(Task* task,
	TimephasedWorkListPtr timephasedComplete, TimephasedWorkListPtr timephasedPlanned)
{
    DatePtr splitsComplete;
	TimephasedWorkPtr lastComplete;
	TimephasedWorkPtr firstPlanned;
    if (!timephasedComplete->isEmpty())
    {
        lastComplete = timephasedComplete->GetAt(timephasedComplete->GetCount() - 1);
        splitsComplete = lastComplete->getFinish();
    }

    if (!timephasedPlanned->isEmpty())
    {
        firstPlanned = timephasedPlanned->GetAt(0);
    }

	DateRangeListPtr splits(new DateRangeList());
	TimephasedWorkPtr lastAssignment;
	DateRangePtr lastRange;
	for (size_t i = 0; i < timephasedComplete->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = timephasedComplete->GetAt(i);

		if (lastAssignment && lastRange
			&& lastAssignment->getTotalAmount()->getDuration() != 0
			&& assignment->getTotalAmount()->getDuration() != 0)
        {
			splits->RemoveAt(splits->GetCount()-1);	// remove last;
			lastRange.reset (new DateRange(lastRange->getStart(), assignment->getFinish()));
        }
        else
        {
			lastRange.reset (new DateRange(assignment->getStart(), assignment->getFinish()));
        }
		splits->Add(lastRange);
        lastAssignment = assignment;
    }

    //
    // We may not have a split, we may just have a partially
    // complete split.
    //
	DatePtr splitStart;
	if (lastComplete && firstPlanned
		&& lastComplete->getTotalAmount()->getDuration() != 0
		&& firstPlanned->getTotalAmount()->getDuration() != 0)
    {
		lastRange = splits->GetAt(splits->GetCount() - 1);
		splits->RemoveAt(splits->GetCount() - 1);
		splitStart = lastRange->getStart();
    }

	lastAssignment.reset();
	lastRange.reset();
	for (size_t i = 0; i < timephasedPlanned->GetCount(); i++)
    {
		TimephasedWorkPtr assignment = timephasedPlanned->GetAt(i);

		if (!splitStart)
        {
			if (lastAssignment && lastRange
				&& lastAssignment->getTotalAmount()->getDuration() != 0
				&& assignment->getTotalAmount()->getDuration() != 0)
			{
					splits->RemoveAt(splits->GetCount() - 1);
					lastRange.reset (new DateRange(lastRange->getStart(), assignment->getFinish()));
			}
			else
			{
				lastRange.reset (new DateRange(assignment->getStart(), assignment->getFinish()));
			}
        }
        else
        {
			lastRange.reset (new DateRange(splitStart, assignment->getFinish()));
        }
		splits->Add(lastRange);
		splitStart.reset();
        lastAssignment = assignment;
    }

    //
    // We must have a minimum of 3 entries for this to be a valid split task
    //
	if (splits->GetCount() > 2)
    {
		task->getSplits()->Append(*splits);
		task->setSplitCompleteDuration(splitsComplete);
    }
    else
    {
		task->setSplits(NULL);
		task->setSplitCompleteDuration(NULL);
    }
}