#include "stdafx.h"
#include "CriteriaReader.h"
#include "MPPUtility.h"

/**
* Main entry point to read criteria data.
* 
* @param file parent project file
* @param data criteria data block
* @param dataOffset offset of the data start within the larger data block
* @param entryOffset offset of start node for walking the tree
* @param prompts optional list to hold prompts
* @param fields optional list of hold fields
* @param criteriaType optional array representing criteria types
* @return first node of the criteria
*/
GenericCriteriaPtr CriteriaReader::process(ProjectFile* file, ByteArrayPtr data, int dataOffset, int entryOffset,
	List<GenericCriteriaPromptPtr>* prompts, List<FieldType*>* fields, BoolArrayPtr criteriaType)
{
    m_file = file;
    m_prompts = prompts;
    m_fields = fields;
    m_criteriaType = criteriaType;
    m_dataOffset = dataOffset;
    if (m_criteriaType)
    {
        (*m_criteriaType)[0] = true;
		(*m_criteriaType)[1] = true;
    }

    m_criteriaBlockMap.RemoveAll();

    m_criteriaData = data;
    m_criteriaTextStart = MPPUtility::getShort(m_criteriaData.get(), m_dataOffset + getCriteriaTextStartOffset());

    //
    // Populate the map
    //
    int criteriaStartOffset = getCriteriaStartOffset();
    int criteriaBlockSize = getCriteriaBlockSize();

    //System.out.println();
    //System.out.println(MPPUtility.hexdump(data, dataOffset, criteriaStartOffset, false));

    if ((int) m_criteriaData->length() <= m_criteriaTextStart)
    {
		return GenericCriteriaPtr(); // bad data
    }

    while (criteriaStartOffset + criteriaBlockSize <= m_criteriaTextStart)
    {
        ByteArrayPtr block (new ByteArray(criteriaBlockSize));
		memcpy(block->get(), &(*m_criteriaData)[m_dataOffset + criteriaStartOffset], criteriaBlockSize);
        //System.arraycopy(m_criteriaData, m_dataOffset + criteriaStartOffset, block, 0, criteriaBlockSize);

		m_criteriaBlockMap.put(criteriaStartOffset, block);
        //System.out.println(Integer.toHexString(criteriaStartOffset) + ": " + MPPUtility.hexdump(block, false));
        criteriaStartOffset += criteriaBlockSize;
    }

    if (entryOffset == -1)
    {
        entryOffset = getCriteriaStartOffset();
    }

	List<GenericCriteriaPtr> list;// = new LinkedList<GenericCriteria>();
	ByteArrayPtr block;
	if (m_criteriaBlockMap.Lookup(entryOffset), block)
	{
		processBlock(&list, block);
	}
    
    GenericCriteriaPtr criteria;
    if (!list.isEmpty())
    {
        criteria = list.GetAt(0);
    }
    return criteria;
}

/**
* Process a single criteria block.
* 
* @param list parent criteria list
* @param block current block 
*/
void CriteriaReader::processBlock(List<GenericCriteriaPtr>* list, ByteArrayPtr block)
{
	if (!block)
		return;

    if (MPPUtility::getShort(block.get(), 0) > 0x3E6)
    {
		addCriteria(list, block);
    }
    else
    {
        switch ((*block)[0])
        {
            case (byte) 0x0B:
            {
                processBlock(list, getChildBlock(block));
                break;
            }

            case (byte) 0x06:
            {
                processBlock(list, getListNextBlock(block));
                break;
            }

            case (byte) 0xED: // EQUALS
            {
                addCriteria(list, block);
                break;
            }

            case (byte) 0x19: // AND
            case (byte) 0x1B:
            {
				addBlock(list, block, TestOperator::getInstance(TestOperator::AND));
                break;
            }

            case (byte) 0x1A: // OR
            case (byte) 0x1C:
            {
				addBlock(list, block, TestOperator::getInstance(TestOperator::OR));
                break;
            }
        }
    }
}

/**
* Adds a basic LHS OPERATOR RHS block.
* 
* @param list parent criteria list
* @param block current block
*/
void CriteriaReader::addCriteria(List<GenericCriteriaPtr>* list, ByteArrayPtr block)
{
    ByteArrayPtr leftBlock = getChildBlock(block);
    ByteArrayPtr rightBlock1 = getListNextBlock(leftBlock);
    ByteArrayPtr rightBlock2 = getListNextBlock(rightBlock1);
    TestOperator* oper = TestOperator::getInstance(MPPUtility::getShort(block.get(), 0) - 0x3E7);
    FieldType* leftValue = getFieldType(leftBlock);
    ObjectPtr rightValue1 = getValue(leftValue, rightBlock1);
	ObjectPtr rightValue2 = !rightBlock2 ? ObjectPtr() : getValue(leftValue, rightBlock2);

    GenericCriteriaPtr criteria  (new GenericCriteria(m_file));
    criteria->setLeftValue(leftValue);
	criteria->setOperator(oper);
	criteria->setRightValue(0, rightValue1);
	criteria->setRightValue(1, rightValue2);
    list->Add(criteria);

    if (m_criteriaType)
    {
		(*m_criteriaType)[0] = !!dynamic_cast<TaskFieldClass*> (leftValue);// instanceof TaskField;
        (*m_criteriaType)[1] = !(*m_criteriaType)[0];
    }

    if (m_fields)
    {
        m_fields->Add(leftValue);
    }

    processBlock(list, getListNextBlock(block));
}

/**
* Adds a logical operator block.
* 
* @param list parent criteria list
* @param block current block
* @param operator logical operator represented by this block
*/
void CriteriaReader::addBlock(List<GenericCriteriaPtr>* list, ByteArrayPtr block, TestOperator* oper)
{
    GenericCriteriaPtr result (new GenericCriteria(m_file));
    result->setOperator(oper);
    list->Add(result);
    processBlock(result->getCriteriaList(), getChildBlock(block));
    processBlock(list, getListNextBlock(block));
}

/**
* Retrieves the value component of a criteria expression.
* 
* @param field field type
* @param block block data
* @return field value
*/
ObjectPtr CriteriaReader::getValue(FieldType* field, ByteArrayPtr block)
{
    ObjectPtr result;

    switch ((*block)[0])
    {
        case 0x07: // Field
        {
        result = getFieldType(block);
        break;
        }

        case 0x01: // Constant value
        {
        result = getConstantValue(field, block);
        break;
        }

        case 0x00: // Prompt
        {
        result = getPromptValue(field, block);
        break;
        }
    }

    return result;
}

/**
* Retrieves a constant value.
* 
* @param type field type
* @param block criteria data block
* @return constant value
*/
ObjectPtr CriteriaReader::getConstantValue(FieldType* type, ByteArrayPtr block)
{
    ObjectPtr value;

    switch (type->getDataType())
    {
        case DT_DURATION:
        {
		value = MPPUtility::getAdjustedDuration(m_file, MPPUtility::getInt(block.get(), getValueOffset()), 
		MPPUtility::getDurationTimeUnits(MPPUtility::getShort(block.get(), getTimeUnitsOffset())));
        break;
        }

		case DT_NUMERIC:
        {
		value.reset (new Double(MPPUtility::getDouble(block.get(), getValueOffset())));
        break;
        }

		case DT_PERCENTAGE:
        {
		value.reset(new Double(MPPUtility::getShort(block.get(), getValueOffset())));
        break;
        }

		case DT_CURRENCY:
        {
		value.reset(new Double(MPPUtility::getDouble(block.get(), getValueOffset()) / 100));
        break;
        }

		case DT_STRING:
        {
		int textOffset = getTextOffset(block);
        value.reset (new Variant (MPPUtility::getUnicodeString(m_criteriaData.get(), 
							m_dataOffset + m_criteriaTextStart + textOffset)));
        break;
        }

		case DT_BOOLEAN:
        {
		int intValue = MPPUtility::getShort(block.get(), getValueOffset());
        value.reset (intValue == 1 ? new Boolean(true) : new Boolean(false));
        break;
        }

		case DT_DATE:
        {
		value = MPPUtility::getTimestamp(block.get(), getValueOffset());
        break;
        }

        default:
        {
        value.reset();
        break;
        }
    }

    return value;
}

/**
* Retrieves a prompt value.
* 
* @param field field type
* @param block criteria data block
* @return prompt value
*/
GenericCriteriaPromptPtr CriteriaReader::getPromptValue(FieldType* field, ByteArrayPtr block)
{
    int textOffset = getPromptOffset(block);
    CString value = MPPUtility::getUnicodeString(m_criteriaData.get(), m_criteriaTextStart + textOffset);
    GenericCriteriaPromptPtr prompt (new GenericCriteriaPrompt(field->getDataType(), value));
    if (m_prompts)
    {
        m_prompts->Add(prompt);
    }
    return prompt;
}