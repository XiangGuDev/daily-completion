#pragma once

#include "..\common\AbstractTimephasedWorkNormaliser.h"

class Duration2;

/**
 * Normalise timephased resource assignment data from an MSPDI file.
 */
class MSPDITimephasedWorkNormaliser : public AbstractTimephasedWorkNormaliser
{
public:
   /**
    * This method converts the internal representation of timephased 
    * resource assignment data used by MS Project into a standardised
    * format to make it easy to work with. 
    * 
    * @param calendar current calendar
    * @param list list of assignment data
    */
	virtual void normalise(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);

   /*
      private void dumpList(String label, LinkedList<TimephasedWork> list)
      {
         System.out.println(label);
         for (TimephasedWork assignment : list)
         {
            System.out.println(assignment);
         }
      }
      */

private:
   /**
    * This method breaks down spans of time into individual days.
    * 
    * @param calendar current project calendar
    * @param list list of assignment data
    */
	void splitDays(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);

   /**
    * This method splits the first day off of a time span.
    * 
    * @param calendar current calendar
    * @param assignment timephased assignment span
    * @return first day and remainder assignments
    */
	TimephasedWorkListPtr splitFirstDay(ProjectCalendarPtr calendar, TimephasedWorkPtr assignment);

   /**
    * This method merges together assignment data for the same day.
    * 
    * @param calendar current calendar
    * @param list assignment data
    */
	void mergeSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);

   /**
    * Ensures that the start and end dates for ranges fit within the
    * working times for a given day.
    * 
    * @param calendar current calendar
    * @param list assignment data
    */
	void validateSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);
};

typedef std::shared_ptr<MSPDITimephasedWorkNormaliser> MSPDITimephasedWorkNormaliserPtr;