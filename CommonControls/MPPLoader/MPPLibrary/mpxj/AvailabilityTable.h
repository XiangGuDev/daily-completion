#pragma once

#include "Availability.h"

/** 
 * This class represents a resource's availability table.
 */
class AvailabilityTable : public List<AvailabilityPtr>
{
public:
   /**
    * Retrieve the table entry valid for the supplied date. 
    * 
    * @param date required date
    * @return cost rate table entry 
    */
	AvailabilityPtr getEntryByDate(DatePtr date);
};

typedef std::shared_ptr<AvailabilityTable> AvailabilityTablePtr;