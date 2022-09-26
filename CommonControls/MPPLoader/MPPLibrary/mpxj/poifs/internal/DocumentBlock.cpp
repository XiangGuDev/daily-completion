#include "stdafx.h"
#include "DocumentBlock.h"
#include "POIFSConstants.h"

/**
    * create a document block from a raw data block
    *
    * @param block the raw data block
    *
    * @exception IOException
    */

DocumentBlock::DocumentBlock(RawDataBlockPtr block)
:BigBlock(block->getBigBlockSize() == POIFSConstants::SMALLER_BIG_BLOCK_SIZE ?
	POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS : POIFSConstants::LARGER_BIG_BLOCK_SIZE_DETAILS)
{
    _data       = block->getData();
    _bytes_read = _data->length();
}

/**
    * Create a single instance initialized with data.
    *
    * @param stream the BwProj::toolkit::IDataReadStream* delivering the data.
    *
    * @exception IOException
    */

DocumentBlock::DocumentBlock(BwProj::toolkit::IDataReadStream* stream, POIFSBigBlockSize bigBlockSize)
:DocumentBlock(bigBlockSize)
{
    //this(bigBlockSize);
    //int count = IOUtils.readFully(stream, _data);
	DWORD count = 0;
	stream->Read(_data->get(), _data->length(), count);

	_bytes_read = count;// (count == -1) ? 0 : count;
}

/**
    * Create a single instance initialized with default values
    */

DocumentBlock::DocumentBlock(POIFSBigBlockSize bigBlockSize)
:BigBlock(bigBlockSize)
{
    //super(bigBlockSize);
	_data.reset(new ByteArray2(bigBlockSize.getBigBlockSize()));
	memset(_data->get(), _default_value, _data->length());
    //Arrays.fill(_data, _default_value);
}

/**
    * Get the number of bytes read for this block
    *
    * @return bytes read into the block
    */

int DocumentBlock::size()
{
    return _bytes_read;
}

/**
    * Was this a partially read block?
    *
    * @return true if the block was only partially filled with data
    */

bool DocumentBlock::partiallyRead()
{
    return _bytes_read != bigBlockSize.getBigBlockSize();
}

/**
    * @return the fill BYTE used
    */

BYTE DocumentBlock::getFillByte()
{
    return _default_value;
}

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

DocumentBlockArrayPtr DocumentBlock::convert(POIFSBigBlockSize bigBlockSize, ByteArrayPtr array1, int size)
{
    DocumentBlockArrayPtr rval(new DocumentBlockArray((
		size + bigBlockSize.getBigBlockSize() - 1) / bigBlockSize.getBigBlockSize()));
	size_t offset = 0;

    for (size_t k = 0; k < rval->length(); k++)
    {
        (*rval)[k].reset (new DocumentBlock(bigBlockSize));
		ByteArrayPtr data = (*rval)[k]->_data;

		if (offset < array1->length())
        {
            size_t length = min(bigBlockSize.getBigBlockSize(), (int)(array1->length() - offset));

			//System.arraycopy(array1, offset, rval[k]._data, 0, length);

			memcpy_s(data->get(), length, &(*array1)[offset], length);

            if (length != bigBlockSize.getBigBlockSize())
            {
                //Arrays.fill(rval[ k ]._data, length, bigBlockSize.getBigBlockSize(), _default_value);
				memset(data->get() + length, _default_value, bigBlockSize.getBigBlockSize() - length);
            }
        }
        else
        {
            //Arrays.fill(rval[ k ]._data, _default_value);
			memset(data->get(), _default_value, data->length());
        }
        offset += bigBlockSize.getBigBlockSize();
    }
    return rval;
}

DataInputBlockPtr DocumentBlock::getDataInputBlock(DocumentBlockArrayPtr blocks, int offset)
{
	DataInputBlockPtr block;
    if(!blocks || blocks->length() == 0) {
		return block;
    }
        
    // Key things about the size of the block
    POIFSBigBlockSize bigBlockSize = (*blocks)[0]->bigBlockSize;
    int BLOCK_SHIFT = bigBlockSize.getHeaderValue();
    int BLOCK_SIZE = bigBlockSize.getBigBlockSize();
    int BLOCK_MASK = BLOCK_SIZE - 1;

    // Now do the offset lookup
    int firstBlockIndex = offset >> BLOCK_SHIFT;
    int firstBlockOffset= offset & BLOCK_MASK;
	block.reset (new DataInputBlock((*blocks)[firstBlockIndex]->_data, firstBlockOffset));

	return block;
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

void DocumentBlock::writeData(BwProj::toolkit::IDataWriteStream* stream)
{
    doWriteData(stream, _data);
}
