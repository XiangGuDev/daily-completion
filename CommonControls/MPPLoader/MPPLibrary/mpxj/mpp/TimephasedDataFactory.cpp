#include "stdafx.h"
#include "TimephasedDataFactory.h"
#include "MPPUtility.h"
#include "..\common\NumberHelper.h"
#include "..\common\DefaultTimephasedCostContainer.h"

/**
* Given a block of data representing completed work, this method will
* retrieve a set of TimephasedWorkPtr instances which represent
* the day by day work carried out for a specific resource assignment->
*
* @param calendar calendar on which date calculations are based
* @param startDate assignment start date 
* @param data completed work data block
* @return list of TimephasedWorkPtr instances
*/
TimephasedWorkListPtr TimephasedDataFactory::getCompleteWork(ProjectCalendarPtr calendar, DatePtr startDate, ByteArrayPtr data)
{
	TimephasedWorkListPtr list(new TimephasedWorkList());

	if (!(calendar && data && data->length() > 0))
		return list;

    int blockCount = MPPUtility::getShort(data.get(), 0);
    double previousCumulativeWork = 0;
    TimephasedWorkPtr previousAssignment;

    int index = 32;
    int currentBlock = 0;
    while (currentBlock < blockCount && index + 20 <= (int)data->length())
    {
		double time = MPPUtility::getInt(data.get(), index + 0);
		time /= 80;
		DurationPtr startWork = Duration2::getInstance(time, TimeUnit::MINUTES);

		double currentCumulativeWork = (long)MPPUtility::getDouble(data.get(), index + 4);
		double assignmentDuration = currentCumulativeWork - previousCumulativeWork;
		previousCumulativeWork = currentCumulativeWork;
		assignmentDuration /= 1000;
		DurationPtr totalWork = Duration2::getInstance(assignmentDuration, TimeUnit::MINUTES);
		time = (long)MPPUtility::getDouble(data.get(), index + 12);
		time /= 125;
		time *= 6;
		DurationPtr workPerDay = Duration2::getInstance(time, TimeUnit::MINUTES);

		DatePtr start;
		if (startWork->getDuration() == 0)
		{
			start = startDate;
		}
		else
		{
			start = calendar->getDate(startDate, startWork, true);
		}

		TimephasedWorkPtr assignment (new TimephasedWork());
		assignment->setStart(start);
		assignment->setAmountPerDay(workPerDay);
		assignment->setTotalAmount(totalWork);

		if (previousAssignment)
		{
			DatePtr finish = calendar->getDate(startDate, startWork, false);
			previousAssignment->setFinish(finish);
			if (previousAssignment->getStart()->getTime() == previousAssignment->getFinish()->getTime())
			{
				list->removeLast();
			}
		}

		list->Add(assignment);
		previousAssignment = assignment;

		index += 20;
		++currentBlock;
	}

	if (previousAssignment)
	{
		double time = MPPUtility::getInt(data.get(), 24);
		time /= 80;
		DurationPtr finishWork = Duration2::getInstance(time, TimeUnit::MINUTES);
		DatePtr finish = calendar->getDate(startDate, finishWork, false);
		previousAssignment->setFinish(finish);
		if (previousAssignment->getStart()->getTime() == previousAssignment->getFinish()->getTime())
		{
			list->removeLast();
		}
    }

    return list;
}

