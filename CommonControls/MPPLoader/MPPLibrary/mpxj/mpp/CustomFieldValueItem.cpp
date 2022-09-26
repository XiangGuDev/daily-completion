#include "stdafx.h"
#include "CustomFieldValueItem.h"

/**
* Constructor.
* 
* @param uniqueID item unique ID
*/
CustomFieldValueItem::CustomFieldValueItem(int uniqueID)
{
    m_uniqueID = uniqueID;
}

/**
* Get the unique id for this item.
* 
* @return item unique ID
*/
int CustomFieldValueItem::getUniqueID()
{
    return m_uniqueID;
}

/**
* Set the value of this item.
* 
* @param value item value
*/
void CustomFieldValueItem::setValue(ByteArrayPtr value)
{
    m_value = value;
}

/**
* Get the value of this item.
* 
* @return item value
*/
ByteArrayPtr CustomFieldValueItem::getValue()
{
    return m_value;
}

/**
* Set the description for this item.
* 
* @param description item description
*/
void CustomFieldValueItem::setDescription(CString description)
{
    m_description = description;
}

/**
* Get the description for this item.
* 
* @return item description
*/
CString CustomFieldValueItem::getDescription()
{
    return m_description;
}

/**
* Set an Unknown property for this item.
* 
* @param unknown unknown data block
*/
void CustomFieldValueItem::setUnknown(ByteArrayPtr unknown)
{
    m_unknown = unknown;
}

/**
* Get an unknown property for this item.
* 
* @return unknown data block
*/
ByteArrayPtr CustomFieldValueItem::getUnknown()
{
    return m_unknown;
}

/**
* Set the parent ID.
* 
* @param id parent ID
*/
void CustomFieldValueItem::setParent(int id)
{
    m_parentId = id;
}

/**
* Retrieve the parent ID.
* 
* @return parent IDs
*/
int CustomFieldValueItem::getParent()
{
    return m_parentId;
}

///**
//* {@inheritDoc}
//*/
//CString CustomFieldValueItem::toString()
//{
//    return CString.format("[CustomFieldValueItem uniqueID=%d parentId=%d value=%s (%s)", m_uniqueID, m_parentId, MPPUtility.hexdump(m_value, false), MPPUtility.getUnicodeString(m_value));
//}