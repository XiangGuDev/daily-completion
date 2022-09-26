#pragma once

#include "..\TaskField.h"

/**
 * Utility class used to map between the integer values held in MS Project
 * to represent a task field, and the enumerated type used to represent
 * task fields in MPXJ.
 */
class MPPTaskField14
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
    * Retrieve an instance of the TaskField class based on the data read from an
    * MS Project file. This method does not attempt to apply any mapping to 
    * the result.
    *
    * @param value value from an MS Project file
    * @return TaskField instance
    */
	static TaskField getInstanceUnmapped(int value);

   /**
    * Retrieve the ID of a field, as used by MS Project.
    * 
    * @param value field instance
    * @return field ID
    */
	static int getID(TaskField value);

private:
	static void init();
   static const int MAX_VALUE = 1385;
   static TaskField FIELD_ARRAY[MAX_VALUE];
   static int ID_ARRAY[TaskField::TF_FINISH];
};