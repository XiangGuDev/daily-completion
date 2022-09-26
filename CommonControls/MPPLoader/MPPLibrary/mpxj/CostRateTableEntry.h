#pragma once

#include "Rate.h"

/**
 * This class represents a row from a resource's cost rate table.
 * Note that MS Project always represents costs as an hourly rate,
 * it holds an additional field to indicate the format used when
 * displaying the rate.
 */
class CostRateTableEntry : public ComparableT<CostRateTableEntry>, public Object2
{
private:
	/**
	 * Constructor. Used to construct singleton default table entry.
	 */
	CostRateTableEntry();

public:
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
	CostRateTableEntry(RatePtr standardRate, TimeUnit standardRateFormat, RatePtr overtimeRate,
		TimeUnit overtimeRateFormat, NumberPtr costPerUse, DatePtr endDate);

	~CostRateTableEntry();
	/**
	 * Retrieves the end date after which this table entry is not valid.
	 *
	 * @return end date
	 */
	DatePtr getEndDate();

	/**
	 * Retrieves the standard rate represented by this entry.
	 *
	 * @return standard rate
	 */
	RatePtr getStandardRate();

	/**
	 * Retrieves the format used when displaying the standard rate.
	 *
	 * @return standard rate format
	 */
	TimeUnit getStandardRateFormat();

	/**
	 * Retrieves the overtime rate represented by this entry.
	 *
	 * @return overtime rate
	 */
	RatePtr getOvertimeRate();

	/**
	 * Retrieves the format used when displaying the overtime rate.
	 *
	 * @return overtime rate format
	 */
	TimeUnit getOvertimeRateFormat();

	/**
	 * Retrieves the cost per use represented by this entry.
	 *
	 * @return per use rate
	 */
	NumberPtr getCostPerUse();

	/**
	 * {@inheritDoc}
	 */
	virtual int compareTo(CostRateTableEntry* o);

	///**
	// * {@inheritDoc}
	// */
	//virtual CString toString();


	static std::shared_ptr<CostRateTableEntry> getDEFAULT_ENTRY();

private:
	static std::shared_ptr<CostRateTableEntry> DEFAULT_ENTRY;// = new CostRateTableEntry();

private:
	DatePtr m_endDate;
	RatePtr m_standardRate;
	TimeUnit m_standardRateFormat;
	RatePtr m_overtimeRate;
	TimeUnit m_overtimeRateFormat;
	NumberPtr m_costPerUse;
};

typedef std::shared_ptr<CostRateTableEntry> CostRateTableEntryPtr;