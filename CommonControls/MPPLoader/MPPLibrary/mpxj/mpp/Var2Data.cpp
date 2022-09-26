#include "stdafx.h"
#include "Var2Data.h"
#include "MPPUtility.h"

/**
* Constructor. Extracts the content of the data block, with reference
* to the meta data held in the VarMetaPtr block.
*
* @param meta meta data for this block
* @param is BwProj::toolkit::IDataReadStream* from which data is read
* @throws IOException on file read error
*/
Var2Data::Var2Data(VarMetaPtr meta, BwProj::toolkit::IDataReadStream* is)
{
    m_meta = meta;
    ByteArrayPtr data;

    int itemCount = m_meta->getItemCount();

    for (int loop = 0; loop < itemCount; loop++)
    {
        int itemOffset = meta->getOffset(loop);
        is->reset();
        is->skip(itemOffset);

        int size = readInt(is);

        data = readByteArray2(is, size);

		m_map.put(itemOffset, data);
    }

}

/**
* This method retrieves a byte array containing the data at the
* given offset in the block. If no data is found at the given offset
* this method returns NULL.
*
* @param offset offset of required data
* @return byte array containing required data
*/
ByteArrayPtr Var2Data::getByteArray2(int offset)
{
    ByteArrayPtr result;
	
    if (offset >= 0)
    {
		m_map.Lookup(offset, result);
    }

    return (result);
}

/**
* This method retrieves a byte array of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return byte array containing required data
*/
ByteArrayPtr Var2Data::getByteArray2(int id, int type)
{
	int offset = 0;
	if (m_meta->getOffset(id, type, offset))
		return (getByteArray2(offset));
	return ByteArrayPtr();
}

/**
* This method retrieves the data at the given offset and returns
* it as a CString, assuming the underlying data is composed of
* two byte characters.
*
* @param offset offset of required data
* @return string containing required data
*/
StringPtr Var2Data::getUnicodeString(int offset)
{
	StringPtr result;

	if (offset >= 0)
    {
		ByteArrayPtr value;// = m_map.get(offset);
		if (m_map.Lookup(offset, value))
		{
			result.reset (MPPUtility::getUnicodeString(value.get()));
		}
    }

    return result;
}

/**
* This method retrieves a CString of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return string containing required data
*/
StringPtr Var2Data::getUnicodeString(int id, int type)
{
	int offset = 0;
	if (m_meta->getOffset(id, type, offset))
		return getUnicodeString(offset);
	return StringPtr();
}

/**
* This method retrieves a timestamp of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return required timestamp data
*/
DatePtr Var2Data::getTimestamp(int id, int type)
{
    DatePtr result;

	int offset = 0;

	if (m_meta->getOffset(id, type, offset) && offset >= 0)
    {
		ByteArrayPtr value;// m_map.get(offset);
		if (m_map.Lookup(offset, value) && value->length() >= 4)
		{
			result = MPPUtility::getTimestamp(value.get());
		}
    }

    return (result);
}

/**
* This method retrieves the data at the given offset and returns
* it as a CString, assuming the underlying data is composed of
* single byte characters.
*
* @param offset offset of required data
* @return string containing required data
*/
CString Var2Data::getString(int offset)
{
    CString result;

	if (offset >= 0)
    {
		ByteArrayPtr value;// = m_map.get(offset);
		if (m_map.Lookup(offset, value))
		{
			result = MPPUtility::getString(value.get());
		}
    }

    return (result);
}

/**
* This method retrieves a string of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return required string data
*/
CString Var2Data::getString(int id, int type)
{
	int offset = 0;
	if (m_meta->getOffset(id, type, offset))
		return (getString(offset));
	return L"";
}

/**
* This method retrieves an integer of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return required integer data
*/
int Var2Data::getShort(int id, int type)
{
    int result = 0;

	int offset = 0;

	if (m_meta->getOffset(id, type, offset) && offset >= 0)
    {
		ByteArrayPtr value;// = m_map.get(offset);

		if (m_map.Lookup(offset,value) && value->length() >= 2)
        {
			result = MPPUtility::getShort(value.get());
        }
    }

    return (result);
}

