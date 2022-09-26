#include "stdafx.h"
#include "..\POIFSDocument.h"
#include "PropertyConstants.h"

/**
    * Constructor
    *
    * @param name POIFSDocument name
    * @param size POIFSDocument size
    */

DocumentProperty::DocumentProperty(CString name, int size)
:Property()
{
    //_document = null;
    setName(name);
    setSize(size);
    setNodeColor(_NODE_BLACK);   // simplification
    setPropertyType(PropertyConstants::DOCUMENT_TYPE);
}

/**
    * set the POIFSDocument
    *
    * @param doc the associated POIFSDocument
    */

void DocumentProperty::setDocument(POIFSDocumentPtr doc)
{
    _document = doc;
}

/**
    * get the POIFSDocument
    *
    * @return the associated document
    */

POIFSDocumentPtr DocumentProperty::getDocument()
{
    return _document.lock();
}

/* ********** START extension of Property ********** */

/**
    * give method more visibility
    *
    * @return true if this property should use small blocks
    */

bool DocumentProperty::shouldUseSmallBlocks()
{
    return __super::shouldUseSmallBlocks();
}

/**
    * @return true if a directory type Property
    */

bool DocumentProperty::isDirectory()
{
    return false;
}

/**
* Update the size of the property's data
*/
void DocumentProperty::updateSize(int size)
{
	setSize(size);
}

/**
* reader constructor
*
* @param index index number
* @param array byte data
* @param offset offset into byte data
*/

DocumentProperty::DocumentProperty(int index, ByteArrayPtr array1, int offset)
:Property(index, array1, offset)
{
	//_document = null;
}

/**
    * Perform whatever activities need to be performed prior to
    * writing
    */

void DocumentProperty::preWrite()
{

    // do nothing
}