#include "stdafx.h"
#include "POIFSDocument.h"
#include "internal\POIFSConstants.h"

DocumentBlockArrayPtr POIFSDocument::EMPTY_BIG_BLOCK_ARRAY;
SmallDocumentBlockArrayPtr POIFSDocument::EMPTY_SMALL_BLOCK_ARRAY;

/**
* Constructor from large blocks
*
* @param name the name of the POIFSDocument
* @param blocks the big blocks making up the POIFSDocument
* @param length the actual length of the POIFSDocument
*/
POIFSDocument::POIFSDocument(CString name, RawDataBlockArrayPtr blocks, int length)
:_bigBigBlockSize(POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS)
{
	_size = length;
	if (blocks->length() == 0)
	{
		_bigBigBlockSize = POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS;
	}
	else
	{
		_bigBigBlockSize = ((*blocks)[0]->getBigBlockSize() == POIFSConstants::SMALLER_BIG_BLOCK_SIZE ?
			POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS : POIFSConstants::LARGER_BIG_BLOCK_SIZE_DETAILS );
	}

	ListManagedBlockArrayPtr tempBlocks(new ListManagedBlockArray(blocks->length()));
	for (size_t i = 0; i < blocks->length(); i++)
		(*tempBlocks)[i] = (*blocks)[i];

	_big_store.reset(new BigBlockStore(_bigBigBlockSize, convertRawBlocksToBigBlocks(tempBlocks)));
	_property.reset (new DocumentProperty(name, _size));
	_small_store.reset (new SmallBlockStore(_bigBigBlockSize, EMPTY_SMALL_BLOCK_ARRAY));

	_property->setDocument(shared_from_this());
}

// TODO - awkward typing going on here
DocumentBlockArrayPtr POIFSDocument::convertRawBlocksToBigBlocks(ListManagedBlockArrayPtr blocks)
{
	DocumentBlockArrayPtr result (new DocumentBlockArray(blocks->length()));
	for (size_t i = 0; i < result->length(); i++)
	{
		(*result)[i].reset(new DocumentBlock(std::dynamic_pointer_cast<RawDataBlock>((*blocks)[i])));
	}
	return result;
}

SmallDocumentBlockArrayPtr POIFSDocument::convertRawBlocksToSmallBlocks(ListManagedBlockArrayPtr blocks)
{
	if (blocks->length() == 0)
		return SmallDocumentBlockArrayPtr();

	SmallDocumentBlockArrayPtr result(new SmallDocumentBlockArray(blocks->length()));
	for (size_t i = 0; i < blocks->length(); i++)
	{
		(*result)[i] = std::dynamic_pointer_cast<SmallDocumentBlock>((*blocks)[i]);
	}

	//if (blocks instanceof SmallDocumentBlock[]) {
	//	return (SmallDocumentBlock[]) blocks;
	//}
	//SmallDocumentBlock[] result = new SmallDocumentBlock[blocks.length];
	//System.arraycopy(blocks, 0, result, 0, blocks.length);
	return result;
}

/**
	* Constructor from small blocks
	*
	* @param name the name of the POIFSDocument
	* @param blocks the small blocks making up the POIFSDocument
	* @param length the actual length of the POIFSDocument
	*/
POIFSDocument::POIFSDocument(CString name, SmallDocumentBlockArrayPtr blocks, int length)
:_bigBigBlockSize(POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS)
{
	_size = length;

	if (blocks->length() == 0) {
		_bigBigBlockSize = POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS;
	}
	else {
		_bigBigBlockSize = (*blocks)[0]->getBigBlockSize();
	}

	_big_store.reset (new BigBlockStore(_bigBigBlockSize, EMPTY_BIG_BLOCK_ARRAY));
	_property.reset(new DocumentProperty(name, _size));
	_small_store.reset(new SmallBlockStore(_bigBigBlockSize, blocks));

	_property->setDocument(shared_from_this());
}

/**
	* Constructor from small blocks
	*
	* @param name the name of the POIFSDocument
	* @param blocks the small blocks making up the POIFSDocument
	* @param length the actual length of the POIFSDocument
	*/
