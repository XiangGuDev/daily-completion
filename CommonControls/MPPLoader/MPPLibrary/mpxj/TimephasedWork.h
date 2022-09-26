#pragma once

#include "Duration.h"
#include "TimephasedItem.h"

/**
 * Represents timephased work.
 */
class TimephasedWork : public TimephasedItem<DurationPtr>
{
public:
   /**
    * Default constructor.
    */
	TimephasedWork();

   /**
    * Copy constructor.
    * 
    * @param sourceItem item to copy
    */
	TimephasedWork(std::shared_ptr<TimephasedWork> sourceItem);

   /**
    * Copy constructor, allowing scaling.
    * 
    * @param sourceItem item to copy
    * @param totalFactor total amount factor
    * @param perDayFactor per day factor
    */
	TimephasedWork(std::shared_ptr<TimephasedWork> sourceItem, double totalFactor, double perDayFactor);
};

typedef std::shared_ptr<TimephasedWork> TimephasedWorkPtr;
typedef List<TimephasedWorkPtr> TimephasedWorkList;
typedef std::shared_ptr<TimephasedWorkList>TimephasedWorkListPtr;

typedef Array2<TimephasedWorkPtr> TimephasedWorkArray;
typedef std::shared_ptr<TimephasedWorkArray>TimephasedWorkArrayPtr;
