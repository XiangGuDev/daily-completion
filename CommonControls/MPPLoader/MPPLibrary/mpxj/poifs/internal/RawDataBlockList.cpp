#include "stdafx.h"
#include "RawDataBlockList.h"
#include "RawDataBlock.h"

/**
    * Constructor RawDataBlockList
    *
    * @param stream the InputStream from which the data will be read
    * @param bigBlockSize The big block size, either 512 bytes or 4096 bytes
    *
    * @exception IOException on I/O errors, and if an incomplete
    *            block is read
    */

RawDataBlockList::RawDataBlockList(BwProj::toolkit::IDataReadStream* stream, POIFSBigBlockSize bigBlockSize)
{
	List<RawDataBlockPtr> blocks;// = new ArrayList<RawDataBlock>();

    while (true)
    {
        RawDataBlockPtr block (new RawDataBlock(stream, bigBlockSize.getBigBlockSize()));
            
        // If there was data, add the block to the list
        if(block->hasData()) {
            blocks.Add(block);
        }

        // If the stream is now at the End Of File, we're done
        if (block->eof()) {
            break;
        }
    }

	ListManagedBlockArrayPtr arr(new ListManagedBlockArray(blocks.GetCount()));
	for (size_t i = 0; i < blocks.GetCount(); i++)
	{
		(*arr)[i] = blocks.GetAt(i);
	}

	setBlocks(arr);// blocks.toArray(new RawDataBlock[blocks.size()]));
}