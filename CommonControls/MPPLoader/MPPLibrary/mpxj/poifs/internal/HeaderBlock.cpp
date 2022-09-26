#include "stdafx.h"
#include "HeaderBlock.h"
#include "LittleEndian.h"
#include "IntegerField.h"
#include "ShortField.h"
#include "LongField.h"

/**
* create a new HeaderBlockReader from an BwProj::toolkit::IDataReadStream*
*
* @param stream the source BwProj::toolkit::IDataReadStream*
*
* @exception IOtoolkit::Exception on errors or bad data
*/
HeaderBlock::HeaderBlock(BwProj::toolkit::IDataReadStream* stream)
:HeaderBlock(readFirst512(stream))
{
	// Grab the first 512 bytes
	// (For 4096 sized blocks, the remaining 3584 bytes are zero)
	// Then, process the contents
	//this(readFirst512(stream));

	// Fetch the rest of the block if needed
	if (bigBlockSize.getBigBlockSize() != 512)
	{
		int rest = bigBlockSize.getBigBlockSize() - 512;
		ByteArrayPtr tmp (new ByteArray2(rest));
		//IOUtils.readFully(stream, tmp);
		DWORD readBytes = 0;
		stream->Read(tmp->get(), 512, readBytes);
	}
}

//HeaderBlock(ByteBuffer buffer)
//{
//	this(IOUtils.toByteArray2(buffer, POIFSConstants::SMALLER_BIG_BLOCK_SIZE));
//}

HeaderBlock::HeaderBlock(ByteArrayPtr data)
:bigBlockSize(POIFSConstants::LARGER_BIG_BLOCK_SIZE_DETAILS)
{
	this->_data = data;

	// verify signature
	__int64 signature = LittleEndian::getLong(_data, _signature_offset);

	if (signature != _signature) {
		// Is it one of the usual suspects?
		const BYTE* OOXML_FILE_HEADER = POIFSConstants::OOXML_FILE_HEADER;
		if ((*_data)[0] == OOXML_FILE_HEADER[0] &&
			(*_data)[1] == OOXML_FILE_HEADER[1] &&
			(*_data)[2] == OOXML_FILE_HEADER[2] &&
			(*_data)[3] == OOXML_FILE_HEADER[3]) {
			throw toolkit::Exception (L"The supplied data appears to be in the Office 2007+ XML. "
				"You are calling the part of POI that deals with OLE2 Office Documents. "
				"You need to call a different part of POI to process this data (eg XSSF instead of HSSF)");
		}

		if ((*_data)[0] == 0x09 && (*_data)[1] == 0x00 && // sid=0x0009
			(*_data)[2] == 0x04 && (*_data)[3] == 0x00 && // size=0x0004
			(*_data)[4] == 0x00 && (*_data)[5] == 0x00 && // unused
			((*_data)[6] == 0x10 || (*_data)[6] == 0x20 || (*_data)[6] == 0x40) &&
			(*_data)[7] == 0x00) {
			// BIFF2 raw stream
			throw toolkit::Exception(L"The supplied data appears to be in BIFF2 format. "
				"HSSF only supports the BIFF8 format, try OldExcelExtractor");
		}
		if ((*_data)[0] == 0x09 && (*_data)[1] == 0x02 && // sid=0x0209
			(*_data)[2] == 0x06 && (*_data)[3] == 0x00 && // size=0x0006
			(*_data)[4] == 0x00 && (*_data)[5] == 0x00 && // unused
			((*_data)[6] == 0x10 || (*_data)[6] == 0x20 || (*_data)[6] == 0x40) &&
			(*_data)[7] == 0x00) {
			// BIFF3 raw stream
			throw toolkit::Exception(L"The supplied data appears to be in BIFF3 format. "
				"HSSF only supports the BIFF8 format, try OldExcelExtractor");
		}
		if ((*_data)[0] == 0x09 && (*_data)[1] == 0x04 && // sid=0x0409
			(*_data)[2] == 0x06 && (*_data)[3] == 0x00 && // size=0x0006
			(*_data)[4] == 0x00 && (*_data)[5] == 0x00) { // unused
			if ((((*_data)[6] == 0x10 || (*_data)[6] == 0x20 || (*_data)[6] == 0x40) &&
				(*_data)[7] == 0x00) ||
				((*_data)[6] == 0x00 && (*_data)[7] == 0x01)) {
				// BIFF4 raw stream
				throw toolkit::Exception(L"The supplied data appears to be in BIFF4 format. "
					"HSSF only supports the BIFF8 format, try OldExcelExtractor");
			}
		}

		// Give a generic error if the OLE2 signature isn't found
		throw toolkit::Exception(L"Invalid header signature; - Your file appears not to be a valid OLE2 document");
	}


	// Figure out our block size
	if ((*_data)[30] == 12) {
		this->bigBlockSize = POIFSConstants::LARGER_BIG_BLOCK_SIZE_DETAILS;
	}
	else if ((*_data)[30] == 9) {
		this->bigBlockSize = POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS;
	}
	else {
		throw toolkit::Exception(L"Unsupported blocksize . Expected 2^9 or 2^12.");
	}

	// Setup the fields to read and write the counts and starts
	_bat_count = IntegerField(_bat_count_offset, data).get();
	_property_start = IntegerField(_property_start_offset, _data).get();
	_sbat_start = IntegerField(_sbat_start_offset, _data).get();
	_sbat_count = IntegerField(_sbat_block_count_offset, _data).get();
	_xbat_start = IntegerField(_xbat_start_offset, _data).get();
	_xbat_count = IntegerField(_xbat_count_offset, _data).get();
}

