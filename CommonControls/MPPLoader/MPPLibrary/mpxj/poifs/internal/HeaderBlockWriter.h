#pragma once

#include "BlockWritable.h"
#include "HeaderBlockConstants.h"
#include "POIFSBigBlockSize.h"
#include "HeaderBlock.h"
#include "BATBlock.h"

/**
 * The block containing the archive header
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class HeaderBlockWriter : public HeaderBlockConstants, public BlockWritable
{
public:
    /**
     * Create a single instance initialized with default values
     */
   HeaderBlockWriter(POIFSBigBlockSize bigBlockSize);

    /**
     * Create a single instance initialized with the specified 
     *  existing values
     */
   HeaderBlockWriter(HeaderBlockPtr headerBlock);

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

   BATBlockArrayPtr setBATBlocks(int blockCount, int startBlock);

    /**
     * Set start of Property Table
     *
     * @param startBlock the index of the first block of the Property
     *                   Table
     */
   void setPropertyStart(int startBlock);

    /**
     * Set start of small block allocation table
     *
     * @param startBlock the index of the first big block of the small
     *                   block allocation table
     */
   void setSBATStart(int startBlock);

    /**
     * Set count of SBAT blocks
     *
     * @param count the number of SBAT blocks
     */
   void setSBATBlockCount(int count);

    /**
     * For a given number of BAT blocks, calculate how many XBAT
     * blocks will be needed
     *
     * @param blockCount number of BAT blocks
     *
     * @return number of XBAT blocks needed
     */

   static int calculateXBATStorageRequirements(POIFSBigBlockSize bigBlockSize, int blockCount);

    /* ********** START extension of BigBlock ********** */

    /**
     * Write the block's data to an OutputStream
     *
     * @param stream the OutputStream to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */
   void writeBlocks(BwProj::toolkit::IDataWriteStream* stream);
    
    /**
     * Write the block's data to an existing block
     *
     * @param block the ByteBuffer of the block to which the 
     *               stored data should be written
     *
     * @exception IOException on problems writing to the block
     */
   void writeBlock(ByteArrayPtr block);

private:
	HeaderBlockPtr _header_block;
};   // end class HeaderBlockWriter

typedef std::shared_ptr<HeaderBlockWriter> HeaderBlockWriterPtr;