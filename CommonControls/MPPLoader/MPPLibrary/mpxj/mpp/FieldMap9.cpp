#include "stdafx.h"
#include "FieldMap9.h"
#include "..\AssignmentField.h"
#include "..\ResourceField.h"
#include "..\TaskField.h"
#include "..\common\FieldTypeHelper.h"

//CAtlArray<std::pair<int, int>> FieldMap9::VAR_DATA_MAP_CONTENT;
//Map<FieldType*, int> FieldMap9::VAR_DATA_MAP;

/**
	* Constructor.
	*
	* @param file parent file
	*/
FieldMap9::FieldMap9(ProjectFile* file)
:FieldMap(file)
{
}

/**
	* {@inheritDoc}
	*/
FieldType* FieldMap9::getFieldType(int fieldID)
{
	//ATLASSERT(FALSE);
	//return NULL;
	return FieldTypeHelper::getInstance(fieldID);
}

/**
	* {@inheritDoc}
	*/
bool FieldMap9::useTypeAsVarDataKey()
{
	return false;
}

/**
	* {@inheritDoc}
	*/
bool FieldMap9::substituteVarDataKey(FieldType* type, int& value)
{
	return false;
}

template<class ARRAY_PTR>
void SetFieldItem(ARRAY_PTR ArrayPtr, FieldMap* fieldMap, FieldType* type, FieldLocation location,
	int fixedDataBlockIndex, int fixedDataOffset, int varDataKey, long mask, int metaBlock)
{
	FieldItemPtr item(
		new FieldItem(fieldMap, type, location, fixedDataBlockIndex, fixedDataOffset, varDataKey, mask, metaBlock)
		);

	(*ArrayPtr)[type->getValue()] = item;
}

/**
	* {@inheritDoc}
	*/
