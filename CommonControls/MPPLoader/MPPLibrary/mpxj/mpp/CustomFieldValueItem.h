#pragma once

/**
 * Instances of this type represent a possible value for a custom field that is
 * using value lists.
 */
class CustomFieldValueItem
{
public:
	/**
	 * Constructor.
	 *
	 * @param uniqueID item unique ID
	 */
	CustomFieldValueItem(int uniqueID);

	/**
	 * Get the unique id for this item.
	 *
	 * @return item unique ID
	 */
	int getUniqueID();

	/**
	 * Set the value of this item.
	 *
	 * @param value item value
	 */
	void setValue(ByteArrayPtr value);

	/**
	 * Get the value of this item.
	 *
	 * @return item value
	 */
	ByteArrayPtr getValue();

	/**
	 * Set the description for this item.
	 *
	 * @param description item description
	 */
	void setDescription(CString description);

	/**
	 * Get the description for this item.
	 *
	 * @return item description
	 */
	CString getDescription();

	/**
	 * Set an Unknown property for this item.
	 *
	 * @param unknown unknown data block
	 */
	void setUnknown(ByteArrayPtr unknown);

	/**
	 * Get an unknown property for this item.
	 *
	 * @return unknown data block
	 */
	ByteArrayPtr getUnknown();

	/**
	 * Set the parent ID.
	 *
	 * @param id parent ID
	 */
	void setParent(int id);

	/**
	 * Retrieve the parent ID.
	 *
	 * @return parent IDs
	 */
	int getParent();

	///**
	// * {@inheritDoc}
	// */
	//virtual CString toString();

private:
	int m_uniqueID;
	ByteArrayPtr m_value;
	CString m_description;
	ByteArrayPtr m_unknown;
	int m_parentId;
};

typedef std::shared_ptr<CustomFieldValueItem> CustomFieldValueItemPtr;