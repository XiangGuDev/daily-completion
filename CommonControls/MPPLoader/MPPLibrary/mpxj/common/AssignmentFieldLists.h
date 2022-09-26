#pragma once

#include "..\AssignmentField.h"

/**
 * Assignment fields grouped into logical collections.
 */
class AssignmentFieldLists
{
public:
	static const AssignmentField BASELINE_COSTS[];

	static const AssignmentField BASELINE_WORKS[];

	static const AssignmentField BASELINE_STARTS[];

	static const AssignmentField BASELINE_FINISHES[];

	static const AssignmentField BASELINE_BUDGET_COSTS[];

	static const AssignmentField BASELINE_BUDGET_WORKS[];

	static const AssignmentField CUSTOM_TEXT[];

	static const AssignmentField CUSTOM_START[];

	static const AssignmentField CUSTOM_FINISH[];

	static const AssignmentField CUSTOM_DATE[];

	static const AssignmentField CUSTOM_NUMBER[];

	static const AssignmentField CUSTOM_DURATION[];

	static const AssignmentField CUSTOM_COST[];

	static const AssignmentField CUSTOM_FLAG[];

	static const AssignmentField ENTERPRISE_COST[];

	static const AssignmentField ENTERPRISE_DATE[];

	static const AssignmentField ENTERPRISE_DURATION[];

	static const AssignmentField ENTERPRISE_FLAG[];

	static const AssignmentField ENTERPRISE_NUMBER[];

	static const AssignmentField ENTERPRISE_TEXT[];

	static const AssignmentField ENTERPRISE_CUSTOM_FIELD[];

	static const AssignmentField ENTERPRISE_RESOURCE_MULTI_VALUE[];

	static const AssignmentField ENTERPRISE_RESOURCE_OUTLINE_CODE[];
};