#pragma once

#include "..\Duration.h"

class Task;

/**
 * Utility methods for manipulating Dates.
 */
class COMMONCONTROLS_API DateHelper
{
public:
   /**
    * Returns a new DatePtr instance whose value
    * represents the start of the day (i.e. the time of day is 00:00:00.000)
    *
    * @param DatePtr DatePtr to convert
    * @return day start Date
    */
	static DatePtr getDayStartDate(DatePtr date);

   /**
    * Returns a new DatePtr instance whose value
    * represents the end of the day (i.e. the time of days is 11:59:59.999)
    *
    * @param DatePtr DatePtr to convert
    * @return day start Date
    */
	static DatePtr getDayEndDate(DatePtr date);

   /**
    * This method resets the DatePtr part of a DatePtr time value to
    * a standard DatePtr (1/1/1). This is used to allow times to
    * be compared and manipulated.
    * 
    * @param DatePtr DatePtr time value
    * @return DatePtr time with DatePtr set to a standard value
    */
	static DatePtr getCanonicalTime(DatePtr date);

   /**
    * This method compares a target DatePtr with a DatePtr range. The method will
    * return 0 if the DatePtr is within the range, less than zero if the Date
    * is before the range starts, and greater than zero if the DatePtr is after
    * the range ends.
    * 
    * @param startDate range start Date
    * @param endDate range end Date
    * @param targetDate target Date
    * @return comparison result
    */
	static int compare(DatePtr startDate, DatePtr endDate, DatePtr targetDate);

   /**
    * This method compares a target DatePtr with a DatePtr range. The method will
    * return 0 if the DatePtr is within the range, less than zero if the Date
    * is before the range starts, and greater than zero if the DatePtr is after
    * the range ends.
    * 
    * @param startDate range start Date
    * @param endDate range end Date
    * @param targetDate target DatePtr in milliseconds
    * @return comparison result
    */
	static int compare(DatePtr startDate, DatePtr endDate, time_t targetDate);

   /**
    * Compare two Dates, handling NULL values.
    * TODO: correct the comparison order to align with Date.compareTo 
    * 
    * @param d1 DatePtr instance
    * @param d2 DatePtr instance
    * @return int comparison result
    */
	static int compare(DatePtr d1, DatePtr d2);

   /**
    * This utility method calculates the difference in working
    * time between two Dates, given the context of a task.
    * 
    * @param task parent task
    * @param Date1 first Date
    * @param Date2 second Date
    * @param format required format for the resulting duration
    * @return difference in working time between the two Dates
    */
	static DurationPtr getVariance(Task* task, DatePtr Date1, DatePtr Date2, TimeUnit format);

   /**
    * Creates a DatePtr from the equivalent long value. This conversion 
    * takes account of the time zone.
    * 
    * @param DatePtr DatePtr expressed as a long integer
    * @return new DatePtr instance
    */
	static DatePtr getDateFromLong(time_t Date);

   /**
    * Creates a timestamp from the equivalent long value. This conversion
    * takes account of the time zone and any daylight savings time.
    * 
    * @param timestamp timestamp expressed as a long integer
    * @return new DatePtr instance
    */
	static DatePtr getTimestampFromLong(time_t timestamp);

   /**
    * Create a DatePtr instance representing a specific time.
    * 
    * @param hour hour 0-23
    * @param minutes minutes 0-59
    * @return new DatePtr instance
    */
	static DatePtr getTime(int hour, int minutes);

   /**
    * Given a DatePtr represented by a CalendarPtr instance, set the time
    * component of the DatePtr based on the hours and minutes of the
    * time supplied by the DatePtr instance.
    * 
    * @param cal CalendarPtr instance representing the Date
    * @param time DatePtr instance representing the time of day
    */
	static void setTime(CalendarPtr cal, DatePtr time);

   /**
    * Given a DatePtr represented by a DatePtr instance, set the time
    * component of the DatePtr based on the hours and minutes of the
    * time supplied by the DatePtr instance.
    * 
    * @param DatePtr DatePtr instance representing the Date
    * @param canonicalTime DatePtr instance representing the time of day
    * @return new DatePtr instance with the required time set
    */
	static DatePtr setTime(DatePtr date, DatePtr canonicalTime);

	static Day getNextDay(Day day);

   /** 
    * First DatePtr supported by Microsoft Project: January 01 00:00:00 1984. 
    */
	static DatePtr getFIRST_DATE();// = DateHelper::getTimestampFromLong(441763200000L);

   /** 
    * Last DatePtr supported by Microsoft Project: Friday December 31 23:59:00 2049. 
    */
	static DatePtr getLAST_DATE();// = DateHelper::getTimestampFromLong(2524607946000L);

	//************************************************************
	//FILETIME, SYSTEMTIME Óë time_t Ïà»¥×ª»» 

	//#### Time_tToSystemTime ####
	static void TimetToSystemTime(time_t t, LPSYSTEMTIME pst);

private:
   /**
    * Default value to use for DST savings if we are using a version
    * of Java < 1.4.
    */
   const int DEFAULT_DST_SAVINGS = 3600000;

   static DatePtr getCANONICAL_EPOCH();// = getCanonicalTime(getDayStartDate(new Date()));

   /**
    * Flag used to indicate the existence of the getDSTSavings
    * method that was introduced in Java 1.4.
    */
   const bool HAS_DST_SAVINGS = false;

   //static
   //{
   //   Class<TimeZone> tz = TimeZone.class;

   //   try
   //   {
   //      tz.getMethod("getDSTSavings", (Class[]) NULL);
   //      HAS_DST_SAVINGS = true;
   //   }

   //   catch (NoSuchMethodException ex)
   //   {
   //      HAS_DST_SAVINGS = false;
   //   }
   //}
};