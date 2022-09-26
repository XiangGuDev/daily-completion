#pragma once

#include "ResourceField.h"
#include "TaskField.h"
#include "AssignmentField.h"
#include "ConstraintField.h"


/**
 * Locale data for MPXJ.
 */
class LocaleData
{
public:
	struct COLUMN_TEXT
	{
		LPCTSTR english;
		LPCTSTR chinese;
	};

	enum ColumnType
	{
		TASK_COLUMNS,
		RESOURCE_COLUMNS,
		ASSIGNMENT_COLUMNS,
		CONSTRAINT_COLUMNS,
	};

	static LPCTSTR getColumnText(Locale* locale, ColumnType column_type, int column_index);

private:
	static void init_array();
	static COLUMN_TEXT RESOURCE_COLUMNS_ARRAY[Resource2Field::RF_FINISH+1];
	static COLUMN_TEXT TASK_COLUMNS_ARRAY[TaskField::TF_FINISH+1];
	static COLUMN_TEXT ASSIGNMENT_COLUMNS_ARRAY[AssignmentField::AF_FINISH+1];
	static COLUMN_TEXT CONSTRAINT_COLUMNS_ARRAY[ConstraintField::CF_FINISH+1];
};