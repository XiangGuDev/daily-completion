#pragma once

#include "POIFSBigBlockSize.h"
#include "POIFSConstants.h"
#include "ListManagedBlock.h"
#include "LittleEndianConsts.h"

class BlockList;

/**
 * This class manages and creates the Block Allocation Table, which is
 * basically a set of linked lists of block indices.
 * <P>
 * Each block of the filesystem has an index. The first block, the
 * header, is skipped; the first block after the header is index 0,
 * the next is index 1, and so on.
 * <P>
 * A block's index is also its index into the Block Allocation
 * Table. The entry that it finds in the Block Allocation Table is the
 * index of the next block in the linked list of blocks making up a
 * file, or it is set to -2: end of list.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class BlockAllocationTableReader// : public std::enable_shared_from_this<BlockAllocationTableReader>
{
	friend class BlockListImpl;
public:
    /**
     * create a BlockAllocationTableReader for an existing filesystem. Side
     * effect: when this method finishes, the BAT blocks will have
     * been removed from the raw block list, and any blocks labeled as
     * 'unused' in the block allocation table will also have been
     * removed from the raw block list.
     *
     * @param block_count the number of BAT blocks making up the block
     *                    allocation table
     * @param block_array the array of BAT block indices from the
     *                    filesystem's header
     * @param xbat_count the number of XBAT blocks
     * @param xbat_index the index of the first XBAT block
     * @param raw_block_list the list of RawDataBlocks
     *
     * @exception IOException if, in trying to create the table, we
     *            encounter logic errors
     */
	BlockAllocationTableReader(POIFSBigBlockSize bigBlockSize, int block_count, IntArray2Ptr block_array,
		int xbat_count, int xbat_index, std::shared_ptr<BlockList> raw_block_list);

	static void sanityCheckBlockCount(int block_count);

    /**
     * create a BlockAllocationTableReader from an array of raw data blocks
     *
     * @param blocks the raw data
     * @param raw_block_list the list holding the managed blocks
     *
     * @exception IOException
     */
	BlockAllocationTableReader(POIFSBigBlockSize bigBlockSize,
		ListManagedBlockArrayPtr blocks, std::shared_ptr<BlockList> raw_block_list);

protected:
	BlockAllocationTableReader(POIFSBigBlockSize size);

    /**
     * walk the entries from a specified point and return the
     * associated blocks. The associated blocks are removed from the
     * block list
     *
     * @param startBlock the first block in the chain
     * @param blockList the raw data block list
     *
     * @return array of ListManagedBlocks, in their correct order
     *
     * @exception IOException if there is a problem acquiring the blocks
     */
	ListManagedBlockArrayPtr fetchBlocks(int startBlock, int headerPropertiesStartBlock, BlockList* blockList);

    // methods for debugging reader

    /**
     * determine whether the block specified by index is used or not
     *
     * @param index index of block in question
     *
     * @return true if the specific block is used, else false
     */
	bool isUsed(int index);

    /**
     * return the next block index
     *
     * @param index of the current block
     *
     * @return index of the next block (may be
     *         POIFSConstants.END_OF_CHAIN, indicating end of chain
     *         (duh))
     *
     * @exception IOException if the current block is unused
     */
	int getNextBlockIndex(int index);

    /**
     * Convert an array of blocks into a set of integer indices
     *
     * @param blocks the array of blocks containing the indices
     * @param raw_blocks the list of blocks being managed. Unused
     *                   blocks will be eliminated from the list
     */
	void setEntries(ListManagedBlockArrayPtr blocks, std::shared_ptr<BlockList> raw_blocks);

private:
	/**
	* Maximum number size (in blocks) of the allocation table as supported by
	* POI.<br/>
	*
	* This constant has been chosen to help POI identify corrupted data in the
	* header block (rather than crash immediately with {@link OutOfMemoryError}
	* ). It's not clear if the compound document format actually specifies any
	* upper limits. For files with 512 byte blocks, having an allocation table
	* of 65,335 blocks would correspond to a total file size of 4GB. Needless
	* to say, POI probably cannot handle files anywhere near that size.
	*/
	static const int MAX_BLOCK_COUNT = 65535;
	//IntList _entries;
	CAtlArray<int> _entries;
	POIFSBigBlockSize bigBlockSize;
};

typedef std::shared_ptr<BlockAllocationTableReader> BlockAllocationTableReaderPtr;