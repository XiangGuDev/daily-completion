#pragma once

#include "MpxjEnum.h"

class Duration2;

/**
 * This class represents the Recurring Task Record as found in an MPX file.
 */
class RecurringTask
{
public:
	RecurringTask();

   /**
    * Gets the start date of this recurring task.
    *
    * @return date to start recurring task
    */
	DatePtr getStartDate();

   /**
    * Sets the start date of this recurring task.
    *
    * @param val date to start recurring task
    */
	void setStartDate(DatePtr val);

   /**
    * Gets the finish date of this recurring task.
    *
    * @return date to finish recurring task
    */
	DatePtr getFinishDate();

   /**
    * Sets the finish date of this recurring task.
    *
    * @param val date to finish recurring task
    */
	void setFinishDate(DatePtr val);

   /**
    * Retrieve the duration of the recurring task.
    *
    * @return duration of recurring task
    */
	std::shared_ptr<Duration2> getDuration();

   /**
    * Set the duration of the recurring task.
    *
    * @param duration duration of the recurring task
    */
	void setDuration(std::shared_ptr<Duration2> duration);

   /**
    * Sets the number of occurrences.
    *
    * @return number of occurrences
    */
	int getOccurrences();

   /**
    * Retrieves the number of occurrences.
    *
    * @param occurrences number of occurrences
    */
	void setOccurrences(int occurrences);

   /**
    * Retrieves the recurrence type.
    *
    * @return RecurrenceType instance
    */
	RecurrenceType getRecurrenceType();

   /**
    * Sets the recurrence type.
    * 
    * @param type recurrence type
    */
	void setRecurrenceType(RecurrenceType type);

   /**
    * Retrieves the use end date flag.
    *
    * @return use end date flag
    */
	bool getUseEndDate();

   /**
    * Sets the use end date flag.
    *
    * @param useEndDate use end date flag
    */
	void setUseEndDate(bool useEndDate);

   /**
    * Retrieve the daily workday flag.
    *
    * @return bool flag
    */
	bool getDailyWorkday();

   /**
    * Set the daily workday flag.
    *
    * @param workday workday flag
    */
	void setDailyWorkday(bool workday);

   /**
    * Retrieves a bit field representing days of the week.
    * MSB=Sunday, LSB=Saturday.
    *
    * @return integer bit field
    */
	int getWeeklyDays();

   /**
    * Sets a bit field representing days of the week.
    * MSB=Sunday, LSB=Saturday.
    * 
    * @param days integer bit field
    */
	void setWeeklyDays(int days);

   /**
    * Retrieves the monthly relative flag.
    * 
    * @return bool flag
    */
	bool getMonthlyRelative();

   /**
    * Sets the monthly relative flag.
    *
    * @param relative bool flag
    */
	void setMonthlyRelative(bool relative);

   /**
    * Retrieve the yearly relative flag.
    * 
    * @return bool flag
    */
	bool getYearlyAbsolute();

   /**
    * Set the yearly relative flag.
    *
    * @param absolute bool flag
    */
	void setYearlyAbsolute(bool absolute);

   /**
    * Retrieves the recurring task frequency.
    *
    * @return recurring task frequency
    */
	int getDailyFrequency();

   /**
    * Set the recurring task frequency.
    *
    * @param frequency recurring task frequency
    */
	void setDailyFrequency(int frequency);

   /**
    * Retrieves the recurring task frequency.
    *
    * @return recurring task frequency
    */
	int getWeeklyFrequency();

   /**
    * Set the recurring task frequency.
    *
    * @param frequency recurring task frequency
    */
	void setWeeklyFrequency(int frequency);

   /**
    * Retrieves the monthly relative ordinal value.
    *
    * @return monthly relative ordinal value
    */
	int getMonthlyRelativeOrdinal();

   /**
    * Sets the monthly relative ordinal value.
    *
    * @param ordinal monthly relative ordinal value
    */
	void setMonthlyRelativeOrdinal(int ordinal);

