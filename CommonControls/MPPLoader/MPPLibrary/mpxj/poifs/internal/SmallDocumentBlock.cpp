#include "stdafx.h"
#include "SmallDocumentBlock.h"
#include <MPPLoader/Toolkit/include/MemoryDataStream2.h>

SmallDocumentBlock::SmallDocumentBlock(POIFSBigBlockSize bigBlockSize, ByteArrayPtr data, int index)
:SmallDocumentBlock(bigBlockSize)
{
	//System.arraycopy(data, index * _block_size, _data, 0, _block_size);
	memcpy(_data->get(), &(*data)[index * _block_size], _block_size);
}

SmallDocumentBlock::SmallDocumentBlock(POIFSBigBlockSize bigBlockSize)
:_bigBlockSize(bigBlockSize)
{
	_blocks_per_big_block = getBlocksPerBigBlock(bigBlockSize);
	//_data = new BYTE[_block_size];
	_data.reset(new ByteArray2(_block_size));
}

int SmallDocumentBlock::getBlocksPerBigBlock(POIFSBigBlockSize bigBlockSize)
{
	return bigBlockSize.getBigBlockSize() / _block_size;
}

/**
	* convert a single long array into an array of SmallDocumentBlock
	* instances
	*
	* @param array the BYTE array to be converted
	* @param size the intended size of the array (which may be smaller)
	*
	* @return an array of SmallDocumentBlock instances, filled from
	*         the array
	*/
SmallDocumentBlockArrayPtr SmallDocumentBlock::convert(POIFSBigBlockSize bigBlockSize, ByteArrayPtr array, int size)
{
	SmallDocumentBlockArrayPtr rval (new SmallDocumentBlockArray((size + _block_size - 1) / _block_size));
	size_t offset = 0;

	for (size_t k = 0; k < rval->length(); k++)
	{
		(*rval)[k].reset (new SmallDocumentBlock(bigBlockSize));
		if (offset < array->length())
		{
			int length = min(_block_size, array->length() - offset);

			//System.arraycopy(array, offset, (*rval)[k]->_data, 0, length);
			memcpy((*rval)[k]->_data->get(), &(*array)[offset], length);

			if (length != _block_size)
			{
				//Arrays.fill(rval[k]._data, length, _block_size, _default_fill);
				memset((*rval)[k]->_data->get(), _default_fill, _block_size);
			}
		}
		else
		{
			//Arrays.fill(rval[k]._data, _default_fill);
			memset((*rval)[k]->_data->get(), _default_fill, (*rval)[k]->_data->length());
		}
		offset += _block_size;
	}
	return rval;
}

/**
	* fill out a List of SmallDocumentBlocks so that it fully occupies
	* a set of big blocks
	*
	* @param blocks the List to be filled out
	*
	* @return number of big blocks the list encompasses
	*/
int SmallDocumentBlock::fill(POIFSBigBlockSize bigBlockSize, SmallDocumentBlockListPtr blocks)
{
	int _blocks_per_big_block = getBlocksPerBigBlock(bigBlockSize);

	int count = blocks->GetCount();
	int big_block_count = (count + _blocks_per_big_block - 1)
		/ _blocks_per_big_block;
	int full_count = big_block_count * _blocks_per_big_block;

	for (; count < full_count; count++)
	{
		blocks->Add(makeEmptySmallDocumentBlock(bigBlockSize));
	}
	return big_block_count;
}

/**
	* Factory for creating SmallDocumentBlocks from DocumentBlocks
	*
	* @param store the original DocumentBlocks
	* @param size the total document size
	*
	* @return an array of new SmallDocumentBlocks instances
	*
	* @exception IOException on errors reading from the DocumentBlocks
	* @exception ArrayIndexOutOfBoundsException if, somehow, the store
	*            contains less data than size indicates
	*/
