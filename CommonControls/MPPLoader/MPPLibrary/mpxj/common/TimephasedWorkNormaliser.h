#pragma once

#include "..\TimephasedWork.h"
#include "..\ProjectCalendar.h"

/**
 * Classes implementing this interface are used to normalise timephased 
 * resource assignment data.
 */
class TimephasedWorkNormaliser
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
};

typedef std::shared_ptr<TimephasedWorkNormaliser> TimephasedWorkNormaliserPtr;