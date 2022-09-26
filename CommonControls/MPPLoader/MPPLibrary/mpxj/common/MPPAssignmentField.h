#pragma once

#include "..\AssignmentField.h"

/**
 * Utility class used to map between the integer values held in MS Project
 * to represent an assignment field, and the enumerated type used to represent
 * assignment fields in MPXJ.
 */
class MPPAssignmentField
{
public:
	/**
	 * Retrieve an instance of the AssignmentField class based on the data read from an
	 * MS Project file.
	 *
	 * @param value value from an MS Project file
	 * @return AssignmentField instance
	 */
	static AssignmentField getInstance(int value);

	/**
	 * Retrieve the ID of a field, as used by MS Project.
	 *
	 * @param value field instance
	 * @return field ID
	 */
	static int getID(AssignmentField value);

	static const int ASSIGNMENT_FIELD_BASE = 0x0F400000;

	static const int MAX_VALUE = 715;

private:
	static void init();
	static AssignmentField FIELD_ARRAY[MAX_VALUE];// = new AssignmentField[MAX_VALUE];
	static int ID_ARRAY[AssignmentField::AF_FINISH+1];// = new int[AssignmentField.MAX_VALUE];
};
