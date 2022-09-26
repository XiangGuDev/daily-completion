#include "stdafx.h"
#include "FieldMap.h"
#include "MPPUtility.h"

const int FieldMap::TASK_KEYS[] =
{
	Props::TASK_FIELD_MAP,
	Props::TASK_FIELD_MAP2
};

const int FieldMap::ENTERPRISE_CUSTOM_KEYS[] =
{
	Props::ENTERPRISE_CUSTOM_FIELD_MAP
};

const int FieldMap::RESOURCE_KEYS[] =
{
	Props::RESOURCE_FIELD_MAP,
	Props::RESOURCE_FIELD_MAP2
};

const int FieldMap::ASSIGNMENT_KEYS[] =
{
	Props::ASSIGNMENT_FIELD_MAP,
	Props::ASSIGNMENT_FIELD_MAP2
};

const int FieldMap::RELATION_KEYS[] =
{
	Props::RELATION_FIELD_MAP
};

/**
* Constructor.
* 
* @param file parent project file 
*/
FieldMap::FieldMap(ProjectFile* file)
{
    m_file = file;
    m_defaultProjectTimeUnits = m_file->getProjectHeader()->getDefaultDurationUnits();
	memset(m_maxFixedDataOffset, sizeof(int)* MAX_FIXED_DATA_BLOCKS, 0);
}

/**
* Generic method used to create a field map from a block of data. 
* 
* @param data field map data
*/
void FieldMap::createFieldMap(ByteArrayPtr data)
{
    size_t index = 0;
    int lastDataBlockOffset = 0;
    int dataBlockIndex = 0;

	//int debugCount = 0;

    while (index < data->length())
    {
        long mask = MPPUtility::getInt(data.get(), index + 0);
        //mask = mask << 4;

		int dataBlockOffset = MPPUtility::getShort(data.get(), index + 4);
        //int metaFlags = MPPUtility::getByte(data, index + 8);

		//if (debugCount == 2)
		//	dataBlockOffset = dataBlockOffset;

		FieldType* type = getFieldType(MPPUtility::getInt(data.get(), index + 12));

		//ATLTRACE(L"createFieldMap %d type=%d\n", debugCount, type == NULL ? -1 : type->getValue());

		//debugCount++;

		if (type != NULL)
		{
			int category = MPPUtility::getShort(data.get(), index + 20);
			//int sizeInBytes = MPPUtility::getShort(data, index + 22);
			//int metaIndex = MPPUtility::getInt(data, index + 24);

			//
			// Categories
			//
			// 02 - Short values [RATE_UNITS, WORKGROUP, ACCRUE, TIME_UNITS, PRIORITY, TASK_TYPE, CONSTRAINT, ACCRUE, PERCENTAGE, SHORT, WORK_UNITS]  - BOOKING_TYPE, EARNED_VALUE_METHOD, DELIVERABLE_TYPE, RESOURCE_REQUEST_TYPE - we have as string in MPXJ????
			// 03 - Int values [DURATION, INTEGER] - Recalc outline codes as Boolean?
			// 05 - Rate, Number [RATE, NUMERIC]
			// 08 - CString (and some durations!!!) [STRING, DURATION]
			// 0B - Boolean (meta block 0?) - [BOOLEAN]
			// 13 - Date - [DATE]
			// 48 - GUID - [GUID]
			// 64 - Boolean (meta block 1?)- [BOOLEAN]
			// 65 - Work, Currency [WORK, CURRENCY]
			// 66 - Units [UNITS]
			// 1D - Raw bytes [BINARY, ASCII_STRING] - Exception: outline code indexes, they are integers, but stored as part of a binary block

			int varDataKey = 0;
			if (useTypeAsVarDataKey())
			{
				int substitute = 0;
				if (!substituteVarDataKey(type, substitute))
				{
					varDataKey = (MPPUtility::getInt(data.get(), index + 12) & 0x0000FFFF);
				}
				else
				{
					varDataKey = substitute;
				}
			}
			else
			{
				varDataKey = MPPUtility::getByte(data.get(), index + 6);
			}

			FieldLocation location;
			int metaBlock;

			switch (category)
			{
			case 0x0B:
			{
				location = FieldLocation::META_DATA;
				metaBlock = 0;
				break;
			}

			case 0x64:
			{
				location = FieldLocation::META_DATA;
				metaBlock = 1;
				break;
			}

			default:
			{
				metaBlock = 0;
				if (dataBlockOffset != 65535)
				{
					location = FieldLocation::FIXED_DATA;
					if (dataBlockOffset < lastDataBlockOffset)
					{
						++dataBlockIndex;
					}
					lastDataBlockOffset = dataBlockOffset;

					if (dataBlockOffset > m_maxFixedDataOffset[dataBlockIndex])
					{
						m_maxFixedDataOffset[dataBlockIndex] = dataBlockOffset;
					}
				}
				else
				{
					if (varDataKey != 0)
					{
						location = FieldLocation::VAR_DATA;
					}
					else
					{
						location = FieldLocation::UNKNOWN;
					}
				}
				break;
			}
			}

			FieldItemPtr item(new FieldItem(this, type, location, dataBlockIndex, dataBlockOffset, varDataKey, mask, metaBlock));
			m_map.put(type, item);
		}

        index += 28;
    }
}