/**
* Create a single instance initialized with default values
*/
HeaderBlock::HeaderBlock(POIFSBigBlockSize bsize)
:bigBlockSize(bsize)
{
	// Our data is always 512 big no matter what
	_data.reset (new ByteArray2(POIFSConstants::SMALLER_BIG_BLOCK_SIZE));
	memset(_data->get(), _default_value, _data->length());
	//Arrays.fill(_data, _default_value);

	// Set all the default values
	LongField(_signature_offset, _signature, _data);
	IntegerField(0x08, 0, _data);
	IntegerField(0x0c, 0, _data);
	IntegerField(0x10, 0, _data);
	IntegerField(0x14, 0, _data);
	ShortField(0x18, (short)0x3b, _data);
	ShortField(0x1a, (short)0x3, _data);
	ShortField(0x1c, (short)-2, _data);

	ShortField(0x1e, bsize.getHeaderValue(), _data);
	IntegerField(0x20, 0x6, _data);
	IntegerField(0x24, 0, _data);
	IntegerField(0x28, 0, _data);
	IntegerField(0x34, 0, _data);
	IntegerField(0x38, 0x1000, _data);

	// Initialise the variables
	_bat_count = 0;
	_sbat_count = 0;
	_xbat_count = 0;
	_property_start = POIFSConstants::END_OF_CHAIN;
	_sbat_start = POIFSConstants::END_OF_CHAIN;
	_xbat_start = POIFSConstants::END_OF_CHAIN;
}

ByteArrayPtr HeaderBlock::readFirst512(BwProj::toolkit::IDataReadStream* stream)
{
	// Grab the first 512 bytes
	// (For 4096 sized blocks, the remaining 3584 bytes are zero)
	ByteArrayPtr data(new ByteArray2(512));
	DWORD bsCount = 0;
	//IOUtils.readFully(stream, data);
	stream->Read(data->get(), data->length(), bsCount);

	if (bsCount != 512) {
		throw toolkit::Exception(L"file length error.");// alertShortRead(bsCount, 512);
	}
	return data;
}

//CString HeaderBlock::longToHex(long value)
//{
//	return new CString(HexDump.longToHex(value));
//}

//IOtoolkit::Exception alertShortRead(int pRead, int expectedReadSize)
//{
//	int read;
//	if (pRead < 0) {
//		//Can't have -1 bytes read in the error message!
//		read = 0;
//	}
//	else {
//		read = pRead;
//	}
//	CString type = " byte" + (read == 1 ? ("") : ("s"));
//
//	return new IOtoolkit::Exception("Unable to read entire header; "
//		+ read + type + " read; expected "
//		+ expectedReadSize + " bytes");
//}

/**
* get start of Property Table
*
* @return the index of the first block of the Property Table
*/
int HeaderBlock::getPropertyStart()
{
	return _property_start;
}
/**
* Set start of Property Table
*
* @param startBlock the index of the first block of the Property Table
*/
void HeaderBlock::setPropertyStart(int startBlock)
{
	_property_start = startBlock;
}