POIFSDocument::POIFSDocument(CString name, POIFSBigBlockSize bigBlockSize, ListManagedBlockArrayPtr blocks, int length)
:_bigBigBlockSize(bigBlockSize)
{
	_size = length;
	//_bigBigBlockSize = bigBlockSize;
	_property.reset (new DocumentProperty(name, _size));
	//_property->setDocument(shared_from_this());

	if (Property::isSmall(_size))
	{
		_big_store.reset(new BigBlockStore(bigBlockSize, EMPTY_BIG_BLOCK_ARRAY));
		_small_store.reset(new SmallBlockStore(bigBlockSize, convertRawBlocksToSmallBlocks(blocks)));
	}
	else
	{
		_big_store.reset(new BigBlockStore(bigBlockSize, convertRawBlocksToBigBlocks(blocks)));
		_small_store.reset(new SmallBlockStore(bigBlockSize, EMPTY_SMALL_BLOCK_ARRAY));
	}
}

POIFSDocument::POIFSDocument(CString name, ListManagedBlockArrayPtr blocks, int length)
:POIFSDocument(name, POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS, blocks, length)
{
}

/**
	* Constructor
	*
	* @param name the name of the POIFSDocument
	* @param stream the BwProj::toolkit::IDataReadStream* we read data from
	*/
POIFSDocument::POIFSDocument(CString name, POIFSBigBlockSize bigBlockSize, BwProj::toolkit::IDataReadStream* stream)
:_bigBigBlockSize(bigBlockSize)
{
	DocumentBlockListPtr blocks(new DocumentBlockList());

	_size = 0;
	while (true)
	{
		DocumentBlockPtr block (new DocumentBlock(stream, bigBlockSize));
		int blockSize = block->size();

		if (blockSize > 0)
		{
			blocks->Add(block);
			_size += blockSize;
		}
		if (block->partiallyRead())
		{
			break;
		}
	}
	DocumentBlockArrayPtr bigBlocks(new DocumentBlockArray(blocks->GetCount()));
	BlockWritableArrayPtr bigBlocks2(new BlockWritableArray(blocks->GetCount()));
	//blocks.toArray(new DocumentBlock[blocks.size()]);
	for (size_t i = 0; i < blocks->GetCount(); i++)
	{
		(*bigBlocks)[i] = blocks->GetAt(i);
		(*bigBlocks2)[i] = blocks->GetAt(i);
	}

	_big_store.reset (new BigBlockStore(bigBlockSize, bigBlocks));
	_property.reset(new DocumentProperty(name, _size));
	_property->setDocument(shared_from_this());

	if (_property->shouldUseSmallBlocks())
	{
		_small_store.reset(new SmallBlockStore(bigBlockSize, SmallDocumentBlock::convert(bigBlockSize, bigBlocks2, _size)));
		_big_store.reset(new BigBlockStore(bigBlockSize, DocumentBlockArrayPtr()));
	}
	else {
		_small_store.reset (new SmallBlockStore(bigBlockSize, EMPTY_SMALL_BLOCK_ARRAY));
	}
}

POIFSDocument::POIFSDocument(CString name, BwProj::toolkit::IDataReadStream* stream)
:POIFSDocument(name, POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS, stream)
{
	
}

/**
	* Constructor
	*
	* @param name the name of the POIFSDocument
	* @param size the length of the POIFSDocument
	* @param path the path of the POIFSDocument
	* @param writer the writer who will eventually write the document contents
	*/
POIFSDocument::POIFSDocument(CString name, int size, POIFSBigBlockSize bigBlockSize, POIFSDocumentPathPtr path)
:_bigBigBlockSize(bigBlockSize)
{
	_size = size;
	_property.reset (new DocumentProperty(name, _size));
	_property->setDocument(shared_from_this());

	if (_property->shouldUseSmallBlocks())
	{
		_small_store.reset(new SmallBlockStore(_bigBigBlockSize, path, name, size));
		_big_store.reset(new BigBlockStore(_bigBigBlockSize, EMPTY_BIG_BLOCK_ARRAY));
	}
	else {
		_small_store.reset(new SmallBlockStore(_bigBigBlockSize, EMPTY_SMALL_BLOCK_ARRAY));
		_big_store.reset(new BigBlockStore(_bigBigBlockSize, path, name, size));
	}
}

