#pragma once
#include "BlockListImpl.h"

/**
 * A list of RawDataBlocks instances, and methods to manage the list
 *
 * @author Marc Johnson (mjohnson at apache dot org
 */

class RawDataBlockList : public BlockListImpl
{
public:
    /**
     * Constructor RawDataBlockList
     *
     * @param stream the InputStream from which the data will be read
     * @param bigBlockSize The big block size, either 512 bytes or 4096 bytes
     *
     * @exception IOException on I/O errors, and if an incomplete
     *            block is read
     */

	RawDataBlockList(BwProj::toolkit::IDataReadStream* stream, POIFSBigBlockSize bigBlockSize);
};   // end public class RawDataBlockList

typedef std::shared_ptr<RawDataBlockList> RawDataBlockListPtr;