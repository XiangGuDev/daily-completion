#include "stdafx.h"
#include "FixedData.h"
#include "MPPUtility.h"

/**
* This constructor retrieves the data from the input stream. It
* makes use of the meta data regarding this data block that has
* already been read in from the MPP file.
*
* Note that we actually read in the entire data block in one go.
* This is due to the fact that MS Project sometimes describes data
* using offsets that are out of sequence, and items that may overlap.
* Ideally this data would be read directly from the input stream, but
* this was problematic, so this less than ideal solution has been
* adopted.
*
* @param meta meta data about the contents of this fixed data block
* @param is input stream from which the data is read
* @throws IOException on file read failure
*/
FixedData::FixedData(FixedMeta* meta, BwProj::toolkit::IDataReadStream* is)
:FixedData(meta, is, 0)
{

}

/**
* This version of the above constructor allows us to limit the
* size of blocks we copy where we have an idea of the maximum expected
* block size. This prevents us from reading ridiculously large amounts
* of unnecessary data, causing OutOfMemory exceptions.
* 
* @param meta meta data about the contents of this fixed data block
* @param is input stream from which the data is read
* @param maxExpectedSize maximum expected block size 
* @throws IOException on file read failure
*/
FixedData::FixedData(FixedMeta* meta, BwProj::toolkit::IDataReadStream* is, int maxExpectedSize)
:FixedData(meta, is, maxExpectedSize, 0)
{

}

/**
* This constructor is provided to allow the contents of a fixed data
* block to be read when the size of the items in the data block is
* fixed and known in advance. This is used in one particular instance
* where the contents of the meta data block do not appear to be
* consistent.
*
* @param itemSize the size of the data items in the block
* @param is input stream from which the data is read
* @throws IOException on file read failure
*/
FixedData::FixedData(int itemSize, BwProj::toolkit::IDataReadStream* is)
:FixedData(itemSize, is, false)
{

}

/**
* This version of the above constructor allows us to limit the
* size of blocks we copy where we have an idea of the maximum expected
* block size. This prevents us from reading ridiculously large amounts
* of unnecessary data, causing OutOfMemory exceptions.
* 
* This constructor will also use the given minimum size in the case that the
* meta data block reports a size of 0
* 
* @param meta meta data about the contents of this fixed data block
* @param maxExpectedSize maximum expected block size
* @param minSize minimum size that will be read if size of block is reported as 0.
* @param is input stream from which the data is read
* @throws IOException on file read failure 
*/
FixedData::FixedData(FixedMeta* meta, BwProj::toolkit::IDataReadStream* is, int maxExpectedSize, int minSize)
{
	DWORD readBytes = 0;
    ByteArrayPtr buffer(new ByteArray2(is->available()));
	is->Read(buffer->get(), buffer->length(), readBytes);

    int itemCount = meta->getAdjustedItemCount();
    m_array.reset (new ObjectPtrArray(itemCount));
    m_offset.reset (new IntArray2(itemCount));

    int available = 0;

    for (int loop = 0; loop < itemCount; loop++)
    {
        ByteArrayPtr metaData = meta->getByteArray2Value(loop);
        int itemOffset = MPPUtility::getInt(metaData.get(), 4);

        if (itemOffset < 0 || itemOffset > (int) buffer->length())
        {
			continue;
        }

        int itemSize;
        if (loop + 1 == itemCount)
        {
			itemSize = buffer->length() - itemOffset;
        }
        else
        {
			ByteArrayPtr nextMetaData = meta->getByteArray2Value(loop + 1);
			int nextItemOffset = MPPUtility::getInt(nextMetaData.get(), 4);
			itemSize = nextItemOffset - itemOffset;
        }

        if (itemSize == 0)
        {
			itemSize = minSize;
        }

        available = buffer->length() - itemOffset;

        if (itemSize < 0 || itemSize > available)
        {
			if (maxExpectedSize == 0)
			{
				itemSize = available;
			}
			else
			{
				if (maxExpectedSize < available)
				{
					itemSize = maxExpectedSize;
				}
				else
				{
					itemSize = available;
				}
			}
        }

        if (maxExpectedSize != 0 && itemSize > maxExpectedSize)
        {
			itemSize = maxExpectedSize;
        }

        if (itemSize > 0)
        {
			(*m_array)[loop].reset (MPPUtility::cloneSubArray(buffer.get(), itemOffset, itemSize));
			(*m_offset)[loop] = itemOffset;
        }
    }
}

