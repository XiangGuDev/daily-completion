#include "stdafx.h"
#include "GenericCriteria.h"
#include "ProjectFile.h"
#include "FieldContainer.h"
#include "GenericCriteriaPrompt.h"
#include "common\DateHelper.h"
/**
* Constructor.
* 
* @param projectFile parent project file
*/
GenericCriteria::GenericCriteria(ProjectFile* projectFile)
	:m_leftValue(NULL),
	m_operator(NULL),
	m_symbolicValues(false),
	m_definedRightValues(NULL),
	m_workingRightValues(NULL)
{
    m_projectFile = projectFile;

	//m_definedRightValues[0] = m_definedRightValues[1] = NULL;
	//(*m_workingRightValues)[0] = (*m_workingRightValues)[1] = NULL;
}

/**
* Sets the LHS of the expression.
* 
* @param value LHS value
*/
void GenericCriteria::setLeftValue(FieldType* value)
{
    m_leftValue = value;
}

/**
* Retrieves the LHS of the expression.
* 
* @return LHS value
*/
FieldType* GenericCriteria::getLeftValue()
{
    return (m_leftValue);
}

/**
* Retrieve the operator used in the test.
* 
* @return test operator
*/
TestOperator* GenericCriteria::getOperator()
{
    return m_operator;
}

/**
* Set the operator used in the test.
* 
* @param operator test operator
*/
void GenericCriteria::setOperator(TestOperator* op)
{
    m_operator = op;
}

/**
* Add the value to list of values to be used as part of the
* evaluation of this indicator.
* 
* @param index position in the list
* @param value evaluation value
*/
void GenericCriteria::setRightValue(int index, Object2* value)
{
    m_definedRightValues[index] = value;

	if (dynamic_cast<FieldType*>(value))
    {
        m_symbolicValues = true;
    }
	else if (dynamic_cast<Duration*>(value))
    {
		Duration* value2 = dynamic_cast<Duration*>(value);

		if (value2->getUnits() != TimeUnit::HOURS)
		{
			ProjectHeader* header = m_projectFile->getProjectHeader();
			value = value2->convertUnits(value2->getDuration(), value2->getUnits(), TimeUnit::HOURS, header);
		}
    }

	m_workingRightValues[index] = value;
}

/**
* Retrieve the first value.
* 
* @param index position in the list
* @return first value
*/
Object* GenericCriteria::getValue(int index)
{
    return m_definedRightValues[index];
}

/**
* Evaluate the criteria and return a bool result.
* 
* @param container field container
* @param promptValues responses to prompts
* @return bool flag
*/
bool GenericCriteria::evaluate(FieldContainer* container, 
			Map<GenericCriteriaPrompt*, ObjectPtr>* promptValues)
{
    //
    // Retrieve the LHS value
    //
    FieldType* field = m_leftValue;
    ObjectPtr lhs;

    if (field == NULL)
    {
        lhs = NULL;
    }
    else
    {
        lhs = container->getCurrentValue(field);
        switch (field->getDataType())
        {
        case DT_DATE:
        {
            if (lhs != NULL)
            {
				lhs = DateHelper::getDayStartDate(std::dynamic_pointer_cast<Date>(lhs));
            }
            break;
        }

		case DT_DURATION:
        {
            if (lhs != NULL)
            {
                DurationPtr dur = std::dynamic_pointer_cast<Duration> (lhs);
                lhs = dur->convertUnits(TimeUnit::HOURS, m_projectFile->getProjectHeader().get());
            }
            else
            {
                lhs = Duration::getInstance(0, TimeUnit::HOURS);
            }
            break;
        }

		case DT_STRING:
        {
			lhs = lhs == NULL ? VariantPtr(new Variant(L"")) : lhs;
            break;
        }

        default:
        {
            break;
        }
        }
    }

    //
    // Retrieve the RHS values
    //
	ObjectArrayPtr rhs;
    if (m_symbolicValues == true)
    {
        rhs = processSymbolicValues(m_workingRightValues.get(), container, promptValues);
    }
    else
    {
        rhs = m_workingRightValues;
    }

    //
    // Evaluate
    //
    bool result = false;
    switch (m_operator->getValue())
    {
	case TestOperator::AND:
	case TestOperator::OR:
        {
			result = evaluateLogicalOperator(container, promptValues);
			break;
        }
    default:
		{
		result = m_operator->evaluate(lhs.get(), rhs.get());
			break;
		}
    }

    return result;
}

