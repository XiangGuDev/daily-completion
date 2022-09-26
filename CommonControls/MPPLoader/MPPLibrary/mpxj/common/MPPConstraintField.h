#pragma once

#include "..\ConstraintField.h"

/**
 * Utility class used to map between the integer values held in MS Project
 * to represent a constraint field, and the enumerated type used to represent
 * constraint fields in MPXJ.
 */
class MPPConstraintField
{
public:
	/**
	 * Retrieve an instance of the ConstraintField class based on the data read from an
	 * MS Project file.
	 *
	 * @param value value from an MS Project file
	 * @return ConstraintField instance
	 */
	static ConstraintField getInstance(int value);

	/**
	 * Retrieve the ID of a field, as used by MS Project.
	 *
	 * @param value field instance
	 * @return field ID
	 */
	static int getID(ConstraintField value);

	static const int CONSTRAINT_FIELD_BASE = 0xD400000;

private:
	static void static_data_init();

	static const int MAX_VALUE = 10;

	static ConstraintField FIELD_ARRAY[10];// = new ConstraintField[MAX_VALUE];
	static int ID_ARRAY[CF_FINISH];// = new int[ConstraintField.MAX_VALUE];
};