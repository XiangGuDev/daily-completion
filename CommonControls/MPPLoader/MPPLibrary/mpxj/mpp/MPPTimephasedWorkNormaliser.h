#pragma once

#include "MPPAbstractTimephasedWorkNormaliser.h"

/**
 * Normalise timephased resource assignment data from an MPP file. 
 */
class MPPTimephasedWorkNormaliser : public MPPAbstractTimephasedWorkNormaliser
{
protected:
   /**
    * This method merges together assignment data for the same day.
    * 
    * @param calendar current calendar
    * @param list assignment data
    */
	virtual void mergeSameDay(ProjectCalendarPtr calendar, TimephasedWorkListPtr list);
};

typedef std::shared_ptr<MPPTimephasedWorkNormaliser> MPPTimephasedWorkNormaliserPtr;