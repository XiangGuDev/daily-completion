#pragma once

#include "..\Resource.h"

/**
 * Common code to read resource cost rate tables from MPP files.
 */
class CostRateTableFactory
{
public:
   /**
    * Creates a CostRateTable instance from a block of data.
    * 
    * @param resource parent resource
    * @param index cost rate table index
    * @param data data block
    */
	void process(Resource2Ptr resource, int index, ByteArrayPtr data);

private:
   /**
    * Converts an integer into a time format.
    * 
    * @param format integer format value
    * @return TimeUnit instance
    */
	TimeUnit getFormat(int format);
};