FieldItemArrayPtr FieldMap9::getDefaultTaskData()
{
	static FieldItemArrayPtr result(new FieldItemArray(TaskField::TF_FINISH));

	if (result->length() == 0)
	{
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 0, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ID), FieldLocation::FIXED_DATA, 0, 4, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_EARLY_FINISH), FieldLocation::FIXED_DATA, 0, 8, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_LATE_START), FieldLocation::FIXED_DATA, 0, 12, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_STOP), FieldLocation::FIXED_DATA, 0, 16, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_RESUME), FieldLocation::FIXED_DATA, 0, 20, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FREE_SLACK), FieldLocation::FIXED_DATA, 0, 24, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START_SLACK), FieldLocation::FIXED_DATA, 0, 28, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH_SLACK), FieldLocation::FIXED_DATA, 0, 32, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_PARENT_TASK_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 36, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_LEVEL), FieldLocation::FIXED_DATA, 0, 40, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION_UNITS), FieldLocation::FIXED_DATA, 0, 58, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION), FieldLocation::FIXED_DATA, 0, 60, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_DURATION_UNITS), FieldLocation::FIXED_DATA, 0, 64, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_DURATION), FieldLocation::FIXED_DATA, 0, 66, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_REMAINING_DURATION), FieldLocation::FIXED_DATA, 0, 70, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_BASELINE_DURATION), FieldLocation::FIXED_DATA, 0, 74, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_BASELINE_DURATION_UNITS), FieldLocation::FIXED_DATA, 0, 78, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_CONSTRAINT_TYPE), FieldLocation::FIXED_DATA, 0, 80, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_LEVELING_DELAY), FieldLocation::FIXED_DATA, 0, 82, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_LEVELING_DELAY_UNITS), FieldLocation::FIXED_DATA, 0, 86, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START), FieldLocation::FIXED_DATA, 0, 88, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH), FieldLocation::FIXED_DATA, 0, 92, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_START), FieldLocation::FIXED_DATA, 0, 96, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_FINISH), FieldLocation::FIXED_DATA, 0, 100, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_BASELINE_START), FieldLocation::FIXED_DATA, 0, 104, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_BASELINE_FINISH), FieldLocation::FIXED_DATA, 0, 108, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_CONSTRAINT_DATE), FieldLocation::FIXED_DATA, 0, 112, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_RESUME_NO_EARLIER_THAN), FieldLocation::FIXED_DATA, 0, 116, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_PRIORITY), FieldLocation::FIXED_DATA, 0, 120, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_PERCENT_COMPLETE), FieldLocation::FIXED_DATA, 0, 122, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_PERCENT_WORK_COMPLETE), FieldLocation::FIXED_DATA, 0, 124, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TYPE), FieldLocation::FIXED_DATA, 0, 126, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FIXED_COST_ACCRUAL), FieldLocation::FIXED_DATA, 0, 128, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_CREATED), FieldLocation::FIXED_DATA, 0, 130, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_RECURRING), FieldLocation::FIXED_DATA, 0, 134, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_PRELEVELED_START), FieldLocation::FIXED_DATA, 0, 136, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_PRELEVELED_FINISH), FieldLocation::FIXED_DATA, 0, 140, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_EARLY_START), FieldLocation::FIXED_DATA, 0, 148, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_LATE_FINISH), FieldLocation::FIXED_DATA, 0, 152, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_SUMMARY_PROGRESS), FieldLocation::FIXED_DATA, 0, 156, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_CALENDAR_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 160, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DEADLINE), FieldLocation::FIXED_DATA, 0, 164, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_WORK), FieldLocation::FIXED_DATA, 0, 168, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_BASELINE_WORK), FieldLocation::FIXED_DATA, 0, 176, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_WORK), FieldLocation::FIXED_DATA, 0, 184, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_REMAINING_WORK), FieldLocation::FIXED_DATA, 0, 192, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST), FieldLocation::FIXED_DATA, 0, 200, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FIXED_COST), FieldLocation::FIXED_DATA, 0, 208, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_COST), FieldLocation::FIXED_DATA, 0, 216, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_REMAINING_COST), FieldLocation::FIXED_DATA, 0, 224, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_BASELINE_COST), FieldLocation::FIXED_DATA, 0, 232, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_BASELINE_FIXED_COST), FieldLocation::FIXED_DATA, 0, 256, 0, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_OVERTIME_WORK), FieldLocation::VAR_DATA, 0, 65535, 3, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_REMAINING_OVERTIME_WORK), FieldLocation::VAR_DATA, 0, 65535, 4, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OVERTIME_COST), FieldLocation::VAR_DATA, 0, 65535, 5, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ACTUAL_OVERTIME_COST), FieldLocation::VAR_DATA, 0, 65535, 6, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_REMAINING_OVERTIME_COST), FieldLocation::VAR_DATA, 0, 65535, 7, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_SUBPROJECT_TASKS_UNIQUEID_OFFSET), FieldLocation::VAR_DATA, 0, 65535, 8, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_SUBPROJECT_UNIQUE_TASK_ID), FieldLocation::VAR_DATA, 0, 65535, 9, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_WBS), FieldLocation::VAR_DATA, 0, 65535, 10, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NAME), FieldLocation::VAR_DATA, 0, 65535, 11, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_CONTACT), FieldLocation::VAR_DATA, 0, 65535, 12, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT1), FieldLocation::VAR_DATA, 0, 65535, 14, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT2), FieldLocation::VAR_DATA, 0, 65535, 15, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT3), FieldLocation::VAR_DATA, 0, 65535, 16, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT4), FieldLocation::VAR_DATA, 0, 65535, 17, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT5), FieldLocation::VAR_DATA, 0, 65535, 18, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT6), FieldLocation::VAR_DATA, 0, 65535, 19, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT7), FieldLocation::VAR_DATA, 0, 65535, 20, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT8), FieldLocation::VAR_DATA, 0, 65535, 21, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT9), FieldLocation::VAR_DATA, 0, 65535, 22, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT10), FieldLocation::VAR_DATA, 0, 65535, 23, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START1), FieldLocation::VAR_DATA, 0, 65535, 24, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH1), FieldLocation::VAR_DATA, 0, 65535, 25, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START2), FieldLocation::VAR_DATA, 0, 65535, 26, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH2), FieldLocation::VAR_DATA, 0, 65535, 27, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START3), FieldLocation::VAR_DATA, 0, 65535, 28, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH3), FieldLocation::VAR_DATA, 0, 65535, 29, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START4), FieldLocation::VAR_DATA, 0, 65535, 30, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH4), FieldLocation::VAR_DATA, 0, 65535, 31, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START5), FieldLocation::VAR_DATA, 0, 65535, 32, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH5), FieldLocation::VAR_DATA, 0, 65535, 33, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START6), FieldLocation::VAR_DATA, 0, 65535, 34, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH6), FieldLocation::VAR_DATA, 0, 65535, 35, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START7), FieldLocation::VAR_DATA, 0, 65535, 36, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH7), FieldLocation::VAR_DATA, 0, 65535, 37, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START8), FieldLocation::VAR_DATA, 0, 65535, 38, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH8), FieldLocation::VAR_DATA, 0, 65535, 39, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START9), FieldLocation::VAR_DATA, 0, 65535, 40, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH9), FieldLocation::VAR_DATA, 0, 65535, 41, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_START10), FieldLocation::VAR_DATA, 0, 65535, 42, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_FINISH10), FieldLocation::VAR_DATA, 0, 65535, 43, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER1), FieldLocation::VAR_DATA, 0, 65535, 45, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER2), FieldLocation::VAR_DATA, 0, 65535, 46, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER3), FieldLocation::VAR_DATA, 0, 65535, 47, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER4), FieldLocation::VAR_DATA, 0, 65535, 48, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER5), FieldLocation::VAR_DATA, 0, 65535, 49, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER6), FieldLocation::VAR_DATA, 0, 65535, 50, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER7), FieldLocation::VAR_DATA, 0, 65535, 51, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER8), FieldLocation::VAR_DATA, 0, 65535, 52, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER9), FieldLocation::VAR_DATA, 0, 65535, 53, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER10), FieldLocation::VAR_DATA, 0, 65535, 54, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION1), FieldLocation::VAR_DATA, 0, 65535, 55, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION1_UNITS), FieldLocation::VAR_DATA, 0, 65535, 56, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION2), FieldLocation::VAR_DATA, 0, 65535, 57, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION2_UNITS), FieldLocation::VAR_DATA, 0, 65535, 58, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION3), FieldLocation::VAR_DATA, 0, 65535, 59, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION3_UNITS), FieldLocation::VAR_DATA, 0, 65535, 60, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION4), FieldLocation::VAR_DATA, 0, 65535, 61, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION4_UNITS), FieldLocation::VAR_DATA, 0, 65535, 62, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION5), FieldLocation::VAR_DATA, 0, 65535, 63, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION5_UNITS), FieldLocation::VAR_DATA, 0, 65535, 64, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION6), FieldLocation::VAR_DATA, 0, 65535, 65, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION6_UNITS), FieldLocation::VAR_DATA, 0, 65535, 66, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION7), FieldLocation::VAR_DATA, 0, 65535, 67, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION7_UNITS), FieldLocation::VAR_DATA, 0, 65535, 68, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION8), FieldLocation::VAR_DATA, 0, 65535, 69, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION8_UNITS), FieldLocation::VAR_DATA, 0, 65535, 70, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION9), FieldLocation::VAR_DATA, 0, 65535, 71, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION9_UNITS), FieldLocation::VAR_DATA, 0, 65535, 72, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION10), FieldLocation::VAR_DATA, 0, 65535, 73, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DURATION10_UNITS), FieldLocation::VAR_DATA, 0, 65535, 74, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_RECURRING_DATA), FieldLocation::VAR_DATA, 0, 65535, 76, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_SUBPROJECT_TASK_ID), FieldLocation::VAR_DATA, 0, 65535, 79, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE1), FieldLocation::VAR_DATA, 0, 65535, 80, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE2), FieldLocation::VAR_DATA, 0, 65535, 81, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE3), FieldLocation::VAR_DATA, 0, 65535, 82, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE4), FieldLocation::VAR_DATA, 0, 65535, 83, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE5), FieldLocation::VAR_DATA, 0, 65535, 84, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE6), FieldLocation::VAR_DATA, 0, 65535, 85, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE7), FieldLocation::VAR_DATA, 0, 65535, 86, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE8), FieldLocation::VAR_DATA, 0, 65535, 87, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE9), FieldLocation::VAR_DATA, 0, 65535, 88, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_DATE10), FieldLocation::VAR_DATA, 0, 65535, 89, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT11), FieldLocation::VAR_DATA, 0, 65535, 90, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT12), FieldLocation::VAR_DATA, 0, 65535, 91, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT13), FieldLocation::VAR_DATA, 0, 65535, 92, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT14), FieldLocation::VAR_DATA, 0, 65535, 93, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT15), FieldLocation::VAR_DATA, 0, 65535, 94, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT16), FieldLocation::VAR_DATA, 0, 65535, 95, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT17), FieldLocation::VAR_DATA, 0, 65535, 96, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT18), FieldLocation::VAR_DATA, 0, 65535, 97, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT19), FieldLocation::VAR_DATA, 0, 65535, 98, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT20), FieldLocation::VAR_DATA, 0, 65535, 99, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT21), FieldLocation::VAR_DATA, 0, 65535, 100, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT22), FieldLocation::VAR_DATA, 0, 65535, 101, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT23), FieldLocation::VAR_DATA, 0, 65535, 102, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT24), FieldLocation::VAR_DATA, 0, 65535, 103, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT25), FieldLocation::VAR_DATA, 0, 65535, 104, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT26), FieldLocation::VAR_DATA, 0, 65535, 105, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT27), FieldLocation::VAR_DATA, 0, 65535, 106, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT28), FieldLocation::VAR_DATA, 0, 65535, 107, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT29), FieldLocation::VAR_DATA, 0, 65535, 108, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_TEXT30), FieldLocation::VAR_DATA, 0, 65535, 109, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER11), FieldLocation::VAR_DATA, 0, 65535, 110, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER12), FieldLocation::VAR_DATA, 0, 65535, 111, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER13), FieldLocation::VAR_DATA, 0, 65535, 112, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER14), FieldLocation::VAR_DATA, 0, 65535, 113, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER15), FieldLocation::VAR_DATA, 0, 65535, 114, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER16), FieldLocation::VAR_DATA, 0, 65535, 115, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER17), FieldLocation::VAR_DATA, 0, 65535, 116, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER18), FieldLocation::VAR_DATA, 0, 65535, 117, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER19), FieldLocation::VAR_DATA, 0, 65535, 118, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NUMBER20), FieldLocation::VAR_DATA, 0, 65535, 119, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE1_INDEX), FieldLocation::VAR_DATA, 0, 65535, 123, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE2_INDEX), FieldLocation::VAR_DATA, 0, 65535, 124, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE3_INDEX), FieldLocation::VAR_DATA, 0, 65535, 125, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE4_INDEX), FieldLocation::VAR_DATA, 0, 65535, 126, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE5_INDEX), FieldLocation::VAR_DATA, 0, 65535, 127, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE6_INDEX), FieldLocation::VAR_DATA, 0, 65535, 128, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE7_INDEX), FieldLocation::VAR_DATA, 0, 65535, 129, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE8_INDEX), FieldLocation::VAR_DATA, 0, 65535, 130, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE9_INDEX), FieldLocation::VAR_DATA, 0, 65535, 131, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_OUTLINE_CODE10_INDEX), FieldLocation::VAR_DATA, 0, 65535, 132, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_HYPERLINK_DATA), FieldLocation::VAR_DATA, 0, 65535, 133, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST1), FieldLocation::VAR_DATA, 0, 65535, 134, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST2), FieldLocation::VAR_DATA, 0, 65535, 135, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST3), FieldLocation::VAR_DATA, 0, 65535, 136, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST4), FieldLocation::VAR_DATA, 0, 65535, 137, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST5), FieldLocation::VAR_DATA, 0, 65535, 138, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST6), FieldLocation::VAR_DATA, 0, 65535, 139, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST7), FieldLocation::VAR_DATA, 0, 65535, 140, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST8), FieldLocation::VAR_DATA, 0, 65535, 141, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST9), FieldLocation::VAR_DATA, 0, 65535, 142, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_COST10), FieldLocation::VAR_DATA, 0, 65535, 143, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_NOTES), FieldLocation::VAR_DATA, 0, 65535, 144, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_SUBPROJECT_FILE), FieldLocation::VAR_DATA, 0, 65535, 160, 0, 0);
		SetFieldItem(result, this, TaskFieldClass::getInstance(TaskField::TF_ENTERPRISE_DATA), FieldLocation::VAR_DATA, 0, 65535, 163, 0, 0);
	};
	return result;
}

