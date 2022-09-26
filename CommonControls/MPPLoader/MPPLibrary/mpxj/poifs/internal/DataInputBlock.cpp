#include "stdafx.h"
#include "DataInputBlock.h"

DataInputBlock::DataInputBlock(ByteArrayPtr data, int startOffset)
{
	_buf = data;
	_readIndex = startOffset;
	_maxIndex = _buf->length();
}

int DataInputBlock::available()
{
	return _maxIndex-_readIndex;
}

int DataInputBlock::readUByte()
{
	return (*_buf)[_readIndex++] & 0xFF;
}

/**
	* Reads a <tt>short</tt> which was encoded in <em>little endian</em> format.
	*/
int DataInputBlock::readUShortLE()
{
	int i = _readIndex;
		
	int b0 = (*_buf)[i++] & 0xFF;
	int b1 = (*_buf)[i++] & 0xFF;
	_readIndex = i;
	return (b1 << 8) + (b0 << 0);
}

/**
	* Reads a <tt>short</tt> which spans the end of <tt>prevBlock</tt> and the start of this block.
	*/
int DataInputBlock::readUShortLE(DataInputBlockPtr prevBlock)
{
	// simple case - will always be one byte in each block
	int i = prevBlock->_buf->length() - 1;
		
	int b0 = (*prevBlock->_buf)[i++] & 0xFF;
	int b1 = (*_buf)[_readIndex++] & 0xFF;
	return (b1 << 8) + (b0 << 0);
}

/**
	* Reads an <tt>int</tt> which was encoded in <em>little endian</em> format.
	*/
int DataInputBlock::readIntLE()
{
	int i = _readIndex;
		
	int b0 = (*_buf)[i++] & 0xFF;
	int b1 = (*_buf)[i++] & 0xFF;
	int b2 = (*_buf)[i++] & 0xFF;
	int b3 = (*_buf)[i++] & 0xFF;
	_readIndex = i;
	return (b3 << 24) + (b2 << 16) + (b1 << 8) + (b0 << 0);
}

/**
	* Reads an <tt>int</tt> which spans the end of <tt>prevBlock</tt> and the start of this block.
	*/
int DataInputBlock::readIntLE(DataInputBlockPtr prevBlock, int prevBlockAvailable)
{
	ByteArray2 buf (4);
		
	readSpanning(prevBlock, prevBlockAvailable, buf);
	int b0 = buf[0] & 0xFF;
	int b1 = buf[1] & 0xFF;
	int b2 = buf[2] & 0xFF;
	int b3 = buf[3] & 0xFF;
	return (b3 << 24) + (b2 << 16) + (b1 << 8) + (b0 << 0);
}

/**
	* Reads a <tt>long</tt> which was encoded in <em>little endian</em> format.
	*/
__int64 DataInputBlock::readLongLE()
{
	int i = _readIndex;
		
	int b0 = (*_buf)[i++] & 0xFF;
	int b1 = (*_buf)[i++] & 0xFF;
	int b2 = (*_buf)[i++] & 0xFF;
	int b3 = (*_buf)[i++] & 0xFF;
	int b4 = (*_buf)[i++] & 0xFF;
	int b5 = (*_buf)[i++] & 0xFF;
	int b6 = (*_buf)[i++] & 0xFF;
	int b7 = (*_buf)[i++] & 0xFF;
	_readIndex = i;
	return (((__int64)b7 << 56) +
			((__int64)b6 << 48) +
			((__int64)b5 << 40) +
			((__int64)b4 << 32) +
			((__int64)b3 << 24) +
			(b2 << 16) +
			(b1 <<  8) +
			(b0 <<  0));
}

/**
	* Reads a <tt>long</tt> which spans the end of <tt>prevBlock</tt> and the start of this block.
	*/
__int64 DataInputBlock::readLongLE(DataInputBlockPtr prevBlock, int prevBlockAvailable)
{
	ByteArray2 buf(8);
		
	readSpanning(prevBlock, prevBlockAvailable, buf);
		
	int b0 = buf[0] & 0xFF;
	int b1 = buf[1] & 0xFF;
	int b2 = buf[2] & 0xFF;
	int b3 = buf[3] & 0xFF;
	int b4 = buf[4] & 0xFF;
	int b5 = buf[5] & 0xFF;
	int b6 = buf[6] & 0xFF;
	int b7 = buf[7] & 0xFF;
	return (((__int64)b7 << 56) +
			((__int64)b6 << 48) +
			((__int64)b5 << 40) +
			((__int64)b4 << 32) +
			((__int64)b3 << 24) +
			(b2 << 16) +
			(b1 <<  8) +
			(b0 <<  0));
}

/**
	* Reads <tt>len</tt> bytes from this block into the supplied buffer.
	*/
void DataInputBlock::readFully(ByteArrayPtr buf, int off, int len)
{
	//System.arraycopy(_buf, _readIndex, buf, off, len);
	readFully(buf->get(), off, len);
}

void DataInputBlock::readFully(BYTE* buf, int off, int len)
{
	memcpy(&buf[off], &(*_buf)[_readIndex], len);
	_readIndex += len;
}

/**
* Reads a small amount of data from across the boundary between two blocks.
* The {@link #_readIndex} of this (the second) block is updated accordingly.
* Note- this method (and other code) assumes that the second {@link DataInputBlock}
* always is big enough to complete the read without being exhausted.
*/
void DataInputBlock::readSpanning(DataInputBlockPtr prevBlock, int prevBlockAvailable, ByteArray2& buf)
{
	//System.arraycopy(prevBlock._buf, prevBlock._readIndex, buf, 0, prevBlockAvailable);

	memcpy(buf.get(), &(*prevBlock->_buf)[prevBlock->_readIndex], prevBlockAvailable);

	int secondReadLen = buf.length() - prevBlockAvailable;
	//System.arraycopy(_buf, 0, buf, prevBlockAvailable, secondReadLen);

	memcpy(&buf[prevBlockAvailable], _buf->get(), secondReadLen);

	_readIndex = secondReadLen;
}
