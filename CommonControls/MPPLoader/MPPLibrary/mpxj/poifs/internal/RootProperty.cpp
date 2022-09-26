#include "stdafx.h"
#include "RootProperty.h"
#include "POIFSConstants.h"
#include "PropertyConstants.h"
#include "SmallDocumentBlock.h"

#define NAME L"Root Entry"

RootProperty::RootProperty()
:DirectoryProperty(NAME)
{
    // overrides
    setNodeColor(_NODE_BLACK);
    setPropertyType(PropertyConstants::ROOT_TYPE);
    setStartBlock(POIFSConstants::END_OF_CHAIN);
}

/**
    * reader constructor
    *
    * @param index index number
    * @param array byte data
    * @param offset offset into byte data
    */
RootProperty::RootProperty(int index, ByteArrayPtr array1, int offset)
:DirectoryProperty(index, array1, offset)
{
}

/**
    * set size
    *
    * @param size size in terms of small blocks
    */
void RootProperty::setSize(int size)
{
    __super::setSize(SmallDocumentBlock::calcSize(size));
}

/**
    * Returns the fixed name "Root Entry", as the
    *  raw property doesn't have a real name set
    */
CString RootProperty::getName()
{
	return NAME;
}