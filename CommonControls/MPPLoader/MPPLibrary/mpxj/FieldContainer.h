#pragma once

#include "FieldType.h"

class FieldListener;

/**
 * This interface is implemented by the Task and Resource2 classes. It
 * defines the common methods used to set and retrieve field values
 * using their identifiers.
 */
class FieldContainer : public Object2
{
public:
   /**
    * Set a field value.
    * 
    * @param field field identifier
    * @param value field value
    */
	virtual void set(FieldType* field, ObjectPtr value) = 0;

   /**
    * Retrieve a field value. Use the cached value. Do not attempt to
    * calculate a value if the cached value is NULL.
    * 
    * @param field field identifier
    * @return field value
    */
	virtual ObjectPtr getCachedValue(FieldType* field) = 0;

   /**
    * Retrieve a field value. If the cached value is NULL, attempt to
    * calculate the current value.
    * 
    * @param field field identifier
    * @return field value
    */
	virtual ObjectPtr getCurrentValue(FieldType* field) = 0;

   /**
    * Add a listener to receive field events.
    * 
    * @param listener target listener
    */
   virtual void addFieldListener(FieldListener* listener) = 0;

   /**
    * Remove a listener.
    * 
    * @param listener target listener
    */
   virtual void removeFieldListener(FieldListener* listener) = 0;
};

typedef std::shared_ptr<FieldContainer> FieldContainerPtr;