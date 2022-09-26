#include "stdafx.h"
#include "BATBlock.h"
#include "LittleEndian.h"

/**
    * Create a single instance initialized with default values
    */
BATBlock::BATBlock(POIFSBigBlockSize bigBlockSize)
:BigBlock(bigBlockSize)
{
    int _entries_per_block = bigBlockSize.getBATEntriesPerBlock();
    _values.reset (new IntArray2(_entries_per_block));
    _has_free_sectors = true;

    //Arrays.fill(_values, POIFSConstants::UNUSED_BLOCK);
	memset(_values->get(), POIFSConstants::UNUSED_BLOCK, sizeof(int) * _entries_per_block);
}

/**
    * Create a single instance initialized (perhaps partially) with entries
    *
    * @param entries the array of block allocation table entries
    * @param start_index the index of the first entry to be written
    *                    to the block
    * @param end_index the index, plus one, of the last entry to be
    *                  written to the block (writing is for all index
    *                  k, start_index <= k < end_index)
    */

BATBlock::BATBlock(POIFSBigBlockSize bigBlockSize, IntArray2Ptr entries, int start_index, int end_index)
:BATBlock(bigBlockSize)
{
    for (int k = start_index; k < end_index; k++)
	{
        (*_values)[k - start_index] = (*entries)[k];
    }
        
    // Do we have any free sectors?
    if(end_index - start_index == _values->length())
	{
        recomputeFree();
    }
}
    
void BATBlock::recomputeFree()
{
    bool hasFree = false;
	for (size_t k = 0; k < _values->length(); k++)
	{
        if((*_values)[k] == POIFSConstants::UNUSED_BLOCK)
		{
            hasFree = true;
            break;
        }
    }
    _has_free_sectors = hasFree;
}

/**
    * Create a single BATBlock from the byte buffer, which must hold at least
    *  one big block of data to be read.
    */
BATBlockPtr BATBlock::createBATBlock(POIFSBigBlockSize bigBlockSize, ByteArrayPtr data)
{
    // Create an empty block
	BATBlockPtr block(new BATBlock(bigBlockSize));
       
    // Fill it
	ByteArrayPtr buffer(new ByteArray2(LittleEndian::INT_SIZE));
	for (size_t i = 0; i<block->_values->length(); i++)
	{
		ATLASSERT(FALSE);
        //data->get(buffer);	这条语句不知道是否是对data的更新??
        (*block->_values)[i] = LittleEndian::getInt(buffer);
    }
    block->recomputeFree();
       
    // All done
    return block;
}
    
/**
    * Creates a single BATBlock, with all the values set to empty.
    */
BATBlockPtr BATBlock::createEmptyBATBlock(POIFSBigBlockSize bigBlockSize, bool isXBAT)
{
    BATBlockPtr block( new BATBlock(bigBlockSize));
    if(isXBAT) {
        block->setXBATChain(bigBlockSize, POIFSConstants::END_OF_CHAIN);
    }
    return block;
}

/**
    * Create an array of BATBlocks from an array of int block
    * allocation table entries
    *
    * @param entries the array of int entries
    *
    * @return the newly created array of BATBlocks
    */
BATBlockArrayPtr BATBlock::createBATBlocks(POIFSBigBlockSize bigBlockSize, IntArray2Ptr entries)
{
    int        block_count = calculateStorageRequirements(bigBlockSize, entries->length());
    BATBlockArrayPtr blocks (new BATBlockArray( block_count ));
    int        index       = 0;
    int        remaining   = entries->length();

    int _entries_per_block = bigBlockSize.getBATEntriesPerBlock();
	for (size_t j = 0; j < entries->length(); j += _entries_per_block)
    {
        (*blocks)[ index++ ].reset (new BATBlock(bigBlockSize, entries, j,
                                            (remaining > _entries_per_block)
                                            ? j + _entries_per_block
                                            : entries->length()));
        remaining         -= _entries_per_block;
    }
    return blocks;
}
    
/**
    * Create an array of XBATBlocks from an array of int block
    * allocation table entries
    *
    * @param entries the array of int entries
    * @param startBlock the start block of the array of XBAT blocks
    *
    * @return the newly created array of BATBlocks
    */

BATBlockArrayPtr BATBlock::createXBATBlocks(POIFSBigBlockSize bigBlockSize,
                                            IntArray2Ptr entries, int startBlock)
{
    int        block_count =
        calculateXBATStorageRequirements(bigBlockSize, entries->length());
    BATBlockArrayPtr blocks (new BATBlockArray( block_count ));
	size_t        index = 0;
    int        remaining   = entries->length();

    int _entries_per_xbat_block = bigBlockSize.getXBATEntriesPerBlock();
    if (block_count != 0)
    {
        for (size_t j = 0; j < entries->length(); j += _entries_per_xbat_block)
        {
			(*blocks)[index++].reset(
                new BATBlock(bigBlockSize, entries, j,
                                (remaining > _entries_per_xbat_block)
                                ? j + _entries_per_xbat_block
                                : entries->length()));
            remaining         -= _entries_per_xbat_block;
        }
        for (index = 0; index < blocks->length() - 1; index++)
        {
            (*blocks)[ index ]->setXBATChain(bigBlockSize, startBlock + index + 1);
        }
        (*blocks)[ index ]->setXBATChain(bigBlockSize, POIFSConstants::END_OF_CHAIN);
    }
    return blocks;
}

/**
    * Calculate how many BATBlocks are needed to hold a specified
    * number of BAT entries.
    *
    * @param entryCount the number of entries
    *
    * @return the number of BATBlocks needed
    */
