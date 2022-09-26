#include "stdafx.h"
#include "PropertyBlock.h"

/**
    * Create a single instance initialized with default values
    *
    * @param properties the properties to be inserted
    * @param offset the offset into the properties array
    */

PropertyBlock::PropertyBlock(POIFSBigBlockSize bigBlockSize, PropertyArrayPtr properties, int offset)
:BigBlock(bigBlockSize)
{
    _properties.reset(new PropertyArray(bigBlockSize.getPropertiesPerBlock())); 
    for (size_t j = 0; j < _properties->length(); j++)
    {
        (*_properties)[ j ] = (*properties)[ j + offset ];
    }
}

/**
    * Create an array of PropertyBlocks from an array of Property
    * instances, creating empty Property instances to make up any
    * shortfall
    *
    * @param properties the Property instances to be converted into
    *                   PropertyBlocks, in a java List
    *
    * @return the array of newly created PropertyBlock instances
    */

class EmptyProperty : public Property
{
public:
	void preWrite()
	{
	}

	bool isDirectory()
	{
		return false;
	}
};

BlockWritableArrayPtr PropertyBlock::createPropertyBlockArray(
	POIFSBigBlockSize bigBlockSize, PropertyListPtr properties)
{
    int _properties_per_block = bigBlockSize.getPropertiesPerBlock();
    int block_count = (properties->GetCount() + _properties_per_block - 1) / _properties_per_block;

    PropertyArrayPtr to_be_written (new PropertyArray(block_count * _properties_per_block));
	for (size_t i = 0; i < properties->GetCount(); i++)
		(*to_be_written)[i] = properties->GetAt(i);

    //System.arraycopy(properties.toArray(new Property[ 0 ]), 0, to_be_written, 0, properties.size());

	for (size_t j = properties->GetCount(); j < to_be_written->length(); j++)
    {
        // create an instance of an anonymous inner class that
        // extends Property
		(*to_be_written)[j].reset (new EmptyProperty());
    }

    BlockWritableArrayPtr rvalue (new BlockWritableArray(block_count));

    for (int j = 0; j < block_count; j++)
    {
        (*rvalue)[ j ].reset (new PropertyBlock(bigBlockSize, to_be_written,
                                        j * _properties_per_block));
    }
    return rvalue;
}

/**
    * Write the block's data to an OutputStream
    *
    * @param stream the OutputStream to which the stored data should
    *               be written
    *
    * @exception IOException on problems writing to the specified
    *            stream
    */

void PropertyBlock::writeData(BwProj::toolkit::IDataWriteStream* stream)
{
    int _properties_per_block = bigBlockSize.getPropertiesPerBlock();
    for (int j = 0; j < _properties_per_block; j++)
    {
        (*_properties)[ j ]->writeData(stream);
    }
}