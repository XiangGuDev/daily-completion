#pragma once

#include "TimephasedCost.h"

/**
 * Timephased data container.
 */
class TimephasedCostContainer
{
public:
   /**
    * Retrieves the timephased data.
    * 
    * @return timephased data
    */
	virtual TimephasedCostListPtr getData() = 0;

   /**
    * Indicates if any timephased data is present.
    * 
    * @return bool flag
    */
	virtual bool hasData() = 0;

};

typedef std::shared_ptr<TimephasedCostContainer> TimephasedCostContainerPtr;