// 输出到控制台（调试用）
void FieldMap::debugPrintFieldMap()
{
	CString msg;

	POSITION pos = m_map.GetStartPosition();
	while (pos)
	{
		Map2<FieldType*, FieldItemPtr>::CPair* pair = m_map.GetNext(pos);

		ATLTRACE(L"type value: %d, varDatakey: %d\n", pair->m_key->getValue(), pair->m_value->getVarDataKey());
	}
}

/**
* Creates a field map for tasks.
* 
* @param props props data
*/
void FieldMap::createTaskFieldMap(PropsPtr props)
{
    ByteArrayPtr fieldMapData;
    for (int key : TASK_KEYS)
    {
        fieldMapData = props->getByteArray2(key);
        if (fieldMapData != NULL)
        {
			break;
        }
    }

    if (fieldMapData == NULL)
    {
        populateDefaultData(getDefaultTaskData());
    }
    else
    {
        createFieldMap(fieldMapData);
    }
}

/**
* Creates a field map for relations.
* 
* @param props props data
*/
void FieldMap::createRelationFieldMap(PropsPtr props)
{
    ByteArrayPtr fieldMapData = NULL;
    for (int key : RELATION_KEYS)
    {
        fieldMapData = props->getByteArray2(key);
        if (fieldMapData != NULL)
        {
        break;
        }
    }

    if (fieldMapData == NULL)
    {
        populateDefaultData(getDefaultRelationData());
    }
    else
    {
        createFieldMap(fieldMapData);
    }
}

/**
* Create a field map for enterprise custom fields.
* 
* @param props props data
* @param c target class
*/
void FieldMap::createEnterpriseCustomFieldMap(PropsPtr props, LPCTSTR type_c)
{
    ByteArrayPtr fieldMapData;
    for (int key : ENTERPRISE_CUSTOM_KEYS)
    {
        fieldMapData = props->getByteArray2(key);
        if (fieldMapData)
        {
			break;
        }
    }

	if (!fieldMapData)
		return;

	int type_len = _tcslen(type_c);

    int index = 4;
    while (index < (int) fieldMapData->length())
    {
		int typeValue = MPPUtility::getInt(fieldMapData.get(), index);
		FieldType* type = getFieldType(typeValue);
		if (type != NULL)
		{
			CString type_name(typeid(*type).name());
			if (type_name.Left(6) == L"class ")
				type_name = type_name.Mid(6);

			//ASSERT(FALSE); // TaskFieldClass等有Class结尾

			LPCTSTR ent_check = L"Enterprise Custom Field";
			if (type && type_name.Left(type_len) == type_c)
			{
				if (type->toString().Left(_tcslen(ent_check)) == ent_check)
				{
					int varDataKey = (typeValue & 0xFFFF);
					FieldItemPtr item(new FieldItem(this, type, FieldLocation::VAR_DATA, 0, 0, varDataKey, 0, 0));
					m_map.put(type, item);
				}
			}
		}

		index += 4;
    }
}

