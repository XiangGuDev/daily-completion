#pragma once

#include "POIFSBigBlockSize.h"
#include "BlockWritable.h"

/**
 * Abstract base class of all POIFS block storage classes. All
 * extensions of BigBlock should write 512 or 4096 bytes of data when
 * requested to write their data (as per their BigBlockSize).
 *
 * This class has package scope, as there is no reason at this time to
 * make the class public.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class BigBlock : public BlockWritable
{
public:
	/**
	* Write the storage to an BwProj::toolkit::IDataWriteStream*
	*
	* @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should
	*               be written
	*
	* @exception IOException on problems writing to the specified
	*            stream
	*/

	void writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
	{
		writeData(stream);
	}
    
	BigBlock(const POIFSBigBlockSize& blockSize)
		:bigBlockSize(blockSize)
	{
       //this->bigBlockSize = bigBlockSize;
    }

protected:

    /**
     * Default implementation of write for extending classes that
     * contain their data in a simple array of bytes.
     *
     * @param stream the BwProj::toolkit::IDataWriteStream* to which the data should be
     *               written.
     * @param data the byte array of to be written.
     *
     * @exception IOException on problems writing to the specified
     *            stream.
     */

    void doWriteData(BwProj::toolkit::IDataWriteStream* stream, ByteArrayPtr data)
    {
		stream->Write(data->get(), data->length());
        //stream.write(data);
    }

    /**
     * Write the block's data to an BwProj::toolkit::IDataWriteStream*
     *
     * @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */

    virtual void writeData(BwProj::toolkit::IDataWriteStream* stream) = 0;

	/**
	* Either 512 bytes ({@link POIFSConstants#SMALLER_BIG_BLOCK_SIZE})
	*  or 4096 bytes ({@link POIFSConstants#LARGER_BIG_BLOCK_SIZE})
	*/
	POIFSBigBlockSize bigBlockSize;
};   // end abstract class BigBlock