#pragma once

#include "Property.h"
#include "BigBlock.h"

/**
 * A block of Property instances
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */
class PropertyBlock : public BigBlock
{
public:
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

	static BlockWritableArrayPtr createPropertyBlockArray(
		POIFSBigBlockSize bigBlockSize, PropertyListPtr properties);

    /**
     * Write the block's data to an OutputStream
     *
     * @param stream the OutputStream to which the stored data should
     *               be written
     *
     * @exception IOException on problems writing to the specified
     *            stream
     */

	void writeData(BwProj::toolkit::IDataWriteStream* stream);

private:
	/**
	* Create a single instance initialized with default values
	*
	* @param properties the properties to be inserted
	* @param offset the offset into the properties array
	*/

	PropertyBlock(POIFSBigBlockSize bigBlockSize, PropertyArrayPtr properties, int offset);

private:
	PropertyArrayPtr	_properties;
};   // end public class PropertyBlock