/**
	* {@inheritDoc}
	*/
FieldItemArrayPtr FieldMap9::getDefaultResource2Data()
{
	FieldItemArrayPtr result (new FieldItemArray(Resource2Field::RF_FINISH));
	if (result->length() == 0)
	{
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 0, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ID), FieldLocation::FIXED_DATA, 0, 4, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_STANDARD_RATE_UNITS), FieldLocation::FIXED_DATA, 0, 8, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OVERTIME_RATE_UNITS), FieldLocation::FIXED_DATA, 0, 10, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ACCRUE_AT), FieldLocation::FIXED_DATA, 0, 12, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_WORKGROUP), FieldLocation::FIXED_DATA, 0, 14, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_AVAILABLE_FROM), FieldLocation::FIXED_DATA, 0, 20, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_AVAILABLE_TO), FieldLocation::FIXED_DATA, 0, 24, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_STANDARD_RATE), FieldLocation::FIXED_DATA, 0, 28, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OVERTIME_RATE), FieldLocation::FIXED_DATA, 0, 36, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_MAX_UNITS), FieldLocation::FIXED_DATA, 0, 44, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_WORK), FieldLocation::FIXED_DATA, 0, 52, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ACTUAL_WORK), FieldLocation::FIXED_DATA, 0, 60, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE_WORK), FieldLocation::FIXED_DATA, 0, 68, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OVERTIME_WORK), FieldLocation::FIXED_DATA, 0, 76, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST_PER_USE), FieldLocation::FIXED_DATA, 0, 84, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_REMAINING_WORK), FieldLocation::FIXED_DATA, 0, 92, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_REGULAR_WORK), FieldLocation::FIXED_DATA, 0, 100, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ACTUAL_OVERTIME_WORK), FieldLocation::FIXED_DATA, 0, 108, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_REMAINING_OVERTIME_WORK), FieldLocation::FIXED_DATA, 0, 116, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_PEAK), FieldLocation::FIXED_DATA, 0, 124, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ACTUAL_COST), FieldLocation::FIXED_DATA, 0, 132, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST), FieldLocation::FIXED_DATA, 0, 140, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE_COST), FieldLocation::FIXED_DATA, 0, 148, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_REMAINING_COST), FieldLocation::FIXED_DATA, 0, 156, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OVERTIME_COST), FieldLocation::FIXED_DATA, 0, 164, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ACTUAL_OVERTIME_COST), FieldLocation::FIXED_DATA, 0, 172, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_REMAINING_OVERTIME_COST), FieldLocation::FIXED_DATA, 0, 180, 0, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NAME), FieldLocation::VAR_DATA, 0, 65535, 1, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_INITIALS), FieldLocation::VAR_DATA, 0, 65535, 3, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_GROUP), FieldLocation::VAR_DATA, 0, 65535, 4, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_CODE), FieldLocation::VAR_DATA, 0, 65535, 5, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_EMAIL_ADDRESS), FieldLocation::VAR_DATA, 0, 65535, 6, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_PHONETICS), FieldLocation::VAR_DATA, 0, 65535, 7, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_MATERIAL_LABEL), FieldLocation::VAR_DATA, 0, 65535, 8, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_WINDOWS_USER_ACCOUNT), FieldLocation::VAR_DATA, 0, 65535, 9, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT1), FieldLocation::VAR_DATA, 0, 65535, 10, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT2), FieldLocation::VAR_DATA, 0, 65535, 11, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT3), FieldLocation::VAR_DATA, 0, 65535, 12, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT4), FieldLocation::VAR_DATA, 0, 65535, 13, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT5), FieldLocation::VAR_DATA, 0, 65535, 14, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT6), FieldLocation::VAR_DATA, 0, 65535, 15, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT7), FieldLocation::VAR_DATA, 0, 65535, 16, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT8), FieldLocation::VAR_DATA, 0, 65535, 17, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT9), FieldLocation::VAR_DATA, 0, 65535, 18, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT10), FieldLocation::VAR_DATA, 0, 65535, 19, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT11), FieldLocation::VAR_DATA, 0, 65535, 20, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT12), FieldLocation::VAR_DATA, 0, 65535, 21, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT13), FieldLocation::VAR_DATA, 0, 65535, 22, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT14), FieldLocation::VAR_DATA, 0, 65535, 23, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT15), FieldLocation::VAR_DATA, 0, 65535, 24, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT16), FieldLocation::VAR_DATA, 0, 65535, 25, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT17), FieldLocation::VAR_DATA, 0, 65535, 26, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT18), FieldLocation::VAR_DATA, 0, 65535, 27, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT19), FieldLocation::VAR_DATA, 0, 65535, 28, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT20), FieldLocation::VAR_DATA, 0, 65535, 29, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT21), FieldLocation::VAR_DATA, 0, 65535, 30, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT22), FieldLocation::VAR_DATA, 0, 65535, 31, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT23), FieldLocation::VAR_DATA, 0, 65535, 32, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT24), FieldLocation::VAR_DATA, 0, 65535, 33, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT25), FieldLocation::VAR_DATA, 0, 65535, 34, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT26), FieldLocation::VAR_DATA, 0, 65535, 35, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT27), FieldLocation::VAR_DATA, 0, 65535, 36, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT28), FieldLocation::VAR_DATA, 0, 65535, 37, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT29), FieldLocation::VAR_DATA, 0, 65535, 38, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_TEXT30), FieldLocation::VAR_DATA, 0, 65535, 39, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START1), FieldLocation::VAR_DATA, 0, 65535, 40, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START2), FieldLocation::VAR_DATA, 0, 65535, 41, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START3), FieldLocation::VAR_DATA, 0, 65535, 42, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START4), FieldLocation::VAR_DATA, 0, 65535, 43, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START5), FieldLocation::VAR_DATA, 0, 65535, 44, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START6), FieldLocation::VAR_DATA, 0, 65535, 45, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START7), FieldLocation::VAR_DATA, 0, 65535, 46, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START8), FieldLocation::VAR_DATA, 0, 65535, 47, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START9), FieldLocation::VAR_DATA, 0, 65535, 48, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_START10), FieldLocation::VAR_DATA, 0, 65535, 49, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH1), FieldLocation::VAR_DATA, 0, 65535, 50, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH2), FieldLocation::VAR_DATA, 0, 65535, 51, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH3), FieldLocation::VAR_DATA, 0, 65535, 52, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH4), FieldLocation::VAR_DATA, 0, 65535, 53, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH5), FieldLocation::VAR_DATA, 0, 65535, 54, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH6), FieldLocation::VAR_DATA, 0, 65535, 55, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH7), FieldLocation::VAR_DATA, 0, 65535, 56, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH8), FieldLocation::VAR_DATA, 0, 65535, 57, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH9), FieldLocation::VAR_DATA, 0, 65535, 58, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_FINISH10), FieldLocation::VAR_DATA, 0, 65535, 59, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER1), FieldLocation::VAR_DATA, 0, 65535, 60, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER2), FieldLocation::VAR_DATA, 0, 65535, 61, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER3), FieldLocation::VAR_DATA, 0, 65535, 62, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER4), FieldLocation::VAR_DATA, 0, 65535, 63, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER5), FieldLocation::VAR_DATA, 0, 65535, 64, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER6), FieldLocation::VAR_DATA, 0, 65535, 65, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER7), FieldLocation::VAR_DATA, 0, 65535, 66, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER8), FieldLocation::VAR_DATA, 0, 65535, 67, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER9), FieldLocation::VAR_DATA, 0, 65535, 68, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER10), FieldLocation::VAR_DATA, 0, 65535, 69, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER11), FieldLocation::VAR_DATA, 0, 65535, 70, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER12), FieldLocation::VAR_DATA, 0, 65535, 71, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER13), FieldLocation::VAR_DATA, 0, 65535, 72, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER14), FieldLocation::VAR_DATA, 0, 65535, 73, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER15), FieldLocation::VAR_DATA, 0, 65535, 74, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER16), FieldLocation::VAR_DATA, 0, 65535, 75, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER17), FieldLocation::VAR_DATA, 0, 65535, 76, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER18), FieldLocation::VAR_DATA, 0, 65535, 77, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER19), FieldLocation::VAR_DATA, 0, 65535, 78, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NUMBER20), FieldLocation::VAR_DATA, 0, 65535, 79, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION1), FieldLocation::VAR_DATA, 0, 65535, 80, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION2), FieldLocation::VAR_DATA, 0, 65535, 81, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION3), FieldLocation::VAR_DATA, 0, 65535, 82, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION4), FieldLocation::VAR_DATA, 0, 65535, 83, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION5), FieldLocation::VAR_DATA, 0, 65535, 84, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION6), FieldLocation::VAR_DATA, 0, 65535, 85, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION7), FieldLocation::VAR_DATA, 0, 65535, 86, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION8), FieldLocation::VAR_DATA, 0, 65535, 87, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION9), FieldLocation::VAR_DATA, 0, 65535, 88, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION10), FieldLocation::VAR_DATA, 0, 65535, 89, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION1_UNITS), FieldLocation::VAR_DATA, 0, 65535, 90, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION2_UNITS), FieldLocation::VAR_DATA, 0, 65535, 91, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION3_UNITS), FieldLocation::VAR_DATA, 0, 65535, 92, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION4_UNITS), FieldLocation::VAR_DATA, 0, 65535, 93, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION5_UNITS), FieldLocation::VAR_DATA, 0, 65535, 94, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION6_UNITS), FieldLocation::VAR_DATA, 0, 65535, 95, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION7_UNITS), FieldLocation::VAR_DATA, 0, 65535, 96, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION8_UNITS), FieldLocation::VAR_DATA, 0, 65535, 97, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION9_UNITS), FieldLocation::VAR_DATA, 0, 65535, 98, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DURATION10_UNITS), FieldLocation::VAR_DATA, 0, 65535, 99, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_SUBPROJECT_RESOURCE_UNIQUE_ID), FieldLocation::VAR_DATA, 0, 65535, 102, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE1), FieldLocation::VAR_DATA, 0, 65535, 103, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE2), FieldLocation::VAR_DATA, 0, 65535, 104, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE3), FieldLocation::VAR_DATA, 0, 65535, 105, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE4), FieldLocation::VAR_DATA, 0, 65535, 106, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE5), FieldLocation::VAR_DATA, 0, 65535, 107, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE6), FieldLocation::VAR_DATA, 0, 65535, 108, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE7), FieldLocation::VAR_DATA, 0, 65535, 109, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE8), FieldLocation::VAR_DATA, 0, 65535, 110, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE9), FieldLocation::VAR_DATA, 0, 65535, 111, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_DATE10), FieldLocation::VAR_DATA, 0, 65535, 112, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE1_INDEX), FieldLocation::VAR_DATA, 0, 65535, 113, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE2_INDEX), FieldLocation::VAR_DATA, 0, 65535, 114, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE3_INDEX), FieldLocation::VAR_DATA, 0, 65535, 115, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE4_INDEX), FieldLocation::VAR_DATA, 0, 65535, 116, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE5_INDEX), FieldLocation::VAR_DATA, 0, 65535, 117, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE6_INDEX), FieldLocation::VAR_DATA, 0, 65535, 118, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE7_INDEX), FieldLocation::VAR_DATA, 0, 65535, 119, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE8_INDEX), FieldLocation::VAR_DATA, 0, 65535, 120, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE9_INDEX), FieldLocation::VAR_DATA, 0, 65535, 121, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_OUTLINE_CODE10_INDEX), FieldLocation::VAR_DATA, 0, 65535, 122, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_HYPERLINK_DATA), FieldLocation::VAR_DATA, 0, 65535, 123, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_NOTES), FieldLocation::VAR_DATA, 0, 65535, 124, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST1), FieldLocation::VAR_DATA, 0, 65535, 125, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST2), FieldLocation::VAR_DATA, 0, 65535, 126, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST3), FieldLocation::VAR_DATA, 0, 65535, 127, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST4), FieldLocation::VAR_DATA, 0, 65535, 128, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST5), FieldLocation::VAR_DATA, 0, 65535, 129, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST6), FieldLocation::VAR_DATA, 0, 65535, 130, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST7), FieldLocation::VAR_DATA, 0, 65535, 131, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST8), FieldLocation::VAR_DATA, 0, 65535, 132, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST9), FieldLocation::VAR_DATA, 0, 65535, 133, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST10), FieldLocation::VAR_DATA, 0, 65535, 134, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_A), FieldLocation::VAR_DATA, 0, 65535, 135, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_B), FieldLocation::VAR_DATA, 0, 65535, 136, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_C), FieldLocation::VAR_DATA, 0, 65535, 137, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_D), FieldLocation::VAR_DATA, 0, 65535, 138, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_COST_RATE_E), FieldLocation::VAR_DATA, 0, 65535, 139, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_AVAILABILITY_DATA), FieldLocation::VAR_DATA, 0, 65535, 142, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ENTERPRISE_DATA), FieldLocation::VAR_DATA, 0, 65535, 143, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE1_WORK), FieldLocation::VAR_DATA, 0, 65535, 144, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE1_COST), FieldLocation::VAR_DATA, 0, 65535, 145, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE2_WORK), FieldLocation::VAR_DATA, 0, 65535, 148, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE2_COST), FieldLocation::VAR_DATA, 0, 65535, 149, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE3_WORK), FieldLocation::VAR_DATA, 0, 65535, 152, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE3_COST), FieldLocation::VAR_DATA, 0, 65535, 153, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE4_WORK), FieldLocation::VAR_DATA, 0, 65535, 156, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE4_COST), FieldLocation::VAR_DATA, 0, 65535, 157, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE5_WORK), FieldLocation::VAR_DATA, 0, 65535, 160, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE5_COST), FieldLocation::VAR_DATA, 0, 65535, 161, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE6_WORK), FieldLocation::VAR_DATA, 0, 65535, 164, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE6_COST), FieldLocation::VAR_DATA, 0, 65535, 165, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE7_WORK), FieldLocation::VAR_DATA, 0, 65535, 168, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE7_COST), FieldLocation::VAR_DATA, 0, 65535, 169, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE8_WORK), FieldLocation::VAR_DATA, 0, 65535, 172, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE8_COST), FieldLocation::VAR_DATA, 0, 65535, 173, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE9_WORK), FieldLocation::VAR_DATA, 0, 65535, 176, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE9_COST), FieldLocation::VAR_DATA, 0, 65535, 177, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE10_WORK), FieldLocation::VAR_DATA, 0, 65535, 180, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_BASELINE10_COST), FieldLocation::VAR_DATA, 0, 65535, 181, 0, 0);
		SetFieldItem(result, this, Resource2FieldClass::getInstance(Resource2Field::RF_ENTERPRISE_UNIQUE_ID), FieldLocation::VAR_DATA, 0, 65535, 185, 0, 0);
	};
	return result;
}

