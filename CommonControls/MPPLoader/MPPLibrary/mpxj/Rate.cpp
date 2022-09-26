#include "stdafx.h"
#include "Rate.h"

/**
	* This constructor builds an instance of this class from a currency
	* amount and a time unit.
	*
	* @param amount currency amount
	* @param time time units
	*/
Rate::Rate(int amount, TimeUnit time)
{
	m_amount = amount;

	m_units = time;
}

/**
	* This constructor builds an instance of this class from a currency
	* amount and a time unit.
	*
	* @param amount currency amount
	* @param time time units
	*/
Rate::Rate(double amount, TimeUnit time)
{
	m_amount = amount;
	m_units = time;
}

/**
	* Accessor method to retrieve the currency amount.
	*
	* @return amount component of the rate
	*/
double Rate::getAmount()
{
	return (m_amount);
}

/**
	* Accessor method to retrieve the time units.
	*
	* @return time component of the rate
	*/
TimeUnit Rate::getUnits()
{
	return (m_units);
}

/**
	* Equality test for amount component of a Rate instance.
	* Note that this does not take into account the units - use with care!
	*
	* @param rhs rate to compare
	* @return true if amount components are equal, within the allowable delta
	*/
bool Rate::amountComponentEquals(Rate rhs)
{
	return NumberHelper::equals(m_amount, rhs.m_amount, 0.00001);
}