POIFSDocument::POIFSDocument(CString name, int size, POIFSDocumentPathPtr path)
:POIFSDocument(name, size, POIFSConstants::SMALLER_BIG_BLOCK_SIZE_DETAILS, path)
{
}

/**
	* @return array of SmallDocumentBlocks; may be empty, cannot be null
	*/
SmallDocumentBlockArrayPtr POIFSDocument::getSmallBlocks()
{
	return _small_store->getBlocks();
}

/**
	* @return size of the document
	*/
int POIFSDocument::getSize()
{
	return _size;
}

/**
	* read data from the internal stores
	*
	* @param buffer the buffer to write to
	* @param offset the offset into our storage to read from
	* This method is currently (Oct 2008) only used by test code. Perhaps it can be deleted
	*/
void POIFSDocument::read(ByteArrayPtr buffer, int offset)
{
	int len = buffer->length();

	DataInputBlockPtr currentBlock = getDataInputBlock(offset);

	int blockAvailable = currentBlock->available();
	if (blockAvailable > len)
	{
		currentBlock->readFully(buffer, 0, len);
		return;
	}

	// else read big amount in chunks
	int remaining = len;
	int writePos = 0;
	int currentOffset = offset;
	while (remaining > 0)
	{
		bool blockIsExpiring = remaining >= blockAvailable;
		int reqSize;
		if (blockIsExpiring) {
			reqSize = blockAvailable;
		}
		else {
			reqSize = remaining;
		}
		currentBlock->readFully(buffer, writePos, reqSize);
		remaining -= reqSize;
		writePos += reqSize;
		currentOffset += reqSize;
		if (blockIsExpiring)
		{
			if (currentOffset == _size)
			{
				if (remaining > 0) {
					throw toolkit::Exception(L"reached end of document stream unexpectedly");
				}
				currentBlock.reset();
				break;
			}
			currentBlock = getDataInputBlock(currentOffset);
			blockAvailable = currentBlock->available();
		}
	}
}

/**
	* @return <code>null</code> if <tt>offset</tt> points to the end of the document stream
	*/
DataInputBlockPtr POIFSDocument::getDataInputBlock(int offset)
{
	if (offset >= _size) {
		if (offset > _size) {
			throw toolkit::Exception(L"Request for Offset doc size is");
		}
		return DataInputBlockPtr();
	}
	if (_property->shouldUseSmallBlocks()) {
		return SmallDocumentBlock::getDataInputBlock(_small_store->getBlocks(), offset);
	}
	return DocumentBlock::getDataInputBlock(_big_store->getBlocks(), offset);
}

/**
	* @return the instance's DocumentProperty
	*/

DocumentPropertyPtr POIFSDocument::getDocumentProperty()
{
	return _property;
}

/**
	* Write the storage to an BwProj::toolkit::IDataWriteStream*
	*
	* @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should be written
	*/
void POIFSDocument::writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
{
	_big_store->writeBlocks(stream);
}

/**
	* Return the number of BigBlock's this instance uses
	*
	* @return count of BigBlock instances
	*/
int POIFSDocument::countBlocks()
{
	return _big_store->countBlocks();
}

/**
	* Set the start block for this instance
	*
	* @param index index into the array of blocks making up the filesystem
	*/
void POIFSDocument::setStartBlock(int index)
{
	_property->setStartBlock(index);
}

/**
	* Constructor
	*
	* @param blocks blocks to construct the store from
	*/
POIFSDocument::SmallBlockStore::SmallBlockStore(POIFSBigBlockSize bigBlockSize, SmallDocumentBlockArrayPtr blocks)
:_bigBlockSize(bigBlockSize)
{
	//_bigBlockSize = bigBlockSize;
	if (blocks)
		_smallBlocks.reset (blocks->clone());

	//_smallBlocks.reset(new SmallDocumentBlockArray(blocks->length()));
	//for (size_t i = 0; i < blocks->length(); i++)
	//	(*_smallBlocks)[i] = (*blocks)[i];

	//this->_path = null;
	//this->_name = null;
	this->_size = -1;
	//this->_writer = null;
}

