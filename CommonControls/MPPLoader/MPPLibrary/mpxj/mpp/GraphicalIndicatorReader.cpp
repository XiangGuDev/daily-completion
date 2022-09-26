#include "stdafx.h"
#include "GraphicalIndicatorReader.h"
#include "..\ProjectFile.h"
#include "MPPUtility.h"
#include "..\common\MPPTaskField.h"
#include "..\common\MPPResourceField.h"

/**
* The main entry point for processing graphical indicator definitions.
* 
* @param file parent project file
* @param props properties data
*/
void GraphicalIndicatorReader::process(ProjectFile* file, PropsPtr props)
{
    m_file = file;
    m_data = props->getByteArray(Props::TASK_FIELD_ATTRIBUTES);

    if (m_data)
    {
        int columnsCount = MPPUtility::getInt(m_data.get(), 4);
        m_headerOffset = 8;
        for (int loop = 0; loop < columnsCount; loop++)
        {
			processColumns();
        }
    }
}

/**
* Processes graphical indicator definitions for each column.
*/
void GraphicalIndicatorReader::processColumns()
{
	int fieldType = MPPUtility::getShort(m_data.get(), m_headerOffset);
    m_headerOffset += 2;

    // unknown bytes
    m_headerOffset += 1;

	int entityType = MPPUtility::getByte(m_data.get(), m_headerOffset);
    m_headerOffset += 1;

	m_dataOffset = MPPUtility::getInt(m_data.get(), m_headerOffset);
    m_headerOffset += 4;

    FieldType* type  = NULL;
    switch (entityType)
    {
        case 0x0B:
        {
			type = TaskFieldClass::getInstance (MPPTaskField::getInstance(fieldType));
			break;
        }

        case 0x0C:
        {
			type = TaskFieldClass::getInstance(MPPResourceField::getInstance(fieldType));
			break;
        }
    }

    //System.out.println("Header: " + type);
    //System.out.println(MPPUtility.hexdump(m_data, m_dataOffset, 36, false, 16, ""));

    GraphicalIndicatorPtr indicator (new GraphicalIndicator());
    indicator->setFieldType(type);
    int flags = (*m_data)[m_dataOffset];
	indicator->setProjectSummaryInheritsFromSummaryRows((flags & 0x08) != 0);
	indicator->setSummaryRowsInheritFromNonSummaryRows((flags & 0x04) != 0);
	indicator->setDisplayGraphicalIndicators((flags & 0x02) != 0);
	indicator->setShowDataValuesInToolTips((flags & 0x01) != 0);
    m_dataOffset += 20;

    int nonSummaryRowOffset = MPPUtility::getInt(m_data.get(), m_dataOffset) - 36;
    m_dataOffset += 4;

	int summaryRowOffset = MPPUtility::getInt(m_data.get(), m_dataOffset) - 36;
    m_dataOffset += 4;

	int projectSummaryOffset = MPPUtility::getInt(m_data.get(), m_dataOffset) - 36;
    m_dataOffset += 4;

	int dataSize = MPPUtility::getInt(m_data.get(), m_dataOffset) - 36;
    m_dataOffset += 4;

    //System.out.println("Data");
    //System.out.println(MPPUtility.hexdump(m_data, m_dataOffset, dataSize, false, 16, ""));

    int maxNonSummaryRowOffset = m_dataOffset + summaryRowOffset;
    int maxSummaryRowOffset = m_dataOffset + projectSummaryOffset;
    int maxProjectSummaryOffset = m_dataOffset + dataSize;

    m_dataOffset += nonSummaryRowOffset;

    while (m_dataOffset + 2 < maxNonSummaryRowOffset)
    {
        indicator->addNonSummaryRowCriteria(processCriteria(type));
    }

    while (m_dataOffset + 2 < maxSummaryRowOffset)
    {
		indicator->addSummaryRowCriteria(processCriteria(type));
    }

    while (m_dataOffset + 2 < maxProjectSummaryOffset)
    {
		indicator->addProjectSummaryCriteria(processCriteria(type));
    }

	m_file->addGraphicalIndicator(type, indicator);
}

