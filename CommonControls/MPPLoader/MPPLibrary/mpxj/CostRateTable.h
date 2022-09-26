#pragma once

#include "CostRateTableEntry.h"

/** 
 * This class represents a resource's cost rate table.
 */
class CostRateTable : public List<CostRateTableEntryPtr>
{
public:
   /**
    * Retrieve the table entry valid for the supplied date. 
    * 
    * @param date required date
    * @return cost rate table entry 
    */
	CostRateTableEntryPtr getEntryByDate(DatePtr date);

   /**
    * Retrieve the index of the table entry valid for the supplied date. 
    * 
    * @param date required date
    * @return cost rate table entry index 
    */
	int getIndexByDate(DatePtr date);
};

typedef std::shared_ptr<CostRateTable> CostRateTablePtr;