#pragma once

#include "ListManagedBlock.h"

/**
 * A big block created from an BwProj::toolkit::IDataReadStream*, holding the raw data
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class RawDataBlock : public ListManagedBlock
{
public:
    /**
     * Constructor RawDataBlock
     *
     * @param stream the BwProj::toolkit::IDataReadStream* from which the data will be read
     *
     * @exception IOException on I/O errors, and if an insufficient
     *            amount of data is read (the BwProj::toolkit::IDataReadStream* must
     *            be an exact multiple of the block size)
     */
	RawDataBlock(BwProj::toolkit::IDataReadStream* stream);

    /**
     * Constructor RawDataBlock
     *
     * @param stream the BwProj::toolkit::IDataReadStream* from which the data will be read
     * @param blockSize the size of the POIFS blocks, normally 512 bytes
     * {@link org.apache.poi.poifs.common.POIFSConstants#SMALLER_BIG_BLOCK_SIZE}
     *
     * @exception IOException on I/O errors, and if an insufficient
     *            amount of data is read (the BwProj::toolkit::IDataReadStream* must
     *            be an exact multiple of the block size)
     */
	RawDataBlock(BwProj::toolkit::IDataReadStream* stream, int blockSize);

    /**
     * When we read the data, did we hit end of file?
     *
     * @return true if the EoF was hit during this block, or
     *  false if not. If you have a dodgy short last block, then
     *  it's possible to both have data, and also hit EoF...
     */
	bool eof();

    /**
     * Did we actually find any data to read? It's possible,
     *  in the event of a short last block, to both have hit
     *  the EoF, but also to have data
     */
	bool hasData();
    
	//CString toString();

    /**
     * Get the data from the block
     *
     * @return the block's data as a byte array
     *
     * @exception IOException if there is no data
     */
	ByteArrayPtr getData();
    
    /**
     * What's the big block size?
     */
	int getBigBlockSize();

private:
	ByteArrayPtr  _data;
	bool _eof;
	bool _hasData;
};

typedef std::shared_ptr<RawDataBlock> RawDataBlockPtr;
typedef List<RawDataBlockPtr> RawDataBlockPtrList;

typedef Array2<RawDataBlockPtr> RawDataBlockArray;
typedef std::shared_ptr<RawDataBlockArray> RawDataBlockArrayPtr;
