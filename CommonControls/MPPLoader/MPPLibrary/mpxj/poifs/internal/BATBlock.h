#pragma once

#include "BigBlock.h"
#include "HeaderBlock.h"

class BATBlock;

typedef std::shared_ptr<BATBlock> BATBlockPtr;

typedef Array2<BATBlockPtr> BATBlockArray;
typedef std::shared_ptr<BATBlockArray> BATBlockArrayPtr;

typedef List<BATBlockPtr> BATBlockList;
typedef std::shared_ptr<BATBlockList> BATBlockListPtr;

class BATBlockAndIndex
{
public:
	BATBlockAndIndex(int index, BATBlockPtr block) {
		this->index = index;
		this->block = block;
	}

	int getIndex() {
		return index;
	}

	BATBlockPtr getBlock() {
		return block;
	}

private:
	int index;
	BATBlockPtr block;
};

typedef std::shared_ptr<BATBlockAndIndex> BATBlockAndIndexPtr;

/**
 * A block of block allocation table entries. BATBlocks are created
 * only through a static factory method: createBATBlocks.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class BATBlock : public BigBlock
{
public:
    /**
     * Create a single BATBlock from the byte buffer, which must hold at least
     *  one big block of data to be read.
     */
	static BATBlockPtr createBATBlock(POIFSBigBlockSize bigBlockSize, ByteArrayPtr data);
    
    /**
     * Creates a single BATBlock, with all the values set to empty.
     */
	static BATBlockPtr createEmptyBATBlock(POIFSBigBlockSize bigBlockSize, bool isXBAT);

    /**
     * Create an array of BATBlocks from an array of int block
     * allocation table entries
     *
     * @param entries the array of int entries
     *
     * @return the newly created array of BATBlocks
     */
	static BATBlockArrayPtr createBATBlocks(POIFSBigBlockSize bigBlockSize, IntArray2Ptr entries);
    
    /**
     * Create an array of XBATBlocks from an array of int block
     * allocation table entries
     *
     * @param entries the array of int entries
     * @param startBlock the start block of the array of XBAT blocks
     *
     * @return the newly created array of BATBlocks
     */

	static BATBlockArrayPtr createXBATBlocks(POIFSBigBlockSize bigBlockSize,
		IntArray2Ptr entries, int startBlock);

    /**
     * Calculate how many BATBlocks are needed to hold a specified
     * number of BAT entries.
     *
     * @param entryCount the number of entries
     *
     * @return the number of BATBlocks needed
     */
	static int calculateStorageRequirements(POIFSBigBlockSize bigBlockSize, int entryCount);

    /**
     * Calculate how many XBATBlocks are needed to hold a specified
     * number of BAT entries.
     *
     * @param entryCount the number of entries
     *
     * @return the number of XBATBlocks needed
     */
	static int calculateXBATStorageRequirements(POIFSBigBlockSize bigBlockSize, int entryCount);
    
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
	static long calculateMaximumSize(POIFSBigBlockSize bigBlockSize, int numBATs);

	static long calculateMaximumSize(HeaderBlockPtr header);

    /**
     * Returns the BATBlock that handles the specified offset,
     *  and the relative index within it.
     * The List of BATBlocks must be in sequential order
     */
	static BATBlockAndIndexPtr getBATBlockAndIndex(int offset, HeaderBlockPtr header, BATBlockListPtr bats);
    
    /**
     * Returns the BATBlock that handles the specified offset,
     *  and the relative index within it, for the mini stream.
     * The List of BATBlocks must be in sequential order
     */
	static BATBlockAndIndexPtr getSBATBlockAndIndex(int offset, HeaderBlockPtr header, BATBlockListPtr sbats);
    
    /**
     * Does this BATBlock have any free sectors in it, or
     *  is it full?
     */
	bool hasFreeSectors();
    
	int getValueAt(int relativeOffset);

	void setValueAt(int relativeOffset, int value);
    
    /**
     * Record where in the file we live
     */
	void setOurBlockIndex(int index);

    /**
     * Retrieve where in the file we live 
     */
	int getOurBlockIndex();

   /**
     * Write the block's data to an OutputStream
     *
     * @param stream the OutputStream to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */
	void writeData(BwProj::toolkit::IDataWriteStream* stream);
    
	void writeData(ByteArrayPtr block);
    
private:
	void setXBATChain(POIFSBigBlockSize bigBlockSize, int chainIndex);

	/**
	* Create a single instance initialized with default values
	*/
	BATBlock(POIFSBigBlockSize bigBlockSize);

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

	BATBlock(POIFSBigBlockSize bigBlockSize, IntArray2Ptr entries, int start_index, int end_index);

	void recomputeFree();

	ByteArrayPtr serialize();

private:
	/**
	* For a regular fat block, these are 128 / 1024
	*  next sector values.
	* For a XFat (DIFat) block, these are 127 / 1023
	*  next sector values, then a chaining value.
	*/
	IntArray2Ptr _values;

	/**
	* Does this BATBlock have any free sectors in it?
	*/
	bool _has_free_sectors;

	/**
	* Where in the file are we?
	*/
	int ourBlockIndex;
};