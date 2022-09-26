#include "stdafx.h"
#include "RecurringTask.h"
#include "Duration.h"

RecurringTask::RecurringTask() :
	m_startDate(NULL),
	m_finishDate(NULL),
	m_duration(NULL),
	m_occurrences(0),
	m_recurrenceType((RecurrenceType)0),
	m_useEndDate(false),
	m_dailyFrequency(0),
	m_dailyWorkday(false),
	m_weeklyFrequency(0),
	m_weeklyDays(0),
	m_monthlyRelative(false),
	m_monthlyRelativeOrdinal(0),
	m_monthlyRelativeDay((Day)0),
	m_monthlyRelativeFrequency(0),
	m_monthlyAbsoluteDay(0),
	m_monthlyAbsoluteFrequency(0),
	m_yearlyAbsolute(false),
	m_yearlyAbsoluteDate(NULL),
	m_yearlyRelativeOrdinal(0),
	m_yearlyRelativeDay((Day)0),
	m_yearlyRelativeMonth(0)
{

}

/**
* Gets the start date of this recurring task.
*
* @return date to start recurring task
*/
DatePtr RecurringTask::getStartDate()
{
    return (m_startDate);
}

/**
* Sets the start date of this recurring task.
*
* @param val date to start recurring task
*/
void RecurringTask::setStartDate(DatePtr val)
{
    m_startDate = val;
}

/**
* Gets the finish date of this recurring task.
*
* @return date to finish recurring task
*/
DatePtr RecurringTask::getFinishDate()
{
    return (m_finishDate);
}

/**
* Sets the finish date of this recurring task.
*
* @param val date to finish recurring task
*/
void RecurringTask::setFinishDate(DatePtr val)
{
    m_finishDate = val;
}

/**
* Retrieve the duration of the recurring task.
*
* @return duration of recurring task
*/
DurationPtr RecurringTask::getDuration()
{
    return (m_duration);
}

/**
* Set the duration of the recurring task.
*
* @param duration duration of the recurring task
*/
void RecurringTask::setDuration(DurationPtr duration)
{
    m_duration = duration;
}

/**
* Sets the number of occurrences.
*
* @return number of occurrences
*/
int RecurringTask::getOccurrences()
{
    return (m_occurrences);
}

/**
* Retrieves the number of occurrences.
*
* @param occurrences number of occurrences
*/
void RecurringTask::setOccurrences(int occurrences)
{
    m_occurrences = occurrences;
}

/**
* Retrieves the recurrence type.
*
* @return RecurrenceType instance
*/
RecurrenceType RecurringTask::getRecurrenceType()
{
    return (m_recurrenceType);
}

/**
* Sets the recurrence type.
* 
* @param type recurrence type
*/
void RecurringTask::setRecurrenceType(RecurrenceType type)
{
    m_recurrenceType = type;
}

/**
* Retrieves the use end date flag.
*
* @return use end date flag
*/
bool RecurringTask::getUseEndDate()
{
    return (m_useEndDate);
}

/**
* Sets the use end date flag.
*
* @param useEndDate use end date flag
*/
void RecurringTask::setUseEndDate(bool useEndDate)
{
    m_useEndDate = useEndDate;
}

/**
* Retrieve the daily workday flag.
*
* @return bool flag
*/
bool RecurringTask::getDailyWorkday()
{
    return (m_dailyWorkday);
}

/**
* Set the daily workday flag.
*
* @param workday workday flag
*/
void RecurringTask::setDailyWorkday(bool workday)
{
    m_dailyWorkday = workday;
}

/**
* Retrieves a bit field representing days of the week.
* MSB=Sunday, LSB=Saturday.
*
* @return integer bit field
*/
int RecurringTask::getWeeklyDays()
{
    return (m_weeklyDays);
}

/**
* Sets a bit field representing days of the week.
* MSB=Sunday, LSB=Saturday.
* 
* @param days integer bit field
*/
void RecurringTask::setWeeklyDays(int days)
{
    m_weeklyDays = days;
}

/**
* Retrieves the monthly relative flag.
* 
* @return bool flag
*/
bool RecurringTask::getMonthlyRelative()
{
    return (m_monthlyRelative);
}

/**
* Sets the monthly relative flag.
*
* @param relative bool flag
*/
void RecurringTask::setMonthlyRelative(bool relative)
{
    m_monthlyRelative = relative;
}

/**
* Retrieve the yearly relative flag.
* 
* @return bool flag
*/
bool RecurringTask::getYearlyAbsolute()
{
    return (m_yearlyAbsolute);
}

