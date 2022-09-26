#pragma once

#include "stdafx.h"
#include "CostRateTableEntry.h"
#include "common\DateHelper.h"
//#include "GarbageCollector.h"

CostRateTableEntryPtr CostRateTableEntry::DEFAULT_ENTRY;

CostRateTableEntryPtr CostRateTableEntry::getDEFAULT_ENTRY()
{
	if (!DEFAULT_ENTRY)
	{
		DEFAULT_ENTRY.reset(new CostRateTableEntry());
		//GarbageCollector::add(new GcSharedPtr<CostRateTableEntry>(DEFAULT_ENTRY));
	}

	return DEFAULT_ENTRY;
}

/**
	* Constructor. Used to construct singleton default table entry.
	*/
CostRateTableEntry::CostRateTableEntry()
{
	m_endDate = DateHelper::getLAST_DATE();
	m_standardRate = RatePtr(new Rate(0, TimeUnit::HOURS));
	m_standardRateFormat = TimeUnit::HOURS;
	m_overtimeRate = m_standardRate;
	m_overtimeRateFormat = TimeUnit::HOURS;
	m_costPerUse = 0;
}

/**
	* Constructor.
	*
	* @param standardRate standard rate
	* @param standardRateFormat standard rate format
	* @param overtimeRate overtime rate
	* @param overtimeRateFormat overtime rate format
	* @param costPerUse cost per use
	* @param endDate end date
	*/
CostRateTableEntry::CostRateTableEntry(RatePtr standardRate, TimeUnit standardRateFormat, RatePtr overtimeRate,
	TimeUnit overtimeRateFormat, NumberPtr costPerUse, DatePtr endDate)
{
	m_endDate = endDate;
	m_standardRate = standardRate;
	m_standardRateFormat = standardRateFormat;
	m_overtimeRate = overtimeRate;
	m_overtimeRateFormat = overtimeRateFormat;
	m_costPerUse = costPerUse;
}

CostRateTableEntry::~CostRateTableEntry()
{
	//DEFAULT_ENTRY.reset();
}

/**
	* Retrieves the end date after which this table entry is not valid.
	*
	* @return end date
	*/
DatePtr CostRateTableEntry::getEndDate()
{
	return m_endDate;
}

/**
	* Retrieves the standard rate represented by this entry.
	*
	* @return standard rate
	*/
RatePtr CostRateTableEntry::getStandardRate()
{
	return m_standardRate;
}

/**
	* Retrieves the format used when displaying the standard rate.
	*
	* @return standard rate format
	*/
TimeUnit CostRateTableEntry::getStandardRateFormat()
{
	return m_standardRateFormat;
}

/**
	* Retrieves the overtime rate represented by this entry.
	*
	* @return overtime rate
	*/
RatePtr CostRateTableEntry::getOvertimeRate()
{
	return m_overtimeRate;
}

/**
	* Retrieves the format used when displaying the overtime rate.
	*
	* @return overtime rate format
	*/
TimeUnit CostRateTableEntry::getOvertimeRateFormat()
{
	return m_overtimeRateFormat;
}

/**
	* Retrieves the cost per use represented by this entry.
	*
	* @return per use rate
	*/
NumberPtr CostRateTableEntry::getCostPerUse()
{
	return m_costPerUse;
}

/**
	* {@inheritDoc}
	*/
int CostRateTableEntry::compareTo(CostRateTableEntry* o)
{
	return DateHelper::compare(m_endDate, o->m_endDate);
}
//
///**
//	* {@inheritDoc}
//	*/
//virtual CString toString()
//{
//	return "[CostRateTableEntry standardRate=" + m_standardRate + " overtimeRate=" + m_overtimeRate + " costPerUse=" + m_costPerUse + " endDate=" + m_endDate + "]";
//}