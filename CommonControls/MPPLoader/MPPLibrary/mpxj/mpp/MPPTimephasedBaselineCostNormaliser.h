#pragma once

#include "..\common\TimephasedCostNormaliser.h"

/**
 * Common implementation detail for normalisation.
 */
class MPPTimephasedBaselineCostNormaliser : public TimephasedCostNormaliser
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
	virtual void normalise(ProjectCalendarPtr calendar, TimephasedCostListPtr list);

private:
   /**
    * This method breaks down spans of time into individual days.
    * 
    * @param calendar current project calendar
    * @param list list of assignment data
    */
	 void splitDays(ProjectCalendarPtr calendar, TimephasedCostListPtr list);

   /**
    * This method splits the first day off of a time span.
    * 
    * @param calendar current calendar
    * @param assignment timephased assignment span
    * @return first day and remainder assignments
    */
	TimephasedCostArrayPtr splitFirstDay(ProjectCalendarPtr calendar, TimephasedCostPtr assignment);

   /**
    * This method merges together assignment data for the same day.
    * 
    * @param list assignment data
    */
	void mergeSameDay(TimephasedCostListPtr list);

protected:
   /**
    * This method merges together assignment data for the same cost.
    * 
    * @param list assignment data
    */
	 void mergeSameCost(TimephasedCostListPtr list);

   /*
   private void dumpList(LinkedList<TimephasedCost> list)
   {
      System.out.println();
      for (TimephasedCost assignment : list)
      {
         System.out.println(assignment);
      }
   }
   */

};

typedef std::shared_ptr<MPPTimephasedBaselineCostNormaliser> MPPTimephasedBaselineCostNormaliserPtr;