/**
* Set the yearly relative flag.
*
* @param absolute bool flag
*/
void RecurringTask::setYearlyAbsolute(bool absolute)
{
    m_yearlyAbsolute = absolute;
}

/**
* Retrieves the recurring task frequency.
*
* @return recurring task frequency
*/
int RecurringTask::getDailyFrequency()
{
    return (m_dailyFrequency);
}

/**
* Set the recurring task frequency.
*
* @param frequency recurring task frequency
*/
void RecurringTask::setDailyFrequency(int frequency)
{
    m_dailyFrequency = frequency;
}

/**
* Retrieves the recurring task frequency.
*
* @return recurring task frequency
*/
int RecurringTask::getWeeklyFrequency()
{
    return (m_weeklyFrequency);
}

/**
* Set the recurring task frequency.
*
* @param frequency recurring task frequency
*/
void RecurringTask::setWeeklyFrequency(int frequency)
{
    m_weeklyFrequency = frequency;
}

/**
* Retrieves the monthly relative ordinal value.
*
* @return monthly relative ordinal value
*/
int RecurringTask::getMonthlyRelativeOrdinal()
{
    return (m_monthlyRelativeOrdinal);
}

/**
* Sets the monthly relative ordinal value.
*
* @param ordinal monthly relative ordinal value
*/
void RecurringTask::setMonthlyRelativeOrdinal(int ordinal)
{
    m_monthlyRelativeOrdinal = ordinal;
}

/**
* Retrieves the monthly relative day.
*
* @return monthly relative day
*/
Day RecurringTask::getMonthlyRelativeDay()
{
    return (m_monthlyRelativeDay);
}

/**
* Sets the monthly relative day.
*
* @param day monthly relative day
*/
void RecurringTask::setMonthlyRelativeDay(Day day)
{
    m_monthlyRelativeDay = day;
}

/**
* Sets the monthly relative frequency.
* 
* @return monthly relative frequency
*/
int RecurringTask::getMonthlyRelativeFrequency()
{
    return (m_monthlyRelativeFrequency);
}

/**
* Retrieves the monthly relative frequency.
*
* @param frequency monthly relative frequency
*/
void RecurringTask::setMonthlyRelativeFrequency(int frequency)
{
    m_monthlyRelativeFrequency = frequency;
}

/**
* Retrieves the monthly absolute day.
*
* @return monthly absolute day.
*/
int RecurringTask::getMonthlyAbsoluteDay()
{
    return (m_monthlyAbsoluteDay);
}

/**
* Sets the monthly absolute day.
*
* @param day monthly absolute day
*/
void RecurringTask::setMonthlyAbsoluteDay(int day)
{
    m_monthlyAbsoluteDay = day;
}

/**
* Retrieves the monthly absolute frequency.
*
* @return monthly absolute frequency
*/
int RecurringTask::getMonthlyAbsoluteFrequency()
{
    return (m_monthlyAbsoluteFrequency);
}

/**
* Sets the monthly absolute frequency.
*
* @param frequency monthly absolute frequency
*/
void RecurringTask::setMonthlyAbsoluteFrequency(int frequency)
{
    m_monthlyAbsoluteFrequency = frequency;
}

/**
* Retrieves the yearly relative ordinal.
*
* @return yearly relative ordinal
*/
int RecurringTask::getYearlyRelativeOrdinal()
{
    return (m_yearlyRelativeOrdinal);
}

/**
* Sets the yearly relative ordinal.
*
* @param ordinal yearly relative ordinal
*/
void RecurringTask::setYearlyRelativeOrdinal(int ordinal)
{
    m_yearlyRelativeOrdinal = ordinal;
}

/**
* Retrieve the yearly relative day.
*
* @return yearly relative day
*/
Day RecurringTask::getYearlyRelativeDay()
{
    return (m_yearlyRelativeDay);
}

/**
* Sets the yearly relative day.
*
* @param day yearly relative day
*/
void RecurringTask::setYearlyRelativeDay(Day day)
{
    m_yearlyRelativeDay = day;
}

/**
* Retrieves the yearly relative month.
*
* @return yearly relative month
*/
int RecurringTask::getYearlyRelativeMonth()
{
    return (m_yearlyRelativeMonth);
}

/**
* Sets the yearly relative month.
*
* @param month yearly relative month
*/
void RecurringTask::setYearlyRelativeMonth(int month)
{
    m_yearlyRelativeMonth = month;
}

/**
* Retrieves the yearly absolute date.
*
* @return yearly absolute date
*/
DatePtr RecurringTask::getYearlyAbsoluteDate()
{
    return (m_yearlyAbsoluteDate);
}

