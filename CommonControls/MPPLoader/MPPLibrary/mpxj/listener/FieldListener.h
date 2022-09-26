#pragma once

class FieldContainer;

#include "..\FieldContainer.h"
#include "..\FieldType.h"

/**
 * Classes implementing this interface can be used to receive notification
 * of changes to task or resource fields.
 */
class FieldListener
{
   /**
    * Called when a field value is changed.
    * 
    * @param container field container
    * @param type field type
    * @param oldValue old value
    * @param newValue new value
    */
public:
	virtual void fieldChange(FieldContainer* container, FieldType* type, ObjectPtr oldValue, ObjectPtr newValue) = 0;
};