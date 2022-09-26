#include "stdafx.h"
#include "ResourceField.h"
#include "LocaleData.h"
//#include "GarbageCollector.h"

class Resource2FieldExt : public FieldType
{
public:
	Resource2FieldExt(int value, DataType type, FieldType* unitsType = NULL)
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
			return LocaleData::getColumnText(locale, LocaleData::RESOURCE_COLUMNS, _value);
		else
			return L"";
	}

private:
	int _value;
	DataType _datatype;
	FieldType* _fieldtype;
};

static std::auto_ptr<Resource2FieldExt> _Resource2Fields[RF_FINISH+1];	// 600

void ResetFieldExt(Resource2Field field, DataType type, FieldType* unitsType = NULL)
{
	_Resource2Fields[field].reset(new Resource2FieldExt(field, type, unitsType));
}

/**
 * Instances of this type represent Resource2 fields.
 */
void InstanceAllResource2Field()
{
	ResetFieldExt(RF_START, DataType::DT_DATE); // Must always be first value

	ResetFieldExt(RF_DURATION1_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION2_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION3_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION4_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION5_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION6_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION7_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION8_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION9_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_DURATION10_UNITS, DataType::DT_TIME_UNITS);

	ResetFieldExt(RF_ID, DataType::DT_INTEGER);
	ResetFieldExt(RF_NAME, DataType::DT_STRING);
	ResetFieldExt(RF_INITIALS, DataType::DT_STRING);
	ResetFieldExt(RF_GROUP, DataType::DT_STRING);
	ResetFieldExt(RF_MAX_UNITS, DataType::DT_UNITS);
	ResetFieldExt(RF_BASE_CALENDAR, DataType::DT_STRING);
	ResetFieldExt(RF_STANDARD_RATE, DataType::DT_RATE);
	ResetFieldExt(RF_OVERTIME_RATE, DataType::DT_RATE);
	ResetFieldExt(RF_TEXT1, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT2, DataType::DT_STRING);
	ResetFieldExt(RF_CODE, DataType::DT_STRING);
	ResetFieldExt(RF_ACTUAL_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_ACTUAL_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_OVERTIME_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST_PER_USE, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ACCRUE_AT, DataType::DT_ACCRUE);
	ResetFieldExt(RF_REMAINING_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_REMAINING_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_WORK_VARIANCE, DataType::DT_DURATION);
	ResetFieldExt(RF_COST_VARIANCE, DataType::DT_CURRENCY);
	ResetFieldExt(RF_OVERALLOCATED, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_PEAK, DataType::DT_UNITS);
	ResetFieldExt(RF_UNIQUE_ID, DataType::DT_INTEGER);
	ResetFieldExt(RF_NOTES, DataType::DT_ASCII_STRING);
	ResetFieldExt(RF_PERCENT_WORK_COMPLETE, DataType::DT_PERCENTAGE);
	ResetFieldExt(RF_TEXT3, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT4, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT5, DataType::DT_STRING);
	ResetFieldExt(RF_OBJECTS, DataType::DT_NUMERIC);
	ResetFieldExt(RF_LINKED_FIELDS, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_EMAIL_ADDRESS, DataType::DT_STRING);
	ResetFieldExt(RF_REGULAR_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_ACTUAL_OVERTIME_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_REMAINING_OVERTIME_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_OVERTIME_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ACTUAL_OVERTIME_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_REMAINING_OVERTIME_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BCWS, DataType::DT_NUMERIC);
	ResetFieldExt(RF_BCWP, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ACWP, DataType::DT_NUMERIC);
	ResetFieldExt(RF_SV, DataType::DT_NUMERIC);
	ResetFieldExt(RF_AVAILABLE_FROM, DataType::DT_DATE);
	ResetFieldExt(RF_AVAILABLE_TO, DataType::DT_DATE);
	ResetFieldExt(RF_INDICATORS, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT6, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT7, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT8, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT9, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT10, DataType::DT_STRING);
	ResetFieldExt(RF_START1, DataType::DT_DATE);
	ResetFieldExt(RF_START2, DataType::DT_DATE);
	ResetFieldExt(RF_START3, DataType::DT_DATE);
	ResetFieldExt(RF_START4, DataType::DT_DATE);
	ResetFieldExt(RF_START5, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH1, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH2, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH3, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH4, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH5, DataType::DT_DATE);
	ResetFieldExt(RF_NUMBER1, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER2, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER3, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER4, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER5, DataType::DT_NUMERIC);
	ResetFieldExt(RF_DURATION1, DataType::DT_DURATION, _Resource2Fields[RF_DURATION1_UNITS].get());
	ResetFieldExt(RF_DURATION2, DataType::DT_DURATION, _Resource2Fields[RF_DURATION2_UNITS].get());
	ResetFieldExt(RF_DURATION3, DataType::DT_DURATION, _Resource2Fields[RF_DURATION3_UNITS].get());
	ResetFieldExt(RF_COST1, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST2, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST3, DataType::DT_CURRENCY);
	ResetFieldExt(RF_FLAG10, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG1, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG2, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG3, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG4, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG5, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG6, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG7, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG8, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG9, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_HYPERLINK, DataType::DT_STRING);
	ResetFieldExt(RF_HYPERLINK_ADDRESS, DataType::DT_STRING);
	ResetFieldExt(RF_HYPERLINK_SUBADDRESS, DataType::DT_STRING);
	ResetFieldExt(RF_HYPERLINK_HREF, DataType::DT_STRING);
	ResetFieldExt(RF_ASSIGNMENT, DataType::DT_STRING);
	ResetFieldExt(RF_TASK_SUMMARY_NAME, DataType::DT_STRING);
	ResetFieldExt(RF_CAN_LEVEL, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_WORK_CONTOUR, DataType::DT_STRING);
	ResetFieldExt(RF_COST4, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST5, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST6, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST7, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST8, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST9, DataType::DT_CURRENCY);
	ResetFieldExt(RF_COST10, DataType::DT_CURRENCY);
	ResetFieldExt(RF_DATE1, DataType::DT_DATE);
	ResetFieldExt(RF_DATE2, DataType::DT_DATE);
	ResetFieldExt(RF_DATE3, DataType::DT_DATE);
	ResetFieldExt(RF_DATE4, DataType::DT_DATE);
	ResetFieldExt(RF_DATE5, DataType::DT_DATE);
	ResetFieldExt(RF_DATE6, DataType::DT_DATE);
	ResetFieldExt(RF_DATE7, DataType::DT_DATE);
	ResetFieldExt(RF_DATE8, DataType::DT_DATE);
	ResetFieldExt(RF_DATE9, DataType::DT_DATE);
	ResetFieldExt(RF_DATE10, DataType::DT_DATE);
	ResetFieldExt(RF_DURATION4, DataType::DT_DURATION, _Resource2Fields[RF_DURATION4_UNITS].get());
	ResetFieldExt(RF_DURATION5, DataType::DT_DURATION, _Resource2Fields[RF_DURATION5_UNITS].get());
	ResetFieldExt(RF_DURATION6, DataType::DT_DURATION, _Resource2Fields[RF_DURATION6_UNITS].get());
	ResetFieldExt(RF_DURATION7, DataType::DT_DURATION, _Resource2Fields[RF_DURATION7_UNITS].get());
	ResetFieldExt(RF_DURATION8, DataType::DT_DURATION, _Resource2Fields[RF_DURATION8_UNITS].get());
	ResetFieldExt(RF_DURATION9, DataType::DT_DURATION, _Resource2Fields[RF_DURATION9_UNITS].get());
	ResetFieldExt(RF_DURATION10, DataType::DT_DURATION, _Resource2Fields[RF_DURATION10_UNITS].get());
	ResetFieldExt(RF_FINISH6, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH7, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH8, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH9, DataType::DT_DATE);
	ResetFieldExt(RF_FINISH10, DataType::DT_DATE);
	ResetFieldExt(RF_FLAG11, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG12, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG13, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG14, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG15, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG16, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG17, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG18, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG19, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_FLAG20, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_NUMBER6, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER7, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER8, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER9, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER10, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER11, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER12, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER13, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER14, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER15, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER16, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER17, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER18, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER19, DataType::DT_NUMERIC);
	ResetFieldExt(RF_NUMBER20, DataType::DT_NUMERIC);
	ResetFieldExt(RF_START6, DataType::DT_DATE);
	ResetFieldExt(RF_START7, DataType::DT_DATE);
	ResetFieldExt(RF_START8, DataType::DT_DATE);
	ResetFieldExt(RF_START9, DataType::DT_DATE);
	ResetFieldExt(RF_START10, DataType::DT_DATE);
	ResetFieldExt(RF_TEXT11, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT12, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT13, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT14, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT15, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT16, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT17, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT18, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT19, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT20, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT21, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT22, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT23, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT24, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT25, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT26, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT27, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT28, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT29, DataType::DT_STRING);
	ResetFieldExt(RF_TEXT30, DataType::DT_STRING);
	ResetFieldExt(RF_PHONETICS, DataType::DT_STRING);
	ResetFieldExt(RF_ASSIGNMENT_DELAY, DataType::DT_STRING);
	ResetFieldExt(RF_ASSIGNMENT_UNITS, DataType::DT_STRING);
	ResetFieldExt(RF_BASELINE_START, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_CONFIRMED, DataType::DT_BOOLEAN);

	ResetFieldExt(RF_LEVELING_DELAY, DataType::DT_STRING);
	ResetFieldExt(RF_RESPONSE_PENDING, DataType::DT_BOOLEAN);

	ResetFieldExt(RF_TEAMSTATUS_PENDING, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_CV, DataType::DT_NUMERIC);
	ResetFieldExt(RF_UPDATE_NEEDED, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_COST_RATE_TABLE, DataType::DT_STRING);
	ResetFieldExt(RF_ACTUAL_START, DataType::DT_DATE);
	ResetFieldExt(RF_ACTUAL_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_WORKGROUP, DataType::DT_WORKGROUP);
	ResetFieldExt(RF_PROJECT, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE1, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE2, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE3, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE4, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE5, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE6, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE7, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE8, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE9, DataType::DT_STRING);
	ResetFieldExt(RF_OUTLINE_CODE10, DataType::DT_STRING);
	ResetFieldExt(RF_MATERIAL_LABEL, DataType::DT_STRING);
	ResetFieldExt(RF_TYPE, DataType::DT_RESOURCE_TYPE);
	ResetFieldExt(RF_VAC, DataType::DT_CURRENCY);
	ResetFieldExt(RF_GROUP_BY_SUMMARY, DataType::DT_STRING);
	ResetFieldExt(RF_WINDOWS_USER_ACCOUNT, DataType::DT_STRING);
	ResetFieldExt(RF_BASELINE1_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE1_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE2_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE2_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE3_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE3_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE4_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE4_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE5_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE5_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE6_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE6_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE7_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE7_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE8_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE8_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE9_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE9_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE10_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE10_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST1, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST2, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST3, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST4, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST5, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST6, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST7, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST8, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST9, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_COST10, DataType::DT_CURRENCY);
	ResetFieldExt(RF_ENTERPRISE_DATE1, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE2, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE3, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE4, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE5, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE6, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE7, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE8, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE9, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE10, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE11, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE12, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE13, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE14, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE15, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE16, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE17, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE18, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE19, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE20, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE21, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE22, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE23, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE24, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE25, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE26, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE27, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE28, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE29, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DATE30, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_DURATION1_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION2_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION3_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION4_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION5_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION6_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION7_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION8_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION9_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION10_UNITS, DataType::DT_TIME_UNITS);
	ResetFieldExt(RF_ENTERPRISE_DURATION1, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION1_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION2, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION2_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION3, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION3_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION4, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION4_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION5, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION5_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION6, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION6_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION7, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION7_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION8, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION8_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION9, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION9_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_DURATION10, DataType::DT_DURATION, _Resource2Fields[RF_ENTERPRISE_DURATION10_UNITS].get());
	ResetFieldExt(RF_ENTERPRISE_FLAG1, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG2, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG3, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG4, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG5, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG6, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG7, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG8, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG9, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG10, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG11, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG12, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG13, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG14, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG15, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG16, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG17, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG18, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG19, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_FLAG20, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_NUMBER1, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER2, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER3, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER4, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER5, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER6, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER7, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER8, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER9, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER10, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER11, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER12, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER13, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER14, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER15, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER16, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER17, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER18, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER19, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER20, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER21, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER22, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER23, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER24, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER25, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER26, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER27, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER28, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER29, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER30, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER31, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER32, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER33, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER34, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER35, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER36, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER37, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER38, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER39, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_NUMBER40, DataType::DT_NUMERIC);
	ResetFieldExt(RF_ENTERPRISE_TEXT1, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT2, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT3, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT4, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT5, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT6, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT7, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT8, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT9, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT10, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT11, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT12, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT13, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT14, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT15, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT16, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT17, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT18, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT19, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT20, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT21, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT22, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT23, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT24, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT25, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT26, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT27, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT28, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT29, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT30, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT31, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT32, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT33, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT34, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT35, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT36, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT37, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT38, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT39, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEXT40, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_IMPORT, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_CREATED, DataType::DT_DATE);
	ResetFieldExt(RF_BUDGET, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_TASK_OUTLINE_NUMBER, DataType::DT_STRING);
	ResetFieldExt(RF_GUID, DataType::DT_GUID);

	ResetFieldExt(RF_ACTIVE, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ASSIGNMENT_OWNER, DataType::DT_STRING);
	ResetFieldExt(RF_BASELINE_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE_BUDGET_WORK, DataType::DT_WORK);

	ResetFieldExt(RF_BASELINE1_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE1_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE1_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE1_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE2_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE2_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE2_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE2_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE3_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE3_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE3_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE3_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE4_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE4_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE4_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE4_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE5_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE5_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE5_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE5_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE6_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE6_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE6_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE6_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE7_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE7_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE7_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE7_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE8_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE8_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE8_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE8_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE9_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE9_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE9_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE9_START, DataType::DT_DATE);

	ResetFieldExt(RF_BASELINE10_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BASELINE10_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_BASELINE10_FINISH, DataType::DT_DATE);
	ResetFieldExt(RF_BASELINE10_START, DataType::DT_DATE);

	ResetFieldExt(RF_BOOKING_TYPE, DataType::DT_STRING);
	ResetFieldExt(RF_BUDGET_COST, DataType::DT_CURRENCY);
	ResetFieldExt(RF_BUDGET_WORK, DataType::DT_WORK);
	ResetFieldExt(RF_CALENDAR_GUID, DataType::DT_GUID);
	ResetFieldExt(RF_COST_CENTER, DataType::DT_STRING);
	ResetFieldExt(RF_DEFAULT_ASSIGNMENT_OWNER, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_BASE_CALENDAR, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_REQUIRED_VALUES, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_TEAM_MEMBER, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_UNIQUE_ID, DataType::DT_INTEGER);
	ResetFieldExt(RF_ERROR_MESSAGE, DataType::DT_STRING);
	ResetFieldExt(RF_GENERIC, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_INACTIVE, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_REQUEST_DEMAND, DataType::DT_STRING);
	ResetFieldExt(RF_SUMMARY, DataType::DT_STRING);
	ResetFieldExt(RF_TEAM_ASSIGNMENT_POOL, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_WBS, DataType::DT_STRING);
	ResetFieldExt(RF_UNAVAILABLE, DataType::DT_STRING); // Dummy entry
	ResetFieldExt(RF_SUBPROJECT_RESOURCE_UNIQUE_ID, DataType::DT_INTEGER);
	ResetFieldExt(RF_HYPERLINK_DATA, DataType::DT_BINARY);
	ResetFieldExt(RF_OUTLINE_CODE1_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE2_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE3_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE4_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE5_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE6_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE7_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE8_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE9_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_OUTLINE_CODE10_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_COST_RATE_A, DataType::DT_BINARY);
	ResetFieldExt(RF_COST_RATE_B, DataType::DT_BINARY);
	ResetFieldExt(RF_COST_RATE_C, DataType::DT_BINARY);
	ResetFieldExt(RF_COST_RATE_D, DataType::DT_BINARY);
	ResetFieldExt(RF_COST_RATE_E, DataType::DT_BINARY);
	ResetFieldExt(RF_AVAILABILITY_DATA, DataType::DT_BINARY);
	ResetFieldExt(RF_ENTERPRISE_DATA, DataType::DT_BINARY);
	ResetFieldExt(RF_OVERTIME_RATE_UNITS, DataType::DT_RATE_UNITS);
	ResetFieldExt(RF_STANDARD_RATE_UNITS, DataType::DT_RATE_UNITS);

	ResetFieldExt(RF_INDEX, DataType::DT_INTEGER);
	ResetFieldExt(RF_HYPERLINK_SCREEN_TIP, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE1, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE2, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE3, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE4, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE5, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE6, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE7, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE8, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE9, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE10, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE11, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE12, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE13, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE14, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE15, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE16, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE17, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE18, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE19, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE20, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE21, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE22, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE23, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE24, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE25, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE26, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE27, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE28, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_OUTLINE_CODE29, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_RBS, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_NAME_USED, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_IS_CHECKED_OUT, DataType::DT_BOOLEAN);
	ResetFieldExt(RF_ENTERPRISE_CHECKED_OUT_BY, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_LAST_MODIFIED_DATE, DataType::DT_DATE);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE20, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE21, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE22, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE23, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE24, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE25, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE26, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE27, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE28, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_MULTI_VALUE29, DataType::DT_STRING);
	ResetFieldExt(RF_ACTUAL_WORK_PROTECTED, DataType::DT_WORK);
	ResetFieldExt(RF_ACTUAL_OVERTIME_WORK_PROTECTED, DataType::DT_WORK);

	// KEEP THESE TOGETHER AND IN ORDER
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD1, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD2, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD3, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD4, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD5, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD6, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD7, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD8, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD9, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD10, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD11, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD12, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD13, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD14, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD15, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD16, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD17, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD18, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD19, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD20, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD21, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD22, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD23, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD24, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD25, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD26, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD27, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD28, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD29, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD30, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD31, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD32, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD33, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD34, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD35, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD36, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD37, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD38, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD39, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD40, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD41, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD42, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD43, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD44, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD45, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD46, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD47, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD48, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD49, DataType::DT_STRING);
	ResetFieldExt(RF_ENTERPRISE_CUSTOM_FIELD50, DataType::DT_STRING);
	// KEEP THESE TOGETHER AND IN ORDER

	ResetFieldExt(RF_FINISH, DataType::DT_DATE); // Must always be last value
};

FieldType* Resource2FieldClass::getInstance(int type)
{
	if (_Resource2Fields[RF_FINISH - 1].get() == NULL)
	{
		InstanceAllResource2Field();
	}

	return _Resource2Fields[type].get();
}