#include "stdafx.h"
#include "SmallBlockTableReader.h"
#include "BlockAllocationTableReader.h"
#include "SmallDocumentBlock.h"

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
BlockListPtr SmallBlockTableReader::getSmallDocumentBlocks(POIFSBigBlockSize bigBlockSize,
        RawDataBlockListPtr blockList, RootPropertyPtr root, int sbatStart)
{
    // Fetch the blocks which hold the Small Blocks stream
    ListManagedBlockArrayPtr smallBlockBlocks (blockList->fetchBlocks(root->getStartBlock(), -1));
        
    // Turn that into a list
	BlockListPtr list(new SmallDocumentBlockList2(
            SmallDocumentBlock::extract(bigBlockSize, smallBlockBlocks)));

    // Process
	BlockAllocationTableReaderPtr reader(new BlockAllocationTableReader(
		bigBlockSize, blockList->fetchBlocks(sbatStart, -1), list));
	list->setBAT(reader);
    return list;
}