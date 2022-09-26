#pragma once

#include "MPPComponent.h"
#include "FixedMeta.h"

/**
 * This class is used to represent the "FixedData" file entries that are
 * found in a Microsoft Project MPP file. The name "Fixed Data" appears to
 * refer to the fact that the items held in these blocks have a known maximum
 * size, rather than all of the items being identically sized records.
 *
 * Note that this class has package level access only, and is not intended
 * for use outside of this context.
 */
class FixedData : public MPPComponent
{
public:
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
	FixedData(FixedMeta* meta, BwProj::toolkit::IDataReadStream* is);

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
	FixedData(FixedMeta* meta, BwProj::toolkit::IDataReadStream* is, int maxExpectedSize);

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
	FixedData(FixedMeta* meta, BwProj::toolkit::IDataReadStream* is, int maxExpectedSize, int minSize);

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
	FixedData(FixedMeta* meta, int itemSize, BwProj::toolkit::IDataReadStream* is);

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
	FixedData(int itemSize, BwProj::toolkit::IDataReadStream* is);

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
	FixedData(int itemSize, BwProj::toolkit::IDataReadStream* is, bool readRemainderBlock);

   /**
    * This method retrieves a byte array containing the data at the
    * given index in the block. If no data is found at the given index
    * this method returns null.
    *
    * @param index index of the data item to be retrieved
    * @return byte array containing the requested data
    */
	ByteArrayPtr getByteArray2Value(int index);

   /**
    * Accessor method used to retrieve the number of items held in
    * this fixed data block. Note that this item count is made without
    * reference to the meta data associated with this block.
    *
    * @return number of items in the block
    */
	int getItemCount();

   /**
    * Returns a flag indicating if the supplied offset is valid for
    * the data in this fixed data block.
    *
    * @param offset offset value
    * @return bool flag
    */
	bool isValidOffset(IntegerPtr offset);

   /**
    * Returns a flag indicating if the supplied offset is valid for
    * the data in this fixed data block.
    *
    * @param offset offset value
    * @return bool flag
    */
	bool isValidOffset(int offset);

   /**
    * This method converts an offset value into an array index, which in
    * turn allows the data present in the fixed block to be retrieved. Note
    * that if the requested offset is not found, then this method returns -1.
    *
    * @param offset Offset of the data in the fixed block
    * @return Index of data item within the fixed data block
    */
	int getIndexFromOffset(int offset);

 //  /**
 //   * This method dumps the contents of this FixedData block as a CString.
 //   * Note that this facility is provided as a debugging aid.
 //   *
 //   * @return formatted contents of this block
 //   */
	//virtual CString toString();

private:
   /**
    * An array containing all of the items of data held in this block.
    */
   ObjectArrayPtr m_array;

   /**
    * Array containing offset values for each item in the array.
    */
   IntArray2Ptr m_offset;

};

typedef std::shared_ptr<FixedData> FixedDataPtr;