/**
	* Constructor for a small block store that will be written later
	*
	* @param path path of the document
	* @param name name of the document
	* @param size length of the document
	* @param writer the object that will eventually write the document
	*/
POIFSDocument::SmallBlockStore::SmallBlockStore(POIFSBigBlockSize bigBlockSize, POIFSDocumentPathPtr path, CString name, int size)
:_bigBlockSize(bigBlockSize)
{
	//_bigBlockSize = bigBlockSize;
	_smallBlocks.reset(new SmallDocumentBlockArray(0));
	_path = path;
	_name = name;
	_size = size;
	//this._writer = writer;
}

/**
	* @return <code>true</code> if this store is a valid source of data
	*/
bool POIFSDocument::SmallBlockStore::isValid()
{
	return _smallBlocks->length() > 0;// || _writer != null;
}

/**
	* @return the SmallDocumentBlocks
	*/
SmallDocumentBlockArrayPtr POIFSDocument::SmallBlockStore::getBlocks()
{
	//if (isValid())
	//{
	//	//ByteArray2OutputStream stream = new ByteArray2OutputStream(_size);
	//	//DocumentOutputStream dstream = new DocumentOutputStream(stream, _size);

	//	//_writer.processPOIFSWriterEvent(new POIFSWriterEvent(dstream, _path, _name, _size));

	//	ByteArrayPtr arr(new ByteArray2(_size));
	//	_smallBlocks = SmallDocumentBlock::convert(_bigBlockSize, arr, _size);
	//}
	return _smallBlocks;
}


/**
	* Constructor
	*
	* @param blocks the blocks making up the store
	*/
POIFSDocument::BigBlockStore::BigBlockStore(POIFSBigBlockSize bigBlockSize, DocumentBlockArrayPtr blocks)
:_bigBlockSize(bigBlockSize)
{
	if (blocks)
		bigBlocks.reset (blocks->clone());
	//_path = null;
	//_name = null;
	_size = -1;
}

/**
	* Constructor for a big block store that will be written later
	*
	* @param path path of the document
	* @param name name of the document
	* @param size length of the document
	* @param writer the object that will eventually write the document
	*/
POIFSDocument::BigBlockStore::BigBlockStore(POIFSBigBlockSize bigBlockSize, POIFSDocumentPathPtr path, CString name, int size)
:_bigBlockSize(bigBlockSize)
{
	bigBlocks.reset(new DocumentBlockArray(0));// = new DocumentBlock[0];
	_path = path;
	_name = name;
	_size = size;
}

/**
	* @return <code>true</code> if this store is a valid source of data
	*/
bool POIFSDocument::BigBlockStore::isValid()
{
	return bigBlocks->length() > 0;// || _writer != null;
}

/**
	* @return the DocumentBlocks
	*/
DocumentBlockArrayPtr POIFSDocument::BigBlockStore::getBlocks()
{
	//if (isValid())
	//{
	//	ATLASSERT(_size >= 0);
	//	ByteArrayPtr arr(new ByteArray2(_size));
	//	bigBlocks = DocumentBlock::convert(_bigBlockSize, arr, _size);
	//}
	return bigBlocks;
}

/**
	* write the blocks to a stream
	*
	* @param stream the stream to which the data is to be written
	*/
void POIFSDocument::BigBlockStore::writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
{
	if (!isValid())
		return;

	//if (_writer != null)
	//{
	//	DocumentOutputStream dstream = new DocumentOutputStream(stream, _size);

	//	_writer.processPOIFSWriterEvent(new POIFSWriterEvent(dstream, _path, _name, _size));
	//	dstream.writeFiller(countBlocks() * _bigBlockSize.getBigBlockSize(),
	//		DocumentBlock.getFillByte());
	//}
	//else 
	//{
		for (size_t k = 0; k < bigBlocks->length(); k++)
		{
			(*bigBlocks)[k]->writeBlocks(stream);
		}
	//}
}

/**
	* @return number of big blocks making up this document
	*/
int POIFSDocument::BigBlockStore::countBlocks()
{
	if (isValid())
		return 0;

	//if (_writer == null)
	//{
	//	return bigBlocks->length();
	//}
	return (_size + _bigBlockSize.getBigBlockSize() - 1) / _bigBlockSize.getBigBlockSize();
}