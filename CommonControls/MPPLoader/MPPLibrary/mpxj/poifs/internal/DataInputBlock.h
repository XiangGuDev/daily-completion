#pragma once

/**
 * Wraps a <tt>byte</tt> array and provides simple data input access.
 * Internally, this class maintains a buffer read index, so that for the most part, primitive
 * data can be read in a data-input-stream-like manner.<p/>
 *
 * Note - the calling class should call the {@link #available()} method to detect end-of-buffer
 * and move to the next data block when the current is exhausted.
 * For optimisation reasons, no error handling is performed in this class.  Thus, mistakes in
 * calling code ran may raise ugly exceptions here, like {@link ArrayIndexOutOfBoundsException},
 * etc .<p/>
 *
 * The multi-byte primitive input methods ({@link #readUShortLE()}, {@link #readIntLE()} and
 * {@link #readLongLE()}) have corresponding 'spanning read' methods which (when required) perform
 * a read across the block boundary.  These spanning read methods take the previous
 * {@link DataInputBlock} as a parameter.
 * Reads of larger amounts of data (into <tt>byte</tt> array buffers) must be managed by the caller
 * since these could conceivably involve more than two blocks.
 *
 * @author Josh Micich
 */
class DataInputBlock
{
public:
	DataInputBlock(ByteArrayPtr data, int startOffset);

	int available();

	int readUByte();

	/**
	 * Reads a <tt>short</tt> which was encoded in <em>little endian</em> format.
	 */
	int readUShortLE();

	/**
	 * Reads a <tt>short</tt> which spans the end of <tt>prevBlock</tt> and the start of this block.
	 */
	int readUShortLE(std::shared_ptr<DataInputBlock> prevBlock);

	/**
	 * Reads an <tt>int</tt> which was encoded in <em>little endian</em> format.
	 */
	int readIntLE();

	/**
	 * Reads an <tt>int</tt> which spans the end of <tt>prevBlock</tt> and the start of this block.
	 */
	int readIntLE(std::shared_ptr<DataInputBlock> prevBlock, int prevBlockAvailable);

	/**
	 * Reads a <tt>long</tt> which was encoded in <em>little endian</em> format.
	 */
	__int64 readLongLE();

	/**
	 * Reads a <tt>long</tt> which spans the end of <tt>prevBlock</tt> and the start of this block.
	 */
	__int64 readLongLE(std::shared_ptr<DataInputBlock> prevBlock, int prevBlockAvailable);

	/**
	 * Reads <tt>len</tt> bytes from this block into the supplied buffer.
	 */
	void readFully(ByteArrayPtr buf, int off, int len);
	void readFully(BYTE* buf, int off, int len);

private:
	/**
	* Reads a small amount of data from across the boundary between two blocks.
	* The {@link #_readIndex} of this (the second) block is updated accordingly.
	* Note- this method (and other code) assumes that the second {@link DataInputBlock}
	* always is big enough to complete the read without being exhausted.
	*/
	void readSpanning(std::shared_ptr<DataInputBlock> prevBlock, int prevBlockAvailable, ByteArray2& buf);

private:
	/**
	* Possibly any size (usually 512K or 64K).  Assumed to be at least 8 bytes for all blocks
	* before the end of the stream.  The last block in the stream can be any size except zero.
	*/
	ByteArrayPtr _buf;
	int _readIndex;
	int _maxIndex;
};

typedef std::shared_ptr<DataInputBlock> DataInputBlockPtr;