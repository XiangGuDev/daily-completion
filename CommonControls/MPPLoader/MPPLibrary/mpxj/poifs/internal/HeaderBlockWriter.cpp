#include "stdafx.h"
#include "HeaderBlockWriter.h"
#include <MPPLoader/Toolkit/include/MemoryDataStream2.h>

/**
    * Create a single instance initialized with default values
    */
HeaderBlockWriter::HeaderBlockWriter(POIFSBigBlockSize bigBlockSize)
{
    _header_block.reset (new HeaderBlock(bigBlockSize));
}

/**
    * Create a single instance initialized with the specified 
    *  existing values
    */
HeaderBlockWriter::HeaderBlockWriter(HeaderBlockPtr headerBlock)
{
    _header_block = headerBlock;
}

/**
    * Set BAT block parameters. Assumes that all BAT blocks are
    * contiguous. Will construct XBAT blocks if necessary and return
    * the array of newly constructed XBAT blocks.
    *
    * @param blockCount count of BAT blocks
    * @param startBlock index of first BAT block
    *
    * @return array of XBAT blocks; may be zero length, will not be
    *         null
    */

BATBlockArrayPtr HeaderBlockWriter::setBATBlocks(int blockCount, int startBlock)
{
    BATBlockArrayPtr rvalue;
    POIFSBigBlockSize bigBlockSize = _header_block->getBigBlockSize();

    _header_block->setBATCount(blockCount);

    // Set the BAT locations
    int limit  = min(blockCount, _max_bats_in_header);
    IntArray2Ptr bat_blocks (new IntArray2(limit));
    for (int j = 0; j < limit; j++) {
        (*bat_blocks)[j] = startBlock + j;
    }
    _header_block->setBATArray(bat_blocks);
        
    // Now do the XBATs
    if (blockCount > _max_bats_in_header)
    {
        int   excess_blocks      = blockCount - _max_bats_in_header;
		IntArray2Ptr excess_block_array (new IntArray2(excess_blocks));

        for (int j = 0; j < excess_blocks; j++)
        {
            (*excess_block_array)[ j ] = startBlock + j
                                        + _max_bats_in_header;
        }
        rvalue = BATBlock::createXBATBlocks(bigBlockSize, excess_block_array,
                                            startBlock + blockCount);
        _header_block->setXBATStart(startBlock + blockCount);
    }
    else
    {
		rvalue = BATBlock::createXBATBlocks(bigBlockSize, IntArray2Ptr(new IntArray2(0)), 0);
        _header_block->setXBATStart(POIFSConstants::END_OF_CHAIN);
    }
    _header_block->setXBATCount(rvalue->length());
    return rvalue;
}

/**
    * Set start of Property Table
    *
    * @param startBlock the index of the first block of the Property
    *                   Table
    */
void HeaderBlockWriter::setPropertyStart(int startBlock)
{
    _header_block->setPropertyStart(startBlock);
}

/**
    * Set start of small block allocation table
    *
    * @param startBlock the index of the first big block of the small
    *                   block allocation table
    */
void HeaderBlockWriter::setSBATStart(int startBlock)
{
    _header_block->setSBATStart(startBlock);
}

/**
    * Set count of SBAT blocks
    *
    * @param count the number of SBAT blocks
    */
void HeaderBlockWriter::setSBATBlockCount(int count)
{
    _header_block->setSBATBlockCount(count);
}

/**
    * For a given number of BAT blocks, calculate how many XBAT
    * blocks will be needed
    *
    * @param blockCount number of BAT blocks
    *
    * @return number of XBAT blocks needed
    */

int HeaderBlockWriter::calculateXBATStorageRequirements(POIFSBigBlockSize bigBlockSize, int blockCount)
{
    return (blockCount > _max_bats_in_header)
            ? BATBlock::calculateXBATStorageRequirements(
                    bigBlockSize, blockCount - _max_bats_in_header)
            : 0;
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
void HeaderBlockWriter::writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
{
    _header_block->writeData(stream);
}
    
/**
    * Write the block's data to an existing block
    *
    * @param block the ByteBuffer of the block to which the 
    *               stored data should be written
    *
    * @exception IOException on problems writing to the block
    */
void HeaderBlockWriter::writeBlock(ByteArrayPtr block)
{
    //ByteArray2OutputStream baos = new ByteArray2OutputStream(
    //        _header_block.getBigBlockSize().getBigBlockSize()
    //);

	BwProj::toolkit::CMemoryDataWriteStream2 baos;
    _header_block->writeData(&baos);

	ByteArrayPtr buffer(new ByteArray2(baos.GetLength()));
	baos.CopyBuffer(buffer->get());
      
	ATLASSERT(FALSE);
	// 以下不知道会否改变block的值，要调一下代码看传过来的是什么
	//block->put(buffer);
}