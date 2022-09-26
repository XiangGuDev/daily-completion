#include "stdafx.h"
#include "AvailabilityFactory.h"
#include "MPPUtility.h"
#include "..\java\Calendar.h"
#include "..\common\NumberHelper.h"

/**
* Populates a resource availability table.
* 
* @param table resource availability table
* @param data file data
*/
void AvailabilityFactory::process(AvailabilityTablePtr table, ByteArrayPtr data)
{
	if (!data) return;

    CalendarPtr cal = Calendar::getInstance();
    int items = MPPUtility::getShort(data.get(), 0);
    int offset = 12;

    for (int loop = 0; loop < items; loop++)
    {
		double unitsValue = MPPUtility::getDouble(data.get(), offset + 4);
		if (unitsValue != 0)
		{
			DatePtr startDate = MPPUtility::getTimestampFromTenths(data.get(), offset);
			DatePtr endDate = MPPUtility::getTimestampFromTenths(data.get(), offset + 20);
			cal->setTime(endDate);
			cal->add(Calendar::MINUTE, -1);
			endDate = cal->getTime();
			DoublePtr units (new Double(unitsValue / 100));
			AvailabilityPtr item (new Availability(startDate, endDate, units));
			table->Add(item);
		}
		offset += 20;
    }

    Collections::sort_object_list(*table);

}