/**
* Creates a field map for resources.
* 
* @param props props data
*/
void FieldMap::createResource2FieldMap(PropsPtr props)
{
    ByteArrayPtr fieldMapData = NULL;
    for (int key : RESOURCE_KEYS)
    {
        fieldMapData = props->getByteArray2(key);
        if (fieldMapData != NULL)
        {
        break;
        }
    }

    if (fieldMapData == NULL)
    {
        populateDefaultData(getDefaultResource2Data());
    }
    else
    {
        createFieldMap(fieldMapData);
    }
}

/**
* Creates a field map for assignments.
* 
* @param props props data
*/
void FieldMap::createAssignmentFieldMap(PropsPtr props)
{
    //System.out.println("ASSIGN");
    ByteArrayPtr fieldMapData = NULL;
    for (int key : ASSIGNMENT_KEYS)
    {
		fieldMapData = props->getByteArray2(key);
        if (fieldMapData != NULL)
        {
        break;
        }
    }

    if (fieldMapData == NULL)
    {
        populateDefaultData(getDefaultAssignmentData());
    }
    else
    {
        createFieldMap(fieldMapData);
    }
}

/**
* This method takes an array of data and uses this to populate the
* field map.
* 
* @param defaultData field map default data
*/
void FieldMap::populateDefaultData(FieldItemArrayPtr defaultData)
{
	for (size_t i = 0; i < defaultData->length(); i ++)
    {
		FieldItemPtr item = (*defaultData)[i];

        m_map.put(item->getType(), item);
    }
}

/**
* Given a container, and a set of raw data blocks, this method extracts
* the field data and writes it into the container.
* 
* @param type expected type
* @param container field container
* @param id entity ID
* @param fixedData fixed data block
* @param varData var data block
*/
void FieldMap::populateContainer(LPCTSTR type_c, FieldContainer* container,
	int id, ByteArrayPtr* fixedData, int arr_len, Var2DataPtr varData)
{

	Array2<ByteArrayPtr> fixedData2(arr_len);
	for (int i = 0; i < arr_len; i++)
	{
		fixedData2[i] = fixedData[i];
	}

    //System.out.println(container.getClass().getSimpleName()+": " + id);
	POSITION pos = m_map.GetStartPosition();

	int type_len = _tcslen(type_c);

	while (pos)
	{
		FieldItemPtr item = m_map.GetNextValue(pos);
		CString type_name(typeid(*(item->getType())).name());
		if (type_name.Left(6) == L"class ")
			type_name = type_name.Mid(6);
		//ASSERT(FALSE);	// type要调试一下
		if (type_name.Left(type_len) == type_c)
		{
			//System.out.println(item.m_type);
			ObjectPtr value = item->read(id, fixedData2, varData);
			//System.out.println(item.m_type->getClass().getSimpleName() + "." + item.m_type +  ": " + value);

			//Resource2* res = dynamic_cast<Resource2*>(container);
			//if (res && item->getType()->getValue() == Resource2Field::RF_MAX_UNITS)
			//	int i = 0;

			if (item->getType()->getValue() == TaskField::TF_DURATION_TEXT) {
				value = item->read(id, fixedData2, varData);
			}

			if (value)
				container->set(item->getType(), value);
		}
	}
}

/**
* Retrieve the maximum offset in the fixed data block.
* 
* @param blockIndex required block index
* @return maximum offset
*/
int FieldMap::getMaxFixedDataOffset(int blockIndex)
{
    return m_maxFixedDataOffset[blockIndex];
}

/**
* Retrieve the fixed data offset for a specific field.
* 
* @param type field type
* @return offset
*/
IntegerPtr FieldMap::getFixedDataOffset(FieldType* type)
{
	IntegerPtr result;
	FieldItemPtr item;
	if (m_map.Lookup(type, item))
    {
		result = IntegerPtr(new Integer(item->getFixedDataOffset()));
    }

    return result;
}

/**
* Retrieve the var data key for a specific field.
* 
* @param type field type
* @return var data key
*/
IntegerPtr FieldMap::getVarDataKey(FieldType* type)
{
	IntegerPtr result;
	FieldItemPtr item;
	if (m_map.Lookup(type, item))
    {
		result = IntegerPtr(new Integer(item->getVarDataKey()));
    }
    return result;
}

