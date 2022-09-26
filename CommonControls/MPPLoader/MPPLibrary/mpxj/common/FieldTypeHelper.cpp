#include "stdafx.h"
#include "FieldTypeHelper.h"
#include "MPPTaskField.h"
#include "MPPResourceField.h"
#include "MPPAssignmentField.h"
#include "MPPConstraintField.h"

#include "MPPTaskField14.h"
#include "MPPResourceField14.h"
#include "MPPAssignmentField14.h"

/**
	* Retrieve a FieldType instance based on an ID value from
	* an MPP9 or MPP12 file.
	*
	* @param fieldID field ID
	* @return FieldType instance
	*/
FieldType* FieldTypeHelper::getInstance(int fieldID)
{
	FieldType* result = NULL;
	int prefix = fieldID & 0xFFFF0000;
	int index = fieldID & 0x0000FFFF;

	switch (prefix)
	{
	case MPPTaskField::TASK_FIELD_BASE:
	{
		result = TaskFieldClass::getInstance (MPPTaskField::getInstance(index));
		if (!result)
		{
			result = getPlaceholder(L"TaskField", index);
		}
		break;
	}

	case MPPResource2Field::RESOURCE_FIELD_BASE:
	{
		result = Resource2FieldClass::getInstance(MPPResource2Field::getInstance(index));
		if (!result)
		{
			result = getPlaceholder(L"Resource2Field", index);
		}
		break;
	}

	case MPPAssignmentField::ASSIGNMENT_FIELD_BASE:
	{
		result = AssignmentFieldClass::getInstance(MPPAssignmentField::getInstance(index));
		if (!result)
		{
			result = getPlaceholder(L"AssignmentField", index);
		}
		break;
	}

	case MPPConstraintField::CONSTRAINT_FIELD_BASE:
	{
		result = ConstraintFieldClass::getInstance(MPPConstraintField::getInstance(index));
		if (!result)
		{
			result = getPlaceholder(L"ConstraintField", index);
		}
		break;
	}

	default:
	{
		result = getPlaceholder(L"", index);
		break;
	}
	}

	return result;
}

/**
	* Retrieve a FieldType instance based on an ID value from
	* an MPP14 field, mapping the START_TEXT, FINISH_TEXT, and DURATION_TEXT
	* values to START, FINISH, and DURATION respectively.
	*
	* @param fieldID field ID
	* @return FieldType instance
	*/
FieldType* FieldTypeHelper::getInstance14(int fieldID)
{
	FieldType* result = NULL;
	int prefix = fieldID & 0xFFFF0000;
	int index = fieldID & 0x0000FFFF;

	switch (prefix)
	{
	case MPPTaskField::TASK_FIELD_BASE:
	{
		result = TaskFieldClass::getInstance(MPPTaskField14::getInstance(index));
		if (!result)
		{
			result = getPlaceholder(L"TaskField", index);
		}
		break;
	}

	case MPPResource2Field::RESOURCE_FIELD_BASE:
	{
		Resource2Field field;
		if (MPPResource2Field14::getInstance(index, field))
		{
			result = Resource2FieldClass::getInstance(field);
			if (!result)
			{
				result = getPlaceholder(L"Resource2Field", index);
			}
		}

		break;
	}

	case MPPAssignmentField::ASSIGNMENT_FIELD_BASE:
	{
		result = AssignmentFieldClass::getInstance(MPPAssignmentField14::getInstance(index));
		if (!result)
		{
			result = getPlaceholder(L"AssignmentField", index);
		}
		break;
	}

	case MPPConstraintField::CONSTRAINT_FIELD_BASE:
	{
		result = ConstraintFieldClass::getInstance(MPPConstraintField::getInstance(index));
		if (!result)
		{
			result = getPlaceholder(L"ConstraintField", index);
		}
		break;
	}

	default:
	{
		result = getPlaceholder(L"", index);
		break;
	}
	}

	return result;
}

/**
* In some circumstances MS Project refers to the text version of a field (e.g. Start Text rather than Star) when we
* actually need to process the non-text version of the field. This method performs that mapping.
*
* @param field field to mapped
* @return mapped field
*/
FieldType* FieldTypeHelper::mapTextFields(FieldType* field)
{
	FieldType* result = field;
	if (field &&  dynamic_cast<TaskFieldClass*>(field))
	{
		//TaskField taskField = (TaskField)field;
		switch ((TaskField) field->getValue())
		{
		case TF_START_TEXT:
		{
			result = TaskFieldClass::getInstance(TaskField::TF_START);
			break;
		}

		case TF_FINISH_TEXT:
		{
			result = TaskFieldClass::getInstance(TaskField::TF_FINISH);
			break;
		}

		case TF_DURATION_TEXT:
		{
			result = TaskFieldClass::getInstance(TaskField::TF_DURATION);
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

class NullFieldType : public FieldType
{
	CString class_type;
	int fieldID;
public:
	NullFieldType(LPCTSTR class_type1, int fieldID1)
	{
		class_type = class_type1;
		fieldID = fieldID1;
	}

	virtual int getValue()
	{
		return fieldID;
	}

	virtual CString getName()
	{
		return class_type;// "Unknown " + class_type + "(" + fieldID + ")";
	}

	virtual CString getName(Locale* locale)
	{
		return getName();
	}

	virtual DataType getDataType()
	{
		return (DataType)NULL;
	}

	virtual FieldType* getUnitsType()
	{
		return NULL;
	}

	virtual CString toString()
	{
		return getName();
	}
};

/**
* Generate a placeholder for an unknown type.
*
* @param type expected type
* @param fieldID field ID
* @return placeholder
*/
FieldType* FieldTypeHelper::getPlaceholder(LPCTSTR class_type, int fieldID)
{
	CString key;
	key.Format(L"%s_%d", class_type, fieldID);

	std::shared_ptr<FieldType> fieldType;
	auto itMap = m_NullFieldType.find(key);
	if (itMap != m_NullFieldType.end())
		return itMap->second.get();
// 	if (m_NullFieldType.Lookup(key, fieldType))
// 		return fieldType.get();


	fieldType.reset(new NullFieldType(class_type, fieldID));
	//m_NullFieldType.put(key, fieldType);
	m_NullFieldType[key] = fieldType;
	return fieldType.get();
}

std::map<CString, std::shared_ptr<FieldType>> FieldTypeHelper::m_NullFieldType;