SmallDocumentBlockArrayPtr SmallDocumentBlock::convert(POIFSBigBlockSize bigBlockSize, BlockWritableArrayPtr store, int size)
{
	BwProj::toolkit::CMemoryDataWriteStream2 stream;

	for (size_t j = 0; j < store->length(); j++)
	{
		(*store)[j]->writeBlocks(&stream);
	}

	ByteArrayPtr data(new ByteArray2(stream.GetLength()));
	stream.CopyBuffer(data->get());

	SmallDocumentBlockArrayPtr rval (new SmallDocumentBlockArray(convertToBlockCount(size)));

	for (size_t index = 0; index < rval->length(); index++)
	{
		(*rval)[index].reset (new SmallDocumentBlock(bigBlockSize, data, index));
	}
	return rval;
}

/**
	* create a list of SmallDocumentBlock's from raw data
	*
	* @param blocks the raw data containing the SmallDocumentBlock
	*               data
	*
	* @return a List of SmallDocumentBlock's extracted from the input
	*/
SmallDocumentBlockListPtr SmallDocumentBlock::extract(POIFSBigBlockSize bigBlockSize, ListManagedBlockArrayPtr blocks)
{
	int _blocks_per_big_block = getBlocksPerBigBlock(bigBlockSize);

	SmallDocumentBlockListPtr sdbs(new SmallDocumentBlockList());

	for (size_t j = 0; j < blocks->length(); j++)
	{
		ByteArrayPtr data = (*blocks)[j]->getData();

		for (int k = 0; k < _blocks_per_big_block; k++)
		{
			sdbs->Add(SmallDocumentBlockPtr(new SmallDocumentBlock(bigBlockSize, data, k)));
		}
	}
	return sdbs;
}

DataInputBlockPtr SmallDocumentBlock::getDataInputBlock(SmallDocumentBlockArrayPtr blocks, int offset)
{
	int firstBlockIndex = offset >> BLOCK_SHIFT;
	int firstBlockOffset = offset & BLOCK_MASK;
	return DataInputBlockPtr (new DataInputBlock((*blocks)[firstBlockIndex]->_data, firstBlockOffset));
}

/**
	* Calculate the storage size of a set of SmallDocumentBlocks
	*
	* @param size number of SmallDocumentBlocks
	*
	* @return total size
	*/
int SmallDocumentBlock::calcSize(int size)
{
	return size * _block_size;
}

SmallDocumentBlockPtr SmallDocumentBlock::makeEmptySmallDocumentBlock(POIFSBigBlockSize bigBlockSize)
{
	SmallDocumentBlockPtr block (new SmallDocumentBlock(bigBlockSize));

	memset(block->_data->get(), block->_data->length(), _default_fill);

	return block;
}

int SmallDocumentBlock::convertToBlockCount(int size)
{
	return (size + _block_size - 1) / _block_size;
}

/**
	* Write the storage to an BwProj::toolkit::IDataWriteStream*
	*
	* @param stream the BwProj::toolkit::IDataWriteStream* to which the stored data should
	*               be written
	*
	* @exception IOException on problems writing to the specified
	*            stream
	*/
void SmallDocumentBlock::writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
{
	stream->Write(_data->get(), _data->length());
	//stream->write(_data);
}

/**
	* Get the data from the block
	*
	* @return the block's data as a BYTE array
	*
	* @exception IOException if there is no data
	*/
ByteArrayPtr SmallDocumentBlock::getData()
{
	return _data;
}

POIFSBigBlockSize SmallDocumentBlock::getBigBlockSize()
{
	return _bigBlockSize;
}

SmallDocumentBlockList2::SmallDocumentBlockList2(SmallDocumentBlockListPtr blocks)
{
	ListManagedBlockArrayPtr arr(new ListManagedBlockArray(blocks->GetCount()));
	for (size_t i = 0; i < arr->length(); i++)
	{
		(*arr)[i] = blocks->GetAt(i);
	}

	setBlocks(arr);
}