   /**
    * Retrieves the monthly relative day.
    *
    * @return monthly relative day
    */
	Day getMonthlyRelativeDay();

   /**
    * Sets the monthly relative day.
    *
    * @param day monthly relative day
    */
	void setMonthlyRelativeDay(Day day);

   /**
    * Sets the monthly relative frequency.
    * 
    * @return monthly relative frequency
    */
	int getMonthlyRelativeFrequency();

   /**
    * Retrieves the monthly relative frequency.
    *
    * @param frequency monthly relative frequency
    */
	void setMonthlyRelativeFrequency(int frequency);

   /**
    * Retrieves the monthly absolute day.
    *
    * @return monthly absolute day.
    */
	int getMonthlyAbsoluteDay();

   /**
    * Sets the monthly absolute day.
    *
    * @param day monthly absolute day
    */
	void setMonthlyAbsoluteDay(int day);

   /**
    * Retrieves the monthly absolute frequency.
    *
    * @return monthly absolute frequency
    */
	int getMonthlyAbsoluteFrequency();

   /**
    * Sets the monthly absolute frequency.
    *
    * @param frequency monthly absolute frequency
    */
	void setMonthlyAbsoluteFrequency(int frequency);

   /**
    * Retrieves the yearly relative ordinal.
    *
    * @return yearly relative ordinal
    */
	int getYearlyRelativeOrdinal();

   /**
    * Sets the yearly relative ordinal.
    *
    * @param ordinal yearly relative ordinal
    */
	void setYearlyRelativeOrdinal(int ordinal);

   /**
    * Retrieve the yearly relative day.
    *
    * @return yearly relative day
    */
	Day getYearlyRelativeDay();

   /**
    * Sets the yearly relative day.
    *
    * @param day yearly relative day
    */
	void setYearlyRelativeDay(Day day);

   /**
    * Retrieves the yearly relative month.
    *
    * @return yearly relative month
    */
	int getYearlyRelativeMonth();

   /**
    * Sets the yearly relative month.
    *
    * @param month yearly relative month
    */
	void setYearlyRelativeMonth(int month);

   /**
    * Retrieves the yearly absolute date.
    *
    * @return yearly absolute date
    */
	DatePtr getYearlyAbsoluteDate();

   /**
    * Sets the yearly absolute date.
    *
    * @param date yearly absolute date
    */
	void setYearlyAbsoluteDate(DatePtr date);

 //  /**
 //   * {@inheritDoc}
 //   */
	//virtual CString toString();

private:
   /**
    * List of ordinal names used to generate debugging output.
    */
	static LPCTSTR ORDINAL[];

   /**
    * List of ordinal names used to generate debugging output.
    */
	static LPCTSTR DAY_ORDINAL[];

   //
   // Common attributes
   //
   DatePtr m_startDate;
   DatePtr m_finishDate;
   std::shared_ptr<Duration2> m_duration;
   int m_occurrences;
   RecurrenceType m_recurrenceType;
   bool m_useEndDate;

   //
   // Daily recurrence attributes
   //
   int m_dailyFrequency;
   bool m_dailyWorkday;

   //
   // Weekly recurrence attributes
   //
   int m_weeklyFrequency;
   int m_weeklyDays;

   //
   // Monthly recurrence attributes
   //
   bool m_monthlyRelative;
   int m_monthlyRelativeOrdinal;
   Day m_monthlyRelativeDay;
   int m_monthlyRelativeFrequency;
   int m_monthlyAbsoluteDay;
   int m_monthlyAbsoluteFrequency;

   //
   // Yearly recurrence attributes
   //   
   bool m_yearlyAbsolute;
   DatePtr m_yearlyAbsoluteDate;
   int m_yearlyRelativeOrdinal;
   Day m_yearlyRelativeDay;
   int m_yearlyRelativeMonth;
};

typedef std::shared_ptr<RecurringTask> RecurringTaskPtr;