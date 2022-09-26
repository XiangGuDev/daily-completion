#pragma once

#include "MpxjEnum.h"
#include "common\NumberHelper.h"

/**
 * This class represents a currency rate per period of time (for example $10/h)
 * as found in an MPX file->
 */
class COMMONCONTROLS_API Rate : public Object2
{
public:
	/**
	 * This constructor builds an instance of this class from a currency
	 * amount and a time unit.
	 *
	 * @param amount currency amount
	 * @param time time units
	 */
	Rate(int amount, TimeUnit time);

	/**
	 * This constructor builds an instance of this class from a currency
	 * amount and a time unit.
	 *
	 * @param amount currency amount
	 * @param time time units
	 */
	Rate(double amount, TimeUnit time);

	/**
	 * Accessor method to retrieve the currency amount.
	 *
	 * @return amount component of the rate
	 */
	double getAmount();

	/**
	 * Accessor method to retrieve the time units.
	 *
	 * @return time component of the rate
	 */
	TimeUnit getUnits();

	///**
	// * {@inheritDoc}
	// */
	//virtual public bool equals(Object obj)
	//{
	//	bool result = false;
	//	if (obj instanceof Rate)
	//	{
	//		Rate rhs = (Rate)obj;
	//		result = amountComponentEquals(rhs) && m_units == rhs.m_units;
	//	}
	//	return result;
	//}

	/**
	 * Equality test for amount component of a Rate instance.
	 * Note that this does not take into account the units - use with care!
	 *
	 * @param rhs rate to compare
	 * @return true if amount components are equal, within the allowable delta
	 */
	bool amountComponentEquals(Rate rhs);

	///**
	// * {@inheritDoc}
	// */
	//virtual public int hashCode()
	//{
	//	return ((int)m_amount + m_units.hashCode());
	//}

	///**
	// * {@inheritDoc}
	// */
	//virtual public CString toString()
	//{
	//	return (m_amount + m_units.toString());
	//}

private:
	/**
	 * Rate amount.
	 */
	double m_amount;

	/**
	 * Time type.
	 */
	TimeUnit m_units;
};

typedef std::shared_ptr<Rate> RatePtr;