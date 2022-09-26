#include "stdafx.h"
#include "BlockAllocationTableWriter.h"
#include "HeaderBlockWriter.h"
#include "POIFSConstants.h"

/**
    * create a BlockAllocationTableWriter
    */
BlockAllocationTableWriter::BlockAllocationTableWriter(POIFSBigBlockSize bigBlockSize)
:_bigBlockSize(bigBlockSize)
{
    _start_block  = POIFSConstants::END_OF_CHAIN;
    //_entries      = new IntList();
    _blocks.reset (new BATBlockArray(0));
}

/**
    * Create the BATBlocks we need
    *
    * @return start block index of BAT blocks
    */
int BlockAllocationTableWriter::createBlocks()
{
    int xbat_blocks = 0;
    int bat_blocks  = 0;

    while (true)
    {
        int calculated_bat_blocks  =
            BATBlock::calculateStorageRequirements(_bigBlockSize,
                                                    bat_blocks
                                                    + xbat_blocks
                                                    + _entries.GetCount());
        int calculated_xbat_blocks =
            HeaderBlockWriter::calculateXBATStorageRequirements(
                    _bigBlockSize, calculated_bat_blocks);

        if ((bat_blocks == calculated_bat_blocks)
                && (xbat_blocks == calculated_xbat_blocks))
        {

            // stable ... we're OK
            break;
        }
        bat_blocks  = calculated_bat_blocks;
        xbat_blocks = calculated_xbat_blocks;
    }
    int startBlock = allocateSpace(bat_blocks);

    allocateSpace(xbat_blocks);
    simpleCreateBlocks();
    return startBlock;
}

/**
    * Allocate space for a block of indices
    *
    * @param blockCount the number of blocks to allocate space for
    *
    * @return the starting index of the blocks
    */
int BlockAllocationTableWriter::allocateSpace(int blockCount)
{
    int startBlock = _entries.GetCount();

    if (blockCount > 0)
    {
        int limit = blockCount - 1;
        int index = startBlock + 1;

        for (int k = 0; k < limit; k++)
        {
            _entries.Add(index++);
        }
        _entries.Add(POIFSConstants::END_OF_CHAIN);
    }
    return startBlock;
}

/**
    * get the starting block
    *
    * @return the starting block index
    */
int BlockAllocationTableWriter::getStartBlock()
{
    return _start_block;
}

/**
    * create the BATBlocks
    */
void BlockAllocationTableWriter::simpleCreateBlocks()
{
	IntArray2Ptr arr(new IntArray2(_entries.GetCount()));
	for (size_t i = 0; i < _entries.GetCount(); i++)
		(*arr)[i] = _entries.GetAt(i);

	_blocks = BATBlock::createBATBlocks(_bigBlockSize, arr);
}

/**
    * Write the storage to an OutputStream
    *
    * @param stream the OutputStream to which the stored data should
    *               be written
    *
    * @exception IOException on problems writing to the specified
    *            stream
    */
void BlockAllocationTableWriter::writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
{
    for (size_t j = 0; j < _blocks->length(); j++)
    {
        (*_blocks)[ j ]->writeBlocks(stream);
    }
}
    
/**
    * Write the BAT into its associated block
    */
void BlockAllocationTableWriter::writeBlock(BATBlockPtr bat, ByteArrayPtr block)
{
    bat->writeData(block);
}

/**
    * Return the number of BigBlock's this instance uses
    *
    * @return count of BigBlock instances
    */
int BlockAllocationTableWriter::countBlocks()
{
    return _blocks->length();
}

/**
    * Set the start block for this instance
    */
void BlockAllocationTableWriter::setStartBlock(int start_block)
{
    _start_block = start_block;
}