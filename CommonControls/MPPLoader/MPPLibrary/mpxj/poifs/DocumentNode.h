#pragma once

#include "DirectoryEntry.h"
#include "internal\DocumentProperty.h"

class DirectoryNode;
class POIFSDocument;

/**
 * Simple implementation of DocumentEntry
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class DocumentNode : public DocumentEntry
{
public:
    /**
     * create a DocumentNode. This method is not public by design; it
     * is intended strictly for the internal use of this package
     *
     * @param property the DocumentProperty for this DocumentEntry
     * @param parent the parent of this entry
     */

	DocumentNode(DocumentPropertyPtr prop, std::shared_ptr<DirectoryNode> parent);

    /**
     * get the POIFSDocument
     *
     * @return the internal POIFSDocument
     */

	std::shared_ptr<POIFSDocument> getDocument();

    /**
     * get the zize of the document, in bytes
     *
     * @return size in bytes
     */

	int getSize();

    /**
     * is this a DocumentEntry?
     *
     * @return true if the Entry is a DocumentEntry, else false
     */

	virtual bool isDocumentEntry();

protected:
    /**
     * extensions use this method to verify internal rules regarding
     * deletion of the underlying store.
     *
     * @return true if it's ok to delete the underlying store, else
     *         false
     */

	virtual bool isDeleteOK();

private:
	// underlying POIFSDocument instance
	std::weak_ptr<POIFSDocument> _document;
};

typedef std::shared_ptr<DocumentNode> DocumentNodePtr;