/**
	* {@inheritDoc}
	*/
FieldItemArrayPtr FieldMap9::getDefaultAssignmentData()
{
	FieldItemArrayPtr result(new FieldItemArray(AssignmentField::AF_FINISH));
	if (result->length() == 0)
	{
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 0, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 0, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TASK_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 4, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_RESOURCE_UNIQUE_ID), FieldLocation::FIXED_DATA, 0, 8, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START), FieldLocation::FIXED_DATA, 0, 12, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH), FieldLocation::FIXED_DATA, 0, 16, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_ASSIGNMENT_DELAY), FieldLocation::FIXED_DATA, 0, 24, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_LEVELING_DELAY_UNITS), FieldLocation::FIXED_DATA, 0, 28, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_LEVELING_DELAY), FieldLocation::FIXED_DATA, 0, 30, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST_RATE_TABLE), FieldLocation::FIXED_DATA, 0, 34, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE_START), FieldLocation::FIXED_DATA, 0, 36, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE_FINISH), FieldLocation::FIXED_DATA, 0, 40, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_VARIABLE_RATE_UNITS), FieldLocation::FIXED_DATA, 0, 52, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_ASSIGNMENT_UNITS), FieldLocation::FIXED_DATA, 0, 54, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_WORK), FieldLocation::FIXED_DATA, 0, 62, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_ACTUAL_WORK), FieldLocation::FIXED_DATA, 0, 70, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_REGULAR_WORK), FieldLocation::FIXED_DATA, 0, 78, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_REMAINING_WORK), FieldLocation::FIXED_DATA, 0, 86, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE_WORK), FieldLocation::FIXED_DATA, 0, 94, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST), FieldLocation::FIXED_DATA, 0, 102, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_ACTUAL_COST), FieldLocation::FIXED_DATA, 0, 110, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_REMAINING_COST), FieldLocation::FIXED_DATA, 0, 118, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE_COST), FieldLocation::FIXED_DATA, 0, 126, 0, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_OVERTIME_WORK), FieldLocation::VAR_DATA, 0, 65535, 1, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_ACTUAL_OVERTIME_WORK), FieldLocation::VAR_DATA, 0, 65535, 2, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_REMAINING_OVERTIME_WORK), FieldLocation::VAR_DATA, 0, 65535, 3, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_ACTUAL_OVERTIME_COST), FieldLocation::VAR_DATA, 0, 65535, 4, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_REMAINING_OVERTIME_COST), FieldLocation::VAR_DATA, 0, 65535, 5, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NOTES), FieldLocation::VAR_DATA, 0, 65535, 6, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TIMEPHASED_WORK), FieldLocation::VAR_DATA, 0, 65535, 7, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TIMEPHASED_ACTUAL_WORK), FieldLocation::VAR_DATA, 0, 65535, 9, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT1), FieldLocation::VAR_DATA, 0, 65535, 15, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT2), FieldLocation::VAR_DATA, 0, 65535, 16, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT3), FieldLocation::VAR_DATA, 0, 65535, 17, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT4), FieldLocation::VAR_DATA, 0, 65535, 18, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT5), FieldLocation::VAR_DATA, 0, 65535, 19, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT6), FieldLocation::VAR_DATA, 0, 65535, 20, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT7), FieldLocation::VAR_DATA, 0, 65535, 21, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT8), FieldLocation::VAR_DATA, 0, 65535, 22, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT9), FieldLocation::VAR_DATA, 0, 65535, 23, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT10), FieldLocation::VAR_DATA, 0, 65535, 24, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT11), FieldLocation::VAR_DATA, 0, 65535, 25, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT12), FieldLocation::VAR_DATA, 0, 65535, 26, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT13), FieldLocation::VAR_DATA, 0, 65535, 27, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT14), FieldLocation::VAR_DATA, 0, 65535, 28, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT15), FieldLocation::VAR_DATA, 0, 65535, 29, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT16), FieldLocation::VAR_DATA, 0, 65535, 30, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT17), FieldLocation::VAR_DATA, 0, 65535, 31, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT18), FieldLocation::VAR_DATA, 0, 65535, 32, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT19), FieldLocation::VAR_DATA, 0, 65535, 33, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT20), FieldLocation::VAR_DATA, 0, 65535, 34, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT21), FieldLocation::VAR_DATA, 0, 65535, 35, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT22), FieldLocation::VAR_DATA, 0, 65535, 36, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT23), FieldLocation::VAR_DATA, 0, 65535, 37, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT24), FieldLocation::VAR_DATA, 0, 65535, 38, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT25), FieldLocation::VAR_DATA, 0, 65535, 39, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT26), FieldLocation::VAR_DATA, 0, 65535, 40, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT27), FieldLocation::VAR_DATA, 0, 65535, 41, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT28), FieldLocation::VAR_DATA, 0, 65535, 42, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT29), FieldLocation::VAR_DATA, 0, 65535, 43, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_TEXT30), FieldLocation::VAR_DATA, 0, 65535, 44, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START1), FieldLocation::VAR_DATA, 0, 65535, 45, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START2), FieldLocation::VAR_DATA, 0, 65535, 46, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START3), FieldLocation::VAR_DATA, 0, 65535, 47, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START4), FieldLocation::VAR_DATA, 0, 65535, 48, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START5), FieldLocation::VAR_DATA, 0, 65535, 49, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START6), FieldLocation::VAR_DATA, 0, 65535, 50, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START7), FieldLocation::VAR_DATA, 0, 65535, 51, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START8), FieldLocation::VAR_DATA, 0, 65535, 52, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START9), FieldLocation::VAR_DATA, 0, 65535, 53, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_START10), FieldLocation::VAR_DATA, 0, 65535, 54, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH1), FieldLocation::VAR_DATA, 0, 65535, 55, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH2), FieldLocation::VAR_DATA, 0, 65535, 56, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH3), FieldLocation::VAR_DATA, 0, 65535, 57, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH4), FieldLocation::VAR_DATA, 0, 65535, 58, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH5), FieldLocation::VAR_DATA, 0, 65535, 59, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH6), FieldLocation::VAR_DATA, 0, 65535, 60, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH7), FieldLocation::VAR_DATA, 0, 65535, 61, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH8), FieldLocation::VAR_DATA, 0, 65535, 62, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH9), FieldLocation::VAR_DATA, 0, 65535, 63, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_FINISH10), FieldLocation::VAR_DATA, 0, 65535, 64, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER1), FieldLocation::VAR_DATA, 0, 65535, 65, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER2), FieldLocation::VAR_DATA, 0, 65535, 66, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER3), FieldLocation::VAR_DATA, 0, 65535, 67, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER4), FieldLocation::VAR_DATA, 0, 65535, 68, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER5), FieldLocation::VAR_DATA, 0, 65535, 69, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER6), FieldLocation::VAR_DATA, 0, 65535, 70, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER7), FieldLocation::VAR_DATA, 0, 65535, 71, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER8), FieldLocation::VAR_DATA, 0, 65535, 72, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER9), FieldLocation::VAR_DATA, 0, 65535, 73, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER10), FieldLocation::VAR_DATA, 0, 65535, 74, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER11), FieldLocation::VAR_DATA, 0, 65535, 75, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER12), FieldLocation::VAR_DATA, 0, 65535, 76, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER13), FieldLocation::VAR_DATA, 0, 65535, 77, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER14), FieldLocation::VAR_DATA, 0, 65535, 78, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER15), FieldLocation::VAR_DATA, 0, 65535, 79, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER16), FieldLocation::VAR_DATA, 0, 65535, 80, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER17), FieldLocation::VAR_DATA, 0, 65535, 81, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER18), FieldLocation::VAR_DATA, 0, 65535, 82, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER19), FieldLocation::VAR_DATA, 0, 65535, 83, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_NUMBER20), FieldLocation::VAR_DATA, 0, 65535, 84, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION1), FieldLocation::VAR_DATA, 0, 65535, 85, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION2), FieldLocation::VAR_DATA, 0, 65535, 86, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION3), FieldLocation::VAR_DATA, 0, 65535, 87, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION4), FieldLocation::VAR_DATA, 0, 65535, 88, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION5), FieldLocation::VAR_DATA, 0, 65535, 89, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION6), FieldLocation::VAR_DATA, 0, 65535, 90, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION7), FieldLocation::VAR_DATA, 0, 65535, 91, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION8), FieldLocation::VAR_DATA, 0, 65535, 92, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION9), FieldLocation::VAR_DATA, 0, 65535, 93, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DURATION10), FieldLocation::VAR_DATA, 0, 65535, 94, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE1), FieldLocation::VAR_DATA, 0, 65535, 108, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE2), FieldLocation::VAR_DATA, 0, 65535, 109, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE3), FieldLocation::VAR_DATA, 0, 65535, 110, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE4), FieldLocation::VAR_DATA, 0, 65535, 111, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE5), FieldLocation::VAR_DATA, 0, 65535, 112, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE6), FieldLocation::VAR_DATA, 0, 65535, 113, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE7), FieldLocation::VAR_DATA, 0, 65535, 114, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE8), FieldLocation::VAR_DATA, 0, 65535, 115, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE9), FieldLocation::VAR_DATA, 0, 65535, 116, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_DATE10), FieldLocation::VAR_DATA, 0, 65535, 117, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST1), FieldLocation::VAR_DATA, 0, 65535, 128, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST2), FieldLocation::VAR_DATA, 0, 65535, 129, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST3), FieldLocation::VAR_DATA, 0, 65535, 130, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST4), FieldLocation::VAR_DATA, 0, 65535, 131, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST5), FieldLocation::VAR_DATA, 0, 65535, 132, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST6), FieldLocation::VAR_DATA, 0, 65535, 133, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST7), FieldLocation::VAR_DATA, 0, 65535, 134, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST8), FieldLocation::VAR_DATA, 0, 65535, 135, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST9), FieldLocation::VAR_DATA, 0, 65535, 136, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_COST10), FieldLocation::VAR_DATA, 0, 65535, 137, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE1_START), FieldLocation::VAR_DATA, 0, 65535, 140, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE1_FINISH), FieldLocation::VAR_DATA, 0, 65535, 141, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE1_WORK), FieldLocation::VAR_DATA, 0, 65535, 142, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE1_COST), FieldLocation::VAR_DATA, 0, 65535, 143, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE2_START), FieldLocation::VAR_DATA, 0, 65535, 148, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE2_FINISH), FieldLocation::VAR_DATA, 0, 65535, 149, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE2_WORK), FieldLocation::VAR_DATA, 0, 65535, 150, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE2_COST), FieldLocation::VAR_DATA, 0, 65535, 151, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE3_START), FieldLocation::VAR_DATA, 0, 65535, 156, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE3_FINISH), FieldLocation::VAR_DATA, 0, 65535, 157, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE3_WORK), FieldLocation::VAR_DATA, 0, 65535, 158, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE3_COST), FieldLocation::VAR_DATA, 0, 65535, 159, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE4_START), FieldLocation::VAR_DATA, 0, 65535, 164, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE4_FINISH), FieldLocation::VAR_DATA, 0, 65535, 165, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE4_WORK), FieldLocation::VAR_DATA, 0, 65535, 166, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE4_COST), FieldLocation::VAR_DATA, 0, 65535, 167, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE5_START), FieldLocation::VAR_DATA, 0, 65535, 172, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE5_FINISH), FieldLocation::VAR_DATA, 0, 65535, 173, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE5_WORK), FieldLocation::VAR_DATA, 0, 65535, 174, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE5_COST), FieldLocation::VAR_DATA, 0, 65535, 175, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE6_START), FieldLocation::VAR_DATA, 0, 65535, 180, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE6_FINISH), FieldLocation::VAR_DATA, 0, 65535, 181, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE6_WORK), FieldLocation::VAR_DATA, 0, 65535, 182, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE6_COST), FieldLocation::VAR_DATA, 0, 65535, 183, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE7_START), FieldLocation::VAR_DATA, 0, 65535, 188, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE7_FINISH), FieldLocation::VAR_DATA, 0, 65535, 189, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE7_WORK), FieldLocation::VAR_DATA, 0, 65535, 190, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE7_COST), FieldLocation::VAR_DATA, 0, 65535, 191, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE8_START), FieldLocation::VAR_DATA, 0, 65535, 196, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE8_FINISH), FieldLocation::VAR_DATA, 0, 65535, 197, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE8_WORK), FieldLocation::VAR_DATA, 0, 65535, 198, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE8_COST), FieldLocation::VAR_DATA, 0, 65535, 199, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE9_START), FieldLocation::VAR_DATA, 0, 65535, 204, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE9_FINISH), FieldLocation::VAR_DATA, 0, 65535, 205, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE9_WORK), FieldLocation::VAR_DATA, 0, 65535, 206, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE9_COST), FieldLocation::VAR_DATA, 0, 65535, 207, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE10_START), FieldLocation::VAR_DATA, 0, 65535, 212, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE10_FINISH), FieldLocation::VAR_DATA, 0, 65535, 213, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE10_WORK), FieldLocation::VAR_DATA, 0, 65535, 214, 0, 0);
		SetFieldItem(result, this, AssignmentFieldClass::getInstance(AssignmentField::AF_BASELINE10_COST), FieldLocation::VAR_DATA, 0, 65535, 215, 0, 0);
	};
	return result;
}