/**
* Used to map from a var data key to a field type. Note this
* is designed for diagnostic use only, and uses an inefficient search.
* 
* @param key var data key
* @return field type
*/
FieldType* FieldMap::getFieldTypeFromVarDataKey(int key)
{
    FieldType* result = NULL;

	POSITION pos = m_map.GetStartPosition();
	while (pos)
	{
		Map2<FieldType*, FieldItemPtr>::CPair* entry = m_map.GetNext(pos);
		if (entry->m_value->getFieldLocation() == FieldLocation::VAR_DATA &&
			entry->m_value->getVarDataKey() == key)
		{
			result = entry->m_key;
			break;
		}
	}

    return result;
}

/**
* Retrieve the field location for a specific field.
* 
* @param type field type
* @return field location
*/
FieldLocation FieldMap::getFieldLocation(FieldType* type)
{
	FieldLocation result = UNKNOWN;

    FieldItemPtr item;
	if (m_map.Lookup(type, item))
    {
        result = item->getFieldLocation();
    }
    return result;
}

/**
* Retrieve a single field value.
* 
* @param id parent entity ID
* @param type field type
* @param fixedData fixed data block
* @param varData var data block
* @return field value
*/
ObjectPtr FieldMap::getFieldData(int id, FieldType* type, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData)
{
    ObjectPtr result;

	FieldItemPtr item;
	if (m_map.Lookup(type, item))
    {
        result = item->read(id, fixedData, varData);
    }

    return result;
}

/**
* Retrieve the parent project file.
* 
* @return project file
*/
ProjectFile* FieldMap::getProjectFile()
{
    return m_file;
}

/**
* Clear the field map.
*/
void FieldMap::clear()
{
    m_map.RemoveAll();
    //Arrays.fill(m_maxFixedDataOffset, 0);
	memset(m_maxFixedDataOffset, sizeof(int)* MAX_FIXED_DATA_BLOCKS, 0);
}

/**
* Diagnostic method used to dump known field map data.
* 
* @param props props block containing field map data
*/
void FieldMap::dumpKnownFieldMaps(PropsPtr props)
{
    //for (int key=131092; key < 131098; key++)
    for (int key = 50331668; key < 50331674; key++)
    {
		ByteArrayPtr fieldMapData = props->getByteArray2(key);
        if (fieldMapData)
        {
			//System.out.println("KEY: " + key);
			createFieldMap(fieldMapData);
			//System.out.println(toString());
			clear();
        }
    }
}

///**
//* {@inheritDoc}
//*/
//CString FieldMap::toString()
//{
//    StringWriter sw = new StringWriter();
//    PrintWriter pw = new PrintWriter(sw);
//
//    ArrayList<FieldItem> items = new ArrayList<FieldItem>(m_map.values());
//    Collections.sort(items);
//
//    pw.println("[FieldMap");
//
//    for (int loop = 0; loop < m_maxFixedDataOffset.length; loop++)
//    {
//        pw.print(" MaxFixedOffset (block ");
//        pw.print(loop);
//        pw.print(")=");
//        pw.println(m_maxFixedDataOffset[loop]);
//    }
//
//    for (FieldItem item : items)
//    {
//        pw.print(" ");
//        pw.println(item);
//    }
//    pw.println("]");
//
//    pw.close();
//    return sw.toString();
//}

/**
* Constructor.
* 
* @param type field type
* @param location identifies which block the field is present in
* @param fixedDataBlockIndex identifies which block the data comes from
* @param fixedDataOffset fixed data block offset
* @param varDataKey var data block key
* @param mask TODO
* @param metaBlock TODO
*/
FieldItem::FieldItem(FieldMap* fieldMap, FieldType* type, FieldLocation location, int fixedDataBlockIndex,
					int fixedDataOffset, int varDataKey, long mask, int metaBlock)
{
	m_fieldMap = fieldMap;
    m_type = type;
    m_location = location;
    m_fixedDataBlockIndex = fixedDataBlockIndex;
    m_fixedDataOffset = fixedDataOffset;
    m_varDataKey = varDataKey;
    m_mask = mask;
    m_metaBlock = metaBlock;

	if (varDataKey == 1253 && type->getValue() == TaskField::TF_START)
		int i = 0;
}