/**
* Sets the yearly absolute date.
*
* @param date yearly absolute date
*/
void RecurringTask::setYearlyAbsoluteDate(DatePtr date)
{
    m_yearlyAbsoluteDate = date;
}

///**
//* {@inheritDoc}
//*/
//CString RecurringTask::toString()
//{
//    DateFormatSymbols dfs = new DateFormatSymbols();
//    SimpleDateFormat df = new SimpleDateFormat("d MMM");
//    ByteArray2OutputStream os = new ByteArray2OutputStream();
//    PrintWriter pw = new PrintWriter(os);
//    pw.print("[RecurringTask");
//
//    if (m_duration != NULL)
//    {
//        pw.print(" Duration " + m_duration);
//        pw.print(" This Occurs " + m_recurrenceType);
//
//        switch (m_recurrenceType)
//        {
//        case DAILY:
//        {
//            pw.print(" " + ORDINAL[m_dailyFrequency.intValue()]);
//            pw.print(m_dailyWorkday ? " Workday" : " Day");
//            break;
//        }
//
//        case WEEKLY:
//        {
//            pw.print(" " + ORDINAL[m_weeklyFrequency.intValue()]);
//            pw.print(" week on ");
//            if ((m_weeklyDays.intValue() & 0x40) != 0)
//            {
//                pw.print("Sunday");
//            }
//
//            if ((m_weeklyDays.intValue() & 0x20) != 0)
//            {
//                pw.print("Monday");
//            }
//
//            if ((m_weeklyDays.intValue() & 0x10) != 0)
//            {
//                pw.print("Tuesday");
//            }
//
//            if ((m_weeklyDays.intValue() & 0x08) != 0)
//            {
//                pw.print("Wednesday");
//            }
//
//            if ((m_weeklyDays.intValue() & 0x04) != 0)
//            {
//                pw.print("Thursday");
//            }
//
//            if ((m_weeklyDays.intValue() & 0x02) != 0)
//            {
//                pw.print("Friday");
//            }
//
//            if ((m_weeklyDays.intValue() & 0x01) != 0)
//            {
//                pw.print("Saturday");
//            }
//
//            break;
//        }
//
//        case MONTHLY:
//        {
//            if (m_monthlyRelative)
//            {
//                pw.print(" on The ");
//                pw.print(DAY_ORDINAL[m_monthlyRelativeOrdinal.intValue()]);
//                pw.print(" ");
//                pw.print(dfs.getWeekdays()[m_monthlyRelativeDay.getValue()]);
//                pw.print(" of ");
//                pw.print(ORDINAL[m_monthlyRelativeFrequency.intValue()]);
//            }
//            else
//            {
//                pw.print(" on Day ");
//                pw.print(m_monthlyAbsoluteDay);
//                pw.print(" of ");
//                pw.print(ORDINAL[m_monthlyAbsoluteFrequency.intValue()]);
//            }
//            pw.print(" month");
//            break;
//        }
//
//        case YEARLY:
//        {
//            pw.print(" on the ");
//            if (m_yearlyAbsolute)
//            {
//                pw.print(df.format(m_yearlyAbsoluteDate));
//            }
//            else
//            {
//                pw.print(DAY_ORDINAL[m_yearlyRelativeOrdinal.intValue()]);
//                pw.print(" ");
//                pw.print(dfs.getWeekdays()[m_yearlyRelativeDay.getValue()]);
//                pw.print(" of ");
//                pw.print(dfs.getMonths()[m_yearlyRelativeMonth.intValue() - 1]);
//            }
//            break;
//        }
//        }
//
//        pw.print(" From " + m_startDate);
//        pw.print(" For " + m_occurrences + " occurrences");
//        pw.print(" To " + m_finishDate);
//    }
//    pw.println("]");
//    pw.flush();
//    return (os.toString());
//}

/**
* List of ordinal names used to generate debugging output.
*/
LPCTSTR ORDINAL[] =
{
    NULL,
    L"every",
	L"every other",
	L"every 3rd",
	L"every 4th",
	L"every 5th",
	L"every 6th",
	L"every 7th",
	L"every 8th",
	L"every 9th",
	L"every 10th",
	L"every 11th",
	L"every 12th",
	NULL
};

/**
* List of ordinal names used to generate debugging output.
*/
LPCTSTR DAY_ORDINAL[] =
{
	NULL,
	L"First",
	L"Second",
	L"Third",
	L"Fourth",
	L"Last",
	NULL
};