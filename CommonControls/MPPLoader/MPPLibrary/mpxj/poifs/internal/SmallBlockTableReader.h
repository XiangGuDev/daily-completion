#pragma once

#include "POIFSBigBlockSize.h"
#include "RootProperty.h"
#include "RawDataBlockList.h"

/**
 * This class implements reading the small document block list from an
 * existing file
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class SmallBlockTableReader
{
public:
    /**
     * fetch the small document block list from an existing file
     *
     * @param blockList the raw data from which the small block table
     *                  will be extracted
     * @param root the root property (which contains the start block
     *             and small block table size)
     * @param sbatStart the start block of the SBAT
     *
     * @return the small document block list
     *
     * @exception IOException
     */
	static BlockListPtr getSmallDocumentBlocks(POIFSBigBlockSize bigBlockSize,
		RawDataBlockListPtr blockList, RootPropertyPtr root, int sbatStart);
};