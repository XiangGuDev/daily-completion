#pragma once

#include "MpxjEnum.h"

/**
 * This interface is implemented by classes which represent a field
 * in a Task, Resource2 or Assignment entity.
 */
class FieldType : public Object2
{
public:
   /**
    * Retrieve the name of this field using the default locale.
    *
    * @return field name
    */
	virtual CString getName()
	{
		return getName (Locale::getDefault());
	}

	virtual int getValue() = 0;

   /**
    * Retrieve the name of this field using the supplied locale.
    *
    * @param locale target locale
    * @return field name
    */
	virtual CString getName(Locale* locale) = 0;

   /**
    * Retrieve the data type of this field.
    * 
    * @return data type
    */
   virtual DataType getDataType() = 0;

   /**
    * Retrieve the associated units field, if any.
    * 
    * @return units field
    */
   virtual FieldType* getUnitsType() = 0;
};

//typedef std::shared_ptr<FieldType> FieldTypePtr;