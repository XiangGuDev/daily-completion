#pragma once

#include "..\AvailabilityTable.h"

/**
 * Common code to read resource availability tables from MPP files.
 */
class AvailabilityFactory
{
public:
   /**
    * Populates a resource availability table.
    * 
    * @param table resource availability table
    * @param data file data
    */
	void process(AvailabilityTablePtr table, ByteArrayPtr data);

};