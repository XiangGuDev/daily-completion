#pragma once

#include "PropertyTableBase.h"
#include "BlockWritable.h"
#include "RawDataBlockList.h"

/**
 * This class embodies the Property Table for the {@link POIFSFileSystem}; 
 *  this is basically the directory for all of the documents in the
 * filesystem.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class PropertyTable : public PropertyTableBase, public BlockWritable
{
public:
	PropertyTable(HeaderBlockPtr headerBlock);

    /**
     * reading constructor (used when we've read in a file and we want
     * to extract the property table from it). Populates the
     * properties thoroughly
     *
     * @param headerBlock the header block of the file
     * @param blockList the list of blocks
     *
     * @exception IOException if anything goes wrong (which should be
     *            a result of the input being NFG)
     */
	PropertyTable(HeaderBlockPtr headerBlock, RawDataBlockListPtr blockList);

    /**
     * Prepare to be written
     */
	void preWrite();
    
    /**
     * Return the number of BigBlock's this instance uses
     *
     * @return count of BigBlock instances
     */
	int countBlocks();

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
	POIFSBigBlockSize _bigBigBlockSize;
	BlockWritableArrayPtr   _blocks;
};

typedef std::shared_ptr<PropertyTable> PropertyTablePtr;