#include "stdafx.h"
#include "PropertyFactory.h"
#include "POIFSConstants.h"
#include "PropertyConstants.h"
#include "DirectoryProperty.h"
#include "DocumentProperty.h"
#include "RootProperty.h"

// no need for an accessible constructor
PropertyFactory::PropertyFactory()
{
}

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
PropertyListPtr PropertyFactory::convertToProperties(ListManagedBlockArrayPtr blocks)
{
	PropertyListPtr properties(new PropertyList());

    for (size_t j = 0; j < blocks->length(); j++)
	{
		ByteArrayPtr data = (*blocks)[ j ]->getData();
        convertToProperties(data, properties);
    }
    return properties;
}
    
void PropertyFactory::convertToProperties(ByteArrayPtr data, PropertyListPtr properties)
{
    int property_count = data->length() / POIFSConstants::PROPERTY_SIZE;
    int offset         = 0;

    for (int k = 0; k < property_count; k++) {
        switch ((*data)[ offset + PropertyConstants::PROPERTY_TYPE_OFFSET ]) {
		case PropertyConstants::DIRECTORY_TYPE :
            properties->Add(
				PropertyPtr(new DirectoryProperty(properties->GetCount(), data, offset))
            );
            break;

		case PropertyConstants::DOCUMENT_TYPE :
            properties->Add(
				PropertyPtr(new DocumentProperty(properties->GetCount(), data, offset))
            );
            break;

		case PropertyConstants::ROOT_TYPE :
            properties->Add(
				PropertyPtr(new RootProperty(properties->GetCount(), data, offset))
            );
            break;

        default :
			properties->Add(PropertyPtr());
            break;
        }
          
        offset += POIFSConstants::PROPERTY_SIZE;
    }
}