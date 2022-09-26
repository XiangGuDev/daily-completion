#pragma once

#include "FieldMap.h"

/**
 * MPP14 field map.
 */
class FieldMap9 : public FieldMap
{
public:
	/**
	 * Constructor.
	 *
	 * @param file parent file
	 */
	FieldMap9(ProjectFile* file);

protected:

	/**
	 * {@inheritDoc}
	 */
	virtual FieldType* getFieldType(int fieldID);

	/**
	 * {@inheritDoc}
	 */
	virtual bool useTypeAsVarDataKey();

	/**
	 * {@inheritDoc}
	 */
	virtual bool substituteVarDataKey(FieldType* type, int& value);

	/**
	 * {@inheritDoc}
	 */
	virtual FieldItemArrayPtr getDefaultTaskData();

	/**
	 * {@inheritDoc}
	 */
	virtual FieldItemArrayPtr getDefaultResource2Data();

	/**
	 * {@inheritDoc}
	 */
	virtual FieldItemArrayPtr getDefaultAssignmentData();

	/**
	 * {@inheritDoc}
	 */
	virtual FieldItemArrayPtr getDefaultRelationData();

private:

	//static void INIT_VAR_DATA_MAP_CONTENT();

	/**
	 * The values for these field types as read from the file, don't
	 * correspond to the values actually used in the var data blocks.
	 * It's not clear why, or whether these values are actually
	 * present somewhere in the file.
	 */
	//static CAtlArray<std::pair<int, int>> VAR_DATA_MAP_CONTENT;

	static Map2<FieldType*, int> VAR_DATA_MAP;// = new HashMap<FieldType*, int>();
};