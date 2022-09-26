#include "stdafx.h"
#include "BlockAllocationTableReader.h"
#include "RawDataBlock.h"
#include "BlockList.h"
#include "LittleEndian.h"

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
    * @exception IOtoolkit::Exception if, in trying to create the table, we
    *            encounter logic errors
    */
BlockAllocationTableReader::BlockAllocationTableReader(POIFSBigBlockSize bigBlockSize, 
	int block_count, IntArray2Ptr block_array, int xbat_count, int xbat_index, BlockListPtr raw_block_list)
:BlockAllocationTableReader(bigBlockSize)
{
    sanityCheckBlockCount(block_count);

    // We want to get the whole of the FAT table
    // To do this:
    //  * Work through raw_block_list, which points to the 
    //     first (up to) 109 BAT blocks
    //  * Jump to the XBAT offset, and read in XBATs which
    //     point to more BAT blocks
    int          limit    = min(block_count, (int)block_array->length());
    int          block_index;
        
    // This will hold all of the BAT blocks in order
	RawDataBlockArrayPtr blocks (new RawDataBlockArray(block_count));

    // Process the first (up to) 109 BAT blocks
    for (block_index = 0; block_index < limit; block_index++)
    {
        // Check that the sector number of the BAT block is a valid one
        int nextOffset = (*block_array)[ block_index ];
        if(nextOffset > raw_block_list->blockCount())
		{
			throw toolkit::Exception(L"Your file contains sectors, but the initial DIFAT array at index");
            //throw std::bad_exception("Your file contains " + raw_block_list.blockCount() + 
            //        " sectors, but the initial DIFAT array at index " + block_index +
            //        " referenced block # " + nextOffset + ". This isn't allowed and " +
            //        " your file is corrupt");
        }
        // Record the sector number of this BAT block 
        (*blocks)[ block_index ] =
            std::dynamic_pointer_cast< RawDataBlock > (raw_block_list->remove(nextOffset));
    }
        
    // Process additional BAT blocks via the XBATs
    if (block_index < block_count)
    {

        // must have extended blocks
        if (xbat_index < 0)
        {
            throw toolkit::Exception(L"BAT count exceeds limit, yet XBAT index indicates no valid entries");
        }
        int chain_index           = xbat_index;
        int max_entries_per_block = bigBlockSize.getXBATEntriesPerBlock(); 
        int chain_index_offset    = bigBlockSize.getNextXBATChainOffset(); 

        // Each XBAT block contains either:
        //  (maximum number of sector indexes) + index of next XBAT
        //  some sector indexes + FREE sectors to max # + EndOfChain
        for (int j = 0; j < xbat_count; j++)
        {
            limit = min(block_count - block_index, max_entries_per_block);
            ByteArrayPtr data   = raw_block_list->remove(chain_index)->getData();
            int    offset = 0;

            for (int k = 0; k < limit; k++)
            {
				(*blocks)[block_index++] = std::dynamic_pointer_cast<RawDataBlock>(
										raw_block_list->remove(LittleEndian::getInt(data, offset)));
                offset += LittleEndianConsts::INT_SIZE;
            }
            chain_index = LittleEndian::getInt(data, chain_index_offset);
            if (chain_index == POIFSConstants::END_OF_CHAIN)
            {
                break;
            }
        }
    }
    if (block_index != block_count)
    {
        throw toolkit::Exception(L"Could not find all blocks");
    }

    // Now that we have all of the raw data blocks which make
    //  up the FAT, go through and create the indices
	ListManagedBlockArrayPtr mbarr(new ListManagedBlockArray(blocks->length()));
	for (size_t i = 0; i < blocks->length(); i++)
		(*mbarr)[i] = std::dynamic_pointer_cast<ListManagedBlock>((*blocks)[i]);
	setEntries(mbarr, raw_block_list);
}

void BlockAllocationTableReader::sanityCheckBlockCount(int block_count)
{
	if (block_count <= 0)
	{
		throw toolkit::Exception(L"Illegal block count; minimum count is 1, got instead" );
	}
	if (block_count > MAX_BLOCK_COUNT)
	{
		throw toolkit::Exception(L"Block count is too high. POI maximum is.");
	}
}

/**
    * create a BlockAllocationTableReader from an array of raw data blocks
    *
    * @param blocks the raw data
    * @param raw_block_list the list holding the managed blocks
    *
    * @exception IOtoolkit::Exception
    */
