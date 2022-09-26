#pragma once

#include "ListManagedBlock.h"
#include "Property.h"

/**
 * Factory for turning an array of RawDataBlock instances containing
 * Property data into an array of proper Property objects.
 *
 * The array produced may be sparse, in that any portion of data that
 * should correspond to a Property, but which does not map to a proper
 * Property (i.e., a DirectoryProperty, DocumentProperty, or
 * RootProperty) will get mapped to a null Property in the array.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class PropertyFactory
{
public:
    /**
     * Convert raw data blocks to an array of Property's
     *
     * @param blocks to be converted
     *
     * @return the converted List of Property objects. May contain
     *         nulls, but will not be null
     *
     * @exception IOException if any of the blocks are empty
     */
	static PropertyListPtr convertToProperties(ListManagedBlockArrayPtr blocks);
    
	static void convertToProperties(ByteArrayPtr data, PropertyListPtr properties);
   
	// no need for an accessible constructor
private:
	PropertyFactory();
};   // end package scope class PropertyFactory