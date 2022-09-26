#pragma once

#include "TimephasedWork.h"

/**
 * Timephased data container.
 */
class TimephasedWorkContainer
{
public:
   /**
    * Retrieves the timephased data.
    * 
    * @return timephased data
    */
	virtual TimephasedWorkListPtr getData() = 0;

   /**
    * Indicates if any timephased data is present.
    * 
    * @return bool flag
    */
   virtual bool hasData() = 0;

};

typedef std::shared_ptr<TimephasedWorkContainer> TimephasedWorkContainerPtr;