/**
	* {@inheritDoc}
	*/
FieldItemArrayPtr FieldMap9::getDefaultRelationData()
{
	static FieldItemArrayPtr empty(new FieldItemArray(0));
	return empty;
}

///**
//	* The values for these field types as read from the file, don't
//	* correspond to the values actually used in the var data blocks.
//	* It's not clear why, or whether these values are actually
//	* present somewhere in the file.
//	*/
//void FieldMap9::INIT_VAR_DATA_MAP_CONTENT()
//{
//	if (VAR_DATA_MAP_CONTENT.GetCount() > 0)
//		return;
//
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST1, 16414));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST2, 16415));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST3, 16416));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST4, 16417));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST5, 16418));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST6, 16419));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST7, 16420));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST8, 16421));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST9, 16422));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_COST10, 16423));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG1, 16394));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG2, 16395));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG3, 16396));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG4, 16397));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG5, 16398));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG6, 16399));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG7, 16400));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG8, 16401));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG9, 16402));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG10, 16403));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG11, 16404));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG12, 16405));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG13, 16406));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG14, 16407));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG15, 16408));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG16, 16409));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG17, 16410));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG18, 16411));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG19, 16412));
//	VAR_DATA_MAP_CONTENT.Add(std::pair<int,int>(AssignmentField::AF_FLAG20, 16413));
//};