/**
* Reads a single field value.
* 
* @param id parent entity ID
* @param fixedData fixed data block
* @param varData var data block
* @return field value
*/
ObjectPtr FieldItem::read(int id, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData)
{
    ObjectPtr result;

    switch (m_location)
    {
    case FIXED_DATA:
    {
        result = readFixedData(id, fixedData, varData);
        break;
    }

    case VAR_DATA:
    {
        result = readVarData(id, fixedData, varData);
        break;
    }

    case META_DATA:
    {
        // We know that the Boolean flags are stored in the
        // "meta data" block, and can see that the first
        // four bytes of each row read from the field map
        // data in the MPP file represents a bit mask... but
        // we just haven't worked out how to convert this into
        // the actual location in the data. For now we rely on
        // the location in the file being fixed. This is why
        // we ignore the META_DATA case.
        break;
    }

    default:
    {
        // Unknown location - ignore this.
        break;
    }
    }

    return result;
}

/**
* Read a field from the fixed data block.
* 
* @param id parent entity ID
* @param fixedData fixed data block
* @param varData var data block
* @return field value
*/
ObjectPtr FieldItem::readFixedData(int id, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData)
{
    ObjectPtr result;
	if (m_fixedDataBlockIndex >= (int)fixedData.length())
		return result;

    ByteArrayPtr data = fixedData[m_fixedDataBlockIndex];
	if (!data || (data && m_fixedDataOffset >= (int)data->length()))
		return result;

    switch (m_type->getDataType())
    {
	case DataType::DT_DATE:
        {
			result = MPPUtility::getTimestamp(data.get(), m_fixedDataOffset);
            break;
        }

	case DataType::DT_INTEGER:
        {
			result = IntegerPtr(new Integer(MPPUtility::getInt(data.get(), m_fixedDataOffset)));
            break;
        }

	case DataType::DT_DURATION:
        {
            FieldType* unitsType = m_type->getUnitsType();
			TimeUnit units = (TimeUnit) std::dynamic_pointer_cast<Integer>(
				m_fieldMap->getFieldData(id, unitsType, fixedData, varData))->intValue();
            if (units == NULL)
            {
				units = m_fieldMap->getProjectFile()->getProjectHeader()->getDefaultDurationUnits();
            }

			result = MPPUtility::getAdjustedDuration(m_fieldMap->getProjectFile(),
				MPPUtility::getInt(data.get(), m_fixedDataOffset), units);
            break;
        }

	case DataType::DT_TIME_UNITS:
        {
			result = IntegerPtr(new Integer(MPPUtility::getDurationTimeUnits(
				MPPUtility::getShort(data.get(), m_fixedDataOffset), m_fieldMap->m_defaultProjectTimeUnits)));
            break;
        }

	case DataType::DT_CONSTRAINT:
        {
			result = IntegerPtr(new Integer((ConstraintType)(MPPUtility::getShort(data.get(), m_fixedDataOffset))));
            break;
        }

	case DataType::DT_PRIORITY:
        {
			result = Priority::getInstance(MPPUtility::getShort(data.get(), m_fixedDataOffset));
            break;
        }

	case DataType::DT_PERCENTAGE:
        {
			result = DoublePtr(new Double(MPPUtility::getPercentage(data.get(), m_fixedDataOffset)));
            break;
        }

	case DataType::DT_TASK_TYPE:
        {
			result = IntegerPtr(new Integer((TaskType)(MPPUtility::getShort(data.get(), m_fixedDataOffset))));
            break;
        }

	case DataType::DT_ACCRUE:
        {
			result = IntegerPtr(new Integer((AccrueType)(MPPUtility::getShort(data.get(), m_fixedDataOffset))));
            break;
        }

	case DataType::DT_CURRENCY:
	case DataType::DT_UNITS:
        {
			result = DoublePtr(new Double(MPPUtility::getDouble(data.get(), m_fixedDataOffset) / 100));
            break;
        }

	case DataType::DT_RATE:
        {
			result = RatePtr(new Rate(MPPUtility::getDouble(data.get(), m_fixedDataOffset), TimeUnit::HOURS));
            break;
        }

	case DataType::DT_WORK:
        {
			result = Duration2::getInstance(MPPUtility::getDouble(data.get(), m_fixedDataOffset) / 60000, TimeUnit::HOURS);
            break;
        }

	case DataType::DT_SHORT:
        {
			result = IntegerPtr(new Integer(MPPUtility::getShort(data.get(), m_fixedDataOffset)));
            break;
        }

		case DataType::DT_BOOLEAN:
        {
			result = BooleanPtr(new Boolean(MPPUtility::getShort(data.get(), m_fixedDataOffset) != 0));
            break;
        }

		case DataType::DT_DELAY:
        {
			result = MPPUtility::getDuration(MPPUtility::getShort(data.get(), m_fixedDataOffset), TimeUnit::HOURS);
            break;
        }

		case DataType::DT_WORK_UNITS:
        {
			int variableRateUnitsValue = MPPUtility::getByte(data.get(), m_fixedDataOffset);
			result = IntegerPtr(new Integer(variableRateUnitsValue == 0 ? NULL : 
								MPPUtility::getWorkTimeUnits(variableRateUnitsValue)));
            break;
        }

		case DataType::DT_WORKGROUP:
        {
			result = IntegerPtr(new Integer((WorkGroup2)(MPPUtility::getShort(data.get(), m_fixedDataOffset))));
            break;
        }

		case DataType::DT_RATE_UNITS:
        {
			result = IntegerPtr(new Integer((TimeUnit)(MPPUtility::getShort(data.get(), m_fixedDataOffset) - 1)));
            break;
        }

		case DataType::DT_GUID:
        {
			result = VariantPtr(new Variant(&MPPUtility::getGUID(data.get(), m_fixedDataOffset)));
            break;
        }

        default:
        {
            //System.out.println("**** UNSUPPORTED FIXED DATA TYPE");
            break;
        }
    }

    return result;
}

