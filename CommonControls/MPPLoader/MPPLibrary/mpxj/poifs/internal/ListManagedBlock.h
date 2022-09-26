#pragma once

/**
 * An interface for blocks managed by a list that works with a
 * BlockAllocationTable to keep block sequences straight
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class ListManagedBlock
{
public:
    /**
     * Get the data from the block
     *
     * @return the block's data as a byte array
     *
     * @exception IOException if there is no data
     */
	virtual ByteArrayPtr getData() = 0;

};   // end public interface ListManagedBlock

typedef std::shared_ptr<ListManagedBlock> ListManagedBlockPtr;
typedef Array2<ListManagedBlockPtr> ListManagedBlockArray;
typedef std::shared_ptr<ListManagedBlockArray> ListManagedBlockArrayPtr;