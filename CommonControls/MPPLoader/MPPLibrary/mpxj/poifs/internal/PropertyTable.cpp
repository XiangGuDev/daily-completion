#include "stdafx.h"
#include "PropertyTable.h"
#include "PropertyBlock.h"
#include "PropertyFactory.h"

PropertyTable::PropertyTable(HeaderBlockPtr headerBlock)
:PropertyTableBase(headerBlock), _bigBigBlockSize(headerBlock->getBigBlockSize())
{
    //_bigBigBlockSize = headerBlock.getBigBlockSize();
    //_blocks = null;
}

/**
    * reading constructor (used when we've read in a file and we want
    * to extract the property table from it). Populates the
    * properties thoroughly
    *
    * @param headerBlock the header block of the file
    * @param blockList the list of blocks
    *
    * @exception IOException if anything goes wrong (which should be
    *            a result of the input being NFG)
    */
PropertyTable::PropertyTable(HeaderBlockPtr headerBlock, RawDataBlockListPtr blockList)
:PropertyTableBase(headerBlock, PropertyFactory::convertToProperties(
		blockList->fetchBlocks(headerBlock->getPropertyStart(), -1))),
		_bigBigBlockSize(headerBlock->getBigBlockSize())
{
    _bigBigBlockSize = headerBlock->getBigBlockSize();
    //_blocks      = null;
}

/**
    * Prepare to be written
    */
void PropertyTable::preWrite()
{
    //PropertyArrayPtr properties = _properties->toArray(new Property[_properties.size()]);

    // give each property its index
	for (size_t k = 0; k < _properties->GetCount(); k++)
    {
		_properties->GetAt(k)->setIndex(k);
    }

    // allocate the blocks for the property table
    _blocks = PropertyBlock::createPropertyBlockArray(_bigBigBlockSize, _properties);

    // prepare each property for writing
	for (size_t k = 0; k < _properties->GetCount(); k++)
    {
		_properties->GetAt(k)->preWrite();
    }
}
    
/**
    * Return the number of BigBlock's this instance uses
    *
    * @return count of BigBlock instances
    */
int PropertyTable::countBlocks()
{
    return (_blocks) ? 0 : _blocks->length();
}

/**
    * Write the storage to an OutputStream
    *
    * @param stream the OutputStream to which the stored data should
    *               be written
    *
    * @exception IOException on problems writing to the specified
    *            stream
    */
void PropertyTable::writeBlocks(BwProj::toolkit::IDataWriteStream* stream)
{
    if (_blocks)
    {
        for (size_t j = 0; j < _blocks->length(); j++)
        {
            (*_blocks)[ j ]->writeBlocks(stream);
        }
    }
}