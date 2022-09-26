#pragma once

#include "EntryNode.h"

/**
 * This interface defines methods specific to Document objects
 * managed by a Filesystem instance.
 *
 * @author Marc Johnson (mjohnson at apache dot org)
 */

class DocumentEntry : public EntryNode
{
public:
	DocumentEntry(PropertyPtr prop, std::shared_ptr<DirectoryNode> parent)
		:EntryNode(prop, parent)
	{

	}
    /**
     * get the zize of the document, in bytes
     *
     * @return size in bytes
     */

     virtual int getSize() = 0;
};   // end public interface DocumentEntry

typedef std::shared_ptr<DocumentEntry> DocumentEntryPtr;