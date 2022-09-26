#include "stdafx.h"
#include "MPPConstraintField.h"

ConstraintField MPPConstraintField::FIELD_ARRAY[10] = { (ConstraintField)0 };
int MPPConstraintField::ID_ARRAY[CF_FINISH] = { -1 };// = new int[ConstraintField.MAX_VALUE];

void MPPConstraintField::static_data_init()
{
	if (FIELD_ARRAY[6] != 0)
		return;

	//memset(FIELD_ARRAY, NULL, sizeof(ConstraintField)* MAX_VALUE);

	FIELD_ARRAY[9] = ConstraintField::CF_UNIQUE_ID;
	FIELD_ARRAY[6] = ConstraintField::CF_TASK1;
	FIELD_ARRAY[7] = ConstraintField::CF_TASK2;

	//Arrays.fill(ID_ARRAY, -1);
	//memset(ID_ARRAY, -1, sizeof(int)* CF_FINISH);

	for (int loop = 0; loop < MAX_VALUE; loop++)
	{
		ConstraintField constraintField = FIELD_ARRAY[loop];
		if (constraintField != NULL)
		{
			ID_ARRAY[constraintField] = loop;
		}
	}
}

/**
* Retrieve an instance of the ConstraintField class based on the data read from an
* MS Project file.
*
* @param value value from an MS Project file
* @return ConstraintField instance
*/
ConstraintField MPPConstraintField::getInstance(int value)
{
	static_data_init();

	ConstraintField result = CF_UNIQUE_ID;

	if (value >= 0 && value < MAX_VALUE)
    {
        result = FIELD_ARRAY[value];
    }

    return (result);
}

/**
* Retrieve the ID of a field, as used by MS Project.
* 
* @param value field instance
* @return field ID
*/
int MPPConstraintField::getID(ConstraintField value)
{
	static_data_init();

    return (ID_ARRAY[value]);
}