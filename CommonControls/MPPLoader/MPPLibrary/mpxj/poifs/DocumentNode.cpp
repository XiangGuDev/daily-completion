#include "stdafx.h"
#include "DocumentNode.h"

/**
* create a DocumentNode. This method is not public by design; it
* is intended strictly for the internal use of this package
*
* @param property the DocumentProperty for this DocumentEntry
* @param parent the parent of this entry
*/

DocumentNode::DocumentNode(DocumentPropertyPtr prop, std::shared_ptr<DirectoryNode> parent)
:DocumentEntry(prop, parent)
{
	//ATLASSERT(prop->getDocument());
	_document = prop->getDocument();
}

/**
* get the POIFSDocument
*
* @return the internal POIFSDocument
*/

std::shared_ptr<POIFSDocument> DocumentNode::getDocument()
{
	return _document.lock();
}

/**
* get the zize of the document, in bytes
*
* @return size in bytes
*/

int DocumentNode::getSize()
{
	return getProperty()->getSize();
}

/**
* is this a DocumentEntry?
*
* @return true if the Entry is a DocumentEntry, else false
*/

bool DocumentNode::isDocumentEntry()
{
	return true;
}

/**
* extensions use this method to verify internal rules regarding
* deletion of the underlying store.
*
* @return true if it's ok to delete the underlying store, else
*         false
*/

bool DocumentNode::isDeleteOK()
{
	return true;
}