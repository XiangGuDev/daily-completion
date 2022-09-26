#include "stdafx.h"
#include "SmallBlockTableWriter.h"

/**
	* Creates new SmallBlockTable
	*
	* @param documents a List of POIFSDocument instances
	* @param root the Filesystem's root property
	*/

SmallBlockTableWriter::SmallBlockTableWriter(POIFSBigBlockSize bigBlockSize,
	POIFSDocumentListPtr documents, RootPropertyPtr root)
{
	_sbat.reset (new BlockAllocationTableWriter(bigBlockSize));
	_small_blocks.reset(new SmallDocumentBlockList());
	_root = root;
	Iterator<POIFSDocumentPtr> iter = documents->iterator();

	while (iter.hasNext())
	{
		POIFSDocumentPtr doc = iter.next();
		SmallDocumentBlockArrayPtr blocks = doc->getSmallBlocks();

		if (blocks->length() != 0)
		{
			doc->setStartBlock(_sbat->allocateSpace(blocks->length()));
			for (size_t j = 0; j < blocks->length(); j++)
			{
				SmallDocumentBlockPtr block = (*blocks)[j];
				_small_blocks->Add(block);
			}
		}
		else {
			doc->setStartBlock(POIFSConstants::END_OF_CHAIN);
		}
	}
	_sbat->simpleCreateBlocks();
	_root->setSize(_small_blocks->GetCount());
	_big_block_count = SmallDocumentBlock::fill(bigBlockSize, _small_blocks);
}

/**
	* Get the number of SBAT blocks
	*
	* @return number of SBAT big blocks
	*/

int SmallBlockTableWriter::getSBATBlockCount()
{
	return (_big_block_count + 15) / 16;
}

/**
	* Get the SBAT
	*
	* @return the Small Block Allocation Table
	*/

BlockAllocationTableWriterPtr SmallBlockTableWriter::getSBAT()
{
	return _sbat;
}

/* ********** START implementation of BATManaged ********** */

/**
	* Return the number of BigBlock's this instance uses
	*
	* @return count of BigBlock instances
	*/

int SmallBlockTableWriter::countBlocks()
{
	return _big_block_count;
}

/**
	* Set the start block for this instance
	*
	* @param start_block
	*/

void SmallBlockTableWriter::setStartBlock(int start_block)
{
	_root->setStartBlock(start_block);
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

void SmallBlockTableWriter::writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
{
	Iterator<SmallDocumentBlockPtr> iter = _small_blocks->iterator();

	while (iter.hasNext())
	{
		//((BlockWritable)iter.next())
		iter.next()->writeBlocks(stream);
	}
}