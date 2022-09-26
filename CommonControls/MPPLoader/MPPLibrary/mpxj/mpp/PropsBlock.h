#pragma once

#include "Props.h"

/**
* This class represents the Props files found in Microsoft Project MPP9 files.
*/
class PropsBlock : public Props
{
public:
	/**
	* Constructor, reads the property data from an input stream.
	*
	* @param is input stream for reading props data
	*/
	PropsBlock(ByteArrayPtr is);

private:
	void populateMap(ByteArrayPtr data, int previousItemOffset, int previousItemKey, int itemOffset);
};