/**
* @return start of small block (MiniFAT) allocation table
*/
int HeaderBlock::getSBATStart()
{
	return _sbat_start;
}
int HeaderBlock::getSBATCount()
{
	return _sbat_count;
}

/**
* Set start of small block allocation table
*
* @param startBlock the index of the first big block of the small
*                   block allocation table
*/
void HeaderBlock::setSBATStart(int startBlock)
{
	_sbat_start = startBlock;
}
/**
* Set count of SBAT blocks
*
* @param count the number of SBAT blocks
*/
void HeaderBlock::setSBATBlockCount(int count)
{
	_sbat_count = count;
}

/**
* @return number of BAT blocks
*/
int HeaderBlock::getBATCount()
{
	return _bat_count;
}
/**
* Sets the number of BAT blocks that are used.
* This is the number used in both the BAT and XBAT.
*/
void HeaderBlock::setBATCount(int count)
{
	_bat_count = count;
}

/**
* Returns the offsets to the first (up to) 109
*  BAT sectors.
* Any additional BAT sectors are held in the XBAT (DIFAT)
*  sectors in a chain.
* @return BAT offset array
*/
IntArray2Ptr HeaderBlock::getBATArray()
{
	// Read them in
	IntArray2Ptr result (new IntArray2(min(_bat_count, _max_bats_in_header)));
	int offset = _bat_array_offset;
	for (size_t j = 0; j < result->length(); j++) {
		(*result)[j] = LittleEndian::getInt(_data, offset);
		offset += LittleEndianConsts::INT_SIZE;
	}
	return result;
}
/**
* Sets the offsets of the first (up to) 109
*  BAT sectors.
*/
void HeaderBlock::setBATArray(IntArray2Ptr bat_array)
{
	int count = min(bat_array->length(), _max_bats_in_header);
	int blank = _max_bats_in_header - count;

	int offset = _bat_array_offset;
	for (int i = 0; i < count; i++) {
		LittleEndian::putInt(_data, offset, (*bat_array)[i]);
		offset += LittleEndianConsts::INT_SIZE;
	}
	for (int i = 0; i < blank; i++) {
		LittleEndian::putInt(_data, offset, POIFSConstants::UNUSED_BLOCK);
		offset += LittleEndianConsts::INT_SIZE;
	}
}

/**
* @return XBAT (DIFAT) count
*/
int HeaderBlock::getXBATCount()
{
	return _xbat_count;
}
/**
* Sets the number of XBAT (DIFAT) blocks used
*/
void HeaderBlock::setXBATCount(int count)
{
	_xbat_count = count;
}

/**
* @return XBAT (DIFAT) index
*/
int HeaderBlock::getXBATIndex()
{
	return _xbat_start;
}
/**
* Sets the first XBAT (DIFAT) block location
*/
void HeaderBlock::setXBATStart(int startBlock)
{
	_xbat_start = startBlock;
}

/**
* @return The Big Block size, normally 512 bytes, sometimes 4096 bytes
*/
POIFSBigBlockSize HeaderBlock::getBigBlockSize()
{
	return bigBlockSize;
}

/**
* Write the block's data to an BwProj::toolkit::IDataWriteStream*
*
* @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should
*               be written
*
* @exception IOtoolkit::Exception on problems writing to the specified
*            stream
*/
void HeaderBlock::writeData(BwProj::toolkit::IDataWriteStream* stream)
{
	// Update the counts and start positions 
	new IntegerField(_bat_count_offset, _bat_count, _data);
	new IntegerField(_property_start_offset, _property_start, _data);
	new IntegerField(_sbat_start_offset, _sbat_start, _data);
	new IntegerField(_sbat_block_count_offset, _sbat_count, _data);
	new IntegerField(_xbat_start_offset, _xbat_start, _data);
	new IntegerField(_xbat_count_offset, _xbat_count, _data);

	// Write the main data out
	stream->Write(_data->get(), 512);	// 512

	// Now do the padding if needed
	for (int i = POIFSConstants::SMALLER_BIG_BLOCK_SIZE; i < bigBlockSize.getBigBlockSize(); i++)
	{
		BYTE zero = 0;
		stream->Write(&zero, 1);
	}
}