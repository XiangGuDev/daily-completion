#include "stdafx.h"
#include "LittleEndianConsts.h"
#include "POIFSConstants.h"


POIFSBigBlockSize::POIFSBigBlockSize(int bigBlockSize, short headerValue)
{
    this->bigBlockSize = bigBlockSize;
	this->headerValue = headerValue;
}

POIFSBigBlockSize::POIFSBigBlockSize(const POIFSBigBlockSize& size)
{
	this->bigBlockSize = size.bigBlockSize;
	this->headerValue = size.headerValue;
}

POIFSBigBlockSize& POIFSBigBlockSize::operator =(const POIFSBigBlockSize& src)
{
	this->bigBlockSize = src.bigBlockSize;
	this->headerValue = src.headerValue;
	return *this;
}
   
int POIFSBigBlockSize::getBigBlockSize() {
    return bigBlockSize;
}
   
/**
* Returns the value that gets written into the 
*  header.
* Is the power of two that corresponds to the
*  size of the block, eg 512 => 9
*/
short POIFSBigBlockSize::getHeaderValue() {
    return headerValue;
}
   
int POIFSBigBlockSize::getPropertiesPerBlock() {
    return bigBlockSize / POIFSConstants::PROPERTY_SIZE;
}
   
int POIFSBigBlockSize::getBATEntriesPerBlock() {
    return bigBlockSize / LittleEndianConsts::INT_SIZE;
}

int POIFSBigBlockSize::getXBATEntriesPerBlock() {
    return getBATEntriesPerBlock() - 1;
}

int POIFSBigBlockSize::getNextXBATChainOffset() {
    return getXBATEntriesPerBlock() * LittleEndianConsts::INT_SIZE;
}