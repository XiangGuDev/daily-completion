#pragma once

#include "POIFSBigBlockSize.h"
#include "BlockWritable.h"
#include "ListManagedBlock.h"
#include "DataInputBlock.h"
#include "BlockListImpl.h"

class SmallDocumentBlock;

typedef std::shared_ptr<SmallDocumentBlock>			SmallDocumentBlockPtr;

typedef Array2<SmallDocumentBlockPtr>				SmallDocumentBlockArray;
typedef std::shared_ptr<SmallDocumentBlockArray>	SmallDocumentBlockArrayPtr;

typedef List<SmallDocumentBlockPtr>					SmallDocumentBlockList;
typedef std::shared_ptr<SmallDocumentBlockList>		SmallDocumentBlockListPtr;


/**
 * Storage for documents that are too small to use regular
 * DocumentBlocks for their data
 *
 * @author  Marc Johnson (mjohnson at apache dot org)
 */
class SmallDocumentBlock : public BlockWritable, public ListManagedBlock
{
public:
    /**
     * convert a single long array into an array of SmallDocumentBlock
     * instances
     *
     * @param array the BYTE array to be converted
     * @param size the intended size of the array (which may be smaller)
     *
     * @return an array of SmallDocumentBlock instances, filled from
     *         the array
     */
	static SmallDocumentBlockArrayPtr convert(POIFSBigBlockSize bigBlockSize, ByteArrayPtr array, int size);

    /**
     * fill out a List of SmallDocumentBlocks so that it fully occupies
     * a set of big blocks
     *
     * @param blocks the List to be filled out
     *
     * @return number of big blocks the list encompasses
     */
	static int fill(POIFSBigBlockSize bigBlockSize, SmallDocumentBlockListPtr blocks);

    /**
     * Factory for creating SmallDocumentBlocks from DocumentBlocks
     *
     * @param store the original DocumentBlocks
     * @param size the total document size
     *
     * @return an array of new SmallDocumentBlocks instances
     *
     * @exception IOException on errors reading from the DocumentBlocks
     * @exception ArrayIndexOutOfBoundsException if, somehow, the store
     *            contains less data than size indicates
     */
	static SmallDocumentBlockArrayPtr convert(POIFSBigBlockSize bigBlockSize, BlockWritableArrayPtr store, int size);

    /**
     * create a list of SmallDocumentBlock's from raw data
     *
     * @param blocks the raw data containing the SmallDocumentBlock
     *               data
     *
     * @return a List of SmallDocumentBlock's extracted from the input
     */
	static SmallDocumentBlockListPtr extract(POIFSBigBlockSize bigBlockSize, ListManagedBlockArrayPtr blocks);

	static DataInputBlockPtr getDataInputBlock(SmallDocumentBlockArrayPtr blocks, int offset);

    /**
     * Calculate the storage size of a set of SmallDocumentBlocks
     *
     * @param size number of SmallDocumentBlocks
     *
     * @return total size
     */
	static int calcSize(int size);

    /**
     * Write the storage to an BwProj::toolkit::IDataWriteStream*
     *
     * @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */
	void writeBlocks(BwProj::toolkit::IDataWriteStream* stream);

    /**
     * Get the data from the block
     *
     * @return the block's data as a BYTE array
     *
     * @exception IOException if there is no data
     */
	ByteArrayPtr getData();
    
	POIFSBigBlockSize getBigBlockSize();

private:
	SmallDocumentBlock(POIFSBigBlockSize bigBlockSize, ByteArrayPtr data, int index);

	SmallDocumentBlock(POIFSBigBlockSize bigBlockSize);

	static int getBlocksPerBigBlock(POIFSBigBlockSize bigBlockSize);

	static SmallDocumentBlockPtr makeEmptySmallDocumentBlock(POIFSBigBlockSize bigBlockSize);

	static int convertToBlockCount(int size);

private:
	static const int	BLOCK_SHIFT = 6;
	static const BYTE	_default_fill = (BYTE)0xff;
	static const int	_block_size = 1 << BLOCK_SHIFT;
	static const int	BLOCK_MASK = _block_size - 1;

	ByteArrayPtr		_data;
	int					_blocks_per_big_block;
	POIFSBigBlockSize	_bigBlockSize;
};

class SmallDocumentBlockList2 : public BlockListImpl
{
public:
	/**
	* Constructor SmallDocumentBlockList
	*
	* @param blocks a list of SmallDocumentBlock instances
	*/

	SmallDocumentBlockList2(SmallDocumentBlockListPtr blocks);
};   // end public class SmallDocumentBlockList