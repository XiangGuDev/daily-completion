#include "stdafx.h"
#include "CostRateTableFactory.h"
#include "MPPUtility.h"

/**
* Creates a CostRateTable instance from a block of data.
* 
* @param resource parent resource
* @param index cost rate table index
* @param data data block
*/
void CostRateTableFactory::process(Resource2Ptr resource, int index, ByteArrayPtr data)
{
    CostRateTablePtr result (new CostRateTable());

    if (data)
    {
        for (size_t i = 16; i + 44 <= data->length(); i += 44)
        {
			RatePtr standardRate (new Rate(MPPUtility::getDouble(data.get(), i), TimeUnit::HOURS));
			TimeUnit standardRateFormat = getFormat(MPPUtility::getShort(data.get(), i + 8));
			RatePtr overtimeRate (new Rate(MPPUtility::getDouble(data.get(), i + 16), TimeUnit::HOURS));
			TimeUnit overtimeRateFormat = getFormat(MPPUtility::getShort(data.get(), i + 24));
			DoublePtr costPerUse(new Double(MPPUtility::getDouble(data.get(), i + 32) / 100.0));
			DatePtr endDate = MPPUtility::getTimestampFromTenths(data.get(), i + 40);
			CostRateTableEntryPtr entry (new CostRateTableEntry(standardRate, standardRateFormat, 
									overtimeRate, overtimeRateFormat, costPerUse, endDate));
			result->Add(entry);
        }
        Collections::sort_object_list(*result);
    }
    else
    {
        //
        // MS Project economises by not actually storing the first cost rate
        // table if it doesn't need to, so we take this into account here.
        //         
        if (index == 0)
        {
			RatePtr standardRate = resource->getStandardRate();
			RatePtr overtimeRate = resource->getOvertimeRate();
			NumberPtr costPerUse = resource->getCostPerUse();
			CostRateTableEntryPtr entry (new CostRateTableEntry(standardRate, standardRate->getUnits(), 
				overtimeRate, overtimeRate->getUnits(), costPerUse, CostRateTableEntry::getDEFAULT_ENTRY()->getEndDate()));
			result->Add(entry);
        }
        else
        {
			result->Add(CostRateTableEntry::getDEFAULT_ENTRY());
        }
    }

    resource->setCostRateTable(index, result);
}

/**
* Converts an integer into a time format.
* 
* @param format integer format value
* @return TimeUnit instance
*/
TimeUnit CostRateTableFactory::getFormat(int format)
{
    TimeUnit result;
    if (format == 0xFFFF)
    {
        result = TimeUnit::HOURS;
    }
    else
    {
        result = MPPUtility::getWorkTimeUnits(format);
    }
    return result;
}