BlockAllocationTableReader::BlockAllocationTableReader(POIFSBigBlockSize bigBlockSize,
	ListManagedBlockArrayPtr blocks, std::shared_ptr<BlockList> raw_block_list)
	:BlockAllocationTableReader(bigBlockSize)
{
    setEntries(blocks, raw_block_list);
}

BlockAllocationTableReader::BlockAllocationTableReader(POIFSBigBlockSize size)
: bigBlockSize(size)
{
    //_entries = new IntList();
}

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
    * @exception IOtoolkit::Exception if there is a problem acquiring the blocks
    */
ListManagedBlockArrayPtr BlockAllocationTableReader::fetchBlocks(int startBlock, 
	int headerPropertiesStartBlock, BlockList* blockList)
{
	List<ListManagedBlockPtr> blocks;// = new ArrayList<ListManagedBlock>();
    int  currentBlock = startBlock;
    bool firstPass = true;
    ListManagedBlockPtr dataBlock;

    // Process the chain from the start to the end
    // Normally we have header, data, end
    // Sometimes we have data, header, end
    // For those cases, stop at the header, not the end
    while (currentBlock != POIFSConstants::END_OF_CHAIN)
	{
        try {
            // Grab the data at the current block offset
            dataBlock = blockList->remove(currentBlock);
            blocks.Add(dataBlock);
            // Now figure out which block we go to next
            currentBlock = _entries[currentBlock];
            firstPass = false;
        } catch(toolkit::Exception& e) {
            if(currentBlock == headerPropertiesStartBlock)
			{
                // Special case where things are in the wrong order
                currentBlock = POIFSConstants::END_OF_CHAIN;
            } else if(currentBlock == 0 && firstPass) {
                // Special case where the termination isn't done right
                //  on an empty set
                currentBlock = POIFSConstants::END_OF_CHAIN;
            } else {
                // Ripple up
                throw e;
            }
        }
    }

	ListManagedBlockArrayPtr arr(new ListManagedBlockArray(blocks.GetCount()));
	for (size_t i = 0; i < blocks.GetCount(); i++)
		(*arr)[i] = blocks.GetAt(i);

	return arr;

    //return blocks.toArray(new ListManagedBlock[blocks.size()]);
}

// methods for debugging reader

/**
    * determine whether the block specified by index is used or not
    *
    * @param index index of block in question
    *
    * @return true if the specific block is used, else false
    */
bool BlockAllocationTableReader::isUsed(int index)
{
	if (index < 0 || index >= (int)_entries.GetCount())
		return false;

	return _entries[index] != -1;

    //try {
    //    return _entries.get(index) != -1;
    //} catch (IndexOutOfBoundstoolkit::Exception e) {
    //    // ignored
    //    return false;
    //}
}

/**
    * return the next block index
    *
    * @param index of the current block
    *
    * @return index of the next block (may be
    *         POIFSConstants.END_OF_CHAIN, indicating end of chain
    *         (duh))
    *
    * @exception IOtoolkit::Exception if the current block is unused
    */
int BlockAllocationTableReader::getNextBlockIndex(int index)
{
    if (isUsed(index))
	{
        return _entries[index];
    }
    throw toolkit::Exception(L"index is unused");
}

/**
    * Convert an array of blocks into a set of integer indices
    *
    * @param blocks the array of blocks containing the indices
    * @param raw_blocks the list of blocks being managed. Unused
    *                   blocks will be eliminated from the list
    */
void BlockAllocationTableReader::setEntries(ListManagedBlockArrayPtr blocks, std::shared_ptr<BlockList> raw_blocks)
{
    int limit = bigBlockSize.getBATEntriesPerBlock(); 

    for (int block_index = 0; block_index < (int)blocks->length(); block_index++)
    {
        ByteArrayPtr data   = (*blocks)[ block_index ]->getData();
        int    offset = 0;

        for (int k = 0; k < limit; k++)
        {
            int entry = LittleEndian::getInt(data, offset);

            if (entry == POIFSConstants::UNUSED_BLOCK)
            {
				raw_blocks->zap(_entries.GetCount());
            }
            _entries.Add(entry);
            offset += LittleEndianConsts::INT_SIZE;
        }

        // discard block
		(*blocks)[block_index].reset();// = null;
    }

	//raw_blocks->setBAT(shared_from_this());
}