#include "stdafx.h"
#include "RawDataBlock.h"
#include "POIFSConstants.h"

/**
    * Constructor RawDataBlock
    *
    * @param stream the BwProj::toolkit::IDataReadStream* from which the data will be read
    *
    * @exception IOtoolkit::Exception on I/O errors, and if an insufficient
    *            amount of data is read (the BwProj::toolkit::IDataReadStream* must
    *            be an exact multiple of the block size)
    */
RawDataBlock::RawDataBlock(BwProj::toolkit::IDataReadStream* stream)
:RawDataBlock(stream, POIFSConstants::SMALLER_BIG_BLOCK_SIZE)
{

}

/**
    * Constructor RawDataBlock
    *
    * @param stream the BwProj::toolkit::IDataReadStream* from which the data will be read
    * @param blockSize the size of the POIFS blocks, normally 512 bytes
    * {@link org.apache.poi.poifs.common.POIFSConstants#SMALLER_BIG_BLOCK_SIZE}
    *
    * @exception IOtoolkit::Exception on I/O errors, and if an insufficient
    *            amount of data is read (the BwProj::toolkit::IDataReadStream* must
    *            be an exact multiple of the block size)
    */
RawDataBlock::RawDataBlock(BwProj::toolkit::IDataReadStream* stream, int blockSize)
{
    _data.reset (new ByteArray2(blockSize));
    //int count = IOUtils.readFully(stream, _data);

	DWORD count = 0;
	stream->Read(_data->get(), blockSize, count);

	_hasData = (count > 0);

	if (count != blockSize)
	{
		_eof = true;
	}
	else
	{
		_eof = false;
	}
}

/**
    * When we read the data, did we hit end of file?
    *
    * @return true if the EoF was hit during this block, or
    *  false if not. If you have a dodgy short last block, then
    *  it's possible to both have data, and also hit EoF...
    */
bool RawDataBlock::eof()
{
    return _eof;
}
/**
    * Did we actually find any data to read? It's possible,
    *  in the event of a short last block, to both have hit
    *  the EoF, but also to have data
    */
bool RawDataBlock::hasData()
{
    return _hasData;
}
    
//CString RawDataBlock::toString() {
//    return "RawDataBlock of size " + _data.length; 
//}

/**
    * Get the data from the block
    *
    * @return the block's data as a byte array
    *
    * @exception IOtoolkit::Exception if there is no data
    */
ByteArrayPtr RawDataBlock::getData()
{
    if (! hasData())
    {
        throw toolkit::Exception(L"Cannot return empty data");
    }
    return _data;
}
    
/**
    * What's the big block size?
    */
int RawDataBlock::getBigBlockSize()
{
    return _data->length();
}