/**
* Read a field value from a var data block.
* 
* @param id parent entity ID
* @param fixedData fixed data block
* @param varData var data block
* @return field value
*/
ObjectPtr FieldItem::readVarData(int id, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData)
{
    ObjectPtr result;

    switch (m_type->getDataType())
    {
	case DataType::DT_DURATION:
    {
        FieldType* unitsType = m_type->getUnitsType();
		IntegerPtr temp = std::dynamic_pointer_cast<Integer>(m_fieldMap->getFieldData(id, unitsType, fixedData, varData));
		TimeUnit units = temp ? (TimeUnit) temp->intValue() : TimeUnit::HOURS;

        result = getCustomFieldDurationValue(varData, id, m_varDataKey, units);
        break;
    }

	case DataType::DT_TIME_UNITS:
    {
        result = IntegerPtr(new Integer(MPPUtility::getDurationTimeUnits(
			varData->getShort(id, m_varDataKey), m_fieldMap->m_defaultProjectTimeUnits)));
        break;
    }

	case DataType::DT_CURRENCY:
    {
        result = DoublePtr(new Double(varData->getDouble(id, m_varDataKey) / 100));
        break;
    }

	case DataType::DT_STRING:
    {
		StringPtr text = getCustomFieldUnicodeStringValue(varData, id, m_varDataKey);
		if (text) {
			result = VariantPtr(new Variant((LPCTSTR)text));
		}
        
        break;
    }

	case DataType::DT_DATE:
    {
        result = getCustomFieldTimestampValue(varData, id, m_varDataKey);
        break;
    }

	case DataType::DT_NUMERIC:
    {
        result = DoublePtr(new Double(getCustomFieldDoubleValue(varData, id, m_varDataKey)));
        break;
    }

	case DataType::DT_INTEGER:
    {
		result = IntegerPtr(new Integer((varData->getInt(id, m_varDataKey))));
        break;
    }

	case DataType::DT_WORK:
    {
		result = Duration2::getInstance(varData->getDouble(id, m_varDataKey) / 60000, TimeUnit::HOURS);
        break;
    }

	case DataType::DT_ASCII_STRING:
    {
        result = VariantPtr(new Variant(varData->getString(id, m_varDataKey)));
        break;
    }

	case DataType::DT_DELAY:
    {
        result = MPPUtility::getDuration(varData->getShort(id, m_varDataKey), TimeUnit::HOURS);
        break;
    }

	case DataType::DT_WORK_UNITS:
    {
        int variableRateUnitsValue = varData->getByte(id, m_varDataKey);
        result = IntegerPtr(new Integer(variableRateUnitsValue == 0 ? NULL : MPPUtility::getWorkTimeUnits(variableRateUnitsValue)));
        break;
    }

	case DataType::DT_RATE_UNITS:
    {
		result = IntegerPtr(new Integer(varData->getShort(id, m_varDataKey) - 1));
        break;
    }

	case DataType::DT_ACCRUE:
    {
		result = IntegerPtr(new Integer(varData->getShort(id, m_varDataKey)));
        break;
    }

	case DataType::DT_SHORT:
    {
		result = IntegerPtr(new Integer(varData->getShort(id, m_varDataKey)));
        break;
    }

	case DataType::DT_BOOLEAN:
    {
        result = BooleanPtr(new Boolean(varData->getShort(id, m_varDataKey) != 0));
        break;
    }

	case DataType::DT_WORKGROUP:
    {
		result = IntegerPtr(new Integer(varData->getShort(id, m_varDataKey)));
        break;
    }

	case DataType::DT_GUID:
    {
		result = VariantPtr(new Variant(&MPPUtility::getGUID(varData->getByteArray2(id, m_varDataKey).get(), 0)));
        break;
    }

	case DataType::DT_BINARY:
    {
        // Do nothing for binary data
        break;
    }

    default:
    {
        //System.out.println("**** UNSUPPORTED VAR DATA TYPE");
        break;
    }
    }

    return result;
}