/**
* Given a block of data representing planned work, this method will
* retrieve a set of TimephasedWorkPtr instances which represent
* the day by day work planned for a specific resource assignment->
*
* @param calendar calendar on which date calculations are based
* @param startDate assignment start date
* @param units assignment units 
* @param data planned work data block
* @param timephasedComplete list of complete work 
* @return list of TimephasedWorkPtr instances 
*/
TimephasedWorkListPtr TimephasedDataFactory::getPlannedWork(ProjectCalendarPtr calendar, 
	DatePtr startDate, double units, ByteArrayPtr data, TimephasedWorkListPtr timephasedComplete)
{
	TimephasedWorkListPtr list(new TimephasedWorkList());

	if (!(calendar && data && data->length() > 0))
		return list;

	int blockCount = MPPUtility::getShort(data.get(), 0);
    if (blockCount == 0)
    {
        if (!timephasedComplete->isEmpty() && units != 0)
        {
            TimephasedWorkPtr lastComplete = timephasedComplete->GetAt(timephasedComplete->GetCount() - 1);

            DatePtr startWork = calendar->getNextWorkStart(lastComplete->getFinish());
			double time = MPPUtility::getDouble(data.get(), 16);
            time /= 1000;
            DurationPtr totalWork = Duration2::getInstance(time, TimeUnit::MINUTES);
            DurationPtr adjustedTotalWork = Duration2::getInstance((time * 100) / units, TimeUnit::MINUTES);
            DatePtr finish = calendar->getDate(startWork, adjustedTotalWork, false);

			time = MPPUtility::getDouble(data.get(), 8);
            time /= 2000;
            time *= 6;
            DurationPtr workPerDay = Duration2::getInstance(time, TimeUnit::MINUTES);

            TimephasedWorkPtr assignment (new TimephasedWork());
            assignment->setStart(startWork);
            assignment->setAmountPerDay(workPerDay);
            assignment->setModified(false);
            assignment->setFinish(finish);
            assignment->setTotalAmount(totalWork);

            if (assignment->getStart()->getTime() != assignment->getFinish()->getTime())
            {
                list->Add(assignment);
            }
        }
    }
    else
    {
        DatePtr offset = startDate;

        if (!timephasedComplete->isEmpty())
        {
			TimephasedWorkPtr lastComplete = timephasedComplete->GetAt(timephasedComplete->GetCount() - 1);
            offset = lastComplete->getFinish();
        }

        int index = 40;
        double previousCumulativeWork = 0;
        TimephasedWorkPtr previousAssignment;
        int currentBlock = 0;

        while (currentBlock < blockCount && index + 28 <= (int)data->length())
        {
			double time = MPPUtility::getInt(data.get(), index);
            time /= 80;
            DurationPtr blockDuration = Duration2::getInstance(time, TimeUnit::MINUTES);
            DatePtr start;
            if (blockDuration->getDuration() == 0)
            {
                start = offset;
            }
            else
            {
                start = calendar->getDate(offset, blockDuration, true);
            }

			double currentCumulativeWork = MPPUtility::getDouble(data.get(), index + 4);
            double assignmentDuration = currentCumulativeWork - previousCumulativeWork;
            assignmentDuration /= 1000;
            DurationPtr totalWork = Duration2::getInstance(assignmentDuration, TimeUnit::MINUTES);
            previousCumulativeWork = currentCumulativeWork;

			time = MPPUtility::getDouble(data.get(), index + 12);
            time /= 2000;
            time *= 6;
            DurationPtr workPerDay = Duration2::getInstance(time, TimeUnit::MINUTES);

			int modifiedFlag = MPPUtility::getShort(data.get(), index + 22);
            bool modified = (modifiedFlag == 0 && currentBlock != 0) || ((modifiedFlag & 0x3000) != 0);

            TimephasedWorkPtr assignment (new TimephasedWork());
            assignment->setStart(start);
            assignment->setAmountPerDay(workPerDay);
            assignment->setModified(modified);
            assignment->setTotalAmount(totalWork);

            if (previousAssignment)
            {
                DatePtr finish = calendar->getDate(offset, blockDuration, false);
                previousAssignment->setFinish(finish);
                if (previousAssignment->getStart()->getTime() == previousAssignment->getFinish()->getTime())
                {
                    list->removeLast();
                }
            }

            list->Add(assignment);
            previousAssignment = assignment;

            index += 28;
            ++currentBlock;
        }

        if (previousAssignment)
        {
			double time = MPPUtility::getInt(data.get(), 24);
            time /= 80;
            DurationPtr blockDuration = Duration2::getInstance(time, TimeUnit::MINUTES);
            DatePtr finish = calendar->getDate(offset, blockDuration, false);
            previousAssignment->setFinish(finish);
            if (previousAssignment->getStart()->getTime() == previousAssignment->getFinish()->getTime())
            {
                list->removeLast();
            }
        }
	}

    return list;
}

/**
* Test the list of TimephasedWorkPtr instances to see
* if any of them have been modified-> 
* 
* @param list list of TimephasedWorkPtr instances
* @return bool flag
*/
bool TimephasedDataFactory::getWorkModified(TimephasedWorkListPtr list)
{
    bool result = false;
	for (size_t i = 0; i < list->GetCount(); i ++)
    {
		TimephasedWorkPtr assignment = list->GetAt(i);

        result = assignment->getModified();
        if (result)
        {
			break;
        }
    }
    return result;
}

