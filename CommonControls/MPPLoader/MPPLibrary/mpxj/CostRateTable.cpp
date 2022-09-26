#include "stdafx.h"
#include "CostRateTable.h"
#include "common\DateHelper.h"

/**
* Retrieve the table entry valid for the supplied date. 
* 
* @param date required date
* @return cost rate table entry 
*/
CostRateTableEntryPtr CostRateTable::getEntryByDate(DatePtr date)
{
	CostRateTableEntryPtr result;

	for (size_t i = 0; i < GetCount(); i ++)
    {
		CostRateTableEntryPtr entry = GetAt(i);
        if (DateHelper::compare(date, entry->getEndDate()) < 0)
        {
			result = entry;
			break;
        }
    }

    return result;
}

/**
* Retrieve the index of the table entry valid for the supplied date. 
* 
* @param date required date
* @return cost rate table entry index 
*/
int CostRateTable::getIndexByDate(DatePtr date)
{
    int result = -1;

	for (size_t i = 0; i < GetCount(); i++)
	{
		CostRateTableEntryPtr entry = GetAt(i);
        if (DateHelper::compare(date, entry->getEndDate()) < 0)
        {
			result = (int) i;
			break;
        }
    }

    return result;
}