#pragma once

/**
 * This class represents the set of operators used to perform a test
 * between two or more operands.
 */
class TestOperator// implements MpxjEnum
{
public:
	enum
	{
		IS_ANY_VALUE = 0,
		IS_WITHIN = 1,
		IS_GREATER_THAN = 2,
		IS_LESS_THAN = 3,
		IS_GREATER_THAN_OR_EQUAL_TO = 4,
		IS_LESS_THAN_OR_EQUAL_TO = 5,
		EQUALS = 6,
		DOES_NOT_EQUAL = 7,
		CONTAINS = 8,
		IS_NOT_WITHIN = 9,
		DOES_NOT_CONTAIN = 10,
		CONTAINS_EXACTLY = 11,
		AND = 12,
		OR = 13
	};

	virtual bool evaluate(Object2* lhs, Object2* rhs) = 0;

	/**
	* Retrieve an instance of the enum based on its int value.
	*
	* @param type int type
	* @return enum instance
	*/
	static TestOperator* getInstance(int type);


	/**
	* Accessor method used to retrieve the numeric representation of the enum.
	*
	* @return int representation of the enum
	*/
	int getValue();

protected:
	/**
	* Private constructor.
	*
	* @param type int version of the enum
	*/
	TestOperator(int type);

	/**
	* This method is used to ensure that if a list of operand values has been
	* supplied, that a single operand is extracted.
	*
	* @param operand operand value
	* @return single operand value
	*/
	Object2* getSingleOperand(Object2* operand);

	/**
	* Determine if the supplied value falls within the specified range.
	*
	* @param lhs single value operand
	* @param rhs range operand
	* @return bool result
	*/
	bool evaluateWithin(Object2* lhs, Object2* rhs);

	/**
	* Implements a simple compare-to operation. Assumes that the LHS
	* operand implements the Comparable interface.
	*
	* @param lhs operand
	* @param rhs operand
	* @return bool result
	*/
	int evaluateCompareTo(Object2* lhs, Object2* rhs);

	/**
	* Assuming the supplied arguments are both Strings, this method
	* determines if rhs is contained within lhs. This test is case insensitive.
	*
	* @param lhs operand
	* @param rhs operand
	* @return bool result
	*/
	bool evaluateContains(Object2* lhs, Object2* rhs);

	/**
	* Assuming the supplied arguments are both Strings, this method
	* determines if rhs is contained within lhs. This test is case sensitive.
	*
	* @param lhs operand
	* @param rhs operand
	* @return bool result
	*/
	bool evaluateContainsExactly(Object2* lhs, Object2* rhs);

	/**
	* Internal representation of the enum int type.
	*/
	 int m_value;

	 static TestOperator* TYPE_VALUES[14];
};