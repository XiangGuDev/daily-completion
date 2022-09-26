#include "stdafx.h"
#include "AvailabilityTable.h"

/**
* Retrieve the table entry valid for the supplied date. 
* 
* @param date required date
* @return cost rate table entry 
*/
AvailabilityPtr AvailabilityTable::getEntryByDate(DatePtr date)
{
    AvailabilityPtr result;

	for (size_t i = 0; i < GetCount(); i ++)
    {
		AvailabilityPtr entry = GetAt(i);
        DateRange* range = entry->getRange();
        int comparisonResult = range->compareTo(date);
        if (comparisonResult >= 0)
        {
			if (comparisonResult == 0)
			{
				result = entry;
			}
			break;
        }
    }

    return result;
}