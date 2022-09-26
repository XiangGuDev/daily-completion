#pragma once

#include "HeaderBlockConstants.h"

/**
 * The block containing the archive header
 */
class HeaderBlock : public HeaderBlockConstants
{
public:
	/**
	 * create a new HeaderBlockReader from an BwProj::toolkit::IDataReadStream*
	 *
	 * @param stream the source BwProj::toolkit::IDataReadStream*
	 *
	 * @exception IOException on errors or bad data
	 */
	HeaderBlock(BwProj::toolkit::IDataReadStream* stream);

	//HeaderBlock(ByteBuffer buffer);

	/**
	 * Create a single instance initialized with default values
	 */
	HeaderBlock(POIFSBigBlockSize bsize);

	/**
	 * get start of Property Table
	 *
	 * @return the index of the first block of the Property Table
	 */
	int getPropertyStart();

	/**
	 * Set start of Property Table
	 *
	 * @param startBlock the index of the first block of the Property Table
	 */
	void setPropertyStart(int startBlock);

	/**
	 * @return start of small block (MiniFAT) allocation table
	 */
	int getSBATStart();

	int getSBATCount();

	/**
	 * Set start of small block allocation table
	 *
	 * @param startBlock the index of the first big block of the small
	 *                   block allocation table
	 */
	void setSBATStart(int startBlock);

	/**
	 * Set count of SBAT blocks
	 *
	 * @param count the number of SBAT blocks
	 */
	void setSBATBlockCount(int count);

	/**
	 * @return number of BAT blocks
	 */
	int getBATCount();

	/**
	 * Sets the number of BAT blocks that are used.
	 * This is the number used in both the BAT and XBAT.
	 */
	void setBATCount(int count);

	/**
	 * Returns the offsets to the first (up to) 109
	 *  BAT sectors.
	 * Any additional BAT sectors are held in the XBAT (DIFAT)
	 *  sectors in a chain.
	 * @return BAT offset array
	 */
	IntArray2Ptr getBATArray();

	/**
	 * Sets the offsets of the first (up to) 109
	 *  BAT sectors.
	 */
	void setBATArray(IntArray2Ptr bat_array);

	/**
	 * @return XBAT (DIFAT) count
	 */
	int getXBATCount();

	/**
	 * Sets the number of XBAT (DIFAT) blocks used
	 */
	void setXBATCount(int count);

	/**
	 * @return XBAT (DIFAT) index
	 */
	int getXBATIndex();

	/**
	 * Sets the first XBAT (DIFAT) block location
	 */
	void setXBATStart(int startBlock);

	/**
	 * @return The Big Block size, normally 512 bytes, sometimes 4096 bytes
	 */
	POIFSBigBlockSize getBigBlockSize();

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

protected:

	HeaderBlock(ByteArrayPtr data);

	static ByteArrayPtr readFirst512(BwProj::toolkit::IDataReadStream* stream);

	//static CString longToHex(long value);

	//static IOException alertShortRead(int pRead, int expectedReadSize);

private:
	/**
	* What big block size the file uses. Most files
	*  use 512 bytes, but a few use 4096
	*/
	POIFSBigBlockSize bigBlockSize;

	/**
	* Number of big block allocation table blocks (int).
	* (Number of FAT Sectors in Microsoft parlance).
	*/
	int _bat_count;

	/**
	* Start of the property set block (int index of the property set
	* chain's first big block).
	*/
	int _property_start;

	/**
	* start of the small block allocation table (int index of small
	* block allocation table's first big block)
	*/
	int _sbat_start;

	/**
	* Number of small block allocation table blocks (int)
	* (Number of MiniFAT Sectors in Microsoft parlance)
	*/
	int _sbat_count;

	/**
	* Big block index for extension to the big block allocation table
	*/
	int _xbat_start;

	/**
	* Number of big block allocation table blocks (int)
	* (Number of DIFAT Sectors in Microsoft parlance)
	*/
	int _xbat_count;

	/**
	* The data. Only ever 512 bytes, because 4096 BYTE
	*  files use zeros for the extra header space.
	*/
	ByteArrayPtr _data;

	static const BYTE _default_value = (BYTE)0xFF;
};

typedef std::shared_ptr<HeaderBlock> HeaderBlockPtr;