/**
* Retrieve custom field value.
* 
* @param varData var data block
* @param id item ID
* @param type item type
* @return item value
*/
ObjectPtr FieldItem::getCustomFieldTimestampValue(Var2DataPtr varData, int id, int type)
{
    ObjectPtr result;

    int mask = varData->getShort(id, type);
    if ((mask & 0xFF00) != m_fieldMap->VALUE_LIST_MASK)
    {
		ByteArrayPtr data = varData->getByteArray2(id, type);

		if (data)
		{
			if (data->length() == 512)
			{
				result = VariantPtr(new Variant(MPPUtility::getUnicodeString(data.get())));
			}
			else if (data->length() >= 4)
			{
				result = MPPUtility::getTimestamp(data.get());
			}
		}
    }
    else
    {
		int uniqueId = varData->getInt(id, 2, type);
		CustomFieldValueItemPtr item = m_fieldMap->getProjectFile()->getCustomFieldValueItem(uniqueId);
		if (item && item->getValue() != NULL)
		{
			result = MPPUtility::getTimestamp(item->getValue().get());
			// TODO: 升级到2015后就错了，原一下原因？
			//result = IntegerPtr(new Integer(date));
		}
    }
    return result;
}

/**
* Retrieve custom field value.
* 
* @param varData var data block
* @param id item ID
* @param type item type
* @param units duration units
* @return item value
*/
ObjectPtr FieldItem::getCustomFieldDurationValue(Var2DataPtr varData, int id, int type, TimeUnit units)
{
    ObjectPtr result;

    ByteArrayPtr data = varData->getByteArray2(id, type);

    if (data)
    {
		if (data->length() == 512)
		{
				result = VariantPtr(new Variant(MPPUtility::getUnicodeString(data.get())));
		}
		else if (data->length() >= 4)
		{
			int duration = MPPUtility::getInt(data.get());
			result = MPPUtility::getAdjustedDuration(m_fieldMap->getProjectFile(), duration, units);
		}
    }

    return result;
}

