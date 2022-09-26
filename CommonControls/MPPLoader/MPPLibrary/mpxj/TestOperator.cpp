#include "stdafx.h"
#include "TestOperator.h"


TestOperator* TestOperator::TYPE_VALUES[14] = { 0 };

class IS_ANY_VALUE_0 : public TestOperator
{
public:
	IS_ANY_VALUE_0() :TestOperator(IS_ANY_VALUE)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (true);
	}
};

class IS_WITHIN_1 : public TestOperator
{
public:
	IS_WITHIN_1() :TestOperator(IS_WITHIN)
	{
	}

	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (evaluateWithin(lhs, rhs));
	}
};

class IS_GREATER_THAN_2 : public TestOperator
{
public:
	IS_GREATER_THAN_2() :TestOperator(IS_GREATER_THAN)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (evaluateCompareTo(lhs, rhs) > 0);
	}
};

class IS_LESS_THAN_3 : public TestOperator
{
public:
	IS_LESS_THAN_3() :TestOperator(IS_LESS_THAN)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (evaluateCompareTo(lhs, rhs) < 0);
	}
};

class IS_GREATER_THAN_OR_EQUAL_TO_4 : public TestOperator
{
public:
	IS_GREATER_THAN_OR_EQUAL_TO_4() :TestOperator(IS_GREATER_THAN_OR_EQUAL_TO)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (evaluateCompareTo(lhs, rhs) >= 0);
	}
};

class IS_LESS_THAN_OR_EQUAL_TO_5 : public TestOperator
{
public:
	IS_LESS_THAN_OR_EQUAL_TO_5() :TestOperator(IS_LESS_THAN_OR_EQUAL_TO)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (evaluateCompareTo(lhs, rhs) <= 0);
	}
};

class EQUALS_6 : public TestOperator
{
public:
	EQUALS_6() :TestOperator(EQUALS)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		bool result = false;

		if (lhs == NULL)
		{
			result = (getSingleOperand(rhs) == NULL);
		}
		else
		{
			result = lhs->equals(getSingleOperand(rhs));
		}
		return (result);
	}
};

class DOES_NOT_EQUAL_7 : public TestOperator
{
public:
	DOES_NOT_EQUAL_7() :TestOperator(DOES_NOT_EQUAL)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		bool result=false;
		if (lhs == NULL)
		{
			result = (getSingleOperand(rhs) != NULL);
		}
		else
		{
			result = !lhs->equals(getSingleOperand(rhs));
		}
		return (result);
	}
};

class CONTAINS_8 : public TestOperator
{
public:
	CONTAINS_8() :TestOperator(CONTAINS)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (evaluateContains(lhs, rhs));
	}
};

class IS_NOT_WITHIN_9 : public TestOperator
{
public:
	IS_NOT_WITHIN_9() :TestOperator(IS_NOT_WITHIN)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (!evaluateWithin(lhs, rhs));
	}
};

class DOES_NOT_CONTAIN_10 : public TestOperator
{
public:
	DOES_NOT_CONTAIN_10() :TestOperator(DOES_NOT_CONTAIN)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (!evaluateContains(lhs, rhs));
	}
};

class CONTAINS_EXACTLY_11 : public TestOperator
{
public:
	CONTAINS_EXACTLY_11() :TestOperator(CONTAINS_EXACTLY)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		return (evaluateContainsExactly(lhs, rhs));
	}
};

class AND_12 : public TestOperator // Extension used by MPXJ, Not MS Project
{
public:
	AND_12() :TestOperator(AND)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		throw std::bad_exception();
	}
};

class OR_13 : public TestOperator // Extension used by MPXJ, Not MS Project
{
public:
	OR_13() :TestOperator(OR)
	{
	}
	/**
	* {@inheritDoc}
	*/
	virtual bool evaluate(Object2* lhs, Object2* rhs)
	{
		throw std::bad_exception();
	}
};

/**
* Private constructor.
*
* @param type int version of the enum
*/
TestOperator::TestOperator(int type)
{
	m_value = type;
}

/**
* Retrieve an instance of the enum based on its int value.
*
* @param type int type
* @return enum instance
*/
TestOperator* TestOperator::getInstance(int type)
{
	if (TYPE_VALUES[type] == NULL)
	{
		TYPE_VALUES[IS_ANY_VALUE] = new IS_ANY_VALUE_0();
		TYPE_VALUES[IS_WITHIN] = new IS_WITHIN_1();
		TYPE_VALUES[IS_GREATER_THAN] = new IS_GREATER_THAN_2();
		TYPE_VALUES[IS_LESS_THAN] = new IS_LESS_THAN_3();
		TYPE_VALUES[IS_GREATER_THAN_OR_EQUAL_TO] = new IS_GREATER_THAN_OR_EQUAL_TO_4();
		TYPE_VALUES[IS_LESS_THAN_OR_EQUAL_TO] = new IS_LESS_THAN_OR_EQUAL_TO_5();
		TYPE_VALUES[EQUALS] = new EQUALS_6();
		TYPE_VALUES[DOES_NOT_EQUAL] = new DOES_NOT_EQUAL_7();
		TYPE_VALUES[CONTAINS] = new CONTAINS_8();
		TYPE_VALUES[IS_NOT_WITHIN] = new IS_NOT_WITHIN_9();
		TYPE_VALUES[DOES_NOT_CONTAIN] = new DOES_NOT_CONTAIN_10();
		TYPE_VALUES[CONTAINS_EXACTLY] = new CONTAINS_EXACTLY_11();
		TYPE_VALUES[AND] = new AND_12();
		TYPE_VALUES[OR] = new OR_13();
	}
	return (TYPE_VALUES[type]);
}

