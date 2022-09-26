#pragma once

#include "BlockAllocationTableReader.h"

/**
 * Interface for lists of blocks that are mapped by block allocation
 * tables
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class BlockList
{
public:
    /**
     * remove the specified block from the list
     *
     * @param index the index of the specified block; if the index is
     *              out of range, that's ok
     */

    virtual void zap(int index) = 0;

    /**
     * remove and return the specified block from the list
     *
     * @param index the index of the specified block
     *
     * @return the specified block
     *
     * @exception IOException if the index is out of range or has
     *            already been removed
     */

	virtual ListManagedBlockPtr remove(int index) = 0;

    /**
     * get the blocks making up a particular stream in the list. The
     * blocks are removed from the list.
     *
     * @param startBlock the index of the first block in the stream
     * @param headerPropertiesStartBlock the index of the first header block in the stream
     *
     * @return the stream as an array of correctly ordered blocks
     *
     * @exception IOException if blocks are missing
     */

	virtual ListManagedBlockArrayPtr fetchBlocks(int startBlock, int headerPropertiesStartBlock) = 0;

    /**
     * set the associated BlockAllocationTable
     *
     * @param bat the associated BlockAllocationTable
     *
     * @exception IOException
     */

	virtual void setBAT(BlockAllocationTableReaderPtr bat) = 0;
    
	virtual int blockCount() = 0;
};   // end public interface BlockList

typedef std::shared_ptr<BlockList> BlockListPtr;