/**
* Retrieve custom field value.
* 
* @param varData var data block
* @param id item ID
* @param type item type
* @return item value
*/
double FieldItem::getCustomFieldDoubleValue(Var2DataPtr varData, int id, int type)
{
    double result = 0;

    int mask = varData->getShort(id, type);
	if ((mask & 0xFF00) != m_fieldMap->VALUE_LIST_MASK)
    {
		result = varData->getDouble(id, type);
    }
    else
    {
		int uniqueId = varData->getInt(id, 2, type);
		CustomFieldValueItemPtr item = m_fieldMap->getProjectFile()->getCustomFieldValueItem(uniqueId);
		if (item && item->getValue() != NULL)
		{
			result = MPPUtility::getDouble(item->getValue().get());
		}
    }
	return result;// NumberHelper::getDouble(result);
}

/**
* Retrieve custom field value.
* 
* @param varData var data block
* @param id item ID
* @param type item type
* @return item value
*/
StringPtr FieldItem::getCustomFieldUnicodeStringValue(Var2DataPtr varData, int id, int type)
{
	StringPtr result;

    int mask = varData->getShort(id, type);
	if ((mask & 0xFF00) != m_fieldMap->VALUE_LIST_MASK)
    {
		result = varData->getUnicodeString(id, type);
    }
    else
    {
		int uniqueId = varData->getInt(id, 2, type);
		CustomFieldValueItemPtr item = m_fieldMap->getProjectFile()->getCustomFieldValueItem(uniqueId);
		if (item && item->getValue() != NULL)
		{
			result.reset(MPPUtility::getUnicodeString(item->getValue().get()));
		}
    }
    return result;
}

/**
* Retrieve the field type.
* 
* @return field type
*/
FieldType* FieldItem::getType()
{
    return m_type;
}

/**
* Retrieve the index of the fixed data block containing this item.
* 
* @return fixed data block index
*/
int FieldItem::getFixedDataBlockIndex()
{
    return m_fixedDataBlockIndex;
}

/**
* Retrieve the fixed data offset for this field.
* 
* @return fixed data offset
*/
int FieldItem::getFixedDataOffset()
{
    return m_fixedDataOffset;
}

/**
* Retrieve the var data key for this field.
* 
* @return var data key
*/
int FieldItem::getVarDataKey()
{
    return m_varDataKey;
}

/**
* Retrieve the field location for this field.
* 
* @return field location
*/
FieldLocation FieldItem::getFieldLocation()
{
    return m_location;
}

/**
* Implements the only method in the Comparable interface to allow
* FieldItem instances to be sorted.
* 
* @param item item to compare with
* @return comparison result
*/
int FieldItem::compareTo(FieldItem* item)
{
	int result = m_location - item->m_location;
	if (result == 0)
    {
		switch (m_location)
		{
		case FieldLocation::FIXED_DATA:
			{
				result = m_fixedDataBlockIndex - item->m_fixedDataBlockIndex;
				if (result == 0)
				{
					result = m_fixedDataOffset - item->m_fixedDataOffset;
				}
				break;
			}

		case FieldLocation::VAR_DATA:
			{
				result = m_varDataKey - item->m_varDataKey;
				break;
			}

			default:
			{
				break;
			}
		}
    }
    return result;
}

///**
//* {@inheritDoc}
//*/
//CString FieldItem::toString()
//{
//    StringBuilder buffer = new StringBuilder();
//    buffer.append("[FieldItem type=");
//    buffer.append(m_type);
//    buffer.append(" location=");
//    buffer.append(m_location);
//
//    switch (m_location)
//    {
//    case FIXED_DATA:
//    {
//        buffer.append(" fixedDataBlockIndex=");
//        buffer.append(m_fixedDataBlockIndex);
//        buffer.append(" fixedDataBlockOffset=");
//        buffer.append(m_fixedDataOffset);
//        break;
//    }
//
//    case VAR_DATA:
//    {
//        buffer.append(" varDataKey=");
//        buffer.append(m_varDataKey);
//        break;
//    }
//
//    case META_DATA:
//    {
//        buffer.append(" mask=");
//        buffer.append(Long.toHexString(m_mask));
//        buffer.append(" block=");
//        buffer.append(m_metaBlock);
//
//        break;
//    }
//
//    default:
//    {
//        break;
//    }
//    }
//
//    buffer.append("]");
//
//    return buffer.toString();
//}