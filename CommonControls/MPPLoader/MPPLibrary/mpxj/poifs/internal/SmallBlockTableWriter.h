#pragma once

#include "POIFSBigBlockSize.h"
#include "BATManaged.h"
#include "RootProperty.h"
#include "BlockAllocationTableWriter.h"
#include "..\POIFSDocument.h"

/**
 * This class implements storage for writing the small blocks used by
 * small documents.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class SmallBlockTableWriter : public BlockWritable, public BATManaged
{
public:
	/**
	 * Creates new SmallBlockTable
	 *
	 * @param documents a List of POIFSDocument instances
	 * @param root the Filesystem's root property
	 */

	SmallBlockTableWriter(POIFSBigBlockSize bigBlockSize,
		POIFSDocumentListPtr documents, RootPropertyPtr root);

	/**
	 * Get the number of SBAT blocks
	 *
	 * @return number of SBAT big blocks
	 */

	int getSBATBlockCount();

	/**
	 * Get the SBAT
	 *
	 * @return the Small Block Allocation Table
	 */

	BlockAllocationTableWriterPtr getSBAT();

	/**
	 * Return the number of BigBlock's this instance uses
	 *
	 * @return count of BigBlock instances
	 */

	int countBlocks();

	/**
	 * Set the start block for this instance
	 *
	 * @param start_block
	 */

	void setStartBlock(int start_block);

	/**
	 * Write the storage to an OutputStream
	 *
	 * @param stream the OutputStream to which the stored data should
	 *               be written
	 *
	 * @exception IOException on problems writing to the specified
	 *            stream
	 */

	void writeBlocks(BwProj::toolkit::IDataWriteStream* stream);

private:
	BlockAllocationTableWriterPtr	_sbat;
	SmallDocumentBlockListPtr		_small_blocks;
	int								_big_block_count;
	RootPropertyPtr					_root;
};

typedef std::shared_ptr<SmallBlockTableWriter> SmallBlockTableWriterPtr;