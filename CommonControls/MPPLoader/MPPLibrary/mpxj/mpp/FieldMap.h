#pragma once

#include "..\ProjectFile.h"
#include "Props.h"
#include "Var2Data.h"

/**
* Enumeration representing the location of field data.
*/
enum FieldLocation
{
	FIXED_DATA,
	VAR_DATA,
	META_DATA,
	UNKNOWN
};

class FieldMap;

/**
* This class is used to collect together the attributes necessary to
* describe the location of each field within the MPP file. It also provides
* the methods used to extract an individual field value.
*/
class FieldItem : public ComparableT<FieldItem>
{
public:
	/**
	* Constructor.
	*
	* @param type field type
	* @param location identifies which block the field is present in
	* @param fixedDataBlockIndex identifies which block the data comes from
	* @param fixedDataOffset fixed data block offset
	* @param varDataKey var data block key
	* @param mask TODO
	* @param metaBlock TODO
	*/
	FieldItem(FieldMap* fieldMap, FieldType* type, FieldLocation location, int fixedDataBlockIndex,
		int fixedDataOffset, int varDataKey, long mask, int metaBlock);

	/**
	* Reads a single field value.
	*
	* @param id parent entity ID
	* @param fixedData fixed data block
	* @param varData var data block
	* @return field value
	*/
	ObjectPtr read(int id, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData);

	/**
	* Retrieve the field type.
	*
	* @return field type
	*/
	FieldType* getType();

	/**
	* Retrieve the index of the fixed data block containing this item.
	*
	* @return fixed data block index
	*/
	int getFixedDataBlockIndex();

	/**
	* Retrieve the fixed data offset for this field.
	*
	* @return fixed data offset
	*/
	int getFixedDataOffset();

	/**
	* Retrieve the var data key for this field.
	*
	* @return var data key
	*/
	int getVarDataKey();

	/**
	* Retrieve the field location for this field.
	*
	* @return field location
	*/
	FieldLocation getFieldLocation();

	/**
	* Implements the only method in the Comparable interface to allow
	* FieldItem instances to be sorted.
	*
	* @param item item to compare with
	* @return comparison result
	*/
	virtual int compareTo(FieldItem* item);

	///**
	//* {@inheritDoc}
	//*/
	//virtual CString toString();

private:
	/**
	* Read a field from the fixed data block.
	*
	* @param id parent entity ID
	* @param fixedData fixed data block
	* @param varData var data block
	* @return field value
	*/
	ObjectPtr readFixedData(int id, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData);

	/**
	* Read a field value from a var data block.
	*
	* @param id parent entity ID
	* @param fixedData fixed data block
	* @param varData var data block
	* @return field value
	*/
	ObjectPtr readVarData(int id, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData);

	/**
	* Retrieve custom field value.
	*
	* @param varData var data block
	* @param id item ID
	* @param type item type
	* @return item value
	*/
	ObjectPtr getCustomFieldTimestampValue(Var2DataPtr varData, int id, int type);

	/**
	* Retrieve custom field value.
	*
	* @param varData var data block
	* @param id item ID
	* @param type item type
	* @param units duration units
	* @return item value
	*/
	ObjectPtr getCustomFieldDurationValue(Var2DataPtr varData, int id, int type, TimeUnit units);

	/**
	* Retrieve custom field value.
	*
	* @param varData var data block
	* @param id item ID
	* @param type item type
	* @return item value
	*/
	double getCustomFieldDoubleValue(Var2DataPtr varData, int id, int type);

	/**
	* Retrieve custom field value.
	*
	* @param varData var data block
	* @param id item ID
	* @param type item type
	* @return item value
	*/
	StringPtr getCustomFieldUnicodeStringValue(Var2DataPtr varData, int id, int type);

private:
	FieldMap* m_fieldMap;
	FieldType* m_type;
	FieldLocation m_location;
	int m_fixedDataBlockIndex;
	int m_fixedDataOffset;
	int m_varDataKey;
	long m_mask;
	int m_metaBlock;
};

typedef std::shared_ptr<FieldItem> FieldItemPtr;
typedef Array2<FieldItemPtr> FieldItemArray;
typedef std::shared_ptr<FieldItemArray> FieldItemArrayPtr;

/**
 * This class is used to represent the mapping present in the MPP file
 * between fields and their locations in various data blocks.
 */
class FieldMap
{
	friend FieldItem;
public:
	/**
	 * Constructor.
	 *
	 * @param file parent project file
	 */
	FieldMap(ProjectFile* file);

	/**
	 * Creates a field map for tasks.
	 *
	 * @param props props data
	 */
	void createTaskFieldMap(PropsPtr props);

	// 输出到控制台（调试用）
	void debugPrintFieldMap();

	/**
	 * Creates a field map for relations.
	 *
	 * @param props props data
	 */
	void createRelationFieldMap(PropsPtr props);

	/**
	 * Create a field map for enterprise custom fields.
	 *
	 * @param props props data
	 * @param c target class
	 */
	void createEnterpriseCustomFieldMap(PropsPtr props, LPCTSTR type_c);

