#pragma once

#include "BigBlock.h"
#include "RawDataBlock.h"
#include "DataInputBlock.h"

class DocumentBlock;

typedef std::shared_ptr<DocumentBlock> DocumentBlockPtr;
typedef Array2<DocumentBlockPtr> DocumentBlockArray;
typedef std::shared_ptr<DocumentBlockArray> DocumentBlockArrayPtr;

typedef List<DocumentBlockPtr> DocumentBlockList;
typedef std::shared_ptr<DocumentBlockList> DocumentBlockListPtr;

/**
 * A block of document data.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class DocumentBlock : public BigBlock
{
public:
    /**
     * create a document block from a raw data block
     *
     * @param block the raw data block
     *
     * @exception IOException
     */

	DocumentBlock(RawDataBlockPtr block);

    /**
     * Create a single instance initialized with data.
     *
     * @param stream the BwProj::toolkit::IDataReadStream* delivering the data.
     *
     * @exception IOException
     */

	DocumentBlock(BwProj::toolkit::IDataReadStream* stream, POIFSBigBlockSize bigBlockSize);

    /**
     * Get the number of bytes read for this block
     *
     * @return bytes read into the block
     */

	int size();

    /**
     * Was this a partially read block?
     *
     * @return true if the block was only partially filled with data
     */

	bool partiallyRead();

    /**
     * @return the fill BYTE used
     */

	static BYTE getFillByte();

    /**
     * convert a single long array into an array of DocumentBlock
     * instances
     *
     * @param array the BYTE array to be converted
     * @param size the intended size of the array (which may be smaller)
     *
     * @return an array of DocumentBlock instances, filled from the
     *         input array
     */

	static DocumentBlockArrayPtr convert(POIFSBigBlockSize bigBlockSize, ByteArrayPtr array1, int size);

	static DataInputBlockPtr getDataInputBlock(DocumentBlockArrayPtr blocks, int offset);

private:
	/**
	* Create a single instance initialized with default values
	*/

	DocumentBlock(POIFSBigBlockSize bigBlockSize);

    /**
     * Write the block's data to an BwProj::toolkit::IDataWriteStream*
     *
     * @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */

	void writeData(BwProj::toolkit::IDataWriteStream* stream);

private:
	static const BYTE	_default_value = (BYTE)0xFF;
	ByteArrayPtr		_data;
	int					_bytes_read;
};

typedef std::shared_ptr<DocumentBlock> DocumentBlockPtr;
typedef Array2<DocumentBlockPtr> DocumentBlockArray;
typedef std::shared_ptr<DocumentBlockArray> DocumentBlockArrayPtr;