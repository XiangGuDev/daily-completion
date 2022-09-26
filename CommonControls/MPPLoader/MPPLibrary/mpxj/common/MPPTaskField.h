#pragma once

#include "..\TaskField.h"

/**
 * Utility class used to map between the integer values held in MS Project
 * to represent a task field, and the enumerated type used to represent
 * task fields in MPXJ.
 */
class MPPTaskField
{
public:
	/**
	 * Retrieve an instance of the TaskField class based on the data read from an
	 * MS Project file.
	 *
	 * @param value value from an MS Project file
	 * @return TaskField instance
	 */
	static TaskField getInstance(int value);

	/**
	 * Retrieve the ID of a field, as used by MS Project.
	 *
	 * @param value field instance
	 * @return field ID
	 */
	static int getID(TaskField value);

	static const int TASK_FIELD_BASE = 0x0B400000;

private:
	static void init();
	static const int MAX_VALUE = 1336;
	static TaskField FIELD_ARRAY[MAX_VALUE];
	static int ID_ARRAY[TaskField::TF_FINISH];
};