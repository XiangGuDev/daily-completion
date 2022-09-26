#include "stdafx.h"
#include "ConstraintField.h"
#include "LocaleData.h"

class ConstraintFieldExt : public FieldType
{
public:
	ConstraintFieldExt(int value, DataType type, FieldType* unitsType = NULL)
	{
		_value = value;
		_datatype = type;
		_fieldtype = unitsType;
	}

	virtual int getValue()
	{
		return _value;
	}

	virtual DataType getDataType()
	{
		return _datatype;
	}

	virtual FieldType* getUnitsType()
	{
		return _fieldtype;
	}

	virtual CString getName(Locale* locale)
	{
		if (_value > RF_START && _value < RF_FINISH)
			return LocaleData::getColumnText(locale, LocaleData::CONSTRAINT_COLUMNS, _value);
		else
			return L"";
	}

private:
	int _value;
	DataType _datatype;
	FieldType* _fieldtype;
};

static std::auto_ptr<ConstraintFieldExt> _ConstraintFields[CF_FINISH + 1];	// 600

void ResetConstraintFieldExt(ConstraintField field, DataType type, FieldType* unitsType = NULL)
{
	_ConstraintFields[field].reset(new ConstraintFieldExt(field, type, unitsType));
}

/**
* Instances of this type represent Resource2 fields.
*/
void InstanceAllConstraintField()
{
	ResetConstraintFieldExt(CF_UNIQUE_ID, DataType::DT_INTEGER);
	ResetConstraintFieldExt(CF_TASK1, DataType::DT_INTEGER);
	ResetConstraintFieldExt(CF_TASK2, DataType::DT_INTEGER);
};

FieldType* ConstraintFieldClass::getInstance(int type)
{
	if (_ConstraintFields[CF_UNIQUE_ID].get() == NULL)
	{
		InstanceAllConstraintField();
	}

	return _ConstraintFields[type].get();
}