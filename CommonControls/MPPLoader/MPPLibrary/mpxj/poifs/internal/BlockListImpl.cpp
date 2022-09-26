#include "stdafx.h"
#include "BlockListImpl.h"

BlockListImpl::BlockListImpl()
{
    //_blocks = new ListManagedBlock[ 0 ];
    //_bat    = null;
}

/**
    * provide blocks to manage
    *
    * @param blocks blocks to be managed
    */
void BlockListImpl::setBlocks(ListManagedBlockArrayPtr blocks)
{
    _blocks = blocks;
}

/**
    * remove the specified block from the list
    *
    * @param index the index of the specified block; if the index is
    *              out of range, that's ok
    */
void BlockListImpl::zap(int index)
{
    if ((index >= 0) && (index < (int)_blocks->length()))
    {
		(*_blocks)[index].reset();
    }
}

/**
    * Unit testing method. Gets, without sanity checks or
    *  removing.
    */
ListManagedBlockPtr BlockListImpl::get(int index)
{
    return (*_blocks)[index];
}

/**
    * remove and return the specified block from the list
    *
    * @param index the index of the specified block
    *
    * @return the specified block
    *
    * @exception IOtoolkit::Exception if the index is out of range or has
    *            already been removed
    */
ListManagedBlockPtr BlockListImpl::remove(int index)
{
	ListManagedBlockPtr result;// = null;

    //try
    //{
        result = (*_blocks)[ index ];
        if (!result)
        {
            throw toolkit::Exception(L"does your POIFS have circular or duplicate block references?");
        }
		(*_blocks)[index].reset();
    //}
    //catch (ArrayIndexOutOfBoundstoolkit::Exception ignored)
    //{
    //    throw new IOtoolkit::Exception("Cannot remove block[ " + index
    //                            + " ]; out of range[ 0 - " +
    //                            (_blocks.length-1) + " ]");
    //}
    return result;
}

/**
    * get the blocks making up a particular stream in the list. The
    * blocks are removed from the list.
    *
    * @param startBlock the index of the first block in the stream
    *
    * @return the stream as an array of correctly ordered blocks
    *
    * @exception IOtoolkit::Exception if blocks are missing
    */
ListManagedBlockArrayPtr BlockListImpl::fetchBlocks(int startBlock, int headerPropertiesStartBlock)
{
    if (!_bat)
    {
        throw toolkit::Exception(L"Improperly initialized list: no block allocation table provided");
    }
    return _bat->fetchBlocks(startBlock, headerPropertiesStartBlock, this);
}

/**
    * set the associated BlockAllocationTable
    *
    * @param bat the associated BlockAllocationTable
    */
void BlockListImpl::setBAT(BlockAllocationTableReaderPtr bat)
{
    if (_bat)
    {
        throw toolkit::Exception(L"Attempt to replace existing BlockAllocationTable");
    }
    _bat = bat;
}
    
/**
    * Returns the count of the number of blocks
    */
int BlockListImpl::blockCount()
{
    return _blocks->length();
}

/**
    * Returns the number of remaining blocks
    */
int BlockListImpl::remainingBlocks()
{
    int c = 0;
    for(int i=0; i < (int)_blocks->length(); i++)
	{
        if((*_blocks)[i])
			c++;
    }
    return c;
}