/**
* Extracts baseline work from the MPP file for a specific baseline->
* Returns null if no baseline work is present, otherwise returns
* a list of timephased work items->
* 
* @param assignment parent assignment
* @param calendar baseline calendar
* @param normaliser normaliser associated with this data
* @param data timephased baseline work data block
* @param raw flag indicating if this data is to be treated as raw
* @return timephased work
*/
TimephasedWorkContainerPtr TimephasedDataFactory::getBaselineWork(Resource2AssignmentPtr assignment, 
	ProjectCalendarPtr calendar, TimephasedWorkNormaliserPtr normaliser, ByteArrayPtr data, bool raw)
{
    TimephasedWorkContainerPtr result;

	if (!(data && data->length() > 0))
		return result;

    TimephasedWorkListPtr list;

    //System->out->println(MPPUtility::hexdump(data, false));
    int index = 8; // 8 byte header
    int blockSize = 40;
    double previousCumulativeWorkPerformedInMinutes = 0;

	DatePtr blockStartDate = MPPUtility::getTimestampFromTenths(data.get(), index + 36);
    index += blockSize;
    TimephasedWorkPtr work;

    while (index + blockSize <= (int)data->length())
    {
		double cumulativeWorkInMinutes = (double)((long)MPPUtility::getDouble(data.get(), index + 20)) / 1000;
		if (!Duration2::durationValueEquals(cumulativeWorkInMinutes, previousCumulativeWorkPerformedInMinutes))
		{
			//double unknownWorkThisPeriodInMinutes = ((long) MPPUtility::getDouble(data, index + 0)) / 1000;
			double normalActualWorkThisPeriodInMinutes = ((double)MPPUtility::getInt(data.get(), index + 8)) / 10;
			double normalRemainingWorkThisPeriodInMinutes = ((double)MPPUtility::getInt(data.get(), index + 28)) / 10;
			double workThisPeriodInMinutes = cumulativeWorkInMinutes - previousCumulativeWorkPerformedInMinutes;
			double overtimeWorkThisPeriodInMinutes = workThisPeriodInMinutes - 
				(normalActualWorkThisPeriodInMinutes + normalRemainingWorkThisPeriodInMinutes);
			double overtimeFactor = overtimeWorkThisPeriodInMinutes / 
				(normalActualWorkThisPeriodInMinutes + normalRemainingWorkThisPeriodInMinutes);

			double normalWorkPerDayInMinutes = 480;
			double overtimeWorkPerDayInMinutes = normalWorkPerDayInMinutes * overtimeFactor;

			work.reset( new TimephasedWork());
			work->setFinish(MPPUtility::getTimestampFromTenths(data.get(), index + 16));
			work->setStart(blockStartDate);
			work->setTotalAmount(Duration2::getInstance(workThisPeriodInMinutes, TimeUnit::MINUTES));
			work->setAmountPerDay(Duration2::getInstance(normalWorkPerDayInMinutes + overtimeWorkPerDayInMinutes, TimeUnit::MINUTES));

			previousCumulativeWorkPerformedInMinutes = cumulativeWorkInMinutes;

			if (!list)
			{
				list.reset (new TimephasedWorkList());
			}
			list->Add(work);
			//System->out->println(work);
		}
		blockStartDate = MPPUtility::getTimestampFromTenths(data.get(), index + 36);
		index += blockSize;
    }

    if (list)
    {
		if (work)
		{
			work->setFinish(assignment->getFinish());
		}
		result.reset (new DefaultTimephasedWorkContainer(calendar, normaliser, list, raw));
    }

    return result;
}

/**
* Extracts baseline cost from the MPP file for a specific baseline->
* Returns null if no baseline cost is present, otherwise returns
* a list of timephased work items->
* 
* @param calendar baseline calendar
* @param normaliser normaliser associated with this data
* @param data timephased baseline work data block
* @param raw flag indicating if this data is to be treated as raw
* @return timephased work
*/
TimephasedCostContainerPtr TimephasedDataFactory::getBaselineCost(ProjectCalendarPtr calendar, 
	TimephasedCostNormaliserPtr normaliser, ByteArrayPtr data, bool raw)
{
    TimephasedCostContainerPtr result;

	if (!(data && data->length() > 0))
		return result;

    TimephasedCostListPtr list;

    //System->out->println(MPPUtility::hexdump(data, false));
    int index = 16; // 16 byte header
    int blockSize = 20;
    double previousTotalCost = 0;

	DatePtr blockStartDate = MPPUtility::getTimestampFromTenths(data.get(), index + 16);
    index += blockSize;

    while (index + blockSize <= (int)data->length())
    {
		DatePtr blockEndDate = MPPUtility::getTimestampFromTenths(data.get(), index + 16);
		double currentTotalCost = (double)((long)MPPUtility::getDouble(data.get(), index + 8)) / 100;
		if (!costEquals(previousTotalCost, currentTotalCost))
		{
			TimephasedCostPtr cost (new TimephasedCost());
			cost->setStart(blockStartDate);
			cost->setFinish(blockEndDate);
			cost->setTotalAmount(DoublePtr(new Double(currentTotalCost - previousTotalCost)));

			if (!list)
			{
				list.reset(new TimephasedCostList());
			}
			list->Add(cost);
			//System->out->println(cost);

			previousTotalCost = currentTotalCost;
		}

		blockStartDate = blockEndDate;
		index += blockSize;
    }

    if (list)
    {
		result.reset (new DefaultTimephasedCostContainer(calendar, normaliser, list, raw));
    }

    return result;
}

/**
* Equality test cost values->
* 
* @param lhs cost value
* @param rhs cost value
* @return true if costs are equal, within the allowable delta
*/
bool TimephasedDataFactory::costEquals(double lhs, double rhs)
{
    return NumberHelper::equals(lhs, rhs, 0.00001);
}