/**
* This method retrieves an integer of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return required integer data
*/
int Var2Data::getByte(int id, int type)
{
    int result = 0;

	int offset = 0;

	if (m_meta->getOffset(id, type, offset) && offset >= 0)
    {
		ByteArrayPtr value;// = m_map.get(offset);

		if (m_map.Lookup(offset, value) && value >= 0)
        {
			result = MPPUtility::getByte(value.get());
        }
    }

    return (result);
}

/**
* This method retrieves an integer of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return required integer data
*/
int Var2Data::getInt(int id, int type)
{
    int result = 0;

	int offset = 0;

	if (m_meta->getOffset(id, type, offset) && offset >= 0)
    {
		ByteArrayPtr value;// = m_map.get(offset);

		if (m_map.Lookup(offset, value) && value->length() >= 4)
        {
			result = MPPUtility::getInt(value.get());
        }
    }

    return (result);
}

/**
* This method retrieves an integer of the specified type,
* belonging to the item with the specified unique ID. Note that
* the integer value is read from an arbitrary offset within the 
* byte array of data.
*
* @param id unique ID of entity to which this data belongs
* @param offset offset into the byte array fom which to read the integer
* @param type data type identifier
* @return required integer data
*/
int Var2Data::getInt(int id, int offset, int type)
{
    int result = 0;

	int metaOffset = 0;

	if (m_meta->getOffset(id, type, metaOffset) && metaOffset >= 0)
    {
		ByteArrayPtr value;// = m_map.get(metaOffset);

        if (m_map.Lookup(metaOffset, value) && value->length() >= (size_t)(offset + 4))
        {
			result = MPPUtility::getInt(value.get(), offset);
        }
    }

    return (result);
}

/**
* This method retrieves an integer of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return required integer data
*/
__int64 Var2Data::getLong(int id, int type)
{
	__int64 result = 0;

	int offset = 0;

	if (m_meta->getOffset(id, type, offset) && offset >= 0)
    {
		ByteArrayPtr value;// = m_map.get(offset);

        if (m_map.Lookup(offset, value) && value->length() >= 8)
        {
			result = MPPUtility::getLong(value.get());
        }
    }

    return (result);
}

/**
* This method retrieves a double of the specified type,
* belonging to the item with the specified unique ID.
*
* @param id unique ID of entity to which this data belongs
* @param type data type identifier
* @return required double data
*/
double Var2Data::getDouble(int id, int type)
{
	__int64 longValue = getLong(id, type);
	//if (longValue)
	//	ASSERT(FALSE);// do test
	return *((double*)(&longValue));
    //return (Double.longBitsToDouble(getLong(id, type)));
}

/**
* Retrieve the underlying meta data. This method is provided
* mainly as a convenience for debugging.
* 
* @return VarMetaPtr instance
*/
VarMetaPtr Var2Data::getVarMeta()
{
    return (m_meta);
}

///**
//* This method dumps the contents of this Var2Data block as a CString.
//* Note that this facility is provided as a debugging aid.
//*
//* @return formatted contents of this block
//*/
//CString Var2Data::toString()
//{
//    StringWriter sw = new StringWriter();
//    PrintWriter pw = new PrintWriter(sw);
//
//    pw.println("BEGIN Var2Data");
//    for (int offset : m_map.keySet())
//    {
//        ByteArrayPtr data = m_map.get(offset);
//        pw.println("   Data at offset: " + offset + " size: " + data.length);
//        pw.println(MPPUtility::hexdump(data, true, 16, "   "));
//    }
//
//    pw.println("END Var2Data");
//    pw.println();
//    pw.close();
//    return (sw.toString());
//}
//
///**
//* This is a specialised version of the toString method which 
//* outputs just the data in this structure for the given unique ID. 
//* 
//* @param id unique ID
//* @return string representation
//*/
//CString Var2Data::toString(int id)
//{
//    StringWriter sw = new StringWriter();
//    PrintWriter pw = new PrintWriter(sw);
//
//    pw.println("BEGIN Var2Data for " + id);
//    for (int type : m_meta->getTypes(id))
//    {
//        int offset = m_meta->getOffset(id, type);
//        ByteArrayPtr data = m_map.get(offset);
//        pw.println("   Data at offset: " + offset + " size: " + data.length);
//        pw.println(MPPUtility::hexdump(data, true, 16, "   "));
//    }
//    pw.println("END Var2Data for " + id);
//    pw.println();
//    pw.close();
//    return (sw.toString());
//}