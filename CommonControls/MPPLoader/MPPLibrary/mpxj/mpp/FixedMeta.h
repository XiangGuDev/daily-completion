#pragma once

#include "MPPComponent.h"
#include "FixedMetaItemSizeProvider.h"

/**
 * This class is used to represent the "FixedMeta" file entries that are
 * found in a Microsoft Project MPP file. These file entries describe the
 * structure of the "FixedData" blocks with which they are associated.
 * The structure of the Fixed Meta block is not currently fully understood.
 *
 * Note that this class has package level access only, and is not intended
 * for use outside of this context.
 */
class FixedMeta : public MPPComponent
{
public:
	/**
	 * Constructor. Reads the meta data from an input stream. Note that
	 * this version of the constructor copes with more MSP inconsistencies.
	 * We already know the block size, so we ignore the item count in the
	 * block and work it out for ourselves.
	 *
	 * @param is input stream from which the meta data is read
	 * @param itemSize size of each item in the block
	 * @throws IOException on file read failure
	 */
	FixedMeta(BwProj::toolkit::IDataReadStream* is, int itemSize);

	/**
	 * Constructor. Supply an item size provider to allow different strategies to be
	 * used to determine the correct item size.
	 *
	 * @param is input stream from which the meta data is read
	 * @param itemSizeProvider item size provider used to calculate the item size
	 * @throws IOException
	 */
	FixedMeta(BwProj::toolkit::IDataReadStream* is, FixedMetaItemSizeProvider* itemSizeProvider);

	/**
	 * Constructor, allowing a selection of possible block sizes to be supplied.
	 * 
	 * @param is input stream
	 * @param itemSizes list of potential block sizes
	 */
	FixedMeta(BwProj::toolkit::IDataReadStream* is, int itemSizes[], int length);

	/**
	 * This method retrieves the number of items in the FixedData block, as reported in the block header.
	 *
	 * @return number of items in the fixed data block
	 */
	int getItemCount();

	/**
	 * This method retrieves the number of items in the FixedData block.
	 * Where we don't trust the number of items reported by the block header
	 * this value is adjusted based on what we know about the block size
	 * and the size of the individual items.
	 *
	 * @return number of items in the fixed data block
	 */
	int getAdjustedItemCount();

	/**
	 * This method retrieves a byte array containing the data at the
	 * given index in the block. If no data is found at the given index
	 * this method returns null.
	 *
	 * @param index index of the data item to be retrieved
	 * @return byte array containing the requested data
	 */
	ByteArrayPtr getByteArray2Value(int index);

	///**
	// * This method dumps the contents of this FixedMeta block as a CString.
	// * Note that this facility is provided as a debugging aid.
	// *
	// * @return formatted contents of this block
	// */
	//virtual CString toString();

protected:
	void FixedMetaProc(BwProj::toolkit::IDataReadStream* is, FixedMetaItemSizeProvider* itemSizeProvider);

private:
	/**
	 * Number of items in the data block, as reported in the block header.
	 */
	int m_itemCount;

	/**
	 * Number of items in the data block, adjusted based on block size and item size.
	 */
	int m_adjustedItemCount;

	/**
	 * Unknown data items relating to each entry in the fixed data block.
	 */
	ObjectArrayPtr m_array;

	/**
	 * Constant representing the magic number appearing
	 * at the start of the block.
	 */
	static const long MAGIC = 0xFADFADBA;

	/**
	 * Header size.
	 */
	static const int HEADER_SIZE = 16;
};

typedef std::shared_ptr<FixedMeta> FixedMetaPtr;