/**
* Evalutes AND and OR operators.
* 
* @param container data context
* @param promptValues responses to prompts
* @return operator result
*/
bool GenericCriteria::evaluateLogicalOperator(FieldContainer* container, 
						Map<GenericCriteriaPrompt*, ObjectPtr>* promptValues)
{
    bool result = false;

    if (m_criteriaList.GetCount() == 0)
    {
        result = true;
    }
    else
    {
		for (size_t i = 0; i < m_criteriaList.GetCount(); i ++)
        {
			GenericCriteriaPtr criteria = m_criteriaList[i];
			result = criteria->evaluate(container, promptValues);
			if ((m_operator->getValue() == TestOperator::AND && !result) || 
					(m_operator->getValue() == TestOperator::OR && result))
			{
				break;
			}
        }
    }

    return result;
}

/**
* This method is called to create a new list of values, converting from
* any symbolic values (represented by FieldType instances) to actual
* values retrieved from a Task or Resource instance.
* 
* @param oldValues list of old values containing symbolic items
* @param container Task or Resource instance
* @param promptValues response to prompts
* @return new list of actual values
*/
ObjectArrayPtr GenericCriteria::processSymbolicValues(ObjectPtrArray* oldValues,
	FieldContainer* container, Map<GenericCriteriaPrompt*, ObjectPtr>* promptValues)
{
	ObjectArrayPtr newValues (new ObjectPtrArray(2));

    for (size_t loop = 0; loop < oldValues->length(); loop++)
    {
        ObjectPtr value = (*oldValues)[loop];
        if (value == NULL)
        {
			continue;
        }

		if (dynamic_cast<FieldType*>(value.get()))
        {
			FieldType* type = dynamic_cast<FieldType*>(value.get());
			value = container->getCachedValue(type);

			switch (type->getDataType())
			{
				case DT_DATE:
				{
					if (value != NULL)
					{
						value = DateHelper::getDayStartDate(std::dynamic_pointer_cast<Date>(value));
					}
					break;
				}

				case DT_DURATION:
				{
					if (value != NULL && (std::dynamic_pointer_cast<Duration>(value))->getUnits() != TimeUnit::HOURS)
					{
						value = (std::dynamic_pointer_cast<Duration>(value))->convertUnits(
							TimeUnit::HOURS, m_projectFile->getProjectHeader().get());
					}
					else
					{
						value = Duration::getInstance(0, TimeUnit::HOURS);
					}
					break;
				}

				case DT_STRING:
				{
					if (value == NULL)
						value = VariantPtr(new Variant(L""));
					//value = value == NULL ? VariantPtr(new Variant(L"")) : value;
					break;
				}

				default:
				{
					break;
				}
			}
        }
        else
        {
			GenericCriteriaPrompt* prompt = std::dynamic_pointer_cast<GenericCriteriaPrompt>(value).get();
			if (prompt && promptValues != NULL)
			{
				//GenericCriteriaPrompt* prompt = dynamic_cast<GenericCriteriaPrompt*>(value);
				if (!promptValues->Lookup(prompt, value))
					throw std::bad_exception();
			}
        }
		(*newValues)[loop] = value;
    }
    return (newValues);
}

/**
* Retrieves the list of child criteria associated with the current criteria.
* 
* @return list of criteria
*/
List<GenericCriteriaPtr>* GenericCriteria::getCriteriaList()
{
    return &m_criteriaList;
}

/**
* Adds a an item to the list of child criteria.
* 
* @param criteria criteria item to add
*/
void GenericCriteria::addCriteria(GenericCriteriaPtr criteria)
{
    m_criteriaList.Add(criteria);
}

///**
//* {@inheritDoc}
//*/
//CString GenericCriteria::toString()
//{
//    StringBuilder sb = new StringBuilder();
//    sb.append("(");
//
//    switch (m_operator)
//    {
//        case AND:
//        case OR:
//        {
//        int index = 0;
//        for (GenericCriteria c : m_criteriaList)
//        {
//            sb.append(c);
//            ++index;
//            if (index < m_criteriaList.size())
//            {
//                sb.append(" ");
//                sb.append(m_operator);
//                sb.append(" ");
//            }
//        }
//        break;
//        }
//
//        default:
//        {
//        sb.append(m_leftValue);
//        sb.append(" ");
//        sb.append(m_operator);
//        sb.append(" ");
//        sb.append(m_definedRightValues[0]);
//        if (m_definedRightValues[1] != NULL)
//        {
//            sb.append(",");
//            sb.append(m_definedRightValues[1]);
//        }
//        }
//    }
//
//    sb.append(")");
//    return (sb.toString());
//}