/**
* This constructor does the same job as the one above, but assumes that
* the item size reported in the meta information is wrong, and
* instead uses the supplied item size.
* 
* @param meta meta data about the contents of this fixed data block
* @param itemSize expected item size
* @param is input stream from which the data is read
* @throws IOException
*/
FixedData::FixedData(FixedMeta* meta, int itemSize, BwProj::toolkit::IDataReadStream* is)
{
	DWORD readBytes = 0;
	ByteArrayPtr buffer(new ByteArray2(is->available()));
	is->Read(buffer->get(), buffer->length(), readBytes);

    int itemCount = meta->getAdjustedItemCount();
    m_array.reset (new ObjectPtrArray(itemCount));
    m_offset.reset (new IntArray2(itemCount));

    ByteArrayPtr metaData;
    int itemOffset = 0;
    int available = 0;

    for (int loop = 0; loop < itemCount; loop++)
    {
        metaData = meta->getByteArray2Value(loop);
        itemOffset = MPPUtility::getInt(metaData.get(), 4);

        if (itemOffset > (int) buffer->length())
        {
			continue;
        }

        available = buffer->length() - itemOffset;

        if (itemSize < 0)
        {
			itemSize = available;
        }
        else if (itemSize > available)
		{
			itemSize = available;
		}

        (*m_array)[loop].reset (MPPUtility::cloneSubArray(buffer.get(), itemOffset, itemSize));
        (*m_offset)[loop] = itemOffset;
    }
}

/**
* This constructor is provided to allow the contents of a fixed data
* block to be read when the size of the items in the data block is
* fixed and known in advance. This is used in one particular instance
* where the contents of the meta data block do not appear to be
* consistent.
*
* @param itemSize the size of the data items in the block
* @param is input stream from which the data is read
* @param readRemainderBlock read the final block even if it is not full size
* @throws IOException on file read failure
*/
FixedData::FixedData(int itemSize, BwProj::toolkit::IDataReadStream* is, bool readRemainderBlock)
{
    int offset = 0;
    int itemCount = is->available() / itemSize;
    if (readRemainderBlock == true && is->available() % itemSize != 0)
    {
        ++itemCount;
    }

    m_array.reset (new ObjectPtrArray(itemCount));
    m_offset.reset (new IntArray2(itemCount));

    for (int loop = 0; loop < itemCount; loop++)
    {
        (*m_offset)[loop] = offset;

        int currentItemSize = itemSize;
        if (readRemainderBlock == true && (int) is->available() < itemSize)
        {
			currentItemSize = is->available();
        }
        (*m_array)[loop] = readByteArray2(is, currentItemSize);
        offset += itemSize;
    }
}

/**
* This method retrieves a byte array containing the data at the
* given index in the block. If no data is found at the given index
* this method returns null.
*
* @param index index of the data item to be retrieved
* @return byte array containing the requested data
*/
ByteArrayPtr FixedData::getByteArray2Value(int index)
{
    ByteArrayPtr result;

	if (index >= 0 && index < (int)m_array->length() && (*m_array)[index].get())
    {
        result = std::dynamic_pointer_cast<ByteArray2> ((*m_array)[index]);
    }

    return (result);
}

/**
* Accessor method used to retrieve the number of items held in
* this fixed data block. Note that this item count is made without
* reference to the meta data associated with this block.
*
* @return number of items in the block
*/
int FixedData::getItemCount()
{
    return (m_array->length());
}

/**
* Returns a flag indicating if the supplied offset is valid for
* the data in this fixed data block.
*
* @param offset offset value
* @return bool flag
*/
bool FixedData::isValidOffset(IntegerPtr offset)
{
    return (offset ? false : isValidOffset(offset->intValue()));
}

/**
* Returns a flag indicating if the supplied offset is valid for
* the data in this fixed data block.
*
* @param offset offset value
* @return bool flag
*/
bool FixedData::isValidOffset(int offset)
{
	return (offset >= 0 && offset < (int)m_array->length());
}

/**
* This method converts an offset value into an array index, which in
* turn allows the data present in the fixed block to be retrieved. Note
* that if the requested offset is not found, then this method returns -1.
*
* @param offset Offset of the data in the fixed block
* @return Index of data item within the fixed data block
*/
int FixedData::getIndexFromOffset(int offset)
{
    int result = -1;

	for (int loop = 0; loop < (int)m_offset->length(); loop++)
    {
        if ((*m_offset)[loop] == offset)
        {
			result = loop;
			break;
        }
    }

    return (result);
}

///**
//* This method dumps the contents of this FixedData block as a CString.
//* Note that this facility is provided as a debugging aid.
//*
//* @return formatted contents of this block
//*/
//CString FixedData::toString()
//{
//    StringWriter sw = new StringWriter();
//    PrintWriter pw = new PrintWriter(sw);
//
//    pw.println("BEGIN FixedData");
//    for (int loop = 0; loop < m_array.length; loop++)
//    {
//        pw.println("   Data at index: " + loop + " offset: " + m_offset[loop]);
//        pw.println("  " + MPPUtility.hexdump((ByteArrayPtr) m_array[loop], true));
//    }
//    pw.println("END FixedData");
//
//    pw.println();
//    pw.close();
//    return (sw.toString());
//}