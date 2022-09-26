#include "stdafx.h"
#include "MppBitFlag.h"
#include "..\FieldContainer.h"
#include "MPPUtility.h"

/**
* Constructor.
* 
* @param type field type
* @param offset offset in buffer
* @param mask bit mask 
* @param zeroValue value to return if expression is zero
* @param nonZeroValue value to return if expression is non-zero
*/
MppBitFlag::MppBitFlag(FieldType* type, int offset, int mask, ObjectPtr zeroValue, ObjectPtr nonZeroValue)
{
    m_type = type;
    m_offset = offset;
    m_mask = mask;
    m_zeroValue = zeroValue;
    m_nonZeroValue = nonZeroValue;
}

/**
* Extracts the value of this bit flag from the supplied byte array
* and sets the value in the supplied container.
* 
* @param container container
* @param data byte array
*/
void MppBitFlag::setValue(FieldContainer* container, ByteArrayPtr data)
{
    if (data)
    {
        container->set(m_type, ((MPPUtility::getInt(data.get(), m_offset) & m_mask) == 0) ? m_zeroValue : m_nonZeroValue);
    }
}