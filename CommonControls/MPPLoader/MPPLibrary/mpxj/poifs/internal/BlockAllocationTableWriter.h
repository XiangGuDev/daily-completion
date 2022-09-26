#pragma once

#include "POIFSBigBlockSize.h"
#include "BATManaged.h"
#include "BlockWritable.h"
#include "BATBlock.h"

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
class BlockAllocationTableWriter : public BlockWritable, public BATManaged
{
public:
    /**
     * create a BlockAllocationTableWriter
     */
	BlockAllocationTableWriter(POIFSBigBlockSize bigBlockSize);

    /**
     * Create the BATBlocks we need
     *
     * @return start block index of BAT blocks
     */
	int createBlocks();

    /**
     * Allocate space for a block of indices
     *
     * @param blockCount the number of blocks to allocate space for
     *
     * @return the starting index of the blocks
     */
	int allocateSpace(int blockCount);

    /**
     * get the starting block
     *
     * @return the starting block index
     */
	int getStartBlock();

    /**
     * create the BATBlocks
     */
	void simpleCreateBlocks();

    /**
     * Write the storage to an OutputStream
     *
     * @param stream the OutputStream to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */
	void writeBlocks(BwProj::toolkit::IDataWriteStream* stream);
    
    /**
     * Write the BAT into its associated block
     */
	static void writeBlock(BATBlockPtr bat, ByteArrayPtr block);

    /**
     * Return the number of BigBlock's this instance uses
     *
     * @return count of BigBlock instances
     */
	int countBlocks();

    /**
     * Set the start block for this instance
     */
	void setStartBlock(int start_block);

private:
	CAtlArray<int>		_entries;
	BATBlockArrayPtr	_blocks;
	int					_start_block;
	POIFSBigBlockSize	_bigBlockSize;
};

typedef std::shared_ptr<BlockAllocationTableWriter> BlockAllocationTableWriterPtr;