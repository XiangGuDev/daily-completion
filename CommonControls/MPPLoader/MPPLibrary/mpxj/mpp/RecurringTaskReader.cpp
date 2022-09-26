#include "stdafx.h"
#include "RecurringTaskReader.h"
#include "..\RecurringTask.h"
#include "..\Task.h"
#include "MPPUtility.h"

/**
* Constructor.
* 
* @param file parent project file
*/
RecurringTaskReader::RecurringTaskReader(ProjectFile* file)
{
    m_file = file;
}

/**
* Reads recurring task data.
* 
* @param task Task instance
* @param data recurring task data
*/
void RecurringTaskReader::processRecurringTask(Task* task, ByteArrayPtr data)
{
    RecurringTaskPtr rt = task->addRecurringTask();
    rt->setStartDate(MPPUtility::getDate(data.get(), 6));
    rt->setFinishDate(MPPUtility::getDate(data.get(), 10));
	rt->setDuration(MPPUtility::getAdjustedDuration(m_file, MPPUtility::getInt(data.get(), 12),
		MPPUtility::getDurationTimeUnits(MPPUtility::getShort(data.get(), 16))));

	rt->setOccurrences(MPPUtility::getShort(data.get(), 18));
	rt->setRecurrenceType((RecurrenceType)(MPPUtility::getShort(data.get(), 20)));
	rt->setUseEndDate(MPPUtility::getShort(data.get(), 24) == 1);
	rt->setDailyWorkday(MPPUtility::getShort(data.get(), 26) == 1);
    int days = 0;
	days += (MPPUtility::getShort(data.get(), 28) == 1 ? 0x40 : 0x00);
	days += (MPPUtility::getShort(data.get(), 30) == 1 ? 0x20 : 0x00);
	days += (MPPUtility::getShort(data.get(), 32) == 1 ? 0x10 : 0x00);
	days += (MPPUtility::getShort(data.get(), 34) == 1 ? 0x08 : 0x00);
	days += (MPPUtility::getShort(data.get(), 36) == 1 ? 0x04 : 0x00);
	days += (MPPUtility::getShort(data.get(), 38) == 1 ? 0x02 : 0x00);
	days += (MPPUtility::getShort(data.get(), 40) == 1 ? 0x01 : 0x00);

	rt->setWeeklyDays(days);
	rt->setMonthlyRelative(MPPUtility::getShort(data.get(), 42) == 1);
	rt->setYearlyAbsolute(MPPUtility::getShort(data.get(), 44) == 1);
	rt->setDailyFrequency(MPPUtility::getShort(data.get(), 46));
	rt->setWeeklyFrequency(MPPUtility::getShort(data.get(), 48));
	rt->setMonthlyRelativeOrdinal(MPPUtility::getShort(data.get(), 50));
	rt->setMonthlyRelativeDay((Day)(MPPUtility::getShort(data.get(), 52) + 1));
	rt->setMonthlyAbsoluteFrequency(MPPUtility::getShort(data.get(), 54));
	rt->setMonthlyAbsoluteDay(MPPUtility::getShort(data.get(), 56));
	rt->setMonthlyRelativeFrequency(MPPUtility::getShort(data.get(), 58));
	rt->setYearlyRelativeOrdinal(MPPUtility::getShort(data.get(), 60));
	rt->setYearlyRelativeDay((Day)(MPPUtility::getShort(data.get(), 62) + 1));
	rt->setYearlyRelativeMonth(MPPUtility::getShort(data.get(), 64));
    rt->setYearlyAbsoluteDate(MPPUtility::getDate(data.get(), 70));
}