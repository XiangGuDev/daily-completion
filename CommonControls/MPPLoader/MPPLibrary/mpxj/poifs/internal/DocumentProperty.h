#pragma once

#include "Property.h"

class POIFSDocument;

/**
 * Trivial extension of Property for POIFSDocuments
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class DocumentProperty : public Property
{
	friend class PropertyFactory;
public:
    /**
     * Constructor
     *
     * @param name POIFSDocument name
     * @param size POIFSDocument size
     */

	DocumentProperty(CString name, int size);

    /**
     * set the POIFSDocument
     *
     * @param doc the associated POIFSDocument
     */

	void setDocument(std::shared_ptr<POIFSDocument> doc);

    /**
     * get the POIFSDocument
     *
     * @return the associated document
     */

	std::shared_ptr<POIFSDocument> getDocument();

    /* ********** START extension of Property ********** */

    /**
     * give method more visibility
     *
     * @return true if this property should use small blocks
     */

	bool shouldUseSmallBlocks();

    /**
     * @return true if a directory type Property
     */

	bool isDirectory();

	/**
	* Update the size of the property's data
	*/
	void updateSize(int size);

protected:
	/**
	* reader constructor
	*
	* @param index index number
	* @param array byte data
	* @param offset offset into byte data
	*/

	DocumentProperty(int index, ByteArrayPtr array1, int offset);

    /**
     * Perform whatever activities need to be performed prior to
     * writing
     */

	void preWrite();

private:
	// the POIFSDocument this property is associated with
	std::weak_ptr<POIFSDocument> _document;
};   // end class DocumentProperty

typedef std::shared_ptr<DocumentProperty> DocumentPropertyPtr;