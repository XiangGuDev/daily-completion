#pragma once

#include "BlockList.h"

/**
 * A simple implementation of BlockList
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */
class BlockListImpl : public BlockList
{
public:
	/**
	* remove the specified block from the list
	*
	* @param index the index of the specified block; if the index is
	*              out of range, that's ok
	*/
	void zap(int index);

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
	ListManagedBlockPtr remove(int index);

    /**
     * get the blocks making up a particular stream in the list. The
     * blocks are removed from the list.
     *
     * @param startBlock the index of the first block in the stream
     *
     * @return the stream as an array of correctly ordered blocks
     *
     * @exception IOException if blocks are missing
     */
	ListManagedBlockArrayPtr fetchBlocks(int startBlock, int headerPropertiesStartBlock);

    /**
     * set the associated BlockAllocationTable
     *
     * @param bat the associated BlockAllocationTable
     */
	void setBAT(BlockAllocationTableReaderPtr bat);
    
    /**
     * Returns the count of the number of blocks
     */
	int blockCount();

protected:
	BlockListImpl();

	/**
	* provide blocks to manage
	*
	* @param blocks blocks to be managed
	*/
	void setBlocks(ListManagedBlockArrayPtr blocks);

	/**
	* Unit testing method. Gets, without sanity checks or
	*  removing.
	*/
	ListManagedBlockPtr get(int index);

    /**
     * Returns the number of remaining blocks
     */
	int remainingBlocks();

private:
	ListManagedBlockArrayPtr		_blocks;
	BlockAllocationTableReaderPtr	_bat;
};