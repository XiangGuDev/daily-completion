#pragma once

#include "..\ResourceField.h"

/**
 * Utility class used to map between the integer values held in MS Project
 * to represent a resource field, and the enumerated type used to represent
 * resource fields in MPXJ.
 */
class MPPResource2Field
{
public:
	/**
	 * Retrieve an instance of the Resource2Field class based on the data read from an
	 * MS Project file.
	 *
	 * @param value value from an MS Project file
	 * @return Resource2Field instance
	 */
	static Resource2Field getInstance(int value);

	/**
	 * Retrieve the ID of a field, as used by MS Project.
	 *
	 * @param value field instance
	 * @return field ID
	 */
	static int getID(Resource2Field value);

	static const int RESOURCE_FIELD_BASE = 0x0C400000;
	static const int MAX_VALUE = 809;

private:
	static void init();

	static Resource2Field FIELD_ARRAY[MAX_VALUE];// = new Resource2Field[MAX_VALUE];
	static int ID_ARRAY[Resource2Field::RF_FINISH+1];// = new int[Resource2Field.MAX_VALUE];
};