int BATBlock::calculateStorageRequirements(POIFSBigBlockSize bigBlockSize, int entryCount)
{
    int _entries_per_block = bigBlockSize.getBATEntriesPerBlock();
    return (entryCount + _entries_per_block - 1) / _entries_per_block;
}

/**
    * Calculate how many XBATBlocks are needed to hold a specified
    * number of BAT entries.
    *
    * @param entryCount the number of entries
    *
    * @return the number of XBATBlocks needed
    */
int BATBlock::calculateXBATStorageRequirements(POIFSBigBlockSize bigBlockSize, int entryCount)
{
    int _entries_per_xbat_block = bigBlockSize.getXBATEntriesPerBlock();
    return (entryCount + _entries_per_xbat_block - 1)
            / _entries_per_xbat_block;
}
    
/**
    * Calculates the maximum size of a file which is addressable given the
    *  number of FAT (BAT) sectors specified. (We don't care if those BAT
    *  blocks come from the 109 in the header, or from header + XBATS, it
    *  won't affect the calculation)
    *  
    * The actual file size will be between [size of fatCount-1 blocks] and
    *   [size of fatCount blocks].
    *  For 512 byte block sizes, this means we may over-estimate by up to 65kb.
    *  For 4096 byte block sizes, this means we may over-estimate by up to 4mb
    */
long BATBlock::calculateMaximumSize(POIFSBigBlockSize bigBlockSize, int numBATs)
{
    long size = 1; // Header isn't FAT addressed
       
    // The header has up to 109 BATs, and extra ones are referenced
    //  from XBATs
    // However, all BATs can contain 128/1024 blocks
    size += (numBATs * bigBlockSize.getBATEntriesPerBlock());
       
    // So far we've been in sector counts, turn into bytes
    return size * bigBlockSize.getBigBlockSize();
}

long BATBlock::calculateMaximumSize(HeaderBlockPtr header)
{
    return calculateMaximumSize(header->getBigBlockSize(), header->getBATCount());
}

/**
    * Returns the BATBlock that handles the specified offset,
    *  and the relative index within it.
    * The List of BATBlocks must be in sequential order
    */
BATBlockAndIndexPtr BATBlock::getBATBlockAndIndex(int offset, HeaderBlockPtr header, BATBlockListPtr bats)
{
    POIFSBigBlockSize bigBlockSize = header->getBigBlockSize();
       
    int whichBAT = (int)floor(offset / bigBlockSize.getBATEntriesPerBlock());
    int index = offset % bigBlockSize.getBATEntriesPerBlock();
	return BATBlockAndIndexPtr(new BATBlockAndIndex(index, bats->GetAt(whichBAT)));
}
    
/**
    * Returns the BATBlock that handles the specified offset,
    *  and the relative index within it, for the mini stream.
    * The List of BATBlocks must be in sequential order
    */
BATBlockAndIndexPtr BATBlock::getSBATBlockAndIndex(int offset, HeaderBlockPtr header, BATBlockListPtr sbats)
{
    POIFSBigBlockSize bigBlockSize = header->getBigBlockSize();
       
    // SBATs are so much easier, as they're chained streams
    int whichSBAT = (int)floor(offset / bigBlockSize.getBATEntriesPerBlock());
    int index = offset % bigBlockSize.getBATEntriesPerBlock();
	return BATBlockAndIndexPtr(new BATBlockAndIndex(index, sbats->GetAt(whichSBAT)));
}
    
void BATBlock::setXBATChain(POIFSBigBlockSize bigBlockSize, int chainIndex)
{
    int _entries_per_xbat_block = bigBlockSize.getXBATEntriesPerBlock();
	(*_values)[_entries_per_xbat_block] = chainIndex;
}
    
/**
    * Does this BATBlock have any free sectors in it, or
    *  is it full?
    */
bool BATBlock::hasFreeSectors()
{
    return _has_free_sectors;
}
    
int BATBlock::getValueAt(int relativeOffset)
{
	if ((size_t)relativeOffset >= _values->length()) {
        throw toolkit::Exception(L"Unable to fetch offset");
    }
	return (*_values)[relativeOffset];
}

void BATBlock::setValueAt(int relativeOffset, int value)
{
    int oldValue = (*_values)[relativeOffset];
	(*_values)[relativeOffset] = value;
       
    // Do we need to re-compute the free?
    if(value == POIFSConstants::UNUSED_BLOCK) {
        _has_free_sectors = true;
        return;
    }
    if(oldValue == POIFSConstants::UNUSED_BLOCK) {
        recomputeFree();
    }
}
    
/**
    * Record where in the file we live
    */
void BATBlock::setOurBlockIndex(int index)
{
    this->ourBlockIndex = index;
}

/**
    * Retrieve where in the file we live 
    */
int BATBlock::getOurBlockIndex()
{
    return ourBlockIndex;
}

/**
    * Write the block's data to an OutputStream
    *
    * @param stream the OutputStream to which the stored data should
    *               be written
    *
    * @exception IOException on problems writing to the specified
    *            stream
    */
void BATBlock::writeData(BwProj::toolkit::IDataWriteStream* stream)
{
    // Save it out
	ByteArrayPtr data = serialize();
	stream->Write(data->get(), data->length());
}
    
void BATBlock::writeData(ByteArrayPtr block)
{
	ATLASSERT(FALSE);
    // Save it out
    //block->put(serialize());
}
    
ByteArrayPtr BATBlock::serialize()
{
    // Create the empty array
	ByteArrayPtr data (new ByteArray2(bigBlockSize.getBigBlockSize()));
       
    // Fill in the values
    int offset = 0;
    for(size_t i=0; i < _values->length(); i++)
	{
        LittleEndian::putInt(data, offset, (*_values)[i]);
        offset += LittleEndian::INT_SIZE;
    }
       
    // Done
    return data;
}