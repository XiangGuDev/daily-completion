#pragma once

#include "..\FieldType.h"

/**
 * Utility class containing methods relating to the FieldType class.
 */
class FieldTypeHelper
{
public:
	/**
	 * Retrieve a FieldType instance based on an ID value from
	 * an MPP9 or MPP12 file.
	 *
	 * @param fieldID field ID
	 * @return FieldType instance
	 */
	static FieldType* getInstance(int fieldID);

	/**
	 * Retrieve a FieldType instance based on an ID value from
	 * an MPP14 field, mapping the START_TEXT, FINISH_TEXT, and DURATION_TEXT
	 * values to START, FINISH, and DURATION respectively.
	 *
	 * @param fieldID field ID
	 * @return FieldType instance
	 */
	static FieldType* getInstance14(int fieldID);

	/**
	* In some circumstances MS Project refers to the text version of a field (e.g. Start Text rather than Star) when we
	* actually need to process the non-text version of the field. This method performs that mapping.
	*
	* @param field field to mapped
	* @return mapped field
	*/
	static FieldType* mapTextFields(FieldType* field);

private:
	/**
	 * Generate a placeholder for an unknown type.
	 *
	 * @param type expected type
	 * @param fieldID field ID
	 * @return placeholder
	 */
	static FieldType* getPlaceholder(LPCTSTR class_type, int fieldID);

	static std::map<CString, std::shared_ptr<FieldType>> m_NullFieldType;
};