/**
* Accessor method used to retrieve the numeric representation of the enum.
*
* @return int representation of the enum
*/
int TestOperator::getValue()
{
	return (m_value);
}

/**
* This method is used to ensure that if a list of operand values has been
* supplied, that a single operand is extracted.
* 
* @param operand operand value
* @return single operand value
*/
Object2* TestOperator::getSingleOperand(Object2* operand)
{
	ObjectPtrArray* list = dynamic_cast<ObjectPtrArray*>(operand);
	if (dynamic_cast<ObjectPtrArray*>(operand))
    {
		operand = (*list)[0].get();
    }

    return (operand);
}

/**
* Determine if the supplied value falls within the specified range.
* 
* @param lhs single value operand
* @param rhs range operand
* @return bool result
*/
bool TestOperator::evaluateWithin(Object2* lhs, Object2* rhs)
{
    bool result = false;

	ObjectPtrArray* rhsList = dynamic_cast<ObjectPtrArray*>(rhs);
	if (rhsList)
    {
        if (lhs != NULL)
        {
			if ((*rhsList)[0] != NULL && (*rhsList)[1] != NULL)
			{
				Comparable* lhsComparable = dynamic_cast<Comparable*> (lhs);
				if (lhsComparable == NULL) 
					throw std::bad_exception();
				// Project also tries with the values flipped
				result = (lhsComparable->compareTo((*rhsList)[0].get()) >= 0 &&
					lhsComparable->compareTo((*rhsList)[1].get()) <= 0) ||
					(lhsComparable->compareTo((*rhsList)[0].get()) <= 0 &&
					lhsComparable->compareTo((*rhsList)[1].get()) >= 0);
			}
        }
        else
        {
			// Project also respects NULL equality (e.g. NA dates)
			result = (*rhsList)[0] == NULL || (*rhsList)[1] == NULL;
        }
    }
    return (result);
}

/**
* Implements a simple compare-to operation. Assumes that the LHS
* operand implements the Comparable interface.
* 
* @param lhs operand
* @param rhs operand
* @return bool result
*/
int TestOperator::evaluateCompareTo(Object2* lhs, Object2* rhs)
{
    int result = 0;

    rhs = getSingleOperand(rhs);

    if (lhs == NULL || rhs == NULL)
    {
        if (lhs == rhs)
        {
			result = 0;
        }
        else
        {
			if (lhs == NULL)
			{
				result = 1;
			}
			else
			{
				result = -1;
			}
        }
    }
    else
    {
		Comparable* comp = dynamic_cast<Comparable*>(lhs);
		if (comp == NULL) 
			throw std::bad_exception();
		result = comp->compareTo(rhs);
    }

    return (result);
}

/**
* Assuming the supplied arguments are both Strings, this method
* determines if rhs is contained within lhs. This test is case insensitive.
* 
* @param lhs operand
* @param rhs operand
* @return bool result
*/
bool TestOperator::evaluateContains(Object2* lhs, Object2* rhs)
{
    bool result = false;

    rhs = getSingleOperand(rhs);

	Variant* lhs_t = dynamic_cast<Variant*>(lhs);
	Variant* rhs_t = dynamic_cast<Variant*>(rhs);
	if (lhs_t && rhs_t)
    {
		CString ltext = lhs_t->toString();
		CString rtext = rhs_t->toString();

		result = ltext.MakeUpper().Find(rtext.MakeUpper()) != -1;
    }

    return (result);
}

/**
* Assuming the supplied arguments are both Strings, this method
* determines if rhs is contained within lhs. This test is case sensitive.
* 
* @param lhs operand
* @param rhs operand
* @return bool result
*/
bool TestOperator::evaluateContainsExactly(Object2* lhs, Object2* rhs)
{
    bool result = false;

    rhs = getSingleOperand(rhs);

	Variant* lhs_t = dynamic_cast<Variant*>(lhs);
	Variant* rhs_t = dynamic_cast<Variant*>(rhs);
	if (lhs_t && rhs_t)
	{
		CString ltext = lhs_t->toString();
		CString rtext = rhs_t->toString();

		result = ltext.Find(rtext) != -1;
    }

    return (result);
}