	/**
	 * Creates a field map for resources.
	 *
	 * @param props props data
	 */
	void createResource2FieldMap(PropsPtr props);

	/**
	 * Creates a field map for assignments.
	 *
	 * @param props props data
	 */
	void createAssignmentFieldMap(PropsPtr props);

	/**
	 * Given a container, and a set of raw data blocks, this method extracts
	 * the field data and writes it into the container.
	 *
	 * @param type expected type
	 * @param container field container
	 * @param id entity ID
	 * @param fixedData fixed data block
	 * @param varData var data block
	 */
	void populateContainer(LPCTSTR type_c, FieldContainer* container,
		int id, ByteArrayPtr* fixedData, int arr_len, Var2DataPtr varData);

	/**
	 * Retrieve the maximum offset in the fixed data block.
	 *
	 * @param blockIndex required block index
	 * @return maximum offset
	 */
	int getMaxFixedDataOffset(int blockIndex);

	/**
	 * Retrieve the fixed data offset for a specific field.
	 *
	 * @param type field type
	 * @return offset
	 */
	IntegerPtr getFixedDataOffset(FieldType* type);

	/**
	 * Retrieve the var data key for a specific field.
	 *
	 * @param type field type
	 * @return var data key
	 */
	IntegerPtr getVarDataKey(FieldType* type);

	/**
	 * Used to map from a var data key to a field type. Note this
	 * is designed for diagnostic use only, and uses an inefficient search.
	 *
	 * @param key var data key
	 * @return field type
	 */
	FieldType* getFieldTypeFromVarDataKey(int key);

	/**
	 * Retrieve the field location for a specific field.
	 *
	 * @param type field type
	 * @return field location
	 */
	FieldLocation getFieldLocation(FieldType* type);

	/**
	 * Clear the field map.
	 */
	void clear();

	/**
	 * Diagnostic method used to dump known field map data.
	 *
	 * @param props props block containing field map data
	 */
	void dumpKnownFieldMaps(PropsPtr props);

	///**
	// * {@inheritDoc}
	// */
	//virtual CString toString();

protected:

	/**
	* Used to determine what value is used as the var data key.
	*
	* @return true if the field type value is used as the var data key
	*/
	virtual bool useTypeAsVarDataKey() = 0;

	/**
	* Abstract method used by child classes to supply default data.
	*
	* @return default data
	*/
	virtual FieldItemArrayPtr getDefaultTaskData() = 0;

	/**
	* Abstract method used by child classes to supply default data.
	*
	* @return default data
	*/
	virtual FieldItemArrayPtr getDefaultResource2Data() = 0;

	/**
	* Abstract method used by child classes to supply default data.
	*
	* @return default data
	*/
	virtual FieldItemArrayPtr getDefaultAssignmentData() = 0;

	/**
	* Abstract method used by child classes to supply default data.
	*
	* @return default data
	*/
	virtual FieldItemArrayPtr getDefaultRelationData() = 0;

	/**
	* Given a field ID, derive the field type.
	*
	* @param fieldID field ID
	* @return field type
	*/
	virtual FieldType* getFieldType(int fieldID) = 0;

	/**
	* In some circumstances the var data key used in the file
	* does not match the var data key derived from the type.
	* This method is used to perform a substitution so that
	* the correct value is used.
	*
	* @param type field type to be tested
	* @return substituted value, or NULL
	*/
	virtual bool substituteVarDataKey(FieldType* type, int& value) = 0;

	/**
	* Retrieve a single field value.
	*
	* @param id parent entity ID
	* @param type field type
	* @param fixedData fixed data block
	* @param varData var data block
	* @return field value
	*/
	ObjectPtr getFieldData(int id, FieldType* type, Array2<ByteArrayPtr>& fixedData, Var2DataPtr varData);

	/**
	* Retrieve the parent project file.
	*
	* @return project file
	*/
	ProjectFile* getProjectFile();

private:
	/**
	* Generic method used to create a field map from a block of data.
	*
	* @param data field map data
	*/
	void createFieldMap(ByteArrayPtr data);

	/**
	* This method takes an array of data and uses this to populate the
	* field map.
	*
	* @param defaultData field map default data
	*/
	void populateDefaultData(FieldItemArrayPtr defaultData);

protected:
	TimeUnit m_defaultProjectTimeUnits;

private:
	ProjectFile* m_file;

	static const int TASK_KEYS[];

	static const int ENTERPRISE_CUSTOM_KEYS[];

	static const int RESOURCE_KEYS[];

	static const int ASSIGNMENT_KEYS[];

	static const int RELATION_KEYS[];

	static const int VALUE_LIST_MASK = 0x0700;

	static const int MAX_FIXED_DATA_BLOCKS = 2;

	Map2<FieldType*, FieldItemPtr> m_map;// = new HashMap<FieldType*, FieldItem>();
	int m_maxFixedDataOffset[MAX_FIXED_DATA_BLOCKS];// = new int[MAX_FIXED_DATA_BLOCKS];
};

typedef std::shared_ptr<FieldMap> FieldMapPtr;