/**
* Process the graphical indicator criteria for a single column.
* 
* @param type field type
* @return indicator criteria data
*/
GraphicalIndicatorCriteriaPtr GraphicalIndicatorReader::processCriteria(FieldType* type)
{
    GraphicalIndicatorCriteriaPtr criteria (new GraphicalIndicatorCriteria(m_file));
    criteria->setLeftValue(type);

    int indicatorType = MPPUtility::getInt(m_data.get(), m_dataOffset);
    m_dataOffset += 4;
    criteria->setIndicator(indicatorType);

    if (m_dataOffset + 4 < (int)m_data->length())
    {
        int operatorValue = MPPUtility::getInt(m_data.get(), m_dataOffset);
        m_dataOffset += 4;
		TestOperator* oper = (operatorValue == 0 ? TestOperator::getInstance(TestOperator::IS_ANY_VALUE) : 
								TestOperator::getInstance(operatorValue - 0x3E7));
		criteria->setOperator(oper);

		if (oper->getValue() != TestOperator::IS_ANY_VALUE)
        {
			processOperandValue(0, type, criteria);

			if (oper->getValue() == TestOperator::IS_WITHIN || oper->getValue() == TestOperator::IS_NOT_WITHIN)
			{
				processOperandValue(1, type, criteria);
			}
        }
    }

    return (criteria);
}

/**
* Process an operand value used in the definition of the graphical
* indicator criteria.
* 
* @param index position in operand list
* @param type field type
* @param criteria indicator criteria
*/
void GraphicalIndicatorReader::processOperandValue(int index, FieldType* type, GraphicalIndicatorCriteriaPtr criteria)
{
    bool valueFlag = (MPPUtility::getInt(m_data.get(), m_dataOffset) == 1);
    m_dataOffset += 4;

    if (valueFlag == false)
    {
        // 4 byte int representing the field type, we need the low bytes
        // the high bytes define if this is a task or a resource field
        int field = MPPUtility::getShort(m_data.get(), m_dataOffset);
        m_dataOffset += 4;
		if (dynamic_cast<TaskFieldClass*>(type))
        {
			criteria->setRightValue(index, TaskFieldClass::getInstance(MPPTaskField::getInstance(field)));
        }
        else
        {
			criteria->setRightValue(index, TaskFieldClass::getInstance(MPPResourceField::getInstance(field)));
        }
    }
    else
    {
        //int dataTypeValue = MPPUtility::getShort(m_data, m_dataOffset);
        m_dataOffset += 2;

        switch (type->getDataType())
        {
        case DT_DURATION: // 0x03
        {
            DurationPtr value = MPPUtility::getAdjustedDuration(m_file, MPPUtility::getInt(m_data.get(), m_dataOffset), 
				MPPUtility::getDurationTimeUnits(MPPUtility::getShort(m_data.get(), m_dataOffset + 4)));
            m_dataOffset += 6;
            criteria->setRightValue(index, value);
            break;
        }

		case DT_NUMERIC: // 0x05
        {
            DoublePtr value (new Double(MPPUtility::getDouble(m_data.get(), m_dataOffset)));
            m_dataOffset += 8;
            criteria->setRightValue(index, value);
            break;
        }

		case DT_CURRENCY: // 0x06
        {
            DoublePtr value (new Double(MPPUtility::getDouble(m_data.get(), m_dataOffset) / 100));
            m_dataOffset += 8;
            criteria->setRightValue(index, value);
            break;
        }

		case DT_STRING: // 0x08
        {
            CString value = MPPUtility::getUnicodeString(m_data.get(), m_dataOffset);
            m_dataOffset += ((value.GetLength() + 1) * 2);
            criteria->setRightValue(index, VariantPtr(new Variant(value)));
            break;
        }

		case DT_BOOLEAN: // 0x0B
        {
            int value = MPPUtility::getShort(m_data.get(), m_dataOffset);
            m_dataOffset += 2;
            criteria->setRightValue(index, BooleanPtr(new Boolean (value == 1 ? true : false)));
            break;
        }

		case DT_DATE: // 0x13
        {
            DatePtr value = MPPUtility::getTimestamp(m_data.get(), m_dataOffset);
            m_dataOffset += 4;
            criteria->setRightValue(index, value);
            break;
        }

        default:
        {
            break;
        }
        }
    }
}