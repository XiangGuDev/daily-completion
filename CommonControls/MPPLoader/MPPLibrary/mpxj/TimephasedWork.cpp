#include "stdafx.h"
#include "TimephasedWork.h"

/**
* Default constructor.
*/
TimephasedWork::TimephasedWork()
{
    //super();
}

/**
* Copy constructor.
* 
* @param sourceItem item to copy
*/
TimephasedWork::TimephasedWork(TimephasedWorkPtr sourceItem)
{
    setStart(sourceItem->getStart());
    setFinish(sourceItem->getFinish());
	setModified(sourceItem->getModified());
	setTotalAmount(sourceItem->getTotalAmount());
	setAmountPerDay(sourceItem->getAmountPerDay());
}

/**
* Copy constructor, allowing scaling.
* 
* @param sourceItem item to copy
* @param totalFactor total amount factor
* @param perDayFactor per day factor
*/
TimephasedWork::TimephasedWork(TimephasedWorkPtr sourceItem, double totalFactor, double perDayFactor)
{
	setStart(sourceItem->getStart());
	setFinish(sourceItem->getFinish());
	setModified(sourceItem->getModified());
	setTotalAmount(Duration2::getInstance(
		sourceItem->getTotalAmount()->getDuration() * totalFactor, 
		sourceItem->getTotalAmount()->getUnits()));
	setAmountPerDay(Duration2::getInstance(
		sourceItem->getAmountPerDay()->getDuration() * perDayFactor,
		sourceItem->getAmountPerDay()->getUnits()));
}