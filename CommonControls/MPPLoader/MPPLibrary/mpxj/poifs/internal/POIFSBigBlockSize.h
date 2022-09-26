#pragma once

/**
 * <p>A class describing attributes of the Big Block Size</p>
 */
class POIFSBigBlockSize
{   
public:
	POIFSBigBlockSize(int bigBlockSize, short headerValue);

	POIFSBigBlockSize(const POIFSBigBlockSize& size);

	POIFSBigBlockSize& operator = (const POIFSBigBlockSize& src);
   
	int getBigBlockSize();
   
   /**
    * Returns the value that gets written into the 
    *  header.
    * Is the power of two that corresponds to the
    *  size of the block, eg 512 => 9
    */
	short getHeaderValue();
   
	int getPropertiesPerBlock();
   
	int getBATEntriesPerBlock();

	int getXBATEntriesPerBlock();

	int getNextXBATChainOffset();

private:
	int bigBlockSize;
	short headerValue;
};