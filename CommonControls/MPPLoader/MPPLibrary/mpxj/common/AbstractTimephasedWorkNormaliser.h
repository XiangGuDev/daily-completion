#pragma once

#include "TimephasedWorkNormaliser.h"

/**
 * Common implementation detail for normalisation.
 */
class AbstractTimephasedWorkNormaliser : public TimephasedWorkNormaliser
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
	virtual void normalise(ProjectCalendarPtr calendar, TimephasedWorkListPtr list) = 0;

protected:
   /**
    * Merges individual days together into time spans where the
    * same work is undertaken each day.
    * 
    * @param list assignment data
    */
	void mergeSameWork(TimephasedWorkListPtr list);

   /**
    * Converts assignment duration values from minutes to hours.
    * 
    * @param list assignment data
    */
	void